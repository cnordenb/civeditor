#include "dat_tests.h"

#include <string.h>

#include "../../src/core/check.h"
#include "../../src/dat/dat_edit.h"
#include "../../src/bonus/bonus_build.h"
#include "../../src/dat/dat_map.h"
#include "../../src/forge/recipe_parts.h"
#include "../../src/hero/hero.h"
#include "../../src/look/look.h"

enum {
    SMALL_DAT_CAPACITY = 98304,
    HEAD_COUNTS_SIZE = 10,
    TERRAIN_BLOCK_HEAD_SIZE = 140,
    TERRAIN_COUNT = 200,
    TERRAIN_HEAD_SIZE = 8,
    TERRAIN_MIDDLE_SIZE = 28,
    TERRAIN_TAIL_SIZE = 364,
    TERRAIN_BLOCK_TAIL_SIZE = 63,
    RANDOM_MAPS_SIZE = 8,
    EFFECT_AND_HEADER_COUNTS_SIZE = 8,
    CIV_COUNT_SIZE = 2,
    TECH_COUNT_SIZE = 2,
    TAIL_AND_TREE_SIZE = 37,
    EMPTY_TEXT_SIZE = 4,
    LOOM_CHANGE_COUNT = 3
};

enum {
    NO_CIVS = 0,
    SMALL_CIV_LIMIT = 4,
    SMALL_UNIT_LIMIT = 255,
    PLAYER_TYPE_SIZE = 1,
    CIV_IDS_AND_ICON_SIZE = 7,
    UNIT_COUNT_SIZE = 2,
    UNIT_POINTER_SIZE = 4,
    UNIT_NAME_AT = 180,
    BUILDING_UNIT_SIZE = 562,
    CREATABLE_UNIT_SIZE = 473,
    TRAIN_LOCATION_COUNT_AT = 365,
    CIV_RECORD_SIZE = 576,
    CREATABLE_TYPE = 70,
    BUILDING_TYPE = 80,
    BUILDING_CLASS = 3,
    FIRST_GRAPHIC = 100
};

static const char VERSION[] = "VER 8.9";
static const unsigned char EMPTY_TEXT[EMPTY_TEXT_SIZE] = { 0x60, 0x0A, 0x00, 0x00 };
static const char LOOM[] = "Loom";

static const DatChange LOOM_CHANGES[LOOM_CHANGE_COUNT] = {
    { .type = 4, .a = -1, .b = 4, .c = 0, .d = 15.0f },
    { .type = 4, .a = -1, .b = 4, .c = 8, .d = 1025.0f },
    { .type = 4, .a = -1, .b = 4, .c = 8, .d = 770.0f }
};

static const unsigned char LOOM_EFFECT_BYTES[] = {
    0x60, 0x0A, 0x04, 0x00, 0x4C, 0x6F, 0x6F, 0x6D, 0x03, 0x00, 0x04, 0xFF, 0xFF, 0x04, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x70, 0x41, 0x04, 0xFF, 0xFF, 0x04, 0x00, 0x08, 0x00, 0x00, 0x20, 0x80, 0x44,
    0x04, 0xFF, 0xFF, 0x04, 0x00, 0x08, 0x00, 0x00, 0x80, 0x40, 0x44
};

static const DatTech LOOM_TECH = {
    .required_techs = { 104, -1, -1, -1, -1, -1 },
    .costs = { { 3, 50, 1 }, { -1, 0, 0 }, { -1, 0, 0 } },
    .required_tech_count = 1,
    .civ = -1,
    .language_dll_name = 7022L,
    .language_dll_description = 8022L,
    .effect_id = 22,
    .icon_id = 6,
    .language_dll_help = 107022L,
    .language_dll_tech_tree = 157022L,
    .name = "Loom",
    .repeatable = 1,
    .research_location_count = 1,
    .research_locations = { { 109, 25, 6, 18022L } }
};

static const unsigned char LOOM_TECH_BYTES[] = {
    0x68, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x03, 0x00, 0x32, 0x00,
    0x01, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x01, 0x00, 0xFF, 0xFF, 0x00,
    0x00, 0x6E, 0x1B, 0x00, 0x00, 0x56, 0x1F, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x06, 0x00, 0x0E,
    0xA2, 0x01, 0x00, 0x5E, 0x65, 0x02, 0x00, 0x60, 0x0A, 0x04, 0x00, 0x4C, 0x6F, 0x6F, 0x6D, 0x01,
    0x01, 0x00, 0x6D, 0x00, 0x19, 0x00, 0x06, 0x66, 0x46, 0x00, 0x00
};

static unsigned char small_dat[SMALL_DAT_CAPACITY];
static unsigned char written[SMALL_DAT_CAPACITY];
static size_t small_size;
static DatBytes small_bytes;
static DatUnitList written_units;

typedef struct {
    int index;
    int unit_class;
    int type;
} SmallUnit;

static SmallUnit small_unit = { 0, BUILDING_CLASS, BUILDING_TYPE };
static DatMap small_map;
static DatMap written_map;
static DatEdit edit;

static size_t put_empty_text(size_t at)
{
    size_t index;

    if (!CHECK(at > 0U) || !CHECK(at < SMALL_DAT_CAPACITY)) {
        return SMALL_DAT_CAPACITY;
    }
    for (index = 0U; (index < EMPTY_TEXT_SIZE) && (at + index < SMALL_DAT_CAPACITY); ++index) {
        small_dat[at + index] = EMPTY_TEXT[index];
    }
    return at + EMPTY_TEXT_SIZE;
}

static size_t put_terrain(size_t at)
{
    if (!CHECK(at > 0U) || !CHECK(at < SMALL_DAT_CAPACITY)) {
        return SMALL_DAT_CAPACITY;
    }
    at = put_empty_text(at + TERRAIN_HEAD_SIZE);
    at = put_empty_text(at);
    at = put_empty_text(at + TERRAIN_MIDDLE_SIZE);
    return at + TERRAIN_TAIL_SIZE;
}

static size_t put_civ(size_t at, int standing_graphic)
{
    size_t pointers = (size_t)(small_unit.index + 1) * UNIT_POINTER_SIZE;
    size_t unit_at;

    if (!CHECK(at > 0U) || !CHECK(small_unit.index >= 0) || !CHECK(small_unit.index < SMALL_UNIT_LIMIT) ||
        !CHECK(at + CIV_RECORD_SIZE + pointers < SMALL_DAT_CAPACITY)) {
        return SMALL_DAT_CAPACITY;
    }
    at = put_empty_text(at + PLAYER_TYPE_SIZE) + CIV_IDS_AND_ICON_SIZE;
    small_dat[at] = (unsigned char)(small_unit.index + 1);
    unit_at = at + UNIT_COUNT_SIZE + pointers;
    small_dat[unit_at - UNIT_POINTER_SIZE] = 1U;
    small_dat[unit_at] = (unsigned char)small_unit.type;
    small_dat[unit_at + DAT_UNIT_CLASS_AT] = (unsigned char)small_unit.unit_class;
    small_dat[unit_at + DAT_UNIT_GRAPHICS_AT] = (unsigned char)standing_graphic;
    (void)put_empty_text(unit_at + UNIT_NAME_AT);
    if (small_unit.type == BUILDING_TYPE) {
        return unit_at + BUILDING_UNIT_SIZE;
    }
    small_dat[unit_at + TRAIN_LOCATION_COUNT_AT] = 1U;
    return unit_at + CREATABLE_UNIT_SIZE;
}

static bool start_small_edit(int civ_count)
{
    size_t at = sizeof VERSION + HEAD_COUNTS_SIZE + TERRAIN_BLOCK_HEAD_SIZE;
    int terrain;
    int civ;

    if (!CHECK(civ_count >= 0) || !CHECK(civ_count <= SMALL_CIV_LIMIT)) {
        return false;
    }
    (void)memset(small_dat, 0, sizeof small_dat);
    (void)memcpy(small_dat, VERSION, sizeof VERSION);
    for (terrain = 0; terrain < TERRAIN_COUNT; ++terrain) {
        at = put_terrain(at);
    }
    at += TERRAIN_BLOCK_TAIL_SIZE + RANDOM_MAPS_SIZE + EFFECT_AND_HEADER_COUNTS_SIZE;
    small_dat[at] = (unsigned char)civ_count;
    at += CIV_COUNT_SIZE;
    for (civ = 0; (civ < civ_count) && (civ < SMALL_CIV_LIMIT); ++civ) {
        at = put_civ(at, FIRST_GRAPHIC + civ);
    }
    small_size = at + TECH_COUNT_SIZE + TAIL_AND_TREE_SIZE;
    small_bytes.data = small_dat;
    small_bytes.size = small_size;
    return CHECK(small_size < SMALL_DAT_CAPACITY) && CHECK(dat_map_read(small_dat, small_size, &small_map)) &&
           CHECK(dat_edit_start(&edit, &small_map));
}

static bool write_small_edit(DatBytes *result)
{
    DatBytes original;
    DatOutput output;

    if (!CHECK(result != NULL) || !CHECK(small_size > 0U)) {
        return false;
    }
    original.data = small_dat;
    original.size = small_size;
    output.data = written;
    output.capacity = sizeof written;
    output.length = 0U;
    if (!CHECK(dat_edit_write(&edit, &original, &output))) {
        return false;
    }
    result->data = written;
    result->size = output.length;
    return CHECK(dat_map_read(written, output.length, &written_map));
}

static bool written_bytes_are(size_t at, const unsigned char *expected, size_t length)
{
    if (!CHECK(expected != NULL) || !CHECK(length <= sizeof written) || !CHECK(at <= sizeof written - length)) {
        return false;
    }
    return memcmp(&written[at], expected, length) == 0;
}

bool test_an_unchanged_dat_file_is_written_back_byte_for_byte(void)
{
    DatBytes result;

    if (!start_small_edit(NO_CIVS) || !write_small_edit(&result)) {
        return false;
    }
    return (result.size == small_size) && (memcmp(written, small_dat, small_size) == 0);
}

bool test_an_added_effect_is_written_as_the_game_holds_it(void)
{
    DatBytes result;
    int change;

    if (!start_small_edit(NO_CIVS) || !CHECK(dat_edit_add_effect(&edit, LOOM) == 0)) {
        return false;
    }
    for (change = 0; change < LOOM_CHANGE_COUNT; ++change) {
        (void)CHECK(dat_edit_add_change(&edit, &LOOM_CHANGES[change]));
    }
    if (!write_small_edit(&result) || !CHECK(written_map.effect_count == 1)) {
        return false;
    }
    return written_bytes_are(written_map.effect_at[0], LOOM_EFFECT_BYTES, sizeof LOOM_EFFECT_BYTES);
}

bool test_an_added_tech_is_written_as_the_game_holds_it(void)
{
    DatBytes result;

    if (!start_small_edit(NO_CIVS) || !CHECK(dat_edit_add_tech(&edit, &LOOM_TECH) == 0) || !write_small_edit(&result) ||
        !CHECK(written_map.tech_count == 1)) {
        return false;
    }
    return written_bytes_are(written_map.tech_at[0], LOOM_TECH_BYTES, sizeof LOOM_TECH_BYTES);
}

enum { BONUS_SLOT = 1, BONUS_ID = 55, BONUS_AGE = 102, ARMOR_CHANGE = 4, CAVALRY = 12, CAMELS = 47, PIERCE = 8 };

static const float PIERCE_PLUS_ONE = 769.0F;

static Catalog bonus_catalog;

static void put_bonus_change(int index, int unit_class)
{
    BonusChange *line;

    if (!CHECK(index >= 0) || !CHECK(index < CATALOG_BONUS_CHANGE_CAPACITY)) {
        return;
    }
    line = &bonus_catalog.bonus_changes[index];
    line->team = false;
    line->bonus_id = BONUS_ID;
    line->entry = 0;
    line->requires.values[0] = BONUS_AGE;
    line->requires.count = 1;
    line->type = ARMOR_CHANGE;
    line->a = DAT_NO_ID;
    line->b = unit_class;
    line->c = PIERCE;
    line->d = PIERCE_PLUS_ONE;
}

bool test_a_bonus_without_techs_is_built_from_its_changes(void)
{
    BonusBuild build = { &bonus_catalog, { &small_bytes, &small_map, &edit, BONUS_SLOT } };
    DatBytes result;
    DatTech tech;
    DatEffect effect;

    put_bonus_change(0, CAVALRY);
    put_bonus_change(1, CAMELS);
    bonus_catalog.bonus_change_count = 2;
    if (!start_small_edit(NO_CIVS) || !CHECK(bonus_build_civ(&build, BONUS_ID) == 1) || !write_small_edit(&result) ||
        !CHECK(written_map.tech_count == 1) || !dat_tech_read(&result, written_map.tech_at[0], &tech) ||
        !dat_effect_read(&result, written_map.effect_at[0], &effect)) {
        return false;
    }
    return (tech.civ == BONUS_SLOT) && (tech.effect_id == 0) && (tech.required_techs[0] == BONUS_AGE) &&
           (tech.required_tech_count == 1) && (effect.change_count == 2);
}

enum { LOOK_CIVS = 2, LOOK_SLOT = 1, ARCHITECTURE_CIV = 0 };

bool test_a_building_takes_the_look_of_its_architectures_civ(void)
{
    DatTarget target = { &small_bytes, &small_map, &edit, LOOK_SLOT };
    LookSources sources = { ARCHITECTURE_CIV, LOOK_NONE, LOOK_NONE,
                            { LOOK_NONE, { LOOK_NONE, LOOK_NONE, LOOK_NONE } } };
    DatBytes result;

    if (!start_small_edit(LOOK_CIVS) || !CHECK(look_build(&target, &sources)) || !write_small_edit(&result) ||
        !CHECK(dat_list_units(&result, written_map.civ_at[LOOK_SLOT], &written_units))) {
        return false;
    }
    return written[written_units.at[0] + DAT_UNIT_GRAPHICS_AT] == FIRST_GRAPHIC + ARCHITECTURE_CIV;
}

enum { MONK_CIV = 0, MONK = 125, DEAD_MONK = 134, MONK_WITH_RELIC = 286, MONK_CLASS = 18 };

bool test_a_monk_takes_the_look_of_its_monk_skin(void)
{
    DatTarget target = { &small_bytes, &small_map, &edit, LOOK_SLOT };
    LookSources sources = { LOOK_NONE, LOOK_NONE, LOOK_NONE, { MONK_CIV, { MONK, DEAD_MONK, MONK_WITH_RELIC } } };
    DatBytes result;
    bool built;

    small_unit.index = MONK;
    small_unit.unit_class = MONK_CLASS;
    built = start_small_edit(LOOK_CIVS) && CHECK(look_build(&target, &sources)) && write_small_edit(&result) &&
            CHECK(dat_list_units(&result, written_map.civ_at[LOOK_SLOT], &written_units));
    small_unit.index = 0;
    small_unit.unit_class = BUILDING_CLASS;
    return built && (written[written_units.at[MONK] + DAT_UNIT_GRAPHICS_AT] == FIRST_GRAPHIC + MONK_CIV);
}

enum { HERO_SLOT = 1, HERO = 5, INFANTRY_CLASS = 6, CASTLE = 82, BYTE_BITS = 8 };

static int written_i16(size_t at)
{
    if (!CHECK(at < sizeof written - 1U)) {
        return DAT_NO_ID;
    }
    return (int)written[at] | ((int)written[at + 1U] << BYTE_BITS);
}

bool test_a_hero_trains_at_the_castle(void)
{
    DatTarget target = { &small_bytes, &small_map, &edit, HERO_SLOT };
    DatBytes result;
    DatUnit unit;
    Hero hero;
    bool trains;

    small_unit.index = HERO;
    small_unit.unit_class = INFANTRY_CLASS;
    small_unit.type = CREATABLE_TYPE;
    trains = start_small_edit(LOOK_CIVS) && CHECK(hero_find(&target, HERO, &hero)) &&
             CHECK(hero_build(&target, &hero)) && write_small_edit(&result) &&
             CHECK(dat_list_units(&result, written_map.civ_at[HERO_SLOT], &written_units)) &&
             CHECK(dat_unit_read(&result, written_units.at[HERO], &unit)) &&
             (written_i16(unit.train_locations_at + DAT_TRAIN_BUILDING_AT) == CASTLE);
    small_unit.index = 0;
    small_unit.unit_class = BUILDING_CLASS;
    small_unit.type = BUILDING_TYPE;
    return trains;
}

enum { PATCH_SLOT = 1, FARM_BONUS = 330, COLLISION_X_AT = 29 };

static const UnitPatch FARM_PATCH = { FARM_BONUS, 0, COLLISION_X_AT, UNIT_FIELD_F32, 1.0F };
static const unsigned char ONE_TILE_BYTES[] = { 0x00, 0x00, 0x80, 0x3F };

bool test_a_unit_patch_changes_its_civs_unit(void)
{
    BonusBuild build = { &bonus_catalog, { &small_bytes, &small_map, &edit, PATCH_SLOT } };
    DatBytes result;

    bonus_catalog.bonus_change_count = 0;
    bonus_catalog.unit_patches[0] = FARM_PATCH;
    bonus_catalog.unit_patch_count = 1;
    if (!start_small_edit(LOOK_CIVS) || !CHECK(bonus_build_civ(&build, FARM_BONUS) == 1) ||
        !write_small_edit(&result) || !CHECK(dat_list_units(&result, written_map.civ_at[PATCH_SLOT], &written_units))) {
        return false;
    }
    return written_bytes_are(written_units.at[0] + COLLISION_X_AT, ONE_TILE_BYTES, sizeof ONE_TILE_BYTES);
}

enum { KEPT_UNIT = 4, LACKED_UNIT = 7, OTHER_UNIT = 9, VILLAGER = 83, BARRACKS = 12, FIRST_TECH = 0, SECOND_TECH = 1 };

static Catalog tree_catalog;
static CivDesign tree_design;
static RecipeChanges tree_changes;

static bool add_unit_tech(int unit)
{
    DatChange enable = { RECIPE_ENABLE_UNIT, 0, 1, -1, 0.0F };
    DatTech tech = LOOM_TECH;

    enable.a = unit;
    tech.effect_id = dat_edit_add_effect(&edit, LOOM);
    return CHECK(tech.effect_id >= 0) && CHECK(dat_edit_add_change(&edit, &enable)) &&
           CHECK(dat_edit_add_tech(&edit, &tech) >= 0);
}

static bool design_has_first_of(TreeKind kind, int first, int second)
{
    civ_design_reset(&tree_design);
    tree_catalog.tree[0].kind = kind;
    tree_catalog.tree[0].id = first;
    tree_catalog.tree[1].kind = kind;
    tree_catalog.tree[1].id = second;
    tree_catalog.tree_count = 2;
    return CHECK(civ_design_toggle_tree(&tree_design, kind, first)) && CHECK(tree_catalog.tree_count == 2);
}

static bool only_second_tech_is_disabled(int first_unit, int second_unit)
{
    RecipeReport report = { 0 };
    DatBytes result;
    RecipeInput input = { &tree_design, &tree_catalog, &result, &written_map, BONUS_SLOT };

    tree_changes.count = 0;
    if (!start_small_edit(NO_CIVS) || !add_unit_tech(first_unit) || !add_unit_tech(second_unit) ||
        !write_small_edit(&result) || !CHECK(recipe_wire_tree(&input, &tree_changes, &report))) {
        return false;
    }
    return (tree_changes.count == 1) && (tree_changes.changes[0].type == RECIPE_DISABLE_TECH) &&
           ((int)tree_changes.changes[0].d == SECOND_TECH);
}

bool test_only_the_tech_of_a_unit_the_civ_lacks_is_disabled(void)
{
    return design_has_first_of(TREE_UNITS, KEPT_UNIT, LACKED_UNIT) &&
           only_second_tech_is_disabled(KEPT_UNIT, LACKED_UNIT);
}

bool test_only_a_tech_the_civ_lacks_is_disabled(void)
{
    return design_has_first_of(TREE_TECHS, FIRST_TECH, SECOND_TECH) &&
           only_second_tech_is_disabled(OTHER_UNIT, OTHER_UNIT);
}

bool test_a_civ_keeps_its_villagers_in_the_dat_file(void)
{
    return design_has_first_of(TREE_UNITS, VILLAGER, LACKED_UNIT) &&
           CHECK(civ_design_toggle_tree(&tree_design, TREE_BUILDINGS, BARRACKS)) &&
           CHECK(civ_design_toggle_tree(&tree_design, TREE_UNITS, VILLAGER)) &&
           only_second_tech_is_disabled(VILLAGER, LACKED_UNIT);
}
