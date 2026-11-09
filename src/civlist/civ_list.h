#ifndef CIV_LIST_H
#define CIV_LIST_H

#include <stdbool.h>
#include <stddef.h>

#include "../json/json_patch.h"

enum { CIV_LIST_KEEP = -1 };

typedef struct {
    int slot;
    int unit;
    int elite_unit;
    int elite_tech;
    int icon;
    bool unit_named;
    long unit_name_sid;
    long unit_description_sid;
    int castle_tech;
    int imperial_tech;
    long description_sid;
} CivListEntry;

typedef JsonPatchOutput CivListOutput;

int civ_list_entry(const JsonDocument *list, int slot);
bool civ_list_write(const JsonDocument *list, const CivListEntry *entry, CivListOutput *output);

#endif
