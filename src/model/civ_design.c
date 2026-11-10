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

    if (!CHECK(design != NULL)) {
        return;
    }
    for (index = 0; index < (int)TEXT_FIELD_COUNT; ++index) {
        design->texts[index][0] = '\0';
    }
    for (index = 0; index < (int)CHOICE_COUNT; ++index) {
        design->choices[index] = CIV_NO_CHOICE;
    }
    design->choices[CHOICE_ARCHITECTURE] = CIV_DEFAULT_ARCHITECTURE;
    design->choices[CHOICE_LANGUAGE] = CIV_DEFAULT_LANGUAGE;
    design->choices[CHOICE_REPLACE] = CIV_DEFAULT_REPLACE;
    design->choices[CHOICE_BASE] = CIV_GENERIC_BASE;
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
    if (!CHECK((int)field >= 0) || !CHECK((int)field < (int)TEXT_FIELD_COUNT)) {
        return 0;
    }
    switch (field) {
    case TEXT_FIELD_NAME:
    case TEXT_FIELD_UNIT_NAME:
    case TEXT_FIELD_CASTLE_TECH_NAME:
    case TEXT_FIELD_IMPERIAL_TECH_NAME:
    case TEXT_FIELD_HERO_NAME:
        return CIV_NAME_CAPACITY;
    case TEXT_FIELD_TAGLINE:
        return CIV_LINE_CAPACITY;
    case TEXT_FIELD_DESCRIPTION:
    case TEXT_FIELD_UNIT_DESCRIPTION:
    case TEXT_FIELD_COUNT:
    default:
        return CIV_DESCRIPTION_CAPACITY;
    }
}

bool civ_design_set_text(CivDesign *design, TextField field, const char *text)
{
    if (!CHECK(design != NULL) || !CHECK(text != NULL) || !in_range((int)field, TEXT_FIELD_COUNT)) {
        return false;
    }
    return text_copy(design->texts[field], (size_t)civ_text_capacity(field), text);
}

const char *civ_design_text(const CivDesign *design, TextField field)
{
    if (!CHECK(design != NULL) || !CHECK((int)field >= 0) || !CHECK(field < TEXT_FIELD_COUNT)) {
        return "";
    }
    return design->texts[field];
}

bool civ_design_set_choice(CivDesign *design, ChoiceField field, int value)
{
    if (!CHECK(design != NULL) || !CHECK((int)field >= 0) || !CHECK(field < CHOICE_COUNT)) {
        return false;
    }
    if (!CHECK(value >= CIV_NO_CHOICE) || !CHECK(value < ID_LIMIT)) {
        return false;
    }
    design->choices[field] = value;
    return true;
}

static int find_pick(const BonusPick *picks, int count, int bonus_id)
{
    int index;

    if (!CHECK(picks != NULL) || !CHECK(count <= CIV_BONUS_CAPACITY)) {
        return -1;
    }
    for (index = 0; (index < count) && (index < CIV_BONUS_CAPACITY); ++index) {
        if (picks[index].id == bonus_id) {
            return index;
        }
    }
    return -1;
}

static int remove_pick(BonusPick *picks, int count, int position)
{
    int index;

    if (!CHECK(picks != NULL) || !CHECK((position >= 0) && (position < count))) {
        return count;
    }
    for (index = position; (index + 1 < count) && (index + 1 < CIV_BONUS_CAPACITY); ++index) {
        picks[index] = picks[index + 1];
    }
    return count - 1;
}

bool civ_design_toggle_bonus(CivDesign *design, int bonus_id)
{
    int position;

    if (!CHECK(design != NULL) || !CHECK((bonus_id >= 0) && (bonus_id < ID_LIMIT))) {
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
    if (!CHECK(design != NULL) || !CHECK(design->bonus_count <= CIV_BONUS_CAPACITY)) {
        return false;
    }
    return find_pick(design->bonuses, design->bonus_count, bonus_id) >= 0;
}

bool civ_design_toggle_team_bonus(CivDesign *design, int bonus_id)
{
    if (!CHECK(design != NULL) || !CHECK((bonus_id >= 0) && (bonus_id < ID_LIMIT))) {
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
    if (!CHECK(design != NULL) || !CHECK(design->team_bonus_count <= CIV_TEAM_BONUS_CAPACITY)) {
        return false;
    }
    return (design->team_bonus_count > 0) && (design->team_bonuses[0].id == bonus_id);
}

static int find_in_set(const IdSet *set, int node_id)
{
    int index;

    if (!CHECK(set != NULL) || !CHECK(set->count <= CIV_TREE_CAPACITY)) {
        return -1;
    }
    for (index = 0; (index < set->count) && (index < CIV_TREE_CAPACITY); ++index) {
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

    if (!CHECK(design != NULL) || !in_range((int)kind, TREE_KIND_COUNT) || !in_range(node_id, ID_LIMIT)) {
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
    if (!CHECK(design != NULL) || !CHECK(node_id >= 0) || !in_range((int)kind, TREE_KIND_COUNT)) {
        return false;
    }
    return find_in_set(&design->tree[kind], node_id) >= 0;
}

bool civ_design_pick_unit(CivDesign *design, UnitOrigin origin, int unit_id)
{
    if (!CHECK(design != NULL) || !CHECK((unit_id >= CIV_NO_CHOICE) && (unit_id < ID_LIMIT))) {
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
    if (!CHECK(design != NULL) || !CHECK((tier == TECH_TIER_CASTLE) || (tier == TECH_TIER_IMPERIAL))) {
        return false;
    }
    if (!CHECK((km_index >= CIV_NO_CHOICE) && (km_index < ID_LIMIT))) {
        return false;
    }
    design->techs[tier].km_index = km_index;
    return true;
}

bool civ_design_pick_hero(CivDesign *design, int unit_id)
{
    if (!CHECK(design != NULL) || !CHECK((unit_id >= CIV_NO_CHOICE) && (unit_id < ID_LIMIT))) {
        return false;
    }
    design->hero_unit = unit_id;
    return true;
}

bool civ_design_is_complete(const CivDesign *design)
{
    if (!CHECK(design != NULL) || !CHECK(design->bonus_count <= CIV_BONUS_CAPACITY)) {
        return false;
    }
    return (design->texts[TEXT_FIELD_NAME][0] != '\0') && (design->unique_unit.origin != UNIT_ORIGIN_NONE) &&
           (design->techs[TECH_TIER_CASTLE].km_index != CIV_NO_CHOICE) &&
           (design->techs[TECH_TIER_IMPERIAL].km_index != CIV_NO_CHOICE) && !civ_design_hero_lacks_name(design);
}

bool civ_design_hero_lacks_name(const CivDesign *design)
{
    if (!CHECK(design != NULL) || !CHECK(design->hero_unit >= CIV_NO_CHOICE)) {
        return false;
    }
    return (design->hero_unit != CIV_NO_CHOICE) && (design->texts[TEXT_FIELD_HERO_NAME][0] == '\0');
}

bool civ_design_tree_is_empty(const CivDesign *design)
{
    if (!CHECK(design != NULL)) {
        return false;
    }
    return (design->tree[TREE_UNITS].count == 0) && (design->tree[TREE_BUILDINGS].count == 0) &&
           (design->tree[TREE_TECHS].count == 0);
}

bool civ_design_tree_keeps(const CivDesign *design, TreeKind kind, int node_id)
{
    int index;

    if (!CHECK(design != NULL) || !in_range((int)kind, TREE_KIND_COUNT)) {
        return false;
    }
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
