#include "../win/win_parts.h"

#include "../core/check.h"

enum {
    POPUP_STYLE = WS_POPUP | WS_CAPTION | WS_SYSMENU,
    POPUP_EXTRA_STYLE = WS_EX_CONTROLPARENT | WS_EX_DLGMODALFRAME,
    FORWARDED_COUNT = 7
};

const wchar_t POPUP_CLASS_NAME[] = L"civeditor.popup";
static const int FORWARDED[FORWARDED_COUNT] = { WM_COMMAND,        WM_DRAWITEM,     WM_MEASUREITEM, WM_CTLCOLORSTATIC,
                                                WM_CTLCOLORLISTBOX, WM_CTLCOLOREDIT, WM_CTLCOLORBTN };

static AppState *owner_of(HWND popup)
{
    LONG_PTR kept = GetWindowLongPtrW(popup, GWLP_USERDATA);

    return (AppState *)kept;
}

static LRESULT paint_paper(HWND popup, HDC context)
{
    RECT client;

    (void)GetClientRect(popup, &client);
    (void)FillRect(context, &client, owner_of(popup)->paper_brush);
    return 1;
}

static bool is_closing(UINT message, WPARAM wparam)
{
    return (message == WM_CLOSE) || ((message == WM_COMMAND) && (LOWORD(wparam) == IDCANCEL));
}

LRESULT CALLBACK popup_procedure(HWND popup, UINT message, WPARAM wparam, LPARAM lparam)
{
    if (message == WM_NCCREATE) {
        (void)SetWindowLongPtrW(popup, GWLP_USERDATA, (LONG_PTR)((const CREATESTRUCTW *)lparam)->lpCreateParams);
    } else if (is_closing(message, wparam)) {
        popup_show(owner_of(popup), false);
        return 0;
    } else if (check_is_among((int)message, FORWARDED, FORWARDED_COUNT)) {
        return SendMessageW(owner_of(popup)->window, message, wparam, lparam);
    } else if (message == WM_ERASEBKGND) {
        return paint_paper(popup, (HDC)wparam);
    }
    return DefWindowProcW(popup, message, wparam, lparam);
}

bool popup_make(AppState *app)
{
    app->popup = CreateWindowExW(POPUP_EXTRA_STYLE, POPUP_CLASS_NAME, L"Appearance", POPUP_STYLE, 0, 0, 0, 0,
                                 app->window, NULL, app->instance, app);
    return app->popup != NULL;
}

static void place_popup(const AppState *app)
{
    RECT frame = { 0, 0, 0, 0 };
    RECT over;

    frame.right = MulDiv(LAYOUT_POPUP_WIDTH, app->dpi, LAYOUT_BASE_DPI);
    frame.bottom = MulDiv(LAYOUT_POPUP_HEIGHT, app->dpi, LAYOUT_BASE_DPI);
    (void)AdjustWindowRectEx(&frame, POPUP_STYLE, FALSE, POPUP_EXTRA_STYLE);
    (void)GetWindowRect(app->window, &over);
    (void)SetWindowPos(app->popup, NULL, over.left + ((over.right - over.left - (frame.right - frame.left)) / 2),
                       over.top + ((over.bottom - over.top - (frame.bottom - frame.top)) / 2),
                       frame.right - frame.left, frame.bottom - frame.top, SWP_NOZORDER | SWP_NOACTIVATE);
}

void popup_show(AppState *app, bool shown)
{
    if (shown) {
        place_popup(app);
    }
    (void)ShowWindow(app->popup, shown ? SW_SHOW : SW_HIDE);
    (void)SetFocus(app->controls[shown ? CONTROL_ARCHITECTURE : CONTROL_APPEARANCE]);
}
