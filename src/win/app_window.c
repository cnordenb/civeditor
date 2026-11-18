#include "win_parts.h"

#include "../base/base_civ.h"
#include "../core/check.h"
#include "../core/text.h"
#include "../gametext/game_text.h"
#include "../shell/shell.h"

enum {
    MESSAGE_LIMIT = 2147483647,
    START_WIDTH = 1200,
    START_HEIGHT = 760,
    WINDOW_STYLE = WS_OVERLAPPEDWINDOW,
    WINDOW_EXTRA_STYLE = WS_EX_CONTROLPARENT,
    BUTTON_COUNT = 4,
    NO_ACTION = -1
};

typedef struct {
    bool done;
    LRESULT result;
} Handled;

static const wchar_t CLASS_NAME[] = L"civbuilder.window";
static AppState app;

static void focus_step(void)
{
    int control;

    for (control = 0; control < CONTROL_COUNT; ++control) {
        if ((layout_spec((ControlId)control).step == app.step) && layout_is_focusable((ControlId)control)) {
            (void)SetFocus(app.controls[control]);
            return;
        }
    }
}

static void go_to_step(WizardStep step, bool focus)
{
    if (!CHECK((int)step >= 0) || !CHECK(step < STEP_COUNT)) {
        return;
    }
    app.step = step;
    controls_show_step(&app);
    sync_details(&app);
    if (focus) {
        focus_step();
    }
}

static void move_in_history(bool undo)
{
    bool moved = history_step(&app.history, undo);

    if (!moved) {
        dialogs_status(&app, undo ? "There is nothing to undo." : "There is nothing to redo.");
        return;
    }
    app.unsaved = true;
    sync_design(&app, false);
    dialogs_status(&app, undo ? "The last change was undone." : "The change was made again.");
}

static bool run_file_action(int action)
{
    if (!CHECK(action >= 0) || !CHECK(action < ACTION_COUNT)) {
        return false;
    }
    if (action == ACTION_SAVE) {
        (void)dialogs_save(&app);
        return true;
    }
    if (action == ACTION_OPEN) {
        (void)dialogs_open(&app);
        return true;
    }
    if (action == ACTION_BUILD) {
        (void)dialogs_build(&app);
        return true;
    }
    if (action == ACTION_ADD) {
        sync_save_civ(&app);
        return true;
    }
    return false;
}

static bool run_other_action(int action)
{
    if (!CHECK(action >= 0) || !CHECK(action < ACTION_COUNT)) {
        return false;
    }
    if (action == ACTION_EXIT) {
        (void)PostMessageW(app.window, WM_CLOSE, 0, 0);
        return true;
    }
    if (action == ACTION_HELP) {
        assist_show_help(&app);
        return true;
    }
    return false;
}

static bool run_history_action(int action)
{
    if (!CHECK(action >= 0) || ((action != ACTION_UNDO) && (action != ACTION_REDO))) {
        return false;
    }
    move_in_history(action == ACTION_UNDO);
    return true;
}

static bool run_scroll_action(int action)
{
    if (!CHECK(action >= 0) || ((action != ACTION_SCROLL_UP) && (action != ACTION_SCROLL_DOWN))) {
        return false;
    }
    assist_scroll(&app, WM_VSCROLL, (action == ACTION_SCROLL_UP) ? SB_PAGEUP : SB_PAGEDOWN);
    return true;
}

static WizardStep step_of_action(int action)
{
    if (action == ACTION_NEXT) {
        return wizard_move(app.step, 1);
    }
    return (action == ACTION_BACK) ? wizard_move(app.step, -1) : (WizardStep)(action - ACTION_FIRST_STEP);
}

static void run_action(int action)
{
    if (!CHECK(action >= 0) || !CHECK(action < ACTION_COUNT)) {
        return;
    }
    if (run_file_action(action) || run_history_action(action) || run_other_action(action) ||
        run_scroll_action(action)) {
        return;
    }
    go_to_step(step_of_action(action), true);
}

static int button_action(int control)
{
    static const int BUTTONS[BUTTON_COUNT][2] = { { CONTROL_BACK, ACTION_BACK },
                                                  { CONTROL_NEXT, ACTION_NEXT },
                                                  { CONTROL_ADD, ACTION_ADD },
                                                  { CONTROL_BUILD, ACTION_BUILD } };
    int button;

    if ((control < 0) || (control >= CONTROL_COUNT)) {
        return NO_ACTION;
    }
    for (button = 0; button < BUTTON_COUNT; ++button) {
        if (BUTTONS[button][0] == control) {
            return BUTTONS[button][1];
        }
    }
    return NO_ACTION;
}

static bool is_menu_action(int identifier, LPARAM lparam)
{
    return (lparam == 0) && (identifier >= ACTION_ID_BASE) && (identifier < ACTION_ID_BASE + ACTION_COUNT);
}

static bool is_step_pick(int control, int code)
{
    return (control == (int)CONTROL_STEPS) && (code == LBN_SELCHANGE) && !app.syncing;
}

static void hide_report_caret(int control, int code, LPARAM lparam)
{
    if ((code == EN_SETFOCUS) && (layout_spec((ControlId)control).kind == WIDGET_REPORT)) {
        (void)HideCaret((HWND)lparam);
    }
}

static void on_command(WPARAM wparam, LPARAM lparam)
{
    int identifier = (int)LOWORD(wparam);
    int code = (int)HIWORD(wparam);
    int control = identifier - CONTROL_ID_BASE;
    int clicked = (code == BN_CLICKED) ? button_action(control) : NO_ACTION;

    hide_report_caret(control, code, lparam);
    assist_follow_focus(&app);
    if (is_menu_action(identifier, lparam)) {
        run_action(identifier - ACTION_ID_BASE);
    } else if (clicked != NO_ACTION) {
        run_action(clicked);
    } else if (is_step_pick(control, code)) {
        go_to_step((WizardStep)(int)SendMessageW(app.controls[CONTROL_STEPS], LB_GETCURSEL, 0, 0), false);
    } else {
        sync_on_notify(&app, control, code);
    }
}

static void read_game_texts(void)
{
    wchar_t game[FILE_PATH_CAPACITY];

    if (ask_game_folder(game)) {
        (void)game_text_read(&app.catalog, game);
    }
}

static bool on_create(HWND window)
{
    app.window = window;
    assist_make_bar(&app);
    if (!controls_create(&app)) {
        return false;
    }
    if (!dialogs_load_catalog(&app)) {
        dialogs_status(&app, "The catalog in data\\ was not found: the lists are empty.");
    } else {
        read_game_texts();
        (void)base_civ_apply(&app.history.present, &app.catalog, CIV_DEFAULT_REPLACE);
    }
    controls_place(&app);
    sync_design(&app, true);
    go_to_step(STEP_IDENTITY, true);
    return true;
}

static void limit_size(MINMAXINFO *limits)
{
    RECT frame = { 0, 0, 0, 0 };

    frame.right = MulDiv(REFLOW_MIN_WIDTH, app.dpi, LAYOUT_BASE_DPI);
    frame.bottom = MulDiv(REFLOW_MIN_HEIGHT, app.dpi, LAYOUT_BASE_DPI);
    (void)AdjustWindowRectEx(&frame, WINDOW_STYLE, TRUE, WINDOW_EXTRA_STYLE);
    limits->ptMinTrackSize.x = frame.right - frame.left;
    limits->ptMinTrackSize.y = frame.bottom - frame.top;
}

static LRESULT paint_background(HWND window, HDC context)
{
    RECT client;

    if (GetClientRect(window, &client) == 0) {
        return 0;
    }
    (void)FillRect(context, &client, app.paper_brush);
    assist_draw_focus(&app, context);
    return 1;
}

static LRESULT color_control(WPARAM wparam, LPARAM lparam)
{
    HBRUSH brush = controls_color(&app, (HDC)wparam, (HWND)lparam);

    return (LRESULT)brush;
}

static Handled handled_with(LRESULT result)
{
    Handled handled = { true, 0 };

    handled.result = result;
    return handled;
}

enum { COLOR_MESSAGE_COUNT = 4 };

static const int COLOR_MESSAGES[COLOR_MESSAGE_COUNT] = { WM_CTLCOLORSTATIC, WM_CTLCOLOREDIT, WM_CTLCOLORLISTBOX,
                                                         WM_CTLCOLORBTN };

static bool is_color_message(UINT message)
{
    return check_is_among((int)message, COLOR_MESSAGES, COLOR_MESSAGE_COUNT);
}

static Handled handle_drawing(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    const Handled passed = { false, 0 };

    if (is_color_message(message)) {
        return handled_with(color_control(wparam, lparam));
    }
    if (message == WM_DRAWITEM) {
        controls_draw_item(&app, (const DRAWITEMSTRUCT *)lparam);
        return handled_with(TRUE);
    }
    if (message == WM_MEASUREITEM) {
        ((MEASUREITEMSTRUCT *)lparam)->itemHeight = (UINT)controls_row_height(&app);
        return handled_with(TRUE);
    }
    return (message == WM_ERASEBKGND) ? handled_with(paint_background(window, (HDC)wparam)) : passed;
}

static Handled handle_frame(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    const Handled passed = { false, 0 };

    if (message == WM_CREATE) {
        return handled_with(on_create(window) ? 0 : -1);
    }
    if (message == WM_SIZE) {
        controls_place(&app);
        return handled_with(0);
    }
    if ((message == WM_VSCROLL) || (message == WM_MOUSEWHEEL)) {
        assist_scroll(&app, message, wparam);
        return handled_with(0);
    }
    if (message == WM_GETMINMAXINFO) {
        limit_size((MINMAXINFO *)lparam);
        return handled_with(0);
    }
    return passed;
}

static Handled handle_ending(HWND window, UINT message)
{
    const Handled passed = { false, 0 };

    if ((message != WM_CLOSE) && (message != WM_DESTROY)) {
        return passed;
    }
    if (message == WM_DESTROY) {
        PostQuitMessage(0);
    } else if (dialogs_may_discard(&app)) {
        (void)DestroyWindow(window);
    }
    return handled_with(0);
}

static Handled handle_input(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    const Handled passed = { false, 0 };

    if (message == WM_COMMAND) {
        on_command(wparam, lparam);
        return handled_with(0);
    }
    if (message == WM_NEXTDLGCTL) {
        assist_move_focus(&app, wparam, lparam);
        return handled_with(0);
    }
    if ((message == WM_SETTINGCHANGE) || (message == WM_SYSCOLORCHANGE)) {
        controls_refresh_style(&app);
        (void)InvalidateRect(window, NULL, TRUE);
        return handled_with(0);
    }
    return passed;
}

static LRESULT CALLBACK window_procedure(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    Handled handled;

    handled = handle_frame(window, message, wparam, lparam);
    if (!handled.done) {
        handled = handle_drawing(window, message, wparam, lparam);
    }
    if (!handled.done) {
        handled = handle_input(window, message, wparam, lparam);
    }
    if (!handled.done) {
        handled = handle_ending(window, message);
    }
    return handled.done ? handled.result : DefWindowProcW(window, message, wparam, lparam);
}

static bool register_class(HINSTANCE instance)
{
    WNDCLASSEXW window_class = { 0 };

    window_class.cbSize = sizeof window_class;
    window_class.lpfnWndProc = window_procedure;
    window_class.hInstance = instance;
    window_class.hCursor = LoadCursorW(NULL, IDC_ARROW);
    window_class.hIcon = LoadIconW(NULL, IDI_APPLICATION);
    window_class.lpszClassName = CLASS_NAME;
    return (RegisterClassExW(&window_class) != 0) || (GetLastError() == ERROR_CLASS_ALREADY_EXISTS);
}

int app_run_messages(void)
{
    MSG message;
    BOOL received;
    int step;

    for (step = 0; step < MESSAGE_LIMIT; ++step) {
        received = GetMessageW(&message, NULL, 0, 0);
        if (received <= 0) {
            return (received == 0) ? (int)message.wParam : 1;
        }
        assist_run_message(&app, &message);
    }
    return 0;
}

HWND app_open(HINSTANCE instance)
{
    if (!register_class(instance)) {
        return NULL;
    }
    app.instance = instance;
    (void)SetProcessPreferredUILanguages(MUI_LANGUAGE_NAME, L"en-US\0", NULL);
    (void)CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    app.dpi = assist_dpi(NULL);
    app.bonus_category = LIST_ALL_CATEGORIES;
    app.unsaved = false;
    app.file_path[0] = L'\0';
    (void)lstrcpynW(app.mod_name, L"civbuilder_mod", MOD_NAME_CAPACITY);
    history_start(&app.history);
    roster_start(&app.roster);
    app.accelerators = menu_keys();
    app.window = CreateWindowExW(WINDOW_EXTRA_STYLE, CLASS_NAME, L"civbuilder", WINDOW_STYLE, CW_USEDEFAULT,
                                 CW_USEDEFAULT, MulDiv(START_WIDTH, app.dpi, LAYOUT_BASE_DPI),
                                 MulDiv(START_HEIGHT, app.dpi, LAYOUT_BASE_DPI), NULL, menu_make(), instance, NULL);
    return app.window;
}

int app_run(HINSTANCE instance, int show)
{
    if (!CHECK(app_open(instance) != NULL)) {
        return 1;
    }
    (void)ShowWindow(app.window, show);
    (void)UpdateWindow(app.window);
    return app_run_messages();
}
