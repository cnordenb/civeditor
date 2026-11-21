#include "present_tests.h"

#include <string.h>

#include "../../src/core/check.h"
#include "../../src/model/command.h"
#include "../../src/present/binding.h"
#include "../../src/present/summary.h"
#include "../dat/recipe_tests.h"

enum { FIRST_ROW = 0, SECOND_ROW = 1, THIRD_ROW = 2, FIRST_BONUS = 5, SECOND_BONUS = 9, NODE = 12 };

static CivDesign own;
static SummaryText summary;

static bool applied(CommandKind kind, int field, int value)
{
    Command command = command_of(kind, field, value);

    civ_design_reset(&own);
    return CHECK(command.kind == kind) && command_apply(&own, &command);
}

static bool row_gives(ControlId control, int row, CommandKind kind, int field, int value)
{
    const ListItem item = { control, row };
    Command command = list_item_command(recipe_game_catalog(), item);

    if (!CHECK((int)control >= 0) || !CHECK(control < CONTROL_COUNT)) {
        return false;
    }
    return (command.kind == kind) && (command.field == field) && (command.value == value);
}

static bool summary_holds(Scenario scenario, const char *words)
{
    if (!CHECK(words != NULL) || !summary_write(&summary, recipe_game_design(scenario), recipe_game_catalog())) {
        return false;
    }
    return strstr(summary.text, words) != NULL;
}

bool test_a_command_to_pick_a_unit_picks_it(void)
{
    return applied(COMMAND_PICK_UNIT, UNIT_ORIGIN_VANILLA, OWN_PICK) && (own.unique_unit.id == OWN_PICK);
}

bool test_a_command_to_pick_a_tech_picks_it(void)
{
    return applied(COMMAND_PICK_TECH, TECH_TIER_IMPERIAL, NODE) && (own.techs[TECH_TIER_IMPERIAL].km_index == NODE);
}

bool test_a_command_to_pick_a_hero_picks_it(void)
{
    return applied(COMMAND_PICK_HERO, 0, OWN_UNIT) && (own.hero_unit == OWN_UNIT);
}

bool test_a_command_to_tick_a_civ_bonus_ticks_it(void)
{
    return applied(COMMAND_TOGGLE_BONUS, 0, FIRST_BONUS) && civ_design_has_bonus(&own, FIRST_BONUS);
}

bool test_a_command_to_tick_a_team_bonus_ticks_it(void)
{
    return applied(COMMAND_TOGGLE_TEAM_BONUS, 0, FIRST_BONUS) && civ_design_has_team_bonus(&own, FIRST_BONUS);
}

bool test_a_command_to_tick_a_node_ticks_it(void)
{
    return applied(COMMAND_TOGGLE_TREE, TREE_BUILDINGS, NODE) && civ_design_tree_has(&own, TREE_BUILDINGS, NODE);
}

bool test_a_command_to_set_a_choice_sets_it(void)
{
    return applied(COMMAND_SET_CHOICE, CHOICE_WONDER, NODE) && (own.choices[CHOICE_WONDER] == NODE);
}

bool test_a_command_of_no_kind_changes_nothing(void)
{
    Command nothing = command_nothing();

    civ_design_reset(&own);
    return !command_apply(&own, &nothing) && (own.hero_unit == CIV_NO_CHOICE);
}

bool test_a_civ_bonus_ticked_twice_is_no_longer_picked(void)
{
    civ_design_reset(&own);
    return civ_design_toggle_bonus(&own, FIRST_BONUS) && civ_design_toggle_bonus(&own, SECOND_BONUS) &&
           civ_design_toggle_bonus(&own, FIRST_BONUS) && !civ_design_has_bonus(&own, FIRST_BONUS) &&
           civ_design_has_bonus(&own, SECOND_BONUS);
}

bool test_a_team_bonus_ticked_twice_is_no_longer_picked(void)
{
    civ_design_reset(&own);
    return civ_design_toggle_team_bonus(&own, FIRST_BONUS) && civ_design_toggle_team_bonus(&own, FIRST_BONUS) &&
           !civ_design_has_team_bonus(&own, FIRST_BONUS);
}

bool test_a_row_of_the_tech_tree_gives_the_command_to_tick_its_node(void)
{
    return row_gives(CONTROL_TREE_LIST, FIRST_ROW, COMMAND_TOGGLE_TREE, TREE_UNITS, LACKED_UNIT);
}

bool test_a_row_of_the_civ_bonuses_gives_the_command_to_tick_its_bonus(void)
{
    return row_gives(CONTROL_BONUS_LIST, SECOND_ROW, COMMAND_TOGGLE_BONUS, 0, GENERIC_BONUS);
}

bool test_a_row_of_the_team_bonuses_gives_the_command_to_tick_its_bonus(void)
{
    return row_gives(CONTROL_TEAM_LIST, SECOND_ROW, COMMAND_TOGGLE_TEAM_BONUS, 0, CHANGES_TEAM_BONUS);
}

bool test_a_row_of_a_look_gives_the_command_to_set_that_choice(void)
{
    return row_gives(CONTROL_ARCHITECTURE, SECOND_ROW, COMMAND_SET_CHOICE, CHOICE_ARCHITECTURE,
                     CIV_DEFAULT_ARCHITECTURE);
}

bool test_a_row_of_the_units_gives_the_command_to_pick_its_unit(void)
{
    return row_gives(CONTROL_UNIT_LIST, SECOND_ROW, COMMAND_PICK_UNIT, UNIT_ORIGIN_VANILLA, OWN_PICK);
}

bool test_a_row_of_the_heroes_gives_the_command_to_pick_its_hero(void)
{
    return row_gives(CONTROL_HERO_CHOOSE, THIRD_ROW, COMMAND_PICK_HERO, 0, OWN_UNIT);
}

bool test_unticking_hero_gives_the_command_to_pick_no_hero(void)
{
    Command command = hero_box_command(recipe_game_catalog(), false);

    return (command.kind == COMMAND_PICK_HERO) && (command.value == CIV_NO_CHOICE);
}

bool test_a_row_of_the_castle_techs_gives_the_command_to_pick_its_tech(void)
{
    return row_gives(CONTROL_CASTLE_TECH_LIST, FIRST_ROW, COMMAND_PICK_TECH, TECH_TIER_CASTLE, PICKED_TECH);
}

bool test_a_row_of_the_imperial_techs_gives_the_command_to_pick_its_tech(void)
{
    return row_gives(CONTROL_IMPERIAL_TECH_LIST, SECOND_ROW, COMMAND_PICK_TECH, TECH_TIER_IMPERIAL, PICKED_TECH);
}

bool test_a_row_of_the_castle_techs_shows_its_techs_name(void)
{
    ListItem item = { CONTROL_CASTLE_TECH_LIST, FIRST_ROW };

    return strcmp(list_item_text(recipe_game_catalog(), item).text, "Bulwark") == 0;
}

bool test_the_summary_names_the_unique_unit_and_the_unit_it_is_made_from(void)
{
    return summary_holds(FULL_CIV, "Unique unit: Tester, from Own");
}

bool test_the_summary_lists_a_civ_bonus_by_its_description(void)
{
    return summary_holds(FULL_CIV, "- Infantry is stronger");
}

bool test_the_summary_names_a_civ_bonus_the_catalog_lacks_by_its_number(void)
{
    return summary_holds(FULL_CIV, "- bonus #99");
}

bool test_the_summary_names_a_unique_tech_by_its_catalog_name(void)
{
    return summary_holds(FULL_CIV, "Castle tech: Bulwark");
}

bool test_the_summary_says_when_the_catalog_lacks_the_unique_unit(void)
{
    return summary_holds(BARE_CIV, "Unique unit: an unknown unit");
}

bool test_the_summary_says_when_the_catalog_lacks_a_unique_tech(void)
{
    return summary_holds(BARE_CIV, "Castle tech: an unknown tech");
}

static const RecipeReport SKIPPED_BONUS = { 3, 1, 0, 0, 0, 0, 0, 0, 0, false, false, false, { 0, 0 } };
static const RecipeReport SKIPPED_HERO = { 0, 0, 0, 0, 0, 0, 0, 0, 0, false, false, true, { 0, 0 } };
static const RecipeReport WHOLE_CIV = { 2, 0, 0, 1, 0, 2, 2, 0, 0, false, false, false, { 0, 0 } };
static const RecipeReport ONCE_ONLY = { 1, 0, 1, 0, 0, 0, 0, 0, 0, false, false, false, { 0, 0 } };

static bool build_report_holds(const RecipeReport *report, const char *words)
{
    static CivDesign built_civ;
    static BuiltCivs built;

    built.designs[0] = &built_civ;
    built.count = 1;

    built.reports = report;
    civ_design_reset(&built_civ);
    (void)CHECK(civ_design_set_text(&built_civ, TEXT_FIELD_NAME, "Testonia"));
    if (!CHECK(report != NULL) || !CHECK(words != NULL) || !summary_write_build(&summary, "testonia", &built)) {
        return false;
    }
    return strstr(summary.text, words) != NULL;
}

bool test_the_build_report_counts_the_civ_bonuses_built_and_skipped(void)
{
    return build_report_holds(&SKIPPED_BONUS, "Civ bonuses built: 3 of 4; the others are not built yet.");
}

bool test_the_build_report_says_when_the_hero_was_not_built(void)
{
    return build_report_holds(&SKIPPED_HERO, "The hero is not built: the civ lacks its unit.");
}

bool test_the_build_report_says_when_a_bonus_counts_only_once(void)
{
    return build_report_holds(&ONCE_ONLY, "A bonus made of the game's techs counts only once.");
}

bool test_the_build_report_of_a_whole_civ_names_the_two_folders(void)
{
    return build_report_holds(&WHOLE_CIV, "built as testonia-data and testonia-ui in the game's mods folder.\n\n"
                                          "Testonia\nCiv bonuses built: 2 of 2.\n\nTurn on");
}

static bool build_is_whole(const RecipeReport *report)
{
    BuiltCivs built = { { NULL }, NULL, 1 };

    built.reports = report;
    return CHECK(report != NULL) && summary_build_is_whole(&built);
}

bool test_a_build_that_left_nothing_out_is_whole(void)
{
    return build_is_whole(&WHOLE_CIV);
}

bool test_a_build_that_skipped_a_civ_bonus_is_not_whole(void)
{
    return !build_is_whole(&SKIPPED_BONUS);
}
