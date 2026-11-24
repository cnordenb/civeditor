#include "../test_outcome.h"

#include "../../src/core/check.h"
#include "code_rules.h"
#include "doc_rules.h"

enum { CODE_RULE_TEST_COUNT = 5 };

static TestOutcome run_code_rule_test(int index)
{
    const char *name;

    if (!CHECK(index > 0) || !CHECK(index <= CODE_RULE_TEST_COUNT) ||
        !CHECK(CODE_RULE_SIMPLE_CONTROL_FLOW == CODE_RULE_TEST_COUNT - 1)) {
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
    case 5:
        name = "no goto, setjmp or longjmp";
        break;
    default:
        return test_outcome_missing();
    }
    return test_outcome_of(name, code_rule_holds((CodeRule)(index - 1)));
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

TestOutcome maintainability_suite_run(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= 100)) {
        return test_outcome_missing();
    }
    if (index <= CODE_RULE_TEST_COUNT) {
        return run_code_rule_test(index);
    }
    return run_document_rule_test(index - CODE_RULE_TEST_COUNT);
}
