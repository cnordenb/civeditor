#include "code_rules.h"

#include "../../src/core/check.h"
#include "../../src/core/text.h"
#include "../scan/repo_files.h"
#include "../scan/source_scan.h"
#include "../test_outcome.h"
#include "../redundancy/rules/p10_patterns.h"
#include "code_patterns.h"

enum {
    FUNCTION_LINE_LIMIT = 60,
    CODE_LINE_LIMIT = 120,
    FILE_LINE_LIMIT = 400,
    NOTE_CAPACITY = 384
};

static RepoFiles files;
static ScannedSource source;

static int long_function_line(void)
{
    int index;

    if (!CHECK(source.function_count <= SCAN_FUNCTION_CAPACITY) || !CHECK(FUNCTION_LINE_LIMIT > 0)) {
        return 0;
    }
    for (index = 0; (index < source.function_count) && (index < SCAN_FUNCTION_CAPACITY); ++index) {
        if (source.functions[index].lines_of_code > FUNCTION_LINE_LIMIT) {
            return source.functions[index].first_line + 1;
        }
    }
    return 0;
}

static int long_line(void)
{
    int line;

    if (!CHECK(source.line_count <= SCAN_LINE_LIMIT) || !CHECK(CODE_LINE_LIMIT > 0)) {
        return 0;
    }
    for (line = 0; (line < source.line_count) && (line < SCAN_LINE_LIMIT); ++line) {
        if (source_line_length(&source, line) > CODE_LINE_LIMIT) {
            return line + 1;
        }
    }
    return 0;
}

static int size_violation_line(CodeRule rule)
{
    if (!CHECK(rule <= CODE_RULE_FILE_LENGTH) || !CHECK(FILE_LINE_LIMIT > 0)) {
        return 0;
    }
    switch ((int)rule) {
    case CODE_RULE_NO_COMMENTS:
        return source.comment_line;
    case CODE_RULE_FUNCTION_LENGTH:
        return long_function_line();
    case CODE_RULE_LINE_LENGTH:
        return long_line();
    default:
        return (source.lines_of_code > FILE_LINE_LIMIT) ? 1 : 0;
    }
}

static int pattern_violation_line(CodeRule rule, const char *path)
{
    if (!CHECK(path != NULL) || !CHECK(rule > CODE_RULE_FILE_LENGTH)) {
        return 0;
    }
    switch ((int)rule) {
    case CODE_RULE_SIMPLE_CONTROL_FLOW:
        return pattern_forbidden_flow_line(&source);
    case CODE_RULE_NO_ALLOCATION:
        return pattern_allocation_line(&source);
    case CODE_RULE_SIMPLE_PREPROCESSOR:
        return pattern_complex_directive_line(&source);
    case CODE_RULE_POINTER_DEPTH:
        return pattern_deep_pointer_line(&source);
    default:
        return pattern_function_pointer_line(&source, path);
    }
}

static int violation_line(CodeRule rule, const char *path)
{
    if (!CHECK(path != NULL) || !CHECK(rule < CODE_RULE_COUNT)) {
        return 0;
    }
    return (rule <= CODE_RULE_FILE_LENGTH) ? size_violation_line(rule) : pattern_violation_line(rule, path);
}

static void note_violation(const char *path, int line)
{
    char note[NOTE_CAPACITY] = "";

    if (!CHECK(path != NULL) || !CHECK(line > 0)) {
        return;
    }
    (void)text_append(note, sizeof note, path);
    (void)text_append(note, sizeof note, ":");
    (void)text_append_number(note, sizeof note, line);
    test_note(note);
}

bool code_rule_holds(CodeRule rule)
{
    int index;
    int line;

    if (!CHECK(rule < CODE_RULE_COUNT) || !repo_files_list(&files)) {
        return false;
    }
    for (index = 0; (index < files.count) && (index < REPO_FILE_CAPACITY); ++index) {
        if (!repo_path_is_code(files.paths[index])) {
            continue;
        }
        if (!source_scan(&source, files.paths[index])) {
            note_violation(files.paths[index], 1);
            return false;
        }
        line = violation_line(rule, files.paths[index]);
        if (line > 0) {
            note_violation(files.paths[index], line);
            return false;
        }
    }
    return true;
}
