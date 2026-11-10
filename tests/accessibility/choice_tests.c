#include "access_tests.h"

#include <windows.h>

#include "../../src/core/check.h"
#include "../../src/win/win_parts.h"
#include "window_fixture.h"

enum { PICKED_BASE_ROW = 2, GENERIC_ROW = 0 };

bool test_an_undone_choice_shows_the_designs_value_again(void)
{
    HWND window = app_open(GetModuleHandleW(NULL));
    HWND choice = GetDlgItem(window, CONTROL_ID_BASE + CONTROL_BASE);
    LRESULT shown = CB_ERR;

    if (CHECK(PICKED_BASE_ROW != GENERIC_ROW) && (window != NULL) && (choice != NULL)) {
        (void)SendMessageW(choice, CB_SETCURSEL, PICKED_BASE_ROW, 0);
        (void)SendMessageW(window, WM_COMMAND, MAKEWPARAM(CONTROL_ID_BASE + CONTROL_BASE, CBN_SELCHANGE),
                           (LPARAM)choice);
        (void)SendMessageW(window, WM_COMMAND, (WPARAM)(ACTION_ID_BASE + ACTION_UNDO), 0);
        shown = SendMessageW(choice, CB_GETCURSEL, 0, 0);
    }
    window_fixture_close(window);
    return shown == GENERIC_ROW;
}