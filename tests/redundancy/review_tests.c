#include "review_tests.h"

#include <wchar.h>

#include "../../src/core/check.h"
#include "../../src/core/file_io.h"
#include "../../src/data/catalog.h"
#include "../../src/mod/mod_places.h"
#include "../../src/shell/shell.h"
#include "../../src/win/win_parts.h"
#include "../accessibility/window_fixture.h"
#include "../mod/game_folder.h"
#include "../test_outcome.h"

enum { SHOWN_CAPACITY = 512, FRANKS_ROW = 1, ROW_LIMIT = 4096, SAVED_CAPACITY = 16384 };
enum { GO_MENU = 2, FIRST_STEP_ITEM = 5, FAR_AWAY = -20000, SMALL_WIDTH = 480, SMALL_HEIGHT = 360, NOTCH = 120 };

static const wchar_t ROOT[] = L"build";
static const wchar_t HOME[] = L"build\\test_home";
static const wchar_t PROFILE[] = L"test_home\\Games\\Age of Empires 2 DE\\7656\\mods\\local";
static const wchar_t LONG_NAME[] = L"AbcdefghijAbcdefghijAbcdefghijAbcdefghijAbcdefghijAbcdefghij";
static const wchar_t CUT_FILE[] = L"build\\AbcdefghijAbcdefghijAbcdefghijAbcdefghijAbcdefgh.civbuilder.json";
static const char STALE[] = "stale";
static const wchar_t GO_STEPS[] = L"&1 Name and tech tree\tCtrl+1|&2 Bonuses\tCtrl+2|&3 UU&&UT\tCtrl+3|"
                                  L"&4 Review\tCtrl+4|";

static wchar_t shown[SHOWN_CAPACITY];
static char saved[SAVED_CAPACITY];
static Catalog catalog;

static HWND control_of(HWND window, ControlId control)
{
    return GetDlgItem(window, CONTROL_ID_BASE + (int)control);
}

static void notify(HWND window, ControlId control, int code)
{
    HWND sender = control_of(window, control);

    (void)SendMessageW(window, WM_COMMAND, MAKEWPARAM(CONTROL_ID_BASE + (int)control, code), (LPARAM)sender);
}

static void type_into(HWND window, ControlId control, const wchar_t *text)
{
    (void)SetWindowTextW(control_of(window, control), text);
    notify(window, control, EN_CHANGE);
}

static void choose_row(HWND window, ControlId control, int row, UINT message, int code)
{
    (void)SendMessageW(control_of(window, control), message, (WPARAM)row, 0);
    notify(window, control, code);
}

static HWND opened_with(const wchar_t *steam, const wchar_t *mod_name)
{
    AskScript script = { true, IDOK, ROOT, NULL, NULL, HOME, NULL };

    script.steam = steam;
    script.text = mod_name;
    ask_script(&script);
    return app_open(GetModuleHandleW(NULL));
}

static HWND opened_with_game(const wchar_t *mod_name)
{
    if (!CHECK(mod_make_folders(ROOT, PROFILE)) || !CHECK(game_folder_is_made())) {
        return NULL;
    }
    return opened_with(GAME_FOLDER_STEAM, mod_name);
}

static const wchar_t *text_of(HWND window, ControlId control)
{
    shown[0] = L'\0';
    (void)GetWindowTextW(control_of(window, control), shown, SHOWN_CAPACITY);
    return shown;
}

static bool closed(HWND window, bool passed)
{
    window_fixture_close(window);
    ask_script(NULL);
    return passed && CHECK(window != NULL);
}

static bool review_says(HWND window, ControlId control, const wchar_t *words)
{
    window_fixture_act(window, ACTION_FIRST_STEP + STEP_REVIEW);
    return closed(window, wcsstr(text_of(window, control), words) != NULL);
}

bool test_the_window_opens_with_the_britons_to_edit(void)
{
    HWND window = app_open(GetModuleHandleW(NULL));
    HWND civ = control_of(window, CONTROL_CIV);
    LRESULT row = SendMessageW(civ, CB_GETCURSEL, 0, 0);

    shown[0] = L'\0';
    if ((row >= 0) && (SendMessageW(civ, CB_GETLBTEXTLEN, (WPARAM)row, 0) < SHOWN_CAPACITY)) {
        (void)SendMessageW(civ, CB_GETLBTEXT, (WPARAM)row, (LPARAM)shown);
    }
    return closed(window, wcscmp(shown, L"Britons") == 0);
}

bool test_the_name_field_holds_the_name_of_the_civ_to_edit(void)
{
    HWND window = app_open(GetModuleHandleW(NULL));

    return closed(window, wcscmp(text_of(window, CONTROL_NAME), L"Britons") == 0);
}

bool test_another_civ_keeps_the_edits_of_the_civ_before(void)
{
    HWND window = app_open(GetModuleHandleW(NULL));

    type_into(window, CONTROL_NAME, L"Testonia");
    choose_row(window, CONTROL_CIV, FRANKS_ROW, CB_SETCURSEL, CBN_SELCHANGE);
    choose_row(window, CONTROL_CIV, 0, CB_SETCURSEL, CBN_SELCHANGE);
    return closed(window, wcscmp(text_of(window, CONTROL_NAME), L"Testonia") == 0);
}

bool test_the_review_counts_no_civ_changed_at_first(void)
{
    return review_says(app_open(GetModuleHandleW(NULL)), CONTROL_CHANGED, L"0 civs changed");
}

bool test_save_civ_counts_the_civ_it_puts_in_the_mod(void)
{
    HWND window = app_open(GetModuleHandleW(NULL));

    type_into(window, CONTROL_NAME, L"Testonia");
    window_fixture_act(window, ACTION_ADD);
    return review_says(window, CONTROL_CHANGED, L"1 civ changed");
}

bool test_save_civ_leaves_a_civ_as_the_game_has_it_out_of_the_mod(void)
{
    HWND window = app_open(GetModuleHandleW(NULL));

    window_fixture_act(window, ACTION_ADD);
    return review_says(window, CONTROL_STATUS, L"leaves it out");
}

bool test_the_review_text_shows_no_caret_when_clicked(void)
{
    HWND window = app_open(GetModuleHandleW(NULL));
    HWND summary = control_of(window, CONTROL_SUMMARY);
    GUITHREADINFO caret = { 0 };

    caret.cbSize = sizeof caret;
    window_fixture_act(window, ACTION_FIRST_STEP + STEP_REVIEW);
    (void)SetFocus(summary);
    (void)GetGUIThreadInfo(GetCurrentThreadId(), &caret);
    return closed(window, (GetFocus() == summary) && ((caret.flags & GUI_CARETBLINKING) == 0U));
}

bool test_a_text_field_shows_its_caret_when_it_takes_the_focus(void)
{
    HWND window = app_open(GetModuleHandleW(NULL));
    HWND name = control_of(window, CONTROL_NAME);
    GUITHREADINFO caret = { 0 };

    caret.cbSize = sizeof caret;
    (void)SetFocus(control_of(window, CONTROL_CIV));
    (void)SetFocus(name);
    (void)GetGUIThreadInfo(GetCurrentThreadId(), &caret);
    return closed(window, (GetFocus() == name) && ((caret.flags & GUI_CARETBLINKING) != 0U));
}

bool test_a_build_with_no_civ_saved_says_so(void)
{
    HWND window = opened_with_game(NULL);

    window_fixture_act(window, ACTION_BUILD);
    return closed(window, wcsstr(ask_last_message(), L"No civ is saved in the mod") != NULL);
}

static void save_a_whole_civ(HWND window)
{
    type_into(window, CONTROL_NAME, L"Testonia");
    choose_row(window, CONTROL_UNIT_LIST, 0, LB_SETCURSEL, LBN_SELCHANGE);
    choose_row(window, CONTROL_CASTLE_TECH_LIST, 0, LB_SETCURSEL, LBN_SELCHANGE);
    choose_row(window, CONTROL_IMPERIAL_TECH_LIST, 0, LB_SETCURSEL, LBN_SELCHANGE);
    window_fixture_act(window, ACTION_ADD);
}

bool test_a_mod_is_named_civbuilder_mod_unless_named_otherwise(void)
{
    HWND window = opened_with_game(NULL);

    save_a_whole_civ(window);
    window_fixture_act(window, ACTION_BUILD);
    return closed(window, wcsstr(ask_last_message(), L"built as civbuilder_mod-data") != NULL);
}

bool test_an_empty_mod_name_builds_no_mod(void)
{
    HWND window = opened_with_game(L"");

    save_a_whole_civ(window);
    window_fixture_act(window, ACTION_BUILD);
    return closed(window, wcscmp(text_of(window, CONTROL_STATUS), L"No mod was built.") == 0);
}

bool test_a_long_civ_name_is_cut_in_the_name_of_its_file(void)
{
    HWND window = opened_with(NULL, NULL);
    FileRead read;

    (void)CHECK(file_write_all(CUT_FILE, STALE, sizeof STALE - 1U));
    type_into(window, CONTROL_NAME, LONG_NAME);
    window_fixture_act(window, ACTION_SAVE);
    read = file_read_all(CUT_FILE, saved, sizeof saved);
    return closed(window, (read.length > 0U) && (saved[0] == '{'));
}

static LRESULT bonus_rows(HWND window)
{
    return SendMessageW(control_of(window, CONTROL_BONUS_LIST), LB_GETCOUNT, 0, 0);
}

static int rows_holding(HWND list, const wchar_t *words)
{
    static wchar_t row[SHOWN_CAPACITY];
    LRESULT count = SendMessageW(list, LB_GETCOUNT, 0, 0);
    LRESULT index;
    int holding = 0;

    for (index = 0; (index < count) && (index < ROW_LIMIT); ++index) {
        row[0] = L'\0';
        if (SendMessageW(list, LB_GETTEXTLEN, (WPARAM)index, 0) < SHOWN_CAPACITY) {
            (void)SendMessageW(list, LB_GETTEXT, (WPARAM)index, (LPARAM)row);
        }
        (void)CharLowerBuffW(row, (DWORD)wcslen(row));
        holding += (wcsstr(row, words) != NULL) ? 1 : 0;
    }
    return holding;
}

bool test_a_filter_leaves_the_bonuses_that_hold_it_and_no_other(void)
{
    HWND window = app_open(GetModuleHandleW(NULL));
    HWND list = control_of(window, CONTROL_BONUS_LIST);
    int holding = rows_holding(list, L"food");

    type_into(window, CONTROL_BONUS_FILTER, L"food");
    return closed(window, (holding > 0) && (bonus_rows(window) == holding) && (rows_holding(list, L"food") == holding));
}

bool test_the_categories_together_hold_every_bonus_once(void)
{
    HWND window = app_open(GetModuleHandleW(NULL));
    LRESULT all = bonus_rows(window);
    LRESULT rows = SendMessageW(control_of(window, CONTROL_BONUS_CATEGORY), CB_GETCOUNT, 0, 0);
    LRESULT sum = 0;
    LRESULT row;

    for (row = 1; (row < rows) && (row < ROW_LIMIT); ++row) {
        choose_row(window, CONTROL_BONUS_CATEGORY, (int)row, CB_SETCURSEL, CBN_SELCHANGE);
        sum += bonus_rows(window);
    }
    return closed(window, (rows > 2) && (sum == all));
}

bool test_the_window_opens_with_the_focus_on_the_civ_to_edit(void)
{
    HWND window = app_open(GetModuleHandleW(NULL));

    return closed(window, GetFocus() == control_of(window, CONTROL_CIV));
}

bool test_the_menu_go_names_each_step_as_its_title_reads(void)
{
    HWND window = app_open(GetModuleHandleW(NULL));
    HMENU go = GetSubMenu(GetMenu(window), GO_MENU);
    wchar_t item[SHOWN_CAPACITY];
    int index;

    shown[0] = L'\0';
    for (index = FIRST_STEP_ITEM; (index < GetMenuItemCount(go)) && (index < ROW_LIMIT); ++index) {
        item[0] = L'\0';
        (void)GetMenuStringW(go, (UINT)index, item, SHOWN_CAPACITY, MF_BYPOSITION);
        (void)wcsncat(shown, item, SHOWN_CAPACITY - wcslen(shown) - 1U);
        (void)wcsncat(shown, L"|", SHOWN_CAPACITY - wcslen(shown) - 1U);
    }
    return closed(window, wcscmp(shown, GO_STEPS) == 0);
}

bool test_a_wheel_turn_on_the_minimized_window_leaves_the_step_unscrolled(void)
{
    HWND window = app_open(GetModuleHandleW(NULL));

    (void)SetWindowPos(window, NULL, FAR_AWAY, FAR_AWAY, SMALL_WIDTH, SMALL_HEIGHT,
                       SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW);
    (void)ShowWindow(window, SW_SHOWMINNOACTIVE);
    (void)SendMessageW(window, WM_MOUSEWHEEL, MAKEWPARAM(0, (WORD)-NOTCH), 0);
    (void)ShowWindow(window, SW_SHOWNOACTIVATE);
    return closed(window, GetScrollPos(GetDlgItem(window, WINDOW_BAR_ID), SB_CTL) == 0);
}

static LRESULT heroes_in_catalog(void)
{
    LRESULT heroes = 0;
    int unit;

    if (!CHECK(catalog_load(&catalog, L"data"))) {
        return -1;
    }
    for (unit = 0; unit < catalog.unit_count; ++unit) {
        heroes += (catalog.units[unit].role == UNIT_ROLE_HERO) ? 1 : 0;
    }
    return heroes;
}

static HWND chose_hero(const wchar_t *name)
{
    HWND window = app_open(GetModuleHandleW(NULL));

    window_fixture_act(window, ACTION_FIRST_STEP + (int)STEP_UU_UT);
    window_fixture_tick(window, CONTROL_HERO);
    window_fixture_answer(L"Saladin", name);
    window_fixture_click(window, CONTROL_HERO_CHOOSE);
    return window;
}

bool test_choose_hero_lists_every_hero_of_the_catalog_once(void)
{
    HWND window = chose_hero(NULL);

    return closed(window, window_fixture_asked()->rows == heroes_in_catalog());
}

bool test_the_list_of_choose_hero_is_labelled_heroes(void)
{
    HWND window = chose_hero(NULL);

    return closed(window, wcscmp(window_fixture_asked()->list_label, L"Heroes") == 0);
}

bool test_choose_hero_opens_with_the_focus_on_its_list(void)
{
    HWND window = chose_hero(NULL);

    return closed(window, window_fixture_asked()->list_in_focus);
}

static bool hero_is_named(HWND window, const wchar_t *name)
{
    shown[0] = L'\0';
    (void)GetWindowTextW(control_of(window, CONTROL_HERO_NAME), shown, SHOWN_CAPACITY);
    return closed(window, wcscmp(shown, name) == 0);
}

bool test_a_name_typed_in_choose_hero_stays_when_its_list_takes_the_focus_again(void)
{
    window_fixture_refocus();
    return hero_is_named(chose_hero(L"Testhero"), L"Testhero");
}

static const wchar_t *first_hero(void)
{
    static wchar_t name[SHOWN_CAPACITY];
    int unit;

    name[0] = L'\0';
    if (!CHECK(catalog_load(&catalog, L"data"))) {
        return name;
    }
    for (unit = 0; (unit < catalog.unit_count) && (name[0] == L'\0'); ++unit) {
        if (catalog.units[unit].role == UNIT_ROLE_HERO) {
            (void)dialogs_widen(catalog.units[unit].name, name, SHOWN_CAPACITY);
        }
    }
    return name;
}

bool test_the_first_hero_of_choose_hero_can_be_picked(void)
{
    HWND window = chose_hero(NULL);
    const wchar_t *first = first_hero();

    window_fixture_answer(first, NULL);
    window_fixture_click(window, CONTROL_HERO_CHOOSE);
    return hero_is_named(window, first);
}

bool test_a_one_letter_name_typed_in_choose_hero_is_taken(void)
{
    return hero_is_named(chose_hero(L"X"), L"X");
}
