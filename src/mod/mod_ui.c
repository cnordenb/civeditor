#include "mod_strings.h"

#include "../core/check.h"
#include "../core/text.h"
#include "../json/json_writer.h"
#include "mod_places.h"

enum { VANILLA_CAPACITY = 4194304, LANGUAGE_COUNT = 15, LANGUAGE_WIDTH = 4 };

static const char LANGUAGES[LANGUAGE_COUNT][LANGUAGE_WIDTH] = {
    "br", "de", "en", "es", "fr", "hi", "it", "jp", "ko", "ms", "mx", "tr", "tw", "vi", "zh"
};

static char vanilla[VANILLA_CAPACITY];
static ModStrings strings;
static JsonWriter info;

bool mod_write_info(const wchar_t *mods, const char *folder, const char *title)
{
    wchar_t path[FILE_PATH_CAPACITY];

    if (!CHECK(mods != NULL) || !CHECK(title != NULL) || !CHECK(folder != NULL)) {
        return false;
    }
    json_writer_start(&info);
    json_begin(&info, NULL, '{');
    json_write_string(&info, "Title", title);
    json_write_number(&info, "CacheStatus", 0);
    json_write_string(&info, "Description", "");
    json_write_string(&info, "Author", "");
    json_write_string(&info, "builder_version", "1.0.0");
    json_end(&info, '}');
    return json_writer_succeeded(&info) && mod_file_path(mods, folder, L"info.json", path) &&
           file_write_all(path, info.data, info.length);
}

static void read_vanilla(const ModUi *ui, int language)
{
    char relative[FILE_PATH_CAPACITY];
    wchar_t path[FILE_PATH_CAPACITY];

    if (!CHECK(ui != NULL) || !CHECK((language >= 0) && (language < LANGUAGE_COUNT)) ||
        !CHECK(ui->game != NULL)) {
        return;
    }
    vanilla[0] = '\0';
    if (!mod_join(relative, "resources\\", LANGUAGES[language]) ||
        !text_append(relative, sizeof relative, "\\strings\\key-value\\key-value-strings-utf8.txt") ||
        !mod_game_path(ui->game, relative, path)) {
        return;
    }
    if (!file_read_all(path, vanilla, sizeof vanilla).succeeded) {
        vanilla[0] = '\0';
    }
}

static bool language_file(const ModUi *ui, int language, wchar_t *path)
{
    char folder[FILE_PATH_CAPACITY];

    if (!CHECK(ui != NULL) || !CHECK((language >= 0) && (language < LANGUAGE_COUNT)) ||
        !CHECK(ui->prefix != NULL)) {
        return false;
    }
    return mod_join(folder, ui->prefix, "-ui\\resources\\") &&
           text_append(folder, sizeof folder, LANGUAGES[language]) &&
           text_append(folder, sizeof folder, "\\strings\\key-value") &&
           mod_file_path(ui->mods, folder, L"key-value-modded-strings-utf8.txt", path);
}

static bool write_language(const ModUi *ui, int language)
{
    wchar_t path[FILE_PATH_CAPACITY];

    if (!CHECK(ui != NULL) || !CHECK((language >= 0) && (language < LANGUAGE_COUNT)) ||
        !CHECK(ui->civ != NULL)) {
        return false;
    }
    read_vanilla(ui, language);
    return mod_strings_write(&strings, ui->civ, vanilla) && language_file(ui, language, path) &&
           file_write_all(path, strings.text, strings.length);
}

bool mod_ui_write(const ModUi *ui)
{
    char folder[FILE_PATH_CAPACITY];
    char title[FILE_PATH_CAPACITY];
    const ModCiv *civ;
    const CivDesign *design;
    int language;
    bool fine;

    if (!CHECK(ui != NULL) || !CHECK(ui->civ != NULL) ||
        !CHECK(ui->mods != NULL)) {
        return false;
    }
    civ = ui->civ;
    design = civ->design;
    fine = mod_join(folder, ui->prefix, "-ui") && mod_join(title, design->texts[TEXT_FIELD_NAME], " (UI)") &&
           mod_write_info(ui->mods, folder, title);
    for (language = 0; fine && (language < LANGUAGE_COUNT); ++language) {
        fine = write_language(ui, language);
    }
    return fine;
}
