#ifndef MOD_STRINGS_H
#define MOD_STRINGS_H

#include "mod_text.h"

enum {
    MOD_STRINGS_CAPACITY = 262144,
    MOD_LABEL_OFFSET = 1000,
    MOD_TOOLTIP_OFFSET = 21000,
    MOD_HELP_OFFSET = 100000,
    MOD_TECH_TREE_OFFSET = 150000
};

typedef struct {
    char text[MOD_STRINGS_CAPACITY];
    size_t length;
    bool failed;
} ModStrings;

typedef struct {
    const ModCiv *civ;
    const wchar_t *game;
    const wchar_t *mods;
    const char *prefix;
} ModUi;

bool mod_strings_write(ModStrings *strings, const ModCiv *civ, const char *vanilla);
bool mod_write_info(const wchar_t *mods, const char *folder, const char *title);
bool mod_ui_write(const ModUi *ui);

#endif
