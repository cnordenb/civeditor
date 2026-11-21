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
        roster->changed[slot] = false;
    }
}

bool roster_keep(CivRoster *roster, const CivDesign *design, const CivDesign *game)
{
    int slot = slot_of(design);
    bool was_changed;

    if (slot < 0) {
        return false;
    }
    was_changed = roster->changed[slot];
    roster->drafts[slot] = *design;
    roster->drafted[slot] = true;
    roster->changed[slot] = !civ_design_same(design, game);
    return roster->changed[slot] != was_changed;
}

bool roster_draft(const CivRoster *roster, int slot, CivDesign *design)
{
    if (!roster->drafted[slot]) {
        return false;
    }
    *design = roster->drafts[slot];
    return true;
}

bool roster_is_changed(const CivRoster *roster, int slot)
{
    return (slot >= 0) && (slot < ROSTER_SLOT_LIMIT) && roster->changed[slot];
}

int roster_count(const CivRoster *roster)
{
    int count = 0;
    int slot;

    for (slot = 0; slot < ROSTER_SLOT_LIMIT; ++slot) {
        count += roster->changed[slot] ? 1 : 0;
    }
    return count;
}

const CivDesign *roster_civ(const CivRoster *roster, int index)
{
    int found = 0;
    int slot;

    for (slot = 0; slot < ROSTER_SLOT_LIMIT; ++slot) {
        if (roster->changed[slot] && (found == index)) {
            return &roster->drafts[slot];
        }
        found += roster->changed[slot] ? 1 : 0;
    }
    return NULL;
}
