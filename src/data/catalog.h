#ifndef CATALOG_H
#define CATALOG_H

#include <stdbool.h>
#include <stdint.h>
#include <wchar.h>

#include "../model/civ_design.h"
#include "tsv.h"

enum {
    CATALOG_NAME_CAPACITY = 64,
    CATALOG_TEXT_CAPACITY = 320,
    CATALOG_APPEARANCE_CAPACITY = 256,
    CATALOG_BONUS_CAPACITY = 640,
    CATALOG_TEAM_BONUS_CAPACITY = 192,
    CATALOG_UNIT_CAPACITY = 400,
    CATALOG_TECH_CAPACITY = 320,
    CATALOG_TREE_CAPACITY = 640,
    CATALOG_BASE_CAPACITY = 64,
    CATALOG_GENERIC_BASE = 0,
    CATALOG_BONUS_CHANGE_CAPACITY = 4096,
    CATALOG_UNIT_PATCH_CAPACITY = 512,
    CATALOG_LOOK_UNITS = 3,
    CATALOG_MISSING = -1
};

typedef enum {
    APPEARANCE_CIV,
    APPEARANCE_ARCHITECTURE,
    APPEARANCE_SCOUT,
    APPEARANCE_MONK,
    APPEARANCE_BASE,
    APPEARANCE_KIND_COUNT
} AppearanceKind;

typedef enum {
    CIV_STRING_CASTLE_TECH = TECH_TIER_CASTLE,
    CIV_STRING_IMPERIAL_TECH = TECH_TIER_IMPERIAL,
    CIV_STRING_UNIT,
    CIV_STRING_ELITE_UNIT,
    CIV_STRING_HERO,
    CIV_STRING_COUNT
} CivString;

typedef enum {
    BONUS_NAVAL,
    BONUS_MONK,
    BONUS_COST,
    BONUS_ECONOMY,
    BONUS_SPEED,
    BONUS_COMBAT,
    BONUS_RESEARCH,
    BONUS_BUILDING,
    BONUS_UNLOCK,
    BONUS_GENERAL,
    BONUS_CATEGORY_COUNT
} BonusCategory;

typedef enum {
    UNIT_ROLE_VANILLA,
    UNIT_ROLE_CUSTOM,
    UNIT_ROLE_HERO
} UnitRole;

typedef enum {
    UNIT_STAT_FOOD,
    UNIT_STAT_WOOD,
    UNIT_STAT_STONE,
    UNIT_STAT_GOLD,
    UNIT_STAT_HP,
    UNIT_STAT_ATTACK,
    UNIT_STAT_MELEE_ARMOR,
    UNIT_STAT_PIERCE_ARMOR,
    UNIT_STAT_RANGE,
    UNIT_STAT_SPEED,
    UNIT_STAT_RELOAD,
    UNIT_STAT_COUNT
} UnitStat;

typedef struct {
    AppearanceKind kind;
    int id;
    char name[CATALOG_NAME_CAPACITY];
    int string_ids[CIV_STRING_COUNT];
    int look_civ;
    int look_units[CATALOG_LOOK_UNITS];
} AppearanceEntry;

typedef struct {
    int id;
    BonusCategory category;
    char headline[CATALOG_NAME_CAPACITY];
    char description[CATALOG_TEXT_CAPACITY];
    TsvNumberList effects;
} BonusEntry;

typedef struct {
    UnitRole role;
    int id;
    char name[CATALOG_NAME_CAPACITY];
    int stats[UNIT_STAT_COUNT];
    int make_tech;
    int elite_tech;
} UnitEntry;

typedef struct {
    TechTier tier;
    int id;
    char name[CATALOG_NAME_CAPACITY];
    char description[CATALOG_TEXT_CAPACITY];
    int cost[RESOURCE_KIND_COUNT];
    int time;
    TsvNumberList effects;
} TechEntry;

typedef struct {
    TreeKind kind;
    int id;
    char name[CATALOG_NAME_CAPACITY];
    int age;
    int building;
    int parent;
    int lane;
    bool universal;
    bool needs_research;
    uint64_t civs;
} TreeEntry;

typedef struct {
    bool team;
    int bonus_id;
    int entry;
    TsvNumberList requires;
    int type;
    int a;
    int b;
    int c;
    float d;
} BonusChange;

typedef enum {
    UNIT_FIELD_U8,
    UNIT_FIELD_I16,
    UNIT_FIELD_F32,
    UNIT_FIELD_TYPE_COUNT
} UnitFieldType;

typedef struct {
    int bonus_id;
    int unit;
    int offset;
    UnitFieldType type;
    float value;
} UnitPatch;

typedef enum {
    BASE_TEAM_BONUS,
    BASE_UNIT,
    BASE_CASTLE_TECH,
    BASE_IMPERIAL_TECH,
    BASE_ARCHITECTURE,
    BASE_MONK,
    BASE_SCOUT,
    BASE_VALUE_COUNT
} BaseValue;

typedef struct {
    int id;
    TsvNumberList bonuses;
    int values[BASE_VALUE_COUNT];
} BaseCiv;

typedef struct {
    AppearanceEntry appearance[CATALOG_APPEARANCE_CAPACITY];
    int appearance_count;
    BonusEntry bonuses[CATALOG_BONUS_CAPACITY];
    int bonus_count;
    BonusEntry team_bonuses[CATALOG_TEAM_BONUS_CAPACITY];
    int team_bonus_count;
    UnitEntry units[CATALOG_UNIT_CAPACITY];
    int unit_count;
    TechEntry techs[CATALOG_TECH_CAPACITY];
    int tech_count;
    TreeEntry tree[CATALOG_TREE_CAPACITY];
    int tree_count;
    BonusChange bonus_changes[CATALOG_BONUS_CHANGE_CAPACITY];
    int bonus_change_count;
    UnitPatch unit_patches[CATALOG_UNIT_PATCH_CAPACITY];
    int unit_patch_count;
    BaseCiv bases[CATALOG_BASE_CAPACITY];
    int base_count;
} Catalog;

bool catalog_load(Catalog *catalog, const wchar_t *folder);
const char *catalog_category_name(BonusCategory category);
const char *catalog_appearance_name(const Catalog *catalog, AppearanceKind kind, int id);
int catalog_string_id(const Catalog *catalog, int slot, CivString kind);
int catalog_find_look(const Catalog *catalog, AppearanceKind kind, int id);
int catalog_find_bonus(const Catalog *catalog, bool team, int bonus_id);
int catalog_find_unit(const Catalog *catalog, UnitRole role, int unit_id);
int catalog_find_tech(const Catalog *catalog, TechTier tier, int tech_id);
int catalog_find_tree(const Catalog *catalog, TreeKind kind, int node_id);
int catalog_find_base(const Catalog *catalog, int base_id);

#endif
