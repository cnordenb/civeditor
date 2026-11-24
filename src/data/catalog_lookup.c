#include "catalog.h"

#include "../core/check.h"

typedef struct {
    int kind;
    int id;
} CatalogKey;

static const int TABLE_CAPACITIES[CATALOG_TABLE_COUNT] = { CATALOG_UNIT_CAPACITY, CATALOG_TECH_CAPACITY,
                                                           CATALOG_TREE_CAPACITY, CATALOG_APPEARANCE_CAPACITY,
                                                           CATALOG_BASE_CAPACITY };

static CatalogKey key_at(const Catalog *catalog, CatalogTable table, int index)
{
    CatalogKey key = { 0, CATALOG_MISSING };

    if (!CHECK(index >= 0) || !CHECK(table < CATALOG_TABLE_COUNT)) {
        return key;
    }
    if (table == CATALOG_UNITS) {
        key = (CatalogKey){ (int)catalog->units[index].role, catalog->units[index].id };
    } else if (table == CATALOG_TECHS) {
        key = (CatalogKey){ (int)catalog->techs[index].tier, catalog->techs[index].id };
    } else if (table == CATALOG_TREE) {
        key = (CatalogKey){ (int)catalog->tree[index].kind, catalog->tree[index].id };
    } else if (table == CATALOG_APPEARANCES) {
        key = (CatalogKey){ (int)catalog->appearance[index].kind, catalog->appearance[index].id };
    } else {
        key.id = catalog->bases[index].id;
    }
    return key;
}

static int table_count(const Catalog *catalog, CatalogTable table)
{
    int counts[CATALOG_TABLE_COUNT] = { 0, 0, 0, 0, 0 };

    if (!CHECK(table < CATALOG_TABLE_COUNT)) {
        return 0;
    }
    counts[CATALOG_UNITS] = catalog->unit_count;
    counts[CATALOG_TECHS] = catalog->tech_count;
    counts[CATALOG_TREE] = catalog->tree_count;
    counts[CATALOG_APPEARANCES] = catalog->appearance_count;
    counts[CATALOG_BASES] = catalog->base_count;
    return counts[table];
}

int catalog_find(const Catalog *catalog, CatalogTable table, int kind, int id)
{
    int count = table_count(catalog, table);
    CatalogKey key;
    int index;

    if (!CHECK(count <= TABLE_CAPACITIES[table]) || !CHECK(table < CATALOG_TABLE_COUNT)) {
        return CATALOG_MISSING;
    }
    for (index = 0; (index < count) && (index < TABLE_CAPACITIES[table]); ++index) {
        key = key_at(catalog, table, index);
        if ((key.kind == kind) && (key.id == id)) {
            return index;
        }
    }
    return CATALOG_MISSING;
}

int catalog_find_unit(const Catalog *catalog, const UnitPick *pick)
{
    UnitRole role = (pick->origin == UNIT_ORIGIN_CUSTOM) ? UNIT_ROLE_CUSTOM : UNIT_ROLE_VANILLA;

    return catalog_find(catalog, CATALOG_UNITS, (int)role, pick->id);
}

static int node_in_building(const Catalog *catalog, int id, int building)
{
    int index;

    for (index = 0; index < catalog->tree_count; ++index) {
        if ((catalog->tree[index].id == id) && (catalog->tree[index].building == building)) {
            return index;
        }
    }
    return CATALOG_MISSING;
}

TreeNeeds catalog_tree_needs(const Catalog *catalog, int node)
{
    const TreeEntry *entry = &catalog->tree[node];
    TreeNeeds needs = { CATALOG_MISSING, CATALOG_MISSING };

    needs.parent = node_in_building(catalog, entry->parent, entry->building);
    if (needs.parent == CATALOG_MISSING) {
        needs.parent = catalog_find(catalog, CATALOG_TREE, TREE_BUILDINGS, entry->parent);
    }
    if (entry->kind != TREE_BUILDINGS) {
        needs.building = catalog_find(catalog, CATALOG_TREE, TREE_BUILDINGS, entry->building);
    }
    return needs;
}

const char *catalog_appearance_name(const Catalog *catalog, AppearanceKind kind, int id)
{
    int found = catalog_find(catalog, CATALOG_APPEARANCES, (int)kind, id);

    return (found == CATALOG_MISSING) ? "" : catalog->appearance[found].name;
}

int catalog_string_id(const Catalog *catalog, int slot, CivString kind)
{
    const AppearanceEntry *entry;
    int index;

    for (index = 0; index < catalog->appearance_count; ++index) {
        entry = &catalog->appearance[index];
        if ((entry->kind == APPEARANCE_CIV) && (entry->id == slot) && (entry->string_ids[kind] > 0)) {
            return entry->string_ids[kind];
        }
    }
    return CATALOG_MISSING;
}

int catalog_find_look(const Catalog *catalog, AppearanceKind kind, int id)
{
    const AppearanceEntry *entry;
    int index;

    if (!CHECK(catalog->appearance_count <= CATALOG_APPEARANCE_CAPACITY)) {
        return CATALOG_MISSING;
    }
    for (index = 0; (index < catalog->appearance_count) && (index < CATALOG_APPEARANCE_CAPACITY); ++index) {
        entry = &catalog->appearance[index];
        if ((entry->kind == kind) && (entry->id == id) && (entry->look_civ >= 0)) {
            return index;
        }
    }
    return CATALOG_MISSING;
}

int catalog_find_bonus(const Catalog *catalog, bool team, int bonus_id)
{
    const BonusEntry *entries;
    int count;
    int index;

    if (!CHECK(catalog->bonus_count <= CATALOG_BONUS_CAPACITY) ||
        !CHECK(catalog->team_bonus_count <= CATALOG_TEAM_BONUS_CAPACITY)) {
        return CATALOG_MISSING;
    }
    entries = team ? catalog->team_bonuses : catalog->bonuses;
    count = team ? catalog->team_bonus_count : catalog->bonus_count;
    for (index = 0; (index < count) && (index < CATALOG_BONUS_CAPACITY); ++index) {
        if (entries[index].id == bonus_id) {
            return index;
        }
    }
    return CATALOG_MISSING;
}

int catalog_find_base(const Catalog *catalog, int base_id)
{
    return catalog_find(catalog, CATALOG_BASES, 0, base_id);
}

bool catalog_is_ranked(const Catalog *catalog, int slot)
{
    return catalog_find(catalog, CATALOG_APPEARANCES, (int)APPEARANCE_UNRANKED, slot) == CATALOG_MISSING;
}
