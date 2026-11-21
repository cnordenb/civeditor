#include "shell.h"

#include <windowsx.h>

#include "../win/win_parts.h"

enum { REPEAT_FIRST = 400, REPEAT_NEXT = 50, CLASS_ROOM = 16 };

typedef struct {
    int wheel_rest;
    HWND list;
    HWND owner;
    int rows;
} HeldList;

static HeldList held;

static bool is_kind(HWND window, const wchar_t *wanted)
{
    wchar_t kind[CLASS_ROOM] = L"";

    (void)GetClassNameW(window, kind, CLASS_ROOM);
    return lstrcmpiW(kind, wanted) == 0;
}

static bool is_list(HWND window)
{
    return is_kind(window, L"LISTBOX");
}

static HWND rows_of(HWND choice)
{
    COMBOBOXINFO parts;

    parts.cbSize = sizeof parts;
    return (GetComboBoxInfo(choice, &parts) != 0) ? parts.hwndList : NULL;
}

static HWND wheeled_list(HWND window)
{
    if (is_list(window) || is_kind(window, L"ComboLBox")) {
        return window;
    }
    return (SendMessageW(window, CB_GETDROPPEDSTATE, 0, 0) != 0) ? rows_of(window) : NULL;
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

static void turn_wheel(const MSG *message, HWND list)
{
    int rows;

    held.wheel_rest += GET_WHEEL_DELTA_WPARAM(message->wParam);
    rows = held.wheel_rest / WHEEL_DELTA;
    held.wheel_rest %= WHEEL_DELTA;
    move_list(list, -rows);
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

static bool press_arrow(const MSG *message)
{
    int rows = arrow_rows(message->hwnd, GET_Y_LPARAM(message->lParam));

    if (rows != 0) {
        move_list(message->hwnd, rows);
        held.list = message->hwnd;
        held.owner = GetAncestor(message->hwnd, GA_ROOT);
        held.rows = rows;
        (void)SetCapture(message->hwnd);
        (void)SetTimer(held.owner, LIST_REPEAT_TIMER, REPEAT_FIRST, NULL);
    }
    return rows != 0;
}

static bool holds_arrow(void)
{
    return GetCapture() == held.list;
}

static void repeat_arrow(void)
{
    if (!holds_arrow()) {
        (void)KillTimer(held.owner, LIST_REPEAT_TIMER);
        return;
    }
    move_list(held.list, held.rows);
    (void)SetTimer(held.owner, LIST_REPEAT_TIMER, REPEAT_NEXT, NULL);
}

static void follow_arrow(const MSG *message)
{
    if ((message->message == WM_LBUTTONUP) && holds_arrow()) {
        (void)KillTimer(held.owner, LIST_REPEAT_TIMER);
        (void)ReleaseCapture();
    }
    if ((message->message == WM_TIMER) && (message->wParam == LIST_REPEAT_TIMER) && (message->hwnd == held.owner)) {
        repeat_arrow();
    }
}

static bool is_arrow_press(const MSG *message)
{
    return ((message->message == WM_NCLBUTTONDOWN) || (message->message == WM_NCLBUTTONDBLCLK)) &&
           (message->wParam == HTVSCROLL) && is_list(message->hwnd);
}

bool list_scroll_takes(const MSG *message)
{
    HWND list = (message->message == WM_MOUSEWHEEL) ? wheeled_list(message->hwnd) : NULL;

    follow_arrow(message);
    if (list != NULL) {
        turn_wheel(message, list);
        return true;
    }
    return is_arrow_press(message) && press_arrow(message);
}

void list_scroll_open(HWND choice)
{
    (void)ShowWindow(rows_of(choice), SW_SHOWNA);
}
