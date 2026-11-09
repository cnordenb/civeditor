#ifndef FILE_IO_H
#define FILE_IO_H

#include <stdbool.h>
#include <stddef.h>
#include <wchar.h>

enum { FILE_PATH_CAPACITY = 520 };

typedef struct {
    bool succeeded;
    size_t length;
} FileRead;

FileRead file_read_all(const wchar_t *path, char *buffer, size_t capacity);
bool file_write_all(const wchar_t *path, const char *data, size_t length);
bool file_exists(const wchar_t *path);
bool file_join_path(wchar_t *destination, size_t capacity, const wchar_t *folder, const wchar_t *name);

#endif
