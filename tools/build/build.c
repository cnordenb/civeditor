#include <stdio.h>
#include <windows.h>

#include "../../src/core/check.h"
#include "build_steps.h"

static void show_last_failure(void)
{
    if (check_failure_count() > 0) {
        (void)puts(check_last_failure());
    }
}

static int report(bool built, bool tested)
{
    (void)puts(build_verdict(built, tested));
    show_last_failure();
    return (int)!tested;
}

int main(void)
{
    const char *command_line = GetCommandLineA();
    bool built = build_programs(build_mode_of(command_line));
    bool tested = built && build_run_tests(build_test_arguments(command_line));

    return report(built, tested);
}
