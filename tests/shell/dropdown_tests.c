#include "dialog_tests.h"

#include <windows.h>

#include "../../src/win/win_parts.h"
#include "../accessibility/window_fixture.h"

enum { NOTCH = 120, SLOWEST_MOVE = 150, ON_SCREEN = 0 };

static HWND shown_choice(void)
{
    HWND window = app_open(GetModuleHandleW(NULL));

    (void)SetWindowPos(window, NULL, ON_SCREEN, ON_SCREEN, 0, 0,
                       SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW);
    (void)UpdateWindow(window);
    return GetDlgItem(window, CONTROL_ID_BASE + CONTROL_CIV);
}

static ULONGLONG time_to_open(HWND choice)
{
    ULONGLONG start = GetTickCount64();

    (void)SendMessageW(choice, CB_SHOWDROPDOWN, TRUE, 0);
    return GetTickCount64() - start;
}

static HWND rows_of(HWND choice)
{
    COMBOBOXINFO parts = { 0 };

    parts.cbSize = sizeof parts;
    (void)GetComboBoxInfo(choice, &parts);
    return parts.hwndList;
}

static void wheel_down(HWND choice)
{
    (void)PostMessageW(choice, WM_MOUSEWHEEL, MAKEWPARAM(0, (WORD)-NOTCH), 0);
    PostQuitMessage(0);
    (void)app_run_messages();
}

static bool closed_with(HWND choice, bool result)
{
    (void)SendMessageW(choice, CB_SHOWDROPDOWN, FALSE, 0);
    window_fixture_close(GetParent(choice));
    return result;
}

bool test_a_dropdown_opens_without_animation(void)
{
    HWND choice = shown_choice();

    return closed_with(choice, time_to_open(choice) < SLOWEST_MOVE);
}

bool test_each_wheel_notch_moves_an_open_dropdown_one_row(void)
{
    HWND choice = shown_choice();

    (void)time_to_open(choice);
    wheel_down(choice);
    return closed_with(choice, SendMessageW(rows_of(choice), LB_GETTOPINDEX, 0, 0) == 1);
}
