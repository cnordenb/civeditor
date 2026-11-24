#ifndef MODEL_TESTS_H
#define MODEL_TESTS_H

#include <stdbool.h>

bool test_typing_a_name_is_undone_at_once(void);
bool test_redo_makes_an_undone_change_again(void);
bool test_next_walks_every_step_in_order(void);
bool test_summary_names_what_is_still_needed(void);
bool model_tests_summary_holds(const char *words);

#endif
