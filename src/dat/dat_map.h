#ifndef DAT_MAP_H
#define DAT_MAP_H

#include <stdbool.h>
#include <stddef.h>

enum {
    DAT_EFFECT_CAPACITY = 4096,
    DAT_CIV_CAPACITY = 128,
    DAT_TECH_CAPACITY = 4096,
    DAT_NAME_CAPACITY = 256,
    DAT_CHANGE_SIZE = 11,
    DAT_RESEARCH_LOCATION_SIZE = 9,
    DAT_REQUIRED_TECH_SLOTS = 6,
    DAT_TECH_COST_SLOTS = 3,
    DAT_RESEARCH_LOCATION_CAPACITY = 16,
    DAT_UNIT_NAME_AT = 3,
    DAT_UNIT_CREATION_AT = 7,
    DAT_UNIT_CLASS_AT = 11,
    DAT_UNIT_GRAPHICS_AT = 13,
    DAT_UNIT_GRAPHICS_SIZE = 8,
    DAT_UNIT_HIT_POINTS_AT = 22,
    DAT_UNIT_DEAD_UNIT_AT = 45,
    DAT_UNIT_ICON_AT = 51,
    DAT_UNIT_DISABLED_AT = 57,
    DAT_UNIT_HELP_AT = 95,
    DAT_UNIT_FIXED_SIZE = 157,
    DAT_UNIT_CAPACITY = 4096,
    DAT_UNIT_COST_SLOTS = 3,
    DAT_UNIT_COST_SIZE = 6,
    DAT_TRAIN_LOCATION_SIZE = 9,
    DAT_TRAIN_BUILDING_AT = 2,
    DAT_TRAIN_BUTTON_AT = 4,
    DAT_TRAIN_HOT_KEY_AT = 5,
    DAT_DAMAGE_GRAPHIC_SIZE = 5,
    DAT_GRAPHIC_SIZE = 2,
    DAT_GARRISON_GRAPHIC_SIZE = 4,
    DAT_TASK_SIZE = 69,
    DAT_TASK_PROCEEDING_GRAPHIC_AT = 49,
    DAT_TASK_CARRYING_GRAPHIC_AT = 53,
    DAT_BUILDING_SIZE = 98
};

typedef struct {
    const unsigned char *data;
    size_t size;
} DatBytes;

typedef struct {
    size_t effect_count_at;
    int effect_count;
    size_t effect_at[DAT_EFFECT_CAPACITY];
    size_t unit_headers_at;
    size_t civ_count_at;
    int civ_count;
    size_t civ_at[DAT_CIV_CAPACITY];
    size_t tech_count_at;
    int tech_count;
    size_t tech_at[DAT_TECH_CAPACITY];
    size_t tail_at;
} DatMap;

typedef struct {
    char name[DAT_NAME_CAPACITY];
    size_t tech_tree_id_at;
    int tech_tree_id;
    size_t team_bonus_id_at;
    int team_bonus_id;
    size_t icon_set_at;
} DatCiv;

typedef struct {
    size_t at;
    int unit_class;
    int dead_unit;
    size_t damage_graphics_at;
    int damage_graphic_count;
    size_t copy_ids_at;
    size_t walking_graphic_at;
    size_t tasks_at;
    int task_count;
    size_t attack_graphic_at;
    size_t costs_at;
    size_t train_locations_at;
    int train_location_count;
    size_t hero_mode_at;
    size_t garrison_graphic_at;
    size_t building_at;
} DatUnit;

typedef struct {
    size_t at[DAT_UNIT_CAPACITY];
    int count;
} DatUnitList;

typedef struct {
    char name[DAT_NAME_CAPACITY];
    int change_count;
    size_t changes_at;
} DatEffect;

enum {
    DAT_CHANGE_RESOURCE = 1,
    DAT_CHANGE_ADD = 4,
    DAT_CHANGE_MULTIPLY = 5,
    DAT_CHANGE_MULTIPLY_RESOURCE = 6,
    DAT_CHANGE_SPAWN = 7,
    DAT_CHANGE_TECH_COST = 101,
    DAT_CHANGE_TECH_TIME = 103
};

typedef struct {
    int type;
    int a;
    int b;
    int c;
    float d;
} DatChange;

typedef struct {
    int type;
    int amount;
    int flag;
} DatTechCost;

typedef struct {
    int location_id;
    int research_time;
    int button_id;
    long hot_key_id;
} DatResearchLocation;

typedef struct {
    int required_techs[DAT_REQUIRED_TECH_SLOTS];
    DatTechCost costs[DAT_TECH_COST_SLOTS];
    int required_tech_count;
    size_t civ_at;
    int civ;
    int full_tech_mode;
    long language_dll_name;
    long language_dll_description;
    int effect_id;
    int type;
    int icon_id;
    long language_dll_help;
    long language_dll_tech_tree;
    char name[DAT_NAME_CAPACITY];
    int repeatable;
    int research_location_count;
    size_t research_locations_at;
    DatResearchLocation research_locations[DAT_RESEARCH_LOCATION_CAPACITY];
} DatTech;

bool dat_map_read(const unsigned char *data, size_t size, DatMap *map);
bool dat_civ_read(const DatBytes *dat, size_t at, DatCiv *civ);
bool dat_find_unit(const DatBytes *dat, size_t civ_at, int unit_id, size_t *unit_at);
bool dat_list_units(const DatBytes *dat, size_t civ_at, DatUnitList *list);
bool dat_unit_read(const DatBytes *dat, size_t at, DatUnit *unit);
bool dat_effect_read(const DatBytes *dat, size_t at, DatEffect *effect);
bool dat_change_read(const DatBytes *dat, size_t at, DatChange *change);
bool dat_tech_read(const DatBytes *dat, size_t at, DatTech *tech);

#endif
