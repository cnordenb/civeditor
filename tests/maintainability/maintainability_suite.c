#include "../test_outcome.h"

#include "../../src/core/check.h"
#include "../../src/core/text.h"
#include "../../tools/crap/crap_cover.h"
#include "../../tools/crap/crap_score.h"
#include "../../tools/dry/dry_shape.h"
#include "../../tools/mutate/mutant_sites.h"
#include "code_rules.h"
#include "doc_rules.h"

enum { LINE_WIDTH = 64, LINE_COUNT = 6 };

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

static bool renamed_functions_are_alike(void)
{
    static const char FIRST[] = "{ int total = 0; for (index = 0; index < count; ++index) "
                                "{ total += price_of(items, index); } return total; }";
    static const char SECOND[] = "{ int sum = 7; for (at = 0; at < size; ++at) "
                                 "{ sum += price_of(rows, at); } return sum; }";
    static DryShape first;
    static DryShape second;

    if (!CHECK(sizeof FIRST > 1U) || !CHECK(sizeof SECOND > 1U)) {
        return false;
    }
    return dry_shape_of(&first, FIRST, 0, (int)sizeof FIRST - 1) &&
           dry_shape_of(&second, SECOND, 0, (int)sizeof SECOND - 1) &&
           (dry_alike_milli(&first, &second) == DRY_MILLI);
}

static bool line_gives_each_mutant_once(void)
{
    static const char LINE[] = "if (!ready && (item->count <= limit - 1)) { return false; }";
    char swaps[MUTANT_WITH_CAPACITY * MUTANT_WITH_CAPACITY] = "";
    MutantSite site;
    int at = 0;
    int step;

    if (!CHECK(sizeof LINE > 1U) || !CHECK(sizeof swaps > 1U)) {
        return false;
    }
    for (step = 0; step < MUTANT_WITH_CAPACITY * 2; ++step) {
        site = mutant_next(LINE, at, (int)sizeof LINE - 1);
        if (!site.found) {
            break;
        }
        at = site.at + site.length;
        (void)text_append(swaps, sizeof swaps, site.with);
        (void)text_append(swaps, sizeof swaps, ",");
    }
    return text_equal(swaps, " ,||,<,+,0,true,");
}

static bool check_branches_are_left_out_of_coverage(void)
{
    static const char LINES[LINE_COUNT][LINE_WIDTH] = { "        5:   18:    if (!CHECK(a) || !CHECK(b)) {",
                                               "branch  0 taken 5 (fallthrough)", "branch  1 taken 0",
                                               "        5:   19:    if (ready) {", "branch  0 taken 5 (fallthrough)",
                                               "branch  1 taken 0" };
    static LineCoverage coverage;
    CoveredShare share;
    int index;

    if (!CHECK(sizeof LINES > 1U) || !CHECK(LINE_WIDTH > 0)) {
        return false;
    }
    crap_cover_start(&coverage);
    for (index = 0; index < LINE_COUNT; ++index) {
        (void)crap_cover_take_line(&coverage, LINES[index]);
    }
    share = crap_cover_share(&coverage, 18, 19);
    return (share.covered == 1) && (share.total == 2);
}

static bool check_gives_no_mutant(void)
{
    static const char LINE[] = "if (!CHECK(a != 0) || !CHECK(b < 1) || (c < 2)) { return; }";
    MutantSite site = mutant_next(LINE, 0, (int)sizeof LINE - 1);

    if (!CHECK(sizeof LINE > 1U) || !CHECK(MUTANT_WITH_CAPACITY > 0)) {
        return false;
    }
    return site.found && text_equal(site.with, "&&") && (LINE[site.at + 3] == '(');
}

static TestOutcome run_tool_test(int index)
{
    static const char BRANCHES[] = "if (a && b || c) { for (i = 0; i < 3; ++i) { while (x) { } } } "
                                   "switch (k) { case 1: return k ? 1 : 0; } gift(notify);";
    static const char CHECKED[] = "if (!CHECK(a) || !CHECK(b)) { return; } if (c) { }";

    if (!CHECK(index > 0) || !CHECK(index <= 80)) {
        return test_outcome_missing();
    }
    switch (index) {
    case 1:
        return test_outcome_of("the CRAP score is complexity squared times the uncovered share cubed, plus complexity",
                               crap_score_milli(4, 1, 3) == 8740L);
    case 2:
        return test_outcome_of("the complexity of a function counts its branches",
                               crap_complexity(BRANCHES, 0, (int)sizeof BRANCHES - 1) == 8);
    case 3:
        return test_outcome_of("two functions that differ only in their names are alike",
                               renamed_functions_are_alike());
    case 4:
        return test_outcome_of("the mutants of a line change each operator and constant once",
                               line_gives_each_mutant_once());
    case 5:
        return test_outcome_of("the complexity of a function leaves its CHECKs out",
                               crap_complexity(CHECKED, 0, (int)sizeof CHECKED - 1) == 2);
    case 6:
        return test_outcome_of("the coverage of a function leaves the branches of its CHECKs out",
                               check_branches_are_left_out_of_coverage());
    case 7:
        return test_outcome_of("a CHECK gives no mutant", check_gives_no_mutant());
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
    outcome = run_document_rule_test(index - 12);
    if (outcome.exists) {
        return outcome;
    }
    return run_tool_test(index - 20);
}
