#include "catalog.h"

#include "../core/check.h"
#include "../core/file_io.h"
#include "../core/text.h"

enum {
    KIND_CAPACITY = 24,
    RECORD_LIMIT = 4096,
    NAME_WIDTH = 16,
    FILE_NAME_WIDTH = 24,
    CHANGE_KIND_COUNT = 7,
    TEAM_KIND = 1,
    REQUIRE_KIND = 3,
    LOOK_KIND = 6,
    ID_FIELD_COUNT = 3,
    FIELD_TYPE_FIELD = 4,
    PATCH_VALUE_FIELD = 5,
    FIRST_STRING_FIELD = 3,
    LOOK_CIV_FIELD = 3,
    FIRST_LOOK_UNIT_FIELD = 4,
    BASE_BONUS_FIELD = 3,
    TREE_REGION_FIELD = 7,
    TREE_CIVS_FIELD = 8,
    TREE_STATUS_FIELD = 9
};

typedef enum {
    CATALOG_FILE_APPEARANCE,
    CATALOG_FILE_BONUSES,
    CATALOG_FILE_TEAM_BONUSES,
    CATALOG_FILE_UNITS,
    CATALOG_FILE_TECHS,
    CATALOG_FILE_TREE,
    CATALOG_FILE_BONUS_EFFECTS,
    CATALOG_FILE_COUNT
} CatalogFile;

static const wchar_t FILE_NAMES[CATALOG_FILE_COUNT][FILE_NAME_WIDTH] = {
    L"appearance.txt", L"bonuses.txt", L"team_bonuses.txt", L"units.txt", L"techs.txt", L"tree.txt",
    L"bonus_effects.txt"
};

static const char CATEGORY_NAMES[BONUS_CATEGORY_COUNT + 1][NAME_WIDTH] = {
    "naval", "monk", "cost", "economy", "speed", "combat", "research", "building", "unlock", "general"
};

static const char APPEARANCE_NAMES[APPEARANCE_KIND_COUNT][NAME_WIDTH] = {
    "civ", "architecture", "scout", "monk", "base"
};
static const char TREE_NAMES[TREE_KIND_COUNT][NAME_WIDTH] = { "unit", "building", "tech" };
static const char REGION_NAMES[1][NAME_WIDTH] = { "universal" };
static const char STATUS_NAMES[1][NAME_WIDTH] = { "required" };
static const char CHANGE_KINDS[CHANGE_KIND_COUNT][NAME_WIDTH] = { "civ", "team", "unit", "require",
                                                                  "train", "cost", "look" };
static const char FIELD_TYPES[UNIT_FIELD_TYPE_COUNT][NAME_WIDTH] = { "u8", "i16", "f32" };

static TsvFile source;
static TsvRecord record;

static int name_index(const char names[][NAME_WIDTH], int count, const char *wanted)
{
    int index;

    for (index = 0; (index < count) && (index < BONUS_CATEGORY_COUNT); ++index) {
        if (text_equal(names[index], wanted)) {
            return index;
        }
    }
    return CATALOG_MISSING;
}

static int field_index(int field, const char names[][NAME_WIDTH], int count)
{
    char kind[KIND_CAPACITY] = "";

    if (!tsv_text(&record, field, kind, sizeof kind)) {
        return CATALOG_MISSING;
    }
    return name_index(names, count, kind);
}

static bool add_base(Catalog *catalog)
{
    BaseCiv base;
    int value;

    if (catalog->base_count >= CATALOG_BASE_CAPACITY) {
        return false;
    }
    base.id = tsv_number(&record, 1);
    base.bonuses = tsv_number_list(&record, BASE_BONUS_FIELD);
    for (value = 0; value < (int)BASE_VALUE_COUNT; ++value) {
        base.values[value] = tsv_number(&record, BASE_BONUS_FIELD + 1 + value);
    }
    catalog->bases[catalog->base_count] = base;
    ++catalog->base_count;
    return (base.id >= 0) && (base.id < TSV_BIT_LIMIT);
}

static void read_looks(AppearanceEntry *entry)
{
    int string;
    int unit;

    for (string = 0; string < CIV_STRING_COUNT; ++string) {
        entry->string_ids[string] = tsv_number(&record, FIRST_STRING_FIELD + string);
    }
    entry->look_civ = (entry->kind == APPEARANCE_CIV) ? CATALOG_MISSING : tsv_number(&record, LOOK_CIV_FIELD);
    for (unit = 0; unit < CATALOG_LOOK_UNITS; ++unit) {
        entry->look_units[unit] =
            (entry->kind == APPEARANCE_MONK) ? tsv_number(&record, FIRST_LOOK_UNIT_FIELD + unit) : CATALOG_MISSING;
    }
}

static bool add_appearance(Catalog *catalog)
{
    AppearanceEntry entry;
    int kind = field_index(0, APPEARANCE_NAMES, APPEARANCE_KIND_COUNT);

    if ((kind == CATALOG_MISSING) || (catalog->appearance_count >= CATALOG_APPEARANCE_CAPACITY)) {
        return false;
    }
    entry.kind = (AppearanceKind)kind;
    entry.id = tsv_number(&record, 1);
    read_looks(&entry);
    if (!tsv_text(&record, 2, entry.name, sizeof entry.name)) {
        return false;
    }
    catalog->appearance[catalog->appearance_count] = entry;
    ++catalog->appearance_count;
    return (entry.kind != APPEARANCE_BASE) || add_base(catalog);
}

static bool read_bonus(BonusEntry *entry, bool team)
{
    int category = team ? (int)BONUS_GENERAL : field_index(1, CATEGORY_NAMES, BONUS_CATEGORY_COUNT);
    int text_field = team ? 1 : 3;

    if (category == CATALOG_MISSING) {
        return false;
    }
    entry->id = tsv_number(&record, 0);
    entry->category = (BonusCategory)category;
    entry->headline[0] = '\0';
    if (!team && !tsv_text(&record, 2, entry->headline, sizeof entry->headline)) {
        return false;
    }
    entry->effects = tsv_number_list(&record, text_field + 1);
    return CHECK(entry->id != TSV_MISSING) &&
           tsv_text(&record, text_field, entry->description, sizeof entry->description);
}

static bool add_bonus(Catalog *catalog, bool team)
{
    if (team) {
        if (catalog->team_bonus_count >= CATALOG_TEAM_BONUS_CAPACITY) {
            return false;
        }
        ++catalog->team_bonus_count;
        return read_bonus(&catalog->team_bonuses[catalog->team_bonus_count - 1], true);
    }
    if (catalog->bonus_count >= CATALOG_BONUS_CAPACITY) {
        return false;
    }
    ++catalog->bonus_count;
    return read_bonus(&catalog->bonuses[catalog->bonus_count - 1], false);
}

static UnitRole role_of(char letter)
{
    if (letter == 'C') {
        return UNIT_ROLE_CUSTOM;
    }
    return (letter == 'H') ? UNIT_ROLE_HERO : UNIT_ROLE_VANILLA;
}

_Static_assert(UNIT_STAT_SPEED < UNIT_STAT_COUNT, "the speed is among the stats of a unit");

static void read_stats(UnitEntry *entry)
{
    int stat;

    for (stat = 0; stat < UNIT_STAT_COUNT; ++stat) {
        entry->stats[stat] = (stat >= (int)UNIT_STAT_SPEED) ? tsv_hundredths(&record, 3 + stat)
                                                             : tsv_number(&record, 3 + stat);
    }
}

static bool add_unit(Catalog *catalog)
{
    UnitEntry entry;
    char role[KIND_CAPACITY] = "";

    if ((catalog->unit_count >= CATALOG_UNIT_CAPACITY) || !tsv_text(&record, 0, role, sizeof role)) {
        return false;
    }
    entry.role = role_of(role[0]);
    entry.id = tsv_number(&record, 1);
    read_stats(&entry);
    entry.make_tech = tsv_number(&record, 3 + UNIT_STAT_COUNT);
    entry.elite_tech = tsv_number(&record, 4 + UNIT_STAT_COUNT);
    if (!CHECK(entry.id != TSV_MISSING) || !tsv_text(&record, 2, entry.name, sizeof entry.name)) {
        return false;
    }
    catalog->units[catalog->unit_count] = entry;
    ++catalog->unit_count;
    return true;
}

static bool read_tech(TechEntry *entry)
{
    char tier[KIND_CAPACITY] = "";
    int resource;

    if (!tsv_text(&record, 0, tier, sizeof tier)) {
        return false;
    }
    entry->tier = text_equal(tier, "imperial") ? TECH_TIER_IMPERIAL : TECH_TIER_CASTLE;
    entry->id = tsv_number(&record, 1);
    for (resource = 0; resource < RESOURCE_KIND_COUNT; ++resource) {
        entry->cost[resource] = tsv_number(&record, 4 + resource);
    }
    entry->time = tsv_number(&record, 8);
    entry->effects = tsv_number_list(&record, 9);
    return CHECK(entry->id >= 0) && tsv_text(&record, 2, entry->name, sizeof entry->name) &&
           tsv_text(&record, 3, entry->description, sizeof entry->description);
}

static bool add_tech(Catalog *catalog)
{
    TechEntry entry;

    if ((catalog->tech_count >= CATALOG_TECH_CAPACITY) || !read_tech(&entry)) {
        return false;
    }
    catalog->techs[catalog->tech_count] = entry;
    ++catalog->tech_count;
    return true;
}

static bool add_tree_node(Catalog *catalog)
{
    TreeEntry entry;
    int kind = field_index(0, TREE_NAMES, TREE_KIND_COUNT);

    if ((kind == CATALOG_MISSING) || (catalog->tree_count >= CATALOG_TREE_CAPACITY)) {
        return false;
    }
    entry.kind = (TreeKind)kind;
    entry.id = tsv_number(&record, 1);
    entry.age = tsv_number(&record, 3);
    entry.building = tsv_number(&record, 4);
    entry.parent = tsv_number(&record, 5);
    entry.lane = tsv_number(&record, 6);
    entry.universal = (field_index(TREE_REGION_FIELD, REGION_NAMES, 1) == 0);
    entry.needs_research = (field_index(TREE_STATUS_FIELD, STATUS_NAMES, 1) == 0);
    entry.civs = tsv_bits(&record, TREE_CIVS_FIELD);
    if (!CHECK(entry.id != TSV_MISSING) || !tsv_text(&record, 2, entry.name, sizeof entry.name)) {
        return false;
    }
    catalog->tree[catalog->tree_count] = entry;
    ++catalog->tree_count;
    return true;
}

static bool read_unit_patch(UnitPatch *patch, int type, UnitPart part)
{
    patch->part = part;
    patch->bonus_id = tsv_number(&record, 1);
    patch->unit = tsv_number(&record, 2);
    patch->offset = tsv_number(&record, 3);
    patch->type = (UnitFieldType)type;
    return tsv_decimal(&record, PATCH_VALUE_FIELD, &patch->value) && CHECK(patch->bonus_id != TSV_MISSING) &&
           (patch->unit >= 0) && (patch->offset >= 0);
}

static bool add_unit_patch(Catalog *catalog, UnitPart part)
{
    int type = field_index(FIELD_TYPE_FIELD, FIELD_TYPES, UNIT_FIELD_TYPE_COUNT);

    if ((type == CATALOG_MISSING) || !CHECK(catalog->unit_patch_count < CATALOG_UNIT_PATCH_CAPACITY) ||
        !read_unit_patch(&catalog->unit_patches[catalog->unit_patch_count], type, part)) {
        return false;
    }
    ++catalog->unit_patch_count;
    return true;
}

static bool read_ids(int ids[ID_FIELD_COUNT])
{
    ids[0] = tsv_number(&record, 1);
    ids[1] = tsv_number(&record, 2);
    ids[2] = tsv_number(&record, ID_FIELD_COUNT);
    return CHECK(ids[0] != TSV_MISSING) && (ids[1] >= 0) && (ids[2] >= 0);
}

static bool add_id_line(Catalog *catalog, bool look)
{
    int ids[ID_FIELD_COUNT];
    int *count = look ? &catalog->unit_look_count : &catalog->requirement_count;

    if (!read_ids(ids) || !CHECK(*count < (look ? CATALOG_UNIT_LOOK_CAPACITY : CATALOG_REQUIREMENT_CAPACITY))) {
        return false;
    }
    if (look) {
        catalog->unit_looks[*count] = (UnitLook){ ids[0], ids[1], ids[2] };
    } else {
        catalog->requirements[*count] = (BonusRequirement){ ids[0], ids[1], ids[2] };
    }
    ++*count;
    return true;
}

static bool read_bonus_change(BonusChange *entry, int kind)
{
    entry->team = (kind == TEAM_KIND);
    entry->bonus_id = tsv_number(&record, 1);
    entry->entry = tsv_number(&record, 2);
    entry->requires = tsv_number_list(&record, 3);
    entry->type = tsv_number(&record, 4);
    entry->a = tsv_number(&record, 5);
    entry->b = tsv_number(&record, 6);
    entry->c = tsv_number(&record, 7);
    return tsv_decimal(&record, 8, &entry->d) && CHECK(entry->bonus_id != TSV_MISSING) &&
           (entry->entry != TSV_MISSING) && (entry->type >= 0);
}

static UnitPart part_of(int kind)
{
    static const UnitPart PARTS[CHANGE_KIND_COUNT] = { UNIT_PART_COUNT, UNIT_PART_COUNT, UNIT_PART_START,
                                                       UNIT_PART_COUNT, UNIT_PART_TRAIN_LOCATION, UNIT_PART_COSTS,
                                                       UNIT_PART_COUNT };

    return (kind == CATALOG_MISSING) ? UNIT_PART_COUNT : PARTS[kind];
}

static bool add_bonus_change(Catalog *catalog)
{
    int kind = field_index(0, CHANGE_KINDS, CHANGE_KIND_COUNT);
    UnitPart part = part_of(kind);

    if (part != UNIT_PART_COUNT) {
        return add_unit_patch(catalog, part);
    }
    if ((kind == REQUIRE_KIND) || (kind == LOOK_KIND)) {
        return add_id_line(catalog, kind == LOOK_KIND);
    }
    if ((kind == CATALOG_MISSING) || !CHECK(catalog->bonus_change_count < CATALOG_BONUS_CHANGE_CAPACITY) ||
        !read_bonus_change(&catalog->bonus_changes[catalog->bonus_change_count], kind)) {
        return false;
    }
    ++catalog->bonus_change_count;
    return true;
}

static bool add_choice_record(Catalog *catalog, CatalogFile file)
{
    switch ((int)file) {
    case CATALOG_FILE_APPEARANCE:
        return add_appearance(catalog);
    case CATALOG_FILE_BONUSES:
        return add_bonus(catalog, false);
    default:
        return add_bonus(catalog, true);
    }
}

static bool add_record(Catalog *catalog, CatalogFile file)
{
    switch ((int)file) {
    case CATALOG_FILE_UNITS:
        return add_unit(catalog);
    case CATALOG_FILE_TECHS:
        return add_tech(catalog);
    case CATALOG_FILE_BONUS_EFFECTS:
        return add_bonus_change(catalog);
    case CATALOG_FILE_TREE:
        return add_tree_node(catalog);
    default:
        return add_choice_record(catalog, file);
    }
}

static bool load_file(Catalog *catalog, const wchar_t *folder, CatalogFile file)
{
    wchar_t path[FILE_PATH_CAPACITY];
    int step;

    if (!file_join_path(path, FILE_PATH_CAPACITY, folder, FILE_NAMES[file]) || !tsv_open(&source, path)) {
        return false;
    }
    for (step = 0; (step < RECORD_LIMIT) && tsv_next(&source, &record); ++step) {
        if ((record.text[0] != '\0') && !add_record(catalog, file)) {
            return false;
        }
    }
    return CHECK(step < RECORD_LIMIT);
}

bool catalog_load(Catalog *catalog, const wchar_t *folder)
{
    int file;

    catalog->appearance_count = 0;
    catalog->bonus_count = 0;
    catalog->team_bonus_count = 0;
    catalog->unit_count = 0;
    catalog->tech_count = 0;
    catalog->tree_count = 0;
    catalog->base_count = 0;
    catalog->bonus_change_count = 0;
    catalog->unit_patch_count = 0;
    catalog->requirement_count = 0;
    catalog->unit_look_count = 0;
    for (file = 0; file < (int)CATALOG_FILE_COUNT; ++file) {
        if (!load_file(catalog, folder, (CatalogFile)file)) {
            return false;
        }
    }
    return true;
}

const char *catalog_category_name(BonusCategory category)
{
    return (CHECK((int)category >= 0) && CHECK(category < BONUS_CATEGORY_COUNT)) ? CATEGORY_NAMES[category] : "";
}
