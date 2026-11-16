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
static const wchar_t LIBRARY_LIST[] = L"steamapps\\libraryfolders.vdf";
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

enum { QUOTE_END_COUNT = 2 };

static const int QUOTE_ENDS[QUOTE_END_COUNT] = { '"', '\0' };

static size_t after_opening_quote(size_t from)
{
    size_t at = from;
    int step;

    if (!CHECK(from < (size_t)LIBRARY_LIST_CAPACITY) || !CHECK(NARROW_PATH_CAPACITY > 0)) {
        return from;
    }
    for (step = 0; step < NARROW_PATH_CAPACITY; ++step) {
        if (check_is_among(library_list[at], QUOTE_ENDS, QUOTE_END_COUNT)) {
            break;
        }
        ++at;
    }
    return (library_list[at] == '"') ? at + 1U : at;
}

static size_t read_quoted(size_t from, char *path)
{
    size_t at = after_opening_quote(from);
    size_t used = 0U;
    int step;

    if (!CHECK(path != NULL) || !CHECK(from < (size_t)LIBRARY_LIST_CAPACITY)) {
        return from;
    }
    for (step = 0; step < NARROW_PATH_CAPACITY; ++step) {
        if (check_is_among(library_list[at], QUOTE_ENDS, QUOTE_END_COUNT)) {
            break;
        }
        if ((library_list[at] == '\\') && (library_list[at + 1U] == '\\')) {
            ++at;
        }
        if (used + 1U < (size_t)NARROW_PATH_CAPACITY) {
            path[used] = library_list[at];
            ++used;
        }
        ++at;
    }
    path[used] = '\0';
    return at;
}

static bool read_library_list(const wchar_t *steam)
{
    wchar_t path[FILE_PATH_CAPACITY];

    if (!CHECK(steam != NULL) || !file_join_path(path, FILE_PATH_CAPACITY, steam, LIBRARY_LIST)) {
        return false;
    }
    return file_read_all(path, library_list, sizeof library_list).succeeded;
}

static bool find_in_libraries(const wchar_t *steam, wchar_t *folder, size_t capacity)
{
    char narrow[NARROW_PATH_CAPACITY];
    wchar_t library[FILE_PATH_CAPACITY];
    const char *key = library_list;
    int count;

    if (!CHECK(folder != NULL) || !CHECK(capacity > 0U) || !read_library_list(steam)) {
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

bool mod_find_game_under(const wchar_t *steam, wchar_t *folder, size_t capacity)
{
    if (!CHECK(steam != NULL) || !CHECK(folder != NULL) || !CHECK(capacity >= (size_t)FILE_PATH_CAPACITY)) {
        return false;
    }
    return holds_game(steam, folder, capacity) || find_in_libraries(steam, folder, capacity);
}

bool mod_find_game_folder(wchar_t *folder, size_t capacity)
{
    return CHECK(folder != NULL) && mod_find_game_under(STEAM_FOLDER, folder, capacity);
}

static long long folder_age(const wchar_t *path)
{
    struct _stat64 status;

    if (!CHECK(path != NULL) || (_wstat64(path, &status) != 0) || ((status.st_mode & _S_IFDIR) == 0)) {
        return NO_AGE;
    }
    return (long long)status.st_mtime;
}

static long long mods_age(const wchar_t *profiles, const wchar_t *name, wchar_t *mods)
{
    wchar_t profile[FILE_PATH_CAPACITY];

    if (!CHECK(profiles != NULL) || !CHECK(name != NULL) || (name[0] == L'.')) {
        return NO_AGE;
    }
    if (!file_join_path(profile, FILE_PATH_CAPACITY, profiles, name) ||
        !file_join_path(mods, FILE_PATH_CAPACITY, profile, LOCAL_MODS)) {
        return NO_AGE;
    }
    return folder_age(mods);
}

static bool open_profiles(const wchar_t *home, wchar_t *profiles, intptr_t *search, struct _wfinddata64_t *entry)
{
    wchar_t pattern[FILE_PATH_CAPACITY];

    if (!CHECK(home != NULL) || !CHECK(profiles != NULL) || !CHECK(search != NULL)) {
        return false;
    }
    if (!file_join_path(profiles, FILE_PATH_CAPACITY, home, PROFILES) ||
        !file_join_path(pattern, FILE_PATH_CAPACITY, profiles, L"*")) {
        return false;
    }
    *search = _wfindfirst64(pattern, entry);
    return *search != -1;
}

bool mod_find_mods_under(const wchar_t *home, wchar_t *folder, size_t capacity)
{
    wchar_t profiles[FILE_PATH_CAPACITY];
    wchar_t mods[FILE_PATH_CAPACITY];
    struct _wfinddata64_t entry;
    long long newest = NO_AGE;
    long long age;
    intptr_t search = -1;
    int count;

    if (!CHECK(folder != NULL) || !CHECK(capacity >= (size_t)FILE_PATH_CAPACITY) ||
        !open_profiles(home, profiles, &search, &entry)) {
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

bool mod_find_local_mods(wchar_t *folder, size_t capacity)
{
    const wchar_t *home = _wgetenv(L"USERPROFILE");

    if (!CHECK(folder != NULL) || (home == NULL)) {
        return false;
    }
    return mod_find_mods_under(home, folder, capacity);
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

bool mod_join(char *text, const char *first, const char *second)
{
    return text_join(text, FILE_PATH_CAPACITY, first, second);
}

bool mod_game_path(const wchar_t *game, const char *relative, wchar_t *path)
{
    wchar_t wide[FILE_PATH_CAPACITY];

    if (!CHECK(game != NULL) || !CHECK(relative != NULL)) {
        return false;
    }
    return text_widen(relative, wide, FILE_PATH_CAPACITY) && mod_game_file(game, wide, path);
}

bool mod_folder_path(const wchar_t *mods, const char *folder, wchar_t *path)
{
    wchar_t wide[FILE_PATH_CAPACITY];

    if (!CHECK(mods != NULL) || !CHECK(folder != NULL)) {
        return false;
    }
    return text_widen(folder, wide, FILE_PATH_CAPACITY) && mod_make_folders(mods, wide) &&
           file_join_path(path, FILE_PATH_CAPACITY, mods, wide);
}

bool mod_file_path(const wchar_t *mods, const char *folder, const wchar_t *name, wchar_t *path)
{
    wchar_t place[FILE_PATH_CAPACITY];

    if (!CHECK(name != NULL) || !CHECK(path != NULL)) {
        return false;
    }
    return mod_folder_path(mods, folder, place) && file_join_path(path, FILE_PATH_CAPACITY, place, name);
}
