#include "recipe_tests.h"

#include <string.h>

#include "../../src/core/check.h"
#include "../../src/core/text.h"
#include "../../src/dat/dat_parts.h"
#include "small_dat.h"

typedef struct {
    int tech;
    DatChange change;
} GameChange;

static const DatTech GAME_TECH = {
    .required_techs = { -1, -1, -1, -1, -1, -1 },
    .costs = { { -1, 0, 0 }, { -1, 0, 0 }, { -1, 0, 0 } },
    .civ = NO_CIV,
    .effect_id = -1,
    .name = "Game",
    .research_location_count = 1,
    .research_locations = { { 109, 25, 6, 18022L } }
};

static const int GAME_TECH_CIVS[GAME_TECHS] = { NO_CIV,    SLOT,   SLOT,      SLOT,   OTHER_CIV, NO_CIV,
                                                OTHER_CIV, NO_CIV, OTHER_CIV, NO_CIV, NO_CIV };

static const GameChange GAME_CHANGES[GAME_CHANGE_COUNT] = {
    { 0, { RECIPE_DISABLE_TECH, -1, -1, -1, (float)LACKED_TECH } },
    { 0, { RECIPE_DISABLE_TECH, -1, -1, -1, (float)NOWHERE_TECH } },
    { 0, { RECIPE_UNLOCK_TECH, GENERIC_TECH, -1, -1, 0.0F } },
    { OLD_MAKE_TECH, { RECIPE_ENABLE_UNIT, OLD_UNIT, 1, -1, 0.0F } },
    { CASTLE_SOURCE, { ADD_ATTRIBUTE, OWN_UNIT, -1, 0, 10.0F } },
    { CASTLE_SOURCE, { ADD_ATTRIBUTE, OLD_UNIT, -1, 0, 10.0F } },
    { CASTLE_SOURCE, { RECIPE_ENABLE_UNIT, OLD_UNIT, 1, -1, 0.0F } },
    { CASTLE_SOURCE, { UPGRADE_UNIT, OLD_UNIT, ELITE_UNIT, -1, 0.0F } },
    { CASTLE_SOURCE, { SPAWN_UNIT, KEPT_UNIT, -1, -1, 0.0F } },
    { CASTLE_SOURCE, { TEAM_ENABLE_UNIT, KEPT_UNIT, -1, -1, 0.0F } },
    { CASTLE_SOURCE, { RESOURCE, 0, 1, -1, 50.0F } },
    { IMPERIAL_SOURCE, { ADD_ATTRIBUTE, OLD_UNIT, -1, 0, 20.0F } },
    { OWN_MAKE_TECH, { RECIPE_ENABLE_UNIT, OWN_UNIT, 1, -1, 0.0F } },
    { LACKED_TECH, { RECIPE_ENABLE_UNIT, LACKED_UNIT, 1, -1, 0.0F } },
    { BONUS_TECH, { DAT_CHANGE_TECH_COST, OWN_ELITE_TECH, WOOD, 1, -10.0F } },
    { BONUS_TECH, { ADD_ATTRIBUTE, -1, INFANTRY_CLASS, 0, 5.0F } },
    { NOWHERE_TECH, { RESOURCE, 0, 1, -1, 100.0F } },
    { OWN_ELITE_TECH, { UPGRADE_UNIT, OWN_UNIT, ELITE_UNIT, -1, 0.0F } },
    { TEAM_EFFECT, { DAT_CHANGE_TECH_COST, OWN_ELITE_TECH, WOOD, 1, -10.0F } },
    { TEAM_EFFECT, { ADD_ATTRIBUTE, -1, INFANTRY_CLASS, TEAM_ATTRIBUTE, 1.0F } }
};

static const DatChange GENERIC_CHANGE = { RECIPE_ENABLE_UNIT, KEPT_UNIT, 1, -1, 0.0F };

static Catalog game_catalog;
static CivDesign design;
static RecipeReport report;
static unsigned char rebuilt[SMALL_DAT_CAPACITY];
static DatBytes rebuilt_bytes;
static DatMap rebuilt_map;

static DatTech game_tech(int index)
{
    DatTech tech = GAME_TECH;

    if (!CHECK(index >= 0) || !CHECK(index < GAME_TECHS)) {
        return tech;
    }
    tech.civ = GAME_TECH_CIVS[index];
    if (index == NOWHERE_TECH) {
        tech.research_locations[0].location_id = DAT_NO_ID;
        (void)memset(tech.required_techs, 0, sizeof tech.required_techs);
    }
    if (index == BONUS_TECH) {
        tech.required_techs[0] = OWN_ELITE_TECH;
        tech.required_techs[1] = LACKED_TECH;
        tech.required_tech_count = 2;
    }
    return tech;
}

static bool add_game_tech(int index)
{
    DatTech tech = game_tech(index);
    int row;
    bool fine;

    if (!CHECK(index >= 0) || !CHECK(index < GAME_TECHS)) {
        return false;
    }
    tech.effect_id = dat_edit_add_effect(&small.edit, GAME_TECH.name);
    fine = (tech.effect_id == index) &&
           ((index != GENERIC_TECH) || dat_edit_add_change(&small.edit, &GENERIC_CHANGE));
    for (row = 0; fine && (row < GAME_CHANGE_COUNT); ++row) {
        if (GAME_CHANGES[row].tech == index) {
            fine = dat_edit_add_change(&small.edit, &GAME_CHANGES[row].change);
        }
    }
    return fine && (dat_edit_add_tech(&small.edit, &tech) == index);
}

static bool make_game(DatBytes *game)
{
    const SmallUnit building = small.unit;
    const SmallUnit soldier = { OWN_UNIT, INFANTRY_CLASS, SMALL_CREATABLE_TYPE, 0 };
    bool fine;
    int index;

    if (!CHECK(game != NULL) || !CHECK(building.index == 0)) {
        return false;
    }
    small.unit = soldier;
    small.resources = GAME_RESOURCES;
    fine = small_dat_start(GAME_CIVS);
    small.unit = building;
    small.resources = 0;
    for (index = 0; index < GAME_TECHS; ++index) {
        fine = fine && add_game_tech(index);
    }
    return fine && small_dat_write(game);
}

static void fill_bonuses(void)
{
    static const BonusChange CIV_CHANGE = { false, CHANGES_BONUS,  0, { { CASTLE_AGE }, 1 }, ADD_ATTRIBUTE,
                                            -1,    INFANTRY_CLASS, 0, 7.0F };
    static const BonusChange TEAM_CHANGE = { true, CHANGES_TEAM_BONUS, 0, { { 0 }, 0 }, ADD_ATTRIBUTE,
                                             -1,   INFANTRY_CLASS,     0, 3.0F };

    game_catalog.bonuses[0].id = TECH_BONUS;
    game_catalog.bonuses[0].effects = (TsvNumberList){ { BONUS_TECH }, 1 };
    game_catalog.bonuses[1].id = GENERIC_BONUS;
    game_catalog.bonuses[1].effects = (TsvNumberList){ { GENERIC_TECH }, 1 };
    game_catalog.bonuses[2].id = CHANGES_BONUS;
    game_catalog.bonuses[BUILT_BONUSES].id = EMPTY_BONUS;
    (void)text_copy(game_catalog.bonuses[0].description, CATALOG_TEXT_CAPACITY, "Infantry is stronger");
    game_catalog.bonuses[BUILT_BONUSES + 1].id = REQUIRE_BONUS;
    game_catalog.bonuses[BUILT_BONUSES + 2].id = NO_ROOM_BONUS;
    game_catalog.bonuses[BUILT_BONUSES + 3].id = MIXED_BONUS;
    game_catalog.bonuses[BUILT_BONUSES + 3].effects = (TsvNumberList){ { GENERIC_TECH }, 1 };
    game_catalog.bonus_count = BUILT_BONUSES + 4;
    game_catalog.team_bonuses[0].id = EFFECT_TEAM_BONUS;
    game_catalog.team_bonuses[0].effects = (TsvNumberList){ { TEAM_EFFECT }, 1 };
    game_catalog.team_bonuses[1].id = CHANGES_TEAM_BONUS;
    (void)text_copy(game_catalog.team_bonuses[1].description, CATALOG_TEXT_CAPACITY, "Allies are faster");
    game_catalog.team_bonuses[2].id = EMPTY_TEAM_BONUS;
    game_catalog.team_bonus_count = 3;
    game_catalog.bonus_changes[0] = CIV_CHANGE;
    game_catalog.bonus_changes[1] = TEAM_CHANGE;
    game_catalog.bonus_changes[2] =
        (BonusChange){ false, MIXED_BONUS, 0, { { 0 }, 0 }, ADD_ATTRIBUTE, -1, INFANTRY_CLASS, 0, 9.0F };
    game_catalog.bonus_change_count = 3;
    game_catalog.requirements[0] = (BonusRequirement){ REQUIRE_BONUS, GENERIC_TECH, LACKED_TECH };
    game_catalog.requirements[1] = (BonusRequirement){ REQUIRE_BONUS, BONUS_TECH, LACKED_TECH };
    game_catalog.requirements[2] = (BonusRequirement){ NO_ROOM_BONUS, NOWHERE_TECH, LACKED_TECH };
    game_catalog.requirement_count = 3;
    (void)CHECK(game_catalog.bonus_count <= CATALOG_BONUS_CAPACITY);
}

static void fill_catalog(void)
{
    static const UnitEntry OLD = { UNIT_ROLE_VANILLA, OLD_PICK, "Old", { 0 }, OLD_MAKE_TECH, DAT_NO_ID };
    static const UnitEntry OWN = { UNIT_ROLE_VANILLA, OWN_PICK, "Own", { 60, 0, 0, 20 }, OWN_MAKE_TECH,
                                   OWN_ELITE_TECH };
    static const UnitEntry HERO = { UNIT_ROLE_HERO, OWN_UNIT, "Champion", { 0 }, DAT_NO_ID, DAT_NO_ID };
    static const AppearanceEntry CIV = { APPEARANCE_CIV, SLOT, "Slot", { 1, 2, UNIT_SID, ELITE_SID, HERO_SID },
                                         0,              { 0, 0, 0 } };

    (void)memset(&game_catalog, 0, sizeof game_catalog);
    game_catalog.units[0] = OLD;
    game_catalog.units[1] = OWN;
    game_catalog.units[2] = HERO;
    game_catalog.unit_count = 3;
    game_catalog.techs[0].tier = TECH_TIER_CASTLE;
    game_catalog.techs[0].effects = (TsvNumberList){ { CASTLE_SOURCE }, 1 };
    game_catalog.techs[1].tier = TECH_TIER_IMPERIAL;
    game_catalog.techs[1].effects = (TsvNumberList){ { IMPERIAL_SOURCE }, 1 };
    (void)text_copy(game_catalog.techs[0].name, CATALOG_NAME_CAPACITY, "Bulwark");
    (void)text_copy(game_catalog.techs[0].description, CATALOG_TEXT_CAPACITY, "Walls are stronger.");
    game_catalog.tech_count = 2;
    game_catalog.tree[0].id = LACKED_UNIT;
    game_catalog.tree[1].id = KEPT_UNIT;
    game_catalog.tree_count = 2;
    game_catalog.appearance[0] = CIV;
    game_catalog.appearance[1].kind = APPEARANCE_ARCHITECTURE;
    game_catalog.appearance[1].id = CIV_DEFAULT_ARCHITECTURE;
    game_catalog.appearance_count = 2;
    fill_bonuses();
    (void)CHECK(game_catalog.tree[0].kind == TREE_UNITS);
}

static bool fill_full_design(void)
{
    design.techs[TECH_TIER_CASTLE].cost[WOOD] = CHOSEN_WOOD;
    return CHECK(civ_design_set_text(&design, TEXT_FIELD_UNIT_NAME, "Tester")) &&
           CHECK(civ_design_pick_unit(&design, UNIT_ORIGIN_VANILLA, OWN_PICK)) &&
           CHECK(civ_design_pick_tech(&design, TECH_TIER_CASTLE, PICKED_TECH)) &&
           CHECK(civ_design_pick_tech(&design, TECH_TIER_IMPERIAL, PICKED_TECH)) &&
           CHECK(civ_design_toggle_tree(&design, TREE_UNITS, KEPT_UNIT)) &&
           CHECK(civ_design_toggle_bonus(&design, TECH_BONUS)) &&
           CHECK(civ_design_toggle_bonus(&design, GENERIC_BONUS)) &&
           CHECK(civ_design_toggle_bonus(&design, CHANGES_BONUS)) &&
           CHECK(civ_design_toggle_bonus(&design, MISSING_BONUS)) &&
           CHECK(civ_design_toggle_team_bonus(&design, EFFECT_TEAM_BONUS));
}

static bool fill_bare_design(void)
{
    return CHECK(civ_design_set_text(&design, TEXT_FIELD_HERO_NAME, "Hero")) &&
           CHECK(civ_design_pick_unit(&design, UNIT_ORIGIN_VANILLA, MISSING_PICK)) &&
           CHECK(civ_design_pick_tech(&design, TECH_TIER_CASTLE, MISSING_PICK)) &&
           CHECK(civ_design_pick_tech(&design, TECH_TIER_IMPERIAL, MISSING_PICK)) &&
           CHECK(civ_design_pick_hero(&design, OWN_UNIT)) &&
           CHECK(civ_design_toggle_team_bonus(&design, CHANGES_TEAM_BONUS));
}

static bool fill_multiplied_design(bool bare)
{
    bool filled = bare ? (fill_bare_design() && civ_design_toggle_bonus(&design, MIXED_BONUS)) : fill_full_design();

    civ_design_set_multiplier(&design, false, bare ? MIXED_BONUS : TECH_BONUS, 2);
    civ_design_set_multiplier(&design, true, bare ? CHANGES_TEAM_BONUS : EFFECT_TEAM_BONUS, 2);
    return filled;
}

static bool fill_design(Scenario scenario)
{
    civ_design_reset(&design);
    if (!CHECK(civ_design_set_text(&design, TEXT_FIELD_NAME, "Testonia")) ||
        !CHECK(civ_design_set_choice(&design, CHOICE_REPLACE, SLOT))) {
        return false;
    }
    switch (scenario) {
    case BARE_CIV:
        return fill_bare_design();
    case LOST_HERO_CIV:
        return fill_bare_design() && CHECK(civ_design_pick_hero(&design, KEPT_UNIT));
    case MULTIPLIED_CIV:
        return fill_multiplied_design(false);
    case MULTIPLIED_BARE_CIV:
        return fill_multiplied_design(true);
    case REQUIRED_CIV:
        return fill_bare_design() && CHECK(civ_design_toggle_bonus(&design, REQUIRE_BONUS)) &&
               CHECK(civ_design_toggle_bonus(&design, NO_ROOM_BONUS));
    case EMPTY_CIV:
        return fill_bare_design() && CHECK(civ_design_toggle_team_bonus(&design, EMPTY_TEAM_BONUS)) &&
               CHECK(civ_design_toggle_bonus(&design, EMPTY_BONUS));
    case SCOUT_CIV:
        return fill_full_design() && CHECK(civ_design_set_choice(&design, CHOICE_SCOUT, SCOUT));
    case FULL_CIV:
    default:
        return fill_full_design();
    }
}

bool recipe_game_build(Scenario scenario)
{
    DatBytes game;
    DatOutput output = { rebuilt, sizeof rebuilt, 0U };
    RecipeInput input = { &design, &game_catalog, &game, &small.written_map, SLOT };

    fill_catalog();
    if (!fill_design(scenario) || !make_game(&game) || !CHECK(dat_edit_start(&small.edit, &small.written_map)) ||
        !recipe_apply(&input, &small.edit, &report) || !CHECK(dat_edit_write(&small.edit, &game, &output))) {
        return false;
    }
    rebuilt_bytes.data = rebuilt;
    rebuilt_bytes.size = output.length;
    return dat_map_read(rebuilt, output.length, &rebuilt_map);
}

static DatTech scenario_tech(Scenario scenario, int id)
{
    DatTech tech = GAME_TECH;

    tech.civ = NO_TECH;
    if (!CHECK(id >= 0) || !recipe_game_build(scenario) || (id >= rebuilt_map.tech_count)) {
        return tech;
    }
    (void)CHECK(dat_tech_read(&rebuilt_bytes, rebuilt_map.tech_at[id], &tech));
    return tech;
}

DatTech recipe_game_tech(int id)
{
    return scenario_tech(FULL_CIV, id);
}

static bool is_wanted(const DatChange *change, int type, int key)
{
    if (!CHECK(change != NULL) || (type < 0)) {
        return true;
    }
    return (change->type == type) && (((type == RECIPE_DISABLE_TECH) ? (int)change->d : change->a) == key);
}

static int changes_of(int effect_id, int type, int key, DatChange *last)
{
    DatEffect effect;
    DatChange change;
    int count = 0;
    int index;

    if (!CHECK(last != NULL) || (effect_id < 0) || (effect_id >= rebuilt_map.effect_count) ||
        !dat_effect_read(&rebuilt_bytes, rebuilt_map.effect_at[effect_id], &effect)) {
        return NO_TECH;
    }
    for (index = 0; (index < effect.change_count) && (index < RECIPE_TREE_CAPACITY); ++index) {
        if (dat_change_read(&rebuilt_bytes, effect.changes_at + ((size_t)index * DAT_CHANGE_SIZE), &change) &&
            is_wanted(&change, type, key)) {
            *last = change;
            ++count;
        }
    }
    return count;
}

static int civ_effect(Scenario scenario, bool team)
{
    DatCiv civ;

    if (!recipe_game_build(scenario) || !CHECK(dat_civ_read(&rebuilt_bytes, rebuilt_map.civ_at[SLOT], &civ))) {
        return DAT_NO_ID;
    }
    return team ? civ.team_bonus_id : civ.tech_tree_id;
}

int recipe_game_scenario_changes(Scenario scenario, int tech_id, int type, int key)
{
    DatChange last;

    return changes_of(scenario_tech(scenario, tech_id).effect_id, type, key, &last);
}

int recipe_game_changes(int tech_id, int type, int key)
{
    return recipe_game_scenario_changes(FULL_CIV, tech_id, type, key);
}

int recipe_game_tree_changes(Scenario scenario, int type, int key)
{
    DatChange last;

    return changes_of(civ_effect(scenario, false), type, key, &last);
}

static DatChange last_of(int effect_id, int type, int key)
{
    DatChange last = { NO_TECH, NO_TECH, NO_TECH, NO_TECH, 0.0F };

    (void)changes_of(effect_id, type, key, &last);
    return last;
}

DatChange recipe_game_tree_last(Scenario scenario, int type, int key)
{
    return last_of(civ_effect(scenario, false), type, key);
}

long recipe_game_unit_name(Scenario scenario)
{
    DatCursor cursor;
    size_t at = 0U;

    if (!recipe_game_build(scenario) || !dat_find_unit(&rebuilt_bytes, rebuilt_map.civ_at[SLOT], OWN_UNIT, &at) ||
        !dat_cursor_start(&cursor, &rebuilt_bytes, at + DAT_UNIT_NAME_AT)) {
        return DAT_NO_ID;
    }
    return dat_cursor_i32(&cursor);
}

DatChange recipe_game_team_last(Scenario scenario)
{
    return last_of(civ_effect(scenario, true), ANY_CHANGE, 0);
}

DatChange recipe_game_tech_last(int tech_id)
{
    return last_of(recipe_game_tech(tech_id).effect_id, ANY_CHANGE, 0);
}

int recipe_game_required_tech(Scenario scenario, int tech_id, int slot)
{
    DatTech tech = scenario_tech(scenario, tech_id);

    if (!CHECK(slot >= 0) || !CHECK(slot < DAT_REQUIRED_TECH_SLOTS) || (tech.civ == NO_TECH)) {
        return NO_TECH;
    }
    return tech.required_techs[slot];
}

float recipe_game_scenario_amount(Scenario scenario, int tech_id)
{
    return last_of(scenario_tech(scenario, tech_id).effect_id, ANY_CHANGE, 0).d;
}

int recipe_game_tech_civ(int tech_id)
{
    DatTech tech;

    if ((tech_id < 0) || (tech_id >= rebuilt_map.tech_count) ||
        !CHECK(dat_tech_read(&rebuilt_bytes, rebuilt_map.tech_at[tech_id], &tech))) {
        return NO_TECH;
    }
    return tech.civ;
}

int recipe_game_team_changes(int type, int key)
{
    DatChange last;

    return changes_of(civ_effect(FULL_CIV, true), type, key, &last);
}

RecipeReport recipe_game_report(Scenario scenario)
{
    const RecipeReport failed = { NO_TECH, NO_TECH, NO_TECH, NO_TECH, NO_TECH, NO_TECH, NO_TECH, NO_TECH, NO_TECH,
                                  false,   false,   false,   { NO_TECH, NO_TECH } };

    return recipe_game_build(scenario) ? report : failed;
}

long recipe_game_resource(Scenario scenario, int resource)
{
    DatCursor cursor;
    DatCiv civ;
    float value;

    if (!CHECK(resource >= 0) || !CHECK(resource < GAME_RESOURCES) || !recipe_game_build(scenario) ||
        !dat_civ_read(&rebuilt_bytes, rebuilt_map.civ_at[SLOT], &civ)) {
        return DAT_NO_ID;
    }
    if (!dat_cursor_start(&cursor, &rebuilt_bytes,
                          civ.team_bonus_id_at + DAT_SHORT_WIDTH + ((size_t)resource * DAT_LONG_WIDTH))) {
        return DAT_NO_ID;
    }
    value = dat_cursor_f32(&cursor);
    return (long)value;
}

const CivDesign *recipe_game_design(Scenario scenario)
{
    (void)CHECK(fill_design(scenario));
    return &design;
}

const Catalog *recipe_game_catalog(void)
{
    fill_catalog();
    (void)CHECK(game_catalog.unit_count > 0);
    return &game_catalog;
}

bool recipe_game_make(DatBytes *game)
{
    if (!CHECK(game != NULL) || !CHECK(GAME_TECHS > 0)) {
        return false;
    }
    return make_game(game);
}
