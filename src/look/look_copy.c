#include "look.h"

#include <string.h>

#include "../core/check.h"

enum {
    CASTLE_UNIT = 82,
    WONDER_UNIT = 276,
    MONK_CORPSE_UNIT = 134,
    ICON_SET_SIZE = 1,
    TASK_LIMIT = 1024,
    NO_TASK_GRAPHIC = DAT_TASK_SIZE
};

enum {
    CLASS_BUILDING = 3,
    CLASS_MISCELLANEOUS = 11,
    CLASS_TERRAIN = 14,
    CLASS_MONK = 18,
    CLASS_TRADE_CART = 19,
    CLASS_WARSHIP = 22,
    CLASS_WALL = 27,
    CLASS_GATE = 39,
    CLASS_MONK_WITH_RELIC = 43,
    CLASS_TOWER = 52,
    CLASS_KING = 59
};

typedef struct {
    DatUnit to;
    DatUnit from;
} UnitPair;

typedef struct {
    int to;
    int from;
} UnitIds;

static const int MONK_UNITS[CATALOG_LOOK_UNITS] = { 125, 134, 286 };

static DatUnitList to_units;
static DatUnitList from_units;
static bool rubble_done[DAT_UNIT_CAPACITY];

static bool copy_is_inside(const DatBytes *dat, const DatCopy *copy)
{
    return CHECK(copy->width <= dat->size) && CHECK(copy->at <= dat->size - copy->width) &&
           CHECK(copy->from <= dat->size - copy->width);
}

static bool copy_bytes(const DatTarget *target, DatCopy copy)
{
    const DatBytes *dat;

    if (!CHECK(target != NULL) || !CHECK(copy.at > 0U) || !CHECK(copy.from > 0U) ||
        !copy_is_inside(target->dat, &copy)) {
        return false;
    }
    dat = target->dat;
    if (memcmp(&dat->data[copy.at], &dat->data[copy.from], copy.width) == 0) {
        return true;
    }
    return dat_edit_copy(target->edit, &copy);
}

static bool copy_if_both(const DatTarget *target, DatCopy copy)
{
    if (!CHECK(target != NULL) || (copy.at == 0U) || (copy.from == 0U)) {
        return true;
    }
    return copy_bytes(target, copy);
}

static bool copy_field(const DatTarget *target, size_t to, size_t from)
{
    return CHECK(target != NULL) && copy_if_both(target, (DatCopy){ to, from, DAT_GRAPHIC_SIZE });
}

static bool copy_graphics(const DatTarget *target, const UnitPair *pair)
{
    if (!CHECK(target != NULL) || !CHECK(pair != NULL)) {
        return false;
    }
    return copy_bytes(target, (DatCopy){ pair->to.at + DAT_UNIT_GRAPHICS_AT, pair->from.at + DAT_UNIT_GRAPHICS_AT,
                                         DAT_UNIT_GRAPHICS_SIZE });
}

static bool copy_damage_graphics(const DatTarget *target, const UnitPair *pair)
{
    size_t width;

    if (!CHECK(target != NULL) || !CHECK(pair != NULL)) {
        return false;
    }
    if ((pair->to.damage_graphic_count != pair->from.damage_graphic_count) || (pair->to.damage_graphic_count <= 0)) {
        return true;
    }
    width = (size_t)pair->to.damage_graphic_count * DAT_DAMAGE_GRAPHIC_SIZE;
    return copy_bytes(target, (DatCopy){ pair->to.damage_graphics_at, pair->from.damage_graphics_at, width });
}

static bool copy_building(const DatTarget *target, const UnitPair *pair)
{
    if (!CHECK(target != NULL) || !CHECK(pair != NULL)) {
        return false;
    }
    return copy_graphics(target, pair) && copy_damage_graphics(target, pair) &&
           copy_if_both(target, (DatCopy){ pair->to.building_at, pair->from.building_at, DAT_BUILDING_SIZE }) &&
           copy_if_both(target, (DatCopy){ pair->to.garrison_graphic_at, pair->from.garrison_graphic_at,
                                           DAT_GARRISON_GRAPHIC_SIZE });
}

static bool copy_task_graphics(const DatTarget *target, const UnitPair *pair, size_t field_at)
{
    size_t step;
    int count;
    int task;
    bool fine = true;

    if (!CHECK(target != NULL) || !CHECK(pair != NULL) || !CHECK(field_at < DAT_TASK_SIZE)) {
        return false;
    }
    count = (pair->to.task_count < pair->from.task_count) ? pair->to.task_count : pair->from.task_count;
    for (task = 0; fine && (task < count) && (task < TASK_LIMIT); ++task) {
        step = (size_t)task * DAT_TASK_SIZE + field_at;
        fine = copy_field(target, pair->to.tasks_at + step, pair->from.tasks_at + step);
    }
    return fine;
}

static size_t task_graphic_of(int unit_class)
{
    if (unit_class == CLASS_TRADE_CART) {
        return DAT_TASK_CARRYING_GRAPHIC_AT;
    }
    return (unit_class == CLASS_MONK) ? DAT_TASK_PROCEEDING_GRAPHIC_AT : NO_TASK_GRAPHIC;
}

static bool copy_mobile(const DatTarget *target, const UnitPair *pair)
{
    size_t field;

    if (!CHECK(target != NULL) || !CHECK(pair != NULL)) {
        return false;
    }
    field = task_graphic_of(pair->from.unit_class);
    return copy_graphics(target, pair) &&
           copy_field(target, pair->to.walking_graphic_at, pair->from.walking_graphic_at) &&
           copy_field(target, pair->to.attack_graphic_at, pair->from.attack_graphic_at) &&
           ((field == NO_TASK_GRAPHIC) || copy_task_graphics(target, pair, field));
}

static bool list_both(const DatTarget *target, int from_civ)
{
    const DatMap *map;

    if (!CHECK(target != NULL) || !CHECK(target->civ > 0) || (from_civ < 0) ||
        !dat_map_has_civ(target->map, target->civ)) {
        return false;
    }
    map = target->map;
    if ((from_civ >= map->civ_count) || !CHECK(from_civ < DAT_CIV_CAPACITY)) {
        return false;
    }
    return dat_list_units(target->dat, map->civ_at[target->civ], &to_units) &&
           dat_list_units(target->dat, map->civ_at[from_civ], &from_units);
}

static size_t unit_place(const DatUnitList *list, int unit)
{
    if (!CHECK(list != NULL) || (unit < 0) || (unit >= list->count) || (unit >= DAT_UNIT_CAPACITY)) {
        return 0U;
    }
    return list->at[unit];
}

static bool read_units(const DatTarget *target, UnitIds ids, UnitPair *pair)
{
    size_t to = unit_place(&to_units, ids.to);
    size_t from = unit_place(&from_units, ids.from);

    if (!CHECK(target != NULL) || !CHECK(pair != NULL) || (to == 0U) || (from == 0U)) {
        return false;
    }
    return dat_unit_read(target->dat, to, &pair->to) && dat_unit_read(target->dat, from, &pair->from);
}

static bool read_pair(const DatTarget *target, int unit, UnitPair *pair)
{
    return CHECK(target != NULL) && read_units(target, (UnitIds){ unit, unit }, pair);
}

static bool copy_rubble(const DatTarget *target, int rubble)
{
    UnitPair pair;

    if (!CHECK(target != NULL) || !read_pair(target, rubble, &pair)) {
        return true;
    }
    if ((pair.from.unit_class != CLASS_TERRAIN) && (pair.from.unit_class != CLASS_MISCELLANEOUS)) {
        return true;
    }
    return copy_graphics(target, &pair);
}

enum { BUILDING_CLASS_COUNT = 4, MOBILE_CLASS_COUNT = 5 };

static const int BUILDING_CLASSES[BUILDING_CLASS_COUNT] = { CLASS_BUILDING, CLASS_TOWER, CLASS_WALL, CLASS_GATE };
static const int MOBILE_CLASSES[MOBILE_CLASS_COUNT] = { CLASS_KING, CLASS_MONK, CLASS_MONK_WITH_RELIC,
                                                        CLASS_TRADE_CART, CLASS_WARSHIP };

static bool has_a_look_of_its_own(int unit)
{
    return (unit == CASTLE_UNIT) || (unit == WONDER_UNIT);
}

static bool copy_building_and_rubble(const DatTarget *target, const UnitPair *pair)
{
    int rubble;

    if (!CHECK(target != NULL) || !CHECK(pair != NULL) ||
        !CHECK(target->edit != NULL)) {
        return false;
    }
    rubble = pair->from.dead_unit;
    if ((rubble < 0) || (rubble >= DAT_UNIT_CAPACITY) || rubble_done[rubble]) {
        return copy_building(target, pair);
    }
    rubble_done[rubble] = true;
    return copy_building(target, pair) && copy_rubble(target, rubble);
}

static bool copy_architecture_unit(const DatTarget *target, int unit)
{
    UnitPair pair;

    if (!CHECK(target != NULL) || !read_pair(target, unit, &pair)) {
        return true;
    }
    if (check_is_among(pair.from.unit_class, BUILDING_CLASSES, BUILDING_CLASS_COUNT)) {
        return has_a_look_of_its_own(unit) || copy_building_and_rubble(target, &pair);
    }
    return !check_is_among(pair.from.unit_class, MOBILE_CLASSES, MOBILE_CLASS_COUNT) || copy_mobile(target, &pair);
}

static void keep_own_rubble(const DatTarget *target, int unit)
{
    UnitPair pair;

    if (!CHECK(target != NULL) || !read_pair(target, unit, &pair)) {
        return;
    }
    if ((pair.to.dead_unit >= 0) && (pair.to.dead_unit < DAT_UNIT_CAPACITY)) {
        rubble_done[pair.to.dead_unit] = true;
    }
}

static bool copy_icon_set(const DatTarget *target, int from_civ)
{
    const DatMap *map;
    DatCiv to;
    DatCiv from;

    if (!CHECK(target != NULL) || !CHECK(from_civ >= 0) || !CHECK(from_civ < DAT_CIV_CAPACITY)) {
        return false;
    }
    map = target->map;
    return dat_civ_read(target->dat, map->civ_at[target->civ], &to) &&
           dat_civ_read(target->dat, map->civ_at[from_civ], &from) &&
           copy_bytes(target, (DatCopy){ to.icon_set_at, from.icon_set_at, ICON_SET_SIZE });
}

static bool copy_units(const DatTarget *target)
{
    int unit;
    bool fine = true;

    if (!CHECK(target != NULL) || !CHECK(to_units.count <= DAT_UNIT_CAPACITY) ||
        !CHECK(target->dat != NULL)) {
        return false;
    }
    for (unit = 0; fine && (unit < to_units.count) && (unit < DAT_UNIT_CAPACITY); ++unit) {
        fine = copy_architecture_unit(target, unit);
    }
    return fine;
}

static bool copy_monk_corpse(const DatTarget *target)
{
    UnitPair corpse;

    if (!CHECK(target != NULL) || !read_pair(target, MONK_CORPSE_UNIT, &corpse)) {
        return true;
    }
    return copy_graphics(target, &corpse);
}

static bool copy_architecture(const DatTarget *target, int from_civ)
{
    if (!CHECK(target != NULL) || !list_both(target, from_civ)) {
        return true;
    }
    (void)memset(rubble_done, 0, sizeof rubble_done);
    keep_own_rubble(target, CASTLE_UNIT);
    keep_own_rubble(target, WONDER_UNIT);
    return copy_units(target) && copy_monk_corpse(target) && copy_icon_set(target, from_civ);
}

static bool copy_one_building(const DatTarget *target, int from_civ, int unit)
{
    UnitPair pair;

    if (!CHECK(target != NULL) || !CHECK(unit >= 0) || !list_both(target, from_civ) ||
        !read_pair(target, unit, &pair)) {
        return true;
    }
    return copy_building(target, &pair) && copy_rubble(target, pair.from.dead_unit);
}

static bool copy_monk_unit(const DatTarget *target, const UnitPair *pair)
{
    if (!CHECK(target != NULL) || !CHECK(pair != NULL)) {
        return false;
    }
    return copy_graphics(target, pair) &&
           copy_field(target, pair->to.at + DAT_UNIT_ICON_AT, pair->from.at + DAT_UNIT_ICON_AT) &&
           copy_field(target, pair->to.walking_graphic_at, pair->from.walking_graphic_at) &&
           copy_field(target, pair->to.attack_graphic_at, pair->from.attack_graphic_at) &&
           copy_task_graphics(target, pair, DAT_TASK_PROCEEDING_GRAPHIC_AT);
}

static bool copy_monk(const DatTarget *target, const LookMonk *monk)
{
    UnitPair pair;
    int unit;
    bool fine = true;

    if (!CHECK(target != NULL) || !CHECK(monk != NULL) || !list_both(target, monk->civ)) {
        return true;
    }
    for (unit = 0; (unit < CATALOG_LOOK_UNITS) && fine; ++unit) {
        if (read_units(target, (UnitIds){ MONK_UNITS[unit], monk->units[unit] }, &pair) &&
            (pair.to.unit_class == pair.from.unit_class)) {
            fine = copy_monk_unit(target, &pair);
        }
    }
    return fine;
}

static LookMonk monk_of(const LookSources *sources)
{
    LookMonk monk = { LOOK_NONE, { LOOK_NONE, LOOK_NONE, LOOK_NONE } };
    int unit;

    if (!CHECK(sources != NULL)) {
        return monk;
    }
    monk = sources->monk;
    if (monk.civ < 0) {
        monk.civ = sources->architecture;
        for (unit = 0; unit < CATALOG_LOOK_UNITS; ++unit) {
            monk.units[unit] = MONK_UNITS[unit];
        }
    }
    return monk;
}

static int chosen_or(int choice, int fallback)
{
    return (choice < 0) ? fallback : choice;
}

bool look_build(const DatTarget *target, const LookSources *sources)
{
    LookMonk monk;
    int architecture;

    if (!CHECK(target != NULL) || !CHECK(sources != NULL)) {
        return false;
    }
    architecture = sources->architecture;
    monk = monk_of(sources);
    return copy_architecture(target, architecture) &&
           copy_one_building(target, chosen_or(sources->castle, architecture), CASTLE_UNIT) &&
           copy_one_building(target, chosen_or(sources->wonder, architecture), WONDER_UNIT) &&
           copy_monk(target, &monk);
}
