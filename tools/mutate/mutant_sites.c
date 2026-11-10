#include "mutant_sites.h"

#include <stddef.h>

#include "../../src/core/check.h"
#include "../../src/core/text.h"
#include "../../tests/scan/source_scan.h"

enum { SWAP_COUNT = 6, PASSED_COUNT = 7, WORD_COUNT = 4, WORD_WIDTH = 8, PAIR_WIDTH = 4, LOOK_BACK_LIMIT = 256,
       CHECK_WIDTH = 5 };

static const char SWAPS[SWAP_COUNT][2][PAIR_WIDTH] = {
    { "==", "!=" }, { "!=", "==" }, { "&&", "||" }, { "||", "&&" }, { "<=", "<" }, { ">=", ">" }
};
static const char PASSED[PASSED_COUNT][PAIR_WIDTH] = { "<<", ">>", "++", "--", "+=", "-=", "->" };
static const char WORDS[WORD_COUNT][2][WORD_WIDTH] = {
    { "true", "false" }, { "false", "true" }, { "0", "1" }, { "1", "0" }
};

static MutantSite passed(int at, int skip)
{
    MutantSite site = { 0, 0, 1, "", false };

    if (!CHECK(at >= 0) || !CHECK(skip > 0)) {
        return site;
    }
    site.at = at;
    site.skip = skip;
    return site;
}

static MutantSite site_of(int at, int length, const char *with)
{
    MutantSite site = passed(at, length);

    if (!CHECK(with != NULL) || !CHECK(length > 0)) {
        return site;
    }
    site.length = length;
    site.found = text_copy(site.with, sizeof site.with, with);
    return site;
}

static bool pair_is(const char *text, int at, int end, const char *pair)
{
    if (!CHECK(text != NULL) || !CHECK(pair != NULL)) {
        return false;
    }
    return (at + 1 < end) && (text[at] == pair[0]) && (text[at + 1] == pair[1]);
}

static bool follows_operand(const char *text, int at)
{
    char last = '\0';
    int index;

    if (!CHECK(text != NULL) || !CHECK(at >= 0)) {
        return false;
    }
    for (index = at - 1; (index >= 0) && (index > at - LOOK_BACK_LIMIT); --index) {
        if ((text[index] != ' ') && (text[index] != '\n') && (text[index] != '\r') && (text[index] != '\t')) {
            last = text[index];
            break;
        }
    }
    return source_is_word_letter(last) || (last == ')') || (last == ']');
}

static MutantSite pair_site(const char *text, int at, int end)
{
    int index;

    if (!CHECK(text != NULL) || !CHECK(at < end)) {
        return passed(0, 1);
    }
    for (index = 0; index < PASSED_COUNT; ++index) {
        if (pair_is(text, at, end, PASSED[index])) {
            return passed(at, 2);
        }
    }
    for (index = 0; index < SWAP_COUNT; ++index) {
        if (pair_is(text, at, end, SWAPS[index][0])) {
            return site_of(at, 2, SWAPS[index][1]);
        }
    }
    return passed(at, 1);
}

static MutantSite single_site(const char *text, int at, int end)
{
    if (!CHECK(text != NULL) || !CHECK(at < end)) {
        return passed(0, 1);
    }
    switch (text[at]) {
    case '<':
        return site_of(at, 1, "<=");
    case '>':
        return site_of(at, 1, ">=");
    case '!':
        return site_of(at, 1, " ");
    case '+':
        return follows_operand(text, at) ? site_of(at, 1, "-") : passed(at, 1);
    case '-':
        return site_of(at, 1, follows_operand(text, at) ? "+" : " ");
    default:
        return passed(at, 1);
    }
}

static MutantSite word_site(const char *text, int at, int end)
{
    int stop;
    int index;

    if (!CHECK(text != NULL) || !CHECK(at < end)) {
        return passed(0, 1);
    }
    for (stop = at; (stop < end) && (stop < SCAN_TEXT_CAPACITY); ++stop) {
        if (!source_is_word_letter(text[stop])) {
            break;
        }
    }
    if (((at > 0) && (text[at - 1] == '.')) || ((stop < end) && (text[stop] == '.'))) {
        return passed(at, stop - at);
    }
    for (index = 0; index < WORD_COUNT; ++index) {
        if (source_span_is(text, at, stop, WORDS[index][0])) {
            return site_of(at, stop - at, WORDS[index][1]);
        }
    }
    return passed(at, stop - at);
}

static int after_blanks(const char *text, int at, int end)
{
    int step;

    if (!CHECK(text != NULL) || !CHECK(at >= 0)) {
        return end;
    }
    for (step = 0; (step < LOOK_BACK_LIMIT) && (at < end); ++step) {
        if ((text[at] != ' ') && (text[at] != '\n') && (text[at] != '\r') && (text[at] != '\t')) {
            break;
        }
        ++at;
    }
    return at;
}

static int check_length(const char *text, int at, int end)
{
    int from = at;
    int depth = 0;
    int index;

    if (!CHECK(text != NULL) || !CHECK(at < end)) {
        return 0;
    }
    if (pair_is(text, from, end, "||")) {
        from = after_blanks(text, from + 2, end);
    }
    if ((from < end) && (text[from] == '!')) {
        ++from;
    }
    if ((from + CHECK_WIDTH >= end) || !source_span_is(text, from, from + CHECK_WIDTH, "CHECK") ||
        (text[from + CHECK_WIDTH] != '(')) {
        return 0;
    }
    for (index = from + CHECK_WIDTH; (index < end) && (index < SCAN_TEXT_CAPACITY); ++index) {
        depth += (text[index] == '(') ? 1 : 0;
        depth -= (text[index] == ')') ? 1 : 0;
        if (depth == 0) {
            return index + 1 - at;
        }
    }
    return 0;
}

static MutantSite site_at(const char *text, int at, int end)
{
    MutantSite site;

    if (!CHECK(text != NULL) || !CHECK(at < end)) {
        return passed(0, 1);
    }
    if (check_length(text, at, end) > 0) {
        return passed(at, check_length(text, at, end));
    }
    if (source_is_word_letter(text[at])) {
        return word_site(text, at, end);
    }
    site = pair_site(text, at, end);
    if (site.found || (site.skip > 1)) {
        return site;
    }
    return single_site(text, at, end);
}

MutantSite mutant_next(const char *text, int from, int end)
{
    MutantSite site = { 0, 0, 1, "", false };
    int at = from;
    int step;

    if (!CHECK(text != NULL) || !CHECK(from >= 0) || !CHECK(end <= SCAN_TEXT_CAPACITY)) {
        return site;
    }
    for (step = 0; (step < SCAN_TEXT_CAPACITY) && (at < end); ++step) {
        site = site_at(text, at, end);
        if (site.found) {
            return site;
        }
        at += site.skip;
    }
    site.found = false;
    return site;
}
