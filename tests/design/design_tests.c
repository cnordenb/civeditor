#include "design_tests.h"

#include "../../src/core/check.h"
#include "../../src/model/civ_design.h"

enum { A_BONUS = 7, A_TEAM_BONUS = 3, FIRST_NODE = 5, SECOND_NODE = 9 };

static CivDesign design;

static CivDesign *fresh(void)
{
    civ_design_reset(&design);
    return &design;
}

static void fill_tree(void)
{
    int node;

    (void)fresh();
    for (node = 1; node <= CIV_TREE_CAPACITY; ++node) {
        (void)CHECK(civ_design_toggle_tree(&design, TREE_UNITS, node));
    }
}

bool test_a_ticked_bonus_counts_once(void)
{
    return civ_design_toggle_bonus(fresh(), A_BONUS) && (design.bonuses[0].multiplier == 1);
}

bool test_a_picked_team_bonus_counts_once(void)
{
    return civ_design_toggle_team_bonus(fresh(), A_TEAM_BONUS) && (design.team_bonuses[0].multiplier == 1);
}

bool test_a_bonus_ticked_twice_is_not_chosen(void)
{
    (void)CHECK(civ_design_toggle_bonus(fresh(), A_BONUS));
    return civ_design_toggle_bonus(&design, A_BONUS) && (design.bonus_count == 0);
}

bool test_a_node_chosen_twice_is_not_chosen(void)
{
    (void)CHECK(civ_design_toggle_tree(fresh(), TREE_UNITS, FIRST_NODE));
    return civ_design_toggle_tree(&design, TREE_UNITS, FIRST_NODE) &&
           !civ_design_tree_has(&design, TREE_UNITS, FIRST_NODE);
}

bool test_taking_a_node_out_keeps_the_other_nodes(void)
{
    (void)CHECK(civ_design_toggle_tree(fresh(), TREE_UNITS, FIRST_NODE));
    (void)CHECK(civ_design_toggle_tree(&design, TREE_UNITS, SECOND_NODE));
    return civ_design_toggle_tree(&design, TREE_UNITS, FIRST_NODE) &&
           civ_design_tree_has(&design, TREE_UNITS, SECOND_NODE);
}

bool test_a_tree_takes_no_more_nodes_than_it_holds(void)
{
    fill_tree();
    return !civ_design_toggle_tree(&design, TREE_UNITS, CIV_TREE_CAPACITY + 1);
}
