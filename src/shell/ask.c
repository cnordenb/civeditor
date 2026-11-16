#include "shell.h"

#include <wchar.h>

#include "../core/check.h"
#include "../mod/mod_places.h"

static AskScript script;
static wchar_t last_message[ASK_MESSAGE_CAPACITY];

void ask_script(const AskScript *answers)
{
    static const AskScript NONE = { false, 0, NULL, NULL, NULL, NULL };

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
    if (!CHECK(text != NULL) || !CHECK(title != NULL)) {
        return 0;
    }
    if (!script.active) {
        return MessageBoxW(owner, text, title, flags);
    }
    (void)wcsncpy(last_message, text, ASK_MESSAGE_CAPACITY - 1);
    return script.answer;
}

static bool scripted_file(OPENFILENAMEW *dialog, bool saving)
{
    wchar_t path[FILE_PATH_CAPACITY];
    const wchar_t *given = saving ? script.save_folder : script.open_path;

    if (!CHECK(dialog != NULL) || !CHECK(dialog->nMaxFile >= (DWORD)FILE_PATH_CAPACITY) || (given == NULL)) {
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
    return CHECK(dialog != NULL) && ((saving ? GetSaveFileNameW(dialog) : GetOpenFileNameW(dialog)) != 0);
}

bool ask_file(OPENFILENAMEW *dialog, bool saving)
{
    if (!CHECK(dialog != NULL) || !CHECK(dialog->lpstrFile != NULL)) {
        return false;
    }
    return script.active ? scripted_file(dialog, saving) : shown_file(dialog, saving);
}

static bool scripted_folders(wchar_t *game, wchar_t *mods)
{
    if (!CHECK(game != NULL) || !CHECK(mods != NULL) || (script.steam == NULL) || (script.home == NULL)) {
        return false;
    }
    return mod_find_game_under(script.steam, game, FILE_PATH_CAPACITY) &&
           mod_find_mods_under(script.home, mods, FILE_PATH_CAPACITY);
}

static bool found_folders(wchar_t *game, wchar_t *mods)
{
    return CHECK(game != NULL) && mod_find_game_folder(game, FILE_PATH_CAPACITY) &&
           mod_find_local_mods(mods, FILE_PATH_CAPACITY);
}

bool ask_game_folders(wchar_t *game, wchar_t *mods)
{
    if (!CHECK(game != NULL) || !CHECK(mods != NULL) ||
        !CHECK(FILE_PATH_CAPACITY > 0)) {
        return false;
    }
    return script.active ? scripted_folders(game, mods) : found_folders(game, mods);
}
