#include "requirement_tests.h"

#include <string.h>
#include <wchar.h>
#include <windows.h>

#include "../../src/core/check.h"
#include "../../src/forge/forge.h"
#include "../../src/forge/recipe.h"
#include "../../src/mod/mod_places.h"
#include "../../src/win/win_parts.h"
#include "../accessibility/window_fixture.h"
#include "../dat/recipe_tests.h"
#include "../places/places_tests.h"
#include "../shell/dialog_tests.h"

enum { BONUS_LIMIT = 24, NOTCH = 120, FAR_AWAY = -20000, SLOWEST_MOVE = 150 };

static const wchar_t DEFAULT_FILE[] = L"build\\civeditor_civ.civeditor.json";
static const wchar_t PROFILE_MODS[] = L"Games\\Age of Empires 2 DE\\7656\\mods\\local";

static History history;
static wchar_t found[FILE_PATH_CAPACITY];

bool test_the_local_mods_folder_of_a_profile_is_found(void)
{
    return places_write_under(L"test_home", PROFILE_MODS, L"mod.txt", "a dat file") &&
           mod_find_mods_under(L"build\\test_home", found, FILE_PATH_CAPACITY) &&
           (wcsstr(found, L"7656\\mods\\local") != NULL);
}

bool test_undo_brings_back_the_civ_before_a_change(void)
{
    Command bonus = command_of(COMMAND_TOGGLE_BONUS, 0, 7);

    history_start(&history);
    (void)history_do(&history, &bonus);
    return history_step(&history, true) && (history.present.bonus_count == 0);
}

bool test_a_25th_bonus_is_refused(void)
{
    CivDesign design;
    int bonus;

    civ_design_reset(&design);
    for (bonus = 0; bonus < BONUS_LIMIT; ++bonus) {
        (void)civ_design_toggle_bonus(&design, bonus);
    }
    return !civ_design_toggle_bonus(&design, BONUS_LIMIT) && (design.bonus_count == BONUS_LIMIT);
}

bool test_the_mod_folder_name_is_made_from_the_mod_name(void)
{
    char prefix[FORGE_PREFIX_CAPACITY];

    if (!CHECK(forge_prefix("\xC3\x86sir Kings 2!", prefix, sizeof prefix))) {
        return false;
    }
    return strcmp(prefix, "_sir_kings_2_") == 0;
}

bool test_the_replaced_civ_loses_its_own_techs(void)
{
    return recipe_game_tech(OLD_MAKE_TECH).civ == RECIPE_LOST_CIV;
}

bool test_the_report_counts_a_civ_bonus_the_catalog_lacks_as_skipped(void)
{
    return recipe_game_report(FULL_CIV).bonuses_skipped == 1;
}

bool test_a_new_civ_is_saved_as_civeditor_civ_by_default(void)
{
    return dialog_saves_anew(NULL, DEFAULT_FILE);
}

bool test_a_wheel_turn_between_the_lists_redraws_nothing_when_the_step_fits(void)
{
    HWND window = app_open(GetModuleHandleW(NULL));
    bool still;

    (void)SetWindowPos(window, NULL, FAR_AWAY, FAR_AWAY, 0, 0,
                       SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW);
    (void)UpdateWindow(window);
    (void)SendMessageW(window, WM_MOUSEWHEEL, MAKEWPARAM(0, (WORD)-NOTCH), 0);
    still = GetUpdateRect(window, NULL, FALSE) == FALSE;
    window_fixture_close(window);
    return still;
}

bool test_a_dropdown_opens_without_animation(void)
{
    HWND choice = dropdown_shown_choice();

    return dropdown_closed_with(choice, dropdown_time_to_open(choice) < SLOWEST_MOVE);
}

bool test_a_number_of_the_team_bonus_is_set_in_its_field(void)
{
    HWND window = app_open(GetModuleHandleW(NULL));
    HWND field;
    bool set;

    window_fixture_act(window, ACTION_FIRST_STEP + (int)STEP_BONUSES);
    field = window_fixture_panel_field(window, CONTROL_TEAM_INFO, L"Archery Ranges work ", 0);
    (void)SetWindowTextW(field, L"20");
    (void)SendMessageW(GetParent(field), WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(field), EN_KILLFOCUS), (LPARAM)field);
    set = window_fixture_panel_row(window, CONTROL_TEAM_INFO, L"Archery Ranges work 20% faster") != NULL;
    window_fixture_close(window);
    return set;
}

bool test_a_list_that_is_erased_keeps_the_picture_of_its_rows(void)
{
    HWND list = scroll_shown_list(FAR_AWAY);

    return scroll_closed_with(list, scroll_erased_pixel(list, 1) == RGB(0, 0, 0));
}
