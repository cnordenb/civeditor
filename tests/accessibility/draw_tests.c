#include "access_tests.h"

#include <windows.h>

#include "../../src/core/check.h"
#include "../../src/win/win_parts.h"
#include "window_fixture.h"

enum { ROW_WIDTH = 400, ROW_HEIGHT = 80, FILLED_SHARE = 5 };

static int marks_in(HDC picture, int left, int top, int width, int height)
{
    COLORREF paper = GetPixel(picture, ROW_WIDTH - 1, 0);
    int marks = 0;
    int across;
    int down;

    if (!CHECK(picture != NULL) || !CHECK(width <= ROW_WIDTH) || !CHECK(height <= ROW_HEIGHT)) {
        return 0;
    }
    for (down = 0; (down < height) && (down < ROW_HEIGHT); ++down) {
        for (across = 0; (across < width) && (across < ROW_WIDTH); ++across) {
            marks += (GetPixel(picture, left + across, top + down) != paper) ? 1 : 0;
        }
    }
    return marks;
}

static int steps_to_mark(HDC picture, int left, int top, bool down)
{
    int limit = down ? ROW_HEIGHT : ROW_WIDTH;
    int count;

    if (!CHECK(picture != NULL) || !CHECK(left >= 0) || !CHECK(top >= 0)) {
        return 0;
    }
    for (count = 0; (count < limit) && (count < ROW_WIDTH); ++count) {
        if (marks_in(picture, left + (down ? 0 : count), top + (down ? count : 0), 1, 1) != 0) {
            break;
        }
    }
    return count;
}

static bool is_partly_filled(HDC picture, int left, int top, int size)
{
    int marks = marks_in(picture, left, top, size, size);

    if (!CHECK(picture != NULL) || !CHECK(size <= ROW_HEIGHT)) {
        return false;
    }
    return (marks > 0) && (FILLED_SHARE * marks < 2 * size * size);
}

static bool box_holds_a_check(HDC picture)
{
    int left;
    int top;
    int size;
    int line;

    if (!CHECK(picture != NULL)) {
        return false;
    }
    left = steps_to_mark(picture, 0, ROW_HEIGHT / 2, false);
    top = steps_to_mark(picture, left, 0, true);
    size = marks_in(picture, left, 0, 1, ROW_HEIGHT);
    line = marks_in(picture, left + (size / 2), top, 1, size / 2);
    return (size > 4 * line) && (line > 0) &&
           is_partly_filled(picture, left + line, top + line, size - (2 * line));
}

bool test_a_ticked_row_is_drawn_with_a_check(void)
{
    HWND window = app_open(GetModuleHandleW(NULL));
    HDC screen = GetDC(window);
    HDC memory = CreateCompatibleDC(screen);
    HBITMAP picture = CreateCompatibleBitmap(screen, ROW_WIDTH, ROW_HEIGHT);
    HGDIOBJ before = SelectObject(memory, picture);
    DRAWITEMSTRUCT item = { ODT_LISTBOX, CONTROL_ID_BASE + CONTROL_BONUS_LIST, 0, ODA_DRAWENTIRE, ODS_SELECTED,
                            NULL, NULL, { 0, 0, ROW_WIDTH, ROW_HEIGHT }, 0 };
    bool checked;

    item.hwndItem = GetDlgItem(window, CONTROL_ID_BASE + CONTROL_BONUS_LIST);
    item.hDC = memory;
    (void)SendMessageW(window, WM_DRAWITEM, (WPARAM)item.CtlID, (LPARAM)&item);
    checked = (window != NULL) && box_holds_a_check(memory);
    (void)SelectObject(memory, before);
    (void)DeleteObject(picture);
    (void)DeleteDC(memory);
    (void)ReleaseDC(window, screen);
    window_fixture_close(window);
    return checked;
}
