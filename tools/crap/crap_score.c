#include "crap_score.h"

#include <stdbool.h>
#include <stddef.h>

#include "../../src/core/check.h"
#include "../../tests/scan/source_scan.h"

enum { KEYWORD_COUNT = 4, KEYWORD_WIDTH = 8, COMPLEXITY_LIMIT = 1000, SHARE_LIMIT = 1000 };

static const char KEYWORDS[KEYWORD_COUNT][KEYWORD_WIDTH] = { "if", "for", "while", "case" };

long crap_score_milli(int complexity, int covered, int total)
{
    long long missed;
    long long whole;
    long long risk;

    if (!CHECK(complexity > 0) || !CHECK(complexity <= COMPLEXITY_LIMIT) || !CHECK(covered >= 0) ||
        !CHECK(covered <= total)) {
        return 0L;
    }
    if (total == 0) {
        return (long)complexity * CRAP_MILLI;
    }
    if (total > SHARE_LIMIT) {
        covered = (int)(((long long)covered * SHARE_LIMIT) / total);
        total = SHARE_LIMIT;
    }
    missed = (long long)(total - covered);
    whole = (long long)total * total * total;
    risk = ((long long)complexity * complexity * CRAP_MILLI * missed * missed * missed) / whole;
    return (long)risk + ((long)complexity * CRAP_MILLI);
}

static bool word_at(const char *text, int at, int end, const char *word)
{
    int index;

    if (!CHECK(text != NULL) || !CHECK(word != NULL) || !CHECK(at >= 0)) {
        return false;
    }
    if ((at > 0) && source_is_word_letter(text[at - 1])) {
        return false;
    }
    for (index = 0; (index < KEYWORD_WIDTH) && (word[index] != '\0'); ++index) {
        if ((at + index >= end) || (text[at + index] != word[index])) {
            return false;
        }
    }
    return (at + index >= end) || !source_is_word_letter(text[at + index]);
}

static int branches_at(const char *text, int at, int end)
{
    int keyword;

    if (!CHECK(text != NULL) || !CHECK(at < end)) {
        return 0;
    }
    if (text[at] == '?') {
        return 1;
    }
    if ((at + 1 < end) && (text[at] == text[at + 1]) && ((text[at] == '&') || (text[at] == '|'))) {
        return 1;
    }
    for (keyword = 0; keyword < KEYWORD_COUNT; ++keyword) {
        if (word_at(text, at, end, KEYWORDS[keyword])) {
            return 1;
        }
    }
    return 0;
}

int crap_complexity(const char *text, int start, int end)
{
    int complexity = 1;
    int at;

    if (!CHECK(text != NULL) || !CHECK(start >= 0) || !CHECK(start <= end) || !CHECK(end <= SCAN_TEXT_CAPACITY)) {
        return 1;
    }
    for (at = start; (at < end) && (at < SCAN_TEXT_CAPACITY); ++at) {
        complexity += branches_at(text, at, end);
        if (word_at(text, at, end, "CHECK")) {
            --complexity;
        }
    }
    return (complexity > 1) ? complexity : 1;
}
