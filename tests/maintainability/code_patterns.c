#include "code_patterns.h"

#include "../../src/core/check.h"
#include "../../src/core/text.h"

enum { FLOW_WORD_COUNT = 3 };

static const char FLOW_WORDS[FLOW_WORD_COUNT][PATTERN_WORD_WIDTH] = { "goto", "setjmp", "longjmp" };

int pattern_line_at(const ScannedSource *source, int offset)
{
    int line;

    if (!CHECK(source != NULL) || !CHECK(offset >= 0)) {
        return 1;
    }
    for (line = 1; (line < source->line_count) && (line < SCAN_LINE_LIMIT); ++line) {
        if (source->line_starts[line] > offset) {
            return line;
        }
    }
    return source->line_count;
}

int pattern_find_word(const ScannedSource *source, int from, const char *word)
{
    size_t length = text_length(word, PATTERN_WORD_WIDTH);
    int index;

    if (!CHECK(source != NULL) || !CHECK(length > 0U)) {
        return -1;
    }
    for (index = from; (index < source->length) && (index < SCAN_TEXT_CAPACITY); ++index) {
        if ((index > 0) && source_is_word_letter(source->cleaned[index - 1])) {
            continue;
        }
        if (text_starts_with(&source->cleaned[index], word) &&
            !source_is_word_letter(source->cleaned[index + (int)length])) {
            return index;
        }
    }
    return -1;
}

int pattern_first_word_line(const ScannedSource *source, const char words[][PATTERN_WORD_WIDTH], int count)
{
    int index;
    int found;

    if (!CHECK(source != NULL) || !CHECK(words != NULL)) {
        return 0;
    }
    for (index = 0; (index < count) && (index < PATTERN_WORD_LIMIT); ++index) {
        found = pattern_find_word(source, 0, words[index]);
        if (found >= 0) {
            return pattern_line_at(source, found);
        }
    }
    return 0;
}

int pattern_forbidden_flow_line(const ScannedSource *source)
{
    return pattern_first_word_line(source, FLOW_WORDS, FLOW_WORD_COUNT);
}
