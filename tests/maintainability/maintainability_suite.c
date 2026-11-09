#include "../test_outcome.h"

#include "../../src/core/check.h"
#include "code_rules.h"
#include "doc_rules.h"

static TestOutcome run_code_rule_test(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= 13)) {
        return test_outcome_missing();
    }
    switch (index) {
    case 1:
        return test_outcome_of("no comments in the code", code_rule_holds(CODE_RULE_NO_COMMENTS));
    case 2:
        return test_outcome_of("no function over 60 lines", code_rule_holds(CODE_RULE_FUNCTION_LENGTH));
    case 3:
        return test_outcome_of("no line of code over 120 characters", code_rule_holds(CODE_RULE_LINE_LENGTH));
    case 4:
        return test_outcome_of("no file over 400 lines of code", code_rule_holds(CODE_RULE_FILE_LENGTH));
    case 5:
        return test_outcome_of("no goto, setjmp or longjmp", code_rule_holds(CODE_RULE_SIMPLE_CONTROL_FLOW));
    case 6:
        return test_outcome_of("no recursion, direct or indirect", code_has_no_recursion());
    case 7:
        return test_outcome_of("every loop has a fixed upper bound", code_rule_holds(CODE_RULE_BOUNDED_LOOPS));
    case 8:
        return test_outcome_of("no dynamic memory allocation", code_rule_holds(CODE_RULE_NO_ALLOCATION));
    case 9:
        return test_outcome_of("at least two checks per function", code_has_two_checks_per_function());
    case 10:
        return test_outcome_of("preprocessor only includes and guards", code_rule_holds(CODE_RULE_SIMPLE_PREPROCESSOR));
    case 11:
        return test_outcome_of("one level of pointer dereferencing", code_rule_holds(CODE_RULE_POINTER_DEPTH));
    case 12:
        return test_outcome_of("no function pointers but the window procedure",
                               code_rule_holds(CODE_RULE_NO_FUNCTION_POINTERS));
    default:
        return test_outcome_missing();
    }
}

static TestOutcome run_document_rule_test(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= 8)) {
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

TestOutcome maintainability_suite_run(int index)
{
    TestOutcome outcome;

    if (!CHECK(index > 0) || !CHECK(index <= 100)) {
        return test_outcome_missing();
    }
    outcome = run_code_rule_test(index <= 12 ? index : 13);
    if (outcome.exists) {
        return outcome;
    }
    return run_document_rule_test(index - 12);
}
