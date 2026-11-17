#ifndef SOURCE_SCAN_H
#define SOURCE_SCAN_H

#include <stdbool.h>

enum {
    SCAN_TEXT_CAPACITY = 98304,
    SCAN_LINE_LIMIT = 4096,
    SCAN_FUNCTION_CAPACITY = 160,
    SCAN_NAME_CAPACITY = 64
};

typedef struct {
    char name[SCAN_NAME_CAPACITY];
    int first_line;
    int last_line;
    int lines_of_code;
    int body_start;
    int body_end;
} ScannedFunction;

typedef struct {
    char raw[SCAN_TEXT_CAPACITY];
    char cleaned[SCAN_TEXT_CAPACITY];
    int length;
    int line_starts[SCAN_LINE_LIMIT];
    int line_count;
    int longest_line;
    int lines_of_code;
    int comment_line;
    ScannedFunction functions[SCAN_FUNCTION_CAPACITY];
    int function_count;
} ScannedSource;

bool source_scan(ScannedSource *source, const char *path);
bool source_find_functions(ScannedSource *source);
bool source_is_word_letter(char letter);
int source_word_end(const char *text, int at, int end);
bool source_line_is_blank(const ScannedSource *source, int line);
int source_line_length(const ScannedSource *source, int line);
bool source_span_is(const char *text, int start, int stop, const char *word);

#endif
