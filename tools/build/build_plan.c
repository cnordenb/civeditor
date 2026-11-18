#include "build_plan.h"

#include "../../src/core/text.h"

enum {
    RESPONSE_WIDTH = 32,
    LINK_WIDTH = 96,
    FOLDER_WIDTH = 24,
    FLAG_WIDTH = 24,
    VERDICT_COUNT = 3,
    VERDICT_WIDTH = 32,
    ARGUMENT_COUNT = 3,
    ARGUMENT_WIDTH = 16,
    MODE_FOLDERS = 2
};

static const char RESPONSES[SOURCE_PROGRAM_COUNT][RESPONSE_WIDTH] = { "build/application.rsp", "build/tests.rsp",
                                                                     "build/build_tool.rsp" };
static const char LINKS[SOURCE_PROGRAM_COUNT][LINK_WIDTH] = {
    "gcc -g -mwindows -municode -o build/civbuilder.exe @build/application.rsp",
    "gcc -g -o build/tests.exe @build/tests.rsp",
    "gcc -g -o build/build_check.exe @build/build_tool.rsp"
};
static const char COVERAGE_LINK[] = "gcc -g --coverage -o build/tests.exe @build/tests.rsp";
static const char WARNING_FLAGS[] =
    "-std=c11 -pedantic-errors -Wall -Wextra -Werror -Wconversion -Wsign-conversion -Wshadow -Wcast-qual "
    "-Wcast-align=strict -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations "
    "-Wold-style-definition -Wundef -Wwrite-strings -Wpointer-arith -Wswitch-enum -Wswitch-default "
    "-Wformat=2 -Wnull-dereference -Wdouble-promotion -Wvla -Wfloat-equal -Wredundant-decls "
    "-Wjump-misses-init -Wlogical-op -Wduplicated-cond -Wduplicated-branches -Winit-self -Wunused-macros "
    "-Wbad-function-cast -Wnested-externs -Walloca -Wstack-usage=65536 -Wframe-larger-than=65536 "
    "-DUNICODE -D_UNICODE -DCOBJMACROS -pipe -O1 -g";
static const char MODE_FLAGS[BUILD_MODE_COUNT][FLAG_WIDTH] = { " -fanalyzer", "", " --coverage -O0" };
static const char FOLDERS[BUILD_MODE_COUNT][BUILD_FOLDER_LIMIT][FOLDER_WIDTH] = {
    { "build", "build/obj_analyzed" },
    { "build", "build/obj_quick" },
    { "build", "build/obj_coverage", "target", "target/coverage", "target/coverage/c" }
};

BuildMode build_mode_of(const char *command_line)
{
    if (text_contains_folded(command_line, " coverage")) {
        return BUILD_MODE_COVERAGE;
    }
    return text_contains_folded(command_line, " quick") ? BUILD_MODE_QUICK : BUILD_MODE_ANALYZED;
}

const char *build_test_arguments(const char *command_line)
{
    static const char WORDS[ARGUMENT_COUNT][ARGUMENT_WIDTH] = { " nowindow", " norules", " redundancy" };
    static char arguments[ARGUMENT_COUNT * ARGUMENT_WIDTH];
    int word;

    (void)text_copy(arguments, sizeof arguments, "");
    for (word = 0; word < ARGUMENT_COUNT; ++word) {
        if (text_contains_folded(command_line, WORDS[word])) {
            (void)text_append(arguments, sizeof arguments, WORDS[word]);
        }
    }
    return arguments;
}

const char *build_verdict(bool built, bool tested)
{
    static const char VERDICTS[VERDICT_COUNT][VERDICT_WIDTH] = { "Build failed.", "Tests failed.",
                                                                 "Build and tests passed." };

    return VERDICTS[(int)built + (int)tested];
}

int build_folder_count(BuildMode mode)
{
    return (mode == BUILD_MODE_COVERAGE) ? BUILD_FOLDER_LIMIT : MODE_FOLDERS;
}

const char *build_folder(BuildMode mode, int index)
{
    return FOLDERS[mode][index];
}

const char *build_objects_folder(BuildMode mode)
{
    return FOLDERS[mode][1];
}

bool build_compile_command(char command[BUILD_COMMAND_CAPACITY], char object[SOURCE_PATH_CAPACITY], const char *source,
                           BuildMode mode)
{
    return source_built_path(object, build_objects_folder(mode), source, ".o") &&
           text_copy(command, BUILD_COMMAND_CAPACITY, "gcc ") &&
           text_append_three(command, BUILD_COMMAND_CAPACITY, WARNING_FLAGS, MODE_FLAGS[mode], " -c ") &&
           text_append_three(command, BUILD_COMMAND_CAPACITY, source, " -o ", object);
}

BuildPrograms build_programs_of(BuildMode mode)
{
    BuildPrograms every = { SOURCE_PROGRAM_APPLICATION, SOURCE_PROGRAM_COUNT };
    BuildPrograms tests = { SOURCE_PROGRAM_TESTS, SOURCE_PROGRAM_BUILD_TOOL };

    return (mode == BUILD_MODE_COVERAGE) ? tests : every;
}

const char *build_response_file(SourceProgram program)
{
    return RESPONSES[program];
}

const char *build_link_command(SourceProgram program, BuildMode mode)
{
    return (mode == BUILD_MODE_COVERAGE) ? COVERAGE_LINK : LINKS[program];
}

bool build_test_command(char command[BUILD_COMMAND_CAPACITY], const char *arguments)
{
    return text_copy(command, BUILD_COMMAND_CAPACITY, "build\\tests.exe") &&
           text_append(command, BUILD_COMMAND_CAPACITY, arguments);
}
