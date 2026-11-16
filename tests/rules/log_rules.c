#include "../maintainability/doc_rules.h"

#include "../../src/core/check.h"
#include "../../src/core/file_io.h"
#include "../../src/core/text.h"
#include "../../src/data/tsv.h"
#include "../test_outcome.h"

enum {
    PART_LINE_LIMIT = 1000,
    PART_LIMIT = 64,
    PART_DIGIT_LIMIT = 9,
    DAY_CAPACITY = 64,
    DAY_WIDTH = 16,
    TEST_LIMIT = 100,
    FIELD_LIMIT = 120,
    TERM_LIMIT = 120,
    HEADING_CAPACITY = 64
};

typedef struct {
    char days[DAY_CAPACITY][DAY_WIDTH];
    int day_count;
    char listed[DAY_CAPACITY][DAY_WIDTH];
    int listed_count;
} DayIndex;

static TsvFile document;
static TsvRecord line;
static DayIndex index_of_days;

static bool has_shape(const char *text, const char *shape, int length)
{
    int index;

    if (!CHECK(text != NULL) || !CHECK(shape != NULL) || !CHECK(length < DAY_WIDTH)) {
        return false;
    }
    for (index = 0; (index < length) && (index < DAY_WIDTH); ++index) {
        if ((shape[index] == 'd') ? !text_is_digit(text[index]) : (text[index] != shape[index])) {
            return false;
        }
    }
    return true;
}

static bool is_date(const char *text)
{
    static const char SHAPE[] = "dddd-dd-dd";

    if (!CHECK(text != NULL) || !CHECK(SHAPE[0] == 'd')) {
        return false;
    }
    return has_shape(text, SHAPE, DAY_WIDTH - 6) && (text[DAY_WIDTH - 6] == '\0');
}

static bool is_hour(const char *digits)
{
    return CHECK(digits != NULL) && ((digits[0] < '2') || (digits[1] < '4'));
}

static bool time_title_is_valid(const char *text)
{
    static const char SHAPE[] = "dd:dd:dd: ";

    if (!CHECK(text != NULL) || !text_is_digit(text[4])) {
        return true;
    }
    return has_shape(&text[4], SHAPE, (int)sizeof SHAPE - 1) && is_hour(&text[4]) && (text[7] < '6') &&
           (text[10] < '6');
}

static bool note_day(const char *text, bool listed)
{
    if (!CHECK(text != NULL) || !CHECK(index_of_days.day_count <= DAY_CAPACITY)) {
        return false;
    }
    if (listed) {
        return (index_of_days.listed_count < DAY_CAPACITY) &&
               text_copy(index_of_days.listed[index_of_days.listed_count++], DAY_WIDTH, text);
    }
    return is_date(text) && (index_of_days.day_count < DAY_CAPACITY) &&
           text_copy(index_of_days.days[index_of_days.day_count++], DAY_WIDTH, text);
}

static bool read_devlog_line(int number)
{
    char day[DAY_WIDTH] = "";

    if (!CHECK(number > 0) || !CHECK(line.text[0] != '\1')) {
        return false;
    }
    if (text_starts_with(line.text, "## ")) {
        return note_day(&line.text[3], false);
    }
    if (text_starts_with(line.text, "- [") && (text_length(line.text, TSV_LINE_CAPACITY) > 4U)) {
        (void)text_copy_span(day, sizeof day, &line.text[3], 10U);
        return !is_date(day) || !text_starts_with(&line.text[13], "](#") || note_day(day, true);
    }
    return !text_starts_with(line.text, "### ") || time_title_is_valid(line.text);
}

static bool part_path(int part, wchar_t *path)
{
    static const wchar_t FOLDER[] = L"documentation/DEVLOG/DEVLOG";
    int at = (int)(sizeof FOLDER / sizeof FOLDER[0]) - 1;

    if (!CHECK(part > 0) || !CHECK(part < PART_LIMIT) || !CHECK(path != NULL) || (part > PART_DIGIT_LIMIT)) {
        return false;
    }
    (void)wcscpy(path, FOLDER);
    path[at] = (wchar_t)(L'0' + part);
    (void)wcscpy(&path[at + 1], L".md");
    return file_exists(path);
}

static bool lines_keep_form(void)
{
    int number;

    if (!CHECK(PART_LINE_LIMIT > 0) || !CHECK(line.text[0] != '\1')) {
        return false;
    }
    for (number = 2; (number <= PART_LINE_LIMIT + 1) && tsv_next(&document, &line); ++number) {
        if ((number > PART_LINE_LIMIT) || !read_devlog_line(number)) {
            test_note(line.text);
            return false;
        }
    }
    return true;
}

static bool part_keeps_form(int part)
{
    wchar_t path[FILE_PATH_CAPACITY];
    char title[HEADING_CAPACITY] = "# Development log, part ";

    if (!CHECK(part > 0) || !CHECK(part < PART_LIMIT) || !part_path(part, path)) {
        return false;
    }
    (void)text_append_number(title, sizeof title, part);
    if (!tsv_open(&document, path) || !tsv_next(&document, &line) || !text_equal(line.text, title)) {
        return false;
    }
    return lines_keep_form();
}

static bool comes_before(const char *earlier, const char *later)
{
    int index;

    if (!CHECK(earlier != NULL) || !CHECK(later != NULL)) {
        return false;
    }
    for (index = 0; index < DAY_WIDTH; ++index) {
        if (earlier[index] != later[index]) {
            return earlier[index] < later[index];
        }
        if (earlier[index] == '\0') {
            return false;
        }
    }
    return false;
}

static bool day_is_in_place(int day)
{
    if (!CHECK(day >= 0) || !CHECK(day < DAY_CAPACITY)) {
        return false;
    }
    return text_equal(index_of_days.days[day], index_of_days.listed[day]) &&
           ((day == 0) || comes_before(index_of_days.days[day - 1], index_of_days.days[day]));
}

static bool days_are_listed_in_order(void)
{
    int day;

    if (!CHECK(index_of_days.day_count > 0) || (index_of_days.day_count != index_of_days.listed_count)) {
        return false;
    }
    for (day = 0; (day < index_of_days.day_count) && (day < DAY_CAPACITY); ++day) {
        if (!day_is_in_place(day)) {
            return false;
        }
    }
    return true;
}

static bool follows_part_before(const char *last_day, int days_before)
{
    if (!CHECK(last_day != NULL) || !CHECK(index_of_days.day_count > 0)) {
        return false;
    }
    if (text_equal(index_of_days.days[0], last_day)) {
        return days_before < 2;
    }
    return (last_day[0] == '\0') || comes_before(last_day, index_of_days.days[0]);
}

bool devlog_keeps_its_form(void)
{
    wchar_t path[FILE_PATH_CAPACITY];
    char last_day[DAY_WIDTH] = "";
    int days_before = 0;
    int part;

    for (part = 1; (part < PART_LIMIT) && ((part == 1) || part_path(part, path)); ++part) {
        index_of_days.day_count = 0;
        index_of_days.listed_count = 0;
        if (!part_keeps_form(part) || !days_are_listed_in_order() || !follows_part_before(last_day, days_before)) {
            return false;
        }
        days_before = index_of_days.day_count;
        (void)text_copy(last_day, DAY_WIDTH, index_of_days.days[days_before - 1]);
    }
    return CHECK(part > 1);
}

static int heading_number(const char *prefix, int prefix_length)
{
    TextNumber number;
    int length;

    if (!CHECK(prefix != NULL) || !text_starts_with(line.text, prefix)) {
        return 0;
    }
    for (length = 0; length < TEXT_NUMBER_CAPACITY - 1; ++length) {
        if (!text_is_digit(line.text[prefix_length + length])) {
            break;
        }
    }
    number = text_to_number(&line.text[prefix_length], (size_t)length);
    return (number.valid && (length > 0)) ? (int)number.value : 0;
}

static bool field_line_is_too_long(bool in_field)
{
    if (!CHECK(FIELD_LIMIT > 0) || !CHECK(line.text[0] != '\1')) {
        return false;
    }
    return in_field && !text_starts_with(line.text, "### ") &&
           (text_length(line.text, TSV_LINE_CAPACITY) > (size_t)FIELD_LIMIT);
}

static bool takes_heading(int *headings)
{
    int number = heading_number("## Test ", 8);

    if (!CHECK(headings != NULL) || (number <= 0)) {
        return true;
    }
    ++*headings;
    return number == *headings;
}

static bool is_in_a_field(bool before)
{
    if (text_starts_with(line.text, "### ")) {
        return true;
    }
    return text_starts_with(line.text, "## ") ? false : before;
}

static int next_listed(int listed)
{
    return (heading_number("- [Test ", 8) == listed + 1) ? listed + 1 : listed;
}

static bool counts_agree(int headings, int listed)
{
    return CHECK(headings >= 0) && (headings == listed) && (headings <= TEST_LIMIT);
}

bool tests_document_is_numbered_in_order(void)
{
    int headings = 0;
    int listed = 0;
    int step;
    bool in_field = false;

    if (!CHECK(TEST_LIMIT > 0) || !tsv_open(&document, L"documentation/TESTS.md")) {
        return false;
    }
    for (step = 0; (step < TSV_FILE_CAPACITY) && tsv_next(&document, &line); ++step) {
        listed = next_listed(listed);
        in_field = is_in_a_field(in_field);
        if (!takes_heading(&headings) || field_line_is_too_long(in_field)) {
            test_note(line.text);
            return false;
        }
    }
    return counts_agree(headings, listed);
}

static int count_cases(const wchar_t *path)
{
    int count = 0;
    int step;

    if (!CHECK(path != NULL) || !tsv_open(&document, path)) {
        return -1;
    }
    for (step = 0; (step < TSV_FILE_CAPACITY) && tsv_next(&document, &line); ++step) {
        count += text_starts_with(line.text, "    case ") ? 1 : 0;
    }
    return count;
}

bool tests_document_counts_every_test(void)
{
    int documented = 0;
    int step;
    int defined = count_cases(L"tests/functional/functional_suite.c") +
                  count_cases(L"tests/suites/build_suite.c") + count_cases(L"tests/suites/mod_suite.c") +
                  count_cases(L"tests/suites/core_suite.c") + count_cases(L"tests/suites/model_suite.c") +
                  count_cases(L"tests/suites/shell_suite.c") +
                  count_cases(L"tests/accessibility/accessibility_suite.c") +
                  count_cases(L"tests/maintainability/maintainability_suite.c");

    if (!CHECK(defined > 0) || !tsv_open(&document, L"documentation/TESTS.md")) {
        return false;
    }
    for (step = 0; (step < TSV_FILE_CAPACITY) && tsv_next(&document, &line); ++step) {
        documented += (heading_number("## Test ", 8) > 0) ? 1 : 0;
    }
    return documented == defined;
}

bool terminology_lines_are_short(void)
{
    int step;

    if (!CHECK(TERM_LIMIT > 0) || !tsv_open(&document, L"documentation/TERMINOLOGY.md")) {
        return false;
    }
    for (step = 0; (step < TSV_FILE_CAPACITY) && tsv_next(&document, &line); ++step) {
        if (text_length(line.text, TSV_LINE_CAPACITY) > (size_t)TERM_LIMIT) {
            test_note(line.text);
            return false;
        }
    }
    return true;
}
