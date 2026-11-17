#include "tech_tree.h"

#include "../civlist/civ_list.h"
#include "../core/text.h"

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
