#include "mod_places.h"

#include <io.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "../core/check.h"
#include "../core/text.h"

enum {
    LIBRARY_LIST_CAPACITY = 65536,
    LIBRARY_LIMIT = 64,
    PROFILE_LIMIT = 256,
    NARROW_PATH_CAPACITY = 1040,
    NO_AGE = -1
};

static const wchar_t STEAM_FOLDER[] = L"C:\\Program Files (x86)\\Steam";
static const wchar_t LIBRARY_LIST[] = L"C:\\Program Files (x86)\\Steam\\steamapps\\libraryfolders.vdf";
static const wchar_t GAME_IN_LIBRARY[] = L"steamapps\\common\\AoE2DE";
static const wchar_t DAT_IN_GAME[] = L"resources\\_common\\dat\\empires2_x2_p1.dat";
static const wchar_t PROFILES[] = L"Games\\Age of Empires 2 DE";
static const wchar_t LOCAL_MODS[] = L"mods\\local";
static const char PATH_KEY[] = "\"path\"";

static char library_list[LIBRARY_LIST_CAPACITY];

static bool folder_exists(const wchar_t *path)
{
    struct _stat64 status;

    if (!CHECK(path != NULL) || !CHECK(path[0] != L'\0')) {
        return false;
    }
    return (_wstat64(path, &status) == 0) && ((status.st_mode & _S_IFDIR) != 0);
}

bool mod_game_file(const wchar_t *game, const wchar_t *relative, wchar_t *path)
{
    if (!CHECK(game != NULL) || !CHECK(relative != NULL) || !CHECK(path != NULL)) {
        return false;
    }
    return file_join_path(path, FILE_PATH_CAPACITY, game, relative);
}

static bool holds_game(const wchar_t *library, wchar_t *folder, size_t capacity)
{
    wchar_t dat[FILE_PATH_CAPACITY];

    if (!CHECK(library != NULL) || !CHECK(folder != NULL)) {
        return false;
    }
    return file_join_path(folder, capacity, library, GAME_IN_LIBRARY) && mod_game_file(folder, DAT_IN_GAME, dat) &&
           file_exists(dat);
}

static size_t read_quoted(size_t from, char *path)
{
    size_t at = from;
    size_t used = 0U;
    int step;

    if (!CHECK(path != NULL) || !CHECK(from < (size_t)LIBRARY_LIST_CAPACITY)) {
        return from;
    }
    for (step = 0; (step < NARROW_PATH_CAPACITY) && (library_list[at] != '"') && (library_list[at] != '\0'); ++step) {
        ++at;
    }
    at += (library_list[at] == '"') ? 1U : 0U;
    for (step = 0; (step < NARROW_PATH_CAPACITY) && (library_list[at] != '"') && (library_list[at] != '\0'); ++step) {
        at += ((library_list[at] == '\\') && (library_list[at + 1U] == '\\')) ? 1U : 0U;
        if (used + 1U < (size_t)NARROW_PATH_CAPACITY) {
            path[used] = library_list[at];
            ++used;
        }
        ++at;
    }
    path[used] = '\0';
    return at;
}

static bool find_in_libraries(wchar_t *folder, size_t capacity)
{
    FileRead list = file_read_all(LIBRARY_LIST, library_list, sizeof library_list);
    char narrow[NARROW_PATH_CAPACITY];
    wchar_t library[FILE_PATH_CAPACITY];
    const char *key = library_list;
    int count;

    if (!CHECK(folder != NULL) || !CHECK(capacity > 0U) || !list.succeeded) {
        return false;
    }
    for (count = 0; count < LIBRARY_LIMIT; ++count) {
        key = strstr(key, PATH_KEY);
        if (key == NULL) {
            return false;
        }
        key = library_list + read_quoted((size_t)(key - library_list) + sizeof PATH_KEY - 1U, narrow);
        if (text_widen(narrow, library, FILE_PATH_CAPACITY) && holds_game(library, folder, capacity)) {
            return true;
        }
    }
    return false;
}

bool mod_find_game_folder(wchar_t *folder, size_t capacity)
{
    if (!CHECK(folder != NULL) || !CHECK(capacity >= (size_t)FILE_PATH_CAPACITY)) {
        return false;
    }
    return holds_game(STEAM_FOLDER, folder, capacity) || find_in_libraries(folder, capacity);
}

static long long mods_age(const wchar_t *profiles, const wchar_t *name, wchar_t *mods)
{
    wchar_t profile[FILE_PATH_CAPACITY];
    struct _stat64 status;

    if (!CHECK(profiles != NULL) || !CHECK(name != NULL) || (name[0] == L'.')) {
        return NO_AGE;
    }
    if (!file_join_path(profile, FILE_PATH_CAPACITY, profiles, name) ||
        !file_join_path(mods, FILE_PATH_CAPACITY, profile, LOCAL_MODS) || !folder_exists(mods) ||
        (_wstat64(mods, &status) != 0)) {
        return NO_AGE;
    }
    return (long long)status.st_mtime;
}

static bool open_profiles(wchar_t *profiles, intptr_t *search, struct _wfinddata64_t *entry)
{
    const wchar_t *home = _wgetenv(L"USERPROFILE");
    wchar_t pattern[FILE_PATH_CAPACITY];

    if (!CHECK(profiles != NULL) || !CHECK(search != NULL) || (home == NULL)) {
        return false;
    }
    if (!file_join_path(profiles, FILE_PATH_CAPACITY, home, PROFILES) ||
        !file_join_path(pattern, FILE_PATH_CAPACITY, profiles, L"*")) {
        return false;
    }
    *search = _wfindfirst64(pattern, entry);
    return *search != -1;
}

bool mod_find_local_mods(wchar_t *folder, size_t capacity)
{
    wchar_t profiles[FILE_PATH_CAPACITY];
    wchar_t mods[FILE_PATH_CAPACITY];
    struct _wfinddata64_t entry;
    long long newest = NO_AGE;
    long long age;
    intptr_t search = -1;
    int count;

    if (!CHECK(folder != NULL) || !CHECK(capacity >= (size_t)FILE_PATH_CAPACITY) ||
        !open_profiles(profiles, &search, &entry)) {
        return false;
    }
    for (count = 0; count < PROFILE_LIMIT; ++count) {
        age = ((entry.attrib & _A_SUBDIR) != 0U) ? mods_age(profiles, entry.name, mods) : NO_AGE;
        if (age > newest) {
            newest = age;
            (void)wcsncpy(folder, mods, capacity - 1U);
            folder[capacity - 1U] = L'\0';
        }
        if (_wfindnext64(search, &entry) != 0) {
            break;
        }
    }
    (void)_findclose(search);
    return newest > NO_AGE;
}

bool mod_make_folders(const wchar_t *root, const wchar_t *relative)
{
    wchar_t path[FILE_PATH_CAPACITY];
    size_t at;

    if (!CHECK(root != NULL) || !CHECK(relative != NULL) || !file_join_path(path, FILE_PATH_CAPACITY, root, relative)) {
        return false;
    }
    for (at = wcslen(root) + 1U; (at < (size_t)FILE_PATH_CAPACITY) && (path[at] != L'\0'); ++at) {
        if (path[at] == L'\\') {
            path[at] = L'\0';
            (void)_wmkdir(path);
            path[at] = L'\\';
        }
    }
    (void)_wmkdir(path);
    return folder_exists(path);
}
