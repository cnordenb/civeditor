#ifndef P10_PATTERNS_H
#define P10_PATTERNS_H

#include "../../scan/source_scan.h"

int pattern_allocation_line(const ScannedSource *source);
int pattern_complex_directive_line(const ScannedSource *source);
int pattern_deep_pointer_line(const ScannedSource *source);
int pattern_function_pointer_line(const ScannedSource *source, const char *path);

#endif
