#include "../test_outcome.h"

#include "../../src/core/check.h"
#include "access_tests.h"

TestOutcome accessibility_suite_run(int index)
{
    if (!CHECK(index > 0) || !CHECK(index <= 100)) {
        return test_outcome_missing();
    }
    switch (index) {
    case 1:
        return test_outcome_of("every text contrasts at least 7:1 with its background",
                               test_every_text_contrasts_seven_to_one());
    case 2:
        return test_outcome_of("every input has a visible label", test_every_input_has_a_visible_label());
    case 3:
        return test_outcome_of("every control one acts on is at least 44 by 44", test_every_target_is_44_by_44());
    case 4:
        return test_outcome_of("no two controls of a step overlap, even in the smallest window",
                               test_no_two_controls_of_a_step_overlap());
    case 5:
        return test_outcome_of("the window's title names the civ and the step",
                               test_the_window_title_names_the_civ_and_the_step());
    default:
        return test_outcome_missing();
    }
}
