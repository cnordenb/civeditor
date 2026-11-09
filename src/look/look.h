#ifndef LOOK_H
#define LOOK_H

#include <stdbool.h>

#include "../dat/dat_edit.h"
#include "../data/catalog.h"

enum { LOOK_NONE = -1 };

typedef struct {
    int civ;
    int units[CATALOG_LOOK_UNITS];
} LookMonk;

typedef struct {
    int architecture;
    int castle;
    int wonder;
    LookMonk monk;
} LookSources;

bool look_sources_of(const Catalog *catalog, const CivDesign *design, LookSources *sources);
bool look_build(const DatTarget *target, const LookSources *sources);

#endif
