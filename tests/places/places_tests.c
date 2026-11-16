#include "places_tests.h"

#include <string.h>
#include <wchar.h>

#include "../../src/core/check.h"
#include "../../src/mod/mod_places.h"

static const wchar_t BUILD[] = L"build";
static const wchar_t GAME_DAT_FOLDER[] = L"steamapps\\common\\AoE2DE\\resources\\_common\\dat";
static const wchar_t DAT_NAME[] = L"empires2_x2_p1.dat";
static const wchar_t LIST_NAME[] = L"libraryfolders.vdf";
static const char DAT[] = "a dat file";
static const char LIST_OF_ONE[] = "\"libraryfolders\"\n{\n\t\"0\"\n\t{\n"
                                  "\t\t\"path\"\t\t\"build\\\\test_library\"\n\t}\n}\n";
static const char LIST_OF_NOWHERE[] = "\"libraryfolders\"\n{\n\t\"0\"\n\t{\n"
                                      "\t\t\"path\"\t\t\"build\\\\nowhere\"\n\t}\n}\n";

static wchar_t found[FILE_PATH_CAPACITY];

static bool write_under(const wchar_t *root, const wchar_t *folder, const wchar_t *name, const char *text)
{
    wchar_t relative[FILE_PATH_CAPACITY];
    wchar_t place[FILE_PATH_CAPACITY];
    wchar_t path[FILE_PATH_CAPACITY];

    if (!CHECK(root != NULL) || !CHECK(folder != NULL) || !CHECK(text != NULL)) {
        return false;
    }
    return CHECK(file_join_path(relative, FILE_PATH_CAPACITY, root, folder)) &&
           CHECK(mod_make_folders(BUILD, relative)) &&
           CHECK(file_join_path(place, FILE_PATH_CAPACITY, BUILD, relative)) &&
           CHECK(file_join_path(path, FILE_PATH_CAPACITY, place, name)) &&
           CHECK(file_write_all(path, text, strlen(text)));
}

static bool game_is_found_under(const wchar_t *steam, const wchar_t *expected)
{
    if (!CHECK(steam != NULL) || !CHECK(expected != NULL)) {
        return false;
    }
    return mod_find_game_under(steam, found, FILE_PATH_CAPACITY) && (wcsstr(found, expected) != NULL);
}

static bool game_is_listed(const wchar_t *steam_name, const char *list, const wchar_t *steam, const wchar_t *found_in)
{
    return CHECK(list != NULL) && write_under(L"test_library", GAME_DAT_FOLDER, DAT_NAME, DAT) &&
           write_under(steam_name, L"steamapps", LIST_NAME, list) && game_is_found_under(steam, found_in);
}

bool test_the_game_is_found_in_the_folder_of_steam(void)
{
    return write_under(L"test_steam", GAME_DAT_FOLDER, DAT_NAME, DAT) &&
           game_is_found_under(L"build\\test_steam", L"build\\test_steam\\steamapps\\common\\AoE2DE");
}

bool test_the_game_is_found_in_a_library_that_steam_lists(void)
{
    return game_is_listed(L"test_listing", LIST_OF_ONE, L"build\\test_listing", L"build\\test_library\\steamapps");
}

bool test_the_game_is_not_found_where_no_library_holds_it(void)
{
    return write_under(L"test_no_game", L"steamapps", LIST_NAME, LIST_OF_NOWHERE) &&
           !mod_find_game_under(L"build\\test_no_game", found, FILE_PATH_CAPACITY);
}

bool test_the_local_mods_folder_of_a_profile_is_found(void)
{
    return write_under(L"test_home", L"Games\\Age of Empires 2 DE\\7656\\mods\\local", L"mod.txt", DAT) &&
           mod_find_mods_under(L"build\\test_home", found, FILE_PATH_CAPACITY) &&
           (wcsstr(found, L"7656\\mods\\local") != NULL);
}

bool test_a_home_without_a_profile_of_the_game_has_no_mods_folder(void)
{
    return !mod_find_mods_under(L"build\\no_home", found, FILE_PATH_CAPACITY) && (BUILD[0] == L'b');
}
