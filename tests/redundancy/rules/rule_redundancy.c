#include "../../test_outcome.h"

#include "../../../src/core/check.h"
#include "../../maintainability/code_rules.h"
#include "../../rules/gate_tests.h"
#include "../../rules/tool_tests.h"
#include "recursion_rule.h"

enum { SOURCE_TEST_COUNT = 4, PLAN_TEST_COUNT = 5, STEP_TEST_COUNT = 3, LAST_TEST_COUNT = 2, P10_TEST_COUNT = 5 };
enum {
    PLAN_START = SOURCE_TEST_COUNT,
    STEP_START = PLAN_START + PLAN_TEST_COUNT,
    LAST_START = STEP_START + STEP_TEST_COUNT,
    TOOL_TEST_COUNT = LAST_START + LAST_TEST_COUNT
};

static TestOutcome run_source_test(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= 80)) {
        return test_outcome_missing();
    }
    switch (index) {
    case 1:
        return test_outcome_of("the sources of a folder include those of its folders",
                               test_the_sources_of_a_folder_include_those_of_its_folders());
    case 2:
        return test_outcome_of("a folder lists only its own sources", test_a_folder_lists_only_its_own_sources());
    case 3:
        return test_outcome_of("the sources of the project are those of src, tests and tools",
                               test_the_sources_of_the_project_are_those_of_src_tests_and_tools());
    case 4:
        return test_outcome_of("the newest source is the one changed last",
                               test_the_newest_source_is_the_one_changed_last());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_plan_test(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= 80)) {
        return test_outcome_missing();
    }
    switch (index) {
    case 1:
        return test_outcome_of("the object of a source is named after it in the folder given",
                               test_the_object_of_a_source_is_named_after_it_in_the_folder_given());
    case 2:
        return test_outcome_of("a source older than its object is not built again",
                               test_a_source_older_than_its_object_is_not_built_again());
    case 3:
        return test_outcome_of("a source with a newer header is built again",
                               test_a_source_with_a_newer_header_is_built_again());
    case 4:
        return test_outcome_of("a source changed when its object was made is built again",
                               test_a_source_changed_when_its_object_was_made_is_built_again());
    case 5:
        return test_outcome_of("the objects of the build tool are listed without libraries",
                               test_the_objects_of_the_build_tool_are_listed_without_libraries());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_step_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("a coverage build is asked for by coverage",
                               test_a_coverage_build_is_asked_for_by_coverage());
    case 2:
        return test_outcome_of("the tests are run with the arguments given to the build",
                               test_the_tests_are_run_with_the_arguments_given_to_the_build());
    case 3:
        return test_outcome_of("a coverage build links the tests alone", test_a_coverage_build_links_the_tests_alone());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_last_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("a coverage build links the tests with coverage",
                               test_a_coverage_build_links_the_tests_with_coverage());
    case 2:
        return test_outcome_of("a coverage build makes the folder of its report",
                               test_a_coverage_build_makes_the_folder_of_its_report());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_p10_test(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("no recursion, direct or indirect", code_has_no_recursion());
    case 2:
        return test_outcome_of("no dynamic memory allocation", code_rule_holds(CODE_RULE_NO_ALLOCATION));
    case 3:
        return test_outcome_of("preprocessor only includes and guards", code_rule_holds(CODE_RULE_SIMPLE_PREPROCESSOR));
    case 4:
        return test_outcome_of("one level of pointer dereferencing", code_rule_holds(CODE_RULE_POINTER_DEPTH));
    case 5:
        return test_outcome_of("no function pointers but the window procedure",
                               code_rule_holds(CODE_RULE_NO_FUNCTION_POINTERS));
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_gate_test(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= 80)) {
        return test_outcome_missing();
    }
    switch (index) {
    case 1:
        return test_outcome_of("the rule against goto names the line of a goto",
                               test_the_rule_against_goto_names_the_line_of_a_goto());
    case 2:
        return test_outcome_of("the rule against allocation names the line of a malloc",
                               test_the_rule_against_allocation_names_the_line_of_a_malloc());
    case 3:
        return test_outcome_of("the rule of the preprocessor names a macro with arguments",
                               test_the_rule_of_the_preprocessor_names_a_macro_with_arguments());
    case 4:
        return test_outcome_of("the rule of one dereference names two arrows in a row",
                               test_the_rule_of_one_dereference_names_two_arrows_in_a_row());
    case 5:
        return test_outcome_of("the rule against function pointers names one",
                               test_the_rule_against_function_pointers_names_one());
    case 6:
        return test_outcome_of("the rule against comments names the line of a comment",
                               test_the_rule_against_comments_names_the_line_of_a_comment());
    case 7:
        return test_outcome_of("the rule against recursion names a function that calls itself",
                               test_the_rule_against_recursion_names_a_function_that_calls_itself());
    default:
        return test_outcome_missing();
    }
}

TestOutcome rule_redundancy_tools_run(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= TOOL_TEST_COUNT + 1)) {
        return test_outcome_missing();
    }
    if (index <= PLAN_START) {
        return run_source_test(index);
    }
    if (index <= STEP_START) {
        return run_plan_test(index - PLAN_START);
    }
    if (index <= LAST_START) {
        return run_step_test(index - STEP_START);
    }
    return run_last_test(index - LAST_START);
}

TestOutcome rule_redundancy_run(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= 100)) {
        return test_outcome_missing();
    }
    if (index <= TOOL_TEST_COUNT) {
        return rule_redundancy_tools_run(index);
    }
    if (index <= TOOL_TEST_COUNT + P10_TEST_COUNT) {
        return run_p10_test(index - TOOL_TEST_COUNT);
    }
    return run_gate_test(index - TOOL_TEST_COUNT - P10_TEST_COUNT);
}
