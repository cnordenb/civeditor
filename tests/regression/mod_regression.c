#include "mod_regression.h"

#include <string.h>

#include "../../src/core/check.h"
#include "../../src/data/catalog.h"
#include "../../src/mod/mod_strings.h"
#include "../dat/recipe_tests.h"
#include "../mod/mod_tests.h"

enum { LIST_SLOT = 1, OWN_DESCRIPTION_SID = 990101, STABLE = 101, BARRACKS_NODE = 12, LOOK_LINES = 2 };

static const char VANILLA[] = "120201 \"Khitans\"\r\n120200 \"Jurchens\"\r\n120150 \"Britons\"\r\n";
static const char HERO_LINE[] = "\n44500 \"Testhero\"\n";
static const char DESCRIBED_LIST[] = "{\"civilization_list\":[{},{\"tech_tree_description_string_id\":120150}]}";
static const char OWN_DESCRIPTION[] = "{\"civilization_list\":[{},{\"tech_tree_description_string_id\":990101}]}";
static const char OPEN_TREE[] = "{\"civ_techs_buildings\":[{\"Use Type\":\"Building\",\"Node ID\":101,"
                                "\"Node Status\":\"ResearchedCompleted\"}],\"civ_techs_units\":[]}";
static const char LOCKED_TREE[] = "{\"civ_techs_buildings\":[{\"Use Type\":\"Building\",\"Node ID\":101,"
                                  "\"Node Status\":\"NotAvailable\"}],\"civ_techs_units\":[]}";

static const CivListEntry DESCRIBED_ENTRY = { LIST_SLOT,     CIV_LIST_KEEP, CIV_LIST_KEEP, CIV_LIST_KEEP,
                                              CIV_LIST_KEEP, false,         CIV_LIST_KEEP, CIV_LIST_KEEP,
                                              CIV_LIST_KEEP, CIV_LIST_KEEP, OWN_DESCRIPTION_SID };

static ModStrings strings;
static Catalog catalog;

bool test_a_bonus_of_the_games_techs_taken_twice_doubles_the_changes_of_its_copy(void)
{
    float amount = recipe_game_scenario_amount(MULTIPLIED_CIV, NEW_BONUS_TECH);

    return (amount > 9.5F) && (amount < 10.5F);
}

bool test_the_civ_descriptions_are_written_in_id_order(void)
{
    ModCiv civ = mod_tests_named_civ();
    const char *jurchens;
    const char *khitans;

    if (!CHECK(mod_strings_write(&strings, &civ, 1, VANILLA))) {
        return false;
    }
    jurchens = strstr(strings.text, "\n120200 ");
    khitans = strstr(strings.text, "\n120201 ");
    return (jurchens != NULL) && (khitans != NULL) && (jurchens < khitans);
}

bool test_a_skipped_heros_name_is_not_written(void)
{
    return mod_tests_write_hero_strings(true) && (strstr(mod_tests_written_strings(), HERO_LINE) == NULL);
}

bool test_the_civ_list_points_at_the_civs_own_description(void)
{
    return mod_tests_list_written_as(DESCRIBED_LIST, &DESCRIBED_ENTRY, OWN_DESCRIPTION);
}

bool test_a_building_the_civ_lacks_is_not_available_in_its_tech_tree(void)
{
    return mod_tests_tree_written_as(OPEN_TREE, BARRACKS_NODE, LOCKED_TREE);
}

bool test_a_building_the_civ_has_is_available_in_its_tech_tree(void)
{
    return mod_tests_tree_written_as(LOCKED_TREE, STABLE, OPEN_TREE);
}

bool test_the_catalog_holds_every_look_line_of_its_file(void)
{
    return CHECK(catalog_load(&catalog, L"data")) && CHECK(catalog_load(&catalog, L"data")) &&
           (catalog.unit_look_count == LOOK_LINES);
}
