#ifndef SOURCE_LIST_H
#define SOURCE_LIST_H

#include <stdbool.h>

enum {
    SOURCE_PATH_CAPACITY = 260,
    SOURCE_LIST_CAPACITY = 320,
    SOURCE_FOLDER_CAPACITY = 96
};

typedef struct {
    char path[SOURCE_PATH_CAPACITY];
    long long modified;
} SourceFile;

typedef struct {
    SourceFile files[SOURCE_LIST_CAPACITY];
    int count;
} SourceList;

bool source_list_collect(SourceList *list, const char *root, const char *extension);
bool source_list_collect_all(SourceList *list, const char *extension);
bool source_in_tests(const char *path);
const char *source_test_libraries(void);
long long source_list_newest(const SourceList *list);
long long source_file_modified(const char *path);

#endif
