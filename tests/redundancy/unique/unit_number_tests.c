#include "unit_number_tests.h"

#include <string.h>

#include "../../../src/core/text.h"
#include "../../../src/numbers/number_edit.h"

enum {
    LONGBOWMAN = 0,
    IMMORTAL = 97,
    ABSENT = 99,
    NAME_ROW = 0,
    COST_ROW = 1,
    STATS_ROW = 2,
    WOOD_FIELD = 1,
    HP_FIELD = 0,
    ATTACK_FIELD = 1,
    MELEE_FIELD = 2,
    SPEED_FIELD = 5,
    SET_HP = 100,
    TYPED_WOOD = 60,
    TYPED_SPEED = 150,
    TYPED_MELEE = -2,
    YEOMEN = 3,
    YEOMEN_TIME = 60,
    DESCRIPTION_ROW = 1,
    TECH_COST_ROW = 2,
    TIME_ROW = 3,
    TYPED_TECH_WOOD = 600,
    EFFECT = 81,
    EFFECT_ROW = 3,
    TWICE = 2000000,
    LEAST_HP = 1,
    MOST_HP = 32767
};

static const int LONGBOWMAN_STATS[UNIT_STAT_COUNT] = { 0, 35, 0, 40, 35, 6, 0, 0, 5, 96, 200 };
static const int YEOMEN_COST[RESOURCE_KIND_COUNT] = { 0, 750, 0, 450 };

static Catalog catalog;
static CivDesign design;
static BonusShown shown;

static void add_unit(int index, int id, const char *name, const int *stats)
{
    UnitEntry *unit = &catalog.units[index];
    int stat;

    unit->role = UNIT_ROLE_VANILLA;
    unit->id = id;
    (void)text_copy(unit->name, sizeof unit->name, name);
    for (stat = 0; stat < UNIT_STAT_COUNT; ++stat) {
        unit->stats[stat] = (stats == NULL) ? TSV_MISSING : stats[stat];
    }
}

static NumberPlace picked(int id, int row, int number)
{
    NumberPlace place = { &catalog, &design, { NUMBER_PANEL_UNIT, row, number } };

    add_unit(0, LONGBOWMAN, "Longbowman", LONGBOWMAN_STATS);
    add_unit(1, IMMORTAL, "Immortal", NULL);
    catalog.unit_count = 2;
    civ_design_reset(&design);
    if (id != CIV_NO_CHOICE) {
        (void)civ_design_pick_unit(&design, UNIT_ORIGIN_VANILLA, id);
    }
    return place;
}

static bool row_reads(int id, int row, const char *expected)
{
    return number_row(picked(id, row, 0), &shown) && (strcmp(shown.text, expected) == 0);
}

static bool set_stat_shown(void)
{
    NumberPlace place = picked(LONGBOWMAN, STATS_ROW, 0);

    return civ_design_set_unit_stat(&design, UNIT_RANK_BASE, UNIT_STAT_HP, SET_HP) && number_row(place, &shown) &&
           text_starts_with(shown.text, "Hit points 100, attack 6");
}

static bool field_holds_number(void)
{
    char text[NUMBER_TEXT_CAPACITY];

    return number_text(picked(LONGBOWMAN, STATS_ROW, SPEED_FIELD), 1.0, NUMBER_DIGITS, text, sizeof text) &&
           (strcmp(text, "0.96") == 0);
}

static bool typed_sets(int row, int number, const char *typed, UnitStat stat, int value)
{
    Command command = number_typed_command(picked(LONGBOWMAN, row, number), typed);
    Override set;

    if (!command_apply(&design, &command)) {
        return false;
    }
    set = design.unique_unit.stats[UNIT_RANK_BASE][stat];
    return set.set && (set.value == value);
}

static bool typed_refused(void)
{
    return number_typed_command(picked(LONGBOWMAN, STATS_ROW, HP_FIELD), "0").kind == COMMAND_KIND_COUNT;
}

static bool range_named(void)
{
    char text[NUMBER_TEXT_CAPACITY];

    return number_range_text(picked(LONGBOWMAN, STATS_ROW, SPEED_FIELD), "Type a value from ", text, sizeof text) &&
           (strcmp(text, "Type a value from 0.01 to 9.99") == 0);
}

static bool field_named(void)
{
    char text[NUMBER_TEXT_CAPACITY];

    return number_name(picked(LONGBOWMAN, STATS_ROW, ATTACK_FIELD), text, sizeof text) && (strcmp(text, "Attack") == 0);
}

static bool alone_when_none(void)
{
    return row_reads(CIV_NO_CHOICE, NAME_ROW, "No unit is selected yet.") &&
           (number_rows(&catalog, &design, NUMBER_PANEL_UNIT) == 1);
}

static NumberPlace tech_picked(int id, int row, int number)
{
    NumberPlace place = { &catalog, &design, { NUMBER_PANEL_IMPERIAL_TECH, row, number } };
    TechEntry *tech = &catalog.techs[0];

    tech->tier = TECH_TIER_IMPERIAL;
    tech->id = YEOMEN;
    (void)text_copy(tech->name, sizeof tech->name, "Yeomen");
    (void)text_copy(tech->description, sizeof tech->description, "+1 foot archer range");
    (void)memcpy(tech->cost, YEOMEN_COST, sizeof tech->cost);
    tech->time = YEOMEN_TIME;
    catalog.tech_count = 1;
    civ_design_reset(&design);
    (void)civ_design_pick_tech(&design, TECH_TIER_IMPERIAL, id);
    return place;
}

static bool tech_row_reads(int row, const char *expected)
{
    return number_row(tech_picked(YEOMEN, row, 0), &shown) && (strcmp(shown.text, expected) == 0);
}

static bool tech_typed_sets(void)
{
    Command command = number_typed_command(tech_picked(YEOMEN, TECH_COST_ROW, WOOD_FIELD), "600");
    Override set;

    if (!command_apply(&design, &command)) {
        return false;
    }
    set = design.techs[TECH_TIER_IMPERIAL].values[TECH_VALUE_WOOD];
    return set.set && (set.value == TYPED_TECH_WOOD);
}

static bool time_field_named(void)
{
    char text[NUMBER_TEXT_CAPACITY];

    return number_name(tech_picked(YEOMEN, TIME_ROW, 0), text, sizeof text) && (strcmp(text, "Research time") == 0);
}

static bool tech_alone_when_none(void)
{
    return number_row(tech_picked(CIV_NO_CHOICE, NAME_ROW, 0), &shown) &&
           (strcmp(shown.text, "No tech is selected yet.") == 0) &&
           (number_rows(&catalog, &design, NUMBER_PANEL_IMPERIAL_TECH) == 1);
}

static NumberPlace effect_picked(int number)
{
    NumberPlace place = tech_picked(YEOMEN, EFFECT_ROW, number);

    catalog.bonuses[0].id = EFFECT;
    (void)text_copy(catalog.bonuses[0].description, sizeof catalog.bonuses[0].description,
                    "Villagers carry +3 food");
    catalog.bonus_count = 1;
    (void)civ_design_toggle_tech_effect(&design, TECH_TIER_IMPERIAL, EFFECT);
    return place;
}

static bool effect_shown(void)
{
    return number_row(effect_picked(0), &shown) && (strcmp(shown.text, "Villagers carry +3 food") == 0);
}

static bool effect_typed_sets(void)
{
    Command command = number_typed_command(effect_picked(0), "6");

    return command_apply(&design, &command) &&
           (design.techs[TECH_TIER_IMPERIAL].effects[0].multiplier.millionths == TWICE);
}

static bool tech_cost_range_named(void)
{
    char text[NUMBER_TEXT_CAPACITY];

    return number_range_text(tech_picked(YEOMEN, TECH_COST_ROW, WOOD_FIELD), "Type a value from ", text,
                             sizeof text) &&
           (strcmp(text, "Type a value from 0 to 9999") == 0);
}

static bool effect_drops_description(void)
{
    NumberPlace place = effect_picked(0);

    place.focus.pick = DESCRIPTION_ROW;
    return number_row(place, &shown) && text_starts_with(shown.text, "Cost: ");
}

static bool effect_field_named(void)
{
    char text[BONUS_TEXT_CAPACITY] = "";

    return number_name(effect_picked(0), text, sizeof text) && (strcmp(text, "Villagers carry +3 food") == 0);
}

static TestOutcome effect_number_tests_run(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("an effect of a selected tech takes the place of what its game tech does",
                               effect_shown());
    case 2:
        return test_outcome_of("the number of an effect of a selected tech slides", number_slides(effect_picked(0)));
    case 3:
        return test_outcome_of("a value typed into the number of an effect of a tech sets its multiplier",
                               effect_typed_sets());
    case 4:
        return test_outcome_of("the field of the number of an effect of a tech is named by its row",
                               effect_field_named());
    default:
        return game_number_tests_run(index - UNIT_NUMBER_EFFECT_COUNT);
    }
}

static TestOutcome tech_number_tests_run(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("the second row of a selected tech says what it does",
                               tech_row_reads(DESCRIPTION_ROW, "+1 foot archer range"));
    case 2:
        return test_outcome_of("the cost row of a selected tech names each resource with its number",
                               tech_row_reads(TECH_COST_ROW, "Cost: 0 food, 750 wood, 0 stone, 450 gold"));
    case 3:
        return test_outcome_of("the research time row of a selected tech says it in seconds",
                               tech_row_reads(TIME_ROW, "Research time: 60 seconds"));
    case 4:
        return test_outcome_of("a value typed into the field of a cost of a tech sets that cost", tech_typed_sets());
    case 5:
        return test_outcome_of("the field of the research time is named after it", time_field_named());
    case 6:
        return test_outcome_of("with no tech selected the panel holds only the row that says so",
                               tech_alone_when_none());
    case 7:
        return test_outcome_of("the range of a cost of a tech runs from 0 to 9999", tech_cost_range_named());
    case 8:
        return test_outcome_of("with an effect ticked the second row of a selected tech is its cost",
                               effect_drops_description());
    case 9:
        return test_outcome_of("the first row of a selected tech holds its name", tech_row_reads(NAME_ROW, "Yeomen"));
    default:
        return effect_number_tests_run(index - UNIT_NUMBER_TECH_COUNT);
    }
}

TestOutcome unit_number_tests_run(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("the first row of the selected unit holds its name",
                               row_reads(LONGBOWMAN, NAME_ROW, "Longbowman"));
    case 2:
        return test_outcome_of("the cost row of the selected unit names each resource with its number",
                               row_reads(LONGBOWMAN, COST_ROW, "Cost: 0 food, 35 wood, 0 stone, 40 gold"));
    case 3:
        return test_outcome_of("the stats row of the selected unit names each stat with its number",
                               row_reads(LONGBOWMAN, STATS_ROW,
                                         "Hit points 35, attack 6, melee armor 0, pierce armor 0, range 5, "
                                         "speed 0.96, reload time 2"));
    case 4:
        return test_outcome_of("a stat set of the selected unit is shown in its row", set_stat_shown());
    case 5:
        return test_outcome_of("a stat of the selected unit that the catalog does not know is shown as a dash",
                               row_reads(IMMORTAL, STATS_ROW,
                                         "Hit points -, attack -, melee armor -, pierce armor -, range -, "
                                         "speed -, reload time -"));
    case 6:
        return test_outcome_of("the field of a stat of the selected unit holds the number alone",
                               field_holds_number());
    case 7:
        return test_outcome_of("a value typed into the field of a stat sets that stat",
                               typed_sets(STATS_ROW, HP_FIELD, "100", UNIT_STAT_HP, SET_HP));
    case 8:
        return test_outcome_of("a value typed into the field of a cost sets that cost",
                               typed_sets(COST_ROW, WOOD_FIELD, "60", UNIT_STAT_WOOD, TYPED_WOOD));
    case 9:
        return test_outcome_of("a speed typed into its field is kept in hundredths",
                               typed_sets(STATS_ROW, SPEED_FIELD, "1.5", UNIT_STAT_SPEED, TYPED_SPEED));
    case 10:
        return test_outcome_of("a value typed with a minus sets a stat below zero",
                               typed_sets(STATS_ROW, MELEE_FIELD, " -2 ", UNIT_STAT_MELEE_ARMOR, TYPED_MELEE));
    case 11:
        return test_outcome_of("a value typed past the range of a stat is refused", typed_refused());
    case 12:
        return test_outcome_of("the range of a stat is named by its least and most values", range_named());
    case 13:
        return test_outcome_of("the field of a stat is named after the stat", field_named());
    case 14:
        return test_outcome_of("with no unit selected the panel holds only the row that says so", alone_when_none());
    case 15:
        return test_outcome_of("a selected unit not in the catalog is said to be so",
                               row_reads(ABSENT, NAME_ROW, "The selected unit is not in the catalog."));
    case 16:
        return test_outcome_of("the numbers of the selected unit have no slider",
                               !number_slides(picked(LONGBOWMAN, STATS_ROW, HP_FIELD)));
    case 17:
        return test_outcome_of("the least value of the range of a stat typed into its field sets it",
                               typed_sets(STATS_ROW, HP_FIELD, "1", UNIT_STAT_HP, LEAST_HP));
    case 18:
        return test_outcome_of("the most value of the range of a stat typed into its field sets it",
                               typed_sets(STATS_ROW, HP_FIELD, "32767", UNIT_STAT_HP, MOST_HP));
    default:
        return tech_number_tests_run(index - UNIT_NUMBER_OWN_COUNT);
    }
}
