#include "build_steps.h"

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "../../src/core/check.h"
#include "../../src/core/text.h"
#include "source_list.h"

enum { COMMAND_CAPACITY = 4096, RESPONSE_WIDTH = 32, LINK_WIDTH = 96 };

static const char RESPONSES[SOURCE_PROGRAM_COUNT][RESPONSE_WIDTH] = { "build/application.rsp", "build/tests.rsp",
                                                                     "build/build_tool.rsp" };
static const char LINKS[SOURCE_PROGRAM_COUNT][LINK_WIDTH] = {
    "gcc -g -mwindows -municode -o build/civbuilder.exe @build/application.rsp",
    "gcc -g -o build/tests.exe @build/tests.rsp",
    "gcc -g -o build/build_check.exe @build/build_tool.rsp"
};
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
    if (!CHECK(mode <= BUILD_MODE_QUICK) || !CHECK(COMMAND_CAPACITY > 0)) {
        return "build/obj_quick";
    }
    return (mode == BUILD_MODE_ANALYZED) ? "build/obj_analyzed" : "build/obj_quick";
}

static const char *analyzer_flags(BuildMode mode)
{
    if (!CHECK(mode <= BUILD_MODE_QUICK) || !CHECK(sizeof ANALYZER_FLAGS > 1U)) {
        return "";
    }
    return (mode == BUILD_MODE_ANALYZED) ? ANALYZER_FLAGS : "";
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
    if (!CHECK(mode <= BUILD_MODE_QUICK) || !CHECK(GetFileAttributesA("src") != INVALID_FILE_ATTRIBUTES)) {
        return false;
    }
    return make_folder("build") && make_folder(object_folder(mode));
}

static bool compile_one(const SourceFile *source, BuildMode mode, long long newest_header)
{
    char object[SOURCE_PATH_CAPACITY];
    char command[COMMAND_CAPACITY] = "gcc ";

    if (!CHECK(source != NULL) || !CHECK(source_built_path(object, object_folder(mode), source->path, ".o")) ||
        !CHECK(text_append_three(command, COMMAND_CAPACITY, WARNING_FLAGS, analyzer_flags(mode), " -c ")) ||
        !CHECK(text_append_three(command, COMMAND_CAPACITY, source->path, " -o ", object))) {
        return false;
    }
    return source_is_fresh(source, object, newest_header) || run_command(source->path, command);
}

bool build_compile_sources(BuildMode mode)
{
    long long newest_header;
    int index;

    if (!CHECK(source_list_collect_all(&sources, ".c")) || !CHECK(source_list_collect_all(&headers, ".h")) ||
        !CHECK(sources.count > 0) || !CHECK(headers.count > 0)) {
        return false;
    }
    newest_header = source_list_newest(&headers);
    for (index = 0; (index < sources.count) && (index < SOURCE_LIST_CAPACITY); ++index) {
        if (!compile_one(&sources.files[index], mode, newest_header)) {
            return false;
        }
    }
    return true;
}

static bool write_response_file(const char *path, SourceProgram program, BuildMode mode)
{
    FILE *list_file;
    bool written;

    if (!CHECK(path != NULL) || !CHECK(sources.count > 0) || !CHECK(mode <= BUILD_MODE_QUICK)) {
        return false;
    }
    list_file = fopen(path, "w");
    if (list_file == NULL) {
        return false;
    }
    written = source_write_objects(list_file, &sources, program, object_folder(mode));
    return (fclose(list_file) == 0) && written;
}

static bool link_program(SourceProgram program, BuildMode mode)
{
    if (!CHECK(program < SOURCE_PROGRAM_COUNT) || !CHECK(mode <= BUILD_MODE_QUICK)) {
        return false;
    }
    return write_response_file(RESPONSES[program], program, mode) && run_command(RESPONSES[program], LINKS[program]);
}

bool build_link_programs(BuildMode mode)
{
    int program;

    if (!CHECK(sources.count > 0) || !CHECK(mode <= BUILD_MODE_QUICK)) {
        return false;
    }
    for (program = 0; program < SOURCE_PROGRAM_COUNT; ++program) {
        if (!link_program((SourceProgram)program, mode)) {
            return false;
        }
    }
    return true;
}

bool build_programs(BuildMode mode)
{
    if (!CHECK(mode <= BUILD_MODE_QUICK) || !CHECK(COMMAND_CAPACITY > 0)) {
        return false;
    }
    return build_prepare_folders(mode) && build_compile_sources(mode) && build_link_programs(mode);
}

bool build_run_tests(const char *arguments)
{
    char command[COMMAND_CAPACITY] = "build\\tests.exe";

    if (!CHECK(sources.count > 0) || !CHECK(arguments != NULL) ||
        !CHECK(text_append(command, COMMAND_CAPACITY, arguments))) {
        return false;
    }
    return run_command(command, command) && CHECK(check_failure_count() == 0);
}
