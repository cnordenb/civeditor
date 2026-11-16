#include "recipe_parts.h"

#include "../core/check.h"

enum { RESOURCE_CHANGE = 1 };

static bool disableable[DAT_TECH_CAPACITY];
static bool kept[DAT_TECH_CAPACITY];
static bool shielded[DAT_TECH_CAPACITY];
static bool unlockable[DAT_TECH_CAPACITY];
static DatChange unlocks[DAT_TECH_CAPACITY];

static int change_key(const DatChange *change)
{
    if (!CHECK(change != NULL) || !CHECK(change->type >= 0)) {
        return DAT_NO_ID;
    }
    return (change->type == RECIPE_DISABLE_TECH) ? (int)change->d : change->a;
}

void recipe_add_change(RecipeChanges *changes, const DatChange *change)
{
    int index;

    if (!CHECK(changes != NULL) || !CHECK(change != NULL)) {
        return;
    }
    for (index = 0; (index < changes->count) && (index < RECIPE_TREE_CAPACITY); ++index) {
        if ((changes->changes[index].type == change->type) &&
            (change_key(&changes->changes[index]) == change_key(change))) {
            return;
        }
    }
    if (changes->count >= RECIPE_TREE_CAPACITY) {
        changes->overflowed = true;
        return;
    }
    changes->changes[changes->count] = *change;
    ++changes->count;
}

static bool comes_before(const DatChange *first, const DatChange *second)
{
    if (!CHECK(first != NULL) || !CHECK(second != NULL)) {
        return false;
    }
    if (first->type != second->type) {
        return (first->type == RECIPE_UNLOCK_TECH) ||
               ((second->type != RECIPE_UNLOCK_TECH) && (first->type < second->type));
    }
    return change_key(first) < change_key(second);
}

void recipe_sort_changes(RecipeChanges *changes)
{
    DatChange moving;
    int index;
    int place;

    if (!CHECK(changes != NULL) || !CHECK(changes->count <= RECIPE_TREE_CAPACITY)) {
        return;
    }
    for (index = 1; (index < changes->count) && (index < RECIPE_TREE_CAPACITY); ++index) {
        moving = changes->changes[index];
        for (place = index;
             (place > 0) && (place < RECIPE_TREE_CAPACITY) && comes_before(&moving, &changes->changes[place - 1]);
             --place) {
            changes->changes[place] = changes->changes[place - 1];
        }
        changes->changes[place] = moving;
    }
}

static bool civ_tree_effect(const RecipeInput *input, int civ_index, DatEffect *effect)
{
    const DatMap *map;
    DatCiv civ;

    if (!CHECK(input != NULL) || !CHECK(effect != NULL)) {
        return false;
    }
    map = input->map;
    return dat_civ_read(input->dat, map->civ_at[civ_index], &civ) &&
           recipe_read_effect(input, civ.tech_tree_id, effect);
}

static bool unlock_in_effect(const RecipeInput *input, const DatEffect *effect, int tech_id, DatChange *unlock)
{
    RecipeChange found;
    int index;

    if (!CHECK(input != NULL) || !CHECK(effect != NULL)) {
        return false;
    }
    for (index = 0; (index < effect->change_count) && (index < RECIPE_CHANGE_LIMIT); ++index) {
        found = recipe_change_at(input, effect, index);
        if ((found.change.type == RECIPE_UNLOCK_TECH) && (found.change.a == tech_id) && found.read) {
            *unlock = found.change;
            return true;
        }
    }
    return false;
}

bool recipe_find_unlock(const RecipeInput *input, int tech_id, DatChange *unlock)
{
    DatEffect effect;
    const DatMap *map;
    int civ_index;

    if (!CHECK(input != NULL) || !CHECK(unlock != NULL)) {
        return false;
    }
    map = input->map;
    for (civ_index = 1; (civ_index < map->civ_count) && (civ_index < DAT_CIV_CAPACITY); ++civ_index) {
        if (civ_tree_effect(input, civ_index, &effect) && unlock_in_effect(input, &effect, tech_id, unlock)) {
            return true;
        }
    }
    return false;
}

static void mark(bool *marks, int tech_id)
{
    if (!CHECK(marks != NULL) || (tech_id < 0) || (tech_id >= DAT_TECH_CAPACITY)) {
        return;
    }
    marks[tech_id] = true;
}

static void note_civ_change(const DatChange *change)
{
    if (!CHECK(change != NULL) || !CHECK(change->type >= 0)) {
        return;
    }
    if (change->type == RECIPE_DISABLE_TECH) {
        mark(disableable, (int)change->d);
    }
    if ((change->type == RECIPE_UNLOCK_TECH) && (change->a >= 0) && (change->a < DAT_TECH_CAPACITY) &&
        !unlockable[change->a]) {
        unlockable[change->a] = true;
        unlocks[change->a] = *change;
    }
}

static void note_civ_changes(const RecipeInput *input, const DatEffect *effect)
{
    RecipeChange found;
    int index;

    if (!CHECK(input != NULL) || !CHECK(effect != NULL)) {
        return;
    }
    for (index = 0; (index < effect->change_count) && (index < RECIPE_CHANGE_LIMIT); ++index) {
        found = recipe_change_at(input, effect, index);
        if (found.read) {
            note_civ_change(&found.change);
        }
    }
}

static void scan_civ_trees(const RecipeInput *input)
{
    DatEffect effect;
    const DatMap *map;
    int civ_index;

    if (!CHECK(input != NULL) || !CHECK(input->map != NULL)) {
        return;
    }
    map = input->map;
    for (civ_index = 1; (civ_index < map->civ_count) && (civ_index < DAT_CIV_CAPACITY); ++civ_index) {
        if (civ_tree_effect(input, civ_index, &effect)) {
            note_civ_changes(input, &effect);
        }
    }
}

static bool unit_chosen(const CivDesign *design, int unit)
{
    if (!CHECK(design != NULL) || !CHECK(unit >= 0)) {
        return false;
    }
    return civ_design_tree_keeps(design, TREE_UNITS, unit) || civ_design_tree_keeps(design, TREE_BUILDINGS, unit);
}

static int changed_unit(const DatChange *change)
{
    if (!CHECK(change != NULL) || !CHECK(RECIPE_ENABLE_UNIT != RECIPE_UPGRADE_UNIT)) {
        return DAT_NO_ID;
    }
    if (change->type == RECIPE_UPGRADE_UNIT) {
        return change->b;
    }
    return ((change->type == RECIPE_ENABLE_UNIT) && (change->b != 0)) ? change->a : DAT_NO_ID;
}

static bool is_in_tree(const Catalog *catalog, int unit)
{
    return (catalog_find(catalog, CATALOG_TREE, TREE_UNITS, unit) != CATALOG_MISSING) ||
           (catalog_find(catalog, CATALOG_TREE, TREE_BUILDINGS, unit) != CATALOG_MISSING);
}

static void note_unit_change(const RecipeInput *input, int tech_id, const DatChange *change)
{
    int unit = changed_unit(change);

    if (!CHECK(input != NULL) || !CHECK(change != NULL) || (unit < 0)) {
        return;
    }
    if (unit_chosen(input->design, unit)) {
        mark(kept, tech_id);
    } else if (is_in_tree(input->catalog, unit)) {
        mark(disableable, tech_id);
    }
}

static bool researched_nowhere(const DatTech *tech)
{
    int location;

    if (!CHECK(tech != NULL) || !CHECK(tech->research_location_count <= DAT_RESEARCH_LOCATION_CAPACITY)) {
        return false;
    }
    for (location = 0; (location < tech->research_location_count) && (location < DAT_RESEARCH_LOCATION_CAPACITY);
         ++location) {
        if (tech->research_locations[location].location_id >= 0) {
            return false;
        }
    }
    return true;
}

enum { AGE_COUNT = 3 };

static const int AGES[AGE_COUNT] = { RECIPE_FEUDAL_AGE, RECIPE_CASTLE_AGE, RECIPE_IMPERIAL_AGE };

static bool changes_only_resources(const RecipeInput *input, int tech_id, const DatEffect *effect)
{
    RecipeChange found;
    bool only_resources = true;
    int index;

    if (!CHECK(input != NULL) || !CHECK(effect != NULL)) {
        return false;
    }
    for (index = 0; (index < effect->change_count) && (index < RECIPE_CHANGE_LIMIT); ++index) {
        found = recipe_change_at(input, effect, index);
        if (found.read) {
            note_unit_change(input, tech_id, &found.change);
            only_resources = only_resources && (found.change.type == RESOURCE_CHANGE);
        }
    }
    return only_resources;
}

static void scan_tech(const RecipeInput *input, int tech_id)
{
    DatEffect effect;
    DatTech tech;

    if (!CHECK(input != NULL) || !recipe_read_tech(input, tech_id, &tech)) {
        return;
    }
    shielded[tech_id] = (tech.civ >= 0) || check_is_among(tech_id, AGES, AGE_COUNT);
    if (shielded[tech_id] || !recipe_read_effect(input, tech.effect_id, &effect)) {
        return;
    }
    shielded[tech_id] = changes_only_resources(input, tech_id, &effect) && (effect.change_count > 0) &&
                        researched_nowhere(&tech);
}

static void mark_catalog_techs(const RecipeInput *input)
{
    const Catalog *catalog;
    const TreeEntry *node;
    int index;

    if (!CHECK(input != NULL) || !CHECK(input->catalog != NULL)) {
        return;
    }
    catalog = input->catalog;
    for (index = 0; (index < catalog->tree_count) && (index < CATALOG_TREE_CAPACITY); ++index) {
        node = &catalog->tree[index];
        if (node->kind == TREE_TECHS) {
            mark(disableable, node->id);
            if (civ_design_tree_has(input->design, TREE_TECHS, node->id)) {
                mark(kept, node->id);
            }
        }
    }
}

static bool is_disabled(int tech_id)
{
    if (!CHECK(tech_id >= 0) || !CHECK(tech_id < DAT_TECH_CAPACITY)) {
        return false;
    }
    return disableable[tech_id] && !kept[tech_id] && !shielded[tech_id];
}

static void emit_changes(int tech_count, RecipeChanges *changes, RecipeReport *report)
{
    DatChange disable = { RECIPE_DISABLE_TECH, DAT_NO_ID, DAT_NO_ID, DAT_NO_ID, 0.0F };
    int tech_id;

    if (!CHECK(changes != NULL) || !CHECK(report != NULL)) {
        return;
    }
    for (tech_id = 0; (tech_id < tech_count) && (tech_id < DAT_TECH_CAPACITY); ++tech_id) {
        if (kept[tech_id] && unlockable[tech_id]) {
            recipe_add_change(changes, &unlocks[tech_id]);
        }
        if (is_disabled(tech_id)) {
            disable.d = (float)tech_id;
            recipe_add_change(changes, &disable);
            ++report->tree_disabled;
        }
    }
}

bool recipe_wire_tree(const RecipeInput *input, RecipeChanges *changes, RecipeReport *report)
{
    const CivDesign *design;
    const DatMap *map;
    int index;

    if (!CHECK(input != NULL) || !CHECK(changes != NULL) || !CHECK(report != NULL)) {
        return false;
    }
    design = input->design;
    map = input->map;
    if (civ_design_tree_is_empty(design)) {
        return true;
    }
    for (index = 0; index < DAT_TECH_CAPACITY; ++index) {
        disableable[index] = false;
        kept[index] = false;
        shielded[index] = true;
        unlockable[index] = false;
    }
    scan_civ_trees(input);
    for (index = 0; (index < map->tech_count) && (index < DAT_TECH_CAPACITY); ++index) {
        scan_tech(input, index);
    }
    mark_catalog_techs(input);
    emit_changes(map->tech_count, changes, report);
    return true;
}
