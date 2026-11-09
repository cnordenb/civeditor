#ifndef BASE_CIV_H
#define BASE_CIV_H

#include <stdbool.h>

#include "../data/catalog.h"

bool base_civ_apply(CivDesign *design, const Catalog *catalog, int base_id);

#endif
