#include "dialog_tests.h"

#include <wchar.h>
#include <windows.h>

#include "../../src/win/win_parts.h"
#include "../accessibility/window_fixture.h"

enum { ROW_CAPACITY = 2048 };

static wchar_t row_text[ROW_CAPACITY];

static HWND control_of(HWND window, ControlId control)
{
    return GetDlgItem(window, CONTROL_ID_BASE + (int)control);
}

static HWND popup_of(HWND window)
{
    return (window != NULL) ? FindWindowW(POPUP_CLASS_NAME, NULL) : NULL;
}

static void type_into(HWND window, ControlId control, const wchar_t *text)
{
    HWND field = control_of(window, control);

    (void)SetWindowTextW(field, text);
    (void)SendMessageW(window, WM_COMMAND, MAKEWPARAM(CONTROL_ID_BASE + (int)control, EN_CHANGE), (LPARAM)field);
}

static const wchar_t *first_row(HWND list, UINT length_message, UINT text_message)
{
    row_text[0] = L'\0';
    if (SendMessageW(list, length_message, 0, 0) < ROW_CAPACITY) {
        (void)SendMessageW(list, text_message, 0, (LPARAM)row_text);
    }
    return row_text;
}

static const wchar_t *text_of(HWND window, ControlId control)
{
    row_text[0] = L'\0';
    (void)GetWindowTextW(control_of(window, control), row_text, ROW_CAPACITY);
    return row_text;
}

static bool first_row_after_filter(ControlId filter, ControlId list, const wchar_t *typed, const wchar_t *row)
{
    HWND window = app_open(GetModuleHandleW(NULL));
    bool starts;

    type_into(window, filter, typed);
    starts = wcsncmp(first_row(control_of(window, list), LB_GETTEXTLEN, LB_GETTEXT), row, wcslen(row)) == 0;
    window_fixture_close(window);
    return starts;
}

static bool first_civ_after_name(const wchar_t *name, const wchar_t *row)
{
    HWND window = app_open(GetModuleHandleW(NULL));
    bool shown;

    type_into(window, CONTROL_NAME, L"Testonia");
    type_into(window, CONTROL_NAME, name);
    shown = wcscmp(first_row(control_of(window, CONTROL_CIV), CB_GETLBTEXTLEN, CB_GETLBTEXT), row) == 0;
    window_fixture_close(window);
    return shown;
}

bool test_appearance_opens_the_popup_of_the_look_choices(void)
{
    HWND window = app_open(GetModuleHandleW(NULL));
    bool shown;

    window_fixture_click(window, CONTROL_APPEARANCE);
    shown = IsWindowVisible(popup_of(window)) != 0;
    window_fixture_close(window);
    return shown;
}

bool test_the_popup_of_the_look_choices_is_hidden_until_asked_for(void)
{
    HWND window = app_open(GetModuleHandleW(NULL));
    bool hidden = (popup_of(window) != NULL) && (IsWindowVisible(popup_of(window)) == 0);

    window_fixture_close(window);
    return hidden;
}

bool test_the_architecture_is_chosen_in_the_popup_of_the_look_choices(void)
{
    HWND window = app_open(GetModuleHandleW(NULL));
    bool held = GetDlgItem(popup_of(window), CONTROL_ID_BASE + (int)CONTROL_ARCHITECTURE) != NULL;

    window_fixture_close(window);
    return held;
}

bool test_a_civ_outside_ranked_play_is_not_offered_to_edit(void)
{
    HWND window = app_open(GetModuleHandleW(NULL));
    LRESULT row = SendMessageW(control_of(window, CONTROL_CIV), CB_FINDSTRING, (WPARAM)-1, (LPARAM)L"Spartans");

    window_fixture_close(window);
    return row == CB_ERR;
}

bool test_a_changed_civ_is_starred_in_civ_to_edit(void)
{
    return first_civ_after_name(L"Testonia", L"Britons*");
}

bool test_a_civ_changed_back_loses_its_star(void)
{
    return first_civ_after_name(L"Britons", L"Britons");
}

bool test_finish_counts_a_changed_civ_without_a_save(void)
{
    HWND window = app_open(GetModuleHandleW(NULL));
    bool counted;

    type_into(window, CONTROL_NAME, L"Testonia");
    window_fixture_act(window, ACTION_FIRST_STEP + (int)STEP_FINISH);
    counted = wcsstr(text_of(window, CONTROL_CHANGED), L"1 civ changed") != NULL;
    window_fixture_close(window);
    return counted;
}

bool test_the_filter_of_the_team_bonuses_leaves_the_rows_that_hold_its_text(void)
{
    HWND window = app_open(GetModuleHandleW(NULL));
    LRESULT rows;

    type_into(window, CONTROL_TEAM_FILTER, L"no team bonus says this");
    rows = SendMessageW(control_of(window, CONTROL_TEAM_LIST), LB_GETCOUNT, 0, 0);
    window_fixture_close(window);
    return rows == 0;
}

static bool rows_after_filter(ControlId filter, ControlId list, const wchar_t *typed)
{
    HWND window = app_open(GetModuleHandleW(NULL));
    LRESULT rows;

    type_into(window, filter, typed);
    rows = SendMessageW(control_of(window, list), LB_GETCOUNT, 0, 0);
    window_fixture_close(window);
    return rows > 0;
}

bool test_a_civs_name_in_the_filter_of_the_bonuses_gives_its_civ_bonuses(void)
{
    return rows_after_filter(CONTROL_BONUS_FILTER, CONTROL_BONUS_LIST, L"britons");
}

bool test_a_civs_name_in_the_filter_of_the_tech_tree_gives_its_nodes(void)
{
    return rows_after_filter(CONTROL_TREE_FILTER, CONTROL_TREE_LIST, L"britons");
}

bool test_a_civs_name_in_the_filter_of_the_units_gives_its_unique_unit(void)
{
    return first_row_after_filter(CONTROL_UNIT_FILTER, CONTROL_UNIT_LIST, L"britons", L"Longbowman");
}

bool test_a_civs_name_in_the_filter_of_the_castle_techs_gives_its_castle_tech(void)
{
    return first_row_after_filter(CONTROL_CASTLE_TECH_FILTER, CONTROL_CASTLE_TECH_LIST, L"britons", L"Yeomen");
}

bool test_a_civs_name_in_the_filter_of_the_imperial_techs_gives_its_imperial_tech(void)
{
    return first_row_after_filter(CONTROL_IMPERIAL_TECH_FILTER, CONTROL_IMPERIAL_TECH_LIST, L"britons", L"Warwolf");
}

bool test_clear_leaves_the_civ_without_a_civ_bonus(void)
{
    HWND window = app_open(GetModuleHandleW(NULL));
    bool cleared;

    window_fixture_click(window, CONTROL_BONUS_CLEAR);
    window_fixture_act(window, ACTION_FIRST_STEP + (int)STEP_FINISH);
    cleared = wcsstr(text_of(window, CONTROL_SUMMARY), L"Civ bonuses: 0") != NULL;
    window_fixture_close(window);
    return cleared;
}

bool test_the_filter_of_choose_hero_leaves_the_heroes_that_hold_its_text(void)
{
    HWND window = app_open(GetModuleHandleW(NULL));

    window_fixture_act(window, ACTION_FIRST_STEP + (int)STEP_UU_UT);
    window_fixture_tick(window, CONTROL_HERO);
    window_fixture_filter(L"saladin");
    window_fixture_answer(L"Saladin", NULL);
    window_fixture_click(window, CONTROL_HERO_CHOOSE);
    window_fixture_close(window);
    return window_fixture_asked()->rows == 1;
}
