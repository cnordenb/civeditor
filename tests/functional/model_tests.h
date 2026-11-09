#ifndef MODEL_TESTS_H
#define MODEL_TESTS_H

#include <stdbool.h>

bool test_undo_brings_back_the_civ_before_a_change(void);
bool test_typing_a_name_is_undone_at_once(void);
bool test_redo_makes_an_undone_change_again(void);
bool test_a_25th_bonus_is_refused(void);
bool test_next_walks_every_step_in_order(void);
bool test_summary_names_what_is_still_needed(void);
bool test_summary_labels_the_name(void);
bool test_the_last_step_shows_no_next_button(void);

#endif
