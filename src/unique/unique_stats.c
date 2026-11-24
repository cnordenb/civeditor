#include "unique_aim.h"

#include <math.h>

#include "../dat/dat_parts.h"
#include "../forge/recipe_parts.h"

enum { MELEE_CLASS = 4, PIERCE_CLASS = 3, PAID = 1, LEAST_ATTACK = -32769, MADE_CAPACITY = 7 };

typedef enum { FIELD_SHORT, FIELD_FLOAT } FieldKind;

typedef enum {
    AT_NONE,
    AT_UNIT,
    AT_SPEED,
    AT_COMBAT,
    AT_SHOWN_PIERCE,
    AT_ATTACK,
    AT_MELEE_ARMOR,
    AT_PIERCE_ARMOR,
    AT_COUNT
} Anchor;

typedef struct {
    FieldKind kind;
    Anchor real;
    size_t real_offset;
    Anchor shown;
    size_t shown_offset;
} PlaceRule;

typedef struct {
    bool found;
    size_t costs_at;
    size_t anchors[AT_COUNT];
} StatTarget;

static const double HALF = 0.5;
static const double TEN = 10.0;

static const PlaceRule RULES[UNIT_STAT_COUNT] = {
    [UNIT_STAT_HP] = { FIELD_SHORT, AT_UNIT, DAT_UNIT_HIT_POINTS_AT, AT_NONE, 0U },
    [UNIT_STAT_ATTACK] = { FIELD_SHORT, AT_ATTACK, 0U, AT_COMBAT, DAT_COMBAT_SHOWN_ATTACK_AT },
    [UNIT_STAT_MELEE_ARMOR] = { FIELD_SHORT, AT_MELEE_ARMOR, 0U, AT_COMBAT, DAT_COMBAT_SHOWN_MELEE_ARMOR_AT },
    [UNIT_STAT_PIERCE_ARMOR] = { FIELD_SHORT, AT_PIERCE_ARMOR, 0U, AT_SHOWN_PIERCE, 0U },
    [UNIT_STAT_RANGE] = { FIELD_FLOAT, AT_COMBAT, DAT_COMBAT_RANGE_AT, AT_COMBAT, DAT_COMBAT_SHOWN_RANGE_AT },
    [UNIT_STAT_SPEED] = { FIELD_FLOAT, AT_SPEED, 0U, AT_NONE, 0U },
    [UNIT_STAT_RELOAD] = { FIELD_FLOAT, AT_COMBAT, DAT_COMBAT_RELOAD_AT, AT_COMBAT, DAT_COMBAT_SHOWN_RELOAD_AT }
};

static double scale_of(UnitStat stat)
{
    unsigned places = civ_stat_places(stat);

    return pow(TEN, (double)places);
}

static double read_at(const DatBytes *dat, size_t at, FieldKind kind)
{
    DatCursor cursor;
    float real;
    int whole;

    if ((at == 0U) || !dat_cursor_start(&cursor, dat, at)) {
        return 0.0;
    }
    if (kind == FIELD_FLOAT) {
        real = dat_cursor_f32(&cursor);
        return (double)real;
    }
    whole = dat_cursor_i16(&cursor);
    return (double)whole;
}

static size_t entry_at(const DatBytes *dat, size_t list_at, int count, int wanted)
{
    DatCursor cursor;
    int index;

    (void)dat_cursor_start(&cursor, dat, list_at);
    for (index = 0; index < count; ++index) {
        if (dat_cursor_i16(&cursor) == wanted) {
            return cursor.at;
        }
        (void)dat_cursor_skip(&cursor, DAT_SHORT_WIDTH);
    }
    return 0U;
}

static size_t attack_at(const DatBytes *dat, const DatUnit *unit)
{
    DatCursor cursor;
    size_t found = 0U;
    long most = LEAST_ATTACK;
    int index;
    int attack_class;
    int amount;

    (void)dat_cursor_start(&cursor, dat, unit->attacks_at);
    for (index = 0; index < unit->attack_count; ++index) {
        attack_class = dat_cursor_i16(&cursor);
        amount = dat_cursor_i16(&cursor);
        if (((attack_class == MELEE_CLASS) || (attack_class == PIERCE_CLASS)) && (amount > most)) {
            found = cursor.at - DAT_SHORT_WIDTH;
            most = amount;
        }
    }
    return found;
}

static StatTarget target_of(const DatTarget *target, int unit_id)
{
    StatTarget stats = { false, 0U, { 0U } };
    const DatMap *map = target->map;
    DatUnit unit;
    size_t at = 0U;

    if (!dat_find_unit(target->dat, map->civ_at[target->civ], unit_id, &at) ||
        !dat_unit_read(target->dat, at, &unit)) {
        return stats;
    }
    stats.found = true;
    stats.costs_at = unit.costs_at;
    stats.anchors[AT_UNIT] = unit.at;
    stats.anchors[AT_SPEED] = unit.speed_at;
    stats.anchors[AT_COMBAT] = unit.combat_at;
    stats.anchors[AT_SHOWN_PIERCE] = unit.shown_pierce_at;
    stats.anchors[AT_ATTACK] = attack_at(target->dat, &unit);
    stats.anchors[AT_MELEE_ARMOR] = entry_at(target->dat, unit.armours_at, unit.armour_count, MELEE_CLASS);
    stats.anchors[AT_PIERCE_ARMOR] = entry_at(target->dat, unit.armours_at, unit.armour_count, PIERCE_CLASS);
    return stats;
}

static size_t place_of(const StatTarget *stats, Anchor anchor, size_t offset)
{
    return (stats->anchors[anchor] == 0U) ? 0U : stats->anchors[anchor] + offset;
}

static int stat_at(const DatBytes *dat, size_t at, UnitStat stat)
{
    double value = read_at(dat, at, RULES[stat].kind);
    double rounded = floor((value * scale_of(stat)) + HALF);

    return (int)rounded;
}

static int stat_now(const DatBytes *dat, const StatTarget *stats, UnitStat stat)
{
    const PlaceRule *rule = &RULES[stat];

    return stat_at(dat, place_of(stats, rule->real, rule->real_offset), stat);
}

static int stat_known(const DatBytes *dat, const StatTarget *stats, UnitStat stat)
{
    const PlaceRule *rule = &RULES[stat];

    if (stat != UNIT_STAT_RANGE) {
        return stat_now(dat, stats, stat);
    }
    return stat_at(dat, place_of(stats, rule->shown, rule->shown_offset), stat);
}

static bool put_at(DatEdit *edit, size_t at, FieldKind kind, double value)
{
    double rounded = floor(value + HALF);

    if (kind == FIELD_FLOAT) {
        return dat_edit_patch_f32(edit, at, (float)value);
    }
    return dat_edit_patch_i16(edit, at, (int)rounded);
}

static bool write_stat(const DatTarget *target, const StatTarget *stats, UnitStat stat, int wanted)
{
    const PlaceRule *rule = &RULES[stat];
    size_t real = place_of(stats, rule->real, rule->real_offset);
    size_t shown = place_of(stats, rule->shown, rule->shown_offset);
    double value = (double)wanted / scale_of(stat);

    return (real != 0U) && put_at(target->edit, real, rule->kind, value) &&
           ((shown == 0U) || put_at(target->edit, shown, rule->kind, value));
}

bool unique_stat_wanted(const UnitPick *pick, UnitRank rank, UnitStat stat, const int now[UNIT_RANK_COUNT],
                        int *wanted)
{
    Override own = pick->stats[rank][stat];
    Override base = pick->stats[UNIT_RANK_BASE][stat];

    if (own.set) {
        *wanted = own.value;
        return true;
    }
    *wanted = now[rank] + (base.value - now[UNIT_RANK_BASE]);
    return base.set;
}

static int build_stat(const DatTarget *target, const StatTarget ranks[UNIT_RANK_COUNT], const UnitPick *pick,
                      UnitStat stat)
{
    int now[UNIT_RANK_COUNT];
    int skipped = 0;
    int rank;
    int wanted;

    for (rank = 0; rank < (int)UNIT_RANK_COUNT; ++rank) {
        now[rank] = stat_now(target->dat, &ranks[rank], stat);
    }
    for (rank = 0; rank < (int)UNIT_RANK_COUNT; ++rank) {
        if (ranks[rank].found && unique_stat_wanted(pick, (UnitRank)rank, stat, now, &wanted) &&
            (!civ_stat_fits(stat, wanted) || !write_stat(target, &ranks[rank], stat, wanted))) {
            ++skipped;
        }
    }
    return skipped;
}

static void read_costs(const DatBytes *dat, size_t costs_at, DatTechCost now[DAT_UNIT_COST_SLOTS], int *amounts)
{
    DatCursor cursor;
    int index;

    for (index = 0; index < RESOURCE_KIND_COUNT; ++index) {
        amounts[index] = 0;
    }
    (void)dat_cursor_start(&cursor, dat, costs_at);
    for (index = 0; index < DAT_UNIT_COST_SLOTS; ++index) {
        now[index].type = dat_cursor_i16(&cursor);
        now[index].amount = dat_cursor_i16(&cursor);
        now[index].flag = dat_cursor_i16(&cursor);
        if ((now[index].type >= 0) && (now[index].type < RESOURCE_KIND_COUNT)) {
            amounts[now[index].type] = now[index].amount;
        }
    }
}

bool unique_stats_read(const DatTarget *target, int unit_id, int stats[UNIT_STAT_COUNT])
{
    StatTarget found = target_of(target, unit_id);
    DatTechCost now[DAT_UNIT_COST_SLOTS];
    int stat;

    if (!found.found) {
        return false;
    }
    read_costs(target->dat, found.costs_at, now, stats);
    for (stat = RESOURCE_KIND_COUNT; stat < (int)UNIT_STAT_COUNT; ++stat) {
        stats[stat] = stat_known(target->dat, &found, (UnitStat)stat);
    }
    return true;
}

static int made_costs(const DatBytes *dat, size_t costs_at, const UnitPick *pick, DatTechCost made[MADE_CAPACITY])
{
    DatTechCost now[DAT_UNIT_COST_SLOTS];
    int amounts[RESOURCE_KIND_COUNT];
    int count = 0;
    int index;

    read_costs(dat, costs_at, now, amounts);
    for (index = 0; index < RESOURCE_KIND_COUNT; ++index) {
        made[count] = (DatTechCost){ index, civ_override_or(pick->stats[UNIT_RANK_BASE][index], amounts[index]), PAID };
        count += (made[count].amount > 0) ? 1 : 0;
    }
    for (index = 0; index < DAT_UNIT_COST_SLOTS; ++index) {
        made[count] = now[index];
        count += (now[index].type >= RESOURCE_KIND_COUNT) ? 1 : 0;
    }
    return count;
}

static bool write_costs(const DatTarget *target, const StatTarget *stats, const UnitPick *pick)
{
    const DatTechCost empty = { DAT_NO_ID, 0, 0 };
    DatTechCost made[MADE_CAPACITY];
    int count = made_costs(target->dat, stats->costs_at, pick, made);
    int slot;
    bool fine = (count <= DAT_UNIT_COST_SLOTS);

    for (slot = 0; fine && (slot < DAT_UNIT_COST_SLOTS); ++slot) {
        fine = dat_edit_patch_cost(target->edit, stats->costs_at + ((size_t)slot * DAT_UNIT_COST_SIZE),
                                   (slot < count) ? &made[slot] : &empty);
    }
    return fine;
}

static bool any_cost_set(const UnitPick *pick)
{
    int resource;

    for (resource = 0; resource < RESOURCE_KIND_COUNT; ++resource) {
        if (pick->stats[UNIT_RANK_BASE][resource].set) {
            return true;
        }
    }
    return false;
}

int unique_build_stats(const RecipeInput *input, const DatTarget *target)
{
    const UnitEntry *own = recipe_design_unit(input);
    const CivDesign *design = input->design;
    const UnitPick *pick = &design->unique_unit;
    StatTarget ranks[UNIT_RANK_COUNT];
    UniqueUnit unit;
    int skipped = 0;
    int stat;
    int rank;

    if (own == NULL) {
        return 0;
    }
    unit = unique_unit_of_techs(input, own->make_tech, own->elite_tech);
    ranks[UNIT_RANK_BASE] = target_of(target, unit.base);
    ranks[UNIT_RANK_ELITE] = target_of(target, unit.elite);
    for (stat = (int)UNIT_STAT_HP; ranks[UNIT_RANK_BASE].found && (stat < (int)UNIT_STAT_COUNT); ++stat) {
        skipped += build_stat(target, ranks, pick, (UnitStat)stat);
    }
    for (rank = 0; any_cost_set(pick) && (rank < (int)UNIT_RANK_COUNT); ++rank) {
        skipped += (ranks[rank].found && !write_costs(target, &ranks[rank], pick)) ? 1 : 0;
    }
    return skipped;
}
