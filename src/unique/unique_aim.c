#include "unique_aim.h"

#include "../core/check.h"
#include "../forge/recipe_parts.h"

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

typedef struct {
    bool found;
    int unit;
} UnitHit;

static bool names_unit(const DatChange *change, int wanted_type)
{
    if (!CHECK((wanted_type == ENABLE_UNIT) || (wanted_type == UPGRADE_UNIT))) {
        return false;
    }
    return (change->type == wanted_type) && ((wanted_type == UPGRADE_UNIT) || (change->b != 0));
}

static UnitHit unit_at_change(const RecipeInput *input, const DatEffect *effect, int index, int wanted_type)
{
    UnitHit hit = { true, NO_UNIT };
    RecipeChange read;

    read = recipe_change_at(input, effect, index);
    if (!read.read) {
        return hit;
    }
    hit.found = names_unit(&read.change, wanted_type);
    if (hit.found) {
        hit.unit = (wanted_type == ENABLE_UNIT) ? read.change.a : read.change.b;
    }
    return hit;
}

static int unit_in_effect(const RecipeInput *input, int tech_id, int wanted_type)
{
    UnitHit hit = { false, NO_UNIT };
    DatTech tech;
    DatEffect effect;
    int index;

    if (!recipe_read_tech(input, tech_id, &tech) || !recipe_read_effect(input, tech.effect_id, &effect)) {
        return NO_UNIT;
    }
    for (index = 0; !hit.found && (index < effect.change_count) && (index < CHANGE_LIMIT); ++index) {
        hit = unit_at_change(input, &effect, index, wanted_type);
    }
    return hit.unit;
}

UniqueUnit unique_unit_of_techs(const RecipeInput *input, int make_tech, int elite_tech)
{
    UniqueUnit unit = { NO_UNIT, NO_UNIT };

    unit.base = unit_in_effect(input, make_tech, ENABLE_UNIT);
    unit.elite = unit_in_effect(input, elite_tech, UPGRADE_UNIT);
    return unit;
}

static bool is_made_by_civ(const RecipeInput *input, const UnitEntry *entry, int civ)
{
    DatTech tech;

    if (entry->role != UNIT_ROLE_VANILLA) {
        return false;
    }
    return recipe_read_tech(input, entry->make_tech, &tech) && (tech.civ == civ);
}

UniqueUnit unique_unit_of_civ(const RecipeInput *input, int civ)
{
    UniqueUnit none = { NO_UNIT, NO_UNIT };
    const Catalog *catalog;
    const UnitEntry *entry;
    int index;

    if (civ < 0) {
        return none;
    }
    catalog = input->catalog;
    for (index = 0; (index < catalog->unit_count) && (index < CATALOG_UNIT_CAPACITY); ++index) {
        entry = &catalog->units[index];
        if (is_made_by_civ(input, entry, civ)) {
            return unique_unit_of_techs(input, entry->make_tech, entry->elite_tech);
        }
    }
    return none;
}

static bool is_source_unit(const UniqueSwap *swap, int unit)
{
    if (unit == NO_UNIT) {
        return false;
    }
    return (unit == swap->from.base) || (unit == swap->from.elite);
}

static UniqueAim aim_at(DatChange *change, int unit)
{
    if (unit == NO_UNIT) {
        return UNIQUE_DROP;
    }
    change->a = unit;
    return UNIQUE_SWAP;
}

enum { STAT_CHANGE_COUNT = 3 };

static const int STAT_CHANGES[STAT_CHANGE_COUNT] = { SET_ATTRIBUTE, ADD_ATTRIBUTE, MULTIPLY_ATTRIBUTE };

static UniqueAim aim_stat(const UniqueSwap *swap, DatChange *change)
{
    if (!is_source_unit(swap, change->a)) {
        return UNIQUE_KEEP;
    }
    return aim_at(change, (change->a == swap->from.base) ? swap->to.base : swap->to.elite);
}

static UniqueAim aim_enable(const UniqueSwap *swap, DatChange *change)
{
    if ((change->b != 0) && (change->a == swap->from.base) && (change->a != NO_UNIT)) {
        return aim_at(change, swap->to.base);
    }
    return is_source_unit(swap, change->a) ? UNIQUE_DROP : UNIQUE_KEEP;
}

static UniqueAim aim_upgrade(const UniqueSwap *swap, const DatChange *change)
{
    return (is_source_unit(swap, change->a) || is_source_unit(swap, change->b)) ? UNIQUE_DROP : UNIQUE_KEEP;
}

UniqueAim unique_aim(const UniqueSwap *swap, DatChange *change)
{
    if (check_is_among(change->type, STAT_CHANGES, STAT_CHANGE_COUNT)) {
        return aim_stat(swap, change);
    }
    switch (change->type) {
    case ENABLE_UNIT:
        return aim_enable(swap, change);
    case UPGRADE_UNIT:
        return aim_upgrade(swap, change);
    case SPAWN_UNIT:
        return aim_at(change, swap->to.elite);
    case TEAM_ENABLE_UNIT:
        return UNIQUE_DROP;
    default:
        return UNIQUE_KEEP;
    }
}

static int armor_class(const DatChange *change)
{
    if ((change->c != ARMOR_ATTRIBUTE) && (change->c != ATTACK_ATTRIBUTE)) {
        return NO_UNIT;
    }
    return (int)change->d / ARMOR_CLASS_SIZE;
}

static bool same_attribute(const DatChange *left, const DatChange *right)
{
    return (left->type == right->type) && (left->a == right->a) && (left->b == right->b) && (left->c == right->c);
}

bool unique_same_target(const DatChange *left, const DatChange *right)
{
    if (!check_is_among(left->type, STAT_CHANGES, STAT_CHANGE_COUNT)) {
        return false;
    }
    return same_attribute(left, right) && (armor_class(left) == armor_class(right));
}
