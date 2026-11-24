#include "model_tests.h"

#include "../../src/core/check.h"
#include "../../src/core/text.h"
#include "../../src/model/history.h"
#include "../../src/present/summary.h"
#include "../../src/view/layout.h"
#include "../../src/view/wizard.h"

static History history;
static Catalog empty_catalog;
static SummaryText summary;

static bool type_name(const char *name)
{
    Command command = command_set_text(TEXT_FIELD_NAME, name);

    return history_do(&history, &command);
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

bool test_next_walks_every_step_in_order(void)
{
    WizardStep step = STEP_NAME_AND_TREE;
    int visited;

    for (visited = 1; visited < STEP_COUNT; ++visited) {
        if (wizard_move(step, 1) != (WizardStep)(step + 1)) {
            return false;
        }
        step = wizard_move(step, 1);
    }
    return (step == STEP_FINISH) && (wizard_move(step, 1) == STEP_FINISH);
}

bool model_tests_summary_holds(const char *words)
{
    CivDesign design;

    civ_design_reset(&design);
    (void)civ_design_set_text(&design, TEXT_FIELD_NAME, "Vikings");
    return CHECK(words != NULL) && summary_write(&summary, &design, &empty_catalog) &&
           text_contains_folded(summary.text, words);
}

bool test_summary_names_what_is_still_needed(void)
{
    return model_tests_summary_holds("Still needed before a mod can be built: a unique unit; a castle tech;");
}
