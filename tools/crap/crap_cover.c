#include "crap_cover.h"

#include <stdio.h>

#include "../../src/core/check.h"
#include "../../src/core/text.h"

enum { LINE_CAPACITY = 512, LINE_LIMIT = 100000, COMMAND_CAPACITY = 600, DIGIT_LIMIT = 9, BRANCH_LIMIT = 250 };

static const char BRANCH_MARK[] = "branch";
static const char TAKEN_MARK[] = "taken ";
static const char CHECK_MARK[] = "CHECK(";

void crap_cover_start(LineCoverage *coverage)
{
    int line;

    if (!CHECK(coverage != NULL) || !CHECK(SCAN_LINE_LIMIT > 0)) {
        return;
    }
    for (line = 0; line <= SCAN_LINE_LIMIT; ++line) {
        coverage->ran[line] = false;
        coverage->covered[line] = 0U;
        coverage->total[line] = 0U;
    }
    coverage->line = 0;
    coverage->counted = false;
}

static int number_at(const char *text, int at)
{
    int value = 0;
    int step;

    if (!CHECK(text != NULL) || !CHECK(at >= 0)) {
        return 0;
    }
    for (step = 0; (step < DIGIT_LIMIT) && (text[at + step] >= '0') && (text[at + step] <= '9'); ++step) {
        value = (value * 10) + (text[at + step] - '0');
    }
    return value;
}

static int after_mark(const char *line, const char *mark)
{
    int at;

    if (!CHECK(line != NULL) || !CHECK(mark != NULL)) {
        return -1;
    }
    for (at = 0; (at < LINE_CAPACITY) && (line[at] != '\0'); ++at) {
        if (text_starts_with(&line[at], mark)) {
            return at + (int)text_length(mark, LINE_CAPACITY);
        }
    }
    return -1;
}

static int after_spaces(const char *line, int at)
{
    int step;

    if (!CHECK(line != NULL) || !CHECK(at >= 0)) {
        return 0;
    }
    for (step = 0; (step < LINE_CAPACITY) && (line[at] == ' '); ++step) {
        ++at;
    }
    return at;
}

static bool take_source_line(LineCoverage *coverage, const char *line)
{
    int count_at = after_spaces(line, 0);
    int colon = after_mark(line, ":");
    int space = after_mark(&line[count_at], " ");
    int number;

    if (!CHECK(coverage != NULL) || !CHECK(line != NULL) || (colon < 0)) {
        return false;
    }
    number = number_at(line, after_spaces(line, colon));
    if ((number <= 0) || (number > SCAN_LINE_LIMIT) || ((space >= 0) && (space < colon - count_at))) {
        return false;
    }
    coverage->line = number;
    coverage->counted = after_mark(line, CHECK_MARK) < 0;
    coverage->ran[number] = number_at(line, count_at) > 0;
    return true;
}

bool crap_cover_take_line(LineCoverage *coverage, const char *line)
{
    int taken;

    if (!CHECK(coverage != NULL) || !CHECK(line != NULL)) {
        return false;
    }
    if (!text_starts_with(line, BRANCH_MARK)) {
        return take_source_line(coverage, line);
    }
    if (!coverage->counted || (coverage->line <= 0) || (coverage->total[coverage->line] >= BRANCH_LIMIT)) {
        return true;
    }
    coverage->total[coverage->line] += 1U;
    taken = after_mark(line, TAKEN_MARK);
    if ((taken >= 0) && (number_at(line, taken) > 0)) {
        coverage->covered[coverage->line] += 1U;
    }
    return true;
}

bool crap_cover_read(LineCoverage *coverage, const char *source_path)
{
    char command[COMMAND_CAPACITY] = "gcov -b -c -t -o build/cover ";
    char line[LINE_CAPACITY];
    FILE *output;
    int step;

    if (!CHECK(coverage != NULL) || !CHECK(source_path != NULL)) {
        return false;
    }
    crap_cover_start(coverage);
    if (!text_append(command, sizeof command, source_path) || !text_append(command, sizeof command, " 2>nul")) {
        return false;
    }
    output = _popen(command, "r");
    if (output == NULL) {
        return false;
    }
    for (step = 0; step < LINE_LIMIT; ++step) {
        if (fgets(line, (int)sizeof line, output) == NULL) {
            break;
        }
        (void)crap_cover_take_line(coverage, line);
    }
    (void)_pclose(output);
    return true;
}

CoveredShare crap_cover_share(const LineCoverage *coverage, int first_line, int last_line)
{
    CoveredShare share = { 0, 0 };
    bool ran = false;
    int line;

    if (!CHECK(coverage != NULL) || !CHECK(first_line > 0) || !CHECK(first_line <= last_line)) {
        return share;
    }
    for (line = first_line; (line <= last_line) && (line <= SCAN_LINE_LIMIT); ++line) {
        share.covered += coverage->covered[line];
        share.total += coverage->total[line];
        ran = ran || coverage->ran[line];
    }
    if (share.total == 0) {
        share.total = 1;
        share.covered = ran ? 1 : 0;
    }
    return share;
}
