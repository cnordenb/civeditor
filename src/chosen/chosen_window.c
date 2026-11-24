#include "chosen_parts.h"

#include <wchar.h>

#include "../core/check.h"
#include "../core/text.h"

enum { LABEL_ROOM = 160, WHEEL_LINES = 3, SLOTS = CIV_BONUS_CAPACITY * BONUS_NUMBER_CAPACITY };

static const PanelControls PANELS[NUMBER_PANEL_COUNT] = { { CONTROL_BONUS_INFO, CONTROL_BONUS_LIST },
                                                          { CONTROL_UNIT_INFO, CONTROL_UNIT_LIST },
                                                          { CONTROL_CASTLE_TECH_INFO, CONTROL_CASTLE_TECH_LIST },
                                                          { CONTROL_IMPERIAL_TECH_INFO, CONTROL_IMPERIAL_TECH_LIST },
                                                          { CONTROL_TEAM_INFO, CONTROL_TEAM_LIST } };

const wchar_t CHOSEN_CLASS_NAME[] = L"civeditor_chosen";

static AppState *owner;
static ChosenRow placed;

void chosen_start(AppState *app)
{
    INITCOMMONCONTROLSEX classes = { sizeof classes, ICC_BAR_CLASSES };
    NumberFocus none = { NUMBER_PANEL_BONUSES, NUMBER_NONE, NUMBER_NONE };

    (void)InitCommonControlsEx(&classes);
    owner = app;
    ZeroMemory(&app->chosen, sizeof app->chosen);
    app->number_focus = none;
    app->number_active = false;
}

int chosen_field_id(int pick, int number)
{
    return CHOSEN_FIELD_ID_BASE + (pick * BONUS_NUMBER_CAPACITY) + number;
}

PanelControls chosen_controls(NumberPanel panel)
{
    return PANELS[check_index((int)panel, NUMBER_PANEL_COUNT, 0)];
}

HWND chosen_panel(const AppState *app, NumberPanel panel)
{
    return app->controls[chosen_controls(panel).panel];
}

NumberPanel chosen_panel_of(const AppState *app, HWND window)
{
    int panel;

    for (panel = 0; panel < (int)NUMBER_PANEL_COUNT; ++panel) {
        if (window == chosen_panel(app, (NumberPanel)panel)) {
            return (NumberPanel)panel;
        }
    }
    return NUMBER_PANEL_COUNT;
}

bool chosen_is_card(const AppState *app, HWND window)
{
    int panel;

    for (panel = 0; panel < (int)NUMBER_PANEL_COUNT; ++panel) {
        if (window == app->chosen[panel].card) {
            return true;
        }
    }
    return false;
}

NumberPlace chosen_place_of(const AppState *app, NumberFocus focus)
{
    NumberPlace place = { &app->catalog, &app->history.present, focus };

    return place;
}

NumberFocus chosen_focus_of(const AppState *app, HWND window)
{
    NumberFocus focus = { NUMBER_PANEL_BONUSES, NUMBER_NONE, NUMBER_NONE };
    NumberPanel panel = chosen_panel_of(app, GetParent(window));
    int slot = GetDlgCtrlID(window) - CHOSEN_FIELD_ID_BASE;

    if ((panel == NUMBER_PANEL_COUNT) || (slot < 0) || (slot >= SLOTS)) {
        return focus;
    }
    focus.panel = panel;
    focus.pick = slot / BONUS_NUMBER_CAPACITY;
    focus.number = slot % BONUS_NUMBER_CAPACITY;
    return focus;
}

void chosen_show_text(HWND window, const char *text)
{
    wchar_t wide[BONUS_TEXT_CAPACITY];
    wchar_t now[BONUS_TEXT_CAPACITY];

    if (!text_widen(text, wide, BONUS_TEXT_CAPACITY)) {
        return;
    }
    now[0] = L'\0';
    (void)GetWindowTextW(window, now, BONUS_TEXT_CAPACITY);
    if (wcscmp(now, wide) != 0) {
        (void)SetWindowTextW(window, wide);
    }
}

static void fit_font(const AppState *app, HWND field)
{
    int margin = assist_scaled(app, CHOSEN_FIELD_MARGIN);

    (void)SendMessageW(field, WM_SETFONT, (WPARAM)app->font, FALSE);
    (void)SendMessageW(field, EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, MAKELPARAM(margin, margin));
}

static HWND make_child(const AppState *app, NumberPanel panel, const wchar_t *kind, DWORD style, int identifier)
{
    return CreateWindowExW(WS_EX_LEFT, kind, L"", WS_CHILD | WS_CLIPSIBLINGS | style, 0, 0, 0, 0,
                           chosen_panel(app, panel), (HMENU)(INT_PTR)identifier, app->instance, NULL);
}

static HWND row_window(AppState *app, NumberPanel panel, int pick)
{
    HWND *row = &app->chosen[panel].rows[pick];

    if (*row == NULL) {
        *row = make_child(app, panel, L"STATIC", SS_OWNERDRAW, CHOSEN_ROW_ID_BASE + pick);
    }
    return *row;
}

static HWND field_window(AppState *app, NumberFocus focus)
{
    HWND *field = &app->chosen[focus.panel].fields[focus.pick][focus.number];

    if (*field == NULL) {
        *field = make_child(app, focus.panel, L"EDIT", WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL | ES_RIGHT,
                            chosen_field_id(focus.pick, focus.number));
        fit_font(app, *field);
    }
    return *field;
}

static void sync_field(AppState *app, NumberFocus focus)
{
    HWND field = field_window(app, focus);
    NumberPlace place = chosen_place_of(app, focus);
    char text[NUMBER_TEXT_CAPACITY] = "";
    char name[BONUS_TEXT_CAPACITY] = "";
    wchar_t wide[BONUS_TEXT_CAPACITY] = L"";

    (void)number_text(place, number_times(place), NUMBER_DIGITS, text, sizeof text);
    chosen_show_text(field, text);
    if (number_name(place, name, sizeof name) && text_widen(name, wide, BONUS_TEXT_CAPACITY)) {
        assist_annotate(field, ASSIST_NAME, wide);
    }
    (void)ShowWindow(field, SW_SHOWNA);
}

static void hide(HWND window)
{
    if (window != NULL) {
        (void)ShowWindow(window, SW_HIDE);
    }
}

static void sync_row(AppState *app, NumberPanel panel, int pick)
{
    HWND row = row_window(app, panel, pick);
    NumberFocus focus = { panel, pick, 0 };
    bool shown = number_row(chosen_place_of(app, focus), &placed.shown);
    int count = shown ? placed.shown.count : 0;

    chosen_show_text(row, shown ? placed.shown.text : "");
    (void)ShowWindow(row, SW_SHOWNA);
    for (focus.number = 0; focus.number < BONUS_NUMBER_CAPACITY; ++focus.number) {
        if (focus.number < count) {
            sync_field(app, focus);
        } else {
            hide(app->chosen[panel].fields[pick][focus.number]);
        }
    }
}

static void hide_past(const ChosenWindows *windows, int rows)
{
    int slot;

    for (slot = rows * BONUS_NUMBER_CAPACITY; slot < SLOTS; ++slot) {
        hide(windows->fields[slot / BONUS_NUMBER_CAPACITY][slot % BONUS_NUMBER_CAPACITY]);
        if ((slot % BONUS_NUMBER_CAPACITY) == 0) {
            hide(windows->rows[slot / BONUS_NUMBER_CAPACITY]);
        }
    }
}

static void order_fields(const ChosenWindows *windows)
{
    HWND after = HWND_TOP;
    HWND field;
    int slot;

    for (slot = 0; slot < SLOTS; ++slot) {
        field = windows->fields[slot / BONUS_NUMBER_CAPACITY][slot % BONUS_NUMBER_CAPACITY];
        if (field != NULL) {
            (void)SetWindowPos(field, after, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
            after = field;
        }
    }
}

static void show_label(const AppState *app, NumberPanel panel)
{
    ControlId control = chosen_controls(panel).panel;
    char label[LABEL_ROOM];
    wchar_t wide[LABEL_ROOM];

    (void)text_copy(label, sizeof label, layout_spec(control).label);
    if (panel == NUMBER_PANEL_BONUSES) {
        (void)text_append(label, sizeof label, ": ");
        (void)text_append_number(label, sizeof label, app->history.present.bonus_count);
        (void)text_append(label, sizeof label, " of ");
        (void)text_append_number(label, sizeof label, CIV_BONUS_CAPACITY);
    }
    if (dialogs_widen(label, wide, LABEL_ROOM)) {
        (void)SetWindowTextW(app->labels[control], wide);
        assist_annotate(app->controls[control], ASSIST_NAME, wide);
    }
}

static void sync_panel(AppState *app, NumberPanel panel)
{
    int rows = number_rows(&app->catalog, &app->history.present, panel);
    int pick;

    show_label(app, panel);
    for (pick = 0; (pick < rows) && (pick < CIV_BONUS_CAPACITY); ++pick) {
        sync_row(app, panel, pick);
    }
    hide_past(&app->chosen[panel], rows);
    order_fields(&app->chosen[panel]);
    chosen_arrange(app, panel);
}

void chosen_sync(AppState *app)
{
    int panel;

    for (panel = 0; panel < (int)NUMBER_PANEL_COUNT; ++panel) {
        if (layout_shows(PANELS[panel].panel, app->step)) {
            sync_panel(app, (NumberPanel)panel);
        }
    }
}

static void place_field(const ChosenWindows *windows, int pick, int piece, ChosenLines lines, int top)
{
    int number = placed.layout.pieces[piece].number;

    if (number != NUMBER_NONE) {
        (void)MoveWindow(windows->fields[pick][number], lines.inset + placed.layout.x[piece],
                         top + (lines.pad / 2) + (placed.layout.line[piece] * lines.line), placed.layout.widths[piece],
                         lines.field, TRUE);
    }
}

static int place_row(const AppState *app, HDC context, NumberFocus focus, RECT row)
{
    const ChosenWindows *windows = &app->chosen[focus.panel];
    ChosenLines lines = chosen_lines(app, context);
    int height = lines.pad + lines.line;
    int piece;

    if (chosen_lay_out(app, context, focus, row.right - (2 * lines.inset), &placed)) {
        height = lines.pad + (placed.layout.lines * lines.line);
        for (piece = 0; (piece < placed.layout.count) && (piece < NUMBER_PIECE_CAPACITY); ++piece) {
            place_field(windows, focus.pick, piece, lines, row.top);
        }
    }
    (void)MoveWindow(windows->rows[focus.pick], 0, row.top, row.right, height, TRUE);
    return height;
}

static int card_room(const AppState *app, NumberPanel panel, int rows)
{
    NumberFocus last = { panel, rows - 1, 0 };

    return number_slides(chosen_place_of(app, last)) ? assist_scaled(app, CHOSEN_CARD_DROP + CHOSEN_CARD_HEIGHT) : 0;
}

static int place_rows(const AppState *app, NumberPanel panel, RECT client)
{
    HWND holder = chosen_panel(app, panel);
    HDC context = GetDC(holder);
    HGDIOBJ before = SelectObject(context, app->font);
    int rows = number_rows(&app->catalog, &app->history.present, panel);
    NumberFocus focus = { panel, 0, 0 };
    RECT row = client;

    row.top = -app->chosen[panel].scroll;
    for (focus.pick = 0; (focus.pick < rows) && (focus.pick < CIV_BONUS_CAPACITY); ++focus.pick) {
        row.top += place_row(app, context, focus, row);
    }
    (void)SelectObject(context, before);
    (void)ReleaseDC(holder, context);
    return row.top + app->chosen[panel].scroll + card_room(app, panel, rows);
}

static void set_bar(const AppState *app, NumberPanel panel, int view)
{
    SCROLLINFO bar;

    ZeroMemory(&bar, sizeof bar);
    bar.cbSize = sizeof bar;
    bar.fMask = SIF_RANGE | SIF_PAGE | SIF_POS | SIF_DISABLENOSCROLL;
    bar.nMax = app->chosen[panel].content - 1;
    bar.nPage = (UINT)view;
    bar.nPos = app->chosen[panel].scroll;
    (void)SetScrollInfo(chosen_panel(app, panel), SB_VERT, &bar, TRUE);
}

void chosen_arrange(AppState *app, NumberPanel panel)
{
    ChosenWindows *windows = &app->chosen[panel];
    RECT client = { 0, 0, 0, 0 };
    int within;

    (void)GetClientRect(chosen_panel(app, panel), &client);
    windows->content = place_rows(app, panel, client);
    within = reflow_scroll_within(windows->scroll, windows->content, client.bottom);
    if (within != windows->scroll) {
        windows->scroll = within;
        (void)place_rows(app, panel, client);
    }
    set_bar(app, panel, client.bottom);
    chosen_place_card(app);
}

static void scroll_panel_to(AppState *app, NumberPanel panel, int target)
{
    ChosenWindows *windows = &app->chosen[panel];
    RECT client = { 0, 0, 0, 0 };
    int within;

    (void)GetClientRect(chosen_panel(app, panel), &client);
    within = reflow_scroll_within(target, windows->content, client.bottom);
    if (within != windows->scroll) {
        windows->scroll = within;
        chosen_arrange(app, panel);
    }
}

void chosen_reveal(AppState *app, NumberPanel panel, RECT shown)
{
    int scroll = app->chosen[panel].scroll;
    RECT client = { 0, 0, 0, 0 };
    int top = shown.top + scroll;
    int bottom = shown.bottom + scroll;
    int target;

    (void)GetClientRect(chosen_panel(app, panel), &client);
    target = (bottom > scroll + client.bottom) ? bottom - client.bottom : scroll;
    scroll_panel_to(app, panel, (top < target) ? top : target);
}

static void scroll_panel(AppState *app, NumberPanel panel, UINT message, WPARAM wparam)
{
    RECT client = { 0, 0, 0, 0 };
    int line = controls_row_height(app);
    int scroll = app->chosen[panel].scroll;
    BarMove move = { (int)LOWORD(wparam), scroll, line, 0 };

    (void)GetClientRect(chosen_panel(app, panel), &client);
    move.view = client.bottom;
    if (message == WM_MOUSEWHEEL) {
        scroll_panel_to(app, panel,
                        scroll - MulDiv(GET_WHEEL_DELTA_WPARAM(wparam), WHEEL_LINES * line, WHEEL_DELTA));
    } else {
        scroll_panel_to(app, panel, assist_bar_target(chosen_panel(app, panel), SB_VERT, move));
    }
}

static void fit_fonts(const AppState *app, const ChosenWindows *windows)
{
    int slot;
    HWND field;

    for (slot = 0; slot < SLOTS; ++slot) {
        field = windows->fields[slot / BONUS_NUMBER_CAPACITY][slot % BONUS_NUMBER_CAPACITY];
        if (field != NULL) {
            fit_font(app, field);
        }
    }
}

static bool panel_handled(AppState *app, HWND window, UINT message, WPARAM wparam)
{
    NumberPanel panel = chosen_panel_of(app, window);

    if (panel == NUMBER_PANEL_COUNT) {
        return false;
    }
    if (message == WM_SETFONT) {
        fit_fonts(app, &app->chosen[panel]);
    }
    if ((message == WM_SIZE) || (message == WM_SETFONT)) {
        chosen_arrange(app, panel);
        return true;
    }
    if ((message == WM_VSCROLL) || (message == WM_MOUSEWHEEL)) {
        scroll_panel(app, panel, message, wparam);
        return true;
    }
    return false;
}

static bool handled(AppState *app, HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    if (message == WM_CREATE) {
        (void)SetWindowLongPtrW(window, GWL_EXSTYLE, GetWindowLongPtrW(window, GWL_EXSTYLE) | WS_EX_CONTROLPARENT);
        return true;
    }
    if ((message == WM_COMMAND) && (HIWORD(wparam) == EN_SETFOCUS)) {
        assist_follow_focus(app);
        return true;
    }
    if ((message == WM_COMMAND) && (HIWORD(wparam) == EN_KILLFOCUS)) {
        chosen_commit(app, (HWND)lparam);
        return true;
    }
    if (message == WM_HSCROLL) {
        chosen_slide(app, wparam);
        return true;
    }
    return panel_handled(app, window, message, wparam);
}

LRESULT CALLBACK chosen_procedure(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    if ((message == WM_CTLCOLOREDIT) || (message == WM_CTLCOLORSTATIC)) {
        return chosen_color(owner, window, (HDC)wparam);
    }
    if (message == WM_DRAWITEM) {
        chosen_draw_row(owner, chosen_panel_of(owner, window), (const DRAWITEMSTRUCT *)lparam);
        return TRUE;
    }
    if (message == WM_PAINT) {
        chosen_paint(owner, window);
        return 0;
    }
    if (message == WM_PRINTCLIENT) {
        chosen_print(owner, window, (HDC)wparam);
        return 0;
    }
    if (message == WM_ERASEBKGND) {
        return 1;
    }
    return handled(owner, window, message, wparam, lparam) ? 0 : DefWindowProcW(window, message, wparam, lparam);
}
