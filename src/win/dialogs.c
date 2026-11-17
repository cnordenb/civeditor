#include "win_parts.h"

#include <commdlg.h>
#include <wchar.h>

#include "../core/text.h"
#include "../forge/forge.h"
#include "../mod/mod_places.h"
#include "../persist/civ_file.h"
#include "../present/summary.h"
#include "../shell/shell.h"

enum { FOLDER_TRIES = 3, NAME_LIMIT = 96, MESSAGE_CAPACITY = 2048, LIVE_REGION_CHANGED = 0x8019 };

static const wchar_t FILTER[] = L"Civ files (*.civbuilder.json)\0*.civbuilder.json;*.json\0All files (*.*)\0*.*\0";
static const wchar_t DATA_FOLDERS[FOLDER_TRIES][8] = { L"data", L"..\\data", L"" };
static const wchar_t TITLE[] = L"civbuilder";

static CivDesign opened;
static wchar_t chosen_path[FILE_PATH_CAPACITY];
static ForgeResult built;
static char message[MESSAGE_CAPACITY];
static wchar_t wide_message[MESSAGE_CAPACITY];

typedef struct {
    wchar_t game[FILE_PATH_CAPACITY];
    wchar_t mods[FILE_PATH_CAPACITY];
    char prefix[FORGE_PREFIX_CAPACITY];
} BuildPlaces;

bool dialogs_widen(const char *text, wchar_t *wide, int capacity)
{
    wide[0] = L'\0';
    return MultiByteToWideChar(CP_UTF8, 0, text, -1, wide, capacity) > 0;
}

bool dialogs_narrow(const wchar_t *wide, char *text, int capacity)
{
    text[0] = '\0';
    return WideCharToMultiByte(CP_UTF8, 0, wide, -1, text, capacity, NULL, NULL) > 0;
}

void dialogs_status(AppState *app, const char *text)
{
    wchar_t status[STATUS_CAPACITY];

    if (dialogs_widen(text, status, STATUS_CAPACITY)) {
        (void)SetWindowTextW(app->controls[CONTROL_STATUS], status);
        NotifyWinEvent(LIVE_REGION_CHANGED, app->controls[CONTROL_STATUS], OBJID_CLIENT, CHILDID_SELF);
    }
}

static bool exe_folder(wchar_t *folder, DWORD capacity)
{
    DWORD length;
    DWORD index;

    length = GetModuleFileNameW(NULL, folder, capacity);
    if ((length == 0U) || (length >= capacity)) {
        return false;
    }
    for (index = length; index > 0U; --index) {
        if (folder[index - 1U] == L'\\') {
            folder[index - 1U] = L'\0';
            return true;
        }
    }
    return false;
}

bool dialogs_load_catalog(AppState *app)
{
    wchar_t folder[FILE_PATH_CAPACITY];
    wchar_t data[FILE_PATH_CAPACITY];
    int attempt;

    if (!exe_folder(folder, FILE_PATH_CAPACITY)) {
        return false;
    }
    for (attempt = 0; (attempt < FOLDER_TRIES) && (DATA_FOLDERS[attempt][0] != L'\0'); ++attempt) {
        if (file_join_path(data, FILE_PATH_CAPACITY, folder, DATA_FOLDERS[attempt]) &&
            catalog_load(&app->catalog, data)) {
            app->catalog_loaded = true;
            return true;
        }
    }
    app->catalog_loaded = catalog_load(&app->catalog, L"data");
    return app->catalog_loaded;
}

static void suggest_name(const AppState *app)
{
    char name[NAME_LIMIT] = "";

    (void)text_copy_span(name, sizeof name, civ_design_text(&app->history.present, TEXT_FIELD_NAME), NAME_LIMIT / 2);
    (void)text_append(name, sizeof name, (name[0] != '\0') ? ".civbuilder.json" : "civbuilder_civ.civbuilder.json");
    if (!dialogs_widen(name, chosen_path, FILE_PATH_CAPACITY)) {
        chosen_path[0] = L'\0';
    }
}

static OPENFILENAMEW file_dialog(const AppState *app)
{
    OPENFILENAMEW dialog = { 0 };

    dialog.lStructSize = sizeof dialog;
    dialog.hwndOwner = app->window;
    dialog.lpstrFilter = FILTER;
    dialog.lpstrFile = chosen_path;
    dialog.nMaxFile = FILE_PATH_CAPACITY;
    dialog.lpstrDefExt = L"civbuilder.json";
    return dialog;
}

bool dialogs_save(AppState *app)
{
    OPENFILENAMEW dialog;

    suggest_name(app);
    dialog = file_dialog(app);
    dialog.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;
    if (!ask_file(&dialog, true)) {
        dialogs_status(app, "The civ was not saved.");
        return false;
    }
    if (!civ_file_save(&app->history.present, chosen_path)) {
        (void)ask_message(app->window, L"The civ file could not be written there.", TITLE, MB_OK | MB_ICONERROR);
        return false;
    }
    (void)wcsncpy(app->file_path, chosen_path, FILE_PATH_CAPACITY - 1);
    app->unsaved = false;
    dialogs_status(app, "The civ was saved.");
    return true;
}

bool dialogs_open(AppState *app)
{
    OPENFILENAMEW dialog;

    if (!dialogs_may_discard(app)) {
        return false;
    }
    chosen_path[0] = L'\0';
    dialog = file_dialog(app);
    dialog.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;
    if (!ask_file(&dialog, false)) {
        return false;
    }
    if (!civ_file_load(&opened, chosen_path)) {
        (void)ask_message(app->window, L"That file is not a civ file civbuilder can read.", TITLE,
                          MB_OK | MB_ICONERROR);
        return false;
    }
    (void)history_replace(&app->history, &opened);
    (void)wcsncpy(app->file_path, chosen_path, FILE_PATH_CAPACITY - 1);
    app->unsaved = false;
    sync_design(app, true);
    dialogs_status(app, "The civ file was opened.");
    return true;
}

static bool tell(const AppState *app, const char *text, UINT icon)
{
    if (!dialogs_widen(text, wide_message, MESSAGE_CAPACITY)) {
        return false;
    }
    (void)ask_message(app->window, wide_message, TITLE, MB_OK | icon);
    return false;
}

static bool may_build_into(const AppState *app, const wchar_t *mods)
{
    char folder[FILE_PATH_CAPACITY];

    if (!dialogs_narrow(mods, folder, FILE_PATH_CAPACITY)) {
        return false;
    }
    message[0] = '\0';
    if (!text_append_three(message, sizeof message, "Build the mod into this folder?\n\n", folder,
                           "\n\nAn earlier build of a civ with the same name is replaced.") ||
        !dialogs_widen(message, wide_message, MESSAGE_CAPACITY)) {
        return false;
    }
    return ask_message(app->window, wide_message, TITLE, MB_OKCANCEL | MB_ICONQUESTION) == IDOK;
}

static bool can_build(AppState *app, BuildPlaces *places)
{
    if (!app->catalog_loaded || !ask_game_folders(places->game, places->mods) ||
        !forge_prefix(app->history.present.texts[TEXT_FIELD_NAME], places->prefix, sizeof places->prefix)) {
        return tell(app, "A mod cannot be built: the data folder, the game or its mods folder was not found on "
                         "this computer.", MB_ICONERROR);
    }
    if (!may_build_into(app, places->mods)) {
        dialogs_status(app, "No mod was built.");
        return false;
    }
    return true;
}

bool dialogs_build(AppState *app)
{
    static BuildPlaces places;
    static SummaryText report;
    ForgeJob job;
    HCURSOR previous;
    bool done;

    if (!can_build(app, &places)) {
        return false;
    }
    job.design = &app->history.present;
    job.catalog = &app->catalog;
    job.game_folder = places.game;
    job.mods_folder = places.mods;
    previous = SetCursor(LoadCursorW(NULL, IDC_WAIT));
    done = forge_build(&job, &built);
    (void)SetCursor(previous);
    if (!done) {
        dialogs_status(app, "No mod was built.");
        return tell(app, built.problem, MB_ICONERROR);
    }
    (void)summary_write_build(&report, places.prefix, &built.recipe);
    dialogs_status(app, "The mod was built.");
    (void)tell(app, report.text, MB_ICONINFORMATION);
    return true;
}

bool dialogs_may_discard(AppState *app)
{
    int answer;

    if (!app->unsaved) {
        return true;
    }
    answer = ask_message(app->window, L"The civ has changes that are not saved. Save them first?", TITLE,
                         MB_YESNOCANCEL | MB_ICONQUESTION);
    if (answer == IDYES) {
        return dialogs_save(app);
    }
    return answer == IDNO;
}

void dialogs_glossary(AppState *app)
{
    static char text[WIDE_TEXT_CAPACITY];
    static wchar_t wide[WIDE_TEXT_CAPACITY];
    GlossaryEntry entry;
    int index;

    text[0] = '\0';
    for (index = 0; index < LAYOUT_GLOSSARY_COUNT; ++index) {
        entry = layout_glossary(index);
        (void)text_append_three(text, sizeof text, entry.word, ": ", entry.meaning);
        (void)text_append(text, sizeof text, "\n");
    }
    if (dialogs_widen(text, wide, WIDE_TEXT_CAPACITY)) {
        (void)ask_message(app->window, wide, L"Glossary", MB_OK | MB_ICONINFORMATION);
    }
}
