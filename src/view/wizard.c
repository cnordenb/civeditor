#include "wizard.h"

static const WizardPage PAGES[STEP_COUNT] = {
    { "Name and tech tree", "Choose which civ to edit, then change its name, its looks and what its tech tree holds." },
    { "Bonuses", "Select up to 24 bonuses and the one team bonus. Type to filter them, or choose a category." },
    { "UU&UT", "Select the unique unit and the castle and imperial techs, and name them. Tick Hero for a hero." },
    { "Finish", "Check the civ. Every change is kept as it is made; Build mod builds every civ changed into one mod." }
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

static bool differs(const CivDesign *design, const CivDesign *base, WizardStep step)
{
    if (step == STEP_NAME_AND_TREE) {
        return !civ_design_same_identity(design, base) || !civ_design_same_tree(design, base);
    }
    if (step == STEP_BONUSES) {
        return !civ_design_same_bonuses(design, base, BONUS_LIST_CIV) ||
               !civ_design_same_bonuses(design, base, BONUS_LIST_TEAM);
    }
    return !civ_design_same_unit(design, base) || !civ_design_same_tech(design, base, TECH_TIER_CASTLE) ||
           !civ_design_same_tech(design, base, TECH_TIER_IMPERIAL) || !civ_design_same_hero(design, base);
}

bool wizard_step_is_done(const CivDesign *design, const CivDesign *base, WizardStep step)
{
    if (step == STEP_FINISH) {
        return !civ_design_same(design, base) && civ_design_is_complete(design);
    }
    return differs(design, base, step);
}
