#ifndef BONUS_TEXT_H
#define BONUS_TEXT_H

#include <stdbool.h>
#include <stddef.h>

enum { BONUS_TEXT_CAPACITY = 640, BONUS_NUMBER_CAPACITY = 16 };

typedef enum { BONUS_NUMBER_AMOUNT, BONUS_NUMBER_PERCENT, BONUS_NUMBER_POWER } BonusNumberKind;

typedef struct {
    BonusNumberKind kind;
    double base;
    size_t start;
    size_t end;
} BonusNumber;

typedef struct {
    char text[BONUS_TEXT_CAPACITY];
    BonusNumber numbers[BONUS_NUMBER_CAPACITY];
    int count;
} BonusShown;

bool bonus_text_scaled(const char *text, double times, char *scaled, size_t capacity);
bool bonus_text_show(const char *text, double times, BonusShown *shown);
double bonus_number_value(BonusNumber number, double times);
double bonus_number_times(BonusNumber number, double value);
double bonus_tenths(double value);

#endif
