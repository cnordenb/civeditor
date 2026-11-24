#include "dialog_tests.h"

#include <windows.h>

#include "../../src/reflow/reflow.h"
#include "../../src/view/palette.h"
#include "../../src/win/win_parts.h"
#include "../accessibility/window_fixture.h"

enum { FAR_AWAY = -20000, SMALL_WIDTH = 480, SMALL_HEIGHT = 360, PAGE_LIMIT = 16 };

HWND scroll_shown_list(int place)
{
    HWND window = app_open(GetModuleHandleW(NULL));

    window_fixture_act(window, ACTION_FIRST_STEP + (int)layout_spec(CONTROL_TREE_LIST).step);
    (void)SetWindowPos(window, NULL, place, place, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW);
    (void)UpdateWindow(window);
    return GetDlgItem(window, CONTROL_ID_BASE + CONTROL_TREE_LIST);
}

bool scroll_closed_with(HWND list, bool result)
{
    window_fixture_close(GetParent(list));
    return result;
}

COLORREF scroll_erased_pixel(HWND list, int above_end)
{
    RECT client = { 0, 0, 0, 0 };
    HDC screen = GetDC(NULL);
    HDC memory = CreateCompatibleDC(screen);
    HBITMAP picture;
    HGDIOBJ before;
    COLORREF pixel;

    (void)GetClientRect(list, &client);
    picture = CreateCompatibleBitmap(screen, client.right, client.bottom + 1);
    before = SelectObject(memory, picture);
    (void)PatBlt(memory, 0, 0, client.right, client.bottom + 1, BLACKNESS);
    (void)SendMessageW(list, WM_ERASEBKGND, (WPARAM)memory, 0);
    pixel = GetPixel(memory, 0, client.bottom - above_end);
    (void)SelectObject(memory, before);
    (void)DeleteObject(picture);
    (void)DeleteDC(memory);
    (void)ReleaseDC(NULL, screen);
    return pixel;
}

void scroll_run_posted(void)
{
    PostQuitMessage(0);
    (void)app_run_messages();
}

HWND scroll_small_window(WizardStep step)
{
    HWND window = app_open(GetModuleHandleW(NULL));

    window_fixture_act(window, ACTION_FIRST_STEP + (int)step);
    (void)SetWindowPos(window, NULL, FAR_AWAY, FAR_AWAY, SMALL_WIDTH, SMALL_HEIGHT,
                       SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW);
    (void)UpdateWindow(window);
    return window;
}

static int fitting_height(HWND window, WizardStep step)
{
    RECT client = { 0, 0, 0, 0 };
    RECT frame = { 0, 0, 0, 0 };

    (void)GetClientRect(window, &client);
    (void)GetWindowRect(window, &frame);
    return frame.bottom - frame.top - client.bottom +
           MulDiv(reflow_height(step), assist_dpi(window), LAYOUT_BASE_DPI);
}

bool test_the_knob_of_the_windows_scroll_bar_reaches_its_end_with_the_step(void)
{
    HWND window = scroll_small_window(STEP_NAME_AND_TREE);
    HWND bar = GetDlgItem(window, WINDOW_BAR_ID);
    SCROLLINFO knob;
    int page;

    for (page = 0; page < PAGE_LIMIT; ++page) {
        (void)SendMessageW(window, WM_VSCROLL, SB_PAGEDOWN, (LPARAM)bar);
    }
    ZeroMemory(&knob, sizeof knob);
    knob.cbSize = sizeof knob;
    knob.fMask = SIF_ALL;
    (void)GetScrollInfo(bar, SB_CTL, &knob);
    window_fixture_close(window);
    return knob.nPos + (int)knob.nPage == knob.nMax + 1;
}

bool test_a_step_that_just_fits_the_window_shows_no_scroll_bar(void)
{
    HWND window = scroll_small_window(STEP_FINISH);
    bool hidden;

    (void)SetWindowPos(window, NULL, 0, 0, SMALL_WIDTH, fitting_height(window, STEP_FINISH),
                       SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
    hidden = !IsWindowVisible(GetDlgItem(window, WINDOW_BAR_ID));
    window_fixture_close(window);
    return hidden;
}

bool test_the_up_arrow_of_the_windows_scroll_bar_moves_the_step_up_a_line(void)
{
    HWND window = scroll_small_window(STEP_NAME_AND_TREE);
    HWND bar = GetDlgItem(window, WINDOW_BAR_ID);
    int paged;
    int lined;

    (void)SendMessageW(window, WM_VSCROLL, SB_PAGEDOWN, (LPARAM)bar);
    paged = GetScrollPos(bar, SB_CTL);
    (void)SendMessageW(window, WM_VSCROLL, SB_LINEUP, (LPARAM)bar);
    lined = GetScrollPos(bar, SB_CTL);
    window_fixture_close(window);
    return lined == paged - REFLOW_LINE;
}

bool test_a_key_in_a_list_moves_its_caret_one_row(void)
{
    HWND list = scroll_shown_list(FAR_AWAY);

    (void)SendMessageW(list, LB_SETCARETINDEX, 0, FALSE);
    (void)PostMessageW(list, WM_KEYDOWN, VK_DOWN, 0);
    scroll_run_posted();
    return scroll_closed_with(list, SendMessageW(list, LB_GETCARETINDEX, 0, 0) == 1);
}

bool test_the_room_under_the_last_row_of_a_list_is_painted_in_the_colour_of_fields(void)
{
    HWND list = scroll_shown_list(FAR_AWAY);
    Rgb field = palette_color(COLOR_ROLE_FIELD);
    COLORREF under = scroll_erased_pixel(GetDlgItem(GetParent(list), CONTROL_ID_BASE + CONTROL_STEPS), 1);

    return scroll_closed_with(list, under == RGB(field.red, field.green, field.blue));
}

bool test_a_list_full_of_rows_paints_nothing_under_itself(void)
{
    HWND list = scroll_shown_list(FAR_AWAY);

    return scroll_closed_with(list, scroll_erased_pixel(list, 0) == RGB(0, 0, 0));
}
