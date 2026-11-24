#include "dialog_tests.h"

#include <windows.h>

#include "../../src/win/win_parts.h"
#include "../accessibility/window_fixture.h"

enum { NOTCH = 120, ON_SCREEN = 0 };

HWND dropdown_shown_choice(void)
{
    HWND window = app_open(GetModuleHandleW(NULL));

    (void)SetWindowPos(window, NULL, ON_SCREEN, ON_SCREEN, 0, 0,
                       SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW);
    (void)UpdateWindow(window);
    return GetDlgItem(window, CONTROL_ID_BASE + CONTROL_CIV);
}

ULONGLONG dropdown_time_to_open(HWND choice)
{
    ULONGLONG start = GetTickCount64();

    (void)SendMessageW(choice, CB_SHOWDROPDOWN, TRUE, 0);
    return GetTickCount64() - start;
}

HWND dropdown_rows_of(HWND choice)
{
    COMBOBOXINFO parts = { 0 };

    parts.cbSize = sizeof parts;
    (void)GetComboBoxInfo(choice, &parts);
    return parts.hwndList;
}

void dropdown_wheel_down(HWND choice)
{
    (void)PostMessageW(choice, WM_MOUSEWHEEL, MAKEWPARAM(0, (WORD)-NOTCH), 0);
    PostQuitMessage(0);
    (void)app_run_messages();
}

bool dropdown_closed_with(HWND choice, bool result)
{
    (void)SendMessageW(choice, CB_SHOWDROPDOWN, FALSE, 0);
    window_fixture_close(GetParent(choice));
    return result;
}
