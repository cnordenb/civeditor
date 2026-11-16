#include "lines_tests.h"

#include <string.h>

#include "../../src/core/check.h"
#include "../../src/core/file_io.h"
#include "../../src/core/text.h"
#include "../../src/data/tsv.h"

enum { VALUE_CAPACITY = 16, NUMBER_FIELD = 0, WORD_FIELD = 1, LIST_FIELD = 3, SLOT_FIELD = 4, LAST_SLOT = 63 };

static const wchar_t PATH[] = L"build\\test_lines.txt";
static const char LINES[] = "7\tword\t-\t3,5,8\t1,63\r\nsecond\tline\n";

static TsvFile file;
static TsvRecord record;

static bool read_lines(int count)
{
    bool read = true;
    int line;

    if (!CHECK(file_write_all(PATH, LINES, strlen(LINES))) || !CHECK(tsv_open(&file, PATH))) {
        return false;
    }
    for (line = 0; (line < count) && (line < TSV_FIELD_LIMIT); ++line) {
        read = tsv_next(&file, &record);
    }
    return read;
}

bool test_a_line_of_a_file_is_split_at_its_tabs(void)
{
    return read_lines(1) && (record.field_count == 5);
}

bool test_a_number_field_of_a_file_is_read(void)
{
    return read_lines(1) && (tsv_number(&record, NUMBER_FIELD) == 7);
}

bool test_a_text_field_of_a_file_is_read(void)
{
    char value[VALUE_CAPACITY] = "";

    return read_lines(1) && tsv_text(&record, WORD_FIELD, value, sizeof value) && text_equal(value, "word");
}

bool test_a_list_of_numbers_is_read_in_its_order(void)
{
    TsvNumberList list = { { 0 }, 0 };

    if (read_lines(1)) {
        list = tsv_number_list(&record, LIST_FIELD);
    }
    return (list.count == 3) && (list.values[0] == 3) && (list.values[2] == 8);
}

bool test_a_list_of_slots_is_read_as_bits(void)
{
    return read_lines(1) && (tsv_bits(&record, SLOT_FIELD) == ((1ULL << 1) | (1ULL << LAST_SLOT)));
}

bool test_the_second_line_of_a_file_follows_the_first(void)
{
    char value[VALUE_CAPACITY] = "";

    return read_lines(2) && tsv_text(&record, 0, value, sizeof value) && text_equal(value, "second");
}

bool test_a_file_ends_after_its_last_line(void)
{
    return !read_lines(3);
}

bool test_a_file_that_is_missing_is_not_opened(void)
{
    return !tsv_open(&file, L"build\\no_such_lines.txt");
}
