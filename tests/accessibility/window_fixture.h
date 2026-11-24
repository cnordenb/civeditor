#ifndef WINDOW_FIXTURE_H
#define WINDOW_FIXTURE_H

#include <stdbool.h>
#include <windows.h>

enum { WINDOW_FIXTURE_LABEL_CAPACITY = 64 };

typedef struct {
    LRESULT rows;
    bool list_in_focus;
    wchar_t list_label[WINDOW_FIXTURE_LABEL_CAPACITY];
    bool check_above_buttons;
} AskedDialog;

void window_fixture_close(HWND window);
void window_fixture_act(HWND window, int action);
void window_fixture_click(HWND window, int control);
void window_fixture_tick(HWND window, int control);
void window_fixture_answer(const wchar_t *row, const wchar_t *name);
void window_fixture_filter(const wchar_t *filter);
void window_fixture_refocus(void);
void window_fixture_toggle(void);
const AskedDialog *window_fixture_asked(void);
HWND window_fixture_row(HWND window, const wchar_t *text);
HWND window_fixture_field(HWND window, const wchar_t *text, int number);
HWND window_fixture_panel_row(HWND window, int panel, const wchar_t *text);
HWND window_fixture_panel_field(HWND window, int panel, const wchar_t *text, int number);

#endif
