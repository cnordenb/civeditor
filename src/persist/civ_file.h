#ifndef CIV_FILE_H
#define CIV_FILE_H

#include <stdbool.h>
#include <stddef.h>
#include <wchar.h>

#include "../json/json_reader.h"
#include "../json/json_writer.h"
#include "../model/civ_design.h"

enum { CIV_FILE_SCHEMA_VERSION = 2, CIV_FILE_DRAFT_VERSION = 4, CIV_FILE_KEY_WIDTH = 16 };

extern const char CIV_FILE_TREE_KEYS[TREE_KIND_COUNT][CIV_FILE_KEY_WIDTH];
extern const char CIV_FILE_RESOURCE_KEYS[RESOURCE_KIND_COUNT][CIV_FILE_KEY_WIDTH];

bool civ_file_stat_key(UnitRank rank, UnitStat stat, char *key, size_t capacity);
bool civ_file_compose(const CivDesign *design, JsonWriter *writer);
bool civ_file_interpret(const JsonDocument *document, CivDesign *design);
bool civ_file_save(const CivDesign *design, const wchar_t *path);
bool civ_file_load(CivDesign *design, const wchar_t *path);

#endif
