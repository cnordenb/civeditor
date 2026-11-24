#ifndef NUMBER_EDIT_H
#define NUMBER_EDIT_H

#include <stdbool.h>
#include <stddef.h>

#include "../bonus/bonus_text.h"
#include "../data/catalog.h"
#include "../model/civ_design.h"
#include "../model/command.h"
#include "number_layout.h"

enum { NUMBER_TEXT_CAPACITY = 128, NUMBER_SLIDER_LEAST = 100, NUMBER_SLIDER_MOST = 1000 };

typedef enum { NUMBER_DIGITS, NUMBER_WITH_UNIT } NumberText;

typedef enum {
    NUMBER_PANEL_BONUSES,
    NUMBER_PANEL_UNIT,
    NUMBER_PANEL_CASTLE_TECH,
    NUMBER_PANEL_IMPERIAL_TECH,
    NUMBER_PANEL_TEAM,
    NUMBER_PANEL_COUNT
} NumberPanel;

typedef struct {
    NumberPanel panel;
    int pick;
    int number;
} NumberFocus;

typedef struct {
    const Catalog *catalog;
    const CivDesign *design;
    NumberFocus focus;
} NumberPlace;

int number_rows(const Catalog *catalog, const CivDesign *design, NumberPanel panel);
bool number_row(NumberPlace place, BonusShown *shown);
bool number_text(NumberPlace place, double times, NumberText form, char *text, size_t capacity);
bool number_name(NumberPlace place, char *text, size_t capacity);
bool number_range_text(NumberPlace place, const char *before, char *text, size_t capacity);
bool number_slides(NumberPlace place);
double number_times(NumberPlace place);
int number_slider_position(NumberPlace place);
double number_slider_times(int position);
Command number_typed_command(NumberPlace place, const char *typed);
Command number_slid_command(NumberPlace place, int position);

#endif
