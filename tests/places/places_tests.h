#ifndef PLACES_TESTS_H
#define PLACES_TESTS_H

#include <stdbool.h>

bool test_the_game_is_found_in_the_folder_of_steam(void);
bool test_the_game_is_found_in_a_library_that_steam_lists(void);
bool test_the_game_is_not_found_where_no_library_holds_it(void);
bool test_the_local_mods_folder_of_a_profile_is_found(void);
bool test_a_home_without_a_profile_of_the_game_has_no_mods_folder(void);

#endif
