#ifndef GAME_TEXT_H
#define GAME_TEXT_H

#include <stdbool.h>
#include <wchar.h>

#include "../data/catalog.h"

bool game_text_read(Catalog *catalog, const wchar_t *game_folder);

#endif
