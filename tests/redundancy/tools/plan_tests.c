#include "plan_tests.h"

#include <string.h>

#include "../../../tools/build/build_plan.h"
#include "../../../tools/build/source_list.h"

bool test_the_analyzed_build_compiles_with_the_analyzer(void)
{
    static char command[BUILD_COMMAND_CAPACITY];
    static char object[SOURCE_PATH_CAPACITY];

    return build_compile_command(command, object, "src/core/check.c", BUILD_MODE_ANALYZED) &&
           (strstr(command, " -fanalyzer -c src/core/check.c -o ") != NULL);
}
