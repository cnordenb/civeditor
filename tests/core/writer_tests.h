#ifndef WRITER_TESTS_H
#define WRITER_TESTS_H

#include <stdbool.h>

bool test_two_members_are_written_with_a_comma_between(void);
bool test_the_first_member_of_an_object_inside_another_has_no_comma(void);
bool test_an_object_left_open_is_not_a_finished_text(void);
bool test_a_text_too_long_for_the_writer_is_reported(void);

#endif
