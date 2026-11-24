#include "number_unique.h"

#include <math.h>
#include <string.h>

#include "../core/check.h"
#include "../core/text.h"

enum { HUNDREDTHS = 100, SLIDER_STEP = CIV_MILLIONTHS / HUNDREDTHS };

static const double HALF = 0.5;
static const char MISSING[] = "a bonus not in the catalog";

typedef struct {
    const BonusPick *picks;
    int count;
    int first;
    bool team;
} PanelPicks;

static BonusShown shown;

static PanelPicks picks_of(const Catalog *catalog, const CivDesign *design, NumberPanel panel)
{
    PanelPicks found = { design->bonuses, design->bonus_count, 0, false };
    PanelPicks team = { design->team_bonuses, design->team_bonus_count, 0, true };
    const TechPick *tech;

    if (panel == NUMBER_PANEL_BONUSES) {
        return found;
    }
    if (panel == NUMBER_PANEL_TEAM) {
        return team;
    }
    found.first = number_unique_rows(catalog, design, panel);
    found.count = 0;
    if (panel != NUMBER_PANEL_UNIT) {
        tech = &design->techs[number_panel_tier(panel)];
        found.picks = tech->effects;
        found.count = tech->effect_count;
    }
    return found;
}

static const BonusPick *pick_at(NumberPlace place)
{
    PanelPicks picks = picks_of(place.catalog, place.design, place.focus.panel);
    int index = place.focus.pick - picks.first;

    return check_is_index(index, picks.count) ? &picks.picks[index] : NULL;
}

static bool show_pick(NumberPlace place, double times, BonusShown *row)
{
    bool team = place.focus.panel == NUMBER_PANEL_TEAM;
    const BonusEntry *entries = team ? place.catalog->team_bonuses : place.catalog->bonuses;
    const BonusPick *pick = pick_at(place);
    int found = (pick != NULL) ? catalog_find_bonus(place.catalog, team, pick->id) : CATALOG_MISSING;

    return (found != CATALOG_MISSING) && bonus_text_show(entries[found].description, times, row);
}

static bool show_row(NumberPlace place, double times, BonusShown *row)
{
    if (!number_slides(place)) {
        return number_unique_row(place, row);
    }
    return show_pick(place, times, row);
}

static bool show_focus(NumberPlace place, double times)
{
    return show_row(place, times, &shown) && (place.focus.number >= 0) && (place.focus.number < shown.count);
}

int number_rows(const Catalog *catalog, const CivDesign *design, NumberPanel panel)
{
    PanelPicks picks = picks_of(catalog, design, panel);

    return picks.first + picks.count;
}

bool number_slides(NumberPlace place)
{
    return place.focus.pick >= picks_of(place.catalog, place.design, place.focus.panel).first;
}

double number_times(NumberPlace place)
{
    const BonusPick *pick = pick_at(place);

    return (pick == NULL) ? 1.0 : civ_multiplier_times(pick->multiplier);
}

bool number_row(NumberPlace place, BonusShown *row)
{
    if (!CHECK(check_is_index(place.focus.pick, number_rows(place.catalog, place.design, place.focus.panel)))) {
        return false;
    }
    if (show_row(place, number_times(place), row)) {
        return true;
    }
    row->count = 0;
    return text_copy(row->text, sizeof row->text, MISSING);
}

bool number_name(NumberPlace place, char *text, size_t capacity)
{
    if (!number_slides(place)) {
        return number_unique_name(place, text, capacity);
    }
    return number_row(place, &shown) && text_copy(text, capacity, shown.text);
}

bool number_text(NumberPlace place, double times, NumberText form, char *text, size_t capacity)
{
    size_t start;
    size_t end;

    if (!show_focus(place, times)) {
        return false;
    }
    number_span(&shown, place.focus.number, &start, &end);
    end = (form == NUMBER_WITH_UNIT) ? number_unit_end(&shown, end) : end;
    return text_copy_span(text, capacity, &shown.text[start], end - start);
}

bool number_range_text(NumberPlace place, const char *before, char *text, size_t capacity)
{
    char least[NUMBER_TEXT_CAPACITY];
    char most[NUMBER_TEXT_CAPACITY];

    if (!number_slides(place)) {
        return number_unique_range(place, before, text, capacity);
    }
    return number_text(place, 1.0, NUMBER_WITH_UNIT, least, sizeof least) &&
           number_text(place, (double)CIV_MULTIPLIER_LIMIT, NUMBER_WITH_UNIT, most, sizeof most) &&
           text_join(text, capacity, before, least) && text_append_two(text, capacity, " to ", most);
}

static size_t skipped(const char *typed, size_t at, const char *letters)
{
    size_t end = at;

    while ((typed[end] != '\0') && (strchr(letters, typed[end]) != NULL)) {
        ++end;
    }
    return end;
}

static bool typed_value(const char *typed, const char *signs, double *value)
{
    size_t start = skipped(typed, 0U, signs);
    size_t end = skipped(typed, skipped(typed, start, "-"), "0123456789.");
    TextDecimal number = text_to_decimal(&typed[start], end - start);

    *value = number.value;
    return number.valid && (typed[skipped(typed, end, " %x")] == '\0');
}

static Command count_command(NumberPlace place, int millionths)
{
    const BonusPick *pick = pick_at(place);

    if (pick == NULL) {
        return command_nothing();
    }
    if (place.focus.panel == NUMBER_PANEL_BONUSES) {
        return command_of(COMMAND_SET_MULTIPLIER, pick->id, millionths);
    }
    if (place.focus.panel == NUMBER_PANEL_TEAM) {
        return command_of(COMMAND_SET_TEAM_MULTIPLIER, pick->id, millionths);
    }
    return command_set_tech_effect(number_panel_tier(place.focus.panel), pick->id, millionths);
}

static Command bonus_command(NumberPlace place, double value)
{
    BonusNumber number;
    double times;

    if (!show_focus(place, 1.0)) {
        return command_nothing();
    }
    number = shown.numbers[place.focus.number];
    if ((bonus_tenths(value) < bonus_tenths(bonus_number_value(number, 1.0))) ||
        (bonus_tenths(value) > bonus_tenths(bonus_number_value(number, (double)CIV_MULTIPLIER_LIMIT)))) {
        return command_nothing();
    }
    times = fmin(fmax(bonus_number_times(number, value), 1.0), (double)CIV_MULTIPLIER_LIMIT);
    return count_command(place, civ_multiplier_of(times).millionths);
}

Command number_typed_command(NumberPlace place, const char *typed)
{
    bool slides = number_slides(place);
    double value;

    if (!typed_value(typed, slides ? " +-" : " +", &value)) {
        return command_nothing();
    }
    return slides ? bonus_command(place, value) : number_unique_command(place, value);
}

int number_slider_position(NumberPlace place)
{
    return (int)((number_times(place) * (double)HUNDREDTHS) + HALF);
}

double number_slider_times(int position)
{
    return (double)position / (double)HUNDREDTHS;
}

Command number_slid_command(NumberPlace place, int position)
{
    return count_command(place, position * SLIDER_STEP);
}
