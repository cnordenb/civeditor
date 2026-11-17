#include "dialog_tests.h"

#include <windows.h>

#include "../../src/reflow/reflow.h"
#include "../../src/win/win_parts.h"
#include "../accessibility/window_fixture.h"

enum {
    NOTCH = 120,
    SLOWEST_MOVE = 150,
    LEAST_COLOURS = 4,
    COLOUR_ROOM = 8,
    FAR_AWAY = -20000,
    ON_SCREEN = 0,
    START_ROW = 2,
    SMALL_WIDTH = 480,
    SMALL_HEIGHT = 360,
    PAGE_LIMIT = 16
};

typedef enum {
    MOVE_WHEEL,
    MOVE_WHEEL_AGAIN,
    MOVE_UP_ARROW,
    MOVE_UP_ARROW_TWICE,
    MOVE_COUNT
} Move;

static HWND shown_list(int place)
{
    HWND window = app_open(GetModuleHandleW(NULL));

    window_fixture_act(window, ACTION_FIRST_STEP + STEP_TECH_TREE);
    (void)SetWindowPos(window, NULL, place, place, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW);
    (void)UpdateWindow(window);
    return GetDlgItem(window, CONTROL_ID_BASE + CONTROL_TREE_LIST);
}

static HWND scrolled_to(HWND list, LRESULT row)
{
    (void)SendMessageW(list, LB_SETTOPINDEX, (WPARAM)row, 0);
    return list;
}

static HWND list_at_row(LRESULT row)
{
    return scrolled_to(shown_list(FAR_AWAY), row);
}

static HWND list_at_end(void)
{
    HWND list = shown_list(FAR_AWAY);

    return scrolled_to(list, SendMessageW(list, LB_GETCOUNT, 0, 0) - 1);
}

static bool closed_with(HWND list, bool result)
{
    window_fixture_close(GetParent(list));
    return result;
}

static void run_posted(void)
{
    PostQuitMessage(0);
    (void)app_run_messages();
}

static LRESULT top_row(HWND list)
{
    return SendMessageW(list, LB_GETTOPINDEX, 0, 0);
}

static void turn_wheel(HWND list, int delta)
{
    (void)PostMessageW(list, WM_MOUSEWHEEL, MAKEWPARAM(0, (WORD)delta), 0);
}

static SCROLLBARINFO bar_of(HWND list)
{
    SCROLLBARINFO bar = { 0 };

    bar.cbSize = sizeof bar;
    (void)GetScrollBarInfo(list, OBJID_VSCROLL, &bar);
    return bar;
}

static int up_arrow_edge(HWND list)
{
    SCROLLBARINFO bar = bar_of(list);

    return bar.rcScrollBar.top + bar.dxyLineButton - 1;
}

static int down_arrow_edge(HWND list)
{
    SCROLLBARINFO bar = bar_of(list);

    return bar.rcScrollBar.bottom - bar.dxyLineButton;
}

static void press_bar(HWND list, UINT press, int y)
{
    SCROLLBARINFO bar = bar_of(list);

    (void)PostMessageW(list, press, HTVSCROLL, MAKELPARAM((bar.rcScrollBar.left + bar.rcScrollBar.right) / 2, y));
}

static void click_bar(HWND list, UINT press, int y)
{
    press_bar(list, press, y);
    (void)PostMessageW(list, WM_LBUTTONUP, 0, 0);
    (void)PostMessageW(GetParent(list), WM_TIMER, LIST_REPEAT_TIMER, 0);
}

static int with_colour(COLORREF *seen, int count, COLORREF colour)
{
    int index;

    for (index = 0; index < count; ++index) {
        if (seen[index] == colour) {
            return count;
        }
    }
    if (count < COLOUR_ROOM) {
        seen[count] = colour;
        return count + 1;
    }
    return count;
}

static int colours_in_corner(HWND window, bool inside)
{
    COLORREF seen[COLOUR_ROOM];
    RECT place;
    HDC screen = GetDC(window);
    HDC memory = CreateCompatibleDC(screen);
    int size = GetSystemMetrics(SM_CXVSCROLL);
    int count = 0;
    HBITMAP picture;
    HGDIOBJ before;
    int across;
    int down;

    (void)(inside ? GetClientRect(window, &place) : GetWindowRect(window, &place));
    picture = CreateCompatibleBitmap(screen, place.right - place.left, place.bottom - place.top);
    before = SelectObject(memory, picture);
    if (inside) {
        (void)PrintWindow(window, memory, PW_CLIENTONLY);
    } else {
        (void)SendMessageW(window, WM_PRINT, (WPARAM)memory, PRF_NONCLIENT | PRF_CLIENT | PRF_ERASEBKGND);
    }
    for (down = 1; down <= size; ++down) {
        for (across = 1; across <= size; ++across) {
            count = with_colour(seen, count, GetPixel(memory, place.right - place.left - 1 - across, down));
        }
    }
    (void)SelectObject(memory, before);
    (void)DeleteObject(picture);
    (void)DeleteDC(memory);
    (void)ReleaseDC(window, screen);
    return count;
}

static HWND small_window(WizardStep step)
{
    HWND window = app_open(GetModuleHandleW(NULL));

    window_fixture_act(window, ACTION_FIRST_STEP + (int)step);
    (void)SetWindowPos(window, NULL, FAR_AWAY, FAR_AWAY, SMALL_WIDTH, SMALL_HEIGHT,
                       SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW);
    (void)UpdateWindow(window);
    return window;
}

static RECT place_in(HWND window, int identifier)
{
    RECT place = { 0, 0, 0, 0 };

    (void)GetWindowRect(GetDlgItem(window, identifier), &place);
    (void)MapWindowPoints(NULL, window, (POINT *)&place, 2);
    return place;
}

bool test_a_lists_scroll_bar_shows_its_arrows_while_the_mouse_is_away(void)
{
    HWND list = shown_list(FAR_AWAY);

    return closed_with(list, colours_in_corner(list, false) >= LEAST_COLOURS);
}

bool test_the_windows_scroll_bar_shows_its_arrows_while_the_mouse_is_away(void)
{
    HWND window = small_window(STEP_TECH_TREE);
    int colours = colours_in_corner(window, true);

    window_fixture_close(window);
    return colours >= LEAST_COLOURS;
}

bool test_a_step_ends_a_margin_from_the_windows_scroll_bar(void)
{
    HWND window = small_window(STEP_TECH_TREE);
    RECT back = place_in(window, CONTROL_ID_BASE + CONTROL_BACK);
    RECT next = place_in(window, CONTROL_ID_BASE + CONTROL_NEXT);
    RECT bar = place_in(window, WINDOW_BAR_ID);
    int gap = bar.left - next.right;

    window_fixture_close(window);
    return (gap >= back.left - 1) && (gap <= back.left + 1);
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
    HWND window = small_window(STEP_TECH_TREE);
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
    HWND window = small_window(STEP_HERO);
    bool hidden;

    (void)SetWindowPos(window, NULL, 0, 0, SMALL_WIDTH, fitting_height(window, STEP_HERO),
                       SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
    hidden = !IsWindowVisible(GetDlgItem(window, WINDOW_BAR_ID));
    window_fixture_close(window);
    return hidden;
}

bool test_the_up_arrow_of_the_windows_scroll_bar_moves_the_step_up_a_line(void)
{
    HWND window = small_window(STEP_TECH_TREE);
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

bool test_each_wheel_notch_moves_a_list_one_row_its_way(void)
{
    HWND list = list_at_row(START_ROW);

    turn_wheel(list, NOTCH);
    turn_wheel(list, -NOTCH);
    turn_wheel(list, -NOTCH);
    run_posted();
    return closed_with(list, top_row(list) == START_ROW + 1);
}

bool test_a_press_on_either_arrow_moves_a_list_one_row_its_way(void)
{
    HWND list = list_at_row(START_ROW);

    click_bar(list, WM_NCLBUTTONDOWN, up_arrow_edge(list));
    click_bar(list, WM_NCLBUTTONDOWN, down_arrow_edge(list));
    click_bar(list, WM_NCLBUTTONDBLCLK, down_arrow_edge(list));
    run_posted();
    return closed_with(list, top_row(list) == START_ROW + 1);
}

static void post_move(HWND list, Move move)
{
    if (move <= MOVE_WHEEL_AGAIN) {
        turn_wheel(list, -NOTCH);
        return;
    }
    press_bar(list, (move == MOVE_UP_ARROW) ? WM_NCLBUTTONDOWN : WM_NCLBUTTONDBLCLK, up_arrow_edge(list));
    (void)PostMessageW(list, WM_LBUTTONUP, 0, 0);
}

bool test_a_shown_list_scrolls_without_animation(void)
{
    HWND list = shown_list(ON_SCREEN);
    ULONGLONG slowest = 0;
    ULONGLONG start;
    ULONGLONG took;
    int move;

    for (move = 0; move < MOVE_COUNT; ++move) {
        start = GetTickCount64();
        post_move(list, (Move)move);
        run_posted();
        took = GetTickCount64() - start;
        slowest = (took > slowest) ? took : slowest;
    }
    return closed_with(list, slowest < SLOWEST_MOVE);
}

bool test_a_held_arrow_keeps_scrolling_a_list(void)
{
    HWND list = shown_list(FAR_AWAY);

    press_bar(list, WM_NCLBUTTONDOWN, down_arrow_edge(list));
    (void)PostMessageW(GetParent(list), WM_TIMER, LIST_REPEAT_TIMER, 0);
    run_posted();
    return closed_with(list, top_row(list) == START_ROW);
}

bool test_a_press_between_a_lists_arrows_moves_it_a_page(void)
{
    HWND list = list_at_end();
    LRESULT last = top_row(list);

    press_bar(list, WM_NCLBUTTONDOWN, up_arrow_edge(list) + 1);
    (void)PostMessageW(list, WM_LBUTTONUP, 0, 0);
    run_posted();
    return closed_with(list, top_row(list) < last - 1);
}

bool test_a_key_in_a_list_moves_its_caret_one_row(void)
{
    HWND list = shown_list(FAR_AWAY);

    (void)SendMessageW(list, LB_SETCARETINDEX, 0, FALSE);
    (void)PostMessageW(list, WM_KEYDOWN, VK_DOWN, 0);
    run_posted();
    return closed_with(list, SendMessageW(list, LB_GETCARETINDEX, 0, 0) == 1);
}
