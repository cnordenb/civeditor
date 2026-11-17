#ifndef CIV_DESIGN_H
#define CIV_DESIGN_H

#include <stdbool.h>

enum {
    CIV_NAME_CAPACITY = 128,
    CIV_LINE_CAPACITY = 256,
    CIV_DESCRIPTION_CAPACITY = 1536,
    CIV_BONUS_CAPACITY = 24,
    CIV_TEAM_BONUS_CAPACITY = 1,
    CIV_MULTIPLIER_LIMIT = 10,
    CIV_TREE_CAPACITY = 400,
    CIV_NO_CHOICE = -1,
    CIV_DEFAULT_ARCHITECTURE = 2,
    CIV_DEFAULT_REPLACE = 1,
    CIV_DEFAULT_LANGUAGE = 1,
    CIV_GENERIC_BASE = 0,
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
    CHOICE_BASE,
    CHOICE_COUNT
} ChoiceField;

typedef enum {
    TREE_UNITS,
    TREE_BUILDINGS,
    TREE_TECHS,
    TREE_KIND_COUNT
} TreeKind;

typedef enum {
    UNIT_ORIGIN_NONE,
    UNIT_ORIGIN_VANILLA,
    UNIT_ORIGIN_CUSTOM
} UnitOrigin;

typedef enum {
    TECH_TIER_CASTLE,
    TECH_TIER_IMPERIAL,
    TECH_TIER_COUNT
} TechTier;

typedef struct {
    int id;
    int multiplier;
} BonusPick;

typedef struct {
    int ids[CIV_TREE_CAPACITY];
    int count;
} IdSet;

typedef struct {
    UnitOrigin origin;
    int id;
} UnitPick;

typedef struct {
    int km_index;
    int cost[RESOURCE_KIND_COUNT];
    int time;
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
bool civ_design_toggle_team_bonus(CivDesign *design, int bonus_id);
bool civ_design_has_team_bonus(const CivDesign *design, int bonus_id);
void civ_design_set_multiplier(CivDesign *design, bool team, int bonus_id, int multiplier);
bool civ_design_toggle_tree(CivDesign *design, TreeKind kind, int node_id);
bool civ_design_tree_has(const CivDesign *design, TreeKind kind, int node_id);
bool civ_design_tree_is_empty(const CivDesign *design);
bool civ_design_tree_keeps(const CivDesign *design, TreeKind kind, int node_id);
bool civ_design_pick_unit(CivDesign *design, UnitOrigin origin, int unit_id);
bool civ_design_pick_tech(CivDesign *design, TechTier tier, int km_index);
bool civ_design_pick_hero(CivDesign *design, int unit_id);
bool civ_design_is_complete(const CivDesign *design);
bool civ_design_hero_lacks_name(const CivDesign *design);

#endif
