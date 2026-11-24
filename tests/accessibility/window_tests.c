#include "access_tests.h"

#include <windows.h>
#include <oleacc.h>
#include <uiautomationclient.h>

#include "../../src/core/check.h"
#include "../../src/core/text.h"
#include "../../src/win/win_parts.h"
#include "../test_outcome.h"
#include "window_fixture.h"

enum {
    TAB_LIMIT = 2 * CONTROL_COUNT,
    MESSAGE_DRAIN_LIMIT = 64,
    READING_COLUMN = 10000,
    SCROLLED_ROW = 30,
    TICKED_ROW = 34,
    NESTING = 3
};

typedef enum {
    ASK_TAB_REACHES,
    ASK_NAMED_BY_LABEL,
    ASK_TAB_LEAVES,
    ASK_HELP,
    ASK_FOCUS_FRAME
} ControlQuestion;

static void go_to(HWND window, int step)
{
    window_fixture_act(window, ACTION_FIRST_STEP + step);
}

static bool tab_reaches(HWND window, HWND target)
{
    HWND at = GetNextDlgTabItem(window, NULL, FALSE);
    int step;

    if (!CHECK(window != NULL) || (target == NULL)) {
        return false;
    }
    for (step = 0; (step < TAB_LIMIT) && (at != NULL); ++step) {
        if ((at == target) || (IsChild(target, at) != 0)) {
            return true;
        }
        (void)SetFocus(at);
        at = GetNextDlgTabItem(window, at, FALSE);
    }
    return false;
}

static IAccessible *reader_of(HWND control)
{
    static const IID ACCESSIBLE = { 0x618736e0UL, 0x3c3d, 0x11cf, { 0x81, 0x0c, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 } };
    void *object = NULL;

    if (!CHECK(ACCESSIBLE.Data2 != 0U) || (control == NULL) ||
        (AccessibleObjectFromWindow(control, (DWORD)OBJID_CLIENT, &ACCESSIBLE, &object) != S_OK)) {
        return NULL;
    }
    return object;
}

static bool name_begins_with_label(HWND control, const char *label)
{
    wchar_t wide[LAYOUT_LABEL_WIDTH];
    IAccessible *reader = reader_of(control);
    VARIANT self;
    BSTR name = NULL;
    bool begins = false;

    if (!CHECK(label != NULL) || !text_widen(label, wide, LAYOUT_LABEL_WIDTH) || (reader == NULL)) {
        return false;
    }
    VariantInit(&self);
    self.vt = VT_I4;
    self.lVal = CHILDID_SELF;
    if ((IAccessible_get_accName(reader, self, &name) == S_OK) && (name != NULL)) {
        begins = wcsncmp(name, wide, wcslen(wide)) == 0;
        SysFreeString(name);
    }
    (void)IAccessible_Release(reader);
    return begins;
}

static bool tab_leaves(HWND window, HWND control)
{
    MSG key = { NULL, WM_KEYDOWN, VK_TAB, 0, 0, { 0, 0 } };
    MSG waiting;
    int step;

    if (!CHECK(window != NULL) || (control == NULL)) {
        return false;
    }
    (void)SetFocus(control);
    if (GetFocus() != control) {
        return false;
    }
    key.hwnd = control;
    if (IsDialogMessageW(window, &key) == 0) {
        (void)DispatchMessageW(&key);
    }
    for (step = 0; step < MESSAGE_DRAIN_LIMIT; ++step) {
        if (PeekMessageW(&waiting, NULL, 0, 0, PM_REMOVE) == 0) {
            break;
        }
        (void)DispatchMessageW(&waiting);
    }
    return GetFocus() != control;
}

static bool help_is_given(HWND window, HWND target, ControlId control)
{
    wchar_t status[STATUS_CAPACITY] = L"";
    wchar_t help[STATUS_CAPACITY];

    if (!CHECK(window != NULL) || (target == NULL) || !text_widen(layout_help(control), help, STATUS_CAPACITY)) {
        return false;
    }
    (void)SetFocus(target);
    (void)SendMessageW(window, WM_COMMAND, (WPARAM)(ACTION_ID_BASE + ACTION_HELP), 0);
    (void)GetWindowTextW(GetDlgItem(window, CONTROL_ID_BASE + CONTROL_STATUS), status, STATUS_CAPACITY);
    return (help[0] != L'\0') && (wcscmp(status, help) == 0);
}

static bool control_passes(HWND window, ControlId control, ControlQuestion question)
{
    HWND target = GetDlgItem(window, CONTROL_ID_BASE + (int)control);

    if (!CHECK(window != NULL) || !CHECK(control < CONTROL_COUNT)) {
        return false;
    }
    switch (question) {
    case ASK_TAB_REACHES:
        return tab_reaches(window, target);
    case ASK_NAMED_BY_LABEL:
        return name_begins_with_label(target, layout_spec(control).label);
    case ASK_TAB_LEAVES:
        return tab_leaves(window, target);
    case ASK_HELP:
        return help_is_given(window, target, control);
    case ASK_FOCUS_FRAME:
        return access_frame_shows(window, target);
    default:
        return false;
    }
}

static bool every_control_passes(ControlQuestion question)
{
    HWND window = app_open(GetModuleHandleW(NULL));
    bool passed = window != NULL;
    int step;
    int control;

    window_fixture_tick(window, CONTROL_HERO);
    for (step = 0; (step < STEP_COUNT) && passed; ++step) {
        go_to(window, step);
        for (control = 0; (control < CONTROL_COUNT) && passed; ++control) {
            if (layout_shows((ControlId)control, (WizardStep)step) && layout_is_focusable((ControlId)control)) {
                passed = control_passes(window, (ControlId)control, question);
                test_note(layout_spec((ControlId)control).label);
            }
        }
    }
    window_fixture_close(window);
    return passed;
}

bool test_tab_reaches_every_control_of_every_step(void)
{
    return every_control_passes(ASK_TAB_REACHES);
}

bool test_every_control_is_named_by_its_label(void)
{
    bool named = SUCCEEDED(CoInitialize(NULL)) && every_control_passes(ASK_NAMED_BY_LABEL);

    CoUninitialize();
    return named;
}

bool test_the_tab_key_leaves_every_control(void)
{
    return every_control_passes(ASK_TAB_LEAVES);
}

bool test_help_is_given_for_the_control_in_focus(void)
{
    return every_control_passes(ASK_HELP);
}

bool test_the_control_in_focus_has_a_solid_frame(void)
{
    return every_control_passes(ASK_FOCUS_FRAME);
}

bool test_the_list_of_steps_shows_the_current_step(void)
{
    HWND window = app_open(GetModuleHandleW(NULL));
    bool shown = window != NULL;
    int step;

    for (step = 0; (step < STEP_COUNT) && shown; ++step) {
        go_to(window, step);
        shown = SendMessageW(GetDlgItem(window, CONTROL_ID_BASE + CONTROL_STEPS), LB_GETCURSEL, 0, 0) == step;
    }
    window_fixture_close(window);
    return shown;
}

static long reading_place(HWND window, HWND control)
{
    int identifier = GetDlgCtrlID(control) - CONTROL_ID_BASE;
    Box box;

    if (!CHECK(window != NULL) || (identifier < 0) || (identifier >= CONTROL_COUNT)) {
        return -1L;
    }
    if (identifier == (int)CONTROL_STEPS) {
        return 0L;
    }
    box = layout_box((ControlId)identifier, layout_spec((ControlId)identifier).step, LAYOUT_MIN_WIDTH,
                     LAYOUT_MIN_HEIGHT);
    if (layout_spec((ControlId)identifier).place == PLACE_FRAME) {
        return (READING_COLUMN * READING_COLUMN) + box.left;
    }
    return ((long)box.left * READING_COLUMN) + box.top;
}

static HWND control_holding(HWND window, HWND inner)
{
    HWND holder = inner;
    int depth;

    for (depth = 0; (depth < NESTING) && (holder != NULL) && (GetParent(holder) != window); ++depth) {
        holder = GetParent(holder);
    }
    return holder;
}

static bool reads_on(HWND at, HWND next)
{
    RECT from = { 0, 0, 0, 0 };
    RECT to = { 0, 0, 0, 0 };

    (void)GetWindowRect(at, &from);
    (void)GetWindowRect(next, &to);
    return (to.top > from.top) || ((to.top == from.top) && (to.left > from.left));
}

static bool follows(HWND window, HWND at, HWND next)
{
    HWND from = control_holding(window, at);
    HWND to = control_holding(window, next);

    if (from == to) {
        return reads_on(at, next);
    }
    return reading_place(window, to) > reading_place(window, from);
}

static bool tab_follows_reading(HWND window)
{
    HWND at = GetNextDlgTabItem(window, NULL, FALSE);
    HWND next;
    int walked;

    if (!CHECK(window != NULL) || !CHECK(TAB_LIMIT > 0)) {
        return false;
    }
    for (walked = 0; (walked < TAB_LIMIT) && (at != NULL); ++walked) {
        next = GetNextDlgTabItem(window, at, FALSE);
        if (reading_place(window, control_holding(window, next)) == 0L) {
            return true;
        }
        if (!follows(window, at, next)) {
            test_note(layout_spec((ControlId)(GetDlgCtrlID(control_holding(window, next)) - CONTROL_ID_BASE)).label);
            return false;
        }
        at = next;
    }
    return false;
}

bool test_tab_follows_the_reading_order(void)
{
    HWND window = app_open(GetModuleHandleW(NULL));
    bool ordered = window != NULL;
    int step;

    for (step = 0; (step < STEP_COUNT) && ordered; ++step) {
        go_to(window, step);
        ordered = tab_follows_reading(window);
    }
    window_fixture_close(window);
    return ordered;
}

static HWND tick_a_bonus(HWND window)
{
    HWND list = GetDlgItem(window, CONTROL_ID_BASE + CONTROL_BONUS_LIST);

    if (!CHECK(SCROLLED_ROW < TICKED_ROW) || (window == NULL) || (list == NULL)) {
        return NULL;
    }
    go_to(window, (int)layout_spec(CONTROL_BONUS_LIST).step);
    (void)SendMessageW(list, LB_SETTOPINDEX, SCROLLED_ROW, 0);
    (void)SendMessageW(list, LB_SETSEL, TRUE, TICKED_ROW);
    (void)SendMessageW(window, WM_COMMAND, MAKEWPARAM(CONTROL_ID_BASE + CONTROL_BONUS_LIST, LBN_SELCHANGE),
                       (LPARAM)list);
    return list;
}

bool test_a_ticked_row_leaves_the_list_where_it_was(void)
{
    HWND window = app_open(GetModuleHandleW(NULL));
    HWND list = tick_a_bonus(window);
    LRESULT top = (list != NULL) ? SendMessageW(list, LB_GETTOPINDEX, 0, 0) : -1;

    window_fixture_close(window);
    return top == SCROLLED_ROW;
}

static bool row_is_told_as_chosen(HWND list, int row)
{
    IAccessible *reader = reader_of(list);
    VARIANT child;
    VARIANT state;
    bool chosen;

    if (!CHECK(row >= 0) || (reader == NULL)) {
        return false;
    }
    VariantInit(&child);
    VariantInit(&state);
    child.vt = VT_I4;
    child.lVal = row + 1;
    chosen = (IAccessible_get_accState(reader, child, &state) == S_OK) && (state.vt == VT_I4) &&
             ((state.lVal & STATE_SYSTEM_SELECTED) != 0);
    (void)IAccessible_Release(reader);
    return chosen;
}

bool test_a_ticked_row_is_told_as_chosen(void)
{
    HWND window = app_open(GetModuleHandleW(NULL));
    HWND list = tick_a_bonus(window);
    bool told = (list != NULL) && SUCCEEDED(CoInitialize(NULL)) && row_is_told_as_chosen(list, TICKED_ROW) &&
                !row_is_told_as_chosen(list, SCROLLED_ROW);

    CoUninitialize();
    window_fixture_close(window);
    return told;
}

bool test_moving_the_focus_changes_nothing(void)
{
    HWND window = app_open(GetModuleHandleW(NULL));
    wchar_t status[STATUS_CAPACITY] = L"";
    int step;
    int control;

    for (step = 0; (step < STEP_COUNT) && (window != NULL); ++step) {
        go_to(window, step);
        for (control = 0; control < CONTROL_COUNT; ++control) {
            if (layout_shows((ControlId)control, (WizardStep)step) && layout_is_focusable((ControlId)control)) {
                (void)SetFocus(GetDlgItem(window, CONTROL_ID_BASE + control));
            }
        }
    }
    if (window != NULL) {
        (void)SendMessageW(window, WM_COMMAND, (WPARAM)(ACTION_ID_BASE + ACTION_UNDO), 0);
        (void)GetWindowTextW(GetDlgItem(window, CONTROL_ID_BASE + CONTROL_STATUS), status, STATUS_CAPACITY);
    }
    window_fixture_close(window);
    return wcscmp(status, L"There is nothing to undo.") == 0;
}

bool test_windows_is_told_the_language_is_english(void)
{
    HWND window = app_open(GetModuleHandleW(NULL));
    wchar_t languages[LAYOUT_LABEL_WIDTH] = L"";
    ULONG count = 0UL;
    ULONG size = LAYOUT_LABEL_WIDTH;
    bool told = (window != NULL) &&
                (GetProcessPreferredUILanguages(MUI_LANGUAGE_NAME, &count, languages, &size) != 0) &&
                (count == 1UL) && (wcscmp(languages, L"en-US") == 0);

    window_fixture_close(window);
    return told;
}

static bool element_is_polite(IUIAutomationElement *element)
{
    VARIANT setting;

    if (!CHECK(element != NULL) || !CHECK(UIA_LiveSettingPropertyId > 0)) {
        return false;
    }
    VariantInit(&setting);
    return (IUIAutomationElement_GetCurrentPropertyValue(element, UIA_LiveSettingPropertyId, &setting) == S_OK) &&
           (setting.vt == VT_I4) && (setting.lVal == 1);
}

static bool is_polite_live_region(HWND control)
{
    static const CLSID AUTOMATION = {
        0xff48dba4UL, 0x60ef, 0x4201, { 0xaa, 0x87, 0x54, 0x10, 0x3e, 0xef, 0x59, 0x4e }
    };
    static const IID CLIENT = { 0x30cbe57dUL, 0xd9d0, 0x452a, { 0xab, 0x13, 0x7a, 0xc5, 0xac, 0x48, 0x25, 0xee } };
    void *object = NULL;
    IUIAutomation *automation;
    IUIAutomationElement *element = NULL;
    bool polite = false;

    if (!CHECK(control != NULL) ||
        (CoCreateInstance(&AUTOMATION, NULL, CLSCTX_INPROC_SERVER, &CLIENT, &object) != S_OK)) {
        return false;
    }
    automation = object;
    if ((IUIAutomation_ElementFromHandle(automation, control, &element) == S_OK) && (element != NULL)) {
        polite = element_is_polite(element);
        (void)IUIAutomationElement_Release(element);
    }
    (void)IUIAutomation_Release(automation);
    return polite;
}

bool test_the_status_line_is_told_to_a_screen_reader(void)
{
    HWND window = app_open(GetModuleHandleW(NULL));
    bool told = (window != NULL) && SUCCEEDED(CoInitialize(NULL)) &&
                is_polite_live_region(GetDlgItem(window, CONTROL_ID_BASE + CONTROL_STATUS));

    CoUninitialize();
    window_fixture_close(window);
    return told;
}
