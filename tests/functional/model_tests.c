#include "model_tests.h"

#include "../../src/core/text.h"
#include "../../src/model/history.h"
#include "../../src/present/summary.h"
#include "../../src/view/layout.h"
#include "../../src/view/wizard.h"

enum { BONUS_LIMIT = 24 };

static History history;
static Catalog empty_catalog;
static SummaryText summary;

static bool type_name(const char *name)
{
    Command command = command_set_text(TEXT_FIELD_NAME, name);

    return history_do(&history, &command);
}

bool test_undo_brings_back_the_civ_before_a_change(void)
{
    Command bonus = command_of(COMMAND_TOGGLE_BONUS, 0, 7);

    history_start(&history);
    (void)history_do(&history, &bonus);
    return history_step(&history, true) && (history.present.bonus_count == 0);
}

bool test_typing_a_name_is_undone_at_once(void)
{
    history_start(&history);
    (void)type_name("V");
    (void)type_name("Vi");
    (void)type_name("Vik");
    return history_step(&history, true) && text_equal(civ_design_text(&history.present, TEXT_FIELD_NAME), "");
}

bool test_redo_makes_an_undone_change_again(void)
{
    Command unit = command_of(COMMAND_PICK_UNIT, UNIT_ORIGIN_VANILLA, 1263);

    history_start(&history);
    (void)history_do(&history, &unit);
    (void)history_step(&history, true);
    return history_step(&history, false) && (history.present.unique_unit.id == 1263);
}

bool test_a_25th_bonus_is_refused(void)
{
    CivDesign design;
    int bonus;

    civ_design_reset(&design);
    for (bonus = 0; bonus < BONUS_LIMIT; ++bonus) {
        (void)civ_design_toggle_bonus(&design, bonus);
    }
    return !civ_design_toggle_bonus(&design, BONUS_LIMIT) && (design.bonus_count == BONUS_LIMIT);
}

bool test_next_walks_every_step_in_order(void)
{
    WizardStep step = STEP_IDENTITY;
    int visited;

    for (visited = 1; visited < STEP_COUNT; ++visited) {
        if (wizard_move(step, 1) != (WizardStep)(step + 1)) {
            return false;
        }
        step = wizard_move(step, 1);
    }
    return (step == STEP_REVIEW) && (wizard_move(step, 1) == STEP_REVIEW);
}

bool test_summary_names_what_is_still_needed(void)
{
    CivDesign design;

    civ_design_reset(&design);
    (void)civ_design_set_text(&design, TEXT_FIELD_NAME, "Vikings");
    return summary_write(&summary, &design, &empty_catalog) &&
           text_contains_folded(summary.text, "Still needed before a mod can be built: a unique unit; a castle tech;");
}

bool test_summary_labels_the_name(void)
{
    CivDesign design;

    civ_design_reset(&design);
    (void)civ_design_set_text(&design, TEXT_FIELD_NAME, "Vikings");
    return summary_write(&summary, &design, &empty_catalog) && text_contains_folded(summary.text, "Name: Vikings\r\n");
}

bool test_the_last_step_shows_no_next_button(void)
{
    return !layout_shows(CONTROL_NEXT, STEP_REVIEW);
}
