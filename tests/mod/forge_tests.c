#include "forge_tests.h"

#include <string.h>
#include <wchar.h>

#include "../../src/core/check.h"
#include "../../src/deflate/deflate.h"
#include "../../src/forge/forge.h"
#include "../../src/mod/mod_places.h"
#include "../dat/recipe_tests.h"
#include "game_folder.h"

enum { FILE_CAPACITY = 131072, BUILT_TECHS = 17 };

static const wchar_t BUILD[] = L"build";
static const wchar_t GAME[] = L"build\\test_game\\steamapps\\common\\AoE2DE";
static const wchar_t MODS[] = L"build\\test_mods";
static const wchar_t DAT_FOLDER[] = L"test_game\\steamapps\\common\\AoE2DE\\resources\\_common\\dat";
static const wchar_t TREE_FOLDER[] =
    L"test_game\\steamapps\\common\\AoE2DE\\resources\\_common\\dat\\CivTechTrees";
static const wchar_t ENGLISH_FOLDER[] =
    L"test_game\\steamapps\\common\\AoE2DE\\resources\\en\\strings\\key-value";

const wchar_t GAME_FOLDER_STEAM[] = L"build\\test_game";
static const wchar_t DAT_NAME[] = L"empires2_x2_p1.dat";
static const wchar_t LIST_NAME[] = L"civilizations.json";
static const wchar_t MOD_DAT[] = L"testonia-data\\resources\\_common\\dat\\empires2_x2_p1.dat";
static const wchar_t MOD_LIST[] = L"testonia-data\\resources\\_common\\dat\\civilizations.json";
static const wchar_t MOD_TREE[] = L"testonia-data\\resources\\_common\\dat\\CivTechTrees\\BRITONS.json";
static const wchar_t MOD_ENGLISH[] =
    L"testonia-ui\\resources\\en\\strings\\key-value\\key-value-modded-strings-utf8.txt";
static const wchar_t MOD_CHINESE[] =
    L"testonia-ui\\resources\\zh\\strings\\key-value\\key-value-modded-strings-utf8.txt";

static const char CIV_LIST[] = "{\"civilization_list\":[{},{\"name_string_id\":10271,\"tech_tree_name\":\"BRITONS\","
                               "\"unique_unit_id\":5,\"unique_unit_line\":-276}]}";
static const char LIST_OF_NOBODY[] = "{\"civilization_list\":[{},{\"name_string_id\":10271,"
                                     "\"tech_tree_name\":\"NOBODY\"}]}";
static const char TREE[] = "{\"civ_techs_buildings\":[],\"civ_techs_units\":[{\"Use Type\":\"Unit\",\"Node ID\":9,"
                           "\"Node Status\":\"ResearchedCompleted\"}]}";
static const char STRINGS[] = "10271 \"Britons\"\r\n120150 \"The Britons\"\r\n";
static const char NO_DAT[] = "not the game's data";

static char file_text[FILE_CAPACITY];
static unsigned char packed[FILE_CAPACITY];
static unsigned char unpacked[FILE_CAPACITY];
static ForgeResult built;
static DatMap built_map;

static bool write_game_file(const wchar_t *folder, const wchar_t *name, const char *data, size_t length)
{
    wchar_t place[FILE_PATH_CAPACITY];
    wchar_t path[FILE_PATH_CAPACITY];

    if (!CHECK(folder != NULL) || !CHECK(name != NULL) || !CHECK(data != NULL)) {
        return false;
    }
    return mod_make_folders(BUILD, folder) && file_join_path(place, FILE_PATH_CAPACITY, BUILD, folder) &&
           file_join_path(path, FILE_PATH_CAPACITY, place, name) && file_write_all(path, data, length);
}

static bool make_game_folder(void)
{
    DatBytes game;
    ByteBuffer out = { packed, sizeof packed, 0U };
    ByteSpan plain;

    if (!CHECK(sizeof CIV_LIST > 1U) || !CHECK(recipe_game_make(&game))) {
        return false;
    }
    plain.bytes = game.data;
    plain.size = game.size;
    return CHECK(deflate_pack(plain, &out)) && CHECK(mod_make_folders(BUILD, L"test_mods")) &&
           CHECK(write_game_file(DAT_FOLDER, DAT_NAME, (const char *)packed, out.size)) &&
           CHECK(write_game_file(DAT_FOLDER, LIST_NAME, CIV_LIST, sizeof CIV_LIST - 1U)) &&
           CHECK(write_game_file(TREE_FOLDER, L"BRITONS.json", TREE, sizeof TREE - 1U)) &&
           CHECK(write_game_file(ENGLISH_FOLDER, L"key-value-strings-utf8.txt", STRINGS, sizeof STRINGS - 1U));
}

bool game_folder_is_made(void)
{
    return CHECK(sizeof GAME_FOLDER_STEAM > 1U) && make_game_folder();
}

static bool build_mod(Scenario scenario, const wchar_t *game_folder)
{
    ForgeJob job = { recipe_game_design(scenario), recipe_game_catalog(), game_folder, MODS };

    if (!CHECK(game_folder != NULL) || !CHECK(job.design != NULL)) {
        return false;
    }
    return forge_build(&job, &built);
}

static size_t read_mod_file(const wchar_t *relative)
{
    wchar_t path[FILE_PATH_CAPACITY];
    FileRead read;

    if (!CHECK(relative != NULL) || !file_join_path(path, FILE_PATH_CAPACITY, MODS, relative)) {
        return 0U;
    }
    read = file_read_all(path, file_text, sizeof file_text);
    return read.succeeded ? read.length : 0U;
}

static bool mod_file_holds(const wchar_t *relative, const char *words)
{
    if (!CHECK(words != NULL) || !make_game_folder() || !build_mod(FULL_CIV, GAME)) {
        return false;
    }
    return (read_mod_file(relative) > 0U) && (strstr(file_text, words) != NULL);
}

static bool build_fails_saying(const char *words)
{
    if (!CHECK(words != NULL) || build_mod(FULL_CIV, GAME)) {
        return false;
    }
    return strstr(built.problem, words) != NULL;
}

static bool built_folder_holds(const wchar_t *folder, const wchar_t *words)
{
    return CHECK(folder != NULL) && make_game_folder() && build_mod(FULL_CIV, GAME) && (wcsstr(folder, words) != NULL);
}

static bool spoiled_game_says(const wchar_t *name, const char *data, size_t size, const char *words)
{
    return CHECK(name != NULL) && make_game_folder() && write_game_file(DAT_FOLDER, name, data, size) &&
           build_fails_saying(words);
}

bool test_a_built_mod_has_a_data_folder_named_after_the_civ(void)
{
    return built_folder_holds(built.data_mod, L"testonia-data");
}

bool test_a_built_mod_has_a_ui_folder_named_after_the_civ(void)
{
    return built_folder_holds(built.ui_mod, L"testonia-ui");
}

bool test_the_data_mods_info_names_the_civ(void)
{
    return mod_file_holds(L"testonia-data\\info.json", "Testonia");
}

bool test_the_ui_mods_info_says_that_it_is_the_ui_mod(void)
{
    return mod_file_holds(L"testonia-ui\\info.json", "Testonia (UI)");
}

bool test_the_data_mod_holds_the_dat_file_with_the_civs_techs(void)
{
    ByteBuffer plain = { unpacked, sizeof unpacked, 0U };
    ByteSpan span = { (const unsigned char *)file_text, 0U };

    if (!make_game_folder() || !build_mod(FULL_CIV, GAME)) {
        return false;
    }
    span.size = read_mod_file(MOD_DAT);
    return inflate_raw(span, &plain) && dat_map_read(unpacked, plain.size, &built_map) &&
           (built_map.tech_count == BUILT_TECHS);
}

bool test_the_data_mods_list_of_civs_names_the_civs_unique_unit(void)
{
    return mod_file_holds(MOD_LIST, "\"unique_unit_id\":6,");
}

bool test_the_data_mod_holds_the_tech_tree_file_of_the_replaced_civ(void)
{
    return mod_file_holds(MOD_TREE, "civ_techs_units");
}

bool test_the_ui_mod_names_the_civ_in_english(void)
{
    return mod_file_holds(MOD_ENGLISH, "10271 \"Testonia\"");
}

bool test_the_ui_mod_is_written_for_the_last_language_of_the_game(void)
{
    return mod_file_holds(MOD_CHINESE, "10271 \"Testonia\"");
}

bool test_a_game_folder_without_its_list_of_civs_is_reported(void)
{
    return !build_mod(FULL_CIV, L"build\\no_game") && (strstr(built.problem, "list of civs") != NULL);
}

bool test_a_data_file_that_is_not_the_games_is_reported(void)
{
    return spoiled_game_says(DAT_NAME, NO_DAT, sizeof NO_DAT - 1U, "read or unpacked");
}

bool test_a_game_without_the_civs_tech_tree_file_is_reported(void)
{
    return spoiled_game_says(LIST_NAME, LIST_OF_NOBODY, sizeof LIST_OF_NOBODY - 1U, "tech tree");
}

bool test_a_civ_with_parts_the_catalog_lacks_is_built_and_reported(void)
{
    return make_game_folder() && build_mod(BARE_CIV, GAME) && built.recipe.unit_skipped;
}
