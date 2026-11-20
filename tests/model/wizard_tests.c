#include "wizard_tests.h"

#include "../../src/core/check.h"
#include "../../src/view/wizard.h"

enum { A_BUILDING = 12, A_BONUS = 4, A_TEAM_BONUS = 8, A_UNIT = 1, A_TECH = 3, A_HERO = 1911 };

static CivDesign design;
static CivDesign blank;

static CivDesign *fresh(void)
{
    civ_design_reset(&design);
    civ_design_reset(&blank);
    return &design;
}

static bool is_done(WizardStep step)
{
    return wizard_step_is_done(&design, &blank, false, step);
}

static int steps_done(void)
{
    int done = 0;
    int step;

    for (step = 0; step < STEP_COUNT; ++step) {
        done += is_done((WizardStep)step) ? 1 : 0;
    }
    return done;
}

bool test_no_step_of_a_new_design_is_done(void)
{
    (void)fresh();
    return steps_done() == 0;
}

bool test_the_name_and_tree_step_is_done_once_the_civ_has_a_name(void)
{
    return civ_design_set_text(fresh(), TEXT_FIELD_NAME, "Testonia") && is_done(STEP_NAME_AND_TREE);
}

bool test_the_name_and_tree_step_is_done_once_a_node_is_chosen(void)
{
    return civ_design_toggle_tree(fresh(), TREE_BUILDINGS, A_BUILDING) && is_done(STEP_NAME_AND_TREE);
}

bool test_the_bonuses_step_is_done_once_a_bonus_is_ticked(void)
{
    return civ_design_toggle_bonus(fresh(), A_BONUS) && is_done(STEP_BONUSES);
}

bool test_the_bonuses_step_is_done_once_a_team_bonus_is_picked(void)
{
    return civ_design_toggle_team_bonus(fresh(), A_TEAM_BONUS) && is_done(STEP_BONUSES);
}

bool test_the_uu_ut_step_is_done_once_a_unit_is_picked(void)
{
    return civ_design_pick_unit(fresh(), UNIT_ORIGIN_VANILLA, A_UNIT) && is_done(STEP_UU_UT);
}

bool test_the_uu_ut_step_is_done_once_a_castle_tech_is_picked(void)
{
    return civ_design_pick_tech(fresh(), TECH_TIER_CASTLE, A_TECH) && is_done(STEP_UU_UT);
}

bool test_the_uu_ut_step_is_done_once_an_imperial_tech_is_picked(void)
{
    return civ_design_pick_tech(fresh(), TECH_TIER_IMPERIAL, A_TECH) &&
           is_done(STEP_UU_UT);
}

bool test_the_uu_ut_step_is_done_once_a_hero_is_picked(void)
{
    return civ_design_pick_hero(fresh(), A_HERO) && is_done(STEP_UU_UT);
}

bool test_a_pick_finishes_only_its_own_step(void)
{
    (void)CHECK(civ_design_pick_unit(fresh(), UNIT_ORIGIN_VANILLA, A_UNIT));
    return steps_done() == 1;
}

bool test_back_from_the_second_step_goes_to_the_first(void)
{
    return wizard_move(STEP_BONUSES, -1) == STEP_NAME_AND_TREE;
}

bool test_a_tree_that_swaps_a_node_of_the_base_civ_is_changed(void)
{
    (void)CHECK(civ_design_toggle_tree(fresh(), TREE_UNITS, A_UNIT));
    (void)CHECK(civ_design_toggle_tree(&blank, TREE_UNITS, A_UNIT + 1));
    return is_done(STEP_NAME_AND_TREE);
}

bool test_bonuses_that_swap_one_of_the_base_civ_are_changed(void)
{
    (void)CHECK(civ_design_toggle_bonus(fresh(), A_BONUS));
    (void)CHECK(civ_design_toggle_bonus(&blank, A_BONUS + 1));
    return is_done(STEP_BONUSES);
}

bool test_a_team_bonus_other_than_the_base_civs_is_a_change(void)
{
    (void)CHECK(civ_design_toggle_team_bonus(fresh(), A_TEAM_BONUS));
    (void)CHECK(civ_design_toggle_team_bonus(&blank, A_TEAM_BONUS + 1));
    return is_done(STEP_BONUSES);
}

bool test_the_base_civs_own_team_bonus_is_no_change(void)
{
    (void)CHECK(civ_design_toggle_team_bonus(fresh(), A_TEAM_BONUS));
    (void)CHECK(civ_design_toggle_team_bonus(&blank, A_TEAM_BONUS));
    return !is_done(STEP_BONUSES);
}
