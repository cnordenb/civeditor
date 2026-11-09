#include "unique_aim.h"

#include "../core/check.h"

enum {
    NO_UNIT = -1,
    SET_ATTRIBUTE = 0,
    ENABLE_UNIT = 2,
    UPGRADE_UNIT = 3,
    ADD_ATTRIBUTE = 4,
    MULTIPLY_ATTRIBUTE = 5,
    SPAWN_UNIT = 7,
    TEAM_ENABLE_UNIT = 12,
    ARMOR_ATTRIBUTE = 8,
    ATTACK_ATTRIBUTE = 9,
    ARMOR_CLASS_SIZE = 256,
    CHANGE_LIMIT = 32768
};

static int unit_in_effect(const UniqueSource *source, int tech_id, int wanted_type)
{
    const DatMap *map;
    DatTech tech;
    DatEffect effect;
    DatChange change;
    int index;

    if (!CHECK(source != NULL) || !CHECK(source->map != NULL) || (tech_id < 0)) {
        return NO_UNIT;
    }
    map = source->map;
    if ((tech_id >= map->tech_count) || !dat_tech_read(source->dat, map->tech_at[tech_id], &tech) ||
        (tech.effect_id < 0) || (tech.effect_id >= map->effect_count) ||
        !dat_effect_read(source->dat, map->effect_at[tech.effect_id], &effect)) {
        return NO_UNIT;
    }
    for (index = 0; (index < effect.change_count) && (index < CHANGE_LIMIT); ++index) {
        if (!dat_change_read(source->dat, effect.changes_at + ((size_t)index * DAT_CHANGE_SIZE), &change)) {
            return NO_UNIT;
        }
        if ((change.type == ENABLE_UNIT) && (wanted_type == ENABLE_UNIT) && (change.b != 0)) {
            return change.a;
        }
        if ((change.type == UPGRADE_UNIT) && (wanted_type == UPGRADE_UNIT)) {
            return change.b;
        }
    }
    return NO_UNIT;
}

UniqueUnit unique_unit_of_techs(const UniqueSource *source, int make_tech, int elite_tech)
{
    UniqueUnit unit = { NO_UNIT, NO_UNIT };

    if (!CHECK(source != NULL) || !CHECK(source->dat != NULL)) {
        return unit;
    }
    unit.base = unit_in_effect(source, make_tech, ENABLE_UNIT);
    unit.elite = unit_in_effect(source, elite_tech, UPGRADE_UNIT);
    return unit;
}

UniqueUnit unique_unit_of_civ(const UniqueSource *source, int civ)
{
    UniqueUnit none = { NO_UNIT, NO_UNIT };
    const Catalog *catalog;
    const UnitEntry *entry;
    const DatMap *map;
    DatTech tech;
    int index;

    if (!CHECK(source != NULL) || !CHECK((source->catalog != NULL) && (source->map != NULL)) || (civ < 0)) {
        return none;
    }
    catalog = source->catalog;
    map = source->map;
    for (index = 0; (index < catalog->unit_count) && (index < CATALOG_UNIT_CAPACITY); ++index) {
        entry = &catalog->units[index];
        if ((entry->role == UNIT_ROLE_VANILLA) && (entry->make_tech >= 0) && (entry->make_tech < map->tech_count) &&
            dat_tech_read(source->dat, map->tech_at[entry->make_tech], &tech) && (tech.civ == civ)) {
            return unique_unit_of_techs(source, entry->make_tech, entry->elite_tech);
        }
    }
    return none;
}

static bool is_source_unit(const UniqueSwap *swap, int unit)
{
    if (!CHECK(swap != NULL) || (unit == NO_UNIT)) {
        return false;
    }
    return (unit == swap->from.base) || (unit == swap->from.elite);
}

static UniqueAim aim_at(DatChange *change, int unit)
{
    if (!CHECK(change != NULL) || (unit == NO_UNIT)) {
        return UNIQUE_DROP;
    }
    change->a = unit;
    return UNIQUE_SWAP;
}

UniqueAim unique_aim(const UniqueSwap *swap, DatChange *change)
{
    bool stat = false;

    if (!CHECK(swap != NULL) || !CHECK(change != NULL)) {
        return UNIQUE_DROP;
    }
    switch (change->type) {
    case ENABLE_UNIT:
        if ((change->b != 0) && (change->a == swap->from.base) && (change->a != NO_UNIT)) {
            return aim_at(change, swap->to.base);
        }
        return is_source_unit(swap, change->a) ? UNIQUE_DROP : UNIQUE_KEEP;
    case UPGRADE_UNIT:
        return (is_source_unit(swap, change->a) || is_source_unit(swap, change->b)) ? UNIQUE_DROP : UNIQUE_KEEP;
    case SPAWN_UNIT:
        return aim_at(change, swap->to.elite);
    case TEAM_ENABLE_UNIT:
        return UNIQUE_DROP;
    case SET_ATTRIBUTE:
    case ADD_ATTRIBUTE:
    case MULTIPLY_ATTRIBUTE:
        stat = true;
        break;
    default:
        break;
    }
    if (!stat || !is_source_unit(swap, change->a)) {
        return UNIQUE_KEEP;
    }
    return aim_at(change, (change->a == swap->from.base) ? swap->to.base : swap->to.elite);
}

static int armor_class(const DatChange *change)
{
    if (!CHECK(change != NULL) || ((change->c != ARMOR_ATTRIBUTE) && (change->c != ATTACK_ATTRIBUTE))) {
        return NO_UNIT;
    }
    return (int)change->d / ARMOR_CLASS_SIZE;
}

bool unique_same_target(const DatChange *left, const DatChange *right)
{
    if (!CHECK(left != NULL) || !CHECK(right != NULL)) {
        return false;
    }
    if ((left->type != SET_ATTRIBUTE) && (left->type != ADD_ATTRIBUTE) && (left->type != MULTIPLY_ATTRIBUTE)) {
        return false;
    }
    return (left->type == right->type) && (left->a == right->a) && (left->b == right->b) &&
           (left->c == right->c) && (armor_class(left) == armor_class(right));
}
