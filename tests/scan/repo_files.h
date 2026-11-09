#ifndef REPO_FILES_H
#define REPO_FILES_H

#include <stdbool.h>

enum { REPO_PATH_CAPACITY = 260, REPO_FILE_CAPACITY = 640 };

typedef struct {
    char paths[REPO_FILE_CAPACITY][REPO_PATH_CAPACITY];
    int count;
} RepoFiles;

bool repo_files_list(RepoFiles *files);
bool repo_path_is_code(const char *path);
bool repo_path_has_suffix(const char *path, const char *suffix);
int repo_path_name_start(const char *path);

#endif
