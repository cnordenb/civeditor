#ifndef BUILD_PLAN_H
#define BUILD_PLAN_H

#include <stdbool.h>

#include "source_list.h"

enum { BUILD_COMMAND_CAPACITY = 4096, BUILD_FOLDER_LIMIT = 5 };

typedef enum {
    BUILD_MODE_ANALYZED,
    BUILD_MODE_QUICK,
    BUILD_MODE_COVERAGE,
    BUILD_MODE_COUNT
} BuildMode;

typedef struct {
    SourceProgram first;
    SourceProgram end;
} BuildPrograms;

BuildMode build_mode_of(const char *command_line);
const char *build_test_arguments(const char *command_line);
const char *build_verdict(bool built, bool tested);
int build_folder_count(BuildMode mode);
const char *build_folder(BuildMode mode, int index);
const char *build_objects_folder(BuildMode mode);
bool build_compile_command(char command[BUILD_COMMAND_CAPACITY], char object[SOURCE_PATH_CAPACITY], const char *source,
                           BuildMode mode);
BuildPrograms build_programs_of(BuildMode mode);
const char *build_response_file(SourceProgram program);
const char *build_link_command(SourceProgram program, BuildMode mode);
bool build_test_command(char command[BUILD_COMMAND_CAPACITY], const char *arguments);

#endif
