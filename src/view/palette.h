#ifndef PALETTE_H
#define PALETTE_H

#include <stdbool.h>

enum { PALETTE_AAA_CONTRAST = 700, PALETTE_CHANNEL_LIMIT = 255, PALETTE_PAIR_COUNT = 7 };

typedef enum {
    COLOR_ROLE_PAPER,
    COLOR_ROLE_INK,
    COLOR_ROLE_FIELD,
    COLOR_ROLE_MUTED,
    COLOR_ROLE_CHOSEN,
    COLOR_ROLE_CHOSEN_INK,
    COLOR_ROLE_FOCUS,
    COLOR_ROLE_COUNT
} ColorRole;

typedef struct {
    int red;
    int green;
    int blue;
} Rgb;

typedef struct {
    ColorRole text;
    ColorRole background;
} ColorPair;

Rgb palette_color(ColorRole role);
ColorPair palette_pair(int index);
int palette_contrast_hundredths(Rgb text, Rgb background);

#endif
