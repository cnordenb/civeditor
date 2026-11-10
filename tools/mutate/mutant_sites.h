#ifndef MUTANT_SITES_H
#define MUTANT_SITES_H

#include <stdbool.h>

enum { MUTANT_WITH_CAPACITY = 8 };

typedef struct {
    int at;
    int length;
    int skip;
    char with[MUTANT_WITH_CAPACITY];
    bool found;
} MutantSite;

MutantSite mutant_next(const char *text, int from, int end);

#endif
