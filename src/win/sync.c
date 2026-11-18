#include "win_parts.h"

#include <wchar.h>

#include "../base/base_civ.h"
#include "../core/check.h"
#include "../core/text.h"
#include "../present/details.h"
#include "../present/summary.h"

enum { BYTES_PER_CHARACTER = 3, CATEGORY_ROWS = BONUS_CATEGORY_COUNT + 1, STEP_ROW_CAPACITY = 64 };

_Static_assert(CBN_SELCHANGE == LBN_SELCHANGE, "a choice and a list say alike that their row changed");
_Static_assert((int)ROSTER_SLOT_LIMIT == (int)TSV_BIT_LIMIT, "the roster has a slot for every civ the catalog names");

static wchar_t wide[WIDE_TEXT_CAPACITY];
static wchar_t shown[WIDE_TEXT_CAPACITY];
static char narrow[WIDE_TEXT_CAPACITY];
static SummaryText summary;
static DetailText detail;
static CivDesign base_design;

static void show_text(HWND window, const char *text)
{
    if (!dialogs_widen(text, wide, WIDE_TEXT_CAPACITY)) {
        return;
    }
    (void)GetWindowTextW(window, shown, WIDE_TEXT_CAPACITY);
    if (wcscmp(shown, wide) != 0) {
        (void)SetWindowTextW(window, wide);
    }
}

static bool read_text(HWND window, char *text, int capacity)
{
    (void)GetWindowTextW(window, wide, WIDE_TEXT_CAPACITY);
    return dialogs_narrow(wide, text, capacity);
}

static void sync_texts(AppState *app)
{
    int control;
    int field;

    for (control = 0; control < CONTROL_COUNT; ++control) {
        field = binding_of((ControlId)control)->text_field;
        if (field != BINDING_NO_FIELD) {
            int limit = (civ_text_capacity((TextField)field) - 1) / BYTES_PER_CHARACTER;
            (void)SendMessageW(app->controls[control], EM_SETLIMITTEXT, (WPARAM)limit, 0);
            show_text(app->controls[control], civ_design_text(&app->history.present, (TextField)field));
        }
    }
}

static void tick_rows(AppState *app, ControlId control)
{
    HWND list;
    LRESULT top;
    ListItem item;
    bool chosen;
    int row;

    list = app->controls[control];
    top = SendMessageW(list, LB_GETTOPINDEX, 0, 0);
    (void)SendMessageW(list, WM_SETREDRAW, FALSE, 0);
    item.control = control;
    for (row = 0; (row < app->lists[control].count) && (row < LIST_ROW_CAPACITY); ++row) {
        item.item = app->lists[control].items[row];
        chosen = list_item_is_chosen(&app->catalog, &app->history.present, item);
        if (chosen != (SendMessageW(list, LB_GETSEL, (WPARAM)row, 0) > 0)) {
            (void)SendMessageW(list, LB_SETSEL, chosen ? TRUE : FALSE, (LPARAM)row);
        }
    }
    (void)SendMessageW(list, LB_SETTOPINDEX, (WPARAM)((top > 0) ? top : 0), 0);
    (void)SendMessageW(list, WM_SETREDRAW, TRUE, 0);
    (void)InvalidateRect(list, NULL, FALSE);
}

static int first_chosen_row(const AppState *app, ControlId control)
{
    ListItem item;
    int row;

    item.control = control;
    for (row = 0; (row < app->lists[control].count) && (row < LIST_ROW_CAPACITY); ++row) {
        item.item = app->lists[control].items[row];
        if (list_item_is_chosen(&app->catalog, &app->history.present, item)) {
            return row;
        }
    }
    return -1;
}

static void select_rows(AppState *app, ControlId control)
{
    WidgetKind kind = layout_spec(control).kind;
    int chosen_row;

    if (kind == WIDGET_CHECKLIST) {
        tick_rows(app, control);
        return;
    }
    chosen_row = first_chosen_row(app, control);
    if (kind == WIDGET_LIST) {
        (void)SendMessageW(app->controls[control], LB_SETCURSEL, (WPARAM)chosen_row, 0);
    } else if (kind == WIDGET_CHOICE) {
        (void)SendMessageW(app->controls[control], CB_SETCURSEL, (WPARAM)chosen_row, 0);
    }
}

static ListQuery query_of(AppState *app, ControlId control)
{
    ListQuery query = { CONTROL_TITLE, "", LIST_ALL_CATEGORIES };

    query.control = control;
    if (control == CONTROL_TREE_LIST) {
        (void)read_text(app->controls[CONTROL_TREE_FILTER], query.filter, LIST_FILTER_CAPACITY);
    } else if (control == CONTROL_BONUS_LIST) {
        (void)read_text(app->controls[CONTROL_BONUS_FILTER], query.filter, LIST_FILTER_CAPACITY);
        query.category = app->bonus_category;
    }
    return query;
}

void sync_list(AppState *app, ControlId control)
{
    ListQuery query = query_of(app, control);
    bool combo = (layout_spec(control).kind == WIDGET_CHOICE);
    ListItem item;
    RowText row;
    int index;

    if (!list_model_fill(&app->lists[control], &app->catalog, &query)) {
        return;
    }
    (void)SendMessageW(app->controls[control], WM_SETREDRAW, FALSE, 0);
    (void)SendMessageW(app->controls[control], combo ? CB_RESETCONTENT : LB_RESETCONTENT, 0, 0);
    item.control = control;
    for (index = 0; (index < app->lists[control].count) && (index < LIST_ROW_CAPACITY); ++index) {
        item.item = app->lists[control].items[index];
        row = list_item_text(&app->catalog, item);
        if (dialogs_widen(row.text, wide, WIDE_TEXT_CAPACITY)) {
            (void)SendMessageW(app->controls[control], combo ? CB_ADDSTRING : LB_ADDSTRING, 0, (LPARAM)wide);
        }
    }
    (void)SendMessageW(app->controls[control], WM_SETREDRAW, TRUE, 0);
    (void)InvalidateRect(app->controls[control], NULL, TRUE);
    assist_wrap_rows(app, control);
    select_rows(app, control);
}

static void fill_categories(AppState *app)
{
    int category;

    if (!CHECK(app->controls[CONTROL_BONUS_CATEGORY] != NULL)) {
        return;
    }
    (void)SendMessageW(app->controls[CONTROL_BONUS_CATEGORY], CB_RESETCONTENT, 0, 0);
    (void)SendMessageW(app->controls[CONTROL_BONUS_CATEGORY], CB_ADDSTRING, 0, (LPARAM)L"All categories");
    for (category = 0; category < BONUS_CATEGORY_COUNT; ++category) {
        if (dialogs_widen(catalog_category_name((BonusCategory)category), wide, WIDE_TEXT_CAPACITY)) {
            (void)SendMessageW(app->controls[CONTROL_BONUS_CATEGORY], CB_ADDSTRING, 0, (LPARAM)wide);
        }
    }
    (void)SendMessageW(app->controls[CONTROL_BONUS_CATEGORY], CB_SETCURSEL, (WPARAM)(app->bonus_category + 1), 0);
}

static void sync_steps(AppState *app)
{
    char row[STEP_ROW_CAPACITY];
    int step;

    bool in_mod = roster_holds(&app->roster, &app->history.present);

    base_design = app->history.present;
    (void)base_civ_apply(&base_design, &app->catalog, app->history.present.choices[CHOICE_REPLACE]);
    (void)SendMessageW(app->controls[CONTROL_STEPS], WM_SETREDRAW, FALSE, 0);
    (void)SendMessageW(app->controls[CONTROL_STEPS], LB_RESETCONTENT, 0, 0);
    for (step = 0; step < STEP_COUNT; ++step) {
        row[0] = '\0';
        (void)text_append_number(row, sizeof row, (long)step + 1L);
        (void)text_append(row, sizeof row, ". ");
        (void)text_append(row, sizeof row, wizard_page((WizardStep)step)->title);
        (void)text_append(row, sizeof row,
                          wizard_step_is_done(&app->history.present, &base_design, in_mod, (WizardStep)step)
                              ? " (done)"
                              : "");
        if (dialogs_widen(row, wide, WIDE_TEXT_CAPACITY)) {
            (void)SendMessageW(app->controls[CONTROL_STEPS], LB_ADDSTRING, 0, (LPARAM)wide);
        }
    }
    (void)SendMessageW(app->controls[CONTROL_STEPS], LB_SETCURSEL, (WPARAM)app->step, 0);
    (void)SendMessageW(app->controls[CONTROL_STEPS], WM_SETREDRAW, TRUE, 0);
    (void)InvalidateRect(app->controls[CONTROL_STEPS], NULL, TRUE);
}

void sync_details(AppState *app)
{
    int control;

    app->syncing = true;
    roster_keep(&app->roster, &app->history.present);
    detail = details_window_title(&app->history.present, app->step);
    show_text(app->window, detail.text);
    detail = details_title(app->step);
    show_text(app->controls[CONTROL_TITLE], detail.text);
    show_text(app->controls[CONTROL_HELP], wizard_page(app->step)->help);
    for (control = (int)CONTROL_TREE_INFO; control < CONTROL_COUNT; ++control) {
        if ((layout_spec((ControlId)control).kind == WIDGET_TEXT) && layout_shows((ControlId)control, app->step)) {
            detail = (control == (int)CONTROL_CHANGED)
                         ? details_changed(roster_count(&app->roster))
                         : details_of(&app->history.present, &app->catalog, (ControlId)control);
            show_text(app->controls[control], detail.text);
        }
    }
    if ((app->step == STEP_REVIEW) && summary_write(&summary, &app->history.present, &app->catalog)) {
        show_text(app->controls[CONTROL_SUMMARY], summary.text);
    }
    sync_steps(app);
    app->syncing = false;
}

static void sync_choices(AppState *app, bool refill)
{
    int control;

    for (control = 0; control < CONTROL_COUNT; ++control) {
        if (binding_of((ControlId)control)->source == SOURCE_NONE) {
            continue;
        }
        if (refill) {
            sync_list(app, (ControlId)control);
        } else {
            select_rows(app, (ControlId)control);
        }
    }
}

void sync_design(AppState *app, bool refill)
{
    app->syncing = true;
    sync_texts(app);
    if (refill) {
        fill_categories(app);
    }
    sync_choices(app, refill);
    app->syncing = false;
    sync_details(app);
}

static void apply(AppState *app, const Command *command)
{
    if (!history_do(&app->history, command)) {
        dialogs_status(app, "That change cannot be made: the civ is full or the text is too long.");
    } else {
        app->unsaved = true;
    }
    app->syncing = true;
    sync_choices(app, false);
    app->syncing = false;
    sync_details(app);
}

static void fill_list_again(AppState *app, ControlId list)
{
    app->syncing = true;
    sync_list(app, list);
    app->syncing = false;
}

static void on_text_change(AppState *app, ControlId control)
{
    int field = binding_of(control)->text_field;
    Command command;

    if ((control == CONTROL_TREE_FILTER) || (control == CONTROL_BONUS_FILTER)) {
        fill_list_again(app, (control == CONTROL_TREE_FILTER) ? CONTROL_TREE_LIST : CONTROL_BONUS_LIST);
        return;
    }
    if ((field == BINDING_NO_FIELD) || !read_text(app->controls[control], narrow, WIDE_TEXT_CAPACITY)) {
        return;
    }
    command = command_set_text((TextField)field, narrow);
    apply(app, &command);
}

static void choose_civ(AppState *app, int slot)
{
    static CivDesign chosen;

    roster_keep(&app->roster, &app->history.present);
    if (!roster_draft(&app->roster, slot, &chosen) && !base_civ_apply(&chosen, &app->catalog, slot)) {
        dialogs_status(app, "That civ is not in the catalog.");
        return;
    }
    (void)history_replace(&app->history, &chosen);
    sync_design(app, false);
    (void)text_copy(narrow, sizeof narrow, "Now editing the ");
    (void)text_append(narrow, sizeof narrow, catalog_appearance_name(&app->catalog, APPEARANCE_BASE, slot));
    (void)text_append(narrow, sizeof narrow, ".");
    dialogs_status(app, narrow);
}

static const char SAVINGS[ROSTER_LEFT_OUT + 1][STATUS_CAPACITY] = {
    "The civ is saved in the mod. Build mod builds it with every other civ saved.",
    "The mod now holds the civ as it is now.",
    "The civ is as the game has it again, so the mod no longer changes it.",
    "The civ is as the game has it, so the mod leaves it out."
};

void sync_save_civ(AppState *app)
{
    static CivDesign game;
    RosterSaving saving;

    if (!base_civ_apply(&game, &app->catalog, app->history.present.choices[CHOICE_REPLACE])) {
        dialogs_status(app, "That civ is not in the catalog.");
        return;
    }
    saving = roster_save(&app->roster, &app->history.present, &game);
    sync_details(app);
    dialogs_status(app, SAVINGS[saving]);
}

static void on_pick(AppState *app, ControlId control, int row)
{
    ListItem item;
    Command command;

    if ((row < 0) || (row >= app->lists[control].count)) {
        return;
    }
    item.control = control;
    item.item = app->lists[control].items[row];
    if (list_item_is_chosen(&app->catalog, &app->history.present, item) &&
        (layout_spec(control).kind != WIDGET_CHECKLIST)) {
        return;
    }
    if (control == CONTROL_CIV) {
        choose_civ(app, app->catalog.appearance[item.item].id);
        return;
    }
    command = list_item_command(&app->catalog, item);
    apply(app, &command);
}

static void on_checklist_change(AppState *app, ControlId control)
{
    ListItem item;
    int row;
    bool selected;

    item.control = control;
    for (row = 0; (row < app->lists[control].count) && (row < LIST_ROW_CAPACITY); ++row) {
        item.item = app->lists[control].items[row];
        selected = SendMessageW(app->controls[control], LB_GETSEL, (WPARAM)row, 0) > 0;
        if (selected != list_item_is_chosen(&app->catalog, &app->history.present, item)) {
            on_pick(app, control, row);
        }
    }
}

static bool is_text_change(WidgetKind kind, int code)
{
    return ((kind == WIDGET_LINE) || (kind == WIDGET_PARAGRAPH)) && (code == EN_CHANGE);
}

static void on_selection(AppState *app, ControlId control, WidgetKind kind)
{
    if (control == CONTROL_BONUS_CATEGORY) {
        app->bonus_category = (int)SendMessageW(app->controls[control], CB_GETCURSEL, 0, 0) - 1;
        fill_list_again(app, CONTROL_BONUS_LIST);
    } else if (kind == WIDGET_CHOICE) {
        on_pick(app, control, (int)SendMessageW(app->controls[control], CB_GETCURSEL, 0, 0));
    } else if (kind == WIDGET_CHECKLIST) {
        on_checklist_change(app, control);
    } else if ((kind == WIDGET_LIST) && (control != CONTROL_STEPS)) {
        on_pick(app, control, (int)SendMessageW(app->controls[control], LB_GETCURSEL, 0, 0));
    }
}

void sync_on_notify(AppState *app, int control, int code)
{
    WidgetKind kind;

    if (app->syncing || (control < 0) || (control >= CONTROL_COUNT)) {
        return;
    }
    kind = layout_spec((ControlId)control).kind;
    if (is_text_change(kind, code)) {
        on_text_change(app, (ControlId)control);
    } else if (code == LBN_SELCHANGE) {
        on_selection(app, (ControlId)control, kind);
    }
}
