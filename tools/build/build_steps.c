#include "build_steps.h"

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "../../src/core/check.h"
#include "../../src/core/text.h"
#include "source_list.h"

enum { COMMAND_CAPACITY = 4096 };

typedef enum {
    PROGRAM_APPLICATION,
    PROGRAM_TESTS,
    PROGRAM_BUILD_TOOL,
    PROGRAM_CRAPPER,
    PROGRAM_DRYER,
    PROGRAM_MUTATOR
} ProgramKind;

static const char WARNING_FLAGS[] =
    "-std=c11 -pedantic-errors -Wall -Wextra -Werror -Wconversion -Wsign-conversion -Wshadow -Wcast-qual "
    "-Wcast-align=strict -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations "
    "-Wold-style-definition -Wundef -Wwrite-strings -Wpointer-arith -Wswitch-enum -Wswitch-default "
    "-Wformat=2 -Wnull-dereference -Wdouble-promotion -Wvla -Wfloat-equal -Wredundant-decls "
    "-Wjump-misses-init -Wlogical-op -Wduplicated-cond -Wduplicated-branches -Winit-self -Wunused-macros "
    "-Wbad-function-cast -Wnested-externs -Walloca -Wstack-usage=65536 -Wframe-larger-than=65536 "
    "-DUNICODE -D_UNICODE -DCOBJMACROS -pipe -O1 -g";

static const char ANALYZER_FLAGS[] = " -fanalyzer";

static SourceList sources;
static SourceList headers;

static const char *object_folder(BuildMode mode)
{
    if (!CHECK((mode == BUILD_MODE_ANALYZED) || (mode == BUILD_MODE_QUICK))) {
        return "build/obj_quick";
    }
    return (mode == BUILD_MODE_ANALYZED) ? "build/obj_analyzed" : "build/obj_quick";
}

static bool object_path(char *destination, const char *source, BuildMode mode)
{
    size_t length = text_length(source, SOURCE_PATH_CAPACITY);
    size_t start = 0U;
    size_t index;

    if (!CHECK(destination != NULL) || !CHECK(length > 2U)) {
        return false;
    }
    for (index = 0U; (index < length) && (index < SOURCE_PATH_CAPACITY); ++index) {
        if (source[index] == '/') {
            start = index + 1U;
        }
    }
    return text_copy(destination, SOURCE_PATH_CAPACITY, object_folder(mode)) &&
           text_append(destination, SOURCE_PATH_CAPACITY, "/") &&
           text_copy_span(&destination[text_length(destination, SOURCE_PATH_CAPACITY)],
                          SOURCE_PATH_CAPACITY - text_length(destination, SOURCE_PATH_CAPACITY),
                          &source[start], length - start - 2U) &&
           text_append(destination, SOURCE_PATH_CAPACITY, ".o");
}

static bool run_command(const char *label, const char *command)
{
    int status;

    if (!CHECK(label != NULL) || !CHECK(command != NULL)) {
        return false;
    }
    if (fputs(label, stdout) < 0 || fputs("\n", stdout) < 0) {
        return false;
    }
    (void)fflush(stdout);
    status = system(command);
    return CHECK(status == 0);
}

static bool make_folder(const char *path)
{
    if (!CHECK(path != NULL) || !CHECK(path[0] != '\0')) {
        return false;
    }
    if (CreateDirectoryA(path, NULL) != 0) {
        return true;
    }
    return GetLastError() == ERROR_ALREADY_EXISTS;
}

bool build_prepare_folders(BuildMode mode)
{
    if (!CHECK((mode == BUILD_MODE_ANALYZED) || (mode == BUILD_MODE_QUICK)) ||
        !CHECK(GetFileAttributesA("src") != INVALID_FILE_ATTRIBUTES)) {
        return false;
    }
    return make_folder("build") && make_folder(object_folder(mode));
}

static bool compile_one(const SourceFile *source, BuildMode mode, long long newest_header)
{
    char object[SOURCE_PATH_CAPACITY];
    char command[COMMAND_CAPACITY];
    long long object_time;

    if (!CHECK(source != NULL) || !CHECK(newest_header >= 0LL) || !object_path(object, source->path, mode)) {
        return false;
    }
    object_time = source_file_modified(object);
    if ((object_time > source->modified) && (object_time > newest_header)) {
        return true;
    }
    command[0] = '\0';
    return text_append(command, COMMAND_CAPACITY, "gcc ") &&
           text_append(command, COMMAND_CAPACITY, WARNING_FLAGS) &&
           text_append(command, COMMAND_CAPACITY, (mode == BUILD_MODE_ANALYZED) ? ANALYZER_FLAGS : "") &&
           text_append(command, COMMAND_CAPACITY, " -c ") &&
           text_append(command, COMMAND_CAPACITY, source->path) &&
           text_append(command, COMMAND_CAPACITY, " -o ") &&
           text_append(command, COMMAND_CAPACITY, object) &&
           run_command(source->path, command);
}

bool build_compile_sources(BuildMode mode)
{
    long long newest_header;
    int index;

    if (!source_list_collect_all(&sources, ".c") || !source_list_collect_all(&headers, ".h")) {
        return false;
    }
    newest_header = source_list_newest(&headers);
    if (!CHECK(sources.count > 0) || !CHECK(headers.count > 0)) {
        return false;
    }
    for (index = 0; (index < sources.count) && (index < SOURCE_LIST_CAPACITY); ++index) {
        if (!compile_one(&sources.files[index], mode, newest_header)) {
            return false;
        }
    }
    return true;
}

static bool shared_by_tools(const char *path)
{
    if (!CHECK(path != NULL) || !CHECK(path[0] != '\0')) {
        return false;
    }
    return text_starts_with(path, "src/core/") || text_starts_with(path, "tests/scan/") ||
           text_equal(path, "tools/build/source_list.c");
}

static bool belongs_to(ProgramKind program, const char *path)
{
    if (!CHECK(path != NULL) || !CHECK(path[0] != '\0')) {
        return false;
    }
    switch (program) {
    case PROGRAM_APPLICATION:
        return text_starts_with(path, "src/");
    case PROGRAM_TESTS:
        return source_in_tests(path);
    case PROGRAM_BUILD_TOOL:
        return text_starts_with(path, "tools/build/") || text_starts_with(path, "src/core/");
    case PROGRAM_CRAPPER:
        return text_starts_with(path, "tools/crap/") || shared_by_tools(path);
    case PROGRAM_DRYER:
        return text_starts_with(path, "tools/dry/") || shared_by_tools(path);
    case PROGRAM_MUTATOR:
        return text_starts_with(path, "tools/mutate/") || text_equal(path, "tools/crap/crap_cover.c") ||
               text_equal(path, "tools/crap/cover_build.c") || shared_by_tools(path);
    default:
        return false;
    }
}

static bool write_object_list(FILE *list_file, ProgramKind program, BuildMode mode)
{
    char object[SOURCE_PATH_CAPACITY];
    int index;

    if (!CHECK(list_file != NULL) || !CHECK(sources.count <= SOURCE_LIST_CAPACITY)) {
        return false;
    }
    for (index = 0; (index < sources.count) && (index < SOURCE_LIST_CAPACITY); ++index) {
        if (!belongs_to(program, sources.files[index].path)) {
            continue;
        }
        if (!object_path(object, sources.files[index].path, mode) || (fputs(object, list_file) < 0) ||
            (fputs("\n", list_file) < 0)) {
            return false;
        }
    }
    return ((program != PROGRAM_TESTS) && (program != PROGRAM_APPLICATION)) ||
           (fputs(source_test_libraries(), list_file) >= 0);
}

static bool write_response_file(const char *path, ProgramKind program, BuildMode mode)
{
    FILE *list_file;
    bool written;

    if (!CHECK(path != NULL) || !CHECK(sources.count > 0)) {
        return false;
    }
    list_file = fopen(path, "w");
    if (list_file == NULL) {
        return false;
    }
    written = write_object_list(list_file, program, mode);
    return (fclose(list_file) == 0) && written;
}

static bool link_program(ProgramKind program, BuildMode mode, const char *response, const char *command)
{
    if (!CHECK(response != NULL) || !CHECK(command != NULL)) {
        return false;
    }
    return write_response_file(response, program, mode) && run_command(response, command);
}

bool build_link_programs(BuildMode mode)
{
    if (!CHECK(sources.count > 0) || !CHECK(mode == BUILD_MODE_ANALYZED || mode == BUILD_MODE_QUICK)) {
        return false;
    }
    return link_program(PROGRAM_APPLICATION, mode, "build/application.rsp",
                        "gcc -g -mwindows -municode -o build/civbuilder.exe @build/application.rsp") &&
           link_program(PROGRAM_TESTS, mode, "build/tests.rsp", "gcc -g -o build/tests.exe @build/tests.rsp") &&
           link_program(PROGRAM_BUILD_TOOL, mode, "build/build_tool.rsp",
                        "gcc -g -o build/build_check.exe @build/build_tool.rsp") &&
           link_program(PROGRAM_CRAPPER, mode, "build/crapper.rsp", "gcc -g -o build/crapper.exe @build/crapper.rsp") &&
           link_program(PROGRAM_DRYER, mode, "build/dryer.rsp", "gcc -g -o build/dryer.exe @build/dryer.rsp") &&
           link_program(PROGRAM_MUTATOR, mode, "build/mutator.rsp", "gcc -g -o build/mutator.exe @build/mutator.rsp");
}

bool build_run_tests(void)
{
    if (!CHECK(sources.count > 0)) {
        return false;
    }
    return run_command("build\\tests.exe", "build\\tests.exe") && CHECK(check_failure_count() == 0);
}
