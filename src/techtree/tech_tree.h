#ifndef TECH_TREE_H
#define TECH_TREE_H

#include <stdbool.h>

#include "../core/file_io.h"
#include "../data/catalog.h"
#include "../json/json_patch.h"

typedef struct {
    const JsonDocument *tree;
    const CivDesign *design;
    const Catalog *catalog;
} TechTreeJob;

bool tech_tree_write(const TechTreeJob *job, JsonPatchOutput *output);
bool tech_tree_file_name(const JsonDocument *civilizations, int slot, char *name);
bool tech_tree_patch_file(const TechTreeJob *job, const wchar_t *game_file, const wchar_t *mod_file);
void tech_tree_mark(const JsonDocument *tree, Catalog *catalog, int slot);
bool tech_tree_read_game(Catalog *catalog, const JsonDocument *civilizations, const wchar_t *game_folder);

#endif
