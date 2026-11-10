#include "catalog.h"

#include "../core/check.h"

const char *catalog_appearance_name(const Catalog *catalog, AppearanceKind kind, int id)
{
    int index;

    if (!CHECK(catalog != NULL) || !CHECK(catalog->appearance_count <= CATALOG_APPEARANCE_CAPACITY)) {
        return "";
    }
    for (index = 0; (index < catalog->appearance_count) && (index < CATALOG_APPEARANCE_CAPACITY); ++index) {
        if ((catalog->appearance[index].kind == kind) && (catalog->appearance[index].id == id)) {
            return catalog->appearance[index].name;
        }
    }
    return "";
}

int catalog_string_id(const Catalog *catalog, int slot, CivString kind)
{
    const AppearanceEntry *entry;
    int index;

    if (!CHECK(catalog != NULL) || !CHECK((kind >= CIV_STRING_CASTLE_TECH) && (kind < CIV_STRING_COUNT))) {
        return CATALOG_MISSING;
    }
    for (index = 0; (index < catalog->appearance_count) && (index < CATALOG_APPEARANCE_CAPACITY); ++index) {
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

    if (!CHECK(catalog != NULL) || !CHECK(catalog->appearance_count <= CATALOG_APPEARANCE_CAPACITY)) {
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

    if (!CHECK(catalog != NULL) || !CHECK(catalog->bonus_count <= CATALOG_BONUS_CAPACITY) ||
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

int catalog_find_unit(const Catalog *catalog, UnitRole role, int unit_id)
{
    int index;

    if (!CHECK(catalog != NULL) || !CHECK(catalog->unit_count <= CATALOG_UNIT_CAPACITY)) {
        return CATALOG_MISSING;
    }
    for (index = 0; (index < catalog->unit_count) && (index < CATALOG_UNIT_CAPACITY); ++index) {
        if ((catalog->units[index].role == role) && (catalog->units[index].id == unit_id)) {
            return index;
        }
    }
    return CATALOG_MISSING;
}

int catalog_find_tech(const Catalog *catalog, TechTier tier, int tech_id)
{
    int index;

    if (!CHECK(catalog != NULL) || !CHECK(catalog->tech_count <= CATALOG_TECH_CAPACITY)) {
        return CATALOG_MISSING;
    }
    for (index = 0; (index < catalog->tech_count) && (index < CATALOG_TECH_CAPACITY); ++index) {
        if ((catalog->techs[index].tier == tier) && (catalog->techs[index].id == tech_id)) {
            return index;
        }
    }
    return CATALOG_MISSING;
}

int catalog_find_tree(const Catalog *catalog, TreeKind kind, int node_id)
{
    int index;

    if (!CHECK(catalog != NULL) || !CHECK(catalog->tree_count <= CATALOG_TREE_CAPACITY)) {
        return CATALOG_MISSING;
    }
    for (index = 0; (index < catalog->tree_count) && (index < CATALOG_TREE_CAPACITY); ++index) {
        if ((catalog->tree[index].kind == kind) && (catalog->tree[index].id == node_id)) {
            return index;
        }
    }
    return CATALOG_MISSING;
}

int catalog_find_base(const Catalog *catalog, int base_id)
{
    int index;

    if (!CHECK(catalog != NULL) || !CHECK(catalog->base_count <= CATALOG_BASE_CAPACITY)) {
        return CATALOG_MISSING;
    }
    for (index = 0; (index < catalog->base_count) && (index < CATALOG_BASE_CAPACITY); ++index) {
        if (catalog->bases[index].id == base_id) {
            return index;
        }
    }
    return CATALOG_MISSING;
}
