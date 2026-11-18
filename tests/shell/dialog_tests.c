#include "dialog_tests.h"

#include <wchar.h>

#include "../../src/core/check.h"
#include "../../src/mod/mod_places.h"
#include "../../src/shell/shell.h"
#include "../../src/win/win_parts.h"
#include "../accessibility/window_fixture.h"
#include "../mod/game_folder.h"
#include "../test_outcome.h"

enum { TEXT_CAPACITY = 64, PUMP_LIMIT = 4096, ROW_WIDTH = 200, ROW_HEIGHT = 40, NO_ACTION = -1 };

typedef struct {
    int answer;
    const wchar_t *save_folder;
    const wchar_t *open_path;
    const wchar_t *steam;
    const wchar_t *mod_name;
} Answers;

static const wchar_t BUILD[] = L"build";
static const wchar_t DEFAULT_FILE[] = L"build\\civbuilder_civ.civbuilder.json";
static const wchar_t NAMED_FILE[] = L"build\\Testonia.civbuilder.json";
static const wchar_t HOME[] = L"build\\test_home";
static const wchar_t PROFILE[] = L"test_home\\Games\\Age of Empires 2 DE\\7656\\mods\\local";
static const wchar_t NAME[] = L"Testonia";
static const char STALE[] = "stale";

static const Answers YES = { IDOK, NULL, NULL, GAME_FOLDER_STEAM, NULL };
static const Answers NAMING = { IDOK, NULL, NULL, GAME_FOLDER_STEAM, L"Testonia" };
static const Answers CANCEL = { IDCANCEL, NULL, NULL, GAME_FOLDER_STEAM, NULL };
static const Answers NO = { IDNO, NULL, NULL, GAME_FOLDER_STEAM, NULL };
static const Answers SAVING = { IDOK, BUILD, NULL, GAME_FOLDER_STEAM, NULL };
static const Answers SAVING_AND_OPENING = { IDNO, BUILD, NAMED_FILE, GAME_FOLDER_STEAM, NULL };
static const Answers OPENING_NO_CIV = { IDOK, NULL, L"README.md", GAME_FOLDER_STEAM, NULL };
static const Answers WITHOUT_GAME = { IDOK, NULL, NULL, L"build\\no_steam", NULL };

static char saved[TEXT_CAPACITY];

static HWND opened(const Answers *answers)
{
    AskScript script = { true, IDOK, NULL, NULL, NULL, HOME, NULL };

    if (!CHECK(answers != NULL) || !CHECK(mod_make_folders(BUILD, PROFILE)) || !CHECK(game_folder_is_made())) {
        return NULL;
    }
    script.answer = answers->answer;
    script.save_folder = answers->save_folder;
    script.open_path = answers->open_path;
    script.steam = answers->steam;
    script.text = answers->mod_name;
    ask_script(&script);
    return app_open(GetModuleHandleW(NULL));
}

static bool closed(HWND window, bool passed)
{
    char said[STATUS_CAPACITY] = "";

    if (!passed && dialogs_narrow(ask_last_message(), said, STATUS_CAPACITY)) {
        test_note(said);
    }
    window_fixture_close(window);
    ask_script(NULL);
    return passed && CHECK(window != NULL);
}

static HWND control_of(HWND window, ControlId control)
{
    return CHECK(window != NULL) ? GetDlgItem(window, CONTROL_ID_BASE + (int)control) : NULL;
}

static void notify(HWND window, ControlId control, int code)
{
    HWND sender = control_of(window, control);

    if (!CHECK(window != NULL) || !CHECK(code >= 0)) {
        return;
    }
    (void)SendMessageW(window, WM_COMMAND, MAKEWPARAM(CONTROL_ID_BASE + (int)control, code), (LPARAM)sender);
}

static void type_into(HWND window, ControlId control, const wchar_t *text)
{
    if (!CHECK(window != NULL) || (text == NULL)) {
        return;
    }
    (void)SetWindowTextW(control_of(window, control), text);
    notify(window, control, EN_CHANGE);
}

static void pick_first(HWND window, ControlId list)
{
    if (!CHECK(window != NULL) || !CHECK((int)list < CONTROL_COUNT)) {
        return;
    }
    (void)SendMessageW(control_of(window, list), LB_SETCURSEL, 0, 0);
    notify(window, list, LBN_SELCHANGE);
}

static bool text_holds(HWND shown, const wchar_t *words)
{
    wchar_t text[STATUS_CAPACITY] = L"";

    if (!CHECK(shown != NULL) || !CHECK(words != NULL)) {
        return false;
    }
    (void)GetWindowTextW(shown, text, STATUS_CAPACITY);
    return wcsstr(text, words) != NULL;
}

static HWND opened_and_run(const Answers *answers, const wchar_t *name, int action)
{
    HWND window = opened(answers);

    type_into(window, CONTROL_NAME, name);
    if (action != NO_ACTION) {
        window_fixture_act(window, action);
    }
    return window;
}

static bool status_after(const Answers *answers, int action, const wchar_t *words)
{
    HWND window = opened_and_run(answers, NULL, action);

    return closed(window, text_holds(control_of(window, CONTROL_STATUS), words));
}

static bool message_after(const Answers *answers, int action, const wchar_t *words)
{
    HWND window = opened_and_run(answers, NULL, action);

    return closed(window, wcsstr(ask_last_message(), words) != NULL);
}

static bool title_after(const Answers *answers, int action, const wchar_t *words)
{
    HWND window = opened_and_run(answers, NAME, action);

    if (action == ACTION_SAVE) {
        type_into(window, CONTROL_NAME, L"Other");
        window_fixture_act(window, ACTION_OPEN);
    }
    return closed(window, text_holds(window, words));
}

static bool saves_anew(const wchar_t *name, const wchar_t *path)
{
    HWND window;
    FileRead read;

    if (!CHECK(path != NULL) || !CHECK(file_write_all(path, STALE, sizeof STALE - 1U))) {
        return false;
    }
    window = opened_and_run(&SAVING, name, ACTION_SAVE);
    read = file_read_all(path, saved, sizeof saved);
    return closed(window, (read.length > 0U) && (saved[0] == '{'));
}

static bool window_stays(const Answers *answers, const wchar_t *name, int action)
{
    HWND window = opened_and_run(answers, name, action);
    MSG message;
    int step;

    if (action == NO_ACTION) {
        (void)SendMessageW(window, WM_CLOSE, 0, 0);
    }
    for (step = 0; (step < PUMP_LIMIT) && (PeekMessageW(&message, NULL, 0, 0, PM_REMOVE) != 0); ++step) {
        (void)DispatchMessageW(&message);
    }
    return closed(window, IsWindow(window) != 0);
}

static LRESULT step_after_next(void)
{
    HWND window = opened(&YES);
    LRESULT step;

    notify(window, CONTROL_NEXT, BN_CLICKED);
    step = SendMessageW(control_of(window, CONTROL_STEPS), LB_GETCURSEL, 0, 0);
    return closed(window, true) ? step : -1;
}

static COLORREF colour_of(ColorRole role)
{
    Rgb colour = palette_color(role);

    return RGB(colour.red, colour.green, colour.blue);
}

static COLORREF colour_given(UINT message, ControlId control, bool background)
{
    HWND window = opened(&YES);
    HWND asked = control_of(window, control);
    HDC memory = CreateCompatibleDC(NULL);
    COLORREF given;

    (void)SendMessageW(window, message, (WPARAM)memory, (LPARAM)asked);
    given = background ? GetBkColor(memory) : GetTextColor(memory);
    (void)DeleteDC(memory);
    return closed(window, true) ? given : CLR_INVALID;
}

bool test_a_new_civ_is_saved_as_civbuilder_civ_by_default(void)
{
    return saves_anew(NULL, DEFAULT_FILE);
}

bool test_a_named_civ_is_saved_under_its_name(void)
{
    return saves_anew(NAME, NAMED_FILE);
}

bool test_a_save_that_is_cancelled_says_that_the_civ_was_not_saved(void)
{
    return status_after(&YES, ACTION_SAVE, L"The civ was not saved.");
}

bool test_a_saved_civ_is_opened_again(void)
{
    return title_after(&SAVING_AND_OPENING, ACTION_SAVE, L"Testonia - Step 1");
}

bool test_a_file_that_is_no_civ_file_is_refused_with_a_message(void)
{
    return message_after(&OPENING_NO_CIV, ACTION_OPEN, L"not a civ file");
}

bool test_the_next_button_goes_to_the_next_step(void)
{
    return step_after_next() == 1;
}

bool test_a_typed_name_is_shown_in_the_title_of_the_window(void)
{
    return title_after(&YES, NO_ACTION, L"Testonia - Step 1");
}

bool test_redo_says_when_there_is_nothing_to_redo(void)
{
    return status_after(&YES, ACTION_REDO, L"There is nothing to redo.");
}

bool test_no_step_is_shown_as_done_when_the_window_opens(void)
{
    HWND window = opened(&YES);
    wchar_t row[STATUS_CAPACITY] = L"";
    bool none = true;
    int step;

    for (step = 0; (step < STEP_COUNT) && none; ++step) {
        none = (SendMessageW(control_of(window, CONTROL_STEPS), LB_GETTEXT, (WPARAM)step, (LPARAM)row) > 0) &&
               (wcsstr(row, L"(done)") == NULL);
        if (!none) {
            test_note("a step shown as done");
        }
    }
    return closed(window, none);
}

bool test_closing_is_stopped_when_the_question_of_saving_is_cancelled(void)
{
    return window_stays(&CANCEL, NAME, NO_ACTION);
}

bool test_closing_without_saving_closes_the_window(void)
{
    return !window_stays(&NO, NAME, NO_ACTION);
}

bool test_the_exit_command_closes_a_window_without_changes(void)
{
    return !window_stays(&YES, NULL, ACTION_EXIT);
}

bool test_build_is_refused_when_the_game_is_not_found(void)
{
    return message_after(&WITHOUT_GAME, ACTION_BUILD, L"cannot be built");
}

bool test_build_writes_nothing_when_the_question_is_not_confirmed(void)
{
    HWND window = opened_and_run(&CANCEL, NAME, ACTION_ADD);

    window_fixture_act(window, ACTION_BUILD);
    return closed(window, text_holds(control_of(window, CONTROL_STATUS), L"No mod was built."));
}

bool test_build_of_an_unfinished_civ_says_that_it_is_not_finished(void)
{
    HWND window = opened_and_run(&YES, L"", ACTION_ADD);

    window_fixture_act(window, ACTION_BUILD);
    return closed(window, wcsstr(ask_last_message(), L"not finished") != NULL);
}

bool test_build_from_the_window_reports_the_mod_it_built(void)
{
    HWND window = opened_and_run(&NAMING, NAME, NO_ACTION);

    pick_first(window, CONTROL_UNIT_LIST);
    pick_first(window, CONTROL_CASTLE_TECH_LIST);
    pick_first(window, CONTROL_IMPERIAL_TECH_LIST);
    window_fixture_act(window, ACTION_ADD);
    window_fixture_act(window, ACTION_BUILD);
    return closed(window, wcsstr(ask_last_message(), L"The mod was built as testonia-data") != NULL);
}

bool test_the_help_line_is_written_in_the_muted_colour(void)
{
    return colour_given(WM_CTLCOLORSTATIC, CONTROL_HELP, false) == colour_of(COLOR_ROLE_MUTED);
}

bool test_a_field_is_written_on_the_colour_of_fields(void)
{
    return colour_given(WM_CTLCOLOREDIT, CONTROL_NAME, true) == colour_of(COLOR_ROLE_FIELD);
}

bool test_a_row_in_focus_is_drawn_with_a_frame_in_the_focus_colour(void)
{
    HWND window = opened(&YES);
    HDC screen = GetDC(window);
    HDC memory = CreateCompatibleDC(screen);
    HBITMAP picture = CreateCompatibleBitmap(screen, ROW_WIDTH, ROW_HEIGHT);
    HGDIOBJ before = SelectObject(memory, picture);
    DRAWITEMSTRUCT item = { ODT_LISTBOX, CONTROL_ID_BASE + CONTROL_TREE_LIST, 0, ODA_DRAWENTIRE, ODS_FOCUS,
                            NULL, NULL, { 0, 0, ROW_WIDTH, ROW_HEIGHT }, 0 };
    bool framed;

    item.hwndItem = control_of(window, CONTROL_TREE_LIST);
    item.hDC = memory;
    (void)SendMessageW(window, WM_DRAWITEM, (WPARAM)item.CtlID, (LPARAM)&item);
    framed = GetPixel(memory, 0, ROW_HEIGHT / 2) == colour_of(COLOR_ROLE_FOCUS);
    (void)SelectObject(memory, before);
    (void)DeleteObject(picture);
    (void)DeleteDC(memory);
    (void)ReleaseDC(window, screen);
    return closed(window, framed);
}

bool test_picking_the_picked_row_again_makes_no_new_change(void)
{
    HWND window = opened(&YES);

    pick_first(window, CONTROL_UNIT_LIST);
    pick_first(window, CONTROL_UNIT_LIST);
    window_fixture_act(window, ACTION_UNDO);
    window_fixture_act(window, ACTION_UNDO);
    return closed(window, text_holds(control_of(window, CONTROL_STATUS), L"There is nothing to undo."));
}
