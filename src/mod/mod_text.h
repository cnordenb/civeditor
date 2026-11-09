#ifndef MOD_TEXT_H
#define MOD_TEXT_H

#include <stdbool.h>
#include <stddef.h>

#include "../data/catalog.h"

enum { MOD_TEXT_CAPACITY = 8192, MOD_TIP_COUNT = 4, MOD_OWN_DESCRIPTION_BASE = 990100, MOD_OWN_DESCRIPTION_ROOM = 100 };

typedef struct {
    const CivDesign *design;
    const Catalog *catalog;
    int slot;
    long name_sid;
    bool hero_skipped;
} ModCiv;

const char *mod_unit_name(const ModCiv *civ);
bool mod_unit_has_own_texts(const CivDesign *design);
const char *mod_tech_text(const ModCiv *civ, TechTier tier, bool want_name);
bool mod_description(const ModCiv *civ, char *text, size_t capacity);
bool mod_tip(const ModCiv *civ, int index, char *text, size_t capacity);
long mod_description_sid(int slot);

#endif
