#include "catalog_tests.h"

#include "../../src/core/check.h"
#include "../../src/core/text.h"
#include "../../src/data/catalog.h"

enum { BONUS_LINES = 394, TEAM_BONUS_LINES = 80, UNIT_LINES = 146, TECH_LINES = 120, TREE_LINES = 205 };
enum { LONGBOWMAN_SPEED = 96 };
enum { REQUIREMENT_LINES = 15, CAVALIER_LINE = 3, CAVALIER_BONUS = 103, CAVALIER = 209, CAVALIER_REQUIREMENT = 768 };
enum { COW_BONUS = 222, COW = 705, MILL = 68, COW_LINE = 173, BUILDING_IN_TRAIN_LOCATION = 2 };
enum { CASTLE_BONUS = 460, CASTLE = 82, STONE_LINE = 178, STONE_AMOUNT_AT = 2, STONE = 300 };
enum { PALINTONON_BONUS = 457, TREBUCHET = 42, ATHENIANS = 47, LOOK_LINES = 2 };

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

bool test_the_catalog_holds_every_requirement_line_of_its_file(void)
{
    return loaded()->requirement_count == REQUIREMENT_LINES;
}

bool test_a_requirement_line_names_its_bonus_its_tech_and_the_tech_required(void)
{
    BonusRequirement line = loaded()->requirements[CAVALIER_LINE];

    return (line.bonus_id == CAVALIER_BONUS) && (line.tech == CAVALIER) && (line.requirement == CAVALIER_REQUIREMENT);
}

bool test_a_train_line_is_a_unit_patch_placed_in_the_train_location(void)
{
    UnitPatch line = loaded()->unit_patches[COW_LINE];

    return (line.part == UNIT_PART_TRAIN_LOCATION) && (line.bonus_id == COW_BONUS) && (line.unit == COW) &&
           (line.offset == BUILDING_IN_TRAIN_LOCATION) && ((int)line.value == MILL);
}

bool test_a_cost_line_is_a_unit_patch_placed_in_the_costs(void)
{
    UnitPatch line = loaded()->unit_patches[STONE_LINE];

    return (line.part == UNIT_PART_COSTS) && (line.bonus_id == CASTLE_BONUS) && (line.unit == CASTLE) &&
           (line.offset == STONE_AMOUNT_AT) && ((int)line.value == STONE);
}

bool test_a_look_line_names_its_bonus_its_unit_and_the_civ_of_the_look(void)
{
    UnitLook line = loaded()->unit_looks[0];

    return (line.bonus_id == PALINTONON_BONUS) && (line.unit == TREBUCHET) && (line.from_civ == ATHENIANS);
}

bool test_the_catalog_holds_every_look_line_of_its_file(void)
{
    return loaded()->unit_look_count == LOOK_LINES;
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
