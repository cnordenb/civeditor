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

_Static_assert((int)FORGE_CIV_LIMIT == (int)ROSTER_SLOT_LIMIT, "a mod has room for every civ the roster holds");

static const wchar_t FILTER[] = L"Civ files (*.civeditor.json;*.civbuilder.json)\0*.json\0All files (*.*)\0*.*\0";
static const wchar_t DATA_FOLDERS[FOLDER_TRIES][8] = { L"data", L"..\\data", L"" };
static const wchar_t TITLE[] = L"civeditor";

static CivDesign opened;
static wchar_t chosen_path[FILE_PATH_CAPACITY];
static ForgeResult built;
static wchar_t wide_message[MESSAGE_CAPACITY];

typedef struct {
    wchar_t game[FILE_PATH_CAPACITY];
    char name[FORGE_PREFIX_CAPACITY];
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
    const char *typed = civ_design_text(&app->history.present, TEXT_FIELD_NAME);
    const char *given = catalog_appearance_name(&app->catalog, APPEARANCE_BASE,
                                                app->history.present.choices[CHOICE_REPLACE]);
    char name[NAME_LIMIT] = "";

    if (!text_equal(typed, given)) {
        (void)text_copy_span(name, sizeof name, typed, NAME_LIMIT / 2);
    }
    (void)text_append(name, sizeof name, (name[0] != '\0') ? ".civeditor.json" : "civeditor_civ.civeditor.json");
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
    dialog.lpstrDefExt = L"civeditor.json";
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
        (void)ask_message(app->window, L"That file is not a civ file civeditor can read.", TITLE,
                          MB_OK | MB_ICONERROR);
        return false;
    }
    (void)history_replace(&app->history, &opened);
    (void)wcsncpy(app->file_path, chosen_path, FILE_PATH_CAPACITY - 1);
    app->unsaved = false;
    app->built = false;
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

static bool civs_are_ready(AppState *app, ForgeJob *job)
{
    int civ;

    job->design_count = roster_count(&app->roster);
    for (civ = 0; civ < job->design_count; ++civ) {
        job->designs[civ] = roster_civ(&app->roster, civ);
    }
    return forge_civs_are_finished(job, &built) || tell(app, built.problem, MB_ICONINFORMATION);
}

static bool is_named(AppState *app, BuildPlaces *places)
{
    if (!ask_text(app->window, L"Name of the mod. Its two folders in the mods folder are named after it.",
                  app->mod_name, MOD_NAME_CAPACITY) ||
        !dialogs_narrow(app->mod_name, places->name, sizeof places->name)) {
        return false;
    }
    return forge_prefix(places->name, places->prefix, sizeof places->prefix);
}

static bool knows_mods_folder(AppState *app)
{
    if ((app->mods_folder[0] == L'\0') && !ask_folder(ASKED_MODS_FOLDER, app->mods_folder)) {
        app->mods_folder[0] = L'\0';
    }
    return app->mods_folder[0] != L'\0';
}

static bool is_folder(const wchar_t *path)
{
    DWORD attributes = GetFileAttributesW(path);

    return (attributes != INVALID_FILE_ATTRIBUTES) && ((attributes & FILE_ATTRIBUTE_DIRECTORY) != 0U);
}

bool dialogs_options(AppState *app)
{
    static wchar_t folder[FILE_PATH_CAPACITY];

    (void)knows_mods_folder(app);
    (void)lstrcpynW(folder, app->mods_folder, FILE_PATH_CAPACITY);
    if (!ask_text(app->window, L"The game's local mods folder, where Build mod writes. Correct it if it is wrong.",
                  folder, FILE_PATH_CAPACITY)) {
        dialogs_status(app, "The mods folder was left as it was.");
        return false;
    }
    if (!is_folder(folder)) {
        dialogs_status(app, "The mods folder was left as it was.");
        return tell(app, "That folder does not exist, so the mods folder was left as it was.", MB_ICONERROR);
    }
    (void)lstrcpynW(app->mods_folder, folder, FILE_PATH_CAPACITY);
    dialogs_status(app, "Build mod now writes into the mods folder given.");
    return true;
}

static bool can_build(AppState *app, BuildPlaces *places, ForgeJob *job)
{
    if (!app->catalog_loaded || !ask_folder(ASKED_GAME_FOLDER, places->game) || !knows_mods_folder(app)) {
        return tell(app, "A mod cannot be built: the data folder, the game or its mods folder was not found on "
                         "this computer. Options in the menu File sets the mods folder.", MB_ICONERROR);
    }
    return civs_are_ready(app, job) && is_named(app, places);
}

static void report_build(AppState *app, const BuildPlaces *places, const ForgeJob *job)
{
    static SummaryText report;
    static BuiltCivs civs;
    static char line[STATUS_CAPACITY];
    int civ;

    civs.reports = built.recipes;
    civs.count = job->design_count;
    for (civ = 0; civ < job->design_count; ++civ) {
        civs.designs[civ] = job->designs[civ];
    }
    (void)summary_write_build(&report, places->prefix, &civs);
    app->built = summary_build_is_whole(&civs);
    controls_show(app);
    line[0] = '\0';
    (void)text_append_three(line, sizeof line, "The mod was built as ", places->prefix, "-data and its UI mod.");
    dialogs_status(app, line);
    if (!app->built) {
        (void)tell(app, report.text, MB_ICONINFORMATION);
    }
}

static bool not_built(AppState *app)
{
    controls_show(app);
    dialogs_status(app, "No mod was built.");
    return false;
}

bool dialogs_build(AppState *app)
{
    static BuildPlaces places;
    static ForgeJob job;
    HCURSOR previous;
    bool done;

    app->built = false;
    if (!can_build(app, &places, &job)) {
        return not_built(app);
    }
    job.mod_name = places.name;
    job.catalog = &app->catalog;
    job.game_folder = places.game;
    job.mods_folder = app->mods_folder;
    previous = SetCursor(LoadCursorW(NULL, IDC_WAIT));
    done = forge_build(&job, &built);
    (void)SetCursor(previous);
    if (!done) {
        return not_built(app) || tell(app, built.problem, MB_ICONERROR);
    }
    report_build(app, &places, &job);
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

static int list_heroes(AppState *app, wchar_t rows[LIST_ROW_CAPACITY][ASK_ROW_WIDTH])
{
    ListQuery query = { CONTROL_HERO_CHOOSE, "", LIST_ALL_CATEGORIES };
    ListItem item = { CONTROL_HERO_CHOOSE, 0 };
    int chosen = -1;
    int row;

    (void)list_model_fill(&app->lists[CONTROL_HERO_CHOOSE], &app->catalog, &query);
    for (row = 0; (row < app->lists[CONTROL_HERO_CHOOSE].count) && (row < LIST_ROW_CAPACITY); ++row) {
        item.item = app->lists[CONTROL_HERO_CHOOSE].items[row];
        (void)dialogs_widen(list_item_text(&app->catalog, item).text, rows[row], ASK_ROW_WIDTH);
        chosen = list_item_is_chosen(&app->catalog, &app->history.present, item) ? row : chosen;
    }
    return chosen;
}

bool dialogs_choose_hero(AppState *app, Command *command)
{
    static wchar_t rows[LIST_ROW_CAPACITY][ASK_ROW_WIDTH];
    static wchar_t name[CIV_NAME_CAPACITY];
    AskQuestion question = { L"Name of the hero", name, CIV_NAME_CAPACITY, L"Heroes", rows, 0, -1 };
    ListItem item = { CONTROL_HERO_CHOOSE, 0 };

    question.row = list_heroes(app, rows);
    question.row_count = app->lists[CONTROL_HERO_CHOOSE].count;
    if (!dialogs_widen(civ_design_text(&app->history.present, TEXT_FIELD_HERO_NAME), name, CIV_NAME_CAPACITY) ||
        !text_dialog_show(app->window, &question) || (question.row < 0) || (question.row >= question.row_count)) {
        return false;
    }
    item.item = app->lists[CONTROL_HERO_CHOOSE].items[question.row];
    *command = list_item_command(&app->catalog, item);
    return dialogs_narrow(name, command->text, (int)sizeof command->text);
}
