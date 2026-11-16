#ifndef SMALL_DAT_H
#define SMALL_DAT_H

#include <stdbool.h>
#include <stddef.h>

#include "../../src/dat/dat_edit.h"

enum {
    SMALL_DAT_CAPACITY = 98304,
    SMALL_NO_CIVS = 0,
    SMALL_CIV_LIMIT = 4,
    SMALL_CREATABLE_TYPE = 70,
    SMALL_BUILDING_TYPE = 80,
    SMALL_BUILDING_CLASS = 3,
    SMALL_FIRST_GRAPHIC = 100
};

typedef struct {
    int index;
    int unit_class;
    int type;
    int tasks;
} SmallUnit;

typedef struct {
    unsigned char data[SMALL_DAT_CAPACITY];
    size_t size;
    DatBytes bytes;
    DatMap map;
    unsigned char written[SMALL_DAT_CAPACITY];
    DatMap written_map;
    DatEdit edit;
    SmallUnit unit;
    bool rich;
    int resources;
} SmallDat;

extern SmallDat small;

bool small_dat_start(int civ_count);
bool small_dat_write(DatBytes *result);

#endif
