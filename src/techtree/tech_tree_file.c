#include "tech_tree.h"

#include "../civlist/civ_list.h"
#include "../core/text.h"
#include "../mod/mod_places.h"

static JsonDocument game_tree;
static JsonPatchOutput patched;

bool tech_tree_file_name(const JsonDocument *civilizations, int slot, char *name)
{
    int civ;

    civ = civ_list_entry(civilizations, slot);
    return (civ != JSON_NONE) &&
           json_copy_string(civilizations, json_member(civilizations, civ, "tech_tree_name"), name,
                            FILE_PATH_CAPACITY) &&
           text_append(name, FILE_PATH_CAPACITY, ".json");
}

bool tech_tree_patch_file(const TechTreeJob *job, const wchar_t *game_file, const wchar_t *mod_file)
{
    TechTreeJob game_job;
    FileRead read;

    read = file_read_all(game_file, game_tree.text, sizeof game_tree.text);
    game_tree.length = (int)read.length;
    game_job = *job;
    game_job.tree = &game_tree;
    return read.succeeded && json_parse(&game_tree) && tech_tree_write(&game_job, &patched) &&
           file_write_all(mod_file, patched.text, patched.length);
}

static bool read_civ(Catalog *catalog, const JsonDocument *civilizations, const wchar_t *game_folder, int slot)
{
    char name[FILE_PATH_CAPACITY];
    char relative[FILE_PATH_CAPACITY];
    wchar_t path[FILE_PATH_CAPACITY];
    FileRead read;

    if (!tech_tree_file_name(civilizations, slot, name) ||
        !mod_join(relative, "resources\\_common\\dat\\CivTechTrees\\", name) ||
        !mod_game_path(game_folder, relative, path)) {
        return false;
    }
    read = file_read_all(path, game_tree.text, sizeof game_tree.text);
    game_tree.length = (int)read.length;
    if (!read.succeeded || !json_parse(&game_tree)) {
        return false;
    }
    tech_tree_mark(&game_tree, catalog, slot);
    return true;
}

bool tech_tree_read_game(Catalog *catalog, const JsonDocument *civilizations, const wchar_t *game_folder)
{
    bool any = false;
    int entry;

    for (entry = 0; (entry < catalog->appearance_count) && (entry < CATALOG_APPEARANCE_CAPACITY); ++entry) {
        if (catalog->appearance[entry].kind == APPEARANCE_CIV) {
            any = read_civ(catalog, civilizations, game_folder, catalog->appearance[entry].id) || any;
        }
    }
    return any;
}
