#include "wizard_tests.h"

#include "../../src/core/check.h"
#include "../../src/view/wizard.h"

enum { A_BUILDING = 12, A_BONUS = 4, A_TEAM_BONUS = 8, A_UNIT = 1, A_TECH = 3, A_HERO = 1911 };

static CivDesign design;

static CivDesign *fresh(void)
{
    civ_design_reset(&design);
    return &design;
}

static int steps_done(void)
{
    int done = 0;
    int step;

    for (step = 0; step < STEP_COUNT; ++step) {
        done += wizard_step_is_done(&design, (WizardStep)step) ? 1 : 0;
    }
    return done;
}

bool test_no_step_of_a_new_design_is_done(void)
{
    (void)fresh();
    return steps_done() == 0;
}

bool test_the_identity_step_is_done_once_the_civ_has_a_name(void)
{
    return civ_design_set_text(fresh(), TEXT_FIELD_NAME, "Testonia") && wizard_step_is_done(&design, STEP_IDENTITY);
}

bool test_the_tech_tree_step_is_done_once_a_node_is_chosen(void)
{
    return civ_design_toggle_tree(fresh(), TREE_BUILDINGS, A_BUILDING) && wizard_step_is_done(&design, STEP_TECH_TREE);
}

bool test_the_bonuses_step_is_done_once_a_bonus_is_ticked(void)
{
    return civ_design_toggle_bonus(fresh(), A_BONUS) && wizard_step_is_done(&design, STEP_BONUSES);
}

bool test_the_team_bonus_step_is_done_once_one_is_picked(void)
{
    return civ_design_toggle_team_bonus(fresh(), A_TEAM_BONUS) && wizard_step_is_done(&design, STEP_TEAM_BONUS);
}

bool test_the_unique_unit_step_is_done_once_a_unit_is_picked(void)
{
    return civ_design_pick_unit(fresh(), UNIT_ORIGIN_VANILLA, A_UNIT) && wizard_step_is_done(&design, STEP_UNIQUE_UNIT);
}

bool test_the_castle_tech_step_is_done_once_a_tech_is_picked(void)
{
    return civ_design_pick_tech(fresh(), TECH_TIER_CASTLE, A_TECH) && wizard_step_is_done(&design, STEP_CASTLE_TECH);
}

bool test_the_imperial_tech_step_is_done_once_a_tech_is_picked(void)
{
    return civ_design_pick_tech(fresh(), TECH_TIER_IMPERIAL, A_TECH) &&
           wizard_step_is_done(&design, STEP_IMPERIAL_TECH);
}

bool test_the_hero_step_is_done_once_a_hero_is_picked(void)
{
    return civ_design_pick_hero(fresh(), A_HERO) && wizard_step_is_done(&design, STEP_HERO);
}

bool test_a_pick_finishes_only_its_own_step(void)
{
    (void)CHECK(civ_design_pick_unit(fresh(), UNIT_ORIGIN_VANILLA, A_UNIT));
    return steps_done() == 1;
}

bool test_back_from_the_second_step_goes_to_the_first(void)
{
    return wizard_move(STEP_TECH_TREE, -1) == STEP_IDENTITY;
}
