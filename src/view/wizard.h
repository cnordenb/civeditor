#ifndef WIZARD_H
#define WIZARD_H

#include <stdbool.h>

#include "../model/civ_design.h"

typedef enum {
    STEP_NAME_AND_TREE,
    STEP_BONUSES,
    STEP_UU_UT,
    STEP_HERO,
    STEP_REVIEW,
    STEP_COUNT
} WizardStep;

enum { STEP_TITLE_WIDTH = 24, STEP_HELP_WIDTH = 128 };

typedef struct {
    char title[STEP_TITLE_WIDTH];
    char help[STEP_HELP_WIDTH];
} WizardPage;

const WizardPage *wizard_page(WizardStep step);
WizardStep wizard_move(WizardStep step, int by);
bool wizard_step_is_done(const CivDesign *design, const CivDesign *base, bool in_mod, WizardStep step);

#endif
