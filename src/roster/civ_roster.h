#ifndef CIV_ROSTER_H
#define CIV_ROSTER_H

#include <stdbool.h>

#include "../model/civ_design.h"

enum { ROSTER_SLOT_LIMIT = 64 };

typedef enum {
    ROSTER_ADDED,
    ROSTER_UPDATED,
    ROSTER_TAKEN_OUT,
    ROSTER_LEFT_OUT
} RosterSaving;

typedef struct {
    CivDesign drafts[ROSTER_SLOT_LIMIT];
    bool drafted[ROSTER_SLOT_LIMIT];
    CivDesign saved[ROSTER_SLOT_LIMIT];
    bool in_mod[ROSTER_SLOT_LIMIT];
} CivRoster;

void roster_start(CivRoster *roster);
void roster_keep(CivRoster *roster, const CivDesign *design);
bool roster_draft(const CivRoster *roster, int slot, CivDesign *design);
RosterSaving roster_save(CivRoster *roster, const CivDesign *design, const CivDesign *game);
bool roster_holds(const CivRoster *roster, const CivDesign *design);
int roster_count(const CivRoster *roster);
const CivDesign *roster_civ(const CivRoster *roster, int index);

#endif
