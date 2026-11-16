#include "../accessibility/window_fixture.h"

#include "../../src/core/check.h"
#include "../../src/win/win_parts.h"

enum { MESSAGE_DRAIN_LIMIT = 64 };

void window_fixture_close(HWND window)
{
    MSG message;
    int step;

    if (!CHECK(MESSAGE_DRAIN_LIMIT > 0) || (window == NULL)) {
        return;
    }
    (void)DestroyWindow(window);
    for (step = 0; step < MESSAGE_DRAIN_LIMIT; ++step) {
        if (PeekMessageW(&message, NULL, 0, 0, PM_REMOVE) == 0) {
            break;
        }
    }
}

void window_fixture_act(HWND window, int action)
{
    if (!CHECK(window != NULL) || !CHECK(action < ACTION_COUNT)) {
        return;
    }
    (void)SendMessageW(window, WM_COMMAND, (WPARAM)(ACTION_ID_BASE + action), 0);
}
