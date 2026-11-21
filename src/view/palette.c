#include "palette.h"

#include <math.h>

#include "../core/check.h"

static const Rgb COLORS[COLOR_ROLE_COUNT] = {
    { 250, 247, 240 },
    { 28, 28, 28 },
    { 255, 255, 255 },
    { 58, 58, 58 },
    { 24, 52, 96 },
    { 255, 255, 255 },
    { 24, 52, 96 },
    { 20, 110, 40 }
};

static const ColorPair PAIRS[PALETTE_PAIR_COUNT] = {
    { COLOR_ROLE_INK, COLOR_ROLE_PAPER },
    { COLOR_ROLE_INK, COLOR_ROLE_FIELD },
    { COLOR_ROLE_MUTED, COLOR_ROLE_PAPER },
    { COLOR_ROLE_MUTED, COLOR_ROLE_FIELD },
    { COLOR_ROLE_CHOSEN_INK, COLOR_ROLE_CHOSEN },
    { COLOR_ROLE_FOCUS, COLOR_ROLE_FIELD },
    { COLOR_ROLE_FOCUS, COLOR_ROLE_PAPER },
    { COLOR_ROLE_SUCCESS, COLOR_ROLE_PAPER }
};

static const double LINEAR_LIMIT = 0.04045;
static const double LINEAR_SLOPE = 12.92;
static const double GAMMA_OFFSET = 0.055;
static const double GAMMA_SCALE = 1.055;
static const double GAMMA = 2.4;
static const double RED_WEIGHT = 0.2126;
static const double GREEN_WEIGHT = 0.7152;
static const double BLUE_WEIGHT = 0.0722;
static const double FLARE = 0.05;
static const double HUNDRED = 100.0;

Rgb palette_color(ColorRole role)
{
    return COLORS[check_index((int)role, COLOR_ROLE_COUNT, COLOR_ROLE_INK)];
}

ColorPair palette_pair(int index)
{
    if (!CHECK(index >= 0) || !CHECK(index < PALETTE_PAIR_COUNT)) {
        return PAIRS[0];
    }
    return PAIRS[index];
}

static double channel_light(int channel)
{
    double value = (double)channel / (double)PALETTE_CHANNEL_LIMIT;

    if (!CHECK(channel >= 0) || !CHECK(channel <= PALETTE_CHANNEL_LIMIT)) {
        return 0.0;
    }
    if (value <= LINEAR_LIMIT) {
        return value / LINEAR_SLOPE;
    }
    return pow((value + GAMMA_OFFSET) / GAMMA_SCALE, GAMMA);
}

static double luminance(Rgb color)
{
    if (!CHECK(color.red <= PALETTE_CHANNEL_LIMIT) || !CHECK(color.blue <= PALETTE_CHANNEL_LIMIT)) {
        return 0.0;
    }
    return (RED_WEIGHT * channel_light(color.red)) + (GREEN_WEIGHT * channel_light(color.green)) +
           (BLUE_WEIGHT * channel_light(color.blue));
}

int palette_contrast_hundredths(Rgb text, Rgb background)
{
    double first = luminance(text);
    double second = luminance(background);
    double lighter = (first > second) ? first : second;
    double darker = (first > second) ? second : first;

    if (!CHECK(lighter >= 0.0) || !CHECK(darker >= 0.0)) {
        return 0;
    }
    return (int)(((lighter + FLARE) / (darker + FLARE)) * HUNDRED);
}
