#ifndef SUMMARY_H
#define SUMMARY_H

#include <stdbool.h>

#include "../data/catalog.h"
#include "../forge/recipe.h"
#include "../model/civ_design.h"

enum { SUMMARY_CAPACITY = 16384, SUMMARY_CIV_LIMIT = 64 };

typedef struct {
    char text[SUMMARY_CAPACITY];
} SummaryText;

typedef struct {
    const CivDesign *designs[SUMMARY_CIV_LIMIT];
    const RecipeReport *reports;
    int count;
} BuiltCivs;

void summary_add(SummaryText *summary, const char *text);
void summary_add_tree(SummaryText *summary, const CivDesign *design, const char *before_techs,
                      const char *after_techs);
bool summary_write(SummaryText *summary, const CivDesign *design, const Catalog *catalog);
bool summary_write_build(SummaryText *summary, const char *prefix, const BuiltCivs *built);

#endif
