#ifndef BUILD_STEPS_H
#define BUILD_STEPS_H

#include <stdbool.h>

typedef enum {
    BUILD_MODE_ANALYZED,
    BUILD_MODE_QUICK
} BuildMode;

bool build_prepare_folders(BuildMode mode);
bool build_compile_sources(BuildMode mode);
bool build_link_programs(BuildMode mode);
bool build_programs(BuildMode mode);
bool build_run_tests(const char *arguments);

#endif
