#include <stdio.h>
#include <windows.h>

#include "../../src/core/check.h"
#include "../../src/core/text.h"
#include "build_steps.h"

static BuildMode requested_mode(void)
{
    const char *command_line = GetCommandLineA();

    if (!CHECK(command_line != NULL) || !CHECK(command_line[0] != '\0')) {
        return BUILD_MODE_ANALYZED;
    }
    return text_contains_folded(command_line, " quick") ? BUILD_MODE_QUICK : BUILD_MODE_ANALYZED;
}

static int report(bool built, bool tested)
{
    const char *verdict = "Build and tests passed.\n";

    if (!built) {
        verdict = "Build failed.\n";
    } else if (!tested) {
        verdict = "Tests failed.\n";
    }
    if (!CHECK(verdict != NULL) || (fputs(verdict, stdout) < 0)) {
        return 1;
    }
    if ((check_failure_count() > 0) && CHECK(check_last_failure() != NULL)) {
        (void)fputs(check_last_failure(), stdout);
        (void)fputs("\n", stdout);
    }
    return (built && tested) ? 0 : 1;
}

int main(void)
{
    BuildMode mode = requested_mode();
    bool built = build_prepare_folders(mode) && build_compile_sources(mode) && build_link_programs(mode);
    bool tested = built && build_run_tests();

    return report(built, tested);
}
