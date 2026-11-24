#include "stat_build_tests.h"

#include <math.h>

#include "../../../src/core/text.h"
#include "../../../src/dat/dat_parts.h"
#include "../../../src/mod/mod_text.h"
#include "../../../src/present/summary.h"
#include "../../../src/unique/unique_aim.h"
#include "../../dat/recipe_tests.h"
#include "../../dat/small_dat.h"

enum { MELEE_CLASS = 4, PIERCE_CLASS = 3, BASE_HP = 45, ELITE_HP = 60, CHANGED_HP = 100, COST_TYPE_PAID = 1 };

enum { UNPAID = 0, ONE = 1, STAT_BUILD_FIRST_COUNT = 14 };

static const double HUNDRED = 100.0;
static const double CLOSE = 0.001;
static const double EFFECT_TWICE = 18.0;

static DatUnit unit;
static const DatBytes *bytes;
static SummaryText summary;
static CivDesign own;
static unsigned char own_data[SMALL_DAT_CAPACITY];
static DatBytes rebuilt_bytes;
static DatMap rebuilt_map;
static RecipeReport own_report;

static bool own_rebuilt(void)
{
    DatBytes game;
    DatOutput output = { own_data, sizeof own_data, 0U };
    RecipeInput input = { &own, recipe_game_catalog(), &game, &small.written_map, SLOT };
    bool fine;

    small.elite = ELITE_UNIT;
    fine = recipe_game_make(&game) && dat_edit_start(&small.edit, &small.written_map) &&
           recipe_apply(&input, &small.edit, &own_report) && dat_edit_write(&small.edit, &game, &output);
    small.elite = 0;
    rebuilt_bytes.data = own_data;
    rebuilt_bytes.size = output.length;
    bytes = &rebuilt_bytes;
    return fine && dat_map_read(own_data, output.length, &rebuilt_map);
}

static bool stat_built(UnitStat stat, int value, int unit_id)
{
    size_t at = 0U;

    own = *recipe_game_design(FULL_CIV);
    return civ_design_set_unit_stat(&own, UNIT_RANK_BASE, stat, value) && own_rebuilt() &&
           dat_find_unit(bytes, rebuilt_map.civ_at[SLOT], unit_id, &at) && dat_unit_read(bytes, at, &unit);
}

static bool own_castle_tech(DatTech *tech)
{
    RecipeInput input = { &own, recipe_game_catalog(), &rebuilt_bytes, &rebuilt_map, SLOT };

    return own_rebuilt() && recipe_read_tech(&input, own_report.unique_tech_ids[TECH_TIER_CASTLE], tech);
}

static int castle_changes(void)
{
    RecipeInput input = { &own, recipe_game_catalog(), &rebuilt_bytes, &rebuilt_map, SLOT };
    DatTech tech;
    DatEffect effect;

    return (own_castle_tech(&tech) && recipe_read_effect(&input, tech.effect_id, &effect)) ? effect.change_count : 0;
}

static bool built_unit(Scenario scenario)
{
    RebuiltGame rebuilt = recipe_game_rebuilt(scenario);
    size_t at = 0U;

    bytes = rebuilt.bytes;
    return (bytes != NULL) && dat_find_unit(bytes, rebuilt.map->civ_at[SLOT], OWN_UNIT, &at) &&
           dat_unit_read(bytes, at, &unit);
}

static int short_at(size_t at)
{
    DatCursor cursor;

    return dat_cursor_start(&cursor, bytes, at) ? dat_cursor_i16(&cursor) : DAT_NO_ID;
}

static bool float_is(size_t at, double value)
{
    DatCursor cursor;

    return dat_cursor_start(&cursor, bytes, at) && (fabs((double)dat_cursor_f32(&cursor) - value) < CLOSE);
}

static int entry_of(size_t list_at, int count, int wanted)
{
    int index;

    for (index = 0; index < count; ++index) {
        if (short_at(list_at + ((size_t)index * DAT_ATTACK_SIZE)) == wanted) {
            return short_at(list_at + ((size_t)index * DAT_ATTACK_SIZE) + DAT_SHORT_WIDTH);
        }
    }
    return DAT_NO_ID;
}

static bool hp_and_speed_built(void)
{
    return built_unit(STATS_CIV) && (short_at(unit.at + DAT_UNIT_HIT_POINTS_AT) == BUILT_HP) &&
           float_is(unit.speed_at, (double)BUILT_SPEED / HUNDRED);
}

static bool attack_built(void)
{
    return built_unit(STATS_CIV) && (entry_of(unit.attacks_at, unit.attack_count, MELEE_CLASS) == BUILT_ATTACK) &&
           (short_at(unit.combat_at + DAT_COMBAT_SHOWN_ATTACK_AT) == BUILT_ATTACK);
}

static bool armours_built(void)
{
    return built_unit(STATS_CIV) &&
           (entry_of(unit.armours_at, unit.armour_count, MELEE_CLASS) == BUILT_MELEE_ARMOR) &&
           (entry_of(unit.armours_at, unit.armour_count, PIERCE_CLASS) == BUILT_PIERCE_ARMOR) &&
           (short_at(unit.combat_at + DAT_COMBAT_SHOWN_MELEE_ARMOR_AT) == BUILT_MELEE_ARMOR) &&
           (short_at(unit.shown_pierce_at) == BUILT_PIERCE_ARMOR);
}

static bool range_and_reload_built(void)
{
    return built_unit(STATS_CIV) && float_is(unit.combat_at + DAT_COMBAT_RANGE_AT, (double)BUILT_RANGE) &&
           float_is(unit.combat_at + DAT_COMBAT_SHOWN_RANGE_AT, (double)BUILT_RANGE) &&
           float_is(unit.combat_at + DAT_COMBAT_RELOAD_AT, (double)BUILT_RELOAD / HUNDRED) &&
           float_is(unit.combat_at + DAT_COMBAT_SHOWN_RELOAD_AT, (double)BUILT_RELOAD / HUNDRED);
}

static bool slot_is(int slot, int type, int amount, int flag)
{
    size_t at = unit.costs_at + ((size_t)slot * DAT_UNIT_COST_SIZE);

    return (short_at(at) == type) && (short_at(at + DAT_SHORT_WIDTH) == amount) &&
           (short_at(at + (2U * DAT_SHORT_WIDTH)) == flag);
}

static bool costs_built(void)
{
    return built_unit(STATS_CIV) && slot_is(0, UNIT_STAT_FOOD, BUILT_FOOD, COST_TYPE_PAID) &&
           slot_is(1, UNIT_STAT_WOOD, BUILT_WOOD, COST_TYPE_PAID) &&
           slot_is(2, UNIT_STAT_STONE, BUILT_STONE, COST_TYPE_PAID);
}

static bool elite_hp_built(void)
{
    return stat_built(UNIT_STAT_HP, CHANGED_HP, ELITE_UNIT) &&
           (short_at(unit.at + DAT_UNIT_HIT_POINTS_AT) == SMALL_ELITE_HP + CHANGED_HP);
}

static bool elite_reload_built(void)
{
    return stat_built(UNIT_STAT_RELOAD, BUILT_RELOAD, ELITE_UNIT) &&
           float_is(unit.combat_at + DAT_COMBAT_RELOAD_AT, ((SMALL_ELITE_RELOAD * HUNDRED) + BUILT_RELOAD) / HUNDRED);
}

static bool hp_built(int value)
{
    return stat_built(UNIT_STAT_HP, value, OWN_UNIT) && (short_at(unit.at + DAT_UNIT_HIT_POINTS_AT) == value);
}

static bool population_kept(void)
{
    return stat_built(UNIT_STAT_GOLD, BUILT_GOLD, OWN_UNIT) &&
           slot_is(0, UNIT_STAT_FOOD, SMALL_FOOD, COST_TYPE_PAID) &&
           slot_is(1, UNIT_STAT_GOLD, BUILT_GOLD, COST_TYPE_PAID) && slot_is(2, SMALL_POPULATION, ONE, UNPAID);
}

static bool tech_cost_of_one_built(void)
{
    DatTech tech;

    own = *recipe_game_design(FULL_CIV);
    return civ_design_set_tech_value(&own, TECH_TIER_CASTLE, TECH_VALUE_FOOD, ONE) && own_castle_tech(&tech) &&
           (tech.costs[0].type == (int)TECH_VALUE_FOOD) && (tech.costs[0].amount == ONE);
}

static bool unticked_effect_not_built(void)
{
    int changes;

    own = *recipe_game_design(FULL_CIV);
    changes = civ_design_toggle_tech_effect(&own, TECH_TIER_CASTLE, MIXED_BONUS) ? castle_changes() : 0;
    return (changes > 0) && civ_design_toggle_tech_effect(&own, TECH_TIER_CASTLE, TECH_BONUS) &&
           civ_design_toggle_tech_effect(&own, TECH_TIER_CASTLE, TECH_BONUS) && (castle_changes() == changes);
}

static bool effects_described_again(void)
{
    ModCiv civ = { recipe_game_design(STATS_CIV), recipe_game_catalog(), SLOT, 0L, false };

    (void)mod_tech_text(&civ, TECH_TIER_CASTLE, false);
    return text_equal(mod_tech_text(&civ, TECH_TIER_CASTLE, false), "Infantry +18 armor");
}

static bool elite_takes_the_change(void)
{
    CivDesign design;
    const int now[UNIT_RANK_COUNT] = { BASE_HP, ELITE_HP };
    int wanted = 0;

    civ_design_reset(&design);
    return civ_design_pick_unit(&design, UNIT_ORIGIN_VANILLA, OWN_PICK) &&
           civ_design_set_unit_stat(&design, UNIT_RANK_BASE, UNIT_STAT_HP, CHANGED_HP) &&
           unique_stat_wanted(&design.unique_unit, UNIT_RANK_ELITE, UNIT_STAT_HP, now, &wanted) &&
           (wanted == ELITE_HP + (CHANGED_HP - BASE_HP));
}

static BuiltCivs skipped_stat(void)
{
    static RecipeReport report;
    BuiltCivs built = { { NULL }, &report, 1 };

    report = recipe_game_report(COSTLY_CIV);
    built.designs[0] = recipe_game_design(COSTLY_CIV);
    return built;
}

static bool skip_is_told(void)
{
    BuiltCivs built = skipped_stat();

    return summary_write_build(&summary, "mod", &built) &&
           text_contains_folded(summary.text, "A changed stat or cost of the unique unit is not built.");
}

static bool skip_unmakes_whole(void)
{
    RecipeReport report = { 2, 0, 0, 1, 0, 2, 2, 0, 0, 1, false, false, false, { 0, 0 } };
    BuiltCivs built = { { NULL }, &report, 1 };

    return !summary_build_is_whole(&built);
}

static bool effects_described(void)
{
    ModCiv civ = { recipe_game_design(STATS_CIV), recipe_game_catalog(), SLOT, 0L, false };

    return text_equal(mod_tech_text(&civ, TECH_TIER_CASTLE, false), "Infantry +18 armor");
}

static bool game_techs_copied(void)
{
    return recipe_game_scenario_changes(STATS_CIV, NEW_CASTLE_TECH, RECIPE_ENABLE_UNIT, KEPT_UNIT) == 1;
}

static bool games_cost_kept(void)
{
    DatTech tech = recipe_game_tech(NEW_CASTLE_TECH);

    return (tech.costs[1].type == GOLD) && (tech.costs[1].amount == GAME_GOLD);
}

static TestOutcome stat_build_rest_run(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("the elite unit is built with the change made to the unit's hit points, from its own",
                               elite_hp_built());
    case 2:
        return test_outcome_of("the elite unit is built with the change made to the unit's reload time, from its own",
                               elite_reload_built());
    case 3:
        return test_outcome_of("the attack set of a unit with an attack bonus is built into its attack",
                               stat_built(UNIT_STAT_ATTACK, BUILT_ATTACK, OWN_UNIT) &&
                                   (entry_of(unit.attacks_at, unit.attack_count, MELEE_CLASS) == BUILT_ATTACK));
    case 4:
        return test_outcome_of("hit points set to the least of their range are built",
                               hp_built(civ_stat_range(UNIT_STAT_HP).least));
    case 5:
        return test_outcome_of("hit points set to the most of their range are built",
                               hp_built(civ_stat_range(UNIT_STAT_HP).most));
    case 6:
        return test_outcome_of("a cost set of the unique unit keeps its other costs and its population",
                               population_kept());
    case 7:
        return test_outcome_of("a food cost of 1 set alone of the unique unit is built",
                               stat_built(UNIT_STAT_FOOD, ONE, OWN_UNIT) &&
                                   slot_is(0, UNIT_STAT_FOOD, ONE, COST_TYPE_PAID));
    case 8:
        return test_outcome_of("a cost set to 0 is taken out of the costs of the unique unit",
                               stat_built(UNIT_STAT_GOLD, 0, OWN_UNIT) && slot_is(2, DAT_NO_ID, 0, UNPAID));
    case 9:
        return test_outcome_of("a cost of 1 of a unique tech is built", tech_cost_of_one_built());
    case 10:
        return test_outcome_of("an effect ticked and unticked again is not built into the unique tech",
                               unticked_effect_not_built());
    case 11:
        return test_outcome_of("a build whose unique unit is not in the catalog leaves out no stat",
                               recipe_game_report(BARE_CIV).unit_stats_skipped == 0);
    case 12:
        return test_outcome_of("the mod describes a unique tech with effects alike when it is built again",
                               effects_described_again());
    default:
        return unit_number_tests_run(index - (STAT_BUILD_OWN_COUNT - STAT_BUILD_FIRST_COUNT));
    }
}

TestOutcome stat_build_tests_run(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("the hit points and speed set of the unique unit are built", hp_and_speed_built());
    case 2:
        return test_outcome_of("the attack set of the unique unit is built", attack_built());
    case 3:
        return test_outcome_of("the armours set of the unique unit are built", armours_built());
    case 4:
        return test_outcome_of("the range and reload time set of the unique unit are built",
                               range_and_reload_built());
    case 5:
        return test_outcome_of("the costs set of the unique unit are built", costs_built());
    case 6:
        return test_outcome_of("a cost of four resources is counted as not built",
                               recipe_game_report(COSTLY_CIV).unit_stats_skipped == 1);
    case 7:
        return test_outcome_of("the elite unit takes the change made to the unit", elite_takes_the_change());
    case 8:
        return test_outcome_of("a build says that a stat of the unique unit was not built", skip_is_told());
    case 9:
        return test_outcome_of("a build that left out a stat of the unique unit is not whole", skip_unmakes_whole());
    case 10:
        return test_outcome_of("a cost not set of a unique tech is built as the game's", games_cost_kept());
    case 11:
        return test_outcome_of("the research time set of a unique tech is built",
                               recipe_game_tech(NEW_CASTLE_TECH).research_locations[0].research_time == CHOSEN_TIME);
    case 12:
        return test_outcome_of("an effect of a unique tech is built into what the tech does, by its multiplier",
                               fabs((double)recipe_game_scenario_amount(STATS_CIV, NEW_CASTLE_TECH) - EFFECT_TWICE) <
                                   CLOSE);
    case 13:
        return test_outcome_of("the mod describes a unique tech with effects by its effects as they count",
                               effects_described());
    case 14:
        return test_outcome_of("an effect of a unique tech takes the changes of the game techs of its bonus",
                               game_techs_copied());
    default:
        return stat_build_rest_run(index - STAT_BUILD_FIRST_COUNT);
    }
}
