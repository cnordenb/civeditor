#ifndef CODE_PATTERNS_H
#define CODE_PATTERNS_H

#include "../scan/source_scan.h"

enum { PATTERN_WORD_WIDTH = 24, PATTERN_WORD_LIMIT = 11 };

int pattern_line_at(const ScannedSource *source, int offset);
int pattern_find_word(const ScannedSource *source, int from, const char *word);
int pattern_first_word_line(const ScannedSource *source, const char words[][PATTERN_WORD_WIDTH], int count);
int pattern_forbidden_flow_line(const ScannedSource *source);

#endif
