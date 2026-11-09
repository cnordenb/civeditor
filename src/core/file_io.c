#include "file_io.h"

#include <stdio.h>

#include "check.h"

FileRead file_read_all(const wchar_t *path, char *buffer, size_t capacity)
{
    FileRead result = { false, 0U };
    FILE *stream;

    if (!CHECK(path != NULL) || !CHECK(buffer != NULL) || !CHECK(capacity > 1U)) {
        return result;
    }
    stream = _wfopen(path, L"rb");
    if (stream == NULL) {
        return result;
    }
    result.length = fread(buffer, 1U, capacity - 1U, stream);
    result.succeeded = (ferror(stream) == 0) && (feof(stream) != 0);
    buffer[result.length] = '\0';
    if (fclose(stream) != 0) {
        result.succeeded = false;
    }
    return result;
}

bool file_write_all(const wchar_t *path, const char *data, size_t length)
{
    FILE *stream;
    size_t written;

    if (!CHECK(path != NULL) || !CHECK(data != NULL)) {
        return false;
    }
    stream = _wfopen(path, L"wb");
    if (stream == NULL) {
        return false;
    }
    written = fwrite(data, 1U, length, stream);
    return (fclose(stream) == 0) && (written == length);
}

static size_t append_wide(wchar_t *destination, size_t capacity, size_t used, const wchar_t *text)
{
    size_t index;

    if (!CHECK(destination != NULL) || !CHECK(text != NULL)) {
        return capacity;
    }
    for (index = 0U; (index < (size_t)FILE_PATH_CAPACITY) && (text[index] != L'\0'); ++index) {
        if (used + 1U >= capacity) {
            return capacity;
        }
        destination[used] = text[index];
        ++used;
    }
    destination[used] = L'\0';
    return used;
}

bool file_join_path(wchar_t *destination, size_t capacity, const wchar_t *folder, const wchar_t *name)
{
    size_t used;

    if (!CHECK(destination != NULL) || !CHECK(capacity > 1U)) {
        return false;
    }
    destination[0] = L'\0';
    used = append_wide(destination, capacity, 0U, folder);
    used = (used < capacity) ? append_wide(destination, capacity, used, L"\\") : used;
    used = (used < capacity) ? append_wide(destination, capacity, used, name) : used;
    return used < capacity;
}

bool file_exists(const wchar_t *path)
{
    FILE *stream;

    if (!CHECK(path != NULL) || !CHECK(path[0] != L'\0')) {
        return false;
    }
    stream = _wfopen(path, L"rb");
    if (stream == NULL) {
        return false;
    }
    return fclose(stream) == 0;
}
