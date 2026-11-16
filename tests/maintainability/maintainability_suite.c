#include "../test_outcome.h"

#include "../../src/core/check.h"
#include "code_rules.h"
#include "doc_rules.h"
#include "../rules/gate_tests.h"
#include "../rules/tool_tests.h"

enum { P10_RULE_START = 4, DOCUMENT_RULE_START = 12, GATE_START = 20, SOURCE_START = 28, PLAN_START = 31 };

static TestOutcome run_form_rule_test(int index)
{
    const char *name;

    if (!CHECK(index > 0) || !CHECK(index <= P10_RULE_START) || !CHECK(CODE_RULE_FILE_LENGTH == P10_RULE_START - 1)) {
        return test_outcome_missing();
    }
    switch (index) {
    case 1:
        name = "no comments in the code";
        break;
    case 2:
        name = "no function over 60 lines";
        break;
    case 3:
        name = "no line of code over 120 characters";
        break;
    case 4:
        name = "no file over 400 lines of code";
        break;
    default:
        return test_outcome_missing();
    }
    return test_outcome_of(name, code_rule_holds((CodeRule)(index - 1)));
}

static TestOutcome run_p10_rule_test(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= DOCUMENT_RULE_START)) {
        return test_outcome_missing();
    }
    switch (index) {
    case 1:
        return test_outcome_of("no goto, setjmp or longjmp", code_rule_holds(CODE_RULE_SIMPLE_CONTROL_FLOW));
    case 2:
        return test_outcome_of("no recursion, direct or indirect", code_has_no_recursion());
    case 3:
        return test_outcome_of("every loop has a fixed upper bound", code_rule_holds(CODE_RULE_BOUNDED_LOOPS));
    case 4:
        return test_outcome_of("no dynamic memory allocation", code_rule_holds(CODE_RULE_NO_ALLOCATION));
    case 5:
        return test_outcome_of("at least two checks per function", code_has_two_checks_per_function());
    case 6:
        return test_outcome_of("preprocessor only includes and guards", code_rule_holds(CODE_RULE_SIMPLE_PREPROCESSOR));
    case 7:
        return test_outcome_of("one level of pointer dereferencing", code_rule_holds(CODE_RULE_POINTER_DEPTH));
    case 8:
        return test_outcome_of("no function pointers but the window procedure",
                               code_rule_holds(CODE_RULE_NO_FUNCTION_POINTERS));
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_document_rule_test(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= 88)) {
        return test_outcome_missing();
    }
    switch (index) {
    case 1:
        return test_outcome_of("no folder over 7 files, the main folder 8", repo_folders_hold_few_files());
    case 2:
        return test_outcome_of("no two files share a name", repo_file_names_are_unique());
    case 3:
        return test_outcome_of("only C code and documents in the project", repo_holds_only_c_and_documents());
    case 4:
        return test_outcome_of("every placeholder of the template is filled in", repo_placeholders_are_filled());
    case 5:
        return test_outcome_of("the development log keeps its form", devlog_keeps_its_form());
    case 6:
        return test_outcome_of("TESTS.md numbers its tests in order", tests_document_is_numbered_in_order());
    case 7:
        return test_outcome_of("TESTS.md documents every test", tests_document_counts_every_test());
    case 8:
        return test_outcome_of("every line of TERMINOLOGY.md is short", terminology_lines_are_short());
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
        return test_outcome_of("the rule of bounded loops names a loop without a fixed bound",
                               test_the_rule_of_bounded_loops_names_a_loop_without_a_fixed_bound());
    case 4:
        return test_outcome_of("the rule of the preprocessor names a macro with arguments",
                               test_the_rule_of_the_preprocessor_names_a_macro_with_arguments());
    case 5:
        return test_outcome_of("the rule of one dereference names two arrows in a row",
                               test_the_rule_of_one_dereference_names_two_arrows_in_a_row());
    case 6:
        return test_outcome_of("the rule against function pointers names one",
                               test_the_rule_against_function_pointers_names_one());
    case 7:
        return test_outcome_of("the rule against comments names the line of a comment",
                               test_the_rule_against_comments_names_the_line_of_a_comment());
    case 8:
        return test_outcome_of("the rule against recursion names a function that calls itself",
                               test_the_rule_against_recursion_names_a_function_that_calls_itself());
    default:
        return test_outcome_missing();
    }
}

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
        return test_outcome_of("the sources of the project are those of src, tests and tools",
                               test_the_sources_of_the_project_are_those_of_src_tests_and_tools());
    case 3:
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
        return test_outcome_of("the objects of the tests are listed with the libraries of Windows",
                               test_the_objects_of_the_tests_are_listed_with_the_libraries_of_windows());
    case 6:
        return test_outcome_of("the objects of the build tool are listed without libraries",
                               test_the_objects_of_the_build_tool_are_listed_without_libraries());
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_later_test(int index)
{
    if (!CHECK(index > GATE_START) || !CHECK(index <= 100)) {
        return test_outcome_missing();
    }
    if (index <= SOURCE_START) {
        return run_gate_test(index - GATE_START);
    }
    if (index <= PLAN_START) {
        return run_source_test(index - SOURCE_START);
    }
    return run_plan_test(index - PLAN_START);
}

TestOutcome maintainability_suite_run(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= 100)) {
        return test_outcome_missing();
    }
    if (index <= P10_RULE_START) {
        return run_form_rule_test(index);
    }
    if (index <= DOCUMENT_RULE_START) {
        return run_p10_rule_test(index - P10_RULE_START);
    }
    if (index <= GATE_START) {
        return run_document_rule_test(index - DOCUMENT_RULE_START);
    }
    return run_later_test(index);
}
