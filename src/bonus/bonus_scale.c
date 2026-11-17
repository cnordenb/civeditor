#include "bonus_scale.h"

enum {
    MODE_MULTIPLY = 2,
    ARMOR = 8,
    ATTACK = 9,
    EFFECT_FUNCTION = 33,
    SPAWN_LIMIT = 234,
    PACKED_CLASS = ~0xFF,
    PACKED_MOST = 255
};

static const double PERCENT = 100.0;
static const double PACKED_MOST_FACTOR = 2.55;
static const double HALF = 0.5;

static double raised(double base, int power)
{
    double result = 1.0;
    int step;

    for (step = 0; step < power; ++step) {
        result *= base;
    }
    return result;
}

static int nearest(double value)
{
    int whole = (int)value;
    double rest = value - (double)whole;

    if ((rest > HALF) || (!(rest < HALF) && ((whole % 2) != 0))) {
        ++whole;
    }
    return whole;
}

static void put(ScaledChanges *scaled, const DatChange *change, float d)
{
    scaled->changes[scaled->count] = *change;
    scaled->changes[scaled->count].d = d;
    ++scaled->count;
}

static float sign_of(const DatChange *change)
{
    return (change->d < 0.0F) ? -1.0F : 1.0F;
}

static int packed_value(const DatChange *change)
{
    return (int)(change->d * sign_of(change)) & PACKED_MOST;
}

static void put_packed(ScaledChanges *scaled, const DatChange *change, int value)
{
    int packed = (int)(change->d * sign_of(change));

    put(scaled, change, sign_of(change) * (float)((packed & PACKED_CLASS) | value));
}

static void scale_packed_add(ScaledChanges *scaled, const DatChange *change, int multiplier)
{
    int left = packed_value(change) * multiplier;

    while (left > PACKED_MOST) {
        put_packed(scaled, change, PACKED_MOST);
        left -= PACKED_MOST;
    }
    put_packed(scaled, change, left);
}

static void scale_packed_multiply(ScaledChanges *scaled, const DatChange *change, int multiplier)
{
    int value = packed_value(change);
    double factor = raised((double)value / PERCENT, multiplier);
    int made;

    for (made = 1; (made < multiplier) && (factor > PACKED_MOST_FACTOR); ++made) {
        put_packed(scaled, change, PACKED_MOST);
        factor /= PACKED_MOST_FACTOR;
    }
    put_packed(scaled, change, nearest(factor * PERCENT));
}

static bool is_packed(const DatChange *change)
{
    return ((change->type == DAT_CHANGE_ADD) || (change->type == DAT_CHANGE_MULTIPLY)) &&
           ((change->c == ARMOR) || (change->c == ATTACK));
}

static bool is_power(const DatChange *change)
{
    if ((change->type == DAT_CHANGE_TECH_COST) || (change->type == DAT_CHANGE_TECH_TIME)) {
        return change->c == MODE_MULTIPLY;
    }
    return (change->type == DAT_CHANGE_MULTIPLY) || (change->type == DAT_CHANGE_MULTIPLY_RESOURCE);
}

static bool is_linear(const DatChange *change)
{
    if (change->type == DAT_CHANGE_RESOURCE) {
        return (change->a != SPAWN_LIMIT) && (change->a != EFFECT_FUNCTION);
    }
    return change->type == DAT_CHANGE_ADD;
}

ScaledChanges bonus_scale(const DatChange *change, int multiplier)
{
    ScaledChanges scaled;
    DatChange kept = *change;

    scaled.count = 0;
    if (is_packed(change) && (change->type == DAT_CHANGE_ADD)) {
        scale_packed_add(&scaled, change, multiplier);
    } else if (is_packed(change)) {
        scale_packed_multiply(&scaled, change, multiplier);
    } else if (is_power(change)) {
        put(&scaled, change, (float)raised((double)change->d, multiplier));
    } else if (is_linear(change)) {
        put(&scaled, change, change->d * (float)multiplier);
    } else {
        kept.c = (change->type == DAT_CHANGE_SPAWN) ? change->c * multiplier : change->c;
        put(&scaled, &kept, kept.d);
    }
    return scaled;
}

bool bonus_add_scaled(DatEdit *edit, const DatChange *change, int multiplier)
{
    ScaledChanges scaled = bonus_scale(change, multiplier);
    int index;
    bool fine = true;

    for (index = 0; fine && (index < scaled.count); ++index) {
        fine = dat_edit_add_change(edit, &scaled.changes[index]);
    }
    return fine;
}
