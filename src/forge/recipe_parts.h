#ifndef RECIPE_PARTS_H
#define RECIPE_PARTS_H

#include "recipe.h"
#include "../unique/unique_aim.h"

enum {
    RECIPE_CHANGE_LIMIT = 32768,
    RECIPE_ENABLE_UNIT = 2,
    RECIPE_UPGRADE_UNIT = 3,
    RECIPE_UNLOCK_TECH = 8,
    RECIPE_TECH_COST = 101,
    RECIPE_DISABLE_TECH = 102,
    RECIPE_TECH_TIME = 103,
    RECIPE_FEUDAL_AGE = 101,
    RECIPE_CASTLE_AGE = 102,
    RECIPE_IMPERIAL_AGE = 103
};

typedef struct {
    DatChange change;
    bool read;
} RecipeChange;

typedef struct {
    DatChange changes[RECIPE_TREE_CAPACITY];
    int count;
    bool overflowed;
} RecipeChanges;

typedef struct {
    int effect_id;
    UniqueSwap swap;
} RecipeUniqueCopy;

bool recipe_read_tech(const RecipeInput *input, int id, DatTech *tech);
bool recipe_read_effect(const RecipeInput *input, int id, DatEffect *effect);
RecipeChange recipe_change_at(const RecipeInput *input, const DatEffect *effect, int index);
bool recipe_find_unlock(const RecipeInput *input, int tech_id, DatChange *unlock);
void recipe_add_change(RecipeChanges *changes, const DatChange *change);
void recipe_sort_changes(RecipeChanges *changes);

bool recipe_neutralize(const RecipeInput *input, DatEdit *edit);
void recipe_plan_reset(void);
bool recipe_plan_tech(const RecipeInput *input, int tech_id);
bool recipe_commit_plan(const RecipeInput *input, DatEdit *edit);
int recipe_copy_effect(const RecipeInput *input, DatEdit *edit, int effect_id);
int recipe_copy_unique_effect(const RecipeInput *input, DatEdit *edit, const RecipeUniqueCopy *copy);
UniqueSwap recipe_unique_swap(const RecipeInput *input, int source_civ);
bool recipe_name_unique_unit(const RecipeInput *input, DatEdit *edit);

bool recipe_wire_tree(const RecipeInput *input, RecipeChanges *changes, RecipeReport *report);

#endif
