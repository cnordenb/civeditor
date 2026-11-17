#include "wizard.h"

#include "../core/text.h"

static const WizardPage PAGES[STEP_COUNT] = {
    { "Identity", "Choose a base civ first: it sets the tree, bonuses and looks. Then name and describe the civ." },
    { "Tech tree", "Select the units, buildings and techs the civ may have. Space selects the focused one." },
    { "Civ bonuses", "Select up to 24 bonuses. Type to filter them, or choose a category." },
    { "Team bonus", "Select the one bonus the civ gives to itself and its allies." },
    { "Unique unit", "Select the unit only this civ trains at its castle, and name it." },
    { "Castle tech", "Select the unique tech of the Castle Age, and name it." },
    { "Imperial tech", "Select the unique tech of the Imperial Age, and name it." },
    { "Hero unit", "Select a hero for the civ, or none." },
    { "Review", "Check the civ, then save it as a civ file or build it into a mod." }
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

static bool is_typed(const CivDesign *design, TextField field)
{
    return text_length(design->texts[field], 1) > 0;
}

static bool same_tree(const CivDesign *design, const CivDesign *base)
{
    int kind;
    int index;

    for (kind = 0; kind < TREE_KIND_COUNT; ++kind) {
        if (design->tree[kind].count != base->tree[kind].count) {
            return false;
        }
        for (index = 0; index < design->tree[kind].count; ++index) {
            if (!civ_design_tree_has(base, (TreeKind)kind, design->tree[kind].ids[index])) {
                return false;
            }
        }
    }
    return true;
}

static bool same_bonuses(const CivDesign *design, const CivDesign *base)
{
    int index;

    if (design->bonus_count != base->bonus_count) {
        return false;
    }
    for (index = 0; index < design->bonus_count; ++index) {
        if (!civ_design_has_bonus(base, design->bonuses[index].id)) {
            return false;
        }
    }
    return true;
}

static bool same_team_bonus(const CivDesign *design, const CivDesign *base)
{
    return (design->team_bonus_count == base->team_bonus_count) &&
           ((design->team_bonus_count == 0) || civ_design_has_team_bonus(base, design->team_bonuses[0].id));
}

static bool own_unit(const CivDesign *design, const CivDesign *base)
{
    return (design->unique_unit.origin != base->unique_unit.origin) ||
           (design->unique_unit.id != base->unique_unit.id) || is_typed(design, TEXT_FIELD_UNIT_NAME) ||
           is_typed(design, TEXT_FIELD_UNIT_DESCRIPTION);
}

static bool own_tech(const CivDesign *design, const CivDesign *base, TechTier tier, TextField name)
{
    return (design->techs[tier].km_index != base->techs[tier].km_index) || is_typed(design, name);
}

static bool own_pick(const CivDesign *design, const CivDesign *base, WizardStep step)
{
    if (step == STEP_UNIQUE_UNIT) {
        return own_unit(design, base);
    }
    if (step == STEP_CASTLE_TECH) {
        return own_tech(design, base, TECH_TIER_CASTLE, TEXT_FIELD_CASTLE_TECH_NAME);
    }
    if (step == STEP_IMPERIAL_TECH) {
        return own_tech(design, base, TECH_TIER_IMPERIAL, TEXT_FIELD_IMPERIAL_TECH_NAME);
    }
    return (step == STEP_HERO) && (design->hero_unit != CIV_NO_CHOICE);
}

bool wizard_step_is_done(const CivDesign *design, const CivDesign *base, WizardStep step)
{
    if (step == STEP_IDENTITY) {
        return is_typed(design, TEXT_FIELD_NAME);
    }
    if (step == STEP_TECH_TREE) {
        return !same_tree(design, base);
    }
    if (step == STEP_BONUSES) {
        return !same_bonuses(design, base);
    }
    if (step == STEP_TEAM_BONUS) {
        return !same_team_bonus(design, base);
    }
    return (step == STEP_REVIEW) ? civ_design_is_complete(design) : own_pick(design, base, step);
}
