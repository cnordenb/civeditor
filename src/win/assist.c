#include "win_parts.h"

#include <oleacc.h>
#include <uxtheme.h>
#include <windowsx.h>

#include "../core/check.h"

enum { LIVE_POLITE = 1, TEXT_PERCENT_PLAIN = 100, FRAME_REACH = 3, FRAME_WIDTH = 2, FRAME_LIMIT = 64 };
enum { WHEEL_LINES = 3, BAR_MOVES = 4, ROW_TEXT_ROOM = 512, ROW_PAD = 12, ROW_MOST = 255 };
enum { REPEAT_FIRST = 400, REPEAT_NEXT = 50, CLASS_ROOM = 16 };

typedef struct {
    int index;
    int width;
} RowPlace;

void assist_mark_live(HWND status)
{
    static const CLSID SERVICE = { 0xb5f8350bUL, 0x0548, 0x48b1, { 0xa6, 0xee, 0x88, 0xbd, 0x00, 0xb4, 0xa5, 0xe7 } };
    static const IID PROPERTIES = { 0x6e26e776UL, 0x04f0, 0x495d, { 0x80, 0xe4, 0x33, 0x30, 0x35, 0x2e, 0x31, 0x69 } };
    static const GUID LIVE = { 0xc12bcd8eUL, 0x2a8e, 0x4950, { 0x8a, 0xe7, 0x36, 0x25, 0x11, 0x1d, 0x58, 0xeb } };
    void *object = NULL;
    IAccPropServices *service;
    VARIANT polite;

    if (CoCreateInstance(&SERVICE, NULL, CLSCTX_INPROC_SERVER, &PROPERTIES, &object) != S_OK) {
        return;
    }
    service = object;
    VariantInit(&polite);
    polite.vt = VT_I4;
    polite.lVal = LIVE_POLITE;
    (void)IAccPropServices_SetHwndProp(service, status, (DWORD)OBJID_CLIENT, CHILDID_SELF, LIVE, polite);
    (void)IAccPropServices_Release(service);
}

static int text_percent(void)
{
    DWORD percent = TEXT_PERCENT_PLAIN;
    DWORD size = sizeof percent;

    if (RegGetValueW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Accessibility", L"TextScaleFactor",
                     RRF_RT_REG_DWORD, NULL, &percent, &size) != ERROR_SUCCESS) {
        return TEXT_PERCENT_PLAIN;
    }
    return (int)percent;
}

int assist_dpi(HWND window)
{
    HDC screen = GetDC(window);
    int dpi = LAYOUT_BASE_DPI;

    if (screen != NULL) {
        dpi = GetDeviceCaps(screen, LOGPIXELSY);
        (void)ReleaseDC(window, screen);
    }
    if (!CHECK(dpi > 0)) {
        return LAYOUT_BASE_DPI;
    }
    return layout_text_dpi(dpi, text_percent());
}

void assist_move_focus(AppState *app, WPARAM wparam, LPARAM lparam)
{
    HWND target;

    target = (LOWORD(lparam) != 0) ? (HWND)wparam : GetNextDlgTabItem(app->window, GetFocus(), wparam != 0);
    if (target != NULL) {
        (void)SetFocus(target);
    }
}

void assist_show_help(AppState *app)
{
    int control = GetDlgCtrlID(GetFocus()) - CONTROL_ID_BASE;

    if ((control >= 0) && (control < CONTROL_COUNT) && (layout_help((ControlId)control)[0] != '\0')) {
        dialogs_status(app, layout_help((ControlId)control));
    } else {
        dialogs_status(app, wizard_page(app->step)->help);
    }
}

static bool focus_ring(const AppState *app, HWND control, RECT *ring)
{
    int reach;

    if ((control == NULL) || (GetParent(control) != app->window)) {
        return false;
    }
    if (GetWindowRect(control, ring) == 0) {
        return false;
    }
    (void)MapWindowPoints(NULL, app->window, (POINT *)ring, 2);
    reach = MulDiv(FRAME_REACH, app->dpi, LAYOUT_BASE_DPI);
    (void)InflateRect(ring, reach, reach);
    return true;
}

static int view_height(const AppState *app)
{
    RECT client;

    (void)GetClientRect(app->window, &client);
    return MulDiv(client.bottom, LAYOUT_BASE_DPI, app->dpi);
}

int assist_scroll_range(AppState *app, int content, int view)
{
    SCROLLINFO bar;
    RECT client;
    int width = GetSystemMetrics(SM_CXVSCROLL);

    ZeroMemory(&bar, sizeof bar);
    bar.cbSize = sizeof bar;
    bar.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
    bar.nMax = content - 1;
    bar.nPage = (UINT)view;
    bar.nPos = reflow_scroll_within(app->scroll, content, view);
    (void)SetScrollInfo(app->bar, SB_CTL, &bar, TRUE);
    (void)GetClientRect(app->window, &client);
    (void)MoveWindow(app->bar, client.right - width, client.top, width, client.bottom, TRUE);
    (void)ShowWindow(app->bar, (content > view) ? SW_SHOWNA : SW_HIDE);
    return bar.nPos;
}

RECT assist_view(const AppState *app)
{
    RECT view;

    (void)GetClientRect(app->window, &view);
    if ((GetWindowStyle(app->bar) & WS_VISIBLE) == WS_VISIBLE) {
        view.right -= GetSystemMetrics(SM_CXVSCROLL);
    }
    return view;
}

void assist_make_bar(AppState *app)
{
    app->bar = CreateWindowExW(WS_EX_LEFT, L"SCROLLBAR", L"", WS_CHILD | SBS_VERT, CW_USEDEFAULT, CW_USEDEFAULT,
                               CW_USEDEFAULT, CW_USEDEFAULT, app->window, (HMENU)(INT_PTR)WINDOW_BAR_ID,
                               app->instance, NULL);
}

static int bar_target(const AppState *app, int code, int view)
{
    static const int MOVES[BAR_MOVES] = { -REFLOW_LINE, REFLOW_LINE, -1, 1 };
    SCROLLINFO bar;

    if (code > SB_THUMBTRACK) {
        return app->scroll;
    }
    if (code < BAR_MOVES) {
        return app->scroll + (MOVES[code] * ((code < SB_PAGEUP) ? 1 : view));
    }
    bar.cbSize = sizeof bar;
    bar.fMask = SIF_TRACKPOS;
    bar.nTrackPos = app->scroll;
    (void)GetScrollInfo(app->bar, SB_CTL, &bar);
    return bar.nTrackPos;
}

static int page_height(const AppState *app)
{
    SCROLLINFO bar;

    ZeroMemory(&bar, sizeof bar);
    bar.cbSize = sizeof bar;
    bar.fMask = SIF_RANGE;
    (void)GetScrollInfo(app->bar, SB_CTL, &bar);
    return bar.nMax + 1;
}

static void scroll_to(AppState *app, int target)
{
    int within = reflow_scroll_within(target, page_height(app), view_height(app));

    if (within != app->scroll) {
        app->scroll = within;
        controls_place(app);
    }
}

void assist_scroll(AppState *app, UINT message, WPARAM wparam)
{
    int view = view_height(app);

    if (!CHECK(view > 0)) {
        return;
    }
    if (message == WM_MOUSEWHEEL) {
        scroll_to(app, app->scroll - MulDiv(GET_WHEEL_DELTA_WPARAM(wparam), WHEEL_LINES * REFLOW_LINE, WHEEL_DELTA));
    } else {
        scroll_to(app, bar_target(app, (int)LOWORD(wparam), view));
    }
}

static bool is_list(HWND window)
{
    wchar_t kind[CLASS_ROOM] = L"";

    (void)GetClassNameW(window, kind, CLASS_ROOM);
    return lstrcmpiW(kind, L"LISTBOX") == 0;
}

static void move_list(HWND list, int rows)
{
    int moved;

    SetWindowRedraw(list, FALSE);
    for (moved = 0; moved < rows; ++moved) {
        (void)SendMessageW(list, WM_VSCROLL, SB_LINEDOWN, (LPARAM)NULL);
    }
    for (moved = 0; moved > rows; --moved) {
        (void)SendMessageW(list, WM_VSCROLL, SB_LINEUP, (LPARAM)NULL);
    }
    SetWindowRedraw(list, TRUE);
    (void)InvalidateRect(list, NULL, TRUE);
}

static void turn_wheel(AppState *app, const MSG *message)
{
    int rows;

    app->wheel_rest += GET_WHEEL_DELTA_WPARAM(message->wParam);
    rows = app->wheel_rest / WHEEL_DELTA;
    app->wheel_rest %= WHEEL_DELTA;
    move_list(message->hwnd, -rows);
}

static int arrow_rows(HWND list, int y)
{
    SCROLLBARINFO bar;
    bool up;
    bool down;

    bar.cbSize = sizeof bar;
    (void)GetScrollBarInfo(list, OBJID_VSCROLL, &bar);
    up = y < bar.rcScrollBar.top + bar.dxyLineButton;
    down = y >= bar.rcScrollBar.bottom - bar.dxyLineButton;
    return (int)down - (int)up;
}

static bool press_arrow(AppState *app, const MSG *message)
{
    int rows = arrow_rows(message->hwnd, GET_Y_LPARAM(message->lParam));

    if (rows != 0) {
        move_list(message->hwnd, rows);
        app->held_list = message->hwnd;
        app->held_rows = rows;
        (void)SetCapture(message->hwnd);
        (void)SetTimer(app->window, LIST_REPEAT_TIMER, REPEAT_FIRST, NULL);
    }
    return rows != 0;
}

static bool holds_arrow(const AppState *app)
{
    return GetCapture() == app->held_list;
}

static void repeat_arrow(AppState *app)
{
    if (!holds_arrow(app)) {
        (void)KillTimer(app->window, LIST_REPEAT_TIMER);
        return;
    }
    move_list(app->held_list, app->held_rows);
    (void)SetTimer(app->window, LIST_REPEAT_TIMER, REPEAT_NEXT, NULL);
}

static void follow_arrow(AppState *app, const MSG *message)
{
    if ((message->message == WM_LBUTTONUP) && holds_arrow(app)) {
        (void)KillTimer(app->window, LIST_REPEAT_TIMER);
        (void)ReleaseCapture();
    }
    if ((message->message == WM_TIMER) && (message->wParam == LIST_REPEAT_TIMER) && (message->hwnd == app->window)) {
        repeat_arrow(app);
    }
}

static bool is_arrow_press(const MSG *message)
{
    return ((message->message == WM_NCLBUTTONDOWN) || (message->message == WM_NCLBUTTONDBLCLK)) &&
           (message->wParam == HTVSCROLL) && is_list(message->hwnd);
}

static bool takes_list_input(AppState *app, const MSG *message)
{
    follow_arrow(app, message);
    if ((message->message == WM_MOUSEWHEEL) && is_list(message->hwnd)) {
        turn_wheel(app, message);
        return true;
    }
    return is_arrow_press(message) && press_arrow(app, message);
}

void assist_run_message(AppState *app, MSG *message)
{
    if (takes_list_input(app, message) || (TranslateAcceleratorW(app->window, app->accelerators, message) != 0) ||
        (IsDialogMessageW(app->window, message) != 0)) {
        return;
    }
    (void)TranslateMessage(message);
    (void)DispatchMessageW(message);
}

void assist_plain_bar(HWND control)
{
    COMBOBOXINFO choice;

    choice.cbSize = sizeof choice;
    (void)SetWindowTheme((GetComboBoxInfo(control, &choice) != 0) ? choice.hwndList : control, L"", L"");
}

static void show_focus(AppState *app, HWND focus)
{
    Box box = { 0, 0, 0, 0 };
    RECT place;

    if ((focus == NULL) || (GetParent(focus) != app->window) ||
        (GetWindowRect(focus, &place) == 0)) {
        return;
    }
    (void)MapWindowPoints(NULL, app->window, (POINT *)&place, 2);
    box.top = MulDiv(place.top, LAYOUT_BASE_DPI, app->dpi) + app->scroll;
    box.height = MulDiv(place.bottom - place.top, LAYOUT_BASE_DPI, app->dpi);
    scroll_to(app, reflow_scroll_to_show(box, app->scroll, view_height(app)));
}

UINT assist_row_format(const AppState *app, const DRAWITEMSTRUCT *item, const wchar_t *text, RECT *area)
{
    RECT lines;

    if (item->CtlType == ODT_COMBOBOX) {
        return DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS | DT_NOPREFIX;
    }
    area->right -= MulDiv(TEXT_INSET, app->dpi, LAYOUT_BASE_DPI);
    lines = *area;
    (void)DrawTextW(item->hDC, text, -1, &lines, DT_CALCRECT | DT_WORDBREAK | DT_NOPREFIX);
    if (lines.bottom < area->bottom) {
        area->top += (area->bottom - lines.bottom) / 2;
    }
    return DT_WORDBREAK | DT_NOPREFIX;
}

static int row_height(const AppState *app, HWND list, HDC context, RowPlace row)
{
    wchar_t text[ROW_TEXT_ROOM];
    RECT lines = { 0, 0, 0, 0 };
    int least = controls_row_height(app);
    int height;

    if (!CHECK(row.width > 0) || (SendMessageW(list, LB_GETTEXTLEN, (WPARAM)row.index, 0) >= ROW_TEXT_ROOM)) {
        return least;
    }
    (void)SendMessageW(list, LB_GETTEXT, (WPARAM)row.index, (LPARAM)text);
    lines.right = row.width;
    (void)DrawTextW(context, text, -1, &lines, DT_CALCRECT | DT_WORDBREAK | DT_NOPREFIX);
    height = lines.bottom + MulDiv(ROW_PAD, app->dpi, LAYOUT_BASE_DPI);
    height = (height < least) ? least : height;
    return (height > ROW_MOST) ? ROW_MOST : height;
}

static int row_text_width(const AppState *app, HWND list, bool boxed)
{
    RECT client = { 0, 0, 0, 0 };
    int taken = boxed ? (3 * TEXT_INSET) + BOX_SIZE : 2 * TEXT_INSET;
    int bar = ((GetWindowLongPtrW(list, GWL_STYLE) & WS_VSCROLL) != 0) ? 0 : GetSystemMetrics(SM_CXVSCROLL);

    (void)GetClientRect(list, &client);
    return client.right - bar - MulDiv(taken, app->dpi, LAYOUT_BASE_DPI);
}

void assist_wrap_rows(const AppState *app, ControlId control)
{
    WidgetKind kind = layout_spec(control).kind;
    RowPlace row = { 0, 0 };
    HWND list;
    HDC context;
    HGDIOBJ before;
    int count;

    if ((kind != WIDGET_LIST) && (kind != WIDGET_CHECKLIST)) {
        return;
    }
    list = app->controls[control];
    context = GetDC(list);
    before = SelectObject(context, app->font);
    count = (int)SendMessageW(list, LB_GETCOUNT, 0, 0);
    row.width = row_text_width(app, list, kind == WIDGET_CHECKLIST);
    for (row.index = 0; (row.index < count) && (row.index < LIST_ROW_CAPACITY); ++row.index) {
        (void)SendMessageW(list, LB_SETITEMHEIGHT, (WPARAM)row.index, (LPARAM)row_height(app, list, context, row));
    }
    (void)SelectObject(context, before);
    (void)ReleaseDC(list, context);
    (void)InvalidateRect(list, NULL, TRUE);
}

void assist_placed(const AppState *app)
{
    int control;

    for (control = 0; control < CONTROL_COUNT; ++control) {
        assist_wrap_rows(app, (ControlId)control);
    }
    (void)InvalidateRect(app->window, NULL, TRUE);
}

void assist_follow_focus(AppState *app)
{
    HWND focus = GetFocus();
    RECT ring;

    if (focus == app->framed) {
        return;
    }
    if (focus_ring(app, app->framed, &ring)) {
        (void)InvalidateRect(app->window, &ring, TRUE);
    }
    show_focus(app, focus);
    if (focus_ring(app, focus, &ring)) {
        (void)InvalidateRect(app->window, &ring, TRUE);
    }
    app->framed = focus;
}

void assist_draw_focus(const AppState *app, HDC context)
{
    RECT ring;
    int width;
    int drawn;

    if (!focus_ring(app, GetFocus(), &ring)) {
        return;
    }
    width = MulDiv(FRAME_WIDTH, app->dpi, LAYOUT_BASE_DPI);
    for (drawn = 0; (drawn < width) && (drawn < FRAME_LIMIT); ++drawn) {
        (void)FrameRect(context, &ring, app->chosen_brush);
        (void)InflateRect(&ring, -1, -1);
    }
}
