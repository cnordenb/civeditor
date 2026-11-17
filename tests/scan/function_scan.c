#include "source_scan.h"

#include "../../src/core/check.h"
#include "../../src/core/text.h"

static int line_end(const ScannedSource *source, int line)
{
    if (!CHECK(source != NULL) || !CHECK((line >= 0) && (line < source->line_count))) {
        return 0;
    }
    return source->line_starts[line] + source_line_length(source, line);
}

static bool line_is_lone(const ScannedSource *source, int line, char brace)
{
    int start;
    int end;
    int index;

    if (!CHECK(source != NULL) || !CHECK((line >= 0) && (line < source->line_count))) {
        return false;
    }
    start = source->line_starts[line];
    end = line_end(source, line);
    if ((start >= end) || (source->cleaned[start] != brace)) {
        return false;
    }
    for (index = start + 1; (index < end) && (index < SCAN_TEXT_CAPACITY); ++index) {
        if ((source->cleaned[index] != ' ') && (source->cleaned[index] != '\t')) {
            return false;
        }
    }
    return true;
}

static int signature_start(const ScannedSource *source, int brace_line)
{
    int line = brace_line;
    int step;

    if (!CHECK(source != NULL) || !CHECK(brace_line > 0)) {
        return brace_line;
    }
    for (step = 0; (step < SCAN_LINE_LIMIT) && (line > 0); ++step) {
        if (source_line_is_blank(source, line - 1)) {
            break;
        }
        --line;
    }
    return line;
}

static void copy_name(const ScannedSource *source, int open_paren, char *name)
{
    int start = open_paren;
    int step;

    if (!CHECK(source != NULL) || !CHECK(name != NULL)) {
        return;
    }
    for (step = 0; (step < SCAN_NAME_CAPACITY) && (start > 0); ++step) {
        if (!source_is_word_letter(source->cleaned[start - 1])) {
            break;
        }
        --start;
    }
    (void)text_copy_span(name, SCAN_NAME_CAPACITY, &source->cleaned[start], (size_t)(open_paren - start));
}

static void name_function(const ScannedSource *source, ScannedFunction *function)
{
    int index;
    int end;

    if (!CHECK(source != NULL) || !CHECK(function != NULL)) {
        return;
    }
    function->name[0] = '\0';
    end = source->line_starts[function->first_line + 1 < source->line_count ? function->first_line + 1
                                                                              : function->first_line];
    end = (end > source->line_starts[function->first_line]) ? end : source->length;
    for (index = source->line_starts[function->first_line]; (index < end) && (index < SCAN_TEXT_CAPACITY); ++index) {
        if (source->cleaned[index] == '(') {
            copy_name(source, index, function->name);
            return;
        }
    }
}

static int count_code_lines(const ScannedSource *source, int first, int last)
{
    int count = 0;
    int line;

    if (!CHECK(source != NULL) || !CHECK(first <= last)) {
        return 0;
    }
    for (line = first; (line <= last) && (line < SCAN_LINE_LIMIT); ++line) {
        count += source_line_is_blank(source, line) ? 0 : 1;
    }
    return count;
}

static bool record_function(ScannedSource *source, int open_line, int close_line)
{
    ScannedFunction *function;

    if (!CHECK(source != NULL) || !CHECK(source->function_count < SCAN_FUNCTION_CAPACITY)) {
        return false;
    }
    function = &source->functions[source->function_count];
    function->first_line = signature_start(source, open_line);
    function->last_line = close_line;
    function->lines_of_code = count_code_lines(source, function->first_line, close_line);
    function->body_start = source->line_starts[open_line];
    function->body_end = line_end(source, close_line);
    name_function(source, function);
    ++source->function_count;
    return true;
}

bool source_find_functions(ScannedSource *source)
{
    int open_line = -1;
    int line;

    if (!CHECK(source != NULL) || !CHECK(source->line_count <= SCAN_LINE_LIMIT)) {
        return false;
    }
    source->function_count = 0;
    for (line = 0; (line < source->line_count) && (line < SCAN_LINE_LIMIT); ++line) {
        if (line_is_lone(source, line, '{') && (line > 0)) {
            open_line = line;
        } else if (line_is_lone(source, line, '}') && (open_line >= 0)) {
            if (!record_function(source, open_line, line)) {
                return false;
            }
            open_line = -1;
        }
    }
    return true;
}
