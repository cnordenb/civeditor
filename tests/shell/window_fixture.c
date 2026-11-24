#include "../accessibility/window_fixture.h"

#include <wchar.h>

#include "../../src/core/check.h"
#include "../../src/shell/shell.h"
#include "../../src/win/win_parts.h"

enum { MESSAGE_DRAIN_LIMIT = 64, FAR_AWAY = -20000 };

static const wchar_t *answer_row;
static const wchar_t *answer_name;
static const wchar_t *answer_filter;
static HHOOK answer_hook;
static AskedDialog asked;
static bool refocus_list;
static bool toggle_check;

void window_fixture_close(HWND window)
{
    MSG message;
    int step;

    if (!CHECK(MESSAGE_DRAIN_LIMIT > 0) || (window == NULL)) {
        return;
    }
    (void)DestroyWindow(window);
    for (step = 0; step < MESSAGE_DRAIN_LIMIT; ++step) {
        if (PeekMessageW(&message, NULL, 0, 0, PM_REMOVE) == 0) {
            break;
        }
    }
}

void window_fixture_act(HWND window, int action)
{
    if (!CHECK(window != NULL) || !CHECK(action < ACTION_COUNT)) {
        return;
    }
    (void)SendMessageW(window, WM_COMMAND, (WPARAM)(ACTION_ID_BASE + action), 0);
}

void window_fixture_click(HWND window, int control)
{
    HWND button = GetDlgItem(window, CONTROL_ID_BASE + control);

    if (!CHECK(button != NULL)) {
        return;
    }
    (void)SendMessageW(window, WM_COMMAND, MAKEWPARAM(CONTROL_ID_BASE + control, BN_CLICKED), (LPARAM)button);
}

void window_fixture_tick(HWND window, int control)
{
    HWND box = GetDlgItem(window, CONTROL_ID_BASE + control);
    bool ticked = SendMessageW(box, BM_GETCHECK, 0, 0) == BST_CHECKED;

    (void)SendMessageW(box, BM_SETCHECK, ticked ? BST_UNCHECKED : BST_CHECKED, 0);
    window_fixture_click(window, control);
}

static void look_at(HWND list)
{
    asked.rows = SendMessageW(list, LB_GETCOUNT, 0, 0);
    asked.list_in_focus = GetFocus() == list;
    (void)GetWindowTextW(GetWindow(list, GW_HWNDPREV), asked.list_label, WINDOW_FIXTURE_LABEL_CAPACITY);
}

static void pick_row(HWND list, LRESULT row)
{
    if ((GetWindowLongPtrW(list, GWL_STYLE) & LBS_MULTIPLESEL) != 0) {
        (void)SendMessageW(list, LB_SETSEL, (WPARAM)(SendMessageW(list, LB_GETSEL, (WPARAM)row, 0) <= 0), row);
        return;
    }
    (void)SendMessageW(list, LB_SETCURSEL, (WPARAM)row, 0);
}

static void answer_check(HWND dialog)
{
    bool ticked = IsDlgButtonChecked(dialog, ASK_CHECK_ID) == BST_CHECKED;
    RECT check = { 0, 0, 0, 0 };
    RECT button = { 0, 0, 0, 0 };

    asked.check_above_buttons = (GetWindowRect(GetDlgItem(dialog, ASK_CHECK_ID), &check) != 0) &&
                                (GetWindowRect(GetDlgItem(dialog, IDOK), &button) != 0) &&
                                (check.bottom <= button.top);
    if (toggle_check) {
        (void)CheckDlgButton(dialog, ASK_CHECK_ID, ticked ? BST_UNCHECKED : BST_CHECKED);
    }
    toggle_check = false;
    (void)SetWindowPos(dialog, NULL, FAR_AWAY, FAR_AWAY, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
    (void)PostMessageW(dialog, WM_COMMAND, IDOK, 0);
}

static void answer(HWND dialog)
{
    HWND list = GetDlgItem(dialog, ASK_LIST_ID);
    LRESULT row = SendMessageW(list, LB_FINDSTRINGEXACT, (WPARAM)-1, (LPARAM)answer_row);

    if (list == NULL) {
        answer_check(dialog);
        return;
    }
    if (answer_filter != NULL) {
        (void)SetDlgItemTextW(dialog, ASK_FILTER_ID, answer_filter);
        row = SendMessageW(list, LB_FINDSTRINGEXACT, (WPARAM)-1, (LPARAM)answer_row);
        answer_filter = NULL;
    }
    look_at(list);
    pick_row(list, row);
    (void)SendMessageW(dialog, WM_COMMAND, MAKEWPARAM(ASK_LIST_ID, LBN_SELCHANGE), (LPARAM)list);
    if (answer_name != NULL) {
        (void)SetDlgItemTextW(dialog, ASK_ANSWER_ID, answer_name);
    }
    if (refocus_list) {
        (void)SetFocus(GetDlgItem(dialog, ASK_ANSWER_ID));
        (void)SetFocus(list);
        refocus_list = false;
    }
    (void)SetWindowPos(dialog, NULL, FAR_AWAY, FAR_AWAY, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
    (void)PostMessageW(dialog, WM_COMMAND, IDOK, 0);
}

static LRESULT CALLBACK answer_dialog(int code, WPARAM wparam, LPARAM lparam)
{
    const CWPRETSTRUCT *call = (const CWPRETSTRUCT *)lparam;

    if ((code == HC_ACTION) && (call->message == WM_INITDIALOG)) {
        (void)UnhookWindowsHookEx(answer_hook);
        answer(call->hwnd);
    }
    return CallNextHookEx(NULL, code, wparam, lparam);
}

void window_fixture_answer(const wchar_t *row, const wchar_t *name)
{
    const AskedDialog unseen = { -1, false, L"", false };

    asked = unseen;
    answer_row = row;
    answer_name = name;
    answer_hook = SetWindowsHookExW(WH_CALLWNDPROCRET, answer_dialog, NULL, GetCurrentThreadId());
    (void)CHECK(answer_hook != NULL);
}

void window_fixture_filter(const wchar_t *filter)
{
    answer_filter = filter;
}

void window_fixture_refocus(void)
{
    refocus_list = true;
}

void window_fixture_toggle(void)
{
    toggle_check = true;
}

const AskedDialog *window_fixture_asked(void)
{
    return &asked;
}

static bool shown_as(HWND row, const wchar_t *text)
{
    wchar_t shown[BONUS_TEXT_CAPACITY] = L"";

    return (row != NULL) && ((GetWindowLongPtrW(row, GWL_STYLE) & WS_VISIBLE) != 0) &&
           (GetWindowTextW(row, shown, BONUS_TEXT_CAPACITY) > 0) && (wcsncmp(shown, text, wcslen(text)) == 0);
}

HWND window_fixture_panel_row(HWND window, int panel, const wchar_t *text)
{
    HWND holder = GetDlgItem(window, CONTROL_ID_BASE + panel);
    HWND row;
    int pick;

    for (pick = 0; pick < CIV_BONUS_CAPACITY; ++pick) {
        row = GetDlgItem(holder, CHOSEN_ROW_ID_BASE + pick);
        if (shown_as(row, text)) {
            return row;
        }
    }
    return NULL;
}

HWND window_fixture_panel_field(HWND window, int panel, const wchar_t *text, int number)
{
    HWND row = window_fixture_panel_row(window, panel, text);

    return (row == NULL) ? NULL
                         : GetDlgItem(GetParent(row), chosen_field_id(GetDlgCtrlID(row) - CHOSEN_ROW_ID_BASE, number));
}

HWND window_fixture_row(HWND window, const wchar_t *text)
{
    return window_fixture_panel_row(window, CONTROL_BONUS_INFO, text);
}

HWND window_fixture_field(HWND window, const wchar_t *text, int number)
{
    return window_fixture_panel_field(window, CONTROL_BONUS_INFO, text, number);
}
