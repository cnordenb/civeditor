#ifndef SHELL_H
#define SHELL_H

#include <stdbool.h>
#include <windows.h>
#include <commdlg.h>

enum { ASK_MESSAGE_CAPACITY = 16384, ASK_ROW_WIDTH = 64, ASK_ANSWER_ID = 100, ASK_LIST_ID = 101 };

typedef struct {
    bool active;
    int answer;
    const wchar_t *save_folder;
    const wchar_t *open_path;
    const wchar_t *steam;
    const wchar_t *home;
    const wchar_t *text;
} AskScript;

typedef struct {
    const wchar_t *question;
    wchar_t *answer;
    size_t capacity;
    const wchar_t *list_label;
    wchar_t (*rows)[ASK_ROW_WIDTH];
    int row_count;
    int row;
} AskQuestion;

void ask_script(const AskScript *answers);
const wchar_t *ask_last_message(void);
int ask_message(HWND owner, const wchar_t *text, const wchar_t *title, UINT flags);
bool ask_file(OPENFILENAMEW *dialog, bool saving);
bool ask_game_folders(wchar_t *game, wchar_t *mods);
bool ask_game_folder(wchar_t *game);
bool ask_text(HWND owner, const wchar_t *question, wchar_t *answer, size_t capacity);
bool text_dialog_show(HWND owner, AskQuestion *question);

HACCEL menu_keys(void);
HMENU menu_make(void);

#endif
