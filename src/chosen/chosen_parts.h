#ifndef CHOSEN_PARTS_H
#define CHOSEN_PARTS_H

#include "../win/win_parts.h"

enum {
    CHOSEN_FIELD_LEAST = 24,
    CHOSEN_FIELD_MARGIN = 4,
    CHOSEN_FIELD_EDGE = 1,
    CHOSEN_FIELD_INSET = 2,
    CHOSEN_LINE_GAP = 6,
    CHOSEN_CARD_HEIGHT = 44,
    CHOSEN_CARD_DROP = 4,
    CHOSEN_CARD_GAP = 8,
    CHOSEN_TRACK_WIDTH = 200
};

typedef struct {
    int text;
    int field;
    int line;
    int pad;
    int inset;
} ChosenLines;

typedef struct {
    BonusShown shown;
    NumberLayout layout;
} ChosenRow;

typedef struct {
    ControlId panel;
    ControlId list;
} PanelControls;

ChosenLines chosen_lines(const AppState *app, HDC context);
int chosen_text_width(HDC context, const char *text);
bool chosen_lay_out(const AppState *app, HDC context, NumberFocus focus, int width, ChosenRow *row);
NumberPlace chosen_place_of(const AppState *app, NumberFocus focus);
NumberFocus chosen_focus_of(const AppState *app, HWND window);
PanelControls chosen_controls(NumberPanel panel);
HWND chosen_panel(const AppState *app, NumberPanel panel);
NumberPanel chosen_panel_of(const AppState *app, HWND window);
bool chosen_is_card(const AppState *app, HWND window);
void chosen_show_text(HWND window, const char *text);
void chosen_draw_row(const AppState *app, NumberPanel panel, const DRAWITEMSTRUCT *item);
void chosen_print(const AppState *app, HWND window, HDC context);
void chosen_paint(const AppState *app, HWND window);
LRESULT chosen_color(const AppState *app, HWND window, HDC context);
void chosen_arrange(AppState *app, NumberPanel panel);
void chosen_reveal(AppState *app, NumberPanel panel, RECT shown);
void chosen_commit(AppState *app, HWND field);
void chosen_slide(AppState *app, WPARAM wparam);
void chosen_place_card(AppState *app);

#endif
