#include "civ_roster.h"

#include <stddef.h>

#include "../core/check.h"

static int slot_of(const CivDesign *design)
{
    int slot = design->choices[CHOICE_REPLACE];

    return (CHECK(slot >= 0) && CHECK(slot < ROSTER_SLOT_LIMIT)) ? slot : -1;
}

void roster_start(CivRoster *roster)
{
    int slot;

    for (slot = 0; slot < ROSTER_SLOT_LIMIT; ++slot) {
        roster->drafted[slot] = false;
        roster->in_mod[slot] = false;
    }
}

void roster_keep(CivRoster *roster, const CivDesign *design)
{
    int slot = slot_of(design);

    if (slot < 0) {
        return;
    }
    roster->drafts[slot] = *design;
    roster->drafted[slot] = true;
}

bool roster_draft(const CivRoster *roster, int slot, CivDesign *design)
{
    if (!roster->drafted[slot]) {
        return false;
    }
    *design = roster->drafts[slot];
    return true;
}

RosterSaving roster_save(CivRoster *roster, const CivDesign *design, const CivDesign *game)
{
    int slot = slot_of(design);
    bool was_in;

    if (slot < 0) {
        return ROSTER_LEFT_OUT;
    }
    was_in = roster->in_mod[slot];
    roster->in_mod[slot] = !civ_design_same(design, game);
    if (!roster->in_mod[slot]) {
        return was_in ? ROSTER_TAKEN_OUT : ROSTER_LEFT_OUT;
    }
    roster->saved[slot] = *design;
    return was_in ? ROSTER_UPDATED : ROSTER_ADDED;
}

bool roster_holds(const CivRoster *roster, const CivDesign *design)
{
    int slot = slot_of(design);

    return (slot >= 0) && roster->in_mod[slot] && civ_design_same(design, &roster->saved[slot]);
}

int roster_count(const CivRoster *roster)
{
    int count = 0;
    int slot;

    for (slot = 0; slot < ROSTER_SLOT_LIMIT; ++slot) {
        count += roster->in_mod[slot] ? 1 : 0;
    }
    return count;
}

const CivDesign *roster_civ(const CivRoster *roster, int index)
{
    int found = 0;
    int slot;

    for (slot = 0; slot < ROSTER_SLOT_LIMIT; ++slot) {
        if (roster->in_mod[slot] && (found == index)) {
            return &roster->saved[slot];
        }
        found += roster->in_mod[slot] ? 1 : 0;
    }
    return NULL;
}
