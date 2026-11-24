#include "chosen_tests.h"

#include <wchar.h>
#include <windows.h>

#include "../../../src/win/win_parts.h"
#include "../../accessibility/window_fixture.h"
#include "../../shell/dialog_tests.h"
#include "unit_panel_tests.h"

enum { TEXT_ROOM = 256, TWICE = 200, RING_REACH = 3, INSIDE = 2, PIXEL_LIMIT = 65536, CHOSEN_OWN_COUNT = 20 };
enum { FIELD_EDGES = 4, FIELD_LEAST = 24 };

typedef struct {
    HDC screen;
    HDC memory;
    HBITMAP picture;
    HGDIOBJ before;
} Printed;

static const wchar_t HALF_CUT[] = L"Town Centers cost -50% wood";
static const wchar_t QUARTER_CUT[] = L"Town Centers cost -75% wood";
static const wchar_t HALF_CUT_RANGE[] = L"Type a value from -50% to -99.9%.";
static const wchar_t SHEPHERDS[] = L"Shepherds work 25% faster";
static const wchar_t ARCHERS[] = L"Foot archers (except skirmishers) +1 range";
static const wchar_t ARCHERS_MIDWAY[] = L"Foot archers (except skirmishers) +5.5 range";

static wchar_t text[TEXT_ROOM];

static HWND opened(void)
{
    HWND window = app_open(GetModuleHandleW(NULL));

    window_fixture_act(window, ACTION_FIRST_STEP + (int)STEP_BONUSES);
    return window;
}

static HWND cut_field(HWND window)
{
    return window_fixture_field(window, HALF_CUT, 0);
}

static HWND card_of(HWND window)
{
    return GetDlgItem(GetDlgItem(window, CONTROL_ID_BASE + CONTROL_BONUS_INFO), CHOSEN_CARD_ID);
}

static HWND slider_of(HWND window)
{
    return GetDlgItem(card_of(window), CHOSEN_SLIDER_ID);
}

static bool closed_with(HWND window, bool result)
{
    window_fixture_close(window);
    return result;
}

static bool reads(HWND control, const wchar_t *expected)
{
    text[0] = L'\0';
    (void)GetWindowTextW(control, text, TEXT_ROOM);
    return wcscmp(text, expected) == 0;
}

static bool is_shown(HWND control)
{
    return (control != NULL) && ((GetWindowLongPtrW(control, GWL_STYLE) & WS_VISIBLE) != 0);
}

static HWND typed_and_left(const wchar_t *row, const wchar_t *typed)
{
    HWND window = opened();
    HWND field = window_fixture_field(window, row, 0);

    (void)SetWindowTextW(field, typed);
    (void)SendMessageW(GetParent(field), WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(field), EN_KILLFOCUS), (LPARAM)field);
    return window;
}

static HWND focused(void)
{
    HWND window = opened();

    (void)SetFocus(cut_field(window));
    return window;
}

static HWND key_posted(WPARAM key, const wchar_t *typed)
{
    HWND window = focused();
    HWND field = cut_field(window);

    if (typed != NULL) {
        (void)SetWindowTextW(field, typed);
    }
    (void)PostMessageW(field, WM_KEYDOWN, key, 0);
    scroll_run_posted();
    return window;
}

static bool typed_counts(void)
{
    HWND window = typed_and_left(HALF_CUT, L"75");

    return closed_with(window, window_fixture_row(window, QUARTER_CUT) != NULL);
}

static bool status_reads(HWND window, const wchar_t *expected)
{
    return reads(GetDlgItem(window, CONTROL_ID_BASE + CONTROL_STATUS), expected);
}

static bool refusal_named(void)
{
    HWND window = typed_and_left(HALF_CUT, L"100");

    return closed_with(window, status_reads(window, HALF_CUT_RANGE));
}

static bool entered_counts(void)
{
    HWND window = key_posted(VK_RETURN, L"75");

    return closed_with(window, window_fixture_row(window, QUARTER_CUT) != NULL);
}

static bool field_holds_number(void)
{
    HWND window = opened();

    return closed_with(window, reads(cut_field(window), L"-50"));
}

static bool slider_just_below(void)
{
    HWND window = focused();
    RECT field = { 0, 0, 0, 0 };
    RECT card = { 0, 0, 0, 0 };

    (void)GetWindowRect(cut_field(window), &field);
    (void)GetWindowRect(card_of(window), &card);
    return closed_with(window, is_shown(card_of(window)) && (card.top >= field.bottom) &&
                                   (card.top - field.bottom < field.bottom - field.top) && (card.left < field.right) &&
                                   (card.right > field.left));
}

static bool preview_shown(void)
{
    HWND window = focused();
    HWND slider = slider_of(window);

    (void)SendMessageW(slider, TBM_SETPOS, TRUE, TWICE);
    (void)SendMessageW(card_of(window), WM_HSCROLL, MAKEWPARAM(TB_THUMBTRACK, TWICE), (LPARAM)slider);
    return closed_with(window, reads(cut_field(window), L"-75"));
}

static bool hidden_when_left(void)
{
    HWND window = focused();

    (void)SetFocus(GetDlgItem(window, CONTROL_ID_BASE + CONTROL_BONUS_FILTER));
    return closed_with(window, !is_shown(card_of(window)));
}

static bool hidden_by_escape(void)
{
    HWND window = key_posted(VK_ESCAPE, NULL);

    return closed_with(window, !is_shown(card_of(window)) && (GetFocus() == cut_field(window)));
}

static bool row_is_label(void)
{
    HWND window = opened();
    wchar_t kind[TEXT_ROOM] = L"";

    (void)GetClassNameW(window_fixture_row(window, HALF_CUT), kind, TEXT_ROOM);
    return closed_with(window, wcscmp(kind, L"Static") == 0);
}

static bool tab_reaches_slider(void)
{
    HWND window = focused();
    MSG key = { NULL, WM_KEYDOWN, VK_TAB, 0, 0, { 0, 0 } };

    key.hwnd = cut_field(window);
    (void)IsDialogMessageW(window, &key);
    return closed_with(window, GetFocus() == slider_of(window));
}

static COLORREF ring_color(void)
{
    Rgb color = palette_color(COLOR_ROLE_CHOSEN);

    return RGB(color.red, color.green, color.blue);
}

static Printed printed(HWND control)
{
    RECT client = { 0, 0, 0, 0 };
    Printed print;

    (void)GetClientRect(control, &client);
    print.screen = GetDC(control);
    print.memory = CreateCompatibleDC(print.screen);
    print.picture = CreateCompatibleBitmap(print.screen, client.right, client.bottom);
    print.before = SelectObject(print.memory, print.picture);
    (void)SendMessageW(control, WM_PRINTCLIENT, (WPARAM)print.memory, PRF_CLIENT);
    return print;
}

static void unprinted(HWND control, Printed print)
{
    (void)SelectObject(print.memory, print.before);
    (void)DeleteObject(print.picture);
    (void)DeleteDC(print.memory);
    (void)ReleaseDC(control, print.screen);
}

static bool focus_given_up(void)
{
    HWND window = focused();

    window_fixture_click(window, CONTROL_BONUS_CLEAR);
    return closed_with(window, GetFocus() == GetDlgItem(window, CONTROL_ID_BASE + CONTROL_BONUS_LIST));
}

static bool status_kept(void)
{
    HWND window = key_posted(VK_RETURN, L"100");

    window_fixture_click(window, CONTROL_BONUS_CLEAR);
    return closed_with(window, status_reads(window, HALF_CUT_RANGE));
}

static RECT rect_in(HWND window, HWND parent)
{
    RECT rect = { 0, 0, 0, 0 };

    (void)GetWindowRect(window, &rect);
    (void)MapWindowPoints(NULL, parent, (POINT *)&rect, 2);
    return rect;
}

static int scaled(HWND window, int units)
{
    return MulDiv(units, assist_dpi(window), LAYOUT_BASE_DPI);
}

static SIZE text_size(HWND field, const wchar_t *shown)
{
    LRESULT font = SendMessageW(field, WM_GETFONT, 0, 0);
    HDC context = GetDC(field);
    HGDIOBJ before = SelectObject(context, (HGDIOBJ)font);
    SIZE size = { 0, 0 };

    (void)GetTextExtentPoint32W(context, shown, (int)wcslen(shown), &size);
    (void)SelectObject(context, before);
    (void)ReleaseDC(field, context);
    return size;
}

static bool framed(const wchar_t *label, int number, bool below)
{
    HWND window = opened();
    HWND row = window_fixture_row(window, label);
    HWND field = window_fixture_field(window, label, number);
    int reach = scaled(row, RING_REACH);
    RECT place;
    Printed print;
    COLORREF frame;

    (void)SetFocus(field);
    place = rect_in(field, row);
    print = printed(row);
    frame = below ? GetPixel(print.memory, (place.left + place.right) / 2, place.bottom + reach - 1)
                  : GetPixel(print.memory, place.left - reach, (place.top + place.bottom) / 2);
    unprinted(row, print);
    return closed_with(window, frame == ring_color());
}

static bool inked(HDC picture, RECT area)
{
    COLORREF paper = GetPixel(picture, area.left, area.top);
    int width = area.right - area.left;
    int pixel;

    for (pixel = 0; (pixel < width * (area.bottom - area.top)) && (pixel < PIXEL_LIMIT); ++pixel) {
        if (GetPixel(picture, area.left + (pixel % width), area.top + (pixel / width)) != paper) {
            return true;
        }
    }
    return false;
}

static bool end_shown(bool least)
{
    HWND window = focused();
    HWND card = card_of(window);
    RECT track = rect_in(slider_of(window), card);
    RECT area = { 0, 0, 0, 0 };
    Printed print = printed(card);
    bool shown;

    (void)GetClientRect(card, &area);
    area.left = least ? INSIDE : track.right;
    area.right = least ? track.left : area.right - INSIDE;
    area.top = INSIDE;
    area.bottom -= INSIDE;
    shown = inked(print.memory, area);
    unprinted(card, print);
    return closed_with(window, shown);
}

static bool field_after_text(void)
{
    HWND window = opened();
    HWND field = cut_field(window);
    RECT place = rect_in(field, window_fixture_row(window, HALF_CUT));

    return closed_with(window, place.left == scaled(field, TEXT_INSET) + text_size(field, L"Town Centers cost ").cx);
}

static bool wider_number_held(void)
{
    HWND window = typed_and_left(ARCHERS, L"5.5");
    HWND field = window_fixture_field(window, ARCHERS_MIDWAY, 0);
    RECT room = { 0, 0, 0, 0 };

    text[0] = L'\0';
    (void)GetWindowTextW(field, text, TEXT_ROOM);
    (void)SendMessageW(field, EM_GETRECT, 0, (LPARAM)&room);
    return closed_with(window, (field != NULL) && (text_size(field, text).cx <= room.right - room.left));
}

static bool field_as_tall_as_text(void)
{
    HWND window = opened();
    HWND field = cut_field(window);
    RECT place = rect_in(field, window_fixture_row(window, HALF_CUT));
    int tall = text_size(field, L"0").cy + FIELD_EDGES;
    int least = scaled(field, FIELD_LEAST);

    return closed_with(window, place.bottom - place.top == ((tall > least) ? tall : least));
}

TestOutcome chosen_tests_run(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("a value typed into the field of a number and left sets the count that shows it",
                               typed_counts());
    case 2:
        return test_outcome_of("a value typed past the most of a number names its range", refusal_named());
    case 3:
        return test_outcome_of("Enter in the field of a number sets the count that shows it", entered_counts());
    case 4:
        return test_outcome_of("the field of a number in a chosen bonus holds the number alone", field_holds_number());
    case 5:
        return test_outcome_of("the slider is shown just below the field of the number in focus", slider_just_below());
    case 6:
        return test_outcome_of("the slider dragged shows in the field the value it would set", preview_shown());
    case 7:
        return test_outcome_of("the slider is hidden when the focus leaves the fields of numbers", hidden_when_left());
    case 8:
        return test_outcome_of("Esc in the field of a number hides its slider", hidden_by_escape());
    case 9:
        return test_outcome_of("a chosen bonus is shown as a label", row_is_label());
    case 10:
        return test_outcome_of("Tab from the field of a number moves the focus to its slider", tab_reaches_slider());
    case 11:
        return test_outcome_of("the field of the number in focus is framed", framed(HALF_CUT, 0, false));
    case 12:
        return test_outcome_of("the card shows the least value of its number left of the slider", end_shown(true));
    case 13:
        return test_outcome_of("the card shows the most value of its number right of the slider", end_shown(false));
    case 14:
        return test_outcome_of("a field that a change takes away gives the focus to the list of civ bonuses",
                               focus_given_up());
    case 15:
        return test_outcome_of("a field that a change takes away leaves the status line as it was", status_kept());
    case 16:
        return test_outcome_of("the field of a number stands just after the text before it", field_after_text());
    case 17:
        return test_outcome_of("the field of a number holds a number wider than at either end of its range",
                               wider_number_held());
    case 18:
        return test_outcome_of("the frame of the number in focus is drawn whole within its label",
                               framed(SHEPHERDS, 0, true));
    case 19:
        return test_outcome_of("the frame of a number on a later line of its label is drawn around it",
                               framed(ARCHERS, 1, false));
    case 20:
        return test_outcome_of("the field of a number is just as tall as its text needs, and at least 24 pixels",
                               field_as_tall_as_text());
    default:
        return unit_panel_tests_run(index - CHOSEN_OWN_COUNT);
    }
}
