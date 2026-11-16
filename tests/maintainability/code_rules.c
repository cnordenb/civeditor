#include "code_rules.h"

#include "../../src/core/check.h"
#include "../../src/core/text.h"
#include "../scan/call_graph.h"
#include "../scan/repo_files.h"
#include "../scan/source_scan.h"
#include "../test_outcome.h"
#include "code_patterns.h"

enum {
    FUNCTION_LINE_LIMIT = 60,
    CODE_LINE_LIMIT = 120,
    FILE_LINE_LIMIT = 400,
    NOTE_CAPACITY = 384,
    CHECKS_PER_FUNCTION = 2
};

static const char CHECK_SOURCE[] = "src/core/check.c";

static RepoFiles files;
static ScannedSource source;
static CallGraph graph;

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
    case CODE_RULE_BOUNDED_LOOPS:
        return pattern_unbounded_loop_line(&source);
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

static bool add_to_graph(bool calls)
{
    int index;

    if (!CHECK(files.count > 0) || !CHECK(files.count <= REPO_FILE_CAPACITY)) {
        return false;
    }
    for (index = 0; (index < files.count) && (index < REPO_FILE_CAPACITY); ++index) {
        if (!repo_path_is_code(files.paths[index]) || !repo_path_has_suffix(files.paths[index], ".c")) {
            continue;
        }
        if (!source_scan(&source, files.paths[index])) {
            return false;
        }
        if (calls ? !call_graph_add_calls(&graph, &source) : !call_graph_add_functions(&graph, &source)) {
            return false;
        }
    }
    return true;
}

bool code_has_no_recursion(void)
{
    char culprit[SCAN_NAME_CAPACITY] = "";

    if (!repo_files_list(&files) || !CHECK(files.count > 0)) {
        return false;
    }
    call_graph_start(&graph);
    if (!add_to_graph(false) || !add_to_graph(true)) {
        return false;
    }
    if (call_graph_find_cycle(&graph, culprit, sizeof culprit)) {
        test_note(culprit);
        return false;
    }
    return true;
}

static void note_density(long checks, long functions)
{
    char note[NOTE_CAPACITY] = "";

    if (!CHECK(checks >= 0L) || !CHECK(functions >= 0L)) {
        return;
    }
    (void)text_append_number(note, sizeof note, checks);
    (void)text_append(note, sizeof note, " checks in ");
    (void)text_append_number(note, sizeof note, functions);
    (void)text_append(note, sizeof note, " functions");
    test_note(note);
}

static bool is_measured(const char *path)
{
    if (!CHECK(path != NULL) || !CHECK(path[0] != '\0')) {
        return false;
    }
    return repo_path_has_suffix(path, ".c") && !text_starts_with(path, "tests/") && !text_equal(path, CHECK_SOURCE);
}

static long checks_of_source(void)
{
    long checks = 0L;
    int function;

    if (!CHECK(source.function_count >= 0) || !CHECK(source.function_count <= SCAN_FUNCTION_CAPACITY)) {
        return 0L;
    }
    for (function = 0; (function < source.function_count) && (function < SCAN_FUNCTION_CAPACITY); ++function) {
        checks += source.functions[function].checks;
    }
    return checks;
}

bool code_has_two_checks_per_function(void)
{
    long checks = 0L;
    long functions = 0L;
    int index;

    if (!repo_files_list(&files) || !CHECK(files.count > 0)) {
        return false;
    }
    for (index = 0; (index < files.count) && (index < REPO_FILE_CAPACITY); ++index) {
        if (is_measured(files.paths[index]) && source_scan(&source, files.paths[index])) {
            checks += checks_of_source();
            functions += source.function_count;
        }
    }
    note_density(checks, functions);
    return (functions > 0L) && (checks >= functions * CHECKS_PER_FUNCTION);
}
