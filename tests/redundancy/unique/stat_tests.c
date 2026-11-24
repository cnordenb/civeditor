#include "stat_tests.h"

#include "../../../src/core/text.h"
#include "../../../src/model/history.h"
#include "../../../src/persist/civ_file.h"

enum {
    LONGBOWMAN = 8,
    MANGUDAI = 11,
    HIT_POINTS = 100,
    SPEED = 110,
    ELITE_HIT_POINTS = 110,
    YEOMEN = 3,
    WARWOLF = 4,
    WOOD_COST = 600,
    FOOD_COST = 45,
    EFFECT = 81,
    TWICE = 2000000
};

static const wchar_t EXAMPLE_PATH[] = L"tests/fixtures/example.civbuilder.json";
static const char ZERO_COST[] = "{\"format\": \"empireforge_v2\", \"imperial_ut\": {\"vanilla_km_idx\": 3, "
                                "\"cost\": {\"food\": 0, \"wood\": 0, \"stone\": 0, \"gold\": 0}, \"time\": 0}}";

static History history;
static CivDesign reloaded;
static JsonWriter written;
static JsonDocument document;

static bool picked_with(UnitStat stat, int value)
{
    Command pick = command_pick(COMMAND_PICK_UNIT, (int)UNIT_ORIGIN_VANILLA, LONGBOWMAN, "Longbowman");
    Command set = command_of(COMMAND_SET_UNIT_STAT, (int)stat, value);

    history_start(&history);
    return history_do(&history, &pick) && history_do(&history, &set);
}

static Override base_of(const CivDesign *design, UnitStat stat)
{
    return design->unique_unit.stats[UNIT_RANK_BASE][stat];
}

static bool stat_is_set(void)
{
    return picked_with(UNIT_STAT_HP, HIT_POINTS) && base_of(&history.present, UNIT_STAT_HP).set &&
           (base_of(&history.present, UNIT_STAT_HP).value == HIT_POINTS);
}

static bool stat_is_undone(void)
{
    return picked_with(UNIT_STAT_HP, HIT_POINTS) && history_step(&history, true) &&
           !base_of(&history.present, UNIT_STAT_HP).set;
}

static bool stat_goes_with_its_unit(void)
{
    Command other = command_pick(COMMAND_PICK_UNIT, (int)UNIT_ORIGIN_VANILLA, MANGUDAI, "Mangudai");

    return picked_with(UNIT_STAT_HP, HIT_POINTS) && history_do(&history, &other) &&
           !base_of(&history.present, UNIT_STAT_HP).set;
}

static bool stat_out_of_range_is_refused(void)
{
    Command set = command_of(COMMAND_SET_UNIT_STAT, (int)UNIT_STAT_HP, 0);

    return picked_with(UNIT_STAT_SPEED, SPEED) && !history_do(&history, &set);
}

static bool reopened(const CivDesign *design)
{
    if (!civ_file_compose(design, &written)) {
        return false;
    }
    document.length = (int)written.length;
    return text_copy(document.text, sizeof document.text, written.data) && json_parse(&document) &&
           civ_file_interpret(&document, &reloaded);
}

static bool stat_is_saved_and_opened(void)
{
    return picked_with(UNIT_STAT_SPEED, SPEED) && reopened(&history.present) &&
           (base_of(&reloaded, UNIT_STAT_SPEED).value == SPEED);
}

static bool original_stats_are_read(void)
{
    return civ_file_load(&reloaded, EXAMPLE_PATH) && (base_of(&reloaded, UNIT_STAT_SPEED).value == SPEED) &&
           (reloaded.unique_unit.stats[UNIT_RANK_ELITE][UNIT_STAT_HP].value == ELITE_HIT_POINTS);
}

static bool stat_changes_the_civ(void)
{
    Command set = command_of(COMMAND_SET_UNIT_STAT, (int)UNIT_STAT_HP, HIT_POINTS);

    if (!picked_with(UNIT_STAT_SPEED, SPEED)) {
        return false;
    }
    reloaded = history.present;
    return history_do(&history, &set) && !civ_design_same(&history.present, &reloaded);
}

static bool cost_is_saved_and_opened(void)
{
    return picked_with(UNIT_STAT_FOOD, FOOD_COST) && reopened(&history.present) &&
           (base_of(&reloaded, UNIT_STAT_FOOD).value == FOOD_COST);
}

static bool same_unit_keeps_stats(void)
{
    Command same = command_pick(COMMAND_PICK_UNIT, (int)UNIT_ORIGIN_VANILLA, LONGBOWMAN, "Longbowman");

    if (!picked_with(UNIT_STAT_HP, HIT_POINTS)) {
        return false;
    }
    (void)history_do(&history, &same);
    return base_of(&history.present, UNIT_STAT_HP).set;
}

static bool cost_changes_the_civ(void)
{
    Command set = command_of(COMMAND_SET_UNIT_STAT, (int)UNIT_STAT_FOOD, FOOD_COST);

    if (!picked_with(UNIT_STAT_SPEED, SPEED)) {
        return false;
    }
    reloaded = history.present;
    return history_do(&history, &set) && !civ_design_same(&history.present, &reloaded);
}

static bool tech_picked_with(TechValue value, int amount)
{
    Command pick = command_pick(COMMAND_PICK_TECH, (int)TECH_TIER_IMPERIAL, YEOMEN, "Yeomen");
    Command set = command_set_tech_value(TECH_TIER_IMPERIAL, value, amount);

    history_start(&history);
    return history_do(&history, &pick) && history_do(&history, &set);
}

static Override tech_value_of(const CivDesign *design, TechValue value)
{
    return design->techs[TECH_TIER_IMPERIAL].values[value];
}

static bool tech_value_is_set(void)
{
    return tech_picked_with(TECH_VALUE_WOOD, WOOD_COST) && tech_value_of(&history.present, TECH_VALUE_WOOD).set &&
           (tech_value_of(&history.present, TECH_VALUE_WOOD).value == WOOD_COST);
}

static bool tech_value_goes_with_its_tech(void)
{
    Command other = command_pick(COMMAND_PICK_TECH, (int)TECH_TIER_IMPERIAL, WARWOLF, "Warwolf");

    return tech_picked_with(TECH_VALUE_WOOD, WOOD_COST) && history_do(&history, &other) &&
           !tech_value_of(&history.present, TECH_VALUE_WOOD).set;
}

static bool tech_value_out_of_range_is_refused(void)
{
    Command set = command_set_tech_value(TECH_TIER_IMPERIAL, TECH_VALUE_TIME, 0);

    return tech_picked_with(TECH_VALUE_WOOD, WOOD_COST) && !history_do(&history, &set);
}

static bool tech_value_is_saved_and_opened(void)
{
    return tech_picked_with(TECH_VALUE_WOOD, WOOD_COST) && reopened(&history.present) &&
           (tech_value_of(&reloaded, TECH_VALUE_WOOD).value == WOOD_COST);
}

static bool tech_value_not_set_stays_so(void)
{
    return tech_picked_with(TECH_VALUE_WOOD, WOOD_COST) && reopened(&history.present) &&
           !tech_value_of(&reloaded, TECH_VALUE_FOOD).set;
}

static bool free_cost_is_saved_and_opened(void)
{
    return tech_picked_with(TECH_VALUE_FOOD, 0) && reopened(&history.present) &&
           tech_value_of(&reloaded, TECH_VALUE_FOOD).set;
}

static bool zero_cost_is_the_games(void)
{
    document.length = (int)text_length(ZERO_COST, sizeof ZERO_COST);
    return text_copy(document.text, sizeof document.text, ZERO_COST) && json_parse(&document) &&
           civ_file_interpret(&document, &reloaded) && (reloaded.techs[TECH_TIER_IMPERIAL].km_index == YEOMEN) &&
           !tech_value_of(&reloaded, TECH_VALUE_WOOD).set;
}

static bool tech_given_effect(void)
{
    Command pick = command_pick(COMMAND_PICK_TECH, (int)TECH_TIER_IMPERIAL, YEOMEN, "Yeomen");
    Command tick = command_toggle_tech_effect(TECH_TIER_IMPERIAL, EFFECT);
    Command twice = command_set_tech_effect(TECH_TIER_IMPERIAL, EFFECT, TWICE);

    history_start(&history);
    return history_do(&history, &pick) && history_do(&history, &tick) && history_do(&history, &twice);
}

static BonusPick first_effect(const CivDesign *design)
{
    return design->techs[TECH_TIER_IMPERIAL].effects[0];
}

static bool effect_is_kept(void)
{
    return tech_given_effect() && (history.present.techs[TECH_TIER_IMPERIAL].effect_count == 1) &&
           (first_effect(&history.present).id == EFFECT);
}

static bool effect_multiplier_is_set(void)
{
    return tech_given_effect() && (first_effect(&history.present).multiplier.millionths == TWICE);
}

static bool effect_is_saved_and_opened(void)
{
    return tech_given_effect() && reopened(&history.present) && (first_effect(&reloaded).id == EFFECT) &&
           (first_effect(&reloaded).multiplier.millionths == TWICE);
}

static bool effect_makes_custom(void)
{
    return tech_given_effect() && civ_file_compose(&history.present, &written) &&
           text_contains_folded(written.data, "\"custom\"");
}

static bool effect_is_found_ticked(void)
{
    return tech_given_effect() && civ_design_has_tech_effect(&history.present, TECH_TIER_IMPERIAL, EFFECT);
}

static bool no_effect_makes_vanilla(void)
{
    return tech_picked_with(TECH_VALUE_WOOD, WOOD_COST) && civ_file_compose(&history.present, &written) &&
           text_contains_folded(written.data, "\"vanilla\"");
}

static bool effect_changes_the_civ(void)
{
    Command tick = command_toggle_tech_effect(TECH_TIER_IMPERIAL, EFFECT);

    if (!tech_picked_with(TECH_VALUE_WOOD, WOOD_COST)) {
        return false;
    }
    reloaded = history.present;
    return history_do(&history, &tick) && !civ_design_same(&history.present, &reloaded);
}

static TestOutcome effect_tests_run(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("a civ bonus ticked as an effect of a unique tech is kept with it", effect_is_kept());
    case 2:
        return test_outcome_of("the multiplier of an effect of a unique tech is set", effect_multiplier_is_set());
    case 3:
        return test_outcome_of("the effects of a unique tech are saved and opened again",
                               effect_is_saved_and_opened());
    case 4:
        return test_outcome_of("a unique tech with effects is saved as the original's custom tech",
                               effect_makes_custom());
    case 5:
        return test_outcome_of("an effect of a unique tech changes the civ", effect_changes_the_civ());
    case 6:
        return test_outcome_of("a unique tech without effects is saved as the original's vanilla tech",
                               no_effect_makes_vanilla());
    case 7:
        return test_outcome_of("a civ bonus ticked as an effect of a unique tech is found ticked",
                               effect_is_found_ticked());
    default:
        return stat_build_tests_run(index - STAT_EFFECT_COUNT);
    }
}

static TestOutcome tech_tests_run(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("a command to set a value of a unique tech sets it", tech_value_is_set());
    case 2:
        return test_outcome_of("picking another unique tech clears the values set", tech_value_goes_with_its_tech());
    case 3:
        return test_outcome_of("a value of a unique tech out of its range is refused",
                               tech_value_out_of_range_is_refused());
    case 4:
        return test_outcome_of("a value set of a unique tech is saved and opened again",
                               tech_value_is_saved_and_opened());
    case 5:
        return test_outcome_of("a value not set of a unique tech stays not set when saved and opened",
                               tech_value_not_set_stays_so());
    case 6:
        return test_outcome_of("a cost of four zeros in a civ file is the game's cost", zero_cost_is_the_games());
    case 7:
        return test_outcome_of("a cost of 0 set of a unique tech is saved and opened again",
                               free_cost_is_saved_and_opened());
    default:
        return effect_tests_run(index - STAT_TECH_COUNT);
    }
}

TestOutcome stat_tests_run(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("a command to set a stat of the unique unit sets it", stat_is_set());
    case 2:
        return test_outcome_of("undo takes back a stat set of the unique unit", stat_is_undone());
    case 3:
        return test_outcome_of("picking another unique unit clears the stats set", stat_goes_with_its_unit());
    case 4:
        return test_outcome_of("a stat of the unique unit out of its range is refused", stat_out_of_range_is_refused());
    case 5:
        return test_outcome_of("a stat set of the unique unit is saved and opened again", stat_is_saved_and_opened());
    case 6:
        return test_outcome_of("the stats of the unique unit in a civ file of the original are read",
                               original_stats_are_read());
    case 7:
        return test_outcome_of("a stat set of the unique unit changes the civ", stat_changes_the_civ());
    case 8:
        return test_outcome_of("a cost set of the unique unit is saved and opened again", cost_is_saved_and_opened());
    case 9:
        return test_outcome_of("picking the same unique unit again keeps the stats set", same_unit_keeps_stats());
    case 10:
        return test_outcome_of("a cost set of the unique unit changes the civ", cost_changes_the_civ());
    default:
        return tech_tests_run(index - STAT_UNIT_COUNT);
    }
}
