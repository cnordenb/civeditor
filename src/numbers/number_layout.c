#include "number_layout.h"

#include "../core/text.h"

enum { CARD_GAPS = 4 };

void number_span(const BonusShown *shown, int number, size_t *start, size_t *end)
{
    size_t first = shown->numbers[number].start;

    if ((first > 0U) && text_is_sign(shown->text[first - 1U])) {
        --first;
    }
    *start = first;
    *end = shown->numbers[number].end;
}

size_t number_unit_end(const BonusShown *shown, size_t end)
{
    return ((shown->text[end] == '%') || (shown->text[end] == 'x')) ? end + 1U : end;
}

static int number_at(const BonusShown *shown, size_t at, size_t *end)
{
    size_t first;
    int number;

    for (number = 0; number < shown->count; ++number) {
        number_span(shown, number, &first, end);
        if (first == at) {
            return number;
        }
    }
    return NUMBER_NONE;
}

static size_t word_end(const BonusShown *shown, size_t at)
{
    size_t end = at + 1U;
    size_t ignored;

    while ((shown->text[end] != '\0') && (shown->text[end] != ' ') &&
           (number_at(shown, end, &ignored) == NUMBER_NONE)) {
        ++end;
    }
    return end;
}

static size_t space_end(const char *text, size_t at)
{
    size_t end = at;

    while (text[end] == ' ') {
        ++end;
    }
    return end;
}

void number_layout_cut(NumberLayout *layout, const BonusShown *shown)
{
    NumberPiece piece;
    size_t at = 0U;

    layout->count = 0;
    while ((shown->text[at] != '\0') && (layout->count < NUMBER_PIECE_CAPACITY)) {
        piece.start = at;
        piece.number = number_at(shown, at, &piece.end);
        piece.space = shown->text[at] == ' ';
        if (piece.number == NUMBER_NONE) {
            piece.end = piece.space ? space_end(shown->text, at) : word_end(shown, at);
        }
        layout->pieces[layout->count] = piece;
        ++layout->count;
        at = piece.end;
    }
}

static bool starts_word(const NumberLayout *layout, int piece)
{
    return !layout->pieces[piece].space && ((piece == 0) || layout->pieces[piece - 1].space);
}

static int word_width(const NumberLayout *layout, int first)
{
    int width = 0;
    int piece;

    for (piece = first; (piece < layout->count) && !layout->pieces[piece].space; ++piece) {
        width += layout->widths[piece];
    }
    return width;
}

void number_layout_place(NumberLayout *layout, int width)
{
    int x = 0;
    int line = 0;
    int piece;

    for (piece = 0; piece < layout->count; ++piece) {
        if (starts_word(layout, piece) && (x != 0) && (x + word_width(layout, piece) > width)) {
            ++line;
            x = 0;
        }
        layout->x[piece] = x;
        layout->line[piece] = line;
        x += layout->widths[piece];
    }
    layout->lines = line + 1;
}

int number_layout_piece(const NumberLayout *layout, int number)
{
    int piece;

    for (piece = 0; piece < layout->count; ++piece) {
        if (layout->pieces[piece].number == number) {
            return piece;
        }
    }
    return NUMBER_NONE;
}

static int bounded(int value, int least, int most)
{
    int below = (value > most) ? most : value;

    return (below < least) ? least : below;
}

NumberCard number_card_place(int center, int area, NumberCardSizes sizes)
{
    int fixed = sizes.least + sizes.most + (CARD_GAPS * sizes.gap);
    NumberCard card;

    card.track_width = bounded(area - fixed, 0, sizes.track);
    card.width = fixed + card.track_width;
    card.left = bounded(center - (card.width / 2), 0, area - card.width);
    card.track_left = card.left + (2 * sizes.gap) + sizes.least;
    return card;
}
