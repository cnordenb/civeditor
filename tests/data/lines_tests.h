#ifndef LINES_TESTS_H
#define LINES_TESTS_H

#include <stdbool.h>

bool test_a_line_of_a_file_is_split_at_its_tabs(void);
bool test_a_number_field_of_a_file_is_read(void);
bool test_a_text_field_of_a_file_is_read(void);
bool test_a_list_of_numbers_is_read_in_its_order(void);
bool test_a_list_of_slots_is_read_as_bits(void);
bool test_the_second_line_of_a_file_follows_the_first(void);
bool test_a_file_ends_after_its_last_line(void);
bool test_a_file_that_is_missing_is_not_opened(void);

#endif
