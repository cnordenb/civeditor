#include "base_civ.h"

#include <string.h>

#include "../core/check.h"

static CivDesign fresh;

static bool item_belongs(const TreeEntry *entry, int base_id)
{
    if (base_id == CIV_GENERIC_BASE) {
        return entry->universal;
    }
    return (entry->civs & ((uint64_t)1U << (unsigned)base_id)) != 0U;
}

static void set_tree(CivDesign *design, const Catalog *catalog, int base_id)
{
    const TreeEntry *entry;
    int index;

    for (index = 0; index < catalog->tree_count; ++index) {
        entry = &catalog->tree[index];
        if (item_belongs(entry, base_id)) {
            (void)civ_design_toggle_tree(design, entry->kind, entry->id);
        }
    }
}

static void set_bonuses(CivDesign *design, const BaseCiv *base)
{
    int index;

    for (index = 0; index < base->bonuses.count; ++index) {
        (void)civ_design_toggle_bonus(design, base->bonuses.values[index]);
    }
    if (base->values[BASE_TEAM_BONUS] >= 0) {
        (void)civ_design_toggle_team_bonus(design, base->values[BASE_TEAM_BONUS]);
    }
}

static void set_looks(CivDesign *design, const BaseCiv *base)
{
    (void)civ_design_set_choice(design, CHOICE_LANGUAGE, base->id);
    (void)civ_design_set_choice(design, CHOICE_WONDER, base->id);
    (void)civ_design_set_choice(design, CHOICE_CASTLE, base->id);
    (void)civ_design_set_choice(design, CHOICE_REPLACE, base->id);
    (void)civ_design_set_choice(design, CHOICE_ARCHITECTURE, base->values[BASE_ARCHITECTURE]);
    (void)civ_design_set_choice(design, CHOICE_MONK, base->values[BASE_MONK]);
    (void)civ_design_set_choice(design, CHOICE_SCOUT, base->values[BASE_SCOUT]);
}

static void set_uniques(CivDesign *design, const BaseCiv *base)
{
    if (base->values[BASE_UNIT] >= 0) {
        (void)civ_design_pick_unit(design, UNIT_ORIGIN_VANILLA, base->values[BASE_UNIT]);
    }
    (void)civ_design_pick_tech(design, TECH_TIER_CASTLE, base->values[BASE_CASTLE_TECH]);
    (void)civ_design_pick_tech(design, TECH_TIER_IMPERIAL, base->values[BASE_IMPERIAL_TECH]);
}

bool base_civ_apply(CivDesign *design, const Catalog *catalog, int base_id)
{
    int found;

    found = catalog_find_base(catalog, base_id);
    if (found == CATALOG_MISSING) {
        return false;
    }
    civ_design_reset(&fresh);
    (void)memcpy(fresh.texts, design->texts, sizeof fresh.texts);
    fresh.hero_unit = design->hero_unit;
    *design = fresh;
    set_tree(design, catalog, base_id);
    if (base_id != CIV_GENERIC_BASE) {
        set_bonuses(design, &catalog->bases[found]);
        set_looks(design, &catalog->bases[found]);
        set_uniques(design, &catalog->bases[found]);
    }
    design->choices[CHOICE_BASE] = base_id;
    return true;
}
