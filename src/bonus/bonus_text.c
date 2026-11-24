#include "bonus_text.h"

#include <math.h>
#include <string.h>

#include "../core/text.h"
#include "bonus_scale.h"

enum { TENTHS = 10, CHANGE_WORD_COUNT = 5 };

static const double PERCENT = 100.0;
static const double HALF = 0.5;
static const double AMOUNT_LIMIT = 1.0e8;

typedef struct {
    BonusShown *shown;
    char *text;
    double times;
    bool fits;
} ScaledText;

typedef struct {
    const char *word;
    int sign;
} ChangeWord;

static const ChangeWord CHANGE_WORDS[CHANGE_WORD_COUNT] = {
    { " faster", 1 }, { " more", 1 }, { " longer", 1 }, { " less", -1 }, { " cheaper", -1 }
};

static bool is_letter(char letter)
{
    return text_is_between(letter, 'a', 'z') || text_is_between(letter, 'A', 'Z');
}

static bool starts_signed(const char *text, size_t at)
{
    return text_is_sign(text[at]) && text_is_digit(text[at + 1U]) &&
           ((at == 0U) || (!is_letter(text[at - 1U]) && !text_is_digit(text[at - 1U])));
}

static size_t past_digits(const char *text, size_t at)
{
    size_t end = at;

    while (text_is_digit(text[end])) {
        ++end;
    }
    return end;
}

static size_t joint_width(const char *text, size_t at)
{
    if (text[at] == '/') {
        return 1U;
    }
    return ((text[at] == ',') && (text[at + 1U] == ' ')) ? 2U : 0U;
}

static size_t list_end(const char *text, size_t at)
{
    size_t end = at;
    size_t next = at;

    while (text_is_digit(text[next]) || starts_signed(text, next)) {
        end = past_digits(text, next + (text_is_sign(text[next]) ? 1U : 0U));
        next = end + joint_width(text, end);
    }
    return end;
}

static double whole_of(const char *text, size_t at, size_t end)
{
    double whole = 0.0;
    size_t place;

    for (place = at; place < end; ++place) {
        whole = (whole * (double)TENTHS) + (double)(text[place] - '0');
    }
    return whole;
}

static size_t used(const ScaledText *scaled)
{
    return text_length(scaled->text, (size_t)BONUS_TEXT_CAPACITY);
}

static void put_text(ScaledText *scaled, const char *text)
{
    scaled->fits = scaled->fits && text_append(scaled->text, (size_t)BONUS_TEXT_CAPACITY, text);
}

static void put_letter(ScaledText *scaled, char letter)
{
    char one[2];

    one[0] = letter;
    one[1] = '\0';
    put_text(scaled, one);
}

static bool is_settable(BonusNumber number)
{
    if (number.kind == BONUS_NUMBER_POWER) {
        return number.base > 1.0;
    }
    if (number.kind == BONUS_NUMBER_PERCENT) {
        return (fabs(number.base) > 0.0) && (number.base > -1.0);
    }
    return number.base > 0.0;
}

static void put_number(ScaledText *scaled, BonusNumber number)
{
    BonusShown *shown = scaled->shown;
    double tenths = bonus_tenths(fmin(bonus_number_value(number, scaled->times), AMOUNT_LIMIT));

    number.start = used(scaled);
    scaled->fits = scaled->fits && text_append_decimal(shown->text, sizeof shown->text, (long)tenths, 1U);
    number.end = used(scaled);
    if (is_settable(number) && (shown->count < BONUS_NUMBER_CAPACITY)) {
        shown->numbers[shown->count] = number;
        ++shown->count;
    }
}

static size_t put_part(ScaledText *scaled, const char *text, size_t at, double sign, bool percent)
{
    size_t end = past_digits(text, at);
    BonusNumber number = { BONUS_NUMBER_AMOUNT, whole_of(text, at, end), 0U, 0U };

    if (percent) {
        number.kind = BONUS_NUMBER_PERCENT;
        number.base = (sign * number.base) / PERCENT;
    }
    put_number(scaled, number);
    return end;
}

static size_t put_list(ScaledText *scaled, const char *text, size_t at)
{
    size_t end = list_end(text, at);
    bool percent = text[end] == '%';
    double sign = 1.0;
    size_t place = at;

    while (place < end) {
        if (text_is_digit(text[place])) {
            place = put_part(scaled, text, place, sign, percent);
            continue;
        }
        if (text_is_sign(text[place])) {
            sign = (text[place] == '-') ? -1.0 : 1.0;
        }
        put_letter(scaled, text[place]);
        ++place;
    }
    return end;
}

static int change_sign(const char *text, size_t at)
{
    int word;

    for (word = 0; word < CHANGE_WORD_COUNT; ++word) {
        if (text_starts_with(&text[at], CHANGE_WORDS[word].word) &&
            !is_letter(text[at + strlen(CHANGE_WORDS[word].word)])) {
            return CHANGE_WORDS[word].sign;
        }
    }
    return 0;
}

static size_t put_unsigned(ScaledText *scaled, const char *text, size_t at)
{
    size_t end = past_digits(text, at);
    BonusNumber power = { BONUS_NUMBER_POWER, whole_of(text, at, end), 0U, 0U };
    int sign = (text[end] == '%') ? change_sign(text, end + 1U) : 0;
    size_t place;

    if (text[end] == 'x') {
        put_number(scaled, power);
        return end;
    }
    if (sign != 0) {
        return put_part(scaled, text, at, (double)sign, true);
    }
    for (place = at; place < end; ++place) {
        put_letter(scaled, text[place]);
    }
    return end;
}

static size_t scale_at(ScaledText *scaled, const char *text, size_t at)
{
    if (starts_signed(text, at)) {
        return put_list(scaled, text, at);
    }
    if (text_is_digit(text[at])) {
        return put_unsigned(scaled, text, at);
    }
    put_letter(scaled, text[at]);
    return at + 1U;
}

bool bonus_text_show(const char *text, double times, BonusShown *shown)
{
    ScaledText scaled = { shown, shown->text, times, true };
    size_t at = 0U;

    shown->text[0] = '\0';
    shown->count = 0;
    while (scaled.fits && (text[at] != '\0')) {
        at = scale_at(&scaled, text, at);
    }
    return scaled.fits;
}

bool bonus_text_scaled(const char *text, double times, char *scaled, size_t capacity)
{
    static BonusShown shown;

    return bonus_text_show(text, times, &shown) && text_copy(scaled, capacity, shown.text);
}

double bonus_tenths(double value)
{
    return floor((value * (double)TENTHS) + HALF);
}

double bonus_number_value(BonusNumber number, double times)
{
    if (number.kind == BONUS_NUMBER_PERCENT) {
        return fabs(bonus_raised(1.0 + number.base, times) - 1.0) * PERCENT;
    }
    if (number.kind == BONUS_NUMBER_POWER) {
        return bonus_raised(number.base, times);
    }
    return number.base * times;
}

double bonus_number_times(BonusNumber number, double value)
{
    if (number.kind == BONUS_NUMBER_PERCENT) {
        return log(1.0 + (copysign(value, number.base) / PERCENT)) / log(1.0 + number.base);
    }
    if (number.kind == BONUS_NUMBER_POWER) {
        return log(value) / log(number.base);
    }
    return value / number.base;
}
