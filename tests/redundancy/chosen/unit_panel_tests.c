#include "unit_panel_tests.h"

#include <wchar.h>
#include <windows.h>

#include "../../../src/win/win_parts.h"
#include "../../accessibility/window_fixture.h"

enum { TEXT_ROOM = 256 };

static const wchar_t STATS[] = L"Hit points ";
static const wchar_t SHEPHERDS[] = L"Shepherds work 25% faster";

static HWND opened(ControlId list_control)
{
    HWND window = app_open(GetModuleHandleW(NULL));
    HWND list;

    window_fixture_act(window, ACTION_FIRST_STEP + (int)STEP_UU_UT);
    list = GetDlgItem(window, CONTROL_ID_BASE + (int)list_control);
    (void)SendMessageW(list, LB_SETCURSEL, 0, 0);
    (void)SendMessageW(window, WM_COMMAND, MAKEWPARAM(CONTROL_ID_BASE + (int)list_control, LBN_SELCHANGE),
                       (LPARAM)list);
    return window;
}

static HWND hit_points_field(HWND window)
{
    return window_fixture_panel_field(window, CONTROL_UNIT_INFO, STATS, 0);
}

static bool closed_with(HWND window, bool result)
{
    window_fixture_close(window);
    return result;
}

static void type_and_leave(HWND field, const wchar_t *typed)
{
    (void)SetWindowTextW(field, typed);
    (void)SendMessageW(GetParent(field), WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(field), EN_KILLFOCUS), (LPARAM)field);
}

static HWND typed_and_left(const wchar_t *typed)
{
    HWND window = opened(CONTROL_UNIT_LIST);

    type_and_leave(hit_points_field(window), typed);
    return window;
}

static bool tech_typed_sets(void)
{
    HWND window = opened(CONTROL_IMPERIAL_TECH_LIST);

    type_and_leave(window_fixture_panel_field(window, CONTROL_IMPERIAL_TECH_INFO, L"Cost: ", 0), L"600");
    return closed_with(window,
                       window_fixture_panel_row(window, CONTROL_IMPERIAL_TECH_INFO, L"Cost: 600 food,") != NULL);
}

static bool effect_ticked(void)
{
    HWND window = opened(CONTROL_CASTLE_TECH_LIST);

    window_fixture_answer(SHEPHERDS, NULL);
    window_fixture_click(window, CONTROL_CASTLE_TECH_EFFECTS);
    return closed_with(window, window_fixture_panel_row(window, CONTROL_CASTLE_TECH_INFO, SHEPHERDS) != NULL);
}

static bool typed_sets(void)
{
    HWND window = typed_and_left(L"100");

    return closed_with(window, window_fixture_panel_row(window, CONTROL_UNIT_INFO, L"Hit points 100,") != NULL);
}

static bool refusal_named(void)
{
    HWND window = typed_and_left(L"0");
    wchar_t text[TEXT_ROOM] = L"";

    (void)GetWindowTextW(GetDlgItem(window, CONTROL_ID_BASE + CONTROL_STATUS), text, TEXT_ROOM);
    return closed_with(window, wcscmp(text, L"Type a value from 1 to 32767.") == 0);
}

static bool no_slider(void)
{
    HWND window = opened(CONTROL_UNIT_LIST);
    HWND field = hit_points_field(window);
    HWND card;

    (void)SetFocus(field);
    card = GetDlgItem(GetDlgItem(window, CONTROL_ID_BASE + CONTROL_UNIT_INFO), CHOSEN_CARD_ID);
    return closed_with(window, (field != NULL) && (GetFocus() == field) &&
                                   ((card == NULL) || ((GetWindowLongPtrW(card, GWL_STYLE) & WS_VISIBLE) == 0)));
}

TestOutcome unit_panel_tests_run(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("a value typed into a field of the selected unit and left sets its stat", typed_sets());
    case 2:
        return test_outcome_of("a value typed past the range of a stat of the selected unit names its range",
                               refusal_named());
    case 3:
        return test_outcome_of("the field of a stat of the selected unit shows no slider", no_slider());
    case 4:
        return test_outcome_of("a value typed into a field of the selected imperial tech and left sets it",
                               tech_typed_sets());
    case 5:
        return test_outcome_of("a civ bonus ticked in the effects of the castle tech is shown as what it does",
                               effect_ticked());
    default:
        return test_outcome_missing();
    }
}
