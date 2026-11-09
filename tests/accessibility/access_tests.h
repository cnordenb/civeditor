#ifndef ACCESS_TESTS_H
#define ACCESS_TESTS_H

#include <stdbool.h>

bool test_every_text_contrasts_seven_to_one(void);
bool test_every_input_has_a_visible_label(void);
bool test_every_target_is_44_by_44(void);
bool test_no_two_controls_of_a_step_overlap(void);
bool test_the_window_title_names_the_civ_and_the_step(void);

#endif
