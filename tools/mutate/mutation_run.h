#ifndef MUTATION_RUN_H
#define MUTATION_RUN_H

#include <stdbool.h>
#include <stddef.h>

enum { MUTATION_PATH_CAPACITY = 260 };

typedef enum {
    MUTANT_KILLED,
    MUTANT_SURVIVED,
    MUTANT_NOT_COMPILED,
    MUTANT_TIMED_OUT
} MutantFate;

bool mutation_path(char *destination, int worker, const char *name);
bool mutation_prepare(int worker, const char *source_path);
MutantFate mutation_try(int worker, const char *source_path, const char *text, size_t length);

#endif
