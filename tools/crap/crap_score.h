#ifndef CRAP_SCORE_H
#define CRAP_SCORE_H

enum { CRAP_MILLI = 1000, CRAP_LIMIT_MILLI = 10000 };

long crap_score_milli(int complexity, int covered, int total);
int crap_complexity(const char *text, int start, int end);

#endif
