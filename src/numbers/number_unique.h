#ifndef NUMBER_UNIQUE_H
#define NUMBER_UNIQUE_H

#include "number_edit.h"

TechTier number_panel_tier(NumberPanel panel);
int number_unique_rows(const Catalog *catalog, const CivDesign *design, NumberPanel panel);
bool number_unique_row(NumberPlace place, BonusShown *shown);
bool number_unique_name(NumberPlace place, char *text, size_t capacity);
bool number_unique_range(NumberPlace place, const char *before, char *text, size_t capacity);
Command number_unique_command(NumberPlace place, double value);

#endif
