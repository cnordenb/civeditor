#ifndef RECIPE_H
#define RECIPE_H

#include <stdbool.h>

#include "../dat/dat_edit.h"
#include "../data/catalog.h"

enum { RECIPE_COPY_CAPACITY = 96, RECIPE_TREE_CAPACITY = 185, RECIPE_LOST_CIV = 99 };

typedef struct {
    const CivDesign *design;
    const Catalog *catalog;
    const DatBytes *dat;
    const DatMap *map;
    int slot;
} RecipeInput;

typedef struct {
    int bonuses_built;
    int bonuses_skipped;
    int multipliers_skipped;
    int team_bonuses_built;
    int team_bonuses_skipped;
    int unit_techs_built;
    int unique_techs_built;
    int unique_techs_skipped;
    int tree_disabled;
    int unit_stats_skipped;
    bool unit_skipped;
    bool tree_overflowed;
    bool hero_skipped;
    int unique_tech_ids[TECH_TIER_COUNT];
} RecipeReport;

typedef struct {
    int unit;
    int elite_unit;
    int elite_tech;
} RecipeUnitIds;

bool recipe_apply(const RecipeInput *input, DatEdit *edit, RecipeReport *report);
RecipeUnitIds recipe_unique_unit_ids(const RecipeInput *input);

#endif
