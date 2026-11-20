#include "dialog_tests.h"

#include <wchar.h>
#include <windows.h>

#include "../../src/win/win_parts.h"
#include "../accessibility/window_fixture.h"

enum { NAME_CAPACITY = 64 };

static HWND uu_ut_step(void)
{
    HWND window = app_open(GetModuleHandleW(NULL));

    window_fixture_act(window, ACTION_FIRST_STEP + (int)STEP_UU_UT);
    window_fixture_tick(window, CONTROL_HERO);
    return window;
}

static const wchar_t *shown_text(HWND window, ControlId control)
{
    static wchar_t text[NAME_CAPACITY];
    HWND shown = GetDlgItem(window, CONTROL_ID_BASE + (int)control);

    text[0] = L'\0';
    if ((GetWindowLongPtrW(shown, GWL_STYLE) & WS_VISIBLE) != 0) {
        (void)GetWindowTextW(shown, text, NAME_CAPACITY);
    }
    return text;
}

static bool hero_named(HWND window, const wchar_t *name)
{
    bool named = wcscmp(shown_text(window, CONTROL_HERO_NAME), name) == 0;

    window_fixture_close(window);
    return named;
}

bool test_ticking_hero_names_attila_the_hun_above_choose_hero(void)
{
    return hero_named(uu_ut_step(), L"Attila the Hun");
}

bool test_unticking_hero_hides_choose_hero(void)
{
    HWND window = uu_ut_step();
    bool hidden;

    window_fixture_tick(window, CONTROL_HERO);
    hidden = (GetWindowLongPtrW(GetDlgItem(window, CONTROL_ID_BASE + CONTROL_HERO_CHOOSE), GWL_STYLE) &
              WS_VISIBLE) == 0;
    window_fixture_close(window);
    return hidden;
}

bool test_a_hero_picked_in_choose_hero_keeps_the_games_name(void)
{
    HWND window = uu_ut_step();

    window_fixture_answer(L"Saladin", NULL);
    window_fixture_click(window, CONTROL_HERO_CHOOSE);
    return hero_named(window, L"Saladin");
}

bool test_a_hero_renamed_in_choose_hero_takes_the_new_name(void)
{
    HWND window = uu_ut_step();

    window_fixture_answer(L"Saladin", L"Testhero");
    window_fixture_click(window, CONTROL_HERO_CHOOSE);
    return hero_named(window, L"Testhero");
}
