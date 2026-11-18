#include "civ_design.h"

#include "../core/check.h"
#include "../core/text.h"

enum { ID_LIMIT = 100000, ALWAYS_KEPT_COUNT = 6 };

static const int ALWAYS_KEPT[ALWAYS_KEPT_COUNT] = { 83, 109, 621, 68, 562, 584 };

static bool in_range(int value, int count)
{
    return CHECK(value >= 0) && CHECK(value < count);
}

void civ_design_reset(CivDesign *design)
{
    int index;

    for (index = 0; index < (int)TEXT_FIELD_COUNT; ++index) {
        design->texts[index][0] = '\0';
    }
    for (index = 0; index < (int)CHOICE_COUNT; ++index) {
        design->choices[index] = CIV_NO_CHOICE;
    }
    design->choices[CHOICE_ARCHITECTURE] = CIV_DEFAULT_ARCHITECTURE;
    design->choices[CHOICE_LANGUAGE] = CIV_DEFAULT_LANGUAGE;
    design->choices[CHOICE_REPLACE] = CIV_DEFAULT_REPLACE;
    design->bonus_count = 0;
    design->team_bonus_count = 0;
    for (index = 0; index < (int)TREE_KIND_COUNT; ++index) {
        design->tree[index].count = 0;
    }
    design->unique_unit.origin = UNIT_ORIGIN_NONE;
    design->unique_unit.id = CIV_NO_CHOICE;
    for (index = 0; index < (int)TECH_TIER_COUNT; ++index) {
        TechPick empty = { CIV_NO_CHOICE, { 0, 0, 0, 0 }, 0 };
        design->techs[index] = empty;
    }
    design->hero_unit = CIV_NO_CHOICE;
}

int civ_text_capacity(TextField field)
{
    static const int CAPACITIES[TEXT_FIELD_COUNT] = {
        CIV_NAME_CAPACITY, CIV_LINE_CAPACITY, CIV_DESCRIPTION_CAPACITY, CIV_NAME_CAPACITY,
        CIV_DESCRIPTION_CAPACITY, CIV_NAME_CAPACITY, CIV_NAME_CAPACITY, CIV_NAME_CAPACITY
    };

    return CAPACITIES[field];
}

bool civ_design_set_text(CivDesign *design, TextField field, const char *text)
{
    return text_copy(design->texts[field], (size_t)civ_text_capacity(field), text);
}

const char *civ_design_text(const CivDesign *design, TextField field)
{
    return design->texts[field];
}

static bool is_choice_value(int value)
{
    return CHECK(value >= CIV_NO_CHOICE) && CHECK(value < ID_LIMIT);
}

bool civ_design_set_choice(CivDesign *design, ChoiceField field, int value)
{
    if (!is_choice_value(value)) {
        return false;
    }
    design->choices[field] = value;
    return true;
}

static int find_pick(const BonusPick *picks, int count, int bonus_id)
{
    int index;

    for (index = 0; index < count; ++index) {
        if (picks[index].id == bonus_id) {
            return index;
        }
    }
    return -1;
}

static int remove_pick(BonusPick *picks, int count, int position)
{
    int index;

    for (index = position; index + 1 < count; ++index) {
        picks[index] = picks[index + 1];
    }
    return count - 1;
}

bool civ_design_toggle_bonus(CivDesign *design, int bonus_id)
{
    int position;

    if (!CHECK((bonus_id >= 0) && (bonus_id < ID_LIMIT))) {
        return false;
    }
    position = find_pick(design->bonuses, design->bonus_count, bonus_id);
    if (position >= 0) {
        design->bonus_count = remove_pick(design->bonuses, design->bonus_count, position);
        return true;
    }
    if (design->bonus_count >= CIV_BONUS_CAPACITY) {
        return false;
    }
    design->bonuses[design->bonus_count].id = bonus_id;
    design->bonuses[design->bonus_count].multiplier = 1;
    ++design->bonus_count;
    return true;
}

bool civ_design_has_bonus(const CivDesign *design, int bonus_id)
{
    return find_pick(design->bonuses, design->bonus_count, bonus_id) >= 0;
}

bool civ_design_toggle_team_bonus(CivDesign *design, int bonus_id)
{
    if (!CHECK((bonus_id >= 0) && (bonus_id < ID_LIMIT))) {
        return false;
    }
    if ((design->team_bonus_count > 0) && (design->team_bonuses[0].id == bonus_id)) {
        design->team_bonus_count = 0;
        return true;
    }
    design->team_bonuses[0].id = bonus_id;
    design->team_bonuses[0].multiplier = 1;
    design->team_bonus_count = 1;
    return true;
}

bool civ_design_has_team_bonus(const CivDesign *design, int bonus_id)
{
    return (design->team_bonus_count > 0) && (design->team_bonuses[0].id == bonus_id);
}

void civ_design_set_multiplier(CivDesign *design, bool team, int bonus_id, int multiplier)
{
    BonusPick *picks = team ? design->team_bonuses : design->bonuses;
    int position = find_pick(picks, team ? design->team_bonus_count : design->bonus_count, bonus_id);

    if ((position >= 0) && (multiplier >= 1) && (multiplier <= CIV_MULTIPLIER_LIMIT)) {
        picks[position].multiplier = multiplier;
    }
}

static int find_in_set(const IdSet *set, int node_id)
{
    int index;

    for (index = 0; index < set->count; ++index) {
        if (set->ids[index] == node_id) {
            return index;
        }
    }
    return -1;
}

bool civ_design_toggle_tree(CivDesign *design, TreeKind kind, int node_id)
{
    int position;
    IdSet *set;

    if (!in_range(node_id, ID_LIMIT)) {
        return false;
    }
    set = &design->tree[kind];
    position = find_in_set(set, node_id);
    if (position >= 0) {
        set->ids[position] = set->ids[set->count - 1];
        --set->count;
        return true;
    }
    if (set->count >= CIV_TREE_CAPACITY) {
        return false;
    }
    set->ids[set->count] = node_id;
    ++set->count;
    return true;
}

bool civ_design_tree_has(const CivDesign *design, TreeKind kind, int node_id)
{
    return find_in_set(&design->tree[kind], node_id) >= 0;
}

bool civ_design_pick_unit(CivDesign *design, UnitOrigin origin, int unit_id)
{
    if (!CHECK((unit_id >= CIV_NO_CHOICE) && (unit_id < ID_LIMIT))) {
        return false;
    }
    if (!CHECK((origin == UNIT_ORIGIN_NONE) == (unit_id == CIV_NO_CHOICE))) {
        return false;
    }
    design->unique_unit.origin = origin;
    design->unique_unit.id = unit_id;
    return true;
}

bool civ_design_pick_tech(CivDesign *design, TechTier tier, int km_index)
{
    if (!CHECK((km_index >= CIV_NO_CHOICE) && (km_index < ID_LIMIT))) {
        return false;
    }
    design->techs[tier].km_index = km_index;
    return true;
}

bool civ_design_pick_hero(CivDesign *design, int unit_id)
{
    if (!CHECK((unit_id >= CIV_NO_CHOICE) && (unit_id < ID_LIMIT))) {
        return false;
    }
    design->hero_unit = unit_id;
    return true;
}

bool civ_design_is_complete(const CivDesign *design)
{
    return (design->texts[TEXT_FIELD_NAME][0] != '\0') && (design->unique_unit.origin != UNIT_ORIGIN_NONE) &&
           (design->techs[TECH_TIER_CASTLE].km_index != CIV_NO_CHOICE) &&
           (design->techs[TECH_TIER_IMPERIAL].km_index != CIV_NO_CHOICE) && !civ_design_hero_lacks_name(design);
}

bool civ_design_hero_lacks_name(const CivDesign *design)
{
    return (design->hero_unit != CIV_NO_CHOICE) && (design->texts[TEXT_FIELD_HERO_NAME][0] == '\0');
}

bool civ_design_tree_is_empty(const CivDesign *design)
{
    return (design->tree[TREE_UNITS].count == 0) && (design->tree[TREE_BUILDINGS].count == 0) &&
           (design->tree[TREE_TECHS].count == 0);
}

bool civ_design_tree_keeps(const CivDesign *design, TreeKind kind, int node_id)
{
    int index;

    if (civ_design_tree_is_empty(design) || civ_design_tree_has(design, kind, node_id)) {
        return true;
    }
    for (index = 0; (kind != TREE_TECHS) && (index < ALWAYS_KEPT_COUNT); ++index) {
        if (ALWAYS_KEPT[index] == node_id) {
            return true;
        }
    }
    return false;
}
