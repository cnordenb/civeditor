#include "wizard.h"

#include "../core/check.h"

static const WizardPage PAGES[STEP_COUNT + 1] = {
    { "Identity", "Choose a base civ first: it sets the tree, bonuses and looks. Then name and describe the civ." },
    { "Tech tree", "Select the units, buildings and techs the civ may have. Space selects the focused one." },
    { "Civ bonuses", "Select up to 24 bonuses. Type to filter them, or choose a category." },
    { "Team bonus", "Select the one bonus the civ gives to itself and its allies." },
    { "Unique unit", "Select the unit only this civ trains at its castle, and name it." },
    { "Castle tech", "Select the unique tech of the Castle Age, and name it." },
    { "Imperial tech", "Select the unique tech of the Imperial Age, and name it." },
    { "Hero unit", "Select a hero for the civ, or none." },
    { "Review", "Check the civ, then save it as a civ file or build it into a mod." },
    { "", "" }
};

static bool is_step(WizardStep step)
{
    return CHECK((int)step >= 0) && CHECK(step < STEP_COUNT);
}

const WizardPage *wizard_page(WizardStep step)
{
    return &PAGES[is_step(step) ? step : STEP_COUNT];
}

WizardStep wizard_move(WizardStep step, int by)
{
    int target = (int)step + by;

    if (!is_step(step) || !CHECK((by == 1) || (by == -1))) {
        return STEP_IDENTITY;
    }
    return ((target < 0) || (target >= STEP_COUNT)) ? step : (WizardStep)target;
}

static bool pick_is_made(const CivDesign *design, WizardStep step)
{
    if (!CHECK(design != NULL) || !CHECK(step < STEP_COUNT)) {
        return false;
    }
    if (step == STEP_UNIQUE_UNIT) {
        return design->unique_unit.origin != UNIT_ORIGIN_NONE;
    }
    if (step == STEP_CASTLE_TECH) {
        return design->techs[TECH_TIER_CASTLE].km_index != CIV_NO_CHOICE;
    }
    if (step == STEP_IMPERIAL_TECH) {
        return design->techs[TECH_TIER_IMPERIAL].km_index != CIV_NO_CHOICE;
    }
    return (step == STEP_HERO) && (design->hero_unit != CIV_NO_CHOICE);
}

bool wizard_step_is_done(const CivDesign *design, WizardStep step)
{
    if (!CHECK(design != NULL) || !CHECK((int)step >= 0) || !CHECK(step < STEP_COUNT)) {
        return false;
    }
    if (step == STEP_IDENTITY) {
        return design->texts[TEXT_FIELD_NAME][0] != '\0';
    }
    if (step == STEP_TECH_TREE) {
        return !civ_design_tree_is_empty(design);
    }
    if (step == STEP_BONUSES) {
        return design->bonus_count > 0;
    }
    if (step == STEP_TEAM_BONUS) {
        return design->team_bonus_count > 0;
    }
    return (step == STEP_REVIEW) ? civ_design_is_complete(design) : pick_is_made(design, step);
}
