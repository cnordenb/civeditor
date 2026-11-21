#ifndef SHELL_H
#define SHELL_H

#include <stdbool.h>
#include <windows.h>
#include <commdlg.h>

enum { ASK_MESSAGE_CAPACITY = 16384, ASK_ROW_WIDTH = 64, ASK_ANSWER_ID = 100, ASK_LIST_ID = 101, ASK_FILTER_ID = 102 };

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

typedef enum {
    ASKED_GAME_FOLDER,
    ASKED_MODS_FOLDER
} AskedFolder;

void ask_script(const AskScript *answers);
const wchar_t *ask_last_message(void);
int ask_message(HWND owner, const wchar_t *text, const wchar_t *title, UINT flags);
bool ask_file(OPENFILENAMEW *dialog, bool saving);
bool ask_folder(AskedFolder asked, wchar_t *folder);
bool ask_text(HWND owner, const wchar_t *question, wchar_t *answer, size_t capacity);
bool text_dialog_show(HWND owner, AskQuestion *question);
bool list_scroll_takes(const MSG *message);
void list_scroll_open(HWND choice);

HACCEL menu_keys(void);
HMENU menu_make(void);

#endif
