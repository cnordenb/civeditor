#include "build_steps.h"

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "../../src/core/check.h"

static SourceList sources;
static SourceList headers;

static bool run_command(const char *label, const char *command)
{
    (void)puts(label);
    (void)fflush(stdout);
    return CHECK(system(command) == 0);
}

static bool make_folder(const char *path)
{
    return (CreateDirectoryA(path, NULL) != 0) || (GetLastError() == ERROR_ALREADY_EXISTS);
}

static bool make_folders(BuildMode mode)
{
    int failed = 0;
    int index;

    for (index = 0; index < build_folder_count(mode); ++index) {
        failed += (int)!make_folder(build_folder(mode, index));
    }
    return failed == 0;
}

bool build_prepare_folders(BuildMode mode)
{
    return (GetFileAttributesA("src") != INVALID_FILE_ATTRIBUTES) && make_folders(mode);
}

static bool compile_if_stale(const SourceFile *source, const char *object, const char *command, long long newest_header)
{
    return source_is_fresh(source, object, newest_header) || run_command(source->path, command);
}

static bool compile_one(const SourceFile *source, BuildMode mode, long long newest_header)
{
    char object[SOURCE_PATH_CAPACITY];
    char command[BUILD_COMMAND_CAPACITY];

    return CHECK(build_compile_command(command, object, source->path, mode)) &&
           compile_if_stale(source, object, command, newest_header);
}

static bool compile_all(BuildMode mode)
{
    long long newest_header = source_list_newest(&headers);
    int failed = 0;
    int index;

    for (index = 0; index < sources.count; ++index) {
        failed += (int)!compile_one(&sources.files[index], mode, newest_header);
    }
    return failed == 0;
}

static bool collect_sources(void)
{
    return CHECK(source_list_collect_all(&sources, ".c")) && CHECK(source_list_collect_all(&headers, ".h"));
}

bool build_compile_sources(BuildMode mode)
{
    return collect_sources() && compile_all(mode);
}

static bool write_objects_to(FILE *list_file, SourceProgram program, BuildMode mode)
{
    bool written = source_write_objects(list_file, &sources, program, build_objects_folder(mode));

    return (fclose(list_file) == 0) && written;
}

static bool write_response_file(SourceProgram program, BuildMode mode)
{
    FILE *list_file = fopen(build_response_file(program), "w");

    return (list_file != NULL) && write_objects_to(list_file, program, mode);
}

static bool link_program(SourceProgram program, BuildMode mode)
{
    return write_response_file(program, mode) &&
           run_command(build_response_file(program), build_link_command(program, mode));
}

bool build_link_programs(BuildMode mode)
{
    BuildPrograms programs = build_programs_of(mode);
    int failed = 0;
    int program;

    for (program = (int)programs.first; program < (int)programs.end; ++program) {
        failed += (int)!link_program((SourceProgram)program, mode);
    }
    return failed == 0;
}

static bool compile_and_link(BuildMode mode)
{
    return build_compile_sources(mode) && build_link_programs(mode);
}

bool build_programs(BuildMode mode)
{
    return build_prepare_folders(mode) && compile_and_link(mode);
}

static bool tests_pass(const char *command)
{
    return run_command(command, command) && CHECK(check_failure_count() == 0);
}

bool build_run_tests(const char *arguments)
{
    char command[BUILD_COMMAND_CAPACITY];

    return CHECK(build_test_command(command, arguments)) && tests_pass(command);
}
