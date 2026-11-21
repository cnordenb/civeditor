#ifndef WINDOW_FIXTURE_H
#define WINDOW_FIXTURE_H

#include <stdbool.h>
#include <windows.h>

enum { WINDOW_FIXTURE_LABEL_CAPACITY = 64 };

typedef struct {
    LRESULT rows;
    bool list_in_focus;
    wchar_t list_label[WINDOW_FIXTURE_LABEL_CAPACITY];
} AskedDialog;

void window_fixture_close(HWND window);
void window_fixture_act(HWND window, int action);
void window_fixture_click(HWND window, int control);
void window_fixture_tick(HWND window, int control);
void window_fixture_answer(const wchar_t *row, const wchar_t *name);
void window_fixture_filter(const wchar_t *filter);
void window_fixture_refocus(void);
const AskedDialog *window_fixture_asked(void);

#endif
