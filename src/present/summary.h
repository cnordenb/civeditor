#ifndef SUMMARY_H
#define SUMMARY_H

#include <stdbool.h>

#include "../data/catalog.h"
#include "../model/civ_design.h"

enum { SUMMARY_CAPACITY = 16384 };

typedef struct {
    char text[SUMMARY_CAPACITY];
} SummaryText;

bool summary_write(SummaryText *summary, const CivDesign *design, const Catalog *catalog);

#endif
