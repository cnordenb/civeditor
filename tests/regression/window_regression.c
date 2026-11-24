#include "window_regression.h"

#include <wchar.h>
#include <windows.h>

#include "../../src/win/win_parts.h"
#include "../accessibility/window_fixture.h"
#include "../shell/dialog_tests.h"
#include "../test_outcome.h"

enum {
    NOTCH = 120,
    SLOWEST_MOVE = 150,
    LEAST_COLOURS = 4,
    COLOUR_ROOM = 8,
    FAR_AWAY = -20000,
    ON_SCREEN = 0,
    START_ROW = 2,
    TWICE = 200
};

typedef enum {
    MOVE_WHEEL,
    MOVE_WHEEL_AGAIN,
    MOVE_UP_ARROW,
    MOVE_UP_ARROW_TWICE,
    MOVE_COUNT
} Move;

bool test_a_cost_cut_by_half_slid_to_twice_cuts_it_by_three_quarters(void)
{
    HWND window = app_open(GetModuleHandleW(NULL));
    HWND card;
    HWND slider;
    bool cut;

    window_fixture_act(window, ACTION_FIRST_STEP + (int)STEP_BONUSES);
    (void)SetFocus(window_fixture_field(window, L"Town Centers cost -50% wood", 0));
    card = GetDlgItem(GetDlgItem(window, CONTROL_ID_BASE + CONTROL_BONUS_INFO), CHOSEN_CARD_ID);
    slider = GetDlgItem(card, CHOSEN_SLIDER_ID);
    (void)SendMessageW(slider, TBM_SETPOS, TRUE, TWICE);
    (void)SendMessageW(card, WM_HSCROLL, MAKEWPARAM(TB_ENDTRACK, 0), (LPARAM)slider);
    cut = window_fixture_row(window, L"Town Centers cost -75% wood") != NULL;
    window_fixture_close(window);
    return cut;
}

bool test_no_step_is_shown_as_done_when_the_window_opens(void)
{
    HWND window = dialog_opened();
    wchar_t row[STATUS_CAPACITY] = L"";
    bool none = true;
    int step;

    for (step = 0; (step < STEP_COUNT) && none; ++step) {
        none = (SendMessageW(dialog_control_of(window, CONTROL_STEPS), LB_GETTEXT, (WPARAM)step, (LPARAM)row) > 0) &&
               (wcsstr(row, L"(done)") == NULL);
        if (!none) {
            test_note("a step shown as done");
        }
    }
    return dialog_closed(window, none);
}

bool test_the_exit_command_closes_a_window_without_changes(void)
{
    return !dialog_stays_after(ACTION_EXIT);
}

static HWND scrolled_to(HWND list, LRESULT row)
{
    (void)SendMessageW(list, LB_SETTOPINDEX, (WPARAM)row, 0);
    return list;
}

static HWND list_at_row(LRESULT row)
{
    return scrolled_to(scroll_shown_list(FAR_AWAY), row);
}

static HWND list_at_end(void)
{
    HWND list = scroll_shown_list(FAR_AWAY);

    return scrolled_to(list, SendMessageW(list, LB_GETCOUNT, 0, 0) - 1);
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

static RECT place_in(HWND window, int identifier)
{
    RECT place = { 0, 0, 0, 0 };

    (void)GetWindowRect(GetDlgItem(window, identifier), &place);
    (void)MapWindowPoints(NULL, window, (POINT *)&place, 2);
    return place;
}

bool test_a_lists_scroll_bar_shows_its_arrows_while_the_mouse_is_away(void)
{
    HWND list = scroll_shown_list(FAR_AWAY);

    return scroll_closed_with(list, colours_in_corner(list, false) >= LEAST_COLOURS);
}

bool test_the_windows_scroll_bar_shows_its_arrows_while_the_mouse_is_away(void)
{
    HWND window = scroll_small_window(STEP_NAME_AND_TREE);
    int colours = colours_in_corner(window, true);

    window_fixture_close(window);
    return colours >= LEAST_COLOURS;
}

bool test_a_step_ends_a_margin_from_the_windows_scroll_bar(void)
{
    HWND window = scroll_small_window(STEP_NAME_AND_TREE);
    RECT back = place_in(window, CONTROL_ID_BASE + CONTROL_BACK);
    RECT next = place_in(window, CONTROL_ID_BASE + CONTROL_NEXT);
    RECT bar = place_in(window, WINDOW_BAR_ID);
    int gap = bar.left - next.right;

    window_fixture_close(window);
    return (gap >= back.left - 1) && (gap <= back.left + 1);
}

bool test_each_wheel_notch_moves_a_list_one_row_its_way(void)
{
    HWND list = list_at_row(START_ROW);

    turn_wheel(list, NOTCH);
    turn_wheel(list, -NOTCH);
    turn_wheel(list, -NOTCH);
    scroll_run_posted();
    return scroll_closed_with(list, top_row(list) == START_ROW + 1);
}

bool test_a_press_on_either_arrow_moves_a_list_one_row_its_way(void)
{
    HWND list = list_at_row(START_ROW);

    click_bar(list, WM_NCLBUTTONDOWN, up_arrow_edge(list));
    click_bar(list, WM_NCLBUTTONDOWN, down_arrow_edge(list));
    click_bar(list, WM_NCLBUTTONDBLCLK, down_arrow_edge(list));
    scroll_run_posted();
    return scroll_closed_with(list, top_row(list) == START_ROW + 1);
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
    HWND list = scroll_shown_list(ON_SCREEN);
    ULONGLONG slowest = 0;
    ULONGLONG start;
    ULONGLONG took;
    int move;

    for (move = 0; move < MOVE_COUNT; ++move) {
        start = GetTickCount64();
        post_move(list, (Move)move);
        scroll_run_posted();
        took = GetTickCount64() - start;
        slowest = (took > slowest) ? took : slowest;
    }
    return scroll_closed_with(list, slowest < SLOWEST_MOVE);
}

bool test_a_held_arrow_keeps_scrolling_a_list(void)
{
    HWND list = scroll_shown_list(FAR_AWAY);

    press_bar(list, WM_NCLBUTTONDOWN, down_arrow_edge(list));
    (void)PostMessageW(GetParent(list), WM_TIMER, LIST_REPEAT_TIMER, 0);
    scroll_run_posted();
    return scroll_closed_with(list, top_row(list) == START_ROW);
}

bool test_a_press_between_a_lists_arrows_moves_it_a_page(void)
{
    HWND list = list_at_end();
    LRESULT last = top_row(list);

    press_bar(list, WM_NCLBUTTONDOWN, up_arrow_edge(list) + 1);
    (void)PostMessageW(list, WM_LBUTTONUP, 0, 0);
    scroll_run_posted();
    return scroll_closed_with(list, top_row(list) < last - 1);
}

bool test_each_wheel_notch_moves_an_open_dropdown_one_row(void)
{
    HWND choice = dropdown_shown_choice();

    (void)dropdown_time_to_open(choice);
    dropdown_wheel_down(choice);
    return dropdown_closed_with(choice, SendMessageW(dropdown_rows_of(choice), LB_GETTOPINDEX, 0, 0) == 1);
}
