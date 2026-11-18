#ifndef MOD_STRINGS_H
#define MOD_STRINGS_H

#include "mod_text.h"

enum {
    MOD_STRINGS_CAPACITY = 2097152,
    MOD_CIV_LIMIT = 64,
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
    const ModCiv *civs;
    int civ_count;
    const wchar_t *game;
    const wchar_t *mods;
    const char *prefix;
    const char *title;
} ModUi;

bool mod_strings_write(ModStrings *strings, const ModCiv *civs, int count, const char *vanilla);
bool mod_write_info(const wchar_t *mods, const char *folder, const char *title);
bool mod_ui_write(const ModUi *ui);

#endif
