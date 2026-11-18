#include "civ_design.h"

#include "../core/text.h"

static const TextField TECH_NAMES[TECH_TIER_COUNT] = { TEXT_FIELD_CASTLE_TECH_NAME, TEXT_FIELD_IMPERIAL_TECH_NAME };

static bool same_text(const CivDesign *design, const CivDesign *other, TextField field)
{
    return text_equal(design->texts[field], other->texts[field]);
}

static int multiplier_of(const BonusPick *picks, int count, int bonus_id)
{
    int index;

    for (index = 0; index < count; ++index) {
        if (picks[index].id == bonus_id) {
            return picks[index].multiplier;
        }
    }
    return 0;
}

static bool same_picks(const BonusPick *picks, int count, const BonusPick *others, int other_count)
{
    int index;

    if (count != other_count) {
        return false;
    }
    for (index = 0; index < count; ++index) {
        if (multiplier_of(others, other_count, picks[index].id) != picks[index].multiplier) {
            return false;
        }
    }
    return true;
}

bool civ_design_same_identity(const CivDesign *design, const CivDesign *other)
{
    int choice;

    for (choice = 0; choice < (int)CHOICE_COUNT; ++choice) {
        if (design->choices[choice] != other->choices[choice]) {
            return false;
        }
    }
    return same_text(design, other, TEXT_FIELD_NAME) && same_text(design, other, TEXT_FIELD_TAGLINE) &&
           same_text(design, other, TEXT_FIELD_DESCRIPTION);
}

bool civ_design_same_tree(const CivDesign *design, const CivDesign *other)
{
    int kind;
    int index;

    for (kind = 0; kind < (int)TREE_KIND_COUNT; ++kind) {
        if (design->tree[kind].count != other->tree[kind].count) {
            return false;
        }
        for (index = 0; index < design->tree[kind].count; ++index) {
            if (!civ_design_tree_has(other, (TreeKind)kind, design->tree[kind].ids[index])) {
                return false;
            }
        }
    }
    return true;
}

bool civ_design_same_bonuses(const CivDesign *design, const CivDesign *other, BonusList list)
{
    if (list == BONUS_LIST_TEAM) {
        return same_picks(design->team_bonuses, design->team_bonus_count, other->team_bonuses, other->team_bonus_count);
    }
    return same_picks(design->bonuses, design->bonus_count, other->bonuses, other->bonus_count);
}

bool civ_design_same_unit(const CivDesign *design, const CivDesign *other)
{
    return (design->unique_unit.origin == other->unique_unit.origin) &&
           (design->unique_unit.id == other->unique_unit.id) && same_text(design, other, TEXT_FIELD_UNIT_NAME) &&
           same_text(design, other, TEXT_FIELD_UNIT_DESCRIPTION);
}

static bool same_costs(const TechPick *pick, const TechPick *other)
{
    int kind;

    for (kind = 0; kind < (int)RESOURCE_KIND_COUNT; ++kind) {
        if (pick->cost[kind] != other->cost[kind]) {
            return false;
        }
    }
    return pick->time == other->time;
}

bool civ_design_same_tech(const CivDesign *design, const CivDesign *other, TechTier tier)
{
    return (design->techs[tier].km_index == other->techs[tier].km_index) &&
           same_costs(&design->techs[tier], &other->techs[tier]) && same_text(design, other, TECH_NAMES[tier]);
}

bool civ_design_same_hero(const CivDesign *design, const CivDesign *other)
{
    return (design->hero_unit == other->hero_unit) && same_text(design, other, TEXT_FIELD_HERO_NAME);
}

static bool same_identity_to_team_bonus(const CivDesign *design, const CivDesign *other)
{
    return civ_design_same_identity(design, other) && civ_design_same_tree(design, other) &&
           civ_design_same_bonuses(design, other, BONUS_LIST_CIV) &&
           civ_design_same_bonuses(design, other, BONUS_LIST_TEAM);
}

static bool same_unit_to_hero(const CivDesign *design, const CivDesign *other)
{
    return civ_design_same_unit(design, other) && civ_design_same_tech(design, other, TECH_TIER_CASTLE) &&
           civ_design_same_tech(design, other, TECH_TIER_IMPERIAL) && civ_design_same_hero(design, other);
}

bool civ_design_same(const CivDesign *design, const CivDesign *other)
{
    return same_identity_to_team_bonus(design, other) && same_unit_to_hero(design, other);
}
