#include "win_parts.h"

#include "../core/check.h"

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE previous, PWSTR command_line, int show)
{
    if (!CHECK(instance != NULL) || !CHECK(previous == NULL)) {
        return 1;
    }
    (void)command_line;
    (void)SetProcessDPIAware();
    return app_run(instance, show);
}
