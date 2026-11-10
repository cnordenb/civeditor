#include "forge.h"

#include "../core/check.h"
#include "../core/text.h"
#include "../dat/dat_parts.h"
#include "../civlist/civ_list.h"
#include "../json/json_reader.h"
#include "../json/json_writer.h"
#include "../mod/mod_places.h"
#include "../mod/mod_strings.h"
#include "../deflate/deflate.h"
#include "../pack/deflate_store.h"
#include "../techtree/tech_tree.h"

enum {
    BUFFER_CAPACITY = 104857600,
    VANILLA_CAPACITY = 4194304,
    LANGUAGE_COUNT = 15,
    LANGUAGE_WIDTH = 4,
    CONTINUATION_MASK = 0xC0,
    CONTINUATION = 0x80,
    HELP_TO_TOOLTIP = MOD_HELP_OFFSET - MOD_TOOLTIP_OFFSET
};

typedef struct {
    const ForgeJob *job;
    ForgeResult *result;
    size_t plain_size;
    size_t packed_size;
    long name_sid;
    char prefix[FORGE_PREFIX_CAPACITY];
} ForgeRun;

static const char LANGUAGES[LANGUAGE_COUNT][LANGUAGE_WIDTH] = {
    "br", "de", "en", "es", "fr", "hi", "it", "jp", "ko", "ms", "mx", "tr", "tw", "vi", "zh"
};

static unsigned char buffer_one[BUFFER_CAPACITY];
static unsigned char buffer_two[BUFFER_CAPACITY];
static char vanilla[VANILLA_CAPACITY];
static DatMap map;
static DatEdit edit;
static ModStrings strings;
static JsonDocument civilizations;
static CivListOutput civ_list;
static JsonWriter info;

static bool fail(ForgeResult *result, const char *problem)
{
    if (!CHECK(result != NULL) || !CHECK(problem != NULL)) {
        return false;
    }
    (void)text_copy(result->problem, sizeof result->problem, problem);
    return false;
}

static char prefix_letter(unsigned char letter)
{
    bool upper = (letter >= 'A') && (letter <= 'Z');
    bool lower = (letter >= 'a') && (letter <= 'z');
    bool digit = (letter >= '0') && (letter <= '9');

    if (!CHECK(letter != '\0') || !CHECK('a' > 'A')) {
        return '_';
    }
    if (upper) {
        return (char)(letter + ('a' - 'A'));
    }
    return (lower || digit || (letter == '_') || (letter == '-')) ? (char)letter : '_';
}

bool forge_prefix(const char *alias, char *prefix, size_t capacity)
{
    size_t from;
    size_t to = 0U;
    unsigned char letter;

    if (!CHECK(alias != NULL) || !CHECK(prefix != NULL) || !CHECK(capacity > 1U)) {
        return false;
    }
    for (from = 0U; (from < (size_t)CIV_NAME_CAPACITY) && (alias[from] != '\0') && (to + 1U < capacity); ++from) {
        letter = (unsigned char)alias[from];
        if ((letter & CONTINUATION_MASK) != CONTINUATION) {
            prefix[to] = prefix_letter(letter);
            ++to;
        }
    }
    prefix[to] = '\0';
    return (to > 0U) || text_copy(prefix, capacity, "custom_civ");
}

static bool joined(char *text, const char *first, const char *second)
{
    if (!CHECK(text != NULL) || !CHECK(first != NULL) || !CHECK(second != NULL)) {
        return false;
    }
    return text_copy(text, FILE_PATH_CAPACITY, first) && text_append(text, FILE_PATH_CAPACITY, second);
}

static bool game_path(const ForgeRun *run, const char *relative, wchar_t *path)
{
    wchar_t wide[FILE_PATH_CAPACITY];
    const ForgeJob *job;

    if (!CHECK(run != NULL) || !CHECK(relative != NULL)) {
        return false;
    }
    job = run->job;
    return text_widen(relative, wide, FILE_PATH_CAPACITY) && mod_game_file(job->game_folder, wide, path);
}

static bool mods_path(const ForgeRun *run, const char *relative, wchar_t *path)
{
    wchar_t wide[FILE_PATH_CAPACITY];
    const ForgeJob *job;

    if (!CHECK(run != NULL) || !CHECK(relative != NULL)) {
        return false;
    }
    job = run->job;
    return text_widen(relative, wide, FILE_PATH_CAPACITY) && mod_make_folders(job->mods_folder, wide) &&
           file_join_path(path, FILE_PATH_CAPACITY, job->mods_folder, wide);
}

static long find_name_sid(const ForgeRun *run, int slot)
{
    wchar_t path[FILE_PATH_CAPACITY];
    FileRead read;
    int civ;

    if (!CHECK(run != NULL) || !CHECK(slot > 0) ||
        !game_path(run, "resources\\_common\\dat\\civilizations.json", path)) {
        return -1L;
    }
    read = file_read_all(path, civilizations.text, sizeof civilizations.text);
    civilizations.length = (int)read.length;
    if (!read.succeeded || !json_parse(&civilizations)) {
        return -1L;
    }
    civ = civ_list_entry(&civilizations, slot);
    civ = (civ == JSON_NONE) ? JSON_NONE : json_member(&civilizations, civ, "name_string_id");
    if ((civ == JSON_NONE) || !json_number(&civilizations, civ).valid) {
        return -1L;
    }
    return json_number(&civilizations, civ).value;
}

static bool unpack_game(ForgeRun *run)
{
    wchar_t path[FILE_PATH_CAPACITY];
    ByteBuffer plain = { buffer_one, sizeof buffer_one, 0U };
    ByteSpan packed = { buffer_two, 0U };
    FileRead read;

    if (!CHECK(run != NULL) || !CHECK(run->result != NULL)) {
        return false;
    }
    if (!game_path(run, "resources\\_common\\dat\\empires2_x2_p1.dat", path)) {
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

    if (!CHECK(input != NULL) || !dat_cursor_start(&cursor, input->dat, at)) {
        return CIV_LIST_KEEP;
    }
    return dat_cursor_i32(&cursor);
}

static void read_own_texts(const RecipeInput *input, CivListEntry *entry)
{
    const DatMap *slots;
    DatCursor cursor;
    size_t at = 0U;
    long help;

    if (!CHECK(input != NULL) || !CHECK(entry != NULL) || (entry->unit < 0)) {
        return;
    }
    slots = input->map;
    if (!dat_find_unit(input->dat, slots->civ_at[input->slot], entry->unit, &at) ||
        !dat_cursor_start(&cursor, input->dat, at + DAT_UNIT_ICON_AT)) {
        return;
    }
    entry->icon = dat_cursor_i16(&cursor);
    if (!entry->unit_named) {
        help = unit_field(input, at + DAT_UNIT_HELP_AT);
        entry->unit_name_sid = unit_field(input, at + DAT_UNIT_NAME_AT);
        entry->unit_description_sid = (help < MOD_HELP_OFFSET) ? CIV_LIST_KEEP : help - HELP_TO_TOOLTIP;
    }
}

static bool make_civ_list(const ForgeRun *run, const RecipeInput *input)
{
    const ForgeResult *result;
    const RecipeReport *report;
    RecipeUnitIds ids = recipe_unique_unit_ids(input);
    CivListEntry entry;
    long sid;

    if (!CHECK(run != NULL) || !CHECK(input != NULL)) {
        return false;
    }
    result = run->result;
    report = &result->recipe;
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
    return civ_list_write(&civilizations, &entry, &civ_list);
}

static bool edit_game(ForgeRun *run)
{
    const ForgeJob *job;
    const CivDesign *design;
    ForgeResult *result;
    DatBytes original = { buffer_one, 0U };
    DatOutput output = { buffer_two, sizeof buffer_two, 0U };
    ByteBuffer packed = { buffer_one, sizeof buffer_one, 0U };
    ByteSpan plain = { buffer_two, 0U };
    RecipeInput input;

    if (!CHECK(run != NULL) || !CHECK(run->job != NULL)) {
        return false;
    }
    job = run->job;
    design = job->design;
    result = run->result;
    original.size = run->plain_size;
    input.design = design;
    input.catalog = job->catalog;
    input.dat = &original;
    input.map = &map;
    input.slot = design->choices[CHOICE_REPLACE];
    if (!dat_edit_start(&edit, &map) || !recipe_apply(&input, &edit, &result->recipe) ||
        !dat_edit_write(&edit, &original, &output) || !make_civ_list(run, &input)) {
        return fail(result, "The civ could not be written into the game data.");
    }
    plain.size = output.length;
    if (!deflate_pack(plain, &packed) && !deflate_store(plain, &packed)) {
        return fail(result, "The mod's data file could not be packed.");
    }
    run->packed_size = packed.size;
    return true;
}

static bool mods_file(const ForgeRun *run, const char *folder, const wchar_t *name, wchar_t *path)
{
    wchar_t place[FILE_PATH_CAPACITY];

    if (!CHECK(name != NULL) || !CHECK(path != NULL)) {
        return false;
    }
    return mods_path(run, folder, place) && file_join_path(path, FILE_PATH_CAPACITY, place, name);
}

static bool write_info(const ForgeRun *run, const char *folder, const char *title)
{
    wchar_t path[FILE_PATH_CAPACITY];

    if (!CHECK(run != NULL) || !CHECK(title != NULL) || !CHECK(folder != NULL)) {
        return false;
    }
    json_writer_start(&info);
    json_begin(&info, NULL, '{');
    json_write_string(&info, "Title", title);
    json_write_number(&info, "CacheStatus", 0);
    json_write_string(&info, "Description", "");
    json_write_string(&info, "Author", "");
    json_write_string(&info, "builder_version", "1.0.0");
    json_end(&info, '}');
    return json_writer_succeeded(&info) && mods_file(run, folder, L"info.json", path) &&
           file_write_all(path, info.data, info.length);
}

static bool write_tech_tree(const ForgeRun *run, const char *folder)
{
    char name[FILE_PATH_CAPACITY];
    char relative[FILE_PATH_CAPACITY];
    wchar_t wide[FILE_PATH_CAPACITY];
    wchar_t game_file[FILE_PATH_CAPACITY];
    wchar_t mod_file[FILE_PATH_CAPACITY];
    const ForgeJob *job;
    TechTreeJob tree;

    if (!CHECK(run != NULL) || !CHECK(folder != NULL)) {
        return false;
    }
    job = run->job;
    tree.design = job->design;
    tree.catalog = job->catalog;
    return tech_tree_file_name(&civilizations, tree.design->choices[CHOICE_REPLACE], name) &&
           joined(relative, "resources\\_common\\dat\\CivTechTrees\\", name) && game_path(run, relative, game_file) &&
           joined(relative, folder, "\\resources\\_common\\dat\\CivTechTrees") &&
           text_widen(name, wide, FILE_PATH_CAPACITY) && mods_file(run, relative, wide, mod_file) &&
           tech_tree_patch_file(&tree, game_file, mod_file);
}

static bool write_data_mod(ForgeRun *run)
{
    const ForgeJob *job;
    const CivDesign *design;
    ForgeResult *result;
    char folder[FILE_PATH_CAPACITY];
    char relative[FILE_PATH_CAPACITY];
    wchar_t path[FILE_PATH_CAPACITY];

    if (!CHECK(run != NULL) || !CHECK(run->job != NULL)) {
        return false;
    }
    job = run->job;
    design = job->design;
    result = run->result;
    if (!joined(folder, run->prefix, "-data") || !write_info(run, folder, design->texts[TEXT_FIELD_NAME]) ||
        !joined(relative, folder, "\\resources\\_common\\dat") ||
        !mods_file(run, relative, L"empires2_x2_p1.dat", path) ||
        !file_write_all(path, (const char *)buffer_one, run->packed_size) ||
        !mods_file(run, relative, L"civilizations.json", path) ||
        !file_write_all(path, civ_list.text, civ_list.length)) {
        return fail(result, "The data mod could not be written to the mods folder.");
    }
    if (!write_tech_tree(run, folder)) {
        return fail(result, "The civ's tech tree could not be written into the data mod.");
    }
    return mods_path(run, folder, result->data_mod);
}

static bool write_language(const ForgeRun *run, const ModCiv *civ, int language)
{
    char relative[FILE_PATH_CAPACITY];
    char folder[FILE_PATH_CAPACITY];
    wchar_t path[FILE_PATH_CAPACITY];

    if (!CHECK(run != NULL) || !CHECK((language >= 0) && (language < LANGUAGE_COUNT))) {
        return false;
    }
    if (!joined(relative, "resources\\", LANGUAGES[language]) ||
        !text_append(relative, sizeof relative, "\\strings\\key-value\\key-value-strings-utf8.txt") ||
        !game_path(run, relative, path) || !file_read_all(path, vanilla, sizeof vanilla).succeeded) {
        vanilla[0] = '\0';
    }
    return mod_strings_write(&strings, civ, vanilla) && joined(folder, run->prefix, "-ui\\resources\\") &&
           text_append(folder, sizeof folder, LANGUAGES[language]) &&
           text_append(folder, sizeof folder, "\\strings\\key-value") &&
           mods_file(run, folder, L"key-value-modded-strings-utf8.txt", path) &&
           file_write_all(path, strings.text, strings.length);
}

static bool write_ui_mod(ForgeRun *run)
{
    const ForgeJob *job;
    const CivDesign *design;
    ForgeResult *result;
    ModCiv civ;
    char folder[FILE_PATH_CAPACITY];
    char title[FILE_PATH_CAPACITY];
    int language;
    bool fine;

    if (!CHECK(run != NULL) || !CHECK(run->job != NULL)) {
        return false;
    }
    job = run->job;
    design = job->design;
    result = run->result;
    civ.design = design;
    civ.catalog = job->catalog;
    civ.slot = design->choices[CHOICE_REPLACE];
    civ.name_sid = run->name_sid;
    civ.hero_skipped = result->recipe.hero_skipped;
    fine = joined(folder, run->prefix, "-ui") && joined(title, design->texts[TEXT_FIELD_NAME], " (UI)") &&
           write_info(run, folder, title);
    for (language = 0; fine && (language < LANGUAGE_COUNT); ++language) {
        fine = write_language(run, &civ, language);
    }
    if (!fine) {
        return fail(result, "The UI mod could not be written to the mods folder.");
    }
    return mods_path(run, folder, result->ui_mod);
}

bool forge_build(const ForgeJob *job, ForgeResult *result)
{
    static ForgeRun run;
    const CivDesign *design;

    if (!CHECK(job != NULL) || !CHECK(result != NULL) || !CHECK(job->design != NULL)) {
        return false;
    }
    design = job->design;
    result->problem[0] = '\0';
    result->data_mod[0] = L'\0';
    result->ui_mod[0] = L'\0';
    run.job = job;
    run.result = result;
    if (!civ_design_is_complete(design)) {
        return fail(result, "The civ is not finished yet; the review step lists what is still needed.");
    }
    if (!forge_prefix(design->texts[TEXT_FIELD_NAME], run.prefix, sizeof run.prefix)) {
        return fail(result, "The civ's name cannot be made into a folder name.");
    }
    run.name_sid = find_name_sid(&run, design->choices[CHOICE_REPLACE]);
    if (run.name_sid < 0L) {
        return fail(result, "The game's list of civs could not be read.");
    }
    return unpack_game(&run) && edit_game(&run) && write_data_mod(&run) && write_ui_mod(&run);
}
