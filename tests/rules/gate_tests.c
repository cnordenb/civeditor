#include "gate_tests.h"

#include "../../src/core/check.h"
#include "../../src/core/text.h"
#include "../maintainability/code_patterns.h"
#include "../redundancy/rules/p10_patterns.h"
#include "../scan/call_graph.h"

enum {
    GOTO_LINE = 5,
    MALLOC_LINE = 12,
    DEFINE_LINE = 23,
    DEEP_LINE = 27,
    POINTER_LINE = 30,
    COMMENT_LINE = 34
};

static const char BROKEN[] = "tests/fixtures/broken_rules.txt";

static ScannedSource source;
static CallGraph graph;

static const ScannedSource *broken(void)
{
    (void)CHECK(source_scan(&source, BROKEN));
    (void)CHECK(source.line_count > COMMENT_LINE);
    return &source;
}

bool test_the_rule_against_goto_names_the_line_of_a_goto(void)
{
    return pattern_forbidden_flow_line(broken()) == GOTO_LINE;
}

bool test_the_rule_against_allocation_names_the_line_of_a_malloc(void)
{
    return pattern_allocation_line(broken()) == MALLOC_LINE;
}

bool test_the_rule_of_the_preprocessor_names_a_macro_with_arguments(void)
{
    return pattern_complex_directive_line(broken()) == DEFINE_LINE;
}

bool test_the_rule_of_one_dereference_names_two_arrows_in_a_row(void)
{
    return pattern_deep_pointer_line(broken()) == DEEP_LINE;
}

bool test_the_rule_against_function_pointers_names_one(void)
{
    return pattern_function_pointer_line(broken(), BROKEN) == POINTER_LINE;
}

bool test_the_rule_against_comments_names_the_line_of_a_comment(void)
{
    return broken()->comment_line == COMMENT_LINE;
}

bool test_the_rule_against_recursion_names_a_function_that_calls_itself(void)
{
    char culprit[SCAN_NAME_CAPACITY] = "";

    call_graph_start(&graph);
    return call_graph_add_functions(&graph, broken()) && call_graph_add_calls(&graph, &source) &&
           call_graph_find_cycle(&graph, culprit, sizeof culprit) && text_equal(culprit, "again");
}
