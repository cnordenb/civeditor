#include "win_parts.h"

#include <oleacc.h>
#include <uxtheme.h>
#include <windowsx.h>

#include "../core/check.h"
#include "../shell/shell.h"

enum { LIVE_POLITE = 1, TEXT_PERCENT_PLAIN = 100, FRAME_REACH = 3, FRAME_WIDTH = 2, FRAME_LIMIT = 64, NESTING = 3 };
enum { WHEEL_LINES = 3, BAR_MOVES = 4, ROW_TEXT_ROOM = 512, ROW_MOST = 255 };

typedef struct {
    int index;
    int width;
} RowPlace;

static IAccPropServices *open_service(void)
{
    static const CLSID SERVICE = { 0xb5f8350bUL, 0x0548, 0x48b1, { 0xa6, 0xee, 0x88, 0xbd, 0x00, 0xb4, 0xa5, 0xe7 } };
    static const IID PROPERTIES = { 0x6e26e776UL, 0x04f0, 0x495d, { 0x80, 0xe4, 0x33, 0x30, 0x35, 0x2e, 0x31, 0x69 } };
    void *object = NULL;

    return (CoCreateInstance(&SERVICE, NULL, CLSCTX_INPROC_SERVER, &PROPERTIES, &object) == S_OK) ? object : NULL;
}

void assist_annotate(HWND control, AssistProperty property, const wchar_t *text)
{
    static const GUID PROPERTIES_OF[ASSIST_PROPERTY_COUNT] = {
        { 0x608d3df8UL, 0x8128, 0x4aa7, { 0xa4, 0x28, 0xf5, 0x5e, 0x49, 0x26, 0x72, 0x91 } },
        { 0x123fe443UL, 0x211a, 0x4615, { 0x95, 0x27, 0xc4, 0x5a, 0x7e, 0x93, 0x71, 0x7a } }
    };
    IAccPropServices *service = open_service();

    if (service == NULL) {
        return;
    }
    (void)IAccPropServices_SetHwndPropStr(service, control, (DWORD)OBJID_CLIENT, CHILDID_SELF, PROPERTIES_OF[property],
                                          text);
    (void)IAccPropServices_Release(service);
}

void assist_mark_live(HWND status)
{
    static const GUID LIVE = { 0xc12bcd8eUL, 0x2a8e, 0x4950, { 0x8a, 0xe7, 0x36, 0x25, 0x11, 0x1d, 0x58, 0xeb } };
    IAccPropServices *service = open_service();
    VARIANT polite;

    if (service == NULL) {
        return;
    }
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

int assist_scaled(const AppState *app, int units)
{
    return MulDiv(units, app->dpi, LAYOUT_BASE_DPI);
}

void assist_move_focus(AppState *app, WPARAM wparam, LPARAM lparam)
{
    HWND target;

    target = (LOWORD(lparam) != 0) ? (HWND)wparam : GetNextDlgTabItem(app->window, GetFocus(), wparam != 0);
    if (target != NULL) {
        (void)SetFocus(target);
    }
}

static int holding_control(HWND window)
{
    HWND at = window;
    int control;
    int depth;

    for (depth = 0; (depth < NESTING) && (at != NULL); ++depth) {
        control = GetDlgCtrlID(at) - CONTROL_ID_BASE;
        if ((control >= 0) && (control < CONTROL_COUNT)) {
            return control;
        }
        at = GetParent(at);
    }
    return -1;
}

void assist_show_help(AppState *app)
{
    int control = holding_control(GetFocus());

    if ((control >= 0) && (layout_help((ControlId)control)[0] != '\0')) {
        dialogs_status(app, layout_help((ControlId)control));
    } else {
        dialogs_status(app, wizard_page(app->step)->help);
    }
}

static bool focus_ring(const AppState *app, HWND control, RECT *ring)
{
    if ((control == NULL) || (GetParent(control) != app->window)) {
        return false;
    }
    if (GetWindowRect(control, ring) == 0) {
        return false;
    }
    (void)MapWindowPoints(NULL, app->window, (POINT *)ring, 2);
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

int assist_bar_target(HWND window, int bar, BarMove move)
{
    static const int MOVES[BAR_MOVES] = { -1, 1, -1, 1 };
    SCROLLINFO track;

    if (move.code > SB_THUMBTRACK) {
        return move.scroll;
    }
    if (move.code < BAR_MOVES) {
        return move.scroll + (MOVES[move.code] * ((move.code < SB_PAGEUP) ? move.line : move.view));
    }
    track.cbSize = sizeof track;
    track.fMask = SIF_TRACKPOS;
    track.nTrackPos = move.scroll;
    (void)GetScrollInfo(window, bar, &track);
    return track.nTrackPos;
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
        BarMove move = { (int)LOWORD(wparam), app->scroll, REFLOW_LINE, view };

        scroll_to(app, assist_bar_target(app->bar, SB_CTL, move));
    }
}

void assist_run_message(AppState *app, MSG *message)
{
    HWND dialog = (GetActiveWindow() == app->popup) ? app->popup : app->window;

    if (!chosen_take_message(app, message) && !list_scroll_takes(message) &&
        (TranslateAcceleratorW(app->window, app->accelerators, message) == 0) &&
        (IsDialogMessageW(dialog, message) == 0)) {
        (void)TranslateMessage(message);
        (void)DispatchMessageW(message);
    }
    chosen_follow_focus(app);
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

    if ((focus == NULL) || (IsChild(app->window, focus) == 0) || (GetWindowRect(focus, &place) == 0)) {
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
    area->right -= assist_scaled(app, TEXT_INSET);
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
    height = lines.bottom + assist_scaled(app, ROW_PAD);
    height = (height < least) ? least : height;
    return (height > ROW_MOST) ? ROW_MOST : height;
}

static int kept_height(AppState *app, ControlId control, HDC context, RowPlace row)
{
    const ListModel *model = &app->lists[control];
    short *kept = (row.index < model->count) ? &app->wrapped[control].heights[model->items[row.index]] : NULL;
    int height;

    if ((kept != NULL) && (*kept > 0)) {
        return *kept;
    }
    height = row_height(app, app->controls[control], context, row);
    if (kept != NULL) {
        *kept = (short)height;
    }
    return height;
}

static int row_text_width(const AppState *app, HWND list, bool boxed)
{
    RECT client = { 0, 0, 0, 0 };
    int taken = boxed ? (3 * TEXT_INSET) + BOX_SIZE : 2 * TEXT_INSET;
    int bar = ((GetWindowLongPtrW(list, GWL_STYLE) & WS_VSCROLL) != 0) ? 0 : GetSystemMetrics(SM_CXVSCROLL);

    (void)GetClientRect(list, &client);
    return client.right - bar - assist_scaled(app, taken);
}

void assist_wrap_rows(AppState *app, ControlId control)
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
    if (app->wrapped[control].width != row.width) {
        (void)memset(&app->wrapped[control], 0, sizeof app->wrapped[control]);
        app->wrapped[control].width = row.width;
    }
    (void)SendMessageW(list, WM_SETREDRAW, FALSE, 0);
    for (row.index = 0; (row.index < count) && (row.index < LIST_ROW_CAPACITY); ++row.index) {
        (void)SendMessageW(list, LB_SETITEMHEIGHT, (WPARAM)row.index, (LPARAM)kept_height(app, control, context, row));
    }
    (void)SendMessageW(list, WM_SETREDRAW, TRUE, 0);
    (void)SelectObject(context, before);
    (void)ReleaseDC(list, context);
    (void)InvalidateRect(list, NULL, TRUE);
}

void assist_forget_rows(AppState *app)
{
    int control;

    for (control = 0; control < CONTROL_COUNT; ++control) {
        app->wrapped[control].width = 0;
    }
}

static bool is_wrapped(const AppState *app, ControlId control)
{
    WidgetKind kind = layout_spec(control).kind;

    return app->wrapped[control].width == row_text_width(app, app->controls[control], kind == WIDGET_CHECKLIST);
}

void assist_placed(AppState *app)
{
    int control;

    for (control = 0; control < CONTROL_COUNT; ++control) {
        if (layout_shows((ControlId)control, app->step) && !is_wrapped(app, (ControlId)control)) {
            assist_wrap_rows(app, (ControlId)control);
        }
    }
    (void)InvalidateRect(app->window, NULL, TRUE);
}

static void redraw_ring(const AppState *app, HWND control)
{
    RECT ring;
    int reach = assist_scaled(app, FRAME_REACH);

    if (focus_ring(app, control, &ring)) {
        (void)InflateRect(&ring, reach, reach);
        (void)InvalidateRect(app->window, &ring, TRUE);
    }
}

void assist_follow_focus(AppState *app)
{
    HWND focus = GetFocus();

    chosen_follow_focus(app);
    if (focus == app->framed) {
        return;
    }
    redraw_ring(app, app->framed);
    show_focus(app, focus);
    redraw_ring(app, focus);
    app->framed = focus;
}

void assist_draw_ring(const AppState *app, HDC context, RECT control)
{
    RECT ring = control;
    int reach = assist_scaled(app, FRAME_REACH);
    int width = assist_scaled(app, FRAME_WIDTH);
    int drawn;

    (void)InflateRect(&ring, reach, reach);
    for (drawn = 0; (drawn < width) && (drawn < FRAME_LIMIT); ++drawn) {
        (void)FrameRect(context, &ring, app->chosen_brush);
        (void)InflateRect(&ring, -1, -1);
    }
}

void assist_draw_focus(const AppState *app, HDC context)
{
    RECT place;

    if (focus_ring(app, GetFocus(), &place)) {
        assist_draw_ring(app, context, place);
    }
}
