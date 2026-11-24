#include "chosen_parts.h"

#include <wchar.h>

#include "../core/check.h"
#include "../core/text.h"

enum { TEXT_ENDS = 3 };

static ChosenRow drawn;

ChosenLines chosen_lines(const AppState *app, HDC context)
{
    TEXTMETRICW metrics;
    ChosenLines lines;
    int least = assist_scaled(app, CHOSEN_FIELD_LEAST);

    lines.text = (GetTextMetricsW(context, &metrics) != 0) ? metrics.tmHeight : least;
    lines.field = (lines.text + (2 * CHOSEN_FIELD_INSET) > least) ? lines.text + (2 * CHOSEN_FIELD_INSET) : least;
    lines.line = lines.field + assist_scaled(app, CHOSEN_LINE_GAP);
    lines.pad = assist_scaled(app, ROW_PAD);
    lines.inset = assist_scaled(app, TEXT_INSET);
    return lines;
}

int chosen_text_width(HDC context, const char *text)
{
    wchar_t wide[BONUS_TEXT_CAPACITY];
    SIZE size = { 0, 0 };

    if (!text_widen(text, wide, BONUS_TEXT_CAPACITY)) {
        return 0;
    }
    (void)GetTextExtentPoint32W(context, wide, (int)wcslen(wide), &size);
    return size.cx;
}

static int field_width(const AppState *app, HDC context, NumberFocus focus)
{
    NumberPlace place = chosen_place_of(app, focus);
    double times[TEXT_ENDS] = { number_times(place), 1.0, (double)CIV_MULTIPLIER_LIMIT };
    char text[NUMBER_TEXT_CAPACITY];
    int least = chosen_lines(app, context).field;
    int widest = 0;
    int width;
    int end;

    for (end = 0; end < TEXT_ENDS; ++end) {
        width = number_text(place, times[end], NUMBER_DIGITS, text, sizeof text) ? chosen_text_width(context, text) : 0;
        widest = (width > widest) ? width : widest;
    }
    width = widest + (2 * (assist_scaled(app, CHOSEN_FIELD_MARGIN) + CHOSEN_FIELD_INSET));
    return (width > least) ? width : least;
}

static int piece_width(HDC context, const BonusShown *shown, const NumberPiece *piece)
{
    char part[BONUS_TEXT_CAPACITY];

    _Static_assert(sizeof part >= sizeof shown->text, "a piece of a shown text fits in a part");
    (void)text_copy_span(part, sizeof part, &shown->text[piece->start], piece->end - piece->start);
    return chosen_text_width(context, part);
}

bool chosen_lay_out(const AppState *app, HDC context, NumberFocus focus, int width, ChosenRow *row)
{
    NumberFocus field = focus;
    const NumberPiece *at;
    int piece;

    if (!number_row(chosen_place_of(app, focus), &row->shown)) {
        return false;
    }
    number_layout_cut(&row->layout, &row->shown);
    for (piece = 0; piece < row->layout.count; ++piece) {
        at = &row->layout.pieces[piece];
        field.number = at->number;
        row->layout.widths[piece] = (at->number == NUMBER_NONE) ? piece_width(context, &row->shown, at)
                                                                : field_width(app, context, field);
    }
    number_layout_place(&row->layout, width);
    return true;
}

static void draw_piece(const AppState *app, NumberPanel panel, const DRAWITEMSTRUCT *item, ChosenLines lines,
                       int piece)
{
    const NumberPiece *at = &drawn.layout.pieces[piece];
    int pick = (int)item->CtlID - CHOSEN_ROW_ID_BASE;
    char part[BONUS_TEXT_CAPACITY];
    wchar_t wide[BONUS_TEXT_CAPACITY];
    RECT area;

    area.left = item->rcItem.left + lines.inset + drawn.layout.x[piece];
    area.top = item->rcItem.top + (lines.pad / 2) + (drawn.layout.line[piece] * lines.line);
    area.right = area.left + drawn.layout.widths[piece];
    area.bottom = area.top + lines.field;
    if (at->number != NUMBER_NONE) {
        if (GetFocus() == app->chosen[panel].fields[pick][at->number]) {
            assist_draw_ring(app, item->hDC, area);
        }
        return;
    }
    if (text_copy_span(part, sizeof part, &drawn.shown.text[at->start], at->end - at->start) &&
        text_widen(part, wide, BONUS_TEXT_CAPACITY)) {
        (void)TextOutW(item->hDC, area.left, area.top + CHOSEN_FIELD_EDGE, wide, (int)wcslen(wide));
    }
}

void chosen_draw_row(const AppState *app, NumberPanel panel, const DRAWITEMSTRUCT *item)
{
    HGDIOBJ before = SelectObject(item->hDC, app->font);
    ChosenLines lines = chosen_lines(app, item->hDC);
    NumberFocus row = { panel, (int)item->CtlID - CHOSEN_ROW_ID_BASE, 0 };
    int width = item->rcItem.right - item->rcItem.left - (2 * lines.inset);
    int piece;

    (void)FillRect(item->hDC, &item->rcItem, app->field_brush);
    if (CHECK(check_is_index((int)panel, NUMBER_PANEL_COUNT)) && CHECK(check_is_index(row.pick, CIV_BONUS_CAPACITY)) &&
        chosen_lay_out(app, item->hDC, row, width, &drawn)) {
        (void)SetBkMode(item->hDC, TRANSPARENT);
        (void)SetTextColor(item->hDC, controls_row_color(app, false, COLOR_ROLE_INK));
        for (piece = 0; (piece < drawn.layout.count) && (piece < NUMBER_PIECE_CAPACITY); ++piece) {
            draw_piece(app, panel, item, lines, piece);
        }
    }
    (void)SelectObject(item->hDC, before);
}

static void draw_end(HDC context, RECT area, double times, NumberPlace place)
{
    char text[NUMBER_TEXT_CAPACITY];
    wchar_t wide[NUMBER_TEXT_CAPACITY];
    UINT side = (times > 1.0) ? DT_RIGHT : DT_LEFT;

    if (number_text(place, times, NUMBER_WITH_UNIT, text, sizeof text) &&
        text_widen(text, wide, NUMBER_TEXT_CAPACITY)) {
        (void)DrawTextW(context, wide, -1, &area, side | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);
    }
}

static void paint_card(const AppState *app, HWND card, HDC context)
{
    NumberPlace place = chosen_place_of(app, app->number_focus);
    int gap = assist_scaled(app, CHOSEN_CARD_GAP);
    RECT client = { 0, 0, 0, 0 };
    RECT track = { 0, 0, 0, 0 };
    HGDIOBJ before = SelectObject(context, app->font);
    RECT end;

    (void)GetClientRect(card, &client);
    (void)GetWindowRect(GetDlgItem(card, CHOSEN_SLIDER_ID), &track);
    (void)MapWindowPoints(NULL, card, (POINT *)&track, 2);
    (void)FillRect(context, &client, app->paper_brush);
    controls_frame_rect(context, client, controls_row_color(app, false, COLOR_ROLE_INK), CHOSEN_FIELD_EDGE);
    (void)SetBkMode(context, TRANSPARENT);
    (void)SetTextColor(context, controls_row_color(app, false, COLOR_ROLE_INK));
    end = client;
    end.left = gap;
    end.right = track.left - gap;
    draw_end(context, end, 1.0, place);
    end.left = track.right + gap;
    end.right = client.right - gap;
    draw_end(context, end, (double)CIV_MULTIPLIER_LIMIT, place);
    (void)SelectObject(context, before);
}

void chosen_print(const AppState *app, HWND window, HDC context)
{
    RECT client = { 0, 0, 0, 0 };

    if (chosen_is_card(app, window)) {
        paint_card(app, window, context);
        return;
    }
    (void)GetClientRect(window, &client);
    (void)FillRect(context, &client, app->field_brush);
}

void chosen_paint(const AppState *app, HWND window)
{
    PAINTSTRUCT paint;
    HDC context = BeginPaint(window, &paint);

    if (context != NULL) {
        chosen_print(app, window, context);
        (void)EndPaint(window, &paint);
    }
}

LRESULT chosen_color(const AppState *app, HWND window, HDC context)
{
    bool card = chosen_is_card(app, window);

    (void)SetTextColor(context, controls_row_color(app, false, COLOR_ROLE_INK));
    (void)SetBkColor(context, controls_row_color(app, false, card ? COLOR_ROLE_PAPER : COLOR_ROLE_FIELD));
    return (LRESULT)(card ? app->paper_brush : app->field_brush);
}
