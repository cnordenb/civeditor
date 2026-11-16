#ifndef SOURCE_LIST_H
#define SOURCE_LIST_H

#include <stdbool.h>
#include <stdio.h>

enum {
    SOURCE_PATH_CAPACITY = 260,
    SOURCE_LIST_CAPACITY = 320,
    SOURCE_FOLDER_CAPACITY = 96,
    SOURCE_PREFIX_WIDTH = 32,
    SOURCE_PREFIX_LIMIT = 8
};

typedef enum {
    SOURCE_PROGRAM_APPLICATION,
    SOURCE_PROGRAM_TESTS,
    SOURCE_PROGRAM_BUILD_TOOL,
    SOURCE_PROGRAM_COUNT
} SourceProgram;

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
bool source_in_program(SourceProgram program, const char *path);
bool source_built_path(char *destination, const char *folder, const char *source, const char *suffix);
bool source_is_fresh(const SourceFile *source, const char *built_path, long long newest_header);
bool source_write_objects(FILE *out, const SourceList *list, SourceProgram program, const char *folder);
long long source_list_newest(const SourceList *list);
long long source_file_modified(const char *path);

#endif
