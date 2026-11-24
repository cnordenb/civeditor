#ifndef BONUS_BUILD_H
#define BONUS_BUILD_H

#include "../dat/dat_edit.h"
#include "../data/catalog.h"

enum { BONUS_BUILD_FAILED = -1 };

typedef struct {
    const Catalog *catalog;
    DatTarget target;
} BonusBuild;

int bonus_build_civ(const BonusBuild *build, int bonus_id, double times);
int bonus_build_lines(const BonusBuild *build, int bonus_id, bool team, double times);

#endif
