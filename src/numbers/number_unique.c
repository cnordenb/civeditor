#include "number_unique.h"

#include "../core/check.h"
#include "../core/text.h"

enum { SHEET_LINES = 2, SHEET_ROWS = 2, LABEL_WIDTH = 16, TIME_LABEL = UNIT_STAT_COUNT, LABEL_COUNT };

typedef struct {
    int rows;
    int lines;
    int first[SHEET_ROWS + 1];
} SheetShape;

typedef struct {
    int label;
    StatRange range;
    unsigned places;
    Override override;
    int game;
} SheetNumber;

typedef struct {
    SheetShape shape;
    const char *lines[SHEET_LINES];
    SheetNumber numbers[UNIT_STAT_COUNT];
} Sheet;

static const SheetShape UNIT_SHAPE = { 3, 1, { UNIT_STAT_FOOD, UNIT_STAT_HP, UNIT_STAT_COUNT } };
static const SheetShape TECH_SHAPE = { 4, 2, { TECH_VALUE_FOOD, TECH_VALUE_TIME, TECH_VALUE_COUNT } };
static const SheetShape CUSTOM_SHAPE = { 3, 1, { TECH_VALUE_FOOD, TECH_VALUE_TIME, TECH_VALUE_COUNT } };
static const SheetShape EMPTY_SHAPE = { 1, 1, { 0, 0, 0 } };

static const char ABSENCES[2][2][48] = {
    { "No unit is selected yet.", "The selected unit is not in the catalog." },
    { "No tech is selected yet.", "The selected tech is not in the catalog." }
};

static const char BEFORE[LABEL_COUNT][LABEL_WIDTH] = {
    "Cost: ", ", ", ", ", ", ", "Hit points ", ", attack ", ", melee armor ", ", pierce armor ", ", range ",
    ", speed ", ", reload time ", "Research time: "
};

static const char AFTER[LABEL_COUNT][LABEL_WIDTH] = { " food", " wood", " stone", " gold", "", "", "", "", "", "",
                                                      "", " seconds" };

static const char NAMES[LABEL_COUNT][LABEL_WIDTH] = {
    "Food cost", "Wood cost", "Stone cost", "Gold cost", "Hit points", "Attack", "Melee armor", "Pierce armor",
    "Range", "Speed", "Reload time", "Research time"
};

TechTier number_panel_tier(NumberPanel panel)
{
    int tier = (int)panel - (int)NUMBER_PANEL_CASTLE_TECH;

    return (TechTier)tier;
}

static void leave_empty(Sheet *sheet, bool tech, bool picked)
{
    sheet->shape = EMPTY_SHAPE;
    sheet->lines[0] = ABSENCES[tech][picked];
}

static void fill_unit(const Catalog *catalog, const CivDesign *design, Sheet *sheet)
{
    const UnitPick *pick = &design->unique_unit;
    int found = catalog_find_unit(catalog, pick);
    int stat;

    if (found == CATALOG_MISSING) {
        leave_empty(sheet, false, pick->origin != UNIT_ORIGIN_NONE);
        return;
    }
    sheet->shape = UNIT_SHAPE;
    sheet->lines[0] = catalog->units[found].name;
    for (stat = 0; stat < UNIT_STAT_COUNT; ++stat) {
        SheetNumber number = { stat, civ_stat_range((UnitStat)stat), civ_stat_places((UnitStat)stat),
                               pick->stats[UNIT_RANK_BASE][stat], catalog->units[found].stats[stat] };

        sheet->numbers[stat] = number;
    }
}

static void fill_tech(const Catalog *catalog, const CivDesign *design, TechTier tier, Sheet *sheet)
{
    const TechPick *pick = &design->techs[tier];
    int found = catalog_find(catalog, CATALOG_TECHS, (int)tier, pick->km_index);
    const TechEntry *entry;
    int value;

    if (found == CATALOG_MISSING) {
        leave_empty(sheet, true, pick->km_index != CIV_NO_CHOICE);
        return;
    }
    entry = &catalog->techs[found];
    sheet->shape = (pick->effect_count > 0) ? CUSTOM_SHAPE : TECH_SHAPE;
    sheet->lines[0] = entry->name;
    sheet->lines[1] = entry->description;
    for (value = 0; value < TECH_VALUE_COUNT; ++value) {
        bool time = value == TECH_VALUE_TIME;
        SheetNumber number = { time ? TIME_LABEL : value, civ_tech_range((TechValue)value), 0U, pick->values[value],
                               time ? entry->time : entry->cost[value] };

        sheet->numbers[value] = number;
    }
}

static void fill_sheet(const Catalog *catalog, const CivDesign *design, NumberPanel panel, Sheet *sheet)
{
    if (panel == NUMBER_PANEL_UNIT) {
        fill_unit(catalog, design, sheet);
    } else {
        fill_tech(catalog, design, number_panel_tier(panel), sheet);
    }
}

int number_unique_rows(const Catalog *catalog, const CivDesign *design, NumberPanel panel)
{
    Sheet sheet;

    fill_sheet(catalog, design, panel, &sheet);
    return sheet.shape.rows;
}

static bool add_number(BonusShown *shown, const SheetNumber *number)
{
    int value = civ_override_or(number->override, number->game);
    size_t start = text_length(shown->text, sizeof shown->text);
    bool written = (!number->override.set && (value == TSV_MISSING))
                       ? text_append(shown->text, sizeof shown->text, "-")
                       : text_append_decimal(shown->text, sizeof shown->text, value, number->places);

    shown->numbers[shown->count].kind = BONUS_NUMBER_AMOUNT;
    shown->numbers[shown->count].base = (double)value;
    shown->numbers[shown->count].start = start;
    shown->numbers[shown->count].end = text_length(shown->text, sizeof shown->text);
    ++shown->count;
    return written;
}

static bool add_numbers(BonusShown *shown, const Sheet *sheet, int row)
{
    int index;
    bool fine = true;

    for (index = sheet->shape.first[row]; fine && (index < sheet->shape.first[row + 1]); ++index) {
        fine = text_append(shown->text, sizeof shown->text, BEFORE[sheet->numbers[index].label]) &&
               add_number(shown, &sheet->numbers[index]) &&
               text_append(shown->text, sizeof shown->text, AFTER[sheet->numbers[index].label]);
    }
    return fine;
}

bool number_unique_row(NumberPlace place, BonusShown *shown)
{
    Sheet sheet;
    int row = place.focus.pick;

    fill_sheet(place.catalog, place.design, place.focus.panel, &sheet);
    shown->text[0] = '\0';
    shown->count = 0;
    if (row < sheet.shape.lines) {
        return text_copy(shown->text, sizeof shown->text, sheet.lines[row]);
    }
    return add_numbers(shown, &sheet, row - sheet.shape.lines);
}

static int number_at(NumberPlace place, Sheet *sheet)
{
    fill_sheet(place.catalog, place.design, place.focus.panel, sheet);
    return sheet->shape.first[place.focus.pick - sheet->shape.lines] + place.focus.number;
}

bool number_unique_name(NumberPlace place, char *text, size_t capacity)
{
    Sheet sheet;
    int index = number_at(place, &sheet);

    return text_copy(text, capacity, NAMES[sheet.numbers[index].label]);
}

bool number_unique_range(NumberPlace place, const char *before, char *text, size_t capacity)
{
    Sheet sheet;
    int index = number_at(place, &sheet);
    SheetNumber number = sheet.numbers[index];

    return text_copy(text, capacity, before) &&
           text_append_decimal(text, capacity, number.range.least, number.places) &&
           text_append(text, capacity, " to ") && text_append_decimal(text, capacity, number.range.most, number.places);
}

Command number_unique_command(NumberPlace place, double value)
{
    Sheet sheet;
    int index = number_at(place, &sheet);
    int stored;

    if (!civ_fit_decimal(sheet.numbers[index].range, sheet.numbers[index].places, value, &stored)) {
        return command_nothing();
    }
    if (place.focus.panel == NUMBER_PANEL_UNIT) {
        return command_of(COMMAND_SET_UNIT_STAT, index, stored);
    }
    return command_set_tech_value(number_panel_tier(place.focus.panel), (TechValue)index, stored);
}
