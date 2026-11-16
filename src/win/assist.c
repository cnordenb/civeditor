#include "win_parts.h"

#include <oleacc.h>

#include "../core/check.h"

enum { LIVE_POLITE = 1, TEXT_PERCENT_PLAIN = 100, FRAME_REACH = 3, FRAME_WIDTH = 2, FRAME_LIMIT = 64 };
enum { WHEEL_LINES = 3, BAR_MOVES = 4, ROW_TEXT_ROOM = 512, ROW_PAD = 12, ROW_MOST = 255 };

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

    if (!CHECK(status != NULL) ||
        (CoCreateInstance(&SERVICE, NULL, CLSCTX_INPROC_SERVER, &PROPERTIES, &object) != S_OK)) {
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

    if (!CHECK(size == sizeof(DWORD)) || !CHECK(TEXT_PERCENT_PLAIN > 0)) {
        return TEXT_PERCENT_PLAIN;
    }
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
    if (!CHECK(dpi > 0) || !CHECK(LAYOUT_BASE_DPI > 0)) {
        return LAYOUT_BASE_DPI;
    }
    return layout_text_dpi(dpi, text_percent());
}

void assist_move_focus(AppState *app, WPARAM wparam, LPARAM lparam)
{
    HWND target;

    if (!CHECK(app != NULL) || !CHECK(app->window != NULL)) {
        return;
    }
    target = (LOWORD(lparam) != 0) ? (HWND)wparam : GetNextDlgTabItem(app->window, GetFocus(), wparam != 0);
    if (target != NULL) {
        (void)SetFocus(target);
    }
}

void assist_show_help(AppState *app)
{
    int control = GetDlgCtrlID(GetFocus()) - CONTROL_ID_BASE;

    if (!CHECK(app != NULL) || !CHECK(app->step < STEP_COUNT)) {
        return;
    }
    if ((control >= 0) && (control < CONTROL_COUNT) && (layout_help((ControlId)control)[0] != '\0')) {
        dialogs_status(app, layout_help((ControlId)control));
    } else {
        dialogs_status(app, wizard_page(app->step)->help);
    }
}

static bool focus_ring(const AppState *app, HWND control, RECT *ring)
{
    int reach;

    if (!CHECK(app != NULL) || !CHECK(ring != NULL) || (control == NULL) || (GetParent(control) != app->window)) {
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
    RECT client = { 0, 0, 0, 0 };

    if (!CHECK(app != NULL) || !CHECK(app->dpi > 0)) {
        return 1;
    }
    (void)GetClientRect(app->window, &client);
    return MulDiv(client.bottom, LAYOUT_BASE_DPI, app->dpi);
}

int assist_scroll_range(AppState *app, int content, int view)
{
    SCROLLINFO bar;

    if (!CHECK(app != NULL) || !CHECK(content >= 0) || !CHECK(view >= 0)) {
        return 0;
    }
    bar.cbSize = sizeof bar;
    bar.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
    bar.nMin = 0;
    bar.nMax = content - 1;
    bar.nPage = (UINT)view;
    bar.nPos = reflow_scroll_within(app->scroll, content, view);
    bar.nTrackPos = 0;
    (void)SetScrollInfo(app->window, SB_VERT, &bar, TRUE);
    return bar.nPos;
}

static int bar_target(const AppState *app, int code, int view)
{
    static const int MOVES[BAR_MOVES] = { -REFLOW_LINE, REFLOW_LINE, -1, 1 };
    SCROLLINFO bar;

    if (!CHECK(app != NULL) || !CHECK(code >= 0) || (code > SB_THUMBTRACK)) {
        return app->scroll;
    }
    if (code < BAR_MOVES) {
        return app->scroll + (MOVES[code] * ((code < SB_PAGEUP) ? 1 : view));
    }
    bar.cbSize = sizeof bar;
    bar.fMask = SIF_TRACKPOS;
    bar.nTrackPos = app->scroll;
    (void)GetScrollInfo(app->window, SB_VERT, &bar);
    return bar.nTrackPos;
}

void assist_scroll(AppState *app, UINT message, WPARAM wparam)
{
    int view = view_height(app);

    if (!CHECK(app != NULL) || !CHECK(view > 0)) {
        return;
    }
    if (message == WM_MOUSEWHEEL) {
        app->scroll -= MulDiv(GET_WHEEL_DELTA_WPARAM(wparam), WHEEL_LINES * REFLOW_LINE, WHEEL_DELTA);
    } else {
        app->scroll = bar_target(app, (int)LOWORD(wparam), view);
    }
    controls_place(app);
}

static void show_focus(AppState *app, HWND focus)
{
    Box box = { 0, 0, 0, 0 };
    RECT place;
    int target;

    if (!CHECK(app != NULL) || !CHECK(app->dpi > 0) || (focus == NULL) || (GetParent(focus) != app->window) ||
        (GetWindowRect(focus, &place) == 0)) {
        return;
    }
    (void)MapWindowPoints(NULL, app->window, (POINT *)&place, 2);
    box.top = MulDiv(place.top, LAYOUT_BASE_DPI, app->dpi) + app->scroll;
    box.height = MulDiv(place.bottom - place.top, LAYOUT_BASE_DPI, app->dpi);
    target = reflow_scroll_to_show(box, app->scroll, view_height(app));
    if (target != app->scroll) {
        app->scroll = target;
        controls_place(app);
    }
}

UINT assist_row_format(const AppState *app, const DRAWITEMSTRUCT *item, const wchar_t *text, RECT *area)
{
    RECT lines;

    if (!CHECK(app != NULL) || !CHECK(area != NULL) || (item->CtlType == ODT_COMBOBOX)) {
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

    if (!CHECK(app != NULL) || !CHECK(row.width > 0) ||
        (SendMessageW(list, LB_GETTEXTLEN, (WPARAM)row.index, 0) >= ROW_TEXT_ROOM)) {
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

    if (!CHECK(app != NULL) || !CHECK(app->dpi > 0)) {
        return 1;
    }
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

    if (!CHECK(app != NULL) || !CHECK(control < CONTROL_COUNT) ||
        ((kind != WIDGET_LIST) && (kind != WIDGET_CHECKLIST))) {
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

    if (!CHECK(app != NULL) || !CHECK(app->window != NULL)) {
        return;
    }
    for (control = 0; control < CONTROL_COUNT; ++control) {
        assist_wrap_rows(app, (ControlId)control);
    }
    (void)InvalidateRect(app->window, NULL, TRUE);
}

void assist_follow_focus(AppState *app)
{
    HWND focus = GetFocus();
    RECT ring;

    if (!CHECK(app != NULL) || !CHECK(app->window != NULL) || (focus == app->framed)) {
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

    if (!CHECK(app != NULL) || !CHECK(context != NULL) || !focus_ring(app, GetFocus(), &ring)) {
        return;
    }
    width = MulDiv(FRAME_WIDTH, app->dpi, LAYOUT_BASE_DPI);
    for (drawn = 0; (drawn < width) && (drawn < FRAME_LIMIT); ++drawn) {
        (void)FrameRect(context, &ring, app->chosen_brush);
        (void)InflateRect(&ring, -1, -1);
    }
}
