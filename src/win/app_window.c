#include "win_parts.h"

#include "../base/base_civ.h"
#include "../core/check.h"
#include "../core/text.h"

enum {
    MESSAGE_LIMIT = 2147483647,
    FIXED_KEY_COUNT = 9,
    KEY_COUNT = FIXED_KEY_COUNT + STEP_COUNT,
    START_WIDTH = 1200,
    START_HEIGHT = 760,
    MENU_TEXT_CAPACITY = 64,
    WINDOW_STYLE = WS_OVERLAPPEDWINDOW,
    WINDOW_EXTRA_STYLE = WS_EX_CONTROLPARENT
};

static const wchar_t CLASS_NAME[] = L"civbuilder.window";
static AppState app;

static const ACCEL FIXED_KEYS[FIXED_KEY_COUNT] = {
    { FCONTROL | FVIRTKEY, VK_NEXT, ACTION_ID_BASE + ACTION_NEXT },
    { FCONTROL | FVIRTKEY, VK_PRIOR, ACTION_ID_BASE + ACTION_BACK },
    { FCONTROL | FVIRTKEY, 'S', ACTION_ID_BASE + ACTION_SAVE },
    { FCONTROL | FVIRTKEY, 'O', ACTION_ID_BASE + ACTION_OPEN },
    { FCONTROL | FVIRTKEY, 'Z', ACTION_ID_BASE + ACTION_UNDO },
    { FCONTROL | FVIRTKEY, 'Y', ACTION_ID_BASE + ACTION_REDO },
    { FCONTROL | FSHIFT | FVIRTKEY, 'Z', ACTION_ID_BASE + ACTION_REDO },
    { FCONTROL | FVIRTKEY, 'B', ACTION_ID_BASE + ACTION_BUILD },
    { FVIRTKEY, VK_F1, ACTION_ID_BASE + ACTION_HELP }
};

static HACCEL make_accelerators(void)
{
    ACCEL keys[KEY_COUNT];
    int index;

    if (!CHECK(KEY_COUNT > FIXED_KEY_COUNT) || !CHECK(STEP_COUNT <= 9)) {
        return NULL;
    }
    for (index = 0; index < KEY_COUNT; ++index) {
        if (index < FIXED_KEY_COUNT) {
            keys[index] = FIXED_KEYS[index];
        } else {
            keys[index].fVirt = FCONTROL | FVIRTKEY;
            keys[index].key = (WORD)('1' + (index - FIXED_KEY_COUNT));
            keys[index].cmd = (WORD)(ACTION_ID_BASE + ACTION_FIRST_STEP + (index - FIXED_KEY_COUNT));
        }
    }
    return CreateAcceleratorTableW(keys, KEY_COUNT);
}

static void add_item(HMENU menu, int action, const wchar_t *text)
{
    if (!CHECK(menu != NULL) || !CHECK(text != NULL)) {
        return;
    }
    (void)AppendMenuW(menu, MF_STRING, (UINT_PTR)(ACTION_ID_BASE + action), text);
}

static HMENU make_go_menu(void)
{
    HMENU go = CreatePopupMenu();
    char text[MENU_TEXT_CAPACITY];
    wchar_t wide[MENU_TEXT_CAPACITY];
    int step;

    if (!CHECK(go != NULL) || !CHECK(STEP_COUNT > 0)) {
        return go;
    }
    add_item(go, ACTION_NEXT, L"&Next\tCtrl+Page Down");
    add_item(go, ACTION_BACK, L"&Back\tCtrl+Page Up");
    (void)AppendMenuW(go, MF_SEPARATOR, 0, NULL);
    for (step = 0; step < STEP_COUNT; ++step) {
        text[0] = '&';
        text[1] = '\0';
        (void)text_append_number(text, sizeof text, (long)step + 1L);
        (void)text_append(text, sizeof text, " ");
        (void)text_append(text, sizeof text, wizard_title((WizardStep)step));
        (void)text_append(text, sizeof text, "\tCtrl+");
        (void)text_append_number(text, sizeof text, (long)step + 1L);
        if (dialogs_widen(text, wide, MENU_TEXT_CAPACITY)) {
            add_item(go, ACTION_FIRST_STEP + step, wide);
        }
    }
    return go;
}

static HMENU make_menu(void)
{
    HMENU bar = CreateMenu();
    HMENU file = CreatePopupMenu();
    HMENU edit = CreatePopupMenu();
    HMENU go = make_go_menu();
    HMENU help = CreatePopupMenu();

    if (!CHECK(bar != NULL) || !CHECK((file != NULL) && (edit != NULL) && (go != NULL) && (help != NULL))) {
        return bar;
    }
    add_item(file, ACTION_OPEN, L"&Open civ file...\tCtrl+O");
    add_item(file, ACTION_SAVE, L"&Save civ file...\tCtrl+S");
    add_item(file, ACTION_BUILD, L"&Build mod...\tCtrl+B");
    (void)AppendMenuW(file, MF_SEPARATOR, 0, NULL);
    add_item(file, ACTION_EXIT, L"E&xit\tAlt+F4");
    add_item(edit, ACTION_UNDO, L"&Undo\tCtrl+Z");
    add_item(edit, ACTION_REDO, L"&Redo\tCtrl+Y");
    (void)AppendMenuW(bar, MF_POPUP, (UINT_PTR)file, L"&File");
    (void)AppendMenuW(bar, MF_POPUP, (UINT_PTR)edit, L"&Edit");
    (void)AppendMenuW(bar, MF_POPUP, (UINT_PTR)go, L"&Go");
    add_item(help, ACTION_HELP, L"&Help for this control\tF1");
    add_item(help, ACTION_GLOSSARY, L"&Glossary...");
    (void)AppendMenuW(bar, MF_POPUP, (UINT_PTR)help, L"&Help");
    return bar;
}

static void focus_step(void)
{
    int control;

    if (!CHECK(app.step < STEP_COUNT) || !CHECK(app.window != NULL)) {
        return;
    }
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

    if (!CHECK(app.window != NULL) || !CHECK(app.history.past.count <= HISTORY_DEPTH) || !moved) {
        dialogs_status(&app, undo ? "There is nothing to undo." : "There is nothing to redo.");
        return;
    }
    app.unsaved = true;
    sync_model(&app);
    dialogs_status(&app, undo ? "The last change was undone." : "The change was made again.");
}

static void run_action(int action)
{
    if (!CHECK(action >= 0) || !CHECK(action < ACTION_COUNT)) {
        return;
    }
    switch (action) {
    case ACTION_NEXT:
        go_to_step(wizard_move(app.step, 1), true);
        break;
    case ACTION_BACK:
        go_to_step(wizard_move(app.step, -1), true);
        break;
    case ACTION_SAVE:
        (void)dialogs_save(&app);
        break;
    case ACTION_OPEN:
        (void)dialogs_open(&app);
        break;
    case ACTION_UNDO:
    case ACTION_REDO:
        move_in_history(action == ACTION_UNDO);
        break;
    case ACTION_BUILD:
        (void)dialogs_build(&app);
        break;
    case ACTION_EXIT:
        (void)PostMessageW(app.window, WM_CLOSE, 0, 0);
        break;
    case ACTION_HELP:
        assist_show_help(&app);
        break;
    case ACTION_GLOSSARY:
        dialogs_glossary(&app);
        break;
    default:
        go_to_step((WizardStep)(action - ACTION_FIRST_STEP), true);
        break;
    }
}

static int button_action(int control)
{
    if (!CHECK(control >= 0) || !CHECK(control < CONTROL_COUNT)) {
        return -1;
    }
    switch (control) {
    case CONTROL_BACK:
        return ACTION_BACK;
    case CONTROL_NEXT:
        return ACTION_NEXT;
    case CONTROL_SAVE:
        return ACTION_SAVE;
    case CONTROL_OPEN:
        return ACTION_OPEN;
    case CONTROL_BUILD:
        return ACTION_BUILD;
    default:
        return -1;
    }
}

static void on_command(WPARAM wparam, LPARAM lparam)
{
    int identifier = (int)LOWORD(wparam);
    int code = (int)HIWORD(wparam);
    int control = identifier - CONTROL_ID_BASE;

    if (!CHECK(identifier >= 0) || !CHECK(code >= 0)) {
        return;
    }
    assist_follow_focus(&app);
    if ((lparam == 0) && (identifier >= ACTION_ID_BASE) && (identifier < ACTION_ID_BASE + ACTION_COUNT)) {
        run_action(identifier - ACTION_ID_BASE);
    } else if ((control >= 0) && (control < CONTROL_COUNT) && (code == BN_CLICKED) &&
               (button_action(control) >= 0)) {
        run_action(button_action(control));
    } else if ((control == (int)CONTROL_STEPS) && (code == LBN_SELCHANGE) && !app.syncing) {
        go_to_step((WizardStep)(int)SendMessageW(app.controls[CONTROL_STEPS], LB_GETCURSEL, 0, 0), false);
    } else if ((control >= 0) && (control < CONTROL_COUNT)) {
        sync_on_notify(&app, control, code);
    }
}

static bool on_create(HWND window)
{
    if (!CHECK(window != NULL) || !CHECK(app.instance != NULL)) {
        return false;
    }
    app.window = window;
    if (!controls_create(&app)) {
        return false;
    }
    if (!dialogs_load_catalog(&app)) {
        dialogs_status(&app, "The catalog in data\\ was not found: the lists are empty.");
    } else {
        (void)base_civ_apply(&app.history.present, &app.catalog, CIV_GENERIC_BASE);
    }
    controls_place(&app);
    sync_all(&app);
    go_to_step(STEP_IDENTITY, true);
    return true;
}

static void limit_size(MINMAXINFO *limits)
{
    RECT frame = { 0, 0, 0, 0 };

    if (!CHECK(limits != NULL) || !CHECK(app.dpi > 0)) {
        return;
    }
    frame.right = MulDiv(LAYOUT_MIN_WIDTH, app.dpi, LAYOUT_BASE_DPI);
    frame.bottom = MulDiv(LAYOUT_MIN_HEIGHT, app.dpi, LAYOUT_BASE_DPI);
    (void)AdjustWindowRectEx(&frame, WINDOW_STYLE, TRUE, WINDOW_EXTRA_STYLE);
    limits->ptMinTrackSize.x = frame.right - frame.left;
    limits->ptMinTrackSize.y = frame.bottom - frame.top;
}

static LRESULT paint_background(HWND window, HDC context)
{
    RECT client;

    if (!CHECK(window != NULL) || !CHECK(context != NULL) || (GetClientRect(window, &client) == 0)) {
        return 0;
    }
    (void)FillRect(context, &client, app.paper_brush);
    assist_draw_focus(&app, context);
    return 1;
}

static LRESULT color_control(WPARAM wparam, LPARAM lparam)
{
    HBRUSH brush = controls_color(&app, (HDC)wparam, (HWND)lparam);

    if (!CHECK(brush != NULL) || !CHECK(app.window != NULL)) {
        return 0;
    }
    return (LRESULT)brush;
}

static LRESULT CALLBACK window_procedure(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    if (!CHECK(window != NULL)) {
        return 0;
    }
    switch (message) {
    case WM_CREATE:
        return on_create(window) ? 0 : -1;
    case WM_SIZE:
        controls_place(&app);
        return 0;
    case WM_GETMINMAXINFO:
        limit_size((MINMAXINFO *)lparam);
        return 0;
    case WM_COMMAND:
        on_command(wparam, lparam);
        return 0;
    case WM_DRAWITEM:
        controls_draw_item(&app, (const DRAWITEMSTRUCT *)lparam);
        return TRUE;
    case WM_MEASUREITEM:
        ((MEASUREITEMSTRUCT *)lparam)->itemHeight = (UINT)controls_row_height(&app);
        return TRUE;
    case WM_CTLCOLORSTATIC:
    case WM_CTLCOLOREDIT:
    case WM_CTLCOLORLISTBOX:
    case WM_CTLCOLORBTN:
        return color_control(wparam, lparam);
    case WM_ERASEBKGND:
        return paint_background(window, (HDC)wparam);
    case WM_SETTINGCHANGE:
    case WM_SYSCOLORCHANGE:
        controls_refresh_style(&app);
        (void)InvalidateRect(window, NULL, TRUE);
        return 0;
    case WM_NEXTDLGCTL:
        assist_move_focus(&app, wparam, lparam);
        return 0;
    case WM_CLOSE:
        if (dialogs_may_discard(&app)) {
            (void)DestroyWindow(window);
        }
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProcW(window, message, wparam, lparam);
    }
}

static bool register_class(HINSTANCE instance)
{
    WNDCLASSEXW window_class = { 0 };

    if (!CHECK(instance != NULL) || !CHECK(sizeof window_class > 0U)) {
        return false;
    }
    window_class.cbSize = sizeof window_class;
    window_class.lpfnWndProc = window_procedure;
    window_class.hInstance = instance;
    window_class.hCursor = LoadCursorW(NULL, IDC_ARROW);
    window_class.hIcon = LoadIconW(NULL, IDI_APPLICATION);
    window_class.lpszClassName = CLASS_NAME;
    return (RegisterClassExW(&window_class) != 0) || (GetLastError() == ERROR_CLASS_ALREADY_EXISTS);
}

static int run_messages(void)
{
    MSG message;
    BOOL received;
    int step;

    if (!CHECK(app.window != NULL) || !CHECK(app.accelerators != NULL)) {
        return 1;
    }
    for (step = 0; step < MESSAGE_LIMIT; ++step) {
        received = GetMessageW(&message, NULL, 0, 0);
        if (received <= 0) {
            return (received == 0) ? (int)message.wParam : 1;
        }
        if ((TranslateAcceleratorW(app.window, app.accelerators, &message) != 0) ||
            (IsDialogMessageW(app.window, &message) != 0)) {
            continue;
        }
        (void)TranslateMessage(&message);
        (void)DispatchMessageW(&message);
    }
    return 0;
}

HWND app_open(HINSTANCE instance)
{
    if (!CHECK(instance != NULL) || !register_class(instance)) {
        return NULL;
    }
    app.instance = instance;
    (void)SetProcessPreferredUILanguages(MUI_LANGUAGE_NAME, L"en-US\0", NULL);
    (void)CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    app.dpi = assist_dpi(NULL);
    app.bonus_category = LIST_ALL_CATEGORIES;
    history_start(&app.history);
    app.accelerators = make_accelerators();
    app.window = CreateWindowExW(WINDOW_EXTRA_STYLE, CLASS_NAME, L"civbuilder", WINDOW_STYLE, CW_USEDEFAULT,
                                 CW_USEDEFAULT, MulDiv(START_WIDTH, app.dpi, LAYOUT_BASE_DPI),
                                 MulDiv(START_HEIGHT, app.dpi, LAYOUT_BASE_DPI), NULL, make_menu(), instance, NULL);
    return app.window;
}

int app_run(HINSTANCE instance, int show)
{
    if (!CHECK(instance != NULL) || !CHECK(app_open(instance) != NULL)) {
        return 1;
    }
    (void)ShowWindow(app.window, show);
    (void)UpdateWindow(app.window);
    return run_messages();
}
