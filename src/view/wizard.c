#include "wizard.h"

#include "../core/check.h"

static const char TITLES[STEP_COUNT][STEP_TITLE_WIDTH] = {
    "Identity", "Tech tree", "Civ bonuses", "Team bonus", "Unique unit", "Castle tech", "Imperial tech",
    "Hero unit", "Review"
};

static const char HELP[STEP_COUNT][STEP_HELP_WIDTH] = {
    "Name the civ, give it a tagline and a story, and choose how it looks and sounds.",
    "Select the units, buildings and techs the civ may have. Space selects the focused one.",
    "Select up to 24 bonuses. Type to filter them, or choose a category.",
    "Select the one bonus the civ gives to itself and its allies.",
    "Select the unit only this civ trains at its castle, and name it.",
    "Select the unique tech of the Castle Age, and name it.",
    "Select the unique tech of the Imperial Age, and name it.",
    "Select a hero for the civ, or none.",
    "Check the civ, then save it as a civ file or build it into a mod."
};

const char *wizard_title(WizardStep step)
{
    if (!CHECK((int)step >= 0) || !CHECK(step < STEP_COUNT)) {
        return "";
    }
    return TITLES[step];
}

const char *wizard_help(WizardStep step)
{
    if (!CHECK((int)step >= 0) || !CHECK(step < STEP_COUNT)) {
        return "";
    }
    return HELP[step];
}

WizardStep wizard_next(WizardStep step)
{
    if (!CHECK((int)step >= 0) || !CHECK(step < STEP_COUNT)) {
        return STEP_IDENTITY;
    }
    return (step == STEP_REVIEW) ? STEP_REVIEW : (WizardStep)(step + 1);
}

WizardStep wizard_previous(WizardStep step)
{
    if (!CHECK((int)step >= 0) || !CHECK(step < STEP_COUNT)) {
        return STEP_IDENTITY;
    }
    return (step == STEP_IDENTITY) ? STEP_IDENTITY : (WizardStep)(step - 1);
}

static bool tree_is_chosen(const CivDesign *design)
{
    int kind;

    if (!CHECK(design != NULL) || !CHECK(design->tree[TREE_UNITS].count <= CIV_TREE_CAPACITY)) {
        return false;
    }
    for (kind = 0; kind < (int)TREE_KIND_COUNT; ++kind) {
        if (design->tree[kind].count > 0) {
            return true;
        }
    }
    return false;
}

bool wizard_step_is_done(const CivDesign *design, WizardStep step)
{
    if (!CHECK(design != NULL) || !CHECK((int)step >= 0) || !CHECK(step < STEP_COUNT)) {
        return false;
    }
    switch (step) {
    case STEP_IDENTITY:
        return design->texts[TEXT_FIELD_NAME][0] != '\0';
    case STEP_TECH_TREE:
        return tree_is_chosen(design);
    case STEP_BONUSES:
        return design->bonus_count > 0;
    case STEP_TEAM_BONUS:
        return design->team_bonus_count > 0;
    case STEP_UNIQUE_UNIT:
        return design->unique_unit.origin != UNIT_ORIGIN_NONE;
    case STEP_CASTLE_TECH:
        return design->techs[TECH_TIER_CASTLE].km_index != CIV_NO_CHOICE;
    case STEP_IMPERIAL_TECH:
        return design->techs[TECH_TIER_IMPERIAL].km_index != CIV_NO_CHOICE;
    case STEP_HERO:
        return design->hero_unit != CIV_NO_CHOICE;
    case STEP_REVIEW:
        return civ_design_is_complete(design);
    case STEP_COUNT:
    default:
        return false;
    }
}
