#ifndef UNIQUE_AIM_H
#define UNIQUE_AIM_H

#include <stdbool.h>

#include "../dat/dat_map.h"
#include "../data/catalog.h"

typedef struct {
    int base;
    int elite;
} UniqueUnit;

typedef struct {
    UniqueUnit from;
    UniqueUnit to;
} UniqueSwap;

typedef struct {
    const DatBytes *dat;
    const DatMap *map;
    const Catalog *catalog;
} UniqueSource;

typedef enum {
    UNIQUE_KEEP,
    UNIQUE_DROP,
    UNIQUE_SWAP
} UniqueAim;

UniqueUnit unique_unit_of_techs(const UniqueSource *source, int make_tech, int elite_tech);
UniqueUnit unique_unit_of_civ(const UniqueSource *source, int civ);
UniqueAim unique_aim(const UniqueSwap *swap, DatChange *change);
bool unique_same_target(const DatChange *left, const DatChange *right);

#endif
