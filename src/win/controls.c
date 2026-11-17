#include "win_parts.h"

#include "../core/check.h"

enum {
    TEXT_POINTS = 11,
    TITLE_POINTS = 16,
    POINTS_PER_INCH = 72,
    DROP_HEIGHT = 320,
    BOX_INSET = 4,
    BOX_LINE = 2,
    FOCUS_LINE = 3,
    RING_LIMIT = 4 * BOX_SIZE,
    CHECK_KNEE_DIVISOR = 2,
    CLASS_WIDTH = 16,
    ROW_TEXT_CAPACITY = 512,
    CREATION_COUNT = CONTROL_COUNT
};

static const wchar_t CLASSES[WIDGET_KIND_COUNT][CLASS_WIDTH] = {
    L"STATIC", L"EDIT", L"EDIT", L"COMBOBOX", L"LISTBOX", L"LISTBOX", L"BUTTON", L"EDIT"
};

static const DWORD STYLES[WIDGET_KIND_COUNT] = {
    SS_LEFT | SS_NOPREFIX,
    WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL,
    WS_BORDER | WS_TABSTOP | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN,
    WS_TABSTOP | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_OWNERDRAWFIXED | CBS_HASSTRINGS,
    WS_BORDER | WS_TABSTOP | WS_VSCROLL | LBS_NOTIFY | LBS_OWNERDRAWVARIABLE | LBS_HASSTRINGS | LBS_NOINTEGRALHEIGHT,
    WS_BORDER | WS_TABSTOP | WS_VSCROLL | LBS_NOTIFY | LBS_OWNERDRAWVARIABLE | LBS_HASSTRINGS | LBS_NOINTEGRALHEIGHT |
        LBS_MULTIPLESEL,
    WS_TABSTOP | BS_PUSHBUTTON | BS_NOTIFY,
    WS_BORDER | WS_TABSTOP | WS_VSCROLL | ES_MULTILINE | ES_READONLY | ES_AUTOVSCROLL
};

static ControlId creation_order(int position)
{
    if (!CHECK(position >= 0) || !CHECK(position < CREATION_COUNT)) {
        return CONTROL_TITLE;
    }
    if (position < 3) {
        return (ControlId)position;
    }
    if (position < CREATION_COUNT - 3) {
        return (ControlId)(position + 3);
    }
    return (ControlId)(position - (CREATION_COUNT - 6));
}

typedef struct {
    const wchar_t *kind;
    DWORD style;
    int identifier;
} ChildSpec;

static HWND create_child(AppState *app, ChildSpec child)
{
    HWND window = CreateWindowExW(WS_EX_LEFT, child.kind, L"", WS_CHILD | child.style, CW_USEDEFAULT, CW_USEDEFAULT,
                                  CW_USEDEFAULT, CW_USEDEFAULT, app->window, (HMENU)(INT_PTR)child.identifier,
                                  app->instance, NULL);

    if ((child.style & WS_VSCROLL) == WS_VSCROLL) {
        assist_plain_bar(window);
    }
    return window;
}

static bool create_control(AppState *app, ControlId control)
{
    ControlSpec spec = layout_spec(control);
    ChildSpec child = { L"STATIC", SS_LEFT | SS_NOPREFIX, 0 };
    wchar_t label[LAYOUT_LABEL_WIDTH];

    if (!dialogs_widen(spec.label, label, LAYOUT_LABEL_WIDTH)) {
        return false;
    }
    if (layout_has_label(control)) {
        child.identifier = LABEL_ID_BASE + (int)control;
        app->labels[control] = create_child(app, child);
        if (!CHECK(app->labels[control] != NULL)) {
            return false;
        }
        (void)SetWindowTextW(app->labels[control], label);
    }
    child.kind = CLASSES[spec.kind];
    child.style = STYLES[spec.kind];
    child.identifier = CONTROL_ID_BASE + (int)control;
    app->controls[control] = create_child(app, child);
    if (!CHECK(app->controls[control] != NULL)) {
        return false;
    }
    if (spec.kind == WIDGET_BUTTON) {
        (void)SetWindowTextW(app->controls[control], label);
    }
    return true;
}

static void apply_fonts(AppState *app)
{
    int control;

    for (control = 0; control < CONTROL_COUNT; ++control) {
        HFONT font = (control == (int)CONTROL_TITLE) ? app->title_font : app->font;
        (void)SendMessageW(app->controls[control], WM_SETFONT, (WPARAM)font, TRUE);
        if (app->labels[control] != NULL) {
            (void)SendMessageW(app->labels[control], WM_SETFONT, (WPARAM)app->font, TRUE);
        }
        if (layout_spec((ControlId)control).kind == WIDGET_CHOICE) {
            (void)SendMessageW(app->controls[control], CB_SETITEMHEIGHT, (WPARAM)-1,
                               (LPARAM)(controls_row_height(app) - (2 * BOX_INSET)));
        }
    }
}

static HFONT make_font(int dpi, int points, int weight)
{
    if (!CHECK(dpi > 0) || !CHECK(points > 0)) {
        return NULL;
    }
    return CreateFontW(-MulDiv(points, dpi, POINTS_PER_INCH), 0, 0, 0, weight, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                       OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Segoe UI");
}

static COLORREF color_of(const AppState *app, ColorRole role)
{
    static const int SYSTEM_COLORS[COLOR_ROLE_COUNT] = { COLOR_WINDOW,    COLOR_WINDOWTEXT,    COLOR_WINDOW,
                                                         COLOR_WINDOWTEXT, COLOR_HIGHLIGHT,     COLOR_HIGHLIGHTTEXT,
                                                         COLOR_HIGHLIGHT };
    Rgb color = palette_color(role);

    return app->high_contrast ? GetSysColor(SYSTEM_COLORS[role]) : RGB(color.red, color.green, color.blue);
}

static void replace_brush(HBRUSH *brush, COLORREF color)
{
    if (!CHECK(color != CLR_INVALID)) {
        return;
    }
    if (*brush != NULL) {
        (void)DeleteObject(*brush);
    }
    *brush = CreateSolidBrush(color);
}

static void make_fonts(AppState *app)
{
    if (app->font != NULL) {
        (void)DeleteObject(app->font);
        (void)DeleteObject(app->title_font);
    }
    app->font = make_font(app->dpi, TEXT_POINTS, FW_NORMAL);
    app->title_font = make_font(app->dpi, TITLE_POINTS, FW_SEMIBOLD);
}

void controls_refresh_style(AppState *app)
{
    HIGHCONTRASTW contrast;
    int dpi;

    contrast.cbSize = sizeof contrast;
    app->high_contrast = (SystemParametersInfoW(SPI_GETHIGHCONTRAST, sizeof contrast, &contrast, 0) != 0) &&
                         ((contrast.dwFlags & HCF_HIGHCONTRASTON) != 0);
    dpi = assist_dpi(app->window);
    if ((app->font == NULL) || (dpi != app->dpi)) {
        app->dpi = dpi;
        make_fonts(app);
    }
    replace_brush(&app->paper_brush, color_of(app, COLOR_ROLE_PAPER));
    replace_brush(&app->field_brush, color_of(app, COLOR_ROLE_FIELD));
    replace_brush(&app->chosen_brush, color_of(app, COLOR_ROLE_CHOSEN));
    apply_fonts(app);
    controls_place(app);
}

bool controls_create(AppState *app)
{
    int position;

    for (position = 0; position < CREATION_COUNT; ++position) {
        if (!create_control(app, creation_order(position))) {
            return false;
        }
    }
    controls_refresh_style(app);
    assist_mark_live(app->controls[CONTROL_STATUS]);
    return true;
}

static void move_to(const AppState *app, HWND window, Box box)
{
    (void)MoveWindow(window, MulDiv(box.left, app->dpi, LAYOUT_BASE_DPI), MulDiv(box.top, app->dpi, LAYOUT_BASE_DPI),
                     MulDiv(box.width, app->dpi, LAYOUT_BASE_DPI),
                     MulDiv(box.height, app->dpi, LAYOUT_BASE_DPI), TRUE);
}

typedef struct {
    int width;
    int height;
    bool narrow;
} Room;

static Room room_of(const AppState *app)
{
    RECT view = assist_view(app);
    Room room;

    room.width = MulDiv(view.right, LAYOUT_BASE_DPI, app->dpi);
    room.height = MulDiv(view.bottom, LAYOUT_BASE_DPI, app->dpi);
    room.narrow = reflow_is_needed(room.width, room.height);
    room.width = (room.width < REFLOW_MIN_WIDTH) ? REFLOW_MIN_WIDTH : room.width;
    return room;
}

static Box control_box(const AppState *app, ControlId control, Room room, bool label)
{
    Box box;

    if (!room.narrow) {
        return label ? layout_label_box(control, room.width, room.height)
                     : layout_box(control, room.width, room.height);
    }
    box = label ? reflow_label_box(control, app->step, room.width) : reflow_box(control, app->step, room.width);
    box.top -= app->scroll;
    return box;
}

void controls_place(AppState *app)
{
    Room room = room_of(app);
    int control;

    app->scroll = assist_scroll_range(app, room.narrow ? reflow_height(app->step) : room.height, room.height);
    room = room_of(app);
    for (control = 0; control < CONTROL_COUNT; ++control) {
        Box box = control_box(app, (ControlId)control, room, false);
        box.height += (layout_spec((ControlId)control).kind == WIDGET_CHOICE) ? DROP_HEIGHT : 0;
        move_to(app, app->controls[control], box);
        if (app->labels[control] != NULL) {
            move_to(app, app->labels[control], control_box(app, (ControlId)control, room, true));
        }
    }
    assist_placed(app);
}

void controls_show_step(AppState *app)
{
    int control;
    int show;

    for (control = 0; control < CONTROL_COUNT; ++control) {
        show = layout_shows((ControlId)control, app->step) ? SW_SHOW : SW_HIDE;
        (void)ShowWindow(app->controls[control], show);
        if (app->labels[control] != NULL) {
            (void)ShowWindow(app->labels[control], show);
        }
    }
    app->scroll = 0;
    controls_place(app);
}

static bool is_field(int identifier)
{
    return (identifier >= 0) && (identifier < CONTROL_COUNT) && layout_is_focusable((ControlId)identifier);
}

HBRUSH controls_color(const AppState *app, HDC context, HWND control)
{
    int identifier;
    bool field;
    bool muted;

    identifier = GetDlgCtrlID(control) - CONTROL_ID_BASE;
    field = is_field(identifier);
    muted = (control == app->controls[CONTROL_HELP]);
    (void)SetTextColor(context, color_of(app, muted ? COLOR_ROLE_MUTED : COLOR_ROLE_INK));
    (void)SetBkColor(context, color_of(app, field ? COLOR_ROLE_FIELD : COLOR_ROLE_PAPER));
    return field ? app->field_brush : app->paper_brush;
}

static int scaled(const AppState *app, int units)
{
    return MulDiv(units, app->dpi, LAYOUT_BASE_DPI);
}

int controls_row_height(const AppState *app)
{
    return scaled(app, LAYOUT_ROW_HEIGHT);
}

static void frame_rect(HDC context, RECT area, COLORREF color, int thickness)
{
    HBRUSH brush;
    int ring;

    brush = CreateSolidBrush(color);
    if (brush == NULL) {
        return;
    }
    for (ring = 0; (ring < thickness) && (ring < RING_LIMIT); ++ring) {
        (void)FrameRect(context, &area, brush);
        (void)InflateRect(&area, -1, -1);
    }
    (void)DeleteObject(brush);
}

static void draw_check(HDC context, RECT box, COLORREF color, int width)
{
    int across = box.right - box.left;
    int down = box.bottom - box.top;
    HPEN pen = CreatePen(PS_SOLID, width, color);
    HGDIOBJ previous;

    if (pen == NULL) {
        return;
    }
    previous = SelectObject(context, pen);
    (void)MoveToEx(context, box.left, box.top + (down / CHECK_KNEE_DIVISOR), NULL);
    (void)LineTo(context, box.left + (across / CHECK_KNEE_DIVISOR), box.bottom);
    (void)LineTo(context, box.right, box.top);
    (void)SelectObject(context, previous);
    (void)DeleteObject(pen);
}

static RECT check_box(const AppState *app, RECT row)
{
    RECT box;
    int size = scaled(app, BOX_SIZE);

    box.left = row.left + scaled(app, TEXT_INSET);
    box.top = ((row.top + row.bottom) - size) / 2;
    box.right = box.left + size;
    box.bottom = box.top + size;
    return box;
}

static bool item_text(const DRAWITEMSTRUCT *item, wchar_t *text, int capacity)
{
    bool combo = (item->CtlType == ODT_COMBOBOX);
    LRESULT length;

    text[0] = L'\0';
    length = SendMessageW(item->hwndItem, combo ? CB_GETLBTEXTLEN : LB_GETTEXTLEN, item->itemID, 0);
    if ((length < 0) || (length >= capacity)) {
        return false;
    }
    return SendMessageW(item->hwndItem, combo ? CB_GETLBTEXT : LB_GETTEXT, item->itemID, (LPARAM)text) >= 0;
}

static bool is_checklist(int control)
{
    if ((control < 0) || (control >= CONTROL_COUNT)) {
        return false;
    }
    return layout_spec((ControlId)control).kind == WIDGET_CHECKLIST;
}

static int draw_box(const AppState *app, const DRAWITEMSTRUCT *item, bool chosen)
{
    RECT box;
    int right;

    box = check_box(app, item->rcItem);
    right = box.right;
    frame_rect(item->hDC, box, color_of(app, chosen ? COLOR_ROLE_CHOSEN_INK : COLOR_ROLE_INK), scaled(app, BOX_LINE));
    (void)InflateRect(&box, -scaled(app, BOX_INSET), -scaled(app, BOX_INSET));
    if (chosen) {
        draw_check(item->hDC, box, color_of(app, COLOR_ROLE_CHOSEN_INK), scaled(app, BOX_LINE));
    }
    return right;
}

static void draw_row_focus(const AppState *app, const DRAWITEMSTRUCT *item, bool chosen)
{
    if ((item->itemState & ODS_FOCUS) == 0) {
        return;
    }
    frame_rect(item->hDC, item->rcItem, color_of(app, chosen ? COLOR_ROLE_CHOSEN_INK : COLOR_ROLE_FOCUS),
               scaled(app, FOCUS_LINE));
}

void controls_draw_item(const AppState *app, const DRAWITEMSTRUCT *item)
{
    wchar_t text[ROW_TEXT_CAPACITY];
    bool chosen;
    bool checklist;
    RECT area;

    if (item->itemID == (UINT)-1) {
        return;
    }
    chosen = ((item->itemState & ODS_SELECTED) != 0) && ((item->itemState & ODS_COMBOBOXEDIT) == 0);
    checklist = is_checklist((int)item->CtlID - CONTROL_ID_BASE);
    (void)FillRect(item->hDC, &item->rcItem, chosen ? app->chosen_brush : app->field_brush);
    area = item->rcItem;
    area.left += scaled(app, TEXT_INSET);
    if (checklist) {
        area.left = draw_box(app, item, chosen) + scaled(app, TEXT_INSET);
    }
    (void)SetBkMode(item->hDC, TRANSPARENT);
    (void)SetTextColor(item->hDC, color_of(app, chosen ? COLOR_ROLE_CHOSEN_INK : COLOR_ROLE_INK));
    if (item_text(item, text, ROW_TEXT_CAPACITY)) {
        (void)DrawTextW(item->hDC, text, -1, &area, assist_row_format(app, item, text, &area));
    }
    draw_row_focus(app, item, chosen);
}
