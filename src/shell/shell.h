#ifndef SHELL_H
#define SHELL_H

#include <stdbool.h>
#include <windows.h>
#include <commdlg.h>

enum { ASK_MESSAGE_CAPACITY = 16384 };

typedef struct {
    bool active;
    int answer;
    const wchar_t *save_folder;
    const wchar_t *open_path;
    const wchar_t *steam;
    const wchar_t *home;
} AskScript;

void ask_script(const AskScript *answers);
const wchar_t *ask_last_message(void);
int ask_message(HWND owner, const wchar_t *text, const wchar_t *title, UINT flags);
bool ask_file(OPENFILENAMEW *dialog, bool saving);
bool ask_game_folders(wchar_t *game, wchar_t *mods);

HACCEL menu_keys(void);
HMENU menu_make(void);

#endif
