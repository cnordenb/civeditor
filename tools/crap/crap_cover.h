#ifndef CRAP_COVER_H
#define CRAP_COVER_H

#include <stdbool.h>

#include "../../tests/scan/source_scan.h"

typedef struct {
    int covered;
    int total;
} CoveredShare;

typedef struct {
    bool ran[SCAN_LINE_LIMIT + 1];
    unsigned char covered[SCAN_LINE_LIMIT + 1];
    unsigned char total[SCAN_LINE_LIMIT + 1];
    int line;
    bool counted;
} LineCoverage;

void crap_cover_start(LineCoverage *coverage);
bool crap_cover_take_line(LineCoverage *coverage, const char *line);
bool crap_cover_read(LineCoverage *coverage, const char *source_path);
CoveredShare crap_cover_share(const LineCoverage *coverage, int first_line, int last_line);

#endif
