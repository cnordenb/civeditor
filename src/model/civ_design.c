#include "civ_design.h"

#include <math.h>

#include "../core/check.h"
#include "../core/text.h"

enum { ID_LIMIT = 100000, ALWAYS_KEPT_COUNT = 6, HUNDREDTH_PLACES = 2 };

static const int ALWAYS_KEPT[ALWAYS_KEPT_COUNT] = { 83, 109, 621, 68, 562, 584 };
static const int STAT_LEAST[UNIT_STAT_COUNT] = { 0, 0, 0, 0, 1, 0, -99, -99, 0, 1, 1 };
static const int STAT_MOST[UNIT_STAT_COUNT] = { 9999, 9999, 9999, 9999, 32767, 999, 999, 999, 99, 999, 9999 };
static const UnitPick NO_UNIT = { UNIT_ORIGIN_NONE, CIV_NO_CHOICE, { { { false, 0 } } } };
static const TechPick NO_TECH = { CIV_NO_CHOICE, { { false, 0 } }, { { 0, { 0 } } }, 0 };
static const StatRange TIME_RANGE = { 1, 9999 };
static const Multiplier ONCE = { CIV_MILLIONTHS };
static const double HALF = 0.5;
static const double TEN = 10.0;

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
    design->unique_unit = NO_UNIT;
    for (index = 0; index < (int)TECH_TIER_COUNT; ++index) {
        design->techs[index] = NO_TECH;
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

static bool toggle_pick(BonusPick *picks, int *count, int capacity, int bonus_id)
{
    int position;

    if (!CHECK((bonus_id >= 0) && (bonus_id < ID_LIMIT))) {
        return false;
    }
    position = find_pick(picks, *count, bonus_id);
    if (position >= 0) {
        *count = remove_pick(picks, *count, position);
        return true;
    }
    if (*count >= capacity) {
        return false;
    }
    picks[*count].id = bonus_id;
    picks[*count].multiplier = ONCE;
    ++*count;
    return true;
}

bool civ_design_toggle_bonus(CivDesign *design, int bonus_id)
{
    return toggle_pick(design->bonuses, &design->bonus_count, CIV_BONUS_CAPACITY, bonus_id);
}

bool civ_design_toggle_tech_effect(CivDesign *design, TechTier tier, int bonus_id)
{
    TechPick *tech;

    if (design->techs[tier].km_index == CIV_NO_CHOICE) {
        return false;
    }
    tech = &design->techs[tier];
    return toggle_pick(tech->effects, &tech->effect_count, CIV_TECH_EFFECT_CAPACITY, bonus_id);
}

bool civ_design_clear_bonuses(CivDesign *design)
{
    bool had_any = design->bonus_count > 0;

    design->bonus_count = 0;
    return had_any;
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
    design->team_bonuses[0].multiplier = ONCE;
    design->team_bonus_count = 1;
    return true;
}

bool civ_design_has_team_bonus(const CivDesign *design, int bonus_id)
{
    return (design->team_bonus_count > 0) && (design->team_bonuses[0].id == bonus_id);
}

Multiplier civ_multiplier_of(double times)
{
    Multiplier multiplier = { 0 };

    if ((times >= 1.0) && (times <= (double)CIV_MULTIPLIER_LIMIT)) {
        multiplier.millionths = (int)((times * (double)CIV_MILLIONTHS) + HALF);
    }
    return multiplier;
}

double civ_multiplier_times(Multiplier multiplier)
{
    return (double)multiplier.millionths / (double)CIV_MILLIONTHS;
}

static bool set_pick_multiplier(BonusPick *picks, int count, int bonus_id, Multiplier multiplier)
{
    int position = find_pick(picks, count, bonus_id);

    if ((position < 0) || (multiplier.millionths < CIV_MILLIONTHS) ||
        (multiplier.millionths > CIV_MULTIPLIER_LIMIT * CIV_MILLIONTHS)) {
        return false;
    }
    picks[position].multiplier = multiplier;
    return true;
}

bool civ_design_set_multiplier(CivDesign *design, bool team, int bonus_id, Multiplier multiplier)
{
    if (team) {
        return set_pick_multiplier(design->team_bonuses, design->team_bonus_count, bonus_id, multiplier);
    }
    return set_pick_multiplier(design->bonuses, design->bonus_count, bonus_id, multiplier);
}

bool civ_design_has_tech_effect(const CivDesign *design, TechTier tier, int bonus_id)
{
    return find_pick(design->techs[tier].effects, design->techs[tier].effect_count, bonus_id) >= 0;
}

bool civ_design_set_tech_effect(CivDesign *design, TechTier tier, int bonus_id, Multiplier multiplier)
{
    return set_pick_multiplier(design->techs[tier].effects, design->techs[tier].effect_count, bonus_id, multiplier);
}

Multiplier civ_design_multiplier(const CivDesign *design, int bonus_id)
{
    Multiplier none = { 0 };
    int position = find_pick(design->bonuses, design->bonus_count, bonus_id);

    return (position < 0) ? none : design->bonuses[position].multiplier;
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
    if ((design->unique_unit.origin != origin) || (design->unique_unit.id != unit_id)) {
        design->unique_unit = NO_UNIT;
        design->unique_unit.origin = origin;
        design->unique_unit.id = unit_id;
    }
    return true;
}

static bool range_holds(StatRange range, int value)
{
    return (value >= range.least) && (value <= range.most);
}

bool civ_stat_fits(UnitStat stat, int value)
{
    return range_holds(civ_stat_range(stat), value);
}

static bool set_override(Override *override, StatRange range, int value)
{
    if (!range_holds(range, value)) {
        return false;
    }
    override->set = true;
    override->value = value;
    return true;
}

bool civ_design_set_unit_stat(CivDesign *design, UnitRank rank, UnitStat stat, int value)
{
    if (design->unique_unit.origin == UNIT_ORIGIN_NONE) {
        return false;
    }
    return set_override(&design->unique_unit.stats[rank][stat], civ_stat_range(stat), value);
}

bool civ_design_set_tech_value(CivDesign *design, TechTier tier, TechValue value, int amount)
{
    if (design->techs[tier].km_index == CIV_NO_CHOICE) {
        return false;
    }
    return set_override(&design->techs[tier].values[value], civ_tech_range(value), amount);
}

unsigned civ_stat_places(UnitStat stat)
{
    return (stat >= UNIT_STAT_SPEED) ? HUNDREDTH_PLACES : 0U;
}

StatRange civ_stat_range(UnitStat stat)
{
    StatRange range = { STAT_LEAST[stat], STAT_MOST[stat] };

    return range;
}

StatRange civ_tech_range(TechValue value)
{
    return (value == TECH_VALUE_TIME) ? TIME_RANGE : civ_stat_range((UnitStat)value);
}

bool civ_fit_decimal(StatRange range, unsigned places, double value, int *stored)
{
    double scaled = floor((value * pow(TEN, (double)places)) + HALF);

    if ((scaled < (double)range.least) || (scaled > (double)range.most)) {
        return false;
    }
    *stored = (int)scaled;
    return true;
}

int civ_override_or(Override override, int fallback)
{
    return override.set ? override.value : fallback;
}

bool civ_design_pick_tech(CivDesign *design, TechTier tier, int km_index)
{
    if (!CHECK((km_index >= CIV_NO_CHOICE) && (km_index < ID_LIMIT))) {
        return false;
    }
    if (design->techs[tier].km_index != km_index) {
        design->techs[tier] = NO_TECH;
        design->techs[tier].km_index = km_index;
    }
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

bool civ_design_has_hero(const CivDesign *design)
{
    return design->hero_unit != CIV_NO_CHOICE;
}

bool civ_design_hero_lacks_name(const CivDesign *design)
{
    return civ_design_has_hero(design) && (design->texts[TEXT_FIELD_HERO_NAME][0] == '\0');
}

bool civ_design_tree_is_empty(const CivDesign *design)
{
    return (design->tree[TREE_UNITS].count == 0) && (design->tree[TREE_BUILDINGS].count == 0) &&
           (design->tree[TREE_TECHS].count == 0);
}

bool civ_design_always_keeps(TreeKind kind, int node_id)
{
    return (kind != TREE_TECHS) && check_is_among(node_id, ALWAYS_KEPT, ALWAYS_KEPT_COUNT);
}

bool civ_design_tree_keeps(const CivDesign *design, TreeKind kind, int node_id)
{
    return civ_design_tree_is_empty(design) || civ_design_tree_has(design, kind, node_id) ||
           civ_design_always_keeps(kind, node_id);
}
