#include "win_parts.h"

#include <oleacc.h>

#include "../core/check.h"

enum { LIVE_POLITE = 1, TEXT_PERCENT_PLAIN = 100, FRAME_REACH = 3, FRAME_WIDTH = 2, FRAME_LIMIT = 64 };

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
        dialogs_status(app, wizard_help(app->step));
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