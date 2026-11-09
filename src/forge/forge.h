#ifndef FORGE_H
#define FORGE_H

#include <stdbool.h>
#include <stddef.h>
#include <wchar.h>

#include "../core/file_io.h"
#include "recipe.h"

enum { FORGE_PROBLEM_CAPACITY = 256, FORGE_PREFIX_CAPACITY = 128 };

typedef struct {
    const CivDesign *design;
    const Catalog *catalog;
    const wchar_t *game_folder;
    const wchar_t *mods_folder;
} ForgeJob;

typedef struct {
    RecipeReport recipe;
    char problem[FORGE_PROBLEM_CAPACITY];
    wchar_t data_mod[FILE_PATH_CAPACITY];
    wchar_t ui_mod[FILE_PATH_CAPACITY];
} ForgeResult;

bool forge_prefix(const char *alias, char *prefix, size_t capacity);
bool forge_build(const ForgeJob *job, ForgeResult *result);

#endif
