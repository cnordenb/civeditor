#include "forge.h"

#include <string.h>

#include "../core/check.h"
#include "../core/text.h"
#include "../dat/dat_parts.h"
#include "../civlist/civ_list.h"
#include "../json/json_reader.h"
#include "../mod/mod_places.h"
#include "../mod/mod_strings.h"
#include "../deflate/deflate.h"
#include "../pack/deflate_store.h"
#include "../techtree/tech_tree.h"

enum {
    BUFFER_CAPACITY = 104857600,
    CONTINUATION_MASK = 0xC0,
    CONTINUATION = 0x80,
    HELP_TO_TOOLTIP = MOD_HELP_OFFSET - MOD_TOOLTIP_OFFSET
};

typedef struct {
    const ForgeJob *job;
    ForgeResult *result;
    size_t plain_size;
    size_t packed_size;
    long name_sids[FORGE_CIV_LIMIT];
    char prefix[FORGE_PREFIX_CAPACITY];
} ForgeRun;

static unsigned char buffer_one[BUFFER_CAPACITY];
static unsigned char buffer_two[BUFFER_CAPACITY];
static DatMap map;
static DatEdit edit;
static JsonDocument civilizations;
static CivListOutput civ_list;
static ModCiv civs[FORGE_CIV_LIMIT];

static bool fail(ForgeResult *result, const char *problem)
{
    (void)text_copy(result->problem, sizeof result->problem, problem);
    return false;
}

static bool is_folder_letter(char letter)
{
    return text_is_between(letter, 'a', 'z') || text_is_digit(letter) || (letter == '_') || (letter == '-');
}

static char prefix_letter(unsigned char letter)
{
    if (!CHECK(letter != '\0') || !CHECK('a' > 'A')) {
        return '_';
    }
    if (text_is_between((char)letter, 'A', 'Z')) {
        return (char)(letter + ('a' - 'A'));
    }
    return is_folder_letter((char)letter) ? (char)letter : '_';
}

bool forge_prefix(const char *alias, char *prefix, size_t capacity)
{
    size_t from;
    size_t to = 0U;
    unsigned char letter;

    for (from = 0U; (alias[from] != '\0') && (to + 1U < capacity); ++from) {
        letter = (unsigned char)alias[from];
        if ((letter & CONTINUATION_MASK) != CONTINUATION) {
            prefix[to] = prefix_letter(letter);
            ++to;
        }
    }
    prefix[to] = '\0';
    return (to > 0U) || text_copy(prefix, capacity, "custom_civ");
}

static bool read_civ_list(const ForgeRun *run)
{
    const ForgeJob *job = run->job;

    return civ_list_read(&civilizations, job->game_folder);
}

static long name_sid_of(int slot)
{
    TextNumber sid;
    int civ;

    civ = civ_list_entry(&civilizations, slot);
    if (civ == JSON_NONE) {
        return -1L;
    }
    sid = json_number(&civilizations, json_member(&civilizations, civ, "name_string_id"));
    return sid.valid ? sid.value : -1L;
}

static bool find_name_sids(ForgeRun *run)
{
    const ForgeJob *job = run->job;
    bool found;
    int civ;

    found = read_civ_list(run);
    for (civ = 0; found && (civ < job->design_count); ++civ) {
        run->name_sids[civ] = name_sid_of(job->designs[civ]->choices[CHOICE_REPLACE]);
        found = run->name_sids[civ] >= 0L;
    }
    return found;
}

static bool chain_civ_list(void)
{
    if (!CHECK(civ_list.length < sizeof civilizations.text)) {
        return false;
    }
    (void)memcpy(civilizations.text, civ_list.text, civ_list.length);
    civilizations.length = (int)civ_list.length;
    return json_parse(&civilizations);
}

static bool unpack_game(ForgeRun *run)
{
    wchar_t path[FILE_PATH_CAPACITY];
    ByteBuffer plain = { buffer_one, sizeof buffer_one, 0U };
    ByteSpan packed = { buffer_two, 0U };
    const ForgeJob *job;
    FileRead read;

    job = run->job;
    if (!mod_game_path(job->game_folder, "resources\\_common\\dat\\empires2_x2_p1.dat", path)) {
        return fail(run->result, "The game's data file could not be found.");
    }
    read = file_read_all(path, (char *)buffer_two, sizeof buffer_two);
    packed.size = read.length;
    if (!read.succeeded || !inflate_raw(packed, &plain)) {
        return fail(run->result, "The game's data file could not be read or unpacked.");
    }
    if (!dat_map_read(buffer_one, plain.size, &map)) {
        return fail(run->result, "The game's data file has a layout this version does not know.");
    }
    run->plain_size = plain.size;
    return true;
}

static long unit_field(const RecipeInput *input, size_t at)
{
    DatCursor cursor;

    if (!dat_cursor_start(&cursor, input->dat, at)) {
        return CIV_LIST_KEEP;
    }
    return dat_cursor_i32(&cursor);
}

static void keep_game_texts(const RecipeInput *input, size_t unit_at, CivListEntry *entry)
{
    long help;

    help = unit_field(input, unit_at + DAT_UNIT_HELP_AT);
    entry->unit_name_sid = unit_field(input, unit_at + DAT_UNIT_NAME_AT);
    entry->unit_description_sid = (help < MOD_HELP_OFFSET) ? CIV_LIST_KEEP : help - HELP_TO_TOOLTIP;
}

static void read_own_texts(const RecipeInput *input, CivListEntry *entry)
{
    const DatMap *slots;
    DatCursor cursor;
    size_t at = 0U;

    if (entry->unit < 0) {
        return;
    }
    slots = input->map;
    if (!dat_find_unit(input->dat, slots->civ_at[input->slot], entry->unit, &at) ||
        !dat_cursor_start(&cursor, input->dat, at + DAT_UNIT_ICON_AT)) {
        return;
    }
    entry->icon = dat_cursor_i16(&cursor);
    if (!entry->unit_named) {
        keep_game_texts(input, at, entry);
    }
}

static bool make_civ_list(const RecipeInput *input, const RecipeReport *report)
{
    RecipeUnitIds ids = recipe_unique_unit_ids(input);
    CivListEntry entry;
    long sid;

    sid = mod_unit_has_own_texts(input->design) ? catalog_string_id(input->catalog, input->slot, CIV_STRING_UNIT)
                                                : CIV_LIST_KEEP;
    entry.slot = input->slot;
    entry.unit = ids.unit;
    entry.elite_unit = ids.elite_unit;
    entry.elite_tech = ids.elite_tech;
    entry.icon = CIV_LIST_KEEP;
    entry.unit_named = sid >= 0L;
    entry.unit_name_sid = entry.unit_named ? sid : CIV_LIST_KEEP;
    entry.unit_description_sid = entry.unit_named ? sid + MOD_TOOLTIP_OFFSET : CIV_LIST_KEEP;
    entry.castle_tech = report->unique_tech_ids[TECH_TIER_CASTLE];
    entry.imperial_tech = report->unique_tech_ids[TECH_TIER_IMPERIAL];
    entry.description_sid = mod_description_sid(input->slot);
    read_own_texts(input, &entry);
    return civ_list_write(&civilizations, &entry, &civ_list) && chain_civ_list();
}

static bool write_civs(ForgeRun *run, DatOutput *output)
{
    const ForgeJob *job = run->job;
    ForgeResult *result = run->result;
    DatBytes original = { buffer_one, 0U };
    RecipeInput input;
    RecipeReport *report;
    bool fine;
    int civ;

    original.size = run->plain_size;
    input.catalog = job->catalog;
    input.dat = &original;
    input.map = &map;
    fine = dat_edit_start(&edit, &map);
    for (civ = 0; fine && (civ < job->design_count); ++civ) {
        input.design = job->designs[civ];
        input.slot = input.design->choices[CHOICE_REPLACE];
        report = &result->recipes[civ];
        fine = recipe_apply(&input, &edit, report) && make_civ_list(&input, report);
    }
    return fine && dat_edit_write(&edit, &original, output);
}

static bool edit_game(ForgeRun *run)
{
    DatOutput output = { buffer_two, sizeof buffer_two, 0U };
    ByteBuffer packed = { buffer_one, sizeof buffer_one, 0U };
    ByteSpan plain = { buffer_two, 0U };

    if (!write_civs(run, &output)) {
        return fail(run->result, "The civs could not be written into the game data.");
    }
    plain.size = output.length;
    if (!deflate_pack(plain, &packed) && !deflate_store(plain, &packed)) {
        return fail(run->result, "The mod's data file could not be packed.");
    }
    run->packed_size = packed.size;
    return true;
}

static bool mod_tree_file(const ForgeJob *job, const char *folder, const char *name, wchar_t *path)
{
    char relative[FILE_PATH_CAPACITY];
    wchar_t wide[FILE_PATH_CAPACITY];

    return mod_join(relative, folder, "\\resources\\_common\\dat\\CivTechTrees") &&
           text_widen(name, wide, FILE_PATH_CAPACITY) && mod_file_path(job->mods_folder, relative, wide, path);
}

static bool write_tech_tree(const ForgeJob *job, const CivDesign *design, const char *folder)
{
    char name[FILE_PATH_CAPACITY];
    char relative[FILE_PATH_CAPACITY];
    wchar_t game_file[FILE_PATH_CAPACITY];
    wchar_t mod_file[FILE_PATH_CAPACITY];
    TechTreeJob tree;

    tree.design = design;
    tree.catalog = job->catalog;
    if (!tech_tree_file_name(&civilizations, tree.design->choices[CHOICE_REPLACE], name) ||
        !mod_join(relative, "resources\\_common\\dat\\CivTechTrees\\", name)) {
        return false;
    }
    return mod_game_path(job->game_folder, relative, game_file) && mod_tree_file(job, folder, name, mod_file) &&
           tech_tree_patch_file(&tree, game_file, mod_file);
}

static bool write_tech_trees(const ForgeJob *job, const char *folder)
{
    bool fine = true;
    int civ;

    for (civ = 0; fine && (civ < job->design_count); ++civ) {
        fine = write_tech_tree(job, job->designs[civ], folder);
    }
    return fine;
}

static bool write_dat_files(const ForgeRun *run, const char *folder)
{
    char relative[FILE_PATH_CAPACITY];
    wchar_t path[FILE_PATH_CAPACITY];
    const ForgeJob *job;

    job = run->job;
    if (!mod_join(relative, folder, "\\resources\\_common\\dat") ||
        !mod_file_path(job->mods_folder, relative, L"empires2_x2_p1.dat", path) ||
        !file_write_all(path, (const char *)buffer_one, run->packed_size)) {
        return false;
    }
    return mod_file_path(job->mods_folder, relative, L"civilizations.json", path) &&
           file_write_all(path, civ_list.text, civ_list.length);
}

static bool write_data_mod(ForgeRun *run)
{
    const ForgeJob *job;
    ForgeResult *result;
    char folder[FILE_PATH_CAPACITY];

    job = run->job;
    result = run->result;
    if (!mod_join(folder, run->prefix, "-data") || !mod_write_info(job->mods_folder, folder, job->mod_name) ||
        !write_dat_files(run, folder)) {
        return fail(result, "The data mod could not be written to the mods folder.");
    }
    if (!write_tech_trees(job, folder)) {
        return fail(result, "The tech tree of a civ could not be written into the data mod.");
    }
    return mod_folder_path(job->mods_folder, folder, result->data_mod);
}

static bool write_ui_mod(ForgeRun *run)
{
    const ForgeJob *job;
    ForgeResult *result;
    ModUi ui;
    char folder[FILE_PATH_CAPACITY];
    int civ;

    job = run->job;
    result = run->result;
    for (civ = 0; civ < job->design_count; ++civ) {
        civs[civ].design = job->designs[civ];
        civs[civ].catalog = job->catalog;
        civs[civ].slot = job->designs[civ]->choices[CHOICE_REPLACE];
        civs[civ].name_sid = run->name_sids[civ];
        civs[civ].hero_skipped = result->recipes[civ].hero_skipped;
    }
    ui = (ModUi){ civs, job->design_count, job->game_folder, job->mods_folder, run->prefix, job->mod_name };
    if (!mod_ui_write(&ui) || !mod_join(folder, run->prefix, "-ui")) {
        return fail(result, "The UI mod could not be written to the mods folder.");
    }
    return mod_folder_path(job->mods_folder, folder, result->ui_mod);
}

static bool build_mods(ForgeRun *run)
{
    return unpack_game(run) && edit_game(run) && write_data_mod(run) && write_ui_mod(run);
}

bool forge_civs_are_finished(const ForgeJob *job, ForgeResult *result)
{
    int civ;

    if (job->design_count < 1) {
        return fail(result, "No civ is saved in the mod yet: Save civ in the review step puts the civ shown in it.");
    }
    for (civ = 0; civ < job->design_count; ++civ) {
        if (!civ_design_is_complete(job->designs[civ])) {
            return fail(result, "A civ saved in the mod is not finished yet; its review step lists what is still "
                                "needed.");
        }
    }
    return true;
}

bool forge_build(const ForgeJob *job, ForgeResult *result)
{
    static ForgeRun run;

    result->problem[0] = '\0';
    result->data_mod[0] = L'\0';
    result->ui_mod[0] = L'\0';
    run.job = job;
    run.result = result;
    if (!forge_civs_are_finished(job, result)) {
        return false;
    }
    if (!forge_prefix(job->mod_name, run.prefix, sizeof run.prefix)) {
        return fail(result, "The mod's name cannot be made into a folder name.");
    }
    if (!find_name_sids(&run)) {
        return fail(result, "The game's list of civs could not be read.");
    }
    return build_mods(&run);
}
