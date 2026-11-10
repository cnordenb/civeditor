#ifndef WIZARD_H
#define WIZARD_H

#include <stdbool.h>

#include "../model/civ_design.h"

typedef enum {
    STEP_IDENTITY,
    STEP_TECH_TREE,
    STEP_BONUSES,
    STEP_TEAM_BONUS,
    STEP_UNIQUE_UNIT,
    STEP_CASTLE_TECH,
    STEP_IMPERIAL_TECH,
    STEP_HERO,
    STEP_REVIEW,
    STEP_COUNT
} WizardStep;

enum { STEP_TITLE_WIDTH = 24, STEP_HELP_WIDTH = 128 };

const char *wizard_title(WizardStep step);
const char *wizard_help(WizardStep step);
WizardStep wizard_move(WizardStep step, int by);
bool wizard_step_is_done(const CivDesign *design, WizardStep step);

#endif
