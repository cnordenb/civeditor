#include "suites.h"

#include "../../src/core/check.h"
#include "../core/read_tests.h"
#include "../pack/dynamic_tests.h"
#include "../core/text_tests.h"
#include "../core/writer_tests.h"
#include "../data/lines_tests.h"
#include "../data/catalog_tests.h"

enum {
    NUMBER_END = 3,
    WIDEN_END = 10,
    TSV_END = 18,
    ESCAPE_END = 26,
    JSON_END = 32,
    EDGE_END = 38,
    PACK_END = 39,
    TEXT_END = 47,
    WRITER_END = 51,
    LINES_END = 59,
    CATALOG_END = 64
};

static TestOutcome run_number_test(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= SUITE_TEST_LIMIT)) {
        return test_outcome_missing();
    }
    switch (index) {
    case 1:
        return test_outcome_of("a number with a minus sign is negative", test_a_number_with_a_minus_sign_is_negative());
    case 2:
        return test_outcome_of("a number with a letter in it is no number",
                               test_a_number_with_a_letter_in_it_is_no_number());
    case 3:
        return test_outcome_of("a number beyond a billion is no number", test_a_number_beyond_a_billion_is_no_number());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_widen_test(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= SUITE_TEST_LIMIT)) {
        return test_outcome_missing();
    }
    switch (index) {
    case 1:
        return test_outcome_of("a letter of two bytes is widened to one wide letter",
                               test_a_letter_of_two_bytes_is_widened_to_one_wide_letter());
    case 2:
        return test_outcome_of("a letter of three bytes is widened to one wide letter",
                               test_a_letter_of_three_bytes_is_widened_to_one_wide_letter());
    case 3:
        return test_outcome_of("a letter beyond the basic plane is widened to two",
                               test_a_letter_beyond_the_basic_plane_is_widened_to_two());
    case 4:
        return test_outcome_of("a byte that starts no letter is not widened",
                               test_a_byte_that_starts_no_letter_is_not_widened());
    case 5:
        return test_outcome_of("a text that starts inside a letter is not widened",
                               test_a_text_that_starts_inside_a_letter_is_not_widened());
    case 6:
        return test_outcome_of("a letter cut short is not widened", test_a_letter_cut_short_is_not_widened());
    case 7:
        return test_outcome_of("a text longer than its place is not widened",
                               test_a_text_longer_than_its_place_is_not_widened());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_tsv_test(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= SUITE_TEST_LIMIT)) {
        return test_outcome_missing();
    }
    switch (index) {
    case 1:
        return test_outcome_of("a field that is a dash reads as empty", test_a_field_that_is_a_dash_reads_as_empty());
    case 2:
        return test_outcome_of("a field the line lacks reads as no text",
                               test_a_field_the_line_lacks_reads_as_no_text());
    case 3:
        return test_outcome_of("hundredths keep two digits after the point",
                               test_hundredths_keep_two_digits_after_the_point());
    case 4:
        return test_outcome_of("hundredths of one digit after the point end in zero",
                               test_hundredths_of_one_digit_after_the_point_end_in_zero());
    case 5:
        return test_outcome_of("a second point makes no hundredths", test_a_second_point_makes_no_hundredths());
    case 6:
        return test_outcome_of("a decimal with a minus sign is negative",
                               test_a_decimal_with_a_minus_sign_is_negative());
    case 7:
        return test_outcome_of("a decimal with a letter in it is no decimal",
                               test_a_decimal_with_a_letter_in_it_is_no_decimal());
    case 8:
        return test_outcome_of("a minus sign alone is no decimal", test_a_minus_sign_alone_is_no_decimal());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_escape_test(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= SUITE_TEST_LIMIT)) {
        return test_outcome_missing();
    }
    switch (index) {
    case 1:
        return test_outcome_of("an escaped letter of four hex digits is read",
                               test_an_escaped_letter_of_four_hex_digits_is_read());
    case 2:
        return test_outcome_of("an escape with capital hex digits is read",
                               test_an_escape_with_capital_hex_digits_is_read());
    case 3:
        return test_outcome_of("an escaped pair of surrogates is one letter",
                               test_an_escaped_pair_of_surrogates_is_one_letter());
    case 4:
        return test_outcome_of("an escape that is not hex is unreadable",
                               test_an_escape_that_is_not_hex_is_unreadable());
    case 5:
        return test_outcome_of("a high surrogate alone is unreadable", test_a_high_surrogate_alone_is_unreadable());
    case 6:
        return test_outcome_of("a high surrogate before a plain letter is unreadable",
                               test_a_high_surrogate_before_a_plain_letter_is_unreadable());
    case 7:
        return test_outcome_of("an escaped new line is read", test_an_escaped_new_line_is_read());
    case 8:
        return test_outcome_of("an escaped tab is read", test_an_escaped_tab_is_read());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_json_test(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= SUITE_TEST_LIMIT)) {
        return test_outcome_missing();
    }
    switch (index) {
    case 1:
        return test_outcome_of("an escaped carriage return is read", test_an_escaped_carriage_return_is_read());
    case 2:
        return test_outcome_of("an escaped quote is read", test_an_escaped_quote_is_read());
    case 3:
        return test_outcome_of("a string longer than its place is not copied",
                               test_a_string_longer_than_its_place_is_not_copied());
    case 4:
        return test_outcome_of("a text that is a list is no civ file", test_a_text_that_is_a_list_is_no_civ_file());
    case 5:
        return test_outcome_of("an object left open is unreadable", test_an_object_left_open_is_unreadable());
    case 6:
        return test_outcome_of("edits made out of order are written in the file's order",
                               test_edits_made_out_of_order_are_written_in_the_files_order());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_edge_test(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= SUITE_TEST_LIMIT)) {
        return test_outcome_missing();
    }
    switch (index) {
    case 1:
        return test_outcome_of("an escape with the hex digits a and f is read",
                               test_an_escape_with_the_hex_digits_a_and_f_is_read());
    case 2:
        return test_outcome_of("an escape with the capital hex digit f is read",
                               test_an_escape_with_the_capital_hex_digit_f_is_read());
    case 3:
        return test_outcome_of("the first letter of two bytes is read", test_the_first_letter_of_two_bytes_is_read());
    case 4:
        return test_outcome_of("the first letter of three bytes is read",
                               test_the_first_letter_of_three_bytes_is_read());
    case 5:
        return test_outcome_of("the first letter beyond the basic plane is read",
                               test_the_first_letter_beyond_the_basic_plane_is_read());
    case 6:
        return test_outcome_of("the last letter of unicode is read", test_the_last_letter_of_unicode_is_read());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_pack_test(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= SUITE_TEST_LIMIT)) {
        return test_outcome_missing();
    }
    switch (index) {
    case 1:
        return test_outcome_of("a stream of dynamic codes with repeated lengths inflates",
                               test_a_stream_of_dynamic_codes_with_repeated_lengths_inflates());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_text_test(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= SUITE_TEST_LIMIT)) {
        return test_outcome_missing();
    }
    switch (index) {
    case 1:
        return test_outcome_of("a text is not equal to a longer one that starts with it",
                               test_a_text_is_not_equal_to_a_longer_one_that_starts_with_it());
    case 2:
        return test_outcome_of("a text is not equal to its own beginning",
                               test_a_text_is_not_equal_to_its_own_beginning());
    case 3:
        return test_outcome_of("a text that fills its place is copied", test_a_text_that_fills_its_place_is_copied());
    case 4:
        return test_outcome_of("a text one letter too long for its place is not copied",
                               test_a_text_one_letter_too_long_for_its_place_is_not_copied());
    case 5:
        return test_outcome_of("a text that does not fit is not appended",
                               test_a_text_that_does_not_fit_is_not_appended());
    case 6:
        return test_outcome_of("a negative number is appended with its minus sign",
                               test_a_negative_number_is_appended_with_its_minus_sign());
    case 7:
        return test_outcome_of("a capital z is found by a small z", test_a_capital_z_is_found_by_a_small_z());
    case 8:
        return test_outcome_of("a text does not contain a letter it lacks",
                               test_a_text_does_not_contain_a_letter_it_lacks());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_writer_test(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= SUITE_TEST_LIMIT)) {
        return test_outcome_missing();
    }
    switch (index) {
    case 1:
        return test_outcome_of("two members are written with a comma between",
                               test_two_members_are_written_with_a_comma_between());
    case 2:
        return test_outcome_of("the first member of an object inside another has no comma",
                               test_the_first_member_of_an_object_inside_another_has_no_comma());
    case 3:
        return test_outcome_of("an object left open is not a finished text",
                               test_an_object_left_open_is_not_a_finished_text());
    case 4:
        return test_outcome_of("a text too long for the writer is reported",
                               test_a_text_too_long_for_the_writer_is_reported());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_lines_test(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= SUITE_TEST_LIMIT)) {
        return test_outcome_missing();
    }
    switch (index) {
    case 1:
        return test_outcome_of("a line of a file is split at its tabs", test_a_line_of_a_file_is_split_at_its_tabs());
    case 2:
        return test_outcome_of("a number field of a file is read", test_a_number_field_of_a_file_is_read());
    case 3:
        return test_outcome_of("a text field of a file is read", test_a_text_field_of_a_file_is_read());
    case 4:
        return test_outcome_of("a list of numbers is read in its order", test_a_list_of_numbers_is_read_in_its_order());
    case 5:
        return test_outcome_of("a list of slots is read as bits", test_a_list_of_slots_is_read_as_bits());
    case 6:
        return test_outcome_of("the second line of a file follows the first",
                               test_the_second_line_of_a_file_follows_the_first());
    case 7:
        return test_outcome_of("a file ends after its last line", test_a_file_ends_after_its_last_line());
    case 8:
        return test_outcome_of("a file that is missing is not opened", test_a_file_that_is_missing_is_not_opened());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_catalog_test(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= SUITE_TEST_LIMIT)) {
        return test_outcome_missing();
    }
    switch (index) {
    case 1:
        return test_outcome_of("the catalog holds every civ bonus of its file",
                               test_the_catalog_holds_every_civ_bonus_of_its_file());
    case 2:
        return test_outcome_of("the catalog holds every team bonus of its file",
                               test_the_catalog_holds_every_team_bonus_of_its_file());
    case 3:
        return test_outcome_of("the catalog holds every unit of its file",
                               test_the_catalog_holds_every_unit_of_its_file());
    case 4:
        return test_outcome_of("the catalog holds every tech of its file",
                               test_the_catalog_holds_every_tech_of_its_file());
    case 5:
        return test_outcome_of("the catalog holds every node of its file",
                               test_the_catalog_holds_every_node_of_its_file());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_units_test(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= SUITE_TEST_LIMIT)) {
        return test_outcome_missing();
    }
    switch (index) {
    case 1:
        return test_outcome_of("the first unit of the catalog has its name",
                               test_the_first_unit_of_the_catalog_has_its_name());
    case 2:
        return test_outcome_of("a units speed is kept in hundredths", test_a_units_speed_is_kept_in_hundredths());
    case 3:
        return test_outcome_of("the first unit of the catalog is a unit of the game",
                               test_the_first_unit_of_the_catalog_is_a_unit_of_the_game());
    case 4:
        return test_outcome_of("a catalog folder that is missing is not loaded",
                               test_a_catalog_folder_that_is_missing_is_not_loaded());
    default:
        return test_outcome_missing();
    }
}

TestOutcome core_suite_run(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= SUITE_TEST_LIMIT)) {
        return test_outcome_missing();
    }
    if (index <= NUMBER_END) {
        return run_number_test(index);
    }
    if (index <= WIDEN_END) {
        return run_widen_test(index - NUMBER_END);
    }
    if (index <= TSV_END) {
        return run_tsv_test(index - WIDEN_END);
    }
    if (index <= ESCAPE_END) {
        return run_escape_test(index - TSV_END);
    }
    if (index <= JSON_END) {
        return run_json_test(index - ESCAPE_END);
    }
    if (index <= EDGE_END) {
        return run_edge_test(index - JSON_END);
    }
    if (index <= PACK_END) {
        return run_pack_test(index - EDGE_END);
    }
    if (index <= TEXT_END) {
        return run_text_test(index - PACK_END);
    }
    if (index <= WRITER_END) {
        return run_writer_test(index - TEXT_END);
    }
    if (index <= LINES_END) {
        return run_lines_test(index - WRITER_END);
    }
    if (index <= CATALOG_END) {
        return run_catalog_test(index - LINES_END);
    }
    return run_units_test(index - CATALOG_END);
}
