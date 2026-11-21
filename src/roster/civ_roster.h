#ifndef CIV_ROSTER_H
#define CIV_ROSTER_H

#include <stdbool.h>

#include "../model/civ_design.h"

enum { ROSTER_SLOT_LIMIT = 64 };

typedef struct {
    CivDesign drafts[ROSTER_SLOT_LIMIT];
    bool drafted[ROSTER_SLOT_LIMIT];
    bool changed[ROSTER_SLOT_LIMIT];
} CivRoster;

void roster_start(CivRoster *roster);
bool roster_keep(CivRoster *roster, const CivDesign *design, const CivDesign *game);
bool roster_draft(const CivRoster *roster, int slot, CivDesign *design);
bool roster_is_changed(const CivRoster *roster, int slot);
int roster_count(const CivRoster *roster);
const CivDesign *roster_civ(const CivRoster *roster, int index);

#endif
