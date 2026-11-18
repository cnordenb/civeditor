#include "wizard.h"

static const WizardPage PAGES[STEP_COUNT] = {
    { "Identity", "Choose which civ to edit. Every step then shows it as the game has it, ready to change." },
    { "Tech tree", "Select the units, buildings and techs the civ may have. Space selects the focused one." },
    { "Civ bonuses", "Select up to 24 bonuses. Type to filter them, or choose a category." },
    { "Team bonus", "Select the one bonus the civ gives to itself and its allies." },
    { "Unique unit", "Select the unit only this civ trains at its castle, and name it." },
    { "Castle tech", "Select the unique tech of the Castle Age, and name it." },
    { "Imperial tech", "Select the unique tech of the Imperial Age, and name it." },
    { "Hero unit", "Select a hero for the civ, or none." },
    { "Review", "Check the civ. Save civ puts it in the mod; Build mod builds every civ saved into one mod." }
};

const WizardPage *wizard_page(WizardStep step)
{
    return &PAGES[step];
}

WizardStep wizard_move(WizardStep step, int by)
{
    int target = (int)step + by;

    return ((target < 0) || (target >= STEP_COUNT)) ? step : (WizardStep)target;
}

static bool differs_early(const CivDesign *design, const CivDesign *base, WizardStep step)
{
    if (step == STEP_IDENTITY) {
        return !civ_design_same_identity(design, base);
    }
    if (step == STEP_TECH_TREE) {
        return !civ_design_same_tree(design, base);
    }
    if (step == STEP_BONUSES) {
        return !civ_design_same_bonuses(design, base, BONUS_LIST_CIV);
    }
    return (step == STEP_TEAM_BONUS) && !civ_design_same_bonuses(design, base, BONUS_LIST_TEAM);
}

static bool differs_late(const CivDesign *design, const CivDesign *base, WizardStep step)
{
    if (step == STEP_UNIQUE_UNIT) {
        return !civ_design_same_unit(design, base);
    }
    if (step == STEP_CASTLE_TECH) {
        return !civ_design_same_tech(design, base, TECH_TIER_CASTLE);
    }
    if (step == STEP_IMPERIAL_TECH) {
        return !civ_design_same_tech(design, base, TECH_TIER_IMPERIAL);
    }
    return (step == STEP_HERO) && !civ_design_same_hero(design, base);
}

bool wizard_step_is_done(const CivDesign *design, const CivDesign *base, bool in_mod, WizardStep step)
{
    if (step == STEP_REVIEW) {
        return in_mod && civ_design_is_complete(design);
    }
    return differs_early(design, base, step) || differs_late(design, base, step);
}
