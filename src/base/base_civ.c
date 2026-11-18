#include "base_civ.h"

#include "../model/command.h"

static void set_tree(CivDesign *design, const Catalog *catalog, int base_id)
{
    const TreeEntry *entry;
    int index;

    for (index = 0; index < catalog->tree_count; ++index) {
        entry = &catalog->tree[index];
        if ((entry->civs & ((uint64_t)1U << (unsigned)base_id)) != 0U) {
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

static const char *name_in(const Catalog *catalog, CatalogTable table, int kind, int id)
{
    int found = catalog_find(catalog, table, kind, id);

    if (found == CATALOG_MISSING) {
        return "";
    }
    return (table == CATALOG_UNITS) ? catalog->units[found].name : catalog->techs[found].name;
}

static void pick(CivDesign *design, const Catalog *catalog, CommandKind kind, int field, int id)
{
    CatalogTable table = (kind == COMMAND_PICK_UNIT) ? CATALOG_UNITS : CATALOG_TECHS;
    int role = (kind == COMMAND_PICK_UNIT) ? (int)UNIT_ROLE_VANILLA : field;
    Command command = command_pick(kind, field, id, name_in(catalog, table, role, id));

    (void)command_apply(design, &command);
}

static void set_uniques(CivDesign *design, const Catalog *catalog, const BaseCiv *base)
{
    if (base->values[BASE_UNIT] >= 0) {
        pick(design, catalog, COMMAND_PICK_UNIT, (int)UNIT_ORIGIN_VANILLA, base->values[BASE_UNIT]);
    }
    pick(design, catalog, COMMAND_PICK_TECH, (int)TECH_TIER_CASTLE, base->values[BASE_CASTLE_TECH]);
    pick(design, catalog, COMMAND_PICK_TECH, (int)TECH_TIER_IMPERIAL, base->values[BASE_IMPERIAL_TECH]);
}

static void set_texts(CivDesign *design, const Catalog *catalog, const BaseCiv *base)
{
    (void)civ_design_set_text(design, TEXT_FIELD_NAME, catalog_appearance_name(catalog, APPEARANCE_BASE, base->id));
    (void)civ_design_set_text(design, TEXT_FIELD_TAGLINE, base->tagline);
    (void)civ_design_set_text(design, TEXT_FIELD_DESCRIPTION, base->description);
}

bool base_civ_apply(CivDesign *design, const Catalog *catalog, int base_id)
{
    const BaseCiv *base;
    int found;

    found = catalog_find_base(catalog, base_id);
    if (found == CATALOG_MISSING) {
        return false;
    }
    base = &catalog->bases[found];
    civ_design_reset(design);
    set_tree(design, catalog, base_id);
    set_bonuses(design, base);
    set_looks(design, base);
    set_uniques(design, catalog, base);
    set_texts(design, catalog, base);
    return true;
}
