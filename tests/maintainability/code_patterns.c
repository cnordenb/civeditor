#include "code_patterns.h"

#include "../../src/core/check.h"
#include "../../src/core/text.h"

enum {
    WORD_WIDTH = 24,
    FLOW_WORD_COUNT = 3,
    LOOP_WORD_COUNT = 2,
    ALLOCATION_WORD_COUNT = 11,
    CALLBACK_WORD_COUNT = 9,
    DIRECTIVE_COUNT = 4
};

static const char FLOW_WORDS[FLOW_WORD_COUNT][WORD_WIDTH] = { "goto", "setjmp", "longjmp" };
static const char LOOP_WORDS[LOOP_WORD_COUNT][WORD_WIDTH] = { "while", "do" };
static const char ALLOCATION_WORDS[ALLOCATION_WORD_COUNT][WORD_WIDTH] = {
    "malloc", "calloc", "realloc", "free", "alloca", "strdup", "_strdup",
    "HeapAlloc", "VirtualAlloc", "GlobalAlloc", "LocalAlloc"
};
static const char CALLBACK_WORDS[CALLBACK_WORD_COUNT][WORD_WIDTH] = {
    "GWLP_WNDPROC", "SetWindowSubclass", "qsort", "bsearch", "atexit", "signal", "CreateThread",
    "EnumWindows", "EnumChildWindows"
};
static const char DIRECTIVES[DIRECTIVE_COUNT][WORD_WIDTH] = { "include", "ifndef", "endif", "define" };
static const char CALLBACK_FILE[] = "src/win/app_window.c";

static int line_at(const ScannedSource *source, int offset)
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

static int find_word(const ScannedSource *source, int from, const char *word)
{
    size_t length = text_length(word, WORD_WIDTH);
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

static int first_word_line(const ScannedSource *source, const char words[][WORD_WIDTH], int count)
{
    int index;
    int found;

    if (!CHECK(source != NULL) || !CHECK(words != NULL)) {
        return 0;
    }
    for (index = 0; (index < count) && (index < ALLOCATION_WORD_COUNT); ++index) {
        found = find_word(source, 0, words[index]);
        if (found >= 0) {
            return line_at(source, found);
        }
    }
    return 0;
}

int pattern_forbidden_flow_line(const ScannedSource *source)
{
    if (!CHECK(source != NULL) || !CHECK(source->length >= 0)) {
        return 0;
    }
    return first_word_line(source, FLOW_WORDS, FLOW_WORD_COUNT);
}

int pattern_allocation_line(const ScannedSource *source)
{
    if (!CHECK(source != NULL) || !CHECK(source->length >= 0)) {
        return 0;
    }
    return first_word_line(source, ALLOCATION_WORDS, ALLOCATION_WORD_COUNT);
}

static bool has_constant_name(const ScannedSource *source, int start, int end)
{
    int index;
    int run = 0;
    bool capital = false;

    if (!CHECK(source != NULL) || !CHECK(start <= end)) {
        return false;
    }
    for (index = start; (index <= end) && (index < SCAN_TEXT_CAPACITY); ++index) {
        char letter = (index < end) ? source->cleaned[index] : ' ';
        bool upper = ((letter >= 'A') && (letter <= 'Z')) || (letter == '_') || ((letter >= '0') && (letter <= '9'));
        if (upper && ((run > 0) || ((letter >= 'A') && (letter <= 'Z')))) {
            capital = (run == 0) ? true : capital;
            ++run;
        } else if (source_is_word_letter(letter)) {
            run = -SCAN_TEXT_CAPACITY;
        } else {
            if (capital && (run >= 2)) {
                return true;
            }
            run = 0;
            capital = false;
        }
    }
    return false;
}

static bool loop_is_bounded(const ScannedSource *source, int keyword)
{
    int depth = 0;
    int semicolons = 0;
    int condition_start = -1;
    int index;

    if (!CHECK(source != NULL) || !CHECK(keyword >= 0)) {
        return false;
    }
    for (index = keyword + 3; (index < source->length) && (index < SCAN_TEXT_CAPACITY); ++index) {
        char letter = source->cleaned[index];
        depth += (letter == '(') ? 1 : ((letter == ')') ? -1 : 0);
        if ((letter == ';') && (depth == 1)) {
            ++semicolons;
            if (semicolons == 2) {
                return has_constant_name(source, condition_start, index);
            }
            condition_start = index + 1;
        }
        if ((depth == 0) && (letter == ')')) {
            return false;
        }
    }
    return false;
}

int pattern_unbounded_loop_line(const ScannedSource *source)
{
    int found = 0;
    int step;
    int loop_line = first_word_line(source, LOOP_WORDS, LOOP_WORD_COUNT);

    if (!CHECK(source != NULL) || (loop_line > 0)) {
        return loop_line;
    }
    for (step = 0; (step < SCAN_TEXT_CAPACITY) && (found >= 0); ++step) {
        found = find_word(source, found, "for");
        if (found < 0) {
            break;
        }
        if (!loop_is_bounded(source, found)) {
            return line_at(source, found);
        }
        ++found;
    }
    return 0;
}

static bool define_is_allowed(const ScannedSource *source, int name_start)
{
    int end;
    char after;

    if (!CHECK(source != NULL) || !CHECK(name_start < source->length)) {
        return false;
    }
    if (text_starts_with(&source->cleaned[name_start], "CHECK(")) {
        return true;
    }
    for (end = name_start; (end < source->length) && (end < SCAN_TEXT_CAPACITY); ++end) {
        if (!source_is_word_letter(source->cleaned[end])) {
            break;
        }
    }
    after = (end < source->length) ? source->cleaned[end] : '\n';
    return (end - name_start >= 2) && (source->cleaned[end - 2] == '_') && (source->cleaned[end - 1] == 'H') &&
           ((after == '\n') || (after == '\r'));
}

static int directive_problem(const ScannedSource *source, int start)
{
    int index;

    if (!CHECK(source != NULL) || !CHECK(start >= 0)) {
        return 1;
    }
    for (index = 0; index < DIRECTIVE_COUNT; ++index) {
        if (text_starts_with(&source->cleaned[start + 1], DIRECTIVES[index])) {
            break;
        }
    }
    if (index == DIRECTIVE_COUNT) {
        return 1;
    }
    if (!text_starts_with(&source->cleaned[start + 1], "define")) {
        return 0;
    }
    return define_is_allowed(source, start + 8) ? 0 : 1;
}

int pattern_complex_directive_line(const ScannedSource *source)
{
    int line;
    int start;

    if (!CHECK(source != NULL) || !CHECK(source->line_count <= SCAN_LINE_LIMIT)) {
        return 0;
    }
    for (line = 0; (line < source->line_count) && (line < SCAN_LINE_LIMIT); ++line) {
        start = source->line_starts[line];
        if ((source->cleaned[start] == '#') && (directive_problem(source, start) != 0)) {
            return line + 1;
        }
    }
    return 0;
}

int pattern_deep_pointer_line(const ScannedSource *source)
{
    int index;
    int cursor;

    if (!CHECK(source != NULL) || !CHECK(source->length < SCAN_TEXT_CAPACITY)) {
        return 0;
    }
    for (index = 0; (index + 1 < source->length) && (index < SCAN_TEXT_CAPACITY); ++index) {
        if (text_starts_with(&source->cleaned[index], "**") || text_starts_with(&source->cleaned[index], "*const *")) {
            return line_at(source, index);
        }
        if (!text_starts_with(&source->cleaned[index], "->")) {
            continue;
        }
        for (cursor = index + 2; (cursor < source->length) && (cursor < SCAN_TEXT_CAPACITY); ++cursor) {
            if (!source_is_word_letter(source->cleaned[cursor]) && (source->cleaned[cursor] != ' ')) {
                break;
            }
        }
        if (text_starts_with(&source->cleaned[cursor], "->")) {
            return line_at(source, index);
        }
    }
    return 0;
}

static bool declares_function_pointer(const ScannedSource *source, int start)
{
    int cursor;

    if (!CHECK(source != NULL) || !text_starts_with(&source->cleaned[start], "(*")) {
        return false;
    }
    for (cursor = start + 2; (cursor < source->length) && (cursor < SCAN_TEXT_CAPACITY); ++cursor) {
        if (!source_is_word_letter(source->cleaned[cursor])) {
            break;
        }
    }
    return (cursor > start + 2) && text_starts_with(&source->cleaned[cursor], ")(");
}

int pattern_function_pointer_line(const ScannedSource *source, const char *path)
{
    int found;
    int index;

    if (!CHECK(source != NULL) || !CHECK(path != NULL)) {
        return 0;
    }
    for (index = 0; (index + 1 < source->length) && (index < SCAN_TEXT_CAPACITY); ++index) {
        if (declares_function_pointer(source, index)) {
            return line_at(source, index);
        }
    }
    found = find_word(source, 0, "lpfnWndProc");
    if ((found >= 0) && !text_equal(path, CALLBACK_FILE)) {
        return line_at(source, found);
    }
    return first_word_line(source, CALLBACK_WORDS, CALLBACK_WORD_COUNT);
}
