#ifndef MOD_PLACES_H
#define MOD_PLACES_H

#include <stdbool.h>
#include <stddef.h>
#include <wchar.h>

#include "../core/file_io.h"

bool mod_find_game_folder(wchar_t *folder, size_t capacity);
bool mod_find_local_mods(wchar_t *folder, size_t capacity);
bool mod_game_file(const wchar_t *game, const wchar_t *relative, wchar_t *path);
bool mod_make_folders(const wchar_t *root, const wchar_t *relative);

#endif
