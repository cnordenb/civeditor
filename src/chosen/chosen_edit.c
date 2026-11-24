#include "chosen_parts.h"

#include "../core/check.h"
#include "../core/text.h"

enum { LABEL_ROOM = 160, SLIDER_LINE = 10, SLIDER_PAGE = 100 };

static HWND field_in_focus(const AppState *app)
{
    NumberFocus focus = app->number_focus;

    if (!check_is_index((int)focus.panel, NUMBER_PANEL_COUNT) || !check_is_index(focus.pick, CIV_BONUS_CAPACITY) ||
        !check_is_index(focus.number, BONUS_NUMBER_CAPACITY)) {
        return NULL;
    }
    return app->chosen[focus.panel].fields[focus.pick][focus.number];
}

static ChosenWindows *focus_windows(AppState *app)
{
    return &app->chosen[check_index((int)app->number_focus.panel, NUMBER_PANEL_COUNT, 0)];
}

static bool make_card(AppState *app)
{
    ChosenWindows *windows = focus_windows(app);

    if (windows->card == NULL) {
        windows->card = CreateWindowExW(WS_EX_CONTROLPARENT, CHOSEN_CLASS_NAME, L"", WS_CHILD | WS_CLIPSIBLINGS, 0, 0,
                                        0, 0, chosen_panel(app, app->number_focus.panel),
                                        (HMENU)(INT_PTR)CHOSEN_CARD_ID, app->instance, NULL);
    }
    if ((windows->card != NULL) && (windows->slider == NULL)) {
        windows->slider = CreateWindowExW(WS_EX_LEFT, TRACKBAR_CLASSW, L"",
                                          WS_CHILD | WS_VISIBLE | WS_TABSTOP | TBS_HORZ | TBS_NOTICKS, 0, 0, 0, 0,
                                          windows->card, (HMENU)(INT_PTR)CHOSEN_SLIDER_ID, app->instance, NULL);
        (void)SendMessageW(windows->slider, TBM_SETRANGE, FALSE, MAKELPARAM(NUMBER_SLIDER_LEAST, NUMBER_SLIDER_MOST));
        (void)SendMessageW(windows->slider, TBM_SETLINESIZE, 0, SLIDER_LINE);
        (void)SendMessageW(windows->slider, TBM_SETPAGESIZE, 0, SLIDER_PAGE);
    }
    return windows->slider != NULL;
}

static NumberCardSizes card_sizes(const AppState *app, NumberPlace place)
{
    HWND panel = chosen_panel(app, place.focus.panel);
    HDC context = GetDC(panel);
    HGDIOBJ before = SelectObject(context, app->font);
    char least[NUMBER_TEXT_CAPACITY] = "";
    char most[NUMBER_TEXT_CAPACITY] = "";
    NumberCardSizes sizes;

    (void)number_text(place, 1.0, NUMBER_WITH_UNIT, least, sizeof least);
    (void)number_text(place, (double)CIV_MULTIPLIER_LIMIT, NUMBER_WITH_UNIT, most, sizeof most);
    sizes.least = chosen_text_width(context, least);
    sizes.most = chosen_text_width(context, most);
    sizes.track = assist_scaled(app, CHOSEN_TRACK_WIDTH);
    sizes.gap = assist_scaled(app, CHOSEN_CARD_GAP);
    (void)SelectObject(context, before);
    (void)ReleaseDC(panel, context);
    return sizes;
}

static void annotate(HWND control, AssistProperty property, const char *text)
{
    wchar_t wide[LABEL_ROOM];

    if (text_widen(text, wide, LABEL_ROOM)) {
        assist_annotate(control, property, wide);
    }
}

static void show_value(AppState *app, NumberPlace place, double times)
{
    char text[NUMBER_TEXT_CAPACITY];

    if (number_text(place, times, NUMBER_DIGITS, text, sizeof text)) {
        chosen_show_text(field_in_focus(app), text);
    }
    if (number_text(place, times, NUMBER_WITH_UNIT, text, sizeof text)) {
        annotate(focus_windows(app)->slider, ASSIST_VALUE, text);
    }
}

static void set_slider(AppState *app, NumberPlace place)
{
    HWND slider = focus_windows(app)->slider;
    char name[LABEL_ROOM];

    (void)SendMessageW(slider, TBM_SETPOS, TRUE, (LPARAM)number_slider_position(place));
    if (number_range_text(place, "Slide from ", name, sizeof name)) {
        annotate(slider, ASSIST_NAME, name);
    }
    show_value(app, place, number_times(place));
}

static void hide_card(AppState *app)
{
    int panel;

    app->number_active = false;
    for (panel = 0; panel < (int)NUMBER_PANEL_COUNT; ++panel) {
        if (app->chosen[panel].card != NULL) {
            (void)ShowWindow(app->chosen[panel].card, SW_HIDE);
        }
    }
}

static bool is_editing(AppState *app, HWND from)
{
    return (from != NULL) && ((from == field_in_focus(app)) || (from == focus_windows(app)->slider));
}

static void give_up_field(AppState *app)
{
    HWND focus = GetFocus();
    NumberPanel panel = app->number_focus.panel;

    if ((focus == chosen_panel(app, panel)) || is_editing(app, focus)) {
        (void)SetFocus(app->controls[chosen_controls(panel).list]);
    }
    hide_card(app);
}

void chosen_place_card(AppState *app)
{
    HWND panel = chosen_panel(app, app->number_focus.panel);
    HWND field = field_in_focus(app);
    NumberPlace place = chosen_place_of(app, app->number_focus);
    int height = assist_scaled(app, CHOSEN_CARD_HEIGHT);
    RECT client = { 0, 0, 0, 0 };
    RECT below = { 0, 0, 0, 0 };
    ChosenWindows *windows;
    NumberCard card;

    if ((field == NULL) || ((GetWindowLongPtrW(field, GWL_STYLE) & WS_VISIBLE) == 0)) {
        give_up_field(app);
        return;
    }
    if (!app->number_active || !make_card(app)) {
        return;
    }
    windows = focus_windows(app);
    (void)GetClientRect(panel, &client);
    (void)GetWindowRect(field, &below);
    (void)MapWindowPoints(NULL, panel, (POINT *)&below, 2);
    card = number_card_place((below.left + below.right) / 2, client.right, card_sizes(app, place));
    (void)SetWindowPos(windows->card, field, card.left, below.bottom + assist_scaled(app, CHOSEN_CARD_DROP), card.width,
                       height, SWP_NOACTIVATE | SWP_SHOWWINDOW);
    (void)MoveWindow(windows->slider, card.track_left - card.left, CHOSEN_FIELD_EDGE, card.track_width,
                     height - (2 * CHOSEN_FIELD_EDGE), TRUE);
    set_slider(app, place);
    (void)InvalidateRect(windows->card, NULL, TRUE);
}

static void redraw_row(const AppState *app, NumberFocus focus)
{
    if (check_is_index((int)focus.panel, NUMBER_PANEL_COUNT) && check_is_index(focus.pick, CIV_BONUS_CAPACITY) &&
        (app->chosen[focus.panel].rows[focus.pick] != NULL)) {
        (void)InvalidateRect(app->chosen[focus.panel].rows[focus.pick], NULL, TRUE);
    }
}

static void reveal_field(AppState *app, HWND field)
{
    NumberPanel panel = app->number_focus.panel;
    RECT shown = { 0, 0, 0, 0 };
    RECT card = { 0, 0, 0, 0 };

    (void)GetWindowRect(field, &shown);
    if (app->number_active) {
        (void)GetWindowRect(focus_windows(app)->card, &card);
        shown.bottom = card.bottom;
    }
    (void)MapWindowPoints(NULL, chosen_panel(app, panel), (POINT *)&shown, 2);
    chosen_reveal(app, panel, shown);
}

static void enter(AppState *app, NumberFocus found)
{
    redraw_row(app, app->number_focus);
    hide_card(app);
    app->number_focus = found;
    app->number_active = number_slides(chosen_place_of(app, found));
    chosen_place_card(app);
    reveal_field(app, field_in_focus(app));
    redraw_row(app, found);
}

static void leave(AppState *app)
{
    NumberFocus none = { NUMBER_PANEL_BONUSES, NUMBER_NONE, NUMBER_NONE };

    redraw_row(app, app->number_focus);
    app->number_focus = none;
    hide_card(app);
}

static bool same_focus(NumberFocus one, NumberFocus other)
{
    return (one.panel == other.panel) && (one.pick == other.pick) && (one.number == other.number);
}

void chosen_follow_focus(AppState *app)
{
    HWND focus = GetFocus();
    NumberFocus found = chosen_focus_of(app, focus);

    if ((focus != NULL) && (focus == focus_windows(app)->slider)) {
        return;
    }
    if (found.pick == NUMBER_NONE) {
        if (app->number_focus.pick != NUMBER_NONE) {
            leave(app);
        }
        return;
    }
    if (!same_focus(found, app->number_focus)) {
        enter(app, found);
    }
}

static void refuse(AppState *app, NumberPlace place, HWND field, const char *shown)
{
    char refusal[LABEL_ROOM] = "There is no number to set.";

    if (number_range_text(place, "Type a value from ", refusal, sizeof refusal)) {
        (void)text_append(refusal, sizeof refusal, ".");
    }
    dialogs_status(app, refusal);
    chosen_show_text(field, shown);
    (void)SendMessageW(field, EM_SETSEL, 0, -1);
}

void chosen_commit(AppState *app, HWND field)
{
    NumberPlace place = chosen_place_of(app, chosen_focus_of(app, field));
    wchar_t wide[LABEL_ROOM] = L"";
    char typed[LABEL_ROOM] = "";
    char shown[NUMBER_TEXT_CAPACITY] = "";
    Command command;

    if (!number_text(place, number_times(place), NUMBER_DIGITS, shown, sizeof shown)) {
        return;
    }
    (void)GetWindowTextW(field, wide, LABEL_ROOM);
    (void)dialogs_narrow(wide, typed, sizeof typed);
    if (text_equal(typed, shown)) {
        return;
    }
    command = number_typed_command(place, typed);
    if (command.kind == COMMAND_KIND_COUNT) {
        refuse(app, place, field, shown);
        return;
    }
    sync_apply(app, &command);
}

void chosen_slide(AppState *app, WPARAM wparam)
{
    NumberPlace place = chosen_place_of(app, app->number_focus);
    int position = (int)SendMessageW(focus_windows(app)->slider, TBM_GETPOS, 0, 0);
    Command command;

    if (field_in_focus(app) == NULL) {
        return;
    }
    if (LOWORD(wparam) != TB_ENDTRACK) {
        show_value(app, place, number_slider_times(position));
        return;
    }
    if (position != number_slider_position(place)) {
        command = number_slid_command(place, position);
        sync_apply(app, &command);
    }
}

static bool dismissed(AppState *app, HWND from)
{
    NumberPlace place = chosen_place_of(app, app->number_focus);

    if (!app->number_active || !is_editing(app, from)) {
        return false;
    }
    show_value(app, place, number_times(place));
    (void)SetFocus(field_in_focus(app));
    hide_card(app);
    return true;
}

bool chosen_take_message(AppState *app, const MSG *message)
{
    if (message->message != WM_KEYDOWN) {
        return false;
    }
    if ((message->wParam == VK_RETURN) && (chosen_focus_of(app, message->hwnd).pick != NUMBER_NONE)) {
        chosen_commit(app, message->hwnd);
        return true;
    }
    return (message->wParam == VK_ESCAPE) && dismissed(app, message->hwnd);
}
