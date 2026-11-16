#ifndef UNIQUE_AIM_H
#define UNIQUE_AIM_H

#include <stdbool.h>

#include "../dat/dat_map.h"
#include "../data/catalog.h"
#include "../forge/recipe.h"

typedef struct {
    int base;
    int elite;
} UniqueUnit;

typedef struct {
    UniqueUnit from;
    UniqueUnit to;
} UniqueSwap;

typedef enum {
    UNIQUE_KEEP,
    UNIQUE_DROP,
    UNIQUE_SWAP
} UniqueAim;

UniqueUnit unique_unit_of_techs(const RecipeInput *input, int make_tech, int elite_tech);
UniqueUnit unique_unit_of_civ(const RecipeInput *input, int civ);
UniqueAim unique_aim(const UniqueSwap *swap, DatChange *change);
bool unique_same_target(const DatChange *left, const DatChange *right);

#endif
