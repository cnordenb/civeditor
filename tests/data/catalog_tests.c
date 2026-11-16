#include "catalog_tests.h"

#include "../../src/core/check.h"
#include "../../src/core/text.h"
#include "../../src/data/catalog.h"

enum { BONUS_LINES = 394, TEAM_BONUS_LINES = 80, UNIT_LINES = 146, TECH_LINES = 120, TREE_LINES = 205 };
enum { LONGBOWMAN_SPEED = 96 };

static Catalog catalog;

static const Catalog *loaded(void)
{
    (void)CHECK(catalog_load(&catalog, L"data"));
    return &catalog;
}

bool test_the_catalog_holds_every_civ_bonus_of_its_file(void)
{
    return loaded()->bonus_count == BONUS_LINES;
}

bool test_the_catalog_holds_every_team_bonus_of_its_file(void)
{
    return loaded()->team_bonus_count == TEAM_BONUS_LINES;
}

bool test_the_catalog_holds_every_unit_of_its_file(void)
{
    return loaded()->unit_count == UNIT_LINES;
}

bool test_the_catalog_holds_every_tech_of_its_file(void)
{
    return loaded()->tech_count == TECH_LINES;
}

bool test_the_catalog_holds_every_node_of_its_file(void)
{
    return loaded()->tree_count == TREE_LINES;
}

bool test_the_first_unit_of_the_catalog_has_its_name(void)
{
    return text_equal(loaded()->units[0].name, "Longbowman");
}

bool test_a_units_speed_is_kept_in_hundredths(void)
{
    return loaded()->units[0].stats[UNIT_STAT_SPEED] == LONGBOWMAN_SPEED;
}

bool test_the_first_unit_of_the_catalog_is_a_unit_of_the_game(void)
{
    return loaded()->units[0].role == UNIT_ROLE_VANILLA;
}

bool test_a_catalog_folder_that_is_missing_is_not_loaded(void)
{
    return !catalog_load(&catalog, L"build\\no_data");
}
