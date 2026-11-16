#include "history_tests.h"

#include "../../src/core/check.h"
#include "../../src/core/text.h"
#include "../../src/model/history.h"

enum { A_BONUS = 7, UNDO_LIMIT = 2 * HISTORY_DEPTH };

static History history;
static CivDesign other;

static bool toggle(void)
{
    Command bonus = command_of(COMMAND_TOGGLE_BONUS, 0, A_BONUS);

    return history_do(&history, &bonus);
}

static void toggle_times(int times)
{
    int done;

    history_start(&history);
    for (done = 0; (done < times) && (done < UNDO_LIMIT); ++done) {
        (void)CHECK(toggle());
    }
}

static int undos_possible(void)
{
    int undone = 0;
    int step;

    for (step = 0; step < UNDO_LIMIT; ++step) {
        if (!history_step(&history, true)) {
            break;
        }
        ++undone;
    }
    return undone;
}

static bool type_name(const char *name)
{
    Command command = command_set_text(TEXT_FIELD_NAME, name);

    return history_do(&history, &command);
}

bool test_a_new_design_has_nothing_to_undo(void)
{
    history_start(&history);
    return !history_step(&history, true);
}

bool test_a_new_design_has_nothing_to_redo(void)
{
    history_start(&history);
    return !history_step(&history, false);
}

bool test_an_undo_that_fails_leaves_nothing_to_redo(void)
{
    history_start(&history);
    (void)history_step(&history, true);
    return !history_step(&history, false);
}

bool test_a_change_that_is_made_is_reported_as_made(void)
{
    history_start(&history);
    return toggle();
}

bool test_a_change_after_an_undo_leaves_nothing_to_redo(void)
{
    toggle_times(1);
    (void)history_step(&history, true);
    (void)CHECK(toggle());
    return !history_step(&history, false);
}

bool test_undo_goes_back_as_many_changes_as_the_history_holds(void)
{
    toggle_times(HISTORY_DEPTH + 1);
    return undos_possible() == HISTORY_DEPTH;
}

static void number_changes(int count)
{
    Command change;
    int number;

    history_start(&history);
    for (number = 1; (number <= count) && (number < UNDO_LIMIT); ++number) {
        change = command_of(COMMAND_SET_CHOICE, CHOICE_CASTLE, number);
        (void)CHECK(history_do(&history, &change));
    }
}

bool test_undo_past_the_depth_brings_back_the_change_before(void)
{
    number_changes(HISTORY_DEPTH + 1);
    (void)history_step(&history, true);
    return history_step(&history, true) && (history.present.choices[CHOICE_CASTLE] == HISTORY_DEPTH - 1);
}

bool test_a_design_put_in_place_of_another_can_be_undone(void)
{
    civ_design_reset(&other);
    toggle_times(1);
    return history_replace(&history, &other) && history_step(&history, true) && (history.present.bonus_count == 1);
}

bool test_a_design_put_in_place_of_another_leaves_nothing_to_redo(void)
{
    civ_design_reset(&other);
    toggle_times(1);
    (void)history_step(&history, true);
    (void)CHECK(history_replace(&history, &other));
    return !history_step(&history, false);
}

bool test_a_change_after_typing_is_undone_by_itself(void)
{
    history_start(&history);
    (void)CHECK(type_name("V"));
    (void)CHECK(toggle());
    return history_step(&history, true) && text_equal(civ_design_text(&history.present, TEXT_FIELD_NAME), "V");
}

bool test_typing_after_an_undo_is_a_change_of_its_own(void)
{
    history_start(&history);
    (void)CHECK(type_name("V"));
    (void)CHECK(type_name("Vi"));
    (void)history_step(&history, true);
    (void)CHECK(type_name("X"));
    return history_step(&history, true) && text_equal(civ_design_text(&history.present, TEXT_FIELD_NAME), "");
}
