#ifndef FORGE_H
#define FORGE_H

#include <stdbool.h>
#include <stddef.h>
#include <wchar.h>

#include "../core/file_io.h"
#include "../mod/mod_strings.h"
#include "recipe.h"

enum { FORGE_PROBLEM_CAPACITY = 256, FORGE_PREFIX_CAPACITY = 128, FORGE_CIV_LIMIT = MOD_CIV_LIMIT };

typedef struct {
    const CivDesign *designs[FORGE_CIV_LIMIT];
    int design_count;
    const char *mod_name;
    const Catalog *catalog;
    const wchar_t *game_folder;
    const wchar_t *mods_folder;
} ForgeJob;

typedef struct {
    RecipeReport recipes[FORGE_CIV_LIMIT];
    char problem[FORGE_PROBLEM_CAPACITY];
    wchar_t data_mod[FILE_PATH_CAPACITY];
    wchar_t ui_mod[FILE_PATH_CAPACITY];
} ForgeResult;

bool forge_prefix(const char *alias, char *prefix, size_t capacity);
bool forge_civs_are_finished(const ForgeJob *job, ForgeResult *result);
bool forge_build(const ForgeJob *job, ForgeResult *result);

#endif
