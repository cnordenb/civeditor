#include "filter_tests.h"

#include "../../../src/core/check.h"
#include "../../../src/core/text.h"
#include "../../../src/present/binding.h"
#include "../../../src/present/summary.h"

enum { BRITONS = 1, FRANKS = 2, SPARTANS = 48, GROUP_SIZE = 12, NO_ROW = -1 };
enum { SHEPHERDS = 100, CASTLES = 101, KNIGHTS = 102, RANGES = 200, SIGHT = 201 };
enum { LONGBOWMAN = 0, AXEMAN = 1, JOAN = 7, YEOMEN = 2, WARWOLF = 3, BEARDED_AXE = 4, CHIVALRY = 5 };
enum { AXEMAN_ROW = 1, BEARDED_AXE_ROW = 2, CHIVALRY_ROW = 3, SIGHT_ROW = 1 };

static Catalog game;
static ListModel model;
static CivDesign design;

static void add_look(int index, AppearanceKind kind, int slot, const char *name)
{
    game.appearance[index].kind = kind;
    game.appearance[index].id = slot;
    (void)text_copy(game.appearance[index].name, sizeof game.appearance[index].name, name);
}

static void add_base(int index, int slot, int unit, int castle_tech, int imperial_tech)
{
    game.bases[index].id = slot;
    game.bases[index].values[BASE_TEAM_BONUS] = RANGES + index;
    game.bases[index].values[BASE_UNIT] = unit;
    game.bases[index].values[BASE_CASTLE_TECH] = castle_tech;
    game.bases[index].values[BASE_IMPERIAL_TECH] = imperial_tech;
}

static void add_bonus(BonusEntry *entry, int id, const char *description)
{
    entry->id = id;
    (void)text_copy(entry->description, sizeof entry->description, description);
}

static void add_unit(int index, UnitRole role, int id, const char *name)
{
    game.units[index].role = role;
    game.units[index].id = id;
    (void)text_copy(game.units[index].name, sizeof game.units[index].name, name);
}

static void add_tech(int index, TechTier tier, int id, const char *name)
{
    game.techs[index].tier = tier;
    game.techs[index].id = id;
    (void)text_copy(game.techs[index].name, sizeof game.techs[index].name, name);
}

static void add_civs(void)
{
    add_look(0, APPEARANCE_BASE, BRITONS, "Britons");
    add_look(1, APPEARANCE_BASE, FRANKS, "Franks");
    add_look(2, APPEARANCE_BASE, SPARTANS, "Spartans");
    add_look(3, APPEARANCE_UNRANKED, SPARTANS, "Spartans");
    game.appearance_count = 4;
    add_base(0, BRITONS, LONGBOWMAN, YEOMEN, WARWOLF);
    add_base(1, FRANKS, AXEMAN, BEARDED_AXE, CHIVALRY);
    game.bases[0].bonuses.values[0] = SHEPHERDS;
    game.bases[0].bonuses.count = 1;
    game.bases[1].bonuses.values[0] = CASTLES;
    game.bases[1].bonuses.values[1] = KNIGHTS;
    game.bases[1].bonuses.count = 2;
    game.base_count = 2;
}

static const Catalog *made_game(void)
{
    add_civs();
    add_bonus(&game.bonuses[0], SHEPHERDS, "Shepherds work faster");
    add_bonus(&game.bonuses[1], CASTLES, "Castles are cheaper");
    add_bonus(&game.bonuses[2], KNIGHTS, "Knights have more hit points");
    game.bonus_count = 3;
    add_bonus(&game.team_bonuses[0], RANGES, "Archery Ranges work faster");
    add_bonus(&game.team_bonuses[1], SIGHT, "Knights see further");
    game.team_bonus_count = 2;
    add_unit(0, UNIT_ROLE_VANILLA, LONGBOWMAN, "Longbowman");
    add_unit(1, UNIT_ROLE_VANILLA, AXEMAN, "Throwing Axeman");
    add_unit(2, UNIT_ROLE_HERO, JOAN, "Joan of Arc");
    game.unit_count = 3;
    add_tech(0, TECH_TIER_CASTLE, YEOMEN, "Yeomen");
    add_tech(1, TECH_TIER_IMPERIAL, WARWOLF, "Warwolf");
    add_tech(2, TECH_TIER_CASTLE, BEARDED_AXE, "Bearded Axe");
    add_tech(3, TECH_TIER_IMPERIAL, CHIVALRY, "Chivalry");
    game.tech_count = 4;
    game.tree[0].civs = (1ULL << BRITONS) | (1ULL << FRANKS);
    game.tree[1].civs = 1ULL << FRANKS;
    game.tree_count = 2;
    return &game;
}

static int rows(ControlId list, const char *filter)
{
    ListQuery query = { CONTROL_TITLE, "", LIST_ALL_CATEGORIES };

    query.control = list;
    (void)text_copy(query.filter, sizeof query.filter, filter);
    return (CHECK(filter != NULL) && list_model_fill(&model, made_game(), &query)) ? model.count : NO_ROW;
}

static int first_row(ControlId list, const char *filter)
{
    return (rows(list, filter) > 0) ? model.items[0] : NO_ROW;
}

static bool whole_without(int skipped_field)
{
    RecipeReport report = { 2, 0, 0, 1, 0, 2, 2, 0, 0, false, false, false, { 0, 0 } };
    BuiltCivs built = { { NULL }, NULL, 1 };

    report.team_bonuses_skipped = (skipped_field == 1) ? 1 : 0;
    report.multipliers_skipped = (skipped_field == 2) ? 1 : 0;
    report.unique_techs_skipped = (skipped_field == 3) ? 1 : 0;
    report.unit_skipped = skipped_field == 4;
    report.tree_overflowed = skipped_field == 5;
    report.hero_skipped = skipped_field == 6;
    built.reports = &report;
    return summary_build_is_whole(&built);
}

static bool list_holds_after(int count, int value)
{
    TsvNumberList list = { { SHEPHERDS, CASTLES, KNIGHTS }, 0 };

    list.count = count;
    return CHECK(count >= 0) && tsv_list_holds(&list, value);
}

static bool clears(int bonuses)
{
    civ_design_reset(&design);
    if (bonuses > 0) {
        (void)CHECK(civ_design_toggle_bonus(&design, SHEPHERDS));
    }
    return civ_design_clear_bonuses(&design);
}

static TestOutcome run_civ_name_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("a civ's name leaves one castle tech", rows(CONTROL_CASTLE_TECH_LIST, "brit") == 1);
    case 2:
        return test_outcome_of("a civ's name leaves the castle tech of that civ",
                               first_row(CONTROL_CASTLE_TECH_LIST, "franks") == BEARDED_AXE_ROW);
    case 3:
        return test_outcome_of("a civ's name leaves the imperial tech of that civ",
                               first_row(CONTROL_IMPERIAL_TECH_LIST, "franks") == CHIVALRY_ROW);
    case 4:
        return test_outcome_of("a civ's name leaves the unique unit of that civ",
                               first_row(CONTROL_UNIT_LIST, "franks") == AXEMAN_ROW);
    case 5:
        return test_outcome_of("a civ's name leaves one unique unit", rows(CONTROL_UNIT_LIST, "brit") == 1);
    case 6:
        return test_outcome_of("a civ's name leaves every civ bonus of that civ",
                               rows(CONTROL_BONUS_LIST, "franks") == 2);
    case 7:
        return test_outcome_of("a civ's name leaves no civ bonus of another civ",
                               rows(CONTROL_BONUS_LIST, "brit") == 1);
    case 8:
        return test_outcome_of("a civ's name leaves the team bonus of that civ",
                               first_row(CONTROL_TEAM_LIST, "franks") == SIGHT_ROW);
    case 9:
        return test_outcome_of("a civ's name leaves the nodes that civ has", rows(CONTROL_TREE_LIST, "franks") == 2);
    case 10:
        return test_outcome_of("a civ's name leaves no node that civ lacks", rows(CONTROL_TREE_LIST, "brit") == 1);
    case 11:
        return test_outcome_of("the middle of a civ's name is not its name",
                               rows(CONTROL_CASTLE_TECH_LIST, "ritons") == 0);
    case 12:
        return test_outcome_of("a civ's name leaves no hero", rows(CONTROL_HERO_CHOOSE, "franks") == 0);
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_ranked_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("a filter still leaves the rows that hold its text",
                               first_row(CONTROL_CASTLE_TECH_LIST, "bearded") == BEARDED_AXE_ROW);
    case 2:
        return test_outcome_of("Civ to edit leaves out a civ that is not ranked", rows(CONTROL_CIV, "") == 2);
    case 3:
        return test_outcome_of("a civ without an unranked line is ranked", catalog_is_ranked(made_game(), BRITONS));
    case 4:
        return test_outcome_of("a civ with an unranked line is not ranked",
                               !catalog_is_ranked(made_game(), SPARTANS));
    case 5:
        return test_outcome_of("the first filter names its list",
                               binding_filtered_list(CONTROL_TREE_FILTER) == CONTROL_TREE_LIST);
    case 6:
        return test_outcome_of("the last filter names its list",
                               binding_filtered_list(CONTROL_IMPERIAL_TECH_FILTER) == CONTROL_IMPERIAL_TECH_LIST);
    case 7:
        return test_outcome_of("a control that is no filter names no list",
                               binding_filtered_list(CONTROL_NAME) == CONTROL_COUNT);
    case 8:
        return test_outcome_of("a list of numbers holds its first", list_holds_after(3, SHEPHERDS));
    case 9:
        return test_outcome_of("a list of numbers holds its last", list_holds_after(3, KNIGHTS));
    case 10:
        return test_outcome_of("a list of numbers holds no number past its count", !list_holds_after(2, KNIGHTS));
    case 11:
        return test_outcome_of("an empty list of numbers holds nothing", !list_holds_after(0, SHEPHERDS));
    case 12:
        return test_outcome_of("the look choices are shown on no step",
                               !layout_shows(CONTROL_ARCHITECTURE, STEP_NAME_AND_TREE));
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_whole_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("a build with nothing skipped is whole", whole_without(0));
    case 2:
        return test_outcome_of("a build that skipped the team bonus is not whole", !whole_without(1));
    case 3:
        return test_outcome_of("a build that skipped a multiplier is not whole", !whole_without(2));
    case 4:
        return test_outcome_of("a build that skipped a unique tech is not whole", !whole_without(3));
    case 5:
        return test_outcome_of("a build that skipped the unique unit is not whole", !whole_without(4));
    case 6:
        return test_outcome_of("a build whose tech tree overflowed is not whole", !whole_without(5));
    case 7:
        return test_outcome_of("a build that skipped the hero is not whole", !whole_without(6));
    case 8:
        return test_outcome_of("clearing the civ bonuses of a civ with one is a change", clears(1));
    case 9:
        return test_outcome_of("clearing the civ bonuses of a civ with none is no change", !clears(0));
    case 10:
        return test_outcome_of("clearing the civ bonuses leaves none",
                               clears(1) && !civ_design_has_bonus(&design, SHEPHERDS));
    case 11:
        return test_outcome_of("the built mark takes no focus", !layout_is_focusable(CONTROL_BUILT));
    default:
        return test_outcome_missing();
    }
}

TestOutcome filter_tests_run(int index)
{
    if (!CHECK(index > 0) || (index > FILTER_TEST_COUNT)) {
        return test_outcome_missing();
    }
    if (index <= GROUP_SIZE) {
        return run_civ_name_test(index);
    }
    if (index <= 2 * GROUP_SIZE) {
        return run_ranked_test(index - GROUP_SIZE);
    }
    return run_whole_test(index - (2 * GROUP_SIZE));
}
