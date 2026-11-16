#include "repo_files.h"

#include <stdio.h>

#include "../../src/core/check.h"
#include "../../src/core/file_io.h"
#include "../../src/core/text.h"

static const char LIST_COMMAND[] = "git ls-files --cached --others --exclude-standard";

static bool keep_line(RepoFiles *files, const char *line)
{
    size_t length = text_length(line, REPO_PATH_CAPACITY);
    wchar_t wide[REPO_PATH_CAPACITY];
    size_t index;

    if (!CHECK(files != NULL) || !CHECK(files->count < REPO_FILE_CAPACITY) || (length == 0U)) {
        return length == 0U;
    }
    for (index = 0U; (index <= length) && (index < REPO_PATH_CAPACITY); ++index) {
        wide[index] = (wchar_t)(unsigned char)line[index];
    }
    if (!file_exists(wide)) {
        return true;
    }
    ++files->count;
    return text_copy(files->paths[files->count - 1], REPO_PATH_CAPACITY, line);
}

static void trim_line_end(char *line)
{
    size_t length = text_length(line, REPO_PATH_CAPACITY);

    if (!CHECK(line != NULL) || !CHECK(length < REPO_PATH_CAPACITY)) {
        return;
    }
    if ((length > 0U) && (line[length - 1U] == '\n')) {
        line[length - 1U] = '\0';
        --length;
    }
    if ((length > 0U) && (line[length - 1U] == '\r')) {
        line[length - 1U] = '\0';
    }
}

static bool keep_lines(RepoFiles *files, FILE *listing)
{
    char line[REPO_PATH_CAPACITY];
    bool fine = true;
    int step;

    if (!CHECK(files != NULL) || !CHECK(listing != NULL)) {
        return false;
    }
    for (step = 0; (step < REPO_FILE_CAPACITY) && fine; ++step) {
        if (fgets(line, (int)sizeof line, listing) == NULL) {
            break;
        }
        trim_line_end(line);
        fine = keep_line(files, line);
    }
    return fine;
}

bool repo_files_list(RepoFiles *files)
{
    FILE *listing;
    bool fine;

    if (!CHECK(files != NULL) || !CHECK(LIST_COMMAND[0] != '\0')) {
        return false;
    }
    files->count = 0;
    listing = _popen(LIST_COMMAND, "r");
    if (listing == NULL) {
        return false;
    }
    fine = keep_lines(files, listing);
    return (_pclose(listing) == 0) && fine && (files->count > 0);
}

bool repo_path_has_suffix(const char *path, const char *suffix)
{
    size_t path_length = text_length(path, REPO_PATH_CAPACITY);
    size_t suffix_length = text_length(suffix, REPO_PATH_CAPACITY);

    if (!CHECK(path != NULL) || !CHECK(suffix != NULL) || (suffix_length > path_length)) {
        return false;
    }
    return text_equal(&path[path_length - suffix_length], suffix);
}

bool repo_path_is_code(const char *path)
{
    if (!CHECK(path != NULL) || !CHECK(path[0] != '\0')) {
        return false;
    }
    return repo_path_has_suffix(path, ".c") || repo_path_has_suffix(path, ".h");
}

int repo_path_name_start(const char *path)
{
    size_t length = text_length(path, REPO_PATH_CAPACITY);
    size_t index;
    int start = 0;

    if (!CHECK(path != NULL) || !CHECK(length < REPO_PATH_CAPACITY)) {
        return 0;
    }
    for (index = 0U; (index < length) && (index < REPO_PATH_CAPACITY); ++index) {
        if (path[index] == '/') {
            start = (int)index + 1;
        }
    }
    return start;
}
