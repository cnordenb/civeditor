#ifndef WIN_PARTS_H
#define WIN_PARTS_H

#include <stdbool.h>
#include <windows.h>
#include <commctrl.h>

#include "../core/file_io.h"
#include "../data/catalog.h"
#include "../model/history.h"
#include "../numbers/number_edit.h"
#include "../present/binding.h"
#include "../reflow/reflow.h"
#include "../roster/civ_roster.h"
#include "../view/layout.h"
#include "../view/palette.h"

enum {
    CONTROL_ID_BASE = 100,
    LABEL_ID_BASE = 300,
    ACTION_ID_BASE = 500,
    APP_GAME_WANTED = WM_APP + 1,
    WIDE_TEXT_CAPACITY = 16384,
    STATUS_CAPACITY = 320,
    MOD_NAME_CAPACITY = 96,
    BOX_SIZE = 18,
    TEXT_INSET = 8,
    ROW_PAD = 12,
    FOCUS_LINE = 3,
    LIST_REPEAT_TIMER = 1,
    WINDOW_BAR_ID = 900,
    CHOSEN_ROW_ID_BASE = 1000,
    CHOSEN_FIELD_ID_BASE = 1100,
    CHOSEN_SLIDER_ID = 1500,
    CHOSEN_CARD_ID = 1501
};

typedef enum { ASSIST_NAME, ASSIST_VALUE, ASSIST_PROPERTY_COUNT } AssistProperty;

typedef struct {
    int code;
    int scroll;
    int line;
    int view;
} BarMove;

typedef struct {
    HWND rows[CIV_BONUS_CAPACITY];
    HWND fields[CIV_BONUS_CAPACITY][BONUS_NUMBER_CAPACITY];
    HWND card;
    HWND slider;
    int scroll;
    int content;
} ChosenWindows;

typedef enum {
    ACTION_NEXT,
    ACTION_BACK,
    ACTION_SAVE,
    ACTION_OPEN,
    ACTION_UNDO,
    ACTION_REDO,
    ACTION_BUILD,
    ACTION_OPTIONS,
    ACTION_EXIT,
    ACTION_HELP,
    ACTION_SCROLL_UP,
    ACTION_SCROLL_DOWN,
    ACTION_FIRST_STEP,
    ACTION_COUNT = ACTION_FIRST_STEP + STEP_COUNT
} Action;

typedef struct {
    int width;
    short heights[LIST_ROW_CAPACITY];
} WrappedRows;

typedef struct {
    HINSTANCE instance;
    HWND window;
    HWND bar;
    HWND popup;
    HWND framed;
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
    bool built;
    int dpi;
    int scroll;
    History history;
    CivRoster roster;
    Catalog catalog;
    ListModel lists[CONTROL_COUNT];
    int bonus_category;
    ChosenWindows chosen[NUMBER_PANEL_COUNT];
    WrappedRows wrapped[CONTROL_COUNT];
    NumberFocus number_focus;
    bool number_active;
    wchar_t file_path[FILE_PATH_CAPACITY];
    wchar_t mod_name[MOD_NAME_CAPACITY];
    wchar_t mods_folder[FILE_PATH_CAPACITY];
} AppState;

bool controls_create(AppState *app);
void controls_place(AppState *app);
void controls_show(const AppState *app);
void controls_show_step(AppState *app);
HBRUSH controls_color(const AppState *app, HDC context, HWND control);
void controls_draw_item(const AppState *app, const DRAWITEMSTRUCT *item);
int controls_row_height(const AppState *app);
COLORREF controls_row_color(const AppState *app, bool chosen, ColorRole plain);
void controls_frame_rect(HDC context, RECT area, COLORREF color, int thickness);
void controls_refresh_style(AppState *app);

void assist_mark_live(HWND status);
void assist_annotate(HWND control, AssistProperty property, const wchar_t *text);
void assist_draw_ring(const AppState *app, HDC context, RECT control);
int assist_bar_target(HWND window, int bar, BarMove move);
int assist_dpi(HWND window);
int assist_scaled(const AppState *app, int units);
void assist_move_focus(AppState *app, WPARAM wparam, LPARAM lparam);
void assist_show_help(AppState *app);
void assist_follow_focus(AppState *app);
UINT assist_row_format(const AppState *app, const DRAWITEMSTRUCT *item, const wchar_t *text, RECT *area);
void assist_wrap_rows(AppState *app, ControlId control);
void assist_placed(AppState *app);
void assist_forget_rows(AppState *app);
int assist_scroll_range(AppState *app, int content, int view);
RECT assist_view(const AppState *app);
void assist_make_bar(AppState *app);
void assist_scroll(AppState *app, UINT message, WPARAM wparam);
void assist_draw_focus(const AppState *app, HDC context);
void assist_plain_bar(HWND control);
void assist_run_message(AppState *app, MSG *message);

void sync_design(AppState *app, bool refill);
void sync_list(AppState *app, ControlId control);
void sync_on_notify(AppState *app, int control, int code);
void sync_details(AppState *app);
void sync_apply(AppState *app, const Command *command);

void chosen_sync(AppState *app);
bool chosen_take_message(AppState *app, const MSG *message);
void chosen_follow_focus(AppState *app);
void chosen_start(AppState *app);
int chosen_field_id(int pick, int number);
extern const wchar_t CHOSEN_CLASS_NAME[];
LRESULT CALLBACK chosen_procedure(HWND window, UINT message, WPARAM wparam, LPARAM lparam);

bool dialogs_widen(const char *text, wchar_t *wide, int capacity);
bool dialogs_narrow(const wchar_t *wide, char *text, int capacity);
void dialogs_status(AppState *app, const char *text);
bool dialogs_save(AppState *app);
bool dialogs_open(AppState *app);
bool dialogs_load_catalog(AppState *app);
void dialogs_read_game(AppState *app);
bool dialogs_may_discard(AppState *app);
bool dialogs_build(AppState *app);
void dialogs_options(AppState *app);
bool dialogs_choose_hero(AppState *app, Command *command);
bool dialogs_choose_effects(AppState *app, TechTier tier, CivDesign *chosen);

extern const wchar_t POPUP_CLASS_NAME[];
LRESULT CALLBACK popup_procedure(HWND popup, UINT message, WPARAM wparam, LPARAM lparam);
bool popup_make(AppState *app);
void popup_show(AppState *app, bool shown);

HWND app_open(HINSTANCE instance);
int app_run(HINSTANCE instance, int show);
int app_run_messages(void);

#endif
