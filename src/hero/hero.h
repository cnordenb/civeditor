#ifndef HERO_H
#define HERO_H

#include <stdbool.h>

#include "../dat/dat_edit.h"

enum { HERO_UNNAMED = -1 };

typedef struct {
    int id;
    long name_sid;
    DatUnit unit;
} Hero;

bool hero_find(const DatTarget *target, int unit_id, Hero *hero);
DatChange hero_hidden(const Hero *hero);
bool hero_build(const DatTarget *target, const Hero *hero);

#endif
