#ifndef CIV_DESIGN_H
#define CIV_DESIGN_H

#include <stdbool.h>

enum {
    CIV_NAME_CAPACITY = 128,
    CIV_LINE_CAPACITY = 256,
    CIV_DESCRIPTION_CAPACITY = 1536,
    CIV_BONUS_CAPACITY = 24,
    CIV_TEAM_BONUS_CAPACITY = 1,
    CIV_TECH_EFFECT_CAPACITY = 6,
    CIV_MULTIPLIER_LIMIT = 10,
    CIV_MILLIONTHS = 1000000,
    CIV_MILLIONTH_PLACES = 6,
    CIV_TREE_CAPACITY = 400,
    CIV_NO_CHOICE = -1,
    CIV_DEFAULT_ARCHITECTURE = 2,
    CIV_DEFAULT_REPLACE = 1,
    CIV_DEFAULT_LANGUAGE = 1,
    RESOURCE_KIND_COUNT = 4
};

typedef enum {
    TEXT_FIELD_NAME,
    TEXT_FIELD_TAGLINE,
    TEXT_FIELD_DESCRIPTION,
    TEXT_FIELD_UNIT_NAME,
    TEXT_FIELD_UNIT_DESCRIPTION,
    TEXT_FIELD_CASTLE_TECH_NAME,
    TEXT_FIELD_IMPERIAL_TECH_NAME,
    TEXT_FIELD_HERO_NAME,
    TEXT_FIELD_COUNT
} TextField;

typedef enum {
    CHOICE_ARCHITECTURE,
    CHOICE_LANGUAGE,
    CHOICE_WONDER,
    CHOICE_CASTLE,
    CHOICE_MONK,
    CHOICE_SCOUT,
    CHOICE_REPLACE,
    CHOICE_COUNT
} ChoiceField;

typedef enum {
    TREE_UNITS,
    TREE_BUILDINGS,
    TREE_TECHS,
    TREE_KIND_COUNT
} TreeKind;

typedef enum {
    BONUS_LIST_CIV,
    BONUS_LIST_TEAM
} BonusList;

typedef enum {
    UNIT_ORIGIN_NONE,
    UNIT_ORIGIN_VANILLA,
    UNIT_ORIGIN_CUSTOM
} UnitOrigin;

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

typedef enum {
    UNIT_RANK_BASE,
    UNIT_RANK_ELITE,
    UNIT_RANK_COUNT
} UnitRank;

typedef enum {
    TECH_TIER_CASTLE,
    TECH_TIER_IMPERIAL,
    TECH_TIER_COUNT
} TechTier;

typedef enum {
    TECH_VALUE_FOOD,
    TECH_VALUE_WOOD,
    TECH_VALUE_STONE,
    TECH_VALUE_GOLD,
    TECH_VALUE_TIME,
    TECH_VALUE_COUNT
} TechValue;

typedef struct {
    bool set;
    int value;
} Override;

typedef struct {
    int least;
    int most;
} StatRange;

typedef struct {
    int millionths;
} Multiplier;

typedef struct {
    int id;
    Multiplier multiplier;
} BonusPick;

typedef struct {
    int ids[CIV_TREE_CAPACITY];
    int count;
} IdSet;

typedef struct {
    UnitOrigin origin;
    int id;
    Override stats[UNIT_RANK_COUNT][UNIT_STAT_COUNT];
} UnitPick;

typedef struct {
    int km_index;
    Override values[TECH_VALUE_COUNT];
    BonusPick effects[CIV_TECH_EFFECT_CAPACITY];
    int effect_count;
} TechPick;

typedef struct {
    char texts[TEXT_FIELD_COUNT][CIV_DESCRIPTION_CAPACITY];
    int choices[CHOICE_COUNT];
    BonusPick bonuses[CIV_BONUS_CAPACITY];
    int bonus_count;
    BonusPick team_bonuses[CIV_TEAM_BONUS_CAPACITY];
    int team_bonus_count;
    IdSet tree[TREE_KIND_COUNT];
    UnitPick unique_unit;
    TechPick techs[TECH_TIER_COUNT];
    int hero_unit;
} CivDesign;

void civ_design_reset(CivDesign *design);
int civ_text_capacity(TextField field);
bool civ_design_set_text(CivDesign *design, TextField field, const char *text);
const char *civ_design_text(const CivDesign *design, TextField field);
bool civ_design_set_choice(CivDesign *design, ChoiceField field, int value);
bool civ_design_toggle_bonus(CivDesign *design, int bonus_id);
bool civ_design_has_bonus(const CivDesign *design, int bonus_id);
bool civ_design_clear_bonuses(CivDesign *design);
bool civ_design_toggle_team_bonus(CivDesign *design, int bonus_id);
bool civ_design_has_team_bonus(const CivDesign *design, int bonus_id);
Multiplier civ_multiplier_of(double times);
double civ_multiplier_times(Multiplier multiplier);
bool civ_design_set_multiplier(CivDesign *design, bool team, int bonus_id, Multiplier multiplier);
Multiplier civ_design_multiplier(const CivDesign *design, int bonus_id);
bool civ_design_toggle_tree(CivDesign *design, TreeKind kind, int node_id);
bool civ_design_tree_has(const CivDesign *design, TreeKind kind, int node_id);
bool civ_design_tree_is_empty(const CivDesign *design);
bool civ_design_always_keeps(TreeKind kind, int node_id);
bool civ_design_tree_keeps(const CivDesign *design, TreeKind kind, int node_id);
bool civ_design_pick_unit(CivDesign *design, UnitOrigin origin, int unit_id);
bool civ_design_set_unit_stat(CivDesign *design, UnitRank rank, UnitStat stat, int value);
bool civ_stat_fits(UnitStat stat, int value);
unsigned civ_stat_places(UnitStat stat);
StatRange civ_stat_range(UnitStat stat);
StatRange civ_tech_range(TechValue value);
bool civ_fit_decimal(StatRange range, unsigned places, double value, int *stored);
bool civ_design_set_tech_value(CivDesign *design, TechTier tier, TechValue value, int amount);
bool civ_design_toggle_tech_effect(CivDesign *design, TechTier tier, int bonus_id);
bool civ_design_has_tech_effect(const CivDesign *design, TechTier tier, int bonus_id);
bool civ_design_set_tech_effect(CivDesign *design, TechTier tier, int bonus_id, Multiplier multiplier);
int civ_override_or(Override override, int fallback);
bool civ_design_pick_tech(CivDesign *design, TechTier tier, int km_index);
bool civ_design_pick_hero(CivDesign *design, int unit_id);
bool civ_design_is_complete(const CivDesign *design);
bool civ_design_has_hero(const CivDesign *design);
bool civ_design_hero_lacks_name(const CivDesign *design);
bool civ_design_same_identity(const CivDesign *design, const CivDesign *other);
bool civ_design_same_tree(const CivDesign *design, const CivDesign *other);
bool civ_design_same_bonuses(const CivDesign *design, const CivDesign *other, BonusList list);
bool civ_design_same_unit(const CivDesign *design, const CivDesign *other);
bool civ_design_same_tech(const CivDesign *design, const CivDesign *other, TechTier tier);
bool civ_design_same_hero(const CivDesign *design, const CivDesign *other);
bool civ_design_same(const CivDesign *design, const CivDesign *other);

#endif
