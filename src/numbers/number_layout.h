#ifndef NUMBER_LAYOUT_H
#define NUMBER_LAYOUT_H

#include <stdbool.h>
#include <stddef.h>

#include "../bonus/bonus_text.h"

enum { NUMBER_NONE = -1, NUMBER_PIECE_CAPACITY = 192 };

typedef struct {
    size_t start;
    size_t end;
    int number;
    bool space;
} NumberPiece;

typedef struct {
    NumberPiece pieces[NUMBER_PIECE_CAPACITY];
    int widths[NUMBER_PIECE_CAPACITY];
    int x[NUMBER_PIECE_CAPACITY];
    int line[NUMBER_PIECE_CAPACITY];
    int count;
    int lines;
} NumberLayout;

typedef struct {
    int least;
    int most;
    int track;
    int gap;
} NumberCardSizes;

typedef struct {
    int left;
    int width;
    int track_left;
    int track_width;
} NumberCard;

void number_span(const BonusShown *shown, int number, size_t *start, size_t *end);
size_t number_unit_end(const BonusShown *shown, size_t end);
void number_layout_cut(NumberLayout *layout, const BonusShown *shown);
void number_layout_place(NumberLayout *layout, int width);
int number_layout_piece(const NumberLayout *layout, int number);
NumberCard number_card_place(int center, int area, NumberCardSizes sizes);

#endif
