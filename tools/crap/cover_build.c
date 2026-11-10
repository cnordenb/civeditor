#include "cover_build.h"

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "../../src/core/check.h"
#include "../../src/core/text.h"
#include "../build/source_list.h"

enum { COMMAND_CAPACITY = 1024, FOLDER_LENGTH = 12 };

static const char FOLDER[] = "build/cover/";
static const char COMPILE[] = "gcc -std=c11 -DUNICODE -D_UNICODE -DCOBJMACROS -pipe -O0 -g --coverage -c ";
static const char LINK[] = "gcc --coverage -o build/cover/tests.exe @build/cover/tests.rsp";

static SourceList sources;
static SourceList headers;
static bool tests_passed;

static bool cover_path(char *destination, const char *source, const char *suffix)
{
    size_t length = text_length(source, SOURCE_PATH_CAPACITY);
    size_t start = 0U;
    size_t index;

    if (!CHECK(destination != NULL) || !CHECK(suffix != NULL) || !CHECK(length > 2U)) {
        return false;
    }
    for (index = 0U; (index < length) && (index < SOURCE_PATH_CAPACITY); ++index) {
        if (source[index] == '/') {
            start = index + 1U;
        }
    }
    return text_copy(destination, SOURCE_PATH_CAPACITY, FOLDER) &&
           text_copy_span(&destination[FOLDER_LENGTH], SOURCE_PATH_CAPACITY - FOLDER_LENGTH, &source[start],
                          length - start - 2U) &&
           text_append(destination, SOURCE_PATH_CAPACITY, suffix);
}

static bool run(const char *command)
{
    if (!CHECK(command != NULL) || !CHECK(command[0] != '\0')) {
        return false;
    }
    return system(command) == 0;
}

static bool compile_for_coverage(const SourceFile *source, long long newest_header)
{
    char object[SOURCE_PATH_CAPACITY];
    char command[COMMAND_CAPACITY];
    long long built;

    if (!CHECK(source != NULL) || !CHECK(newest_header >= 0LL) || !cover_path(object, source->path, ".o")) {
        return false;
    }
    built = source_file_modified(object);
    if ((built > source->modified) && (built > newest_header)) {
        return true;
    }
    return text_copy(command, sizeof command, COMPILE) && text_append(command, sizeof command, source->path) &&
           text_append(command, sizeof command, " -o ") && text_append(command, sizeof command, object) &&
           run(command);
}

static bool forget_counts(const SourceFile *source)
{
    char counts[SOURCE_PATH_CAPACITY];
    FILE *file;

    if (!CHECK(source != NULL) || !cover_path(counts, source->path, ".gcda")) {
        return false;
    }
    file = fopen(counts, "wb");
    return (file != NULL) && (fclose(file) == 0);
}

static bool prepare_one(const SourceFile *source, long long newest_header, FILE *response)
{
    char object[SOURCE_PATH_CAPACITY];

    if (!CHECK(source != NULL) || !CHECK(response != NULL)) {
        return false;
    }
    if (!source_in_tests(source->path)) {
        return true;
    }
    return compile_for_coverage(source, newest_header) && forget_counts(source) &&
           cover_path(object, source->path, ".o") && (fputs(object, response) >= 0) &&
           (fputs("\n", response) >= 0);
}

static bool prepare_all(void)
{
    long long newest_header = source_list_newest(&headers);
    FILE *response;
    bool fine = true;
    int index;

    if (!CHECK(sources.count > 0) || !CHECK(headers.count > 0)) {
        return false;
    }
    response = fopen("build/cover/tests.rsp", "w");
    if (response == NULL) {
        return false;
    }
    for (index = 0; (index < sources.count) && (index < SOURCE_LIST_CAPACITY) && fine; ++index) {
        fine = prepare_one(&sources.files[index], newest_header, response);
    }
    fine = fine && (fputs(source_test_libraries(), response) >= 0);
    return (fclose(response) == 0) && fine;
}

bool cover_build_run(const char *test_arguments)
{
    char tests[COMMAND_CAPACITY] = "build\\cover\\tests.exe";

    if (!CHECK(test_arguments != NULL) || !text_append(tests, sizeof tests, test_arguments) ||
        !text_append(tests, sizeof tests, " > build\\cover\\tests.txt")) {
        return false;
    }
    if (!source_list_collect_all(&sources, ".c") || !source_list_collect_all(&headers, ".h")) {
        return false;
    }
    (void)CreateDirectoryA("build", NULL);
    (void)CreateDirectoryA("build/cover", NULL);
    if (!CHECK(GetFileAttributesA("build/cover") != INVALID_FILE_ATTRIBUTES) || !prepare_all() || !run(LINK)) {
        return false;
    }
    tests_passed = run(tests);
    if (!tests_passed) {
        (void)fputs("A test failed in the coverage run; see build/cover/tests.txt.\n", stdout);
    }
    return true;
}

bool cover_build_tests_passed(void)
{
    if (!CHECK(sources.count >= 0) || !CHECK(headers.count >= 0)) {
        return false;
    }
    return tests_passed;
}
