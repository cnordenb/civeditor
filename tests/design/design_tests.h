#ifndef DESIGN_TESTS_H
#define DESIGN_TESTS_H

#include <stdbool.h>

bool test_a_ticked_bonus_counts_once(void);
bool test_a_picked_team_bonus_counts_once(void);
bool test_a_bonus_ticked_twice_is_not_chosen(void);
bool test_a_node_chosen_twice_is_not_chosen(void);
bool test_taking_a_node_out_keeps_the_other_nodes(void);
bool test_a_tree_takes_no_more_nodes_than_it_holds(void);

#endif
