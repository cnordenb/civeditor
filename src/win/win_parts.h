#ifndef WIN_PARTS_H
#define WIN_PARTS_H

#include <stdbool.h>
#include <windows.h>

#include "../core/file_io.h"
#include "../data/catalog.h"
#include "../model/history.h"
#include "../present/binding.h"
#include "../view/layout.h"
#include "../view/palette.h"

enum {
    CONTROL_ID_BASE = 100,
    LABEL_ID_BASE = 300,
    ACTION_ID_BASE = 500,
    WIDE_TEXT_CAPACITY = 16384,
    STATUS_CAPACITY = 320
};

typedef enum {
    ACTION_NEXT,
    ACTION_BACK,
    ACTION_SAVE,
    ACTION_OPEN,
    ACTION_UNDO,
    ACTION_REDO,
    ACTION_BUILD,
    ACTION_EXIT,
    ACTION_FIRST_STEP,
    ACTION_COUNT = ACTION_FIRST_STEP + STEP_COUNT
} Action;

typedef struct {
    HINSTANCE instance;
    HWND window;
    HWND controls[CONTROL_COUNT];
    HWND labels[CONTROL_COUNT];
    HFONT font;
    HFONT title_font;
    HBRUSH paper_brush;
    HBRUSH field_brush;
    HBRUSH chosen_brush;
    HACCEL accelerators;
    WizardStep step;
    bool syncing;
    bool high_contrast;
    bool unsaved;
    bool catalog_loaded;
    int dpi;
    History history;
    Catalog catalog;
    ListModel lists[CONTROL_COUNT];
    int bonus_category;
    wchar_t file_path[FILE_PATH_CAPACITY];
} AppState;

bool controls_create(AppState *app);
void controls_place(AppState *app);
void controls_show_step(AppState *app);
HBRUSH controls_color(const AppState *app, HDC context, HWND control);
void controls_draw_item(const AppState *app, const DRAWITEMSTRUCT *item);
int controls_row_height(const AppState *app);
void controls_refresh_style(AppState *app);

void sync_all(AppState *app);
void sync_model(AppState *app);
void sync_list(AppState *app, ControlId control);
void sync_on_notify(AppState *app, int control, int code);
void sync_details(AppState *app);

bool dialogs_widen(const char *text, wchar_t *wide, int capacity);
bool dialogs_narrow(const wchar_t *wide, char *text, int capacity);
void dialogs_status(AppState *app, const char *text);
bool dialogs_save(AppState *app);
bool dialogs_open(AppState *app);
bool dialogs_load_catalog(AppState *app);
bool dialogs_may_discard(AppState *app);
bool dialogs_build(AppState *app);

int app_run(HINSTANCE instance, int show);

#endif
