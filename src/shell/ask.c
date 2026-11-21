#include "shell.h"

#include <wchar.h>

#include "../core/check.h"
#include "../mod/mod_places.h"

static AskScript script;
static wchar_t last_message[ASK_MESSAGE_CAPACITY];

void ask_script(const AskScript *answers)
{
    static const AskScript NONE = { false, 0, NULL, NULL, NULL, NULL, NULL };

    script = (answers == NULL) ? NONE : *answers;
    last_message[0] = L'\0';
    (void)CHECK(script.answer >= 0);
}

const wchar_t *ask_last_message(void)
{
    (void)CHECK(last_message[ASK_MESSAGE_CAPACITY - 1] == L'\0');
    return last_message;
}

int ask_message(HWND owner, const wchar_t *text, const wchar_t *title, UINT flags)
{
    if (!script.active) {
        return MessageBoxW(owner, text, title, flags);
    }
    (void)wcsncpy(last_message, text, ASK_MESSAGE_CAPACITY - 1);
    return script.answer;
}

bool ask_text(HWND owner, const wchar_t *question, wchar_t *answer, size_t capacity)
{
    AskQuestion asked = { question, answer, capacity, NULL, NULL, 0, -1 };

    if (!script.active) {
        return text_dialog_show(owner, &asked);
    }
    if (script.text != NULL) {
        (void)lstrcpynW(answer, script.text, (int)capacity);
    }
    return (script.answer == IDOK) && (answer[0] != L'\0');
}

static bool scripted_file(OPENFILENAMEW *dialog, bool saving)
{
    wchar_t path[FILE_PATH_CAPACITY];
    const wchar_t *given = saving ? script.save_folder : script.open_path;

    if (!CHECK(dialog->nMaxFile >= (DWORD)FILE_PATH_CAPACITY) || (given == NULL)) {
        return false;
    }
    if (!saving) {
        (void)wcsncpy(dialog->lpstrFile, given, FILE_PATH_CAPACITY - 1);
        return true;
    }
    if (!file_join_path(path, FILE_PATH_CAPACITY, given, dialog->lpstrFile)) {
        return false;
    }
    (void)wcsncpy(dialog->lpstrFile, path, FILE_PATH_CAPACITY - 1);
    return true;
}

static bool shown_file(OPENFILENAMEW *dialog, bool saving)
{
    return (saving ? GetSaveFileNameW(dialog) : GetOpenFileNameW(dialog)) != 0;
}

bool ask_file(OPENFILENAMEW *dialog, bool saving)
{
    return script.active ? scripted_file(dialog, saving) : shown_file(dialog, saving);
}

static bool scripted_folder(AskedFolder asked, wchar_t *folder)
{
    if (asked == ASKED_GAME_FOLDER) {
        return (script.steam != NULL) && mod_find_game_under(script.steam, folder, FILE_PATH_CAPACITY);
    }
    return (script.home != NULL) && mod_find_mods_under(script.home, folder, FILE_PATH_CAPACITY);
}

static bool found_folder(AskedFolder asked, wchar_t *folder)
{
    return (asked == ASKED_GAME_FOLDER) ? mod_find_game_folder(folder, FILE_PATH_CAPACITY)
                                        : mod_find_local_mods(folder, FILE_PATH_CAPACITY);
}

bool ask_folder(AskedFolder asked, wchar_t *folder)
{
    return script.active ? scripted_folder(asked, folder) : found_folder(asked, folder);
}
