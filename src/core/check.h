#ifndef CHECK_H
#define CHECK_H

#include <stdbool.h>
#include <stddef.h>

#define CHECK(condition) check_report((condition), __FILE__, __LINE__, #condition)

enum { CHECK_MESSAGE_CAPACITY = 256 };

bool check_report(bool holds, const char *file, int line, const char *condition_text);
int check_failure_count(void);
const char *check_last_failure(void);
void check_forget_failures(void);

#endif
