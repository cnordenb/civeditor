#include "reflow_tests.h"

#include <windows.h>

#include "../../src/core/check.h"
#include "../../src/reflow/reflow.h"
#include "../../src/win/win_parts.h"
#include "../accessibility/window_fixture.h"
#include "../test_outcome.h"

enum {
    BOX_CAPACITY = 2 * CONTROL_COUNT,
    NARROW = REFLOW_MIN_WIDTH,
    SHORT_VIEW = 256,
    EDGE = 16,
    LABEL_ROOM = 26,
    NARROW_WINDOW = 420,
    SHORT_WINDOW = 360,
    TALL_WINDOW = 2000,
    LONG_ROW = 0,
    SHORT_ROW = 2,
    BASE_TOP = 370,
    GAP_BELOW = 8
};

typedef enum {
    ASK_INSIDE,
    ASK_LARGE
} BoxQuestion;

static Box boxes[BOX_CAPACITY];

static int collect_boxes(WizardStep step, bool labels)
{
    int count = 0;
    int control;

    for (control = 0; control < CONTROL_COUNT; ++control) {
        if (!layout_shows((ControlId)control, step) || (!labels && !layout_is_focusable((ControlId)control))) {
            continue;
        }
        boxes[count] = reflow_box((ControlId)control, step, NARROW);
        ++count;
        if (labels && layout_has_label((ControlId)control)) {
            boxes[count] = reflow_label_box((ControlId)control, step, NARROW);
            ++count;
        }
    }
    return count;
}

static bool box_answers(Box box, BoxQuestion question)
{
    if (question == ASK_INSIDE) {
        return (box.left >= 0) && (box.top >= 0) && (box.left + box.width <= NARROW);
    }
    return (box.width >= LAYOUT_TARGET_SIZE) && (box.height >= LAYOUT_TARGET_SIZE);
}

static bool every_box_answers(BoxQuestion question)
{
    int step;
    int index;
    int count;

    for (step = 0; step < STEP_COUNT; ++step) {
        count = collect_boxes((WizardStep)step, question == ASK_INSIDE);
        for (index = 0; (index < count) && (index < BOX_CAPACITY); ++index) {
            if (!box_answers(boxes[index], question)) {
                test_note(wizard_page((WizardStep)step)->title);
                return false;
            }
        }
    }
    return true;
}

static bool step_has_no_overlap(WizardStep step)
{
    int count = collect_boxes(step, true);
    int first;
    int second;

    for (first = 0; (first < count) && (first < BOX_CAPACITY); ++first) {
        for (second = first + 1; (second < count) && (second < BOX_CAPACITY); ++second) {
            if (layout_boxes_overlap(boxes[first], boxes[second])) {
                test_note(wizard_page(step)->title);
                return false;
            }
        }
    }
    return true;
}

static Box box_at(int top)
{
    Box box = { EDGE, 0, NARROW - (2 * EDGE), LAYOUT_TARGET_SIZE };

    box.top = top;
    return box;
}

bool test_a_window_narrower_than_the_wide_layout_reflows(void)
{
    return reflow_is_needed(LAYOUT_MIN_WIDTH - 1, LAYOUT_MIN_HEIGHT);
}

bool test_a_window_shorter_than_the_wide_layout_reflows(void)
{
    return reflow_is_needed(LAYOUT_MIN_WIDTH, LAYOUT_MIN_HEIGHT - 1);
}

bool test_a_window_as_large_as_the_wide_layout_does_not_reflow(void)
{
    return !reflow_is_needed(LAYOUT_MIN_WIDTH, LAYOUT_MIN_HEIGHT);
}

bool test_no_control_reaches_past_a_window_320_wide(void)
{
    return every_box_answers(ASK_INSIDE);
}

bool test_no_two_controls_overlap_in_a_window_320_wide(void)
{
    bool apart = true;
    int step;

    for (step = 0; (step < STEP_COUNT) && apart; ++step) {
        apart = step_has_no_overlap((WizardStep)step);
    }
    return apart;
}

bool test_every_target_is_44_by_44_in_a_window_320_wide(void)
{
    return every_box_answers(ASK_LARGE);
}

bool test_a_step_is_as_high_as_its_last_control_and_a_margin(void)
{
    Box last = reflow_box(CONTROL_NEXT, STEP_BONUSES, NARROW);

    return reflow_height(STEP_BONUSES) == last.top + last.height + EDGE;
}

bool test_a_control_below_the_window_is_scrolled_up_into_it(void)
{
    return reflow_scroll_to_show(box_at(600), 0, SHORT_VIEW) == 600 + LAYOUT_TARGET_SIZE + EDGE - SHORT_VIEW;
}

bool test_a_control_above_the_window_is_scrolled_down_into_it(void)
{
    return reflow_scroll_to_show(box_at(100), 300, SHORT_VIEW) == 100 - LABEL_ROOM - EDGE;
}

bool test_a_control_in_the_window_leaves_it_where_it_is(void)
{
    return reflow_scroll_to_show(box_at(400), 300, SHORT_VIEW) == 300;
}

bool test_the_window_scrolls_no_further_than_its_content(void)
{
    return reflow_scroll_within(900, 700, SHORT_VIEW) == 700 - SHORT_VIEW;
}

bool test_the_window_scrolls_no_higher_than_its_top(void)
{
    return reflow_scroll_within(-5, 700, SHORT_VIEW) == 0;
}

bool test_a_window_that_holds_its_content_does_not_scroll(void)
{
    return reflow_scroll_within(50, 200, SHORT_VIEW) == 0;
}

static HWND sized_window(int width, int height)
{
    HWND window = app_open(GetModuleHandleW(NULL));
    int dpi = assist_dpi(window);

    if (!CHECK(window != NULL) || !CHECK(dpi > 0)) {
        return NULL;
    }
    (void)SetWindowPos(window, NULL, 0, 0, MulDiv(width, dpi, LAYOUT_BASE_DPI), MulDiv(height, dpi, LAYOUT_BASE_DPI),
                       SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
    return window;
}

static RECT place_of(HWND window, ControlId control)
{
    RECT place = { 0, 0, 0, 0 };

    (void)GetWindowRect(GetDlgItem(window, CONTROL_ID_BASE + (int)control), &place);
    (void)MapWindowPoints(NULL, window, (POINT *)&place, 2);
    return place;
}

static int scroll_after(int first, int second, int third)
{
    HWND window = sized_window(NARROW_WINDOW, SHORT_WINDOW);
    int actions[3];
    int index;
    int position;

    actions[0] = first;
    actions[1] = second;
    actions[2] = third;
    for (index = 0; (index < 3) && (window != NULL); ++index) {
        if (actions[index] >= 0) {
            window_fixture_act(window, actions[index]);
        }
    }
    position = GetScrollPos(GetDlgItem(window, WINDOW_BAR_ID), SB_CTL);
    window_fixture_close(window);
    return position;
}

bool test_a_narrow_window_places_its_controls_in_one_column(void)
{
    HWND window = sized_window(NARROW_WINDOW, TALL_WINDOW);
    bool stacked = (window != NULL) &&
                   (place_of(window, CONTROL_ARCHITECTURE).left == place_of(window, CONTROL_NAME).left);

    window_fixture_close(window);
    return stacked;
}

bool test_the_control_in_focus_is_scrolled_into_a_short_window(void)
{
    HWND window = sized_window(NARROW_WINDOW, SHORT_WINDOW);
    RECT client = { 0, 0, 0, 0 };
    RECT place;

    (void)SetFocus(GetDlgItem(window, CONTROL_ID_BASE + CONTROL_SCOUT));
    (void)GetClientRect(window, &client);
    place = place_of(window, CONTROL_SCOUT);
    window_fixture_close(window);
    return (place.top >= 0) && (place.bottom <= client.bottom) && (place.bottom > place.top);
}

bool test_the_window_may_be_as_small_as_320_by_256(void)
{
    HWND window = sized_window(NARROW_WINDOW, SHORT_WINDOW);
    int dpi = assist_dpi(window);
    RECT frame = { 0, 0, 0, 0 };
    MINMAXINFO limits;

    ZeroMemory(&limits, sizeof limits);
    frame.right = MulDiv(REFLOW_MIN_WIDTH, dpi, LAYOUT_BASE_DPI);
    frame.bottom = MulDiv(REFLOW_MIN_HEIGHT, dpi, LAYOUT_BASE_DPI);
    (void)AdjustWindowRectEx(&frame, (DWORD)GetWindowLongPtrW(window, GWL_STYLE), TRUE,
                             (DWORD)GetWindowLongPtrW(window, GWL_EXSTYLE));
    (void)SendMessageW(window, WM_GETMINMAXINFO, 0, (LPARAM)&limits);
    window_fixture_close(window);
    return (limits.ptMinTrackSize.x == frame.right - frame.left) &&
           (limits.ptMinTrackSize.y == frame.bottom - frame.top);
}

bool test_scroll_down_moves_a_short_window_down_a_page(void)
{
    return scroll_after(ACTION_SCROLL_DOWN, -1, -1) > 0;
}

bool test_scroll_up_takes_back_a_scroll_down(void)
{
    return scroll_after(ACTION_SCROLL_DOWN, ACTION_SCROLL_DOWN, ACTION_SCROLL_UP) ==
           scroll_after(ACTION_SCROLL_DOWN, -1, -1);
}

bool test_the_wheel_scrolls_a_short_window(void)
{
    HWND window = sized_window(NARROW_WINDOW, SHORT_WINDOW);
    int position;

    (void)SendMessageW(window, WM_MOUSEWHEEL, MAKEWPARAM(0, (WORD)(short)-WHEEL_DELTA), 0);
    position = GetScrollPos(GetDlgItem(window, WINDOW_BAR_ID), SB_CTL);
    window_fixture_close(window);
    return position > 0;
}

static int bonus_row_height(int row)
{
    HWND window = sized_window(NARROW_WINDOW, TALL_WINDOW);
    int height;

    window_fixture_act(window, ACTION_FIRST_STEP + (int)STEP_BONUSES);
    height = (int)SendMessageW(GetDlgItem(window, CONTROL_ID_BASE + CONTROL_BONUS_LIST), LB_GETITEMHEIGHT,
                               (WPARAM)row, 0);
    window_fixture_close(window);
    return height;
}

bool test_a_long_row_of_a_narrow_list_is_higher_than_a_short_one(void)
{
    return bonus_row_height(LONG_ROW) > bonus_row_height(SHORT_ROW);
}

bool test_a_short_row_of_a_list_is_as_high_as_a_target(void)
{
    HWND window = app_open(GetModuleHandleW(NULL));
    int dpi = assist_dpi(window);

    window_fixture_close(window);
    return bonus_row_height(SHORT_ROW) == MulDiv(LAYOUT_ROW_HEIGHT, dpi, LAYOUT_BASE_DPI);
}

bool test_the_first_control_of_a_step_comes_under_the_help(void)
{
    return reflow_box(CONTROL_BASE, STEP_IDENTITY, NARROW).top == BASE_TOP;
}

bool test_back_and_next_come_under_the_status_line(void)
{
    Box status = reflow_box(CONTROL_STATUS, STEP_IDENTITY, NARROW);

    return reflow_box(CONTROL_BACK, STEP_IDENTITY, NARROW).top == status.top + status.height + GAP_BELOW;
}

bool test_a_control_starts_a_margin_from_the_left(void)
{
    return reflow_box(CONTROL_NAME, STEP_IDENTITY, NARROW).left == EDGE;
}

bool test_next_ends_a_margin_from_the_right(void)
{
    Box next = reflow_box(CONTROL_NEXT, STEP_IDENTITY, NARROW);

    return next.left + next.width == NARROW - EDGE;
}
