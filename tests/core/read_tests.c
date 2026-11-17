#include "read_tests.h"

#include <limits.h>
#include <wchar.h>

#include "../../src/core/check.h"
#include "../../src/core/text.h"
#include "../../src/data/tsv.h"
#include "../../src/json/json_patch.h"

enum { WIDE_CAPACITY = 8, SMALL_CAPACITY = 2, VALUE_CAPACITY = 16, MISSING_FIELD = 3 };

static const wchar_t SMILE[] = { 0xD83D, 0xDE00, 0 };
static const wchar_t FIRST_BEYOND[] = { 0xD800, 0xDC00, 0 };

static JsonDocument document;
static JsonPatch patch;
static JsonPatchOutput patched;

static bool widens_to(const char *text, const wchar_t *expected)
{
    wchar_t wide[WIDE_CAPACITY];

    if (!CHECK(text != NULL) || !CHECK(expected != NULL)) {
        return false;
    }
    return text_widen(text, wide, WIDE_CAPACITY) && (wcscmp(wide, expected) == 0);
}

static bool is_not_widened(const char *text, size_t capacity)
{
    wchar_t wide[WIDE_CAPACITY];

    if (!CHECK(text != NULL) || !CHECK(capacity <= (size_t)WIDE_CAPACITY)) {
        return false;
    }
    return !text_widen(text, wide, capacity);
}

static TsvRecord one_field(const char *text)
{
    TsvRecord record = { "", { 0 }, { 0 }, 1 };

    if (CHECK(text != NULL) && CHECK(text_copy(record.text, sizeof record.text, text))) {
        record.lengths[0] = (int)text_length(text, sizeof record.text);
    }
    return record;
}

static bool decimal_is(const char *text, float expected)
{
    TsvRecord record = one_field(text);
    float value = 0.0F;

    if (!CHECK(text != NULL) || !tsv_decimal(&record, 0, &value)) {
        return false;
    }
    return (value > expected - 0.001F) && (value < expected + 0.001F);
}

static bool is_no_decimal(const char *text)
{
    TsvRecord record = one_field(text);
    float value = 0.0F;

    return CHECK(text != NULL) && !tsv_decimal(&record, 0, &value);
}

static bool parses(const char *json)
{
    if (!CHECK(json != NULL) || !text_copy(document.text, sizeof document.text, json)) {
        return false;
    }
    document.length = (int)text_length(json, sizeof document.text);
    return json_parse(&document);
}

static bool member_reads_as(const char *json, const char *expected)
{
    char value[VALUE_CAPACITY];

    if (!CHECK(expected != NULL) || !parses(json)) {
        return false;
    }
    return json_copy_string(&document, json_member(&document, 0, "a"), value, sizeof value) &&
           text_equal(value, expected);
}

static bool member_is_unreadable(const char *json, size_t capacity)
{
    char value[VALUE_CAPACITY];

    if (!CHECK(capacity <= sizeof value) || !parses(json)) {
        return false;
    }
    return !json_copy_string(&document, json_member(&document, 0, "a"), value, capacity);
}

bool test_a_number_with_a_minus_sign_is_negative(void)
{
    TextNumber number = text_to_number("-12", 3U);

    return number.valid && (number.value == -12L);
}

bool test_a_number_with_a_letter_in_it_is_no_number(void)
{
    return !text_to_number("1x", 2U).valid;
}

bool test_a_number_past_the_largest_long_is_no_number(void)
{
    return !text_to_number("2147483648", 10U).valid;
}

bool test_the_largest_number_a_long_holds_is_read(void)
{
    TextNumber number = text_to_number("2147483647", 10U);

    return number.valid && (number.value == LONG_MAX);
}

bool test_a_letter_of_two_bytes_is_widened_to_one_wide_letter(void)
{
    return widens_to("\xC3\xA9", L"\x00E9");
}

bool test_a_letter_of_three_bytes_is_widened_to_one_wide_letter(void)
{
    return widens_to("\xE2\x82\xAC", L"\x20AC");
}

bool test_a_letter_beyond_the_basic_plane_is_widened_to_two(void)
{
    return widens_to("\xF0\x9F\x98\x80", SMILE);
}

bool test_the_first_letter_beyond_the_basic_plane_is_widened_to_two(void)
{
    return widens_to("\xF0\x90\x80\x80", FIRST_BEYOND);
}

bool test_a_byte_that_starts_no_letter_is_not_widened(void)
{
    return is_not_widened("\xFF", WIDE_CAPACITY);
}

bool test_a_text_that_starts_inside_a_letter_is_not_widened(void)
{
    return is_not_widened("\x80", WIDE_CAPACITY);
}

bool test_a_letter_cut_short_is_not_widened(void)
{
    return is_not_widened("\xC3" "A", WIDE_CAPACITY);
}

bool test_a_text_longer_than_its_place_is_not_widened(void)
{
    return is_not_widened("abc", SMALL_CAPACITY);
}

bool test_a_field_that_is_a_dash_reads_as_empty(void)
{
    TsvRecord record = one_field("-");
    char value[VALUE_CAPACITY] = "x";

    return tsv_text(&record, 0, value, sizeof value) && (value[0] == '\0');
}

bool test_a_field_the_line_lacks_reads_as_no_text(void)
{
    TsvRecord record = one_field("word");
    char value[VALUE_CAPACITY] = "x";

    return !tsv_text(&record, MISSING_FIELD, value, sizeof value) && (value[0] == '\0');
}

static int hundredths_of(const char *text)
{
    TsvRecord record = one_field(text);

    return CHECK(text != NULL) ? tsv_hundredths(&record, 0) : TSV_MISSING;
}

static int tokens_of_unreadable(const char *json)
{
    return (CHECK(json != NULL) && !parses(json)) ? document.count : -1;
}

bool test_hundredths_keep_two_digits_after_the_point(void)
{
    return hundredths_of("1.257") == 125;
}

bool test_hundredths_of_one_digit_after_the_point_end_in_zero(void)
{
    return hundredths_of("1.5") == 150;
}

bool test_a_second_point_makes_no_hundredths(void)
{
    return hundredths_of("1.2.3") == TSV_MISSING;
}

bool test_a_decimal_with_a_minus_sign_is_negative(void)
{
    return decimal_is("-1.5", -1.5F);
}

bool test_a_decimal_with_a_letter_in_it_is_no_decimal(void)
{
    return is_no_decimal("1x");
}

bool test_a_minus_sign_alone_is_no_decimal(void)
{
    return is_no_decimal("-");
}

bool test_an_escaped_letter_of_four_hex_digits_is_read(void)
{
    return member_reads_as("{\"a\":\"\\u00e9\"}", "\xC3\xA9");
}

bool test_an_escape_with_capital_hex_digits_is_read(void)
{
    return member_reads_as("{\"a\":\"\\u20AC\"}", "\xE2\x82\xAC");
}

bool test_an_escaped_pair_of_surrogates_is_one_letter(void)
{
    return member_reads_as("{\"a\":\"\\ud83d\\ude00\"}", "\xF0\x9F\x98\x80");
}

bool test_an_escape_that_is_not_hex_is_unreadable(void)
{
    return member_is_unreadable("{\"a\":\"\\u00zz\"}", VALUE_CAPACITY);
}

bool test_a_high_surrogate_alone_is_unreadable(void)
{
    return member_is_unreadable("{\"a\":\"\\ud83dxxxxxxx\"}", VALUE_CAPACITY);
}

bool test_a_high_surrogate_before_a_plain_letter_is_unreadable(void)
{
    return member_is_unreadable("{\"a\":\"\\ud83d\\u0041\"}", VALUE_CAPACITY);
}

bool test_an_escaped_new_line_is_read(void)
{
    return member_reads_as("{\"a\":\"x\\ny\"}", "x\ny");
}

bool test_an_escaped_tab_is_read(void)
{
    return member_reads_as("{\"a\":\"x\\ty\"}", "x\ty");
}

bool test_an_escaped_carriage_return_is_read(void)
{
    return member_reads_as("{\"a\":\"x\\ry\"}", "x\ry");
}

bool test_an_escaped_quote_is_read(void)
{
    return member_reads_as("{\"a\":\"x\\\"y\"}", "x\"y");
}

bool test_a_string_longer_than_its_place_is_not_copied(void)
{
    return member_is_unreadable("{\"a\":\"abc\"}", SMALL_CAPACITY);
}

bool test_a_text_that_is_a_list_is_no_civ_file(void)
{
    return tokens_of_unreadable("[1]") == 2;
}

bool test_an_object_left_open_is_unreadable(void)
{
    return tokens_of_unreadable("{\"a\":1") == 3;
}

bool test_edits_made_out_of_order_are_written_in_the_files_order(void)
{
    if (!parses("{\"a\":1,\"b\":2}")) {
        return false;
    }
    json_patch_start(&patch, &document);
    json_patch_replace(&patch, json_member(&document, 0, "b"), "20");
    json_patch_replace(&patch, json_member(&document, 0, "a"), "10");
    return json_patch_write(&patch, &patched) && text_equal(patched.text, "{\"a\":10,\"b\":20}");
}

bool test_an_escape_with_the_hex_digits_a_and_f_is_read(void)
{
    return member_reads_as("{\"a\":\"\\u00af\"}", "\xC2\xAF");
}

bool test_an_escape_with_the_capital_hex_digit_f_is_read(void)
{
    return member_reads_as("{\"a\":\"\\u00AF\"}", "\xC2\xAF");
}

bool test_the_first_letter_of_two_bytes_is_read(void)
{
    return member_reads_as("{\"a\":\"\\u0080\"}", "\xC2\x80");
}

bool test_the_first_letter_of_three_bytes_is_read(void)
{
    return member_reads_as("{\"a\":\"\\u0800\"}", "\xE0\xA0\x80");
}

bool test_the_first_letter_beyond_the_basic_plane_is_read(void)
{
    return member_reads_as("{\"a\":\"\\ud800\\udc00\"}", "\xF0\x90\x80\x80");
}

bool test_the_last_letter_of_unicode_is_read(void)
{
    return member_reads_as("{\"a\":\"\\udbff\\udfff\"}", "\xF4\x8F\xBF\xBF");
}
