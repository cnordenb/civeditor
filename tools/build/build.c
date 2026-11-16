#include <stdio.h>
#include <windows.h>

#include "../../src/core/check.h"
#include "../../src/core/text.h"
#include "build_steps.h"

enum { VERDICT_COUNT = 3, VERDICT_WIDTH = 32, ARGUMENT_SETS = 4, ARGUMENT_WIDTH = 24 };

static BuildMode requested_mode(void)
{
    const char *command_line = GetCommandLineA();

    if (!CHECK(command_line != NULL) || !CHECK(command_line[0] != '\0')) {
        return BUILD_MODE_ANALYZED;
    }
    return text_contains_folded(command_line, " quick") ? BUILD_MODE_QUICK : BUILD_MODE_ANALYZED;
}

static const char *test_arguments(void)
{
    static const char ARGUMENTS[ARGUMENT_SETS][ARGUMENT_WIDTH] = { "", " nowindow", " norules", " nowindow norules" };
    const char *command_line = GetCommandLineA();
    int set;

    if (!CHECK(command_line != NULL) || !CHECK(command_line[0] != '\0')) {
        return ARGUMENTS[0];
    }
    set = (text_contains_folded(command_line, " nowindow") ? 1 : 0) +
          (text_contains_folded(command_line, " norules") ? 2 : 0);
    return ARGUMENTS[set];
}

static int report(bool built, bool tested)
{
    static const char VERDICTS[VERDICT_COUNT][VERDICT_WIDTH] = { "Build failed.\n", "Tests failed.\n",
                                                                 "Build and tests passed.\n" };

    if (!CHECK(tested <= built) || !CHECK(fputs(VERDICTS[(int)built + (int)tested], stdout) >= 0)) {
        return 1;
    }
    if (check_failure_count() > 0) {
        (void)fputs(check_last_failure(), stdout);
        (void)fputs("\n", stdout);
    }
    return (int)!tested;
}

int main(void)
{
    BuildMode mode = requested_mode();
    bool built = build_programs(mode);
    bool tested = built && build_run_tests(test_arguments());

    return report(built, tested);
}
