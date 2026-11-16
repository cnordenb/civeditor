#ifndef MOD_PLACES_H
#define MOD_PLACES_H

#include <stdbool.h>
#include <stddef.h>
#include <wchar.h>

#include "../core/file_io.h"

bool mod_find_game_folder(wchar_t *folder, size_t capacity);
bool mod_find_local_mods(wchar_t *folder, size_t capacity);
bool mod_find_game_under(const wchar_t *steam, wchar_t *folder, size_t capacity);
bool mod_find_mods_under(const wchar_t *home, wchar_t *folder, size_t capacity);
bool mod_game_file(const wchar_t *game, const wchar_t *relative, wchar_t *path);
bool mod_make_folders(const wchar_t *root, const wchar_t *relative);
bool mod_join(char *text, const char *first, const char *second);
bool mod_game_path(const wchar_t *game, const char *relative, wchar_t *path);
bool mod_folder_path(const wchar_t *mods, const char *folder, wchar_t *path);
bool mod_file_path(const wchar_t *mods, const char *folder, const wchar_t *name, wchar_t *path);

#endif
