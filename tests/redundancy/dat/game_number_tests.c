#include "game_number_tests.h"

#include <string.h>

#include "../../../src/forge/forge.h"
#include "../../../src/gamedata/game_numbers.h"
#include "../../mod/game_folder.h"
#include "../../dat/recipe_tests.h"
#include "../../dat/small_dat.h"

enum {
    OLD_ENTRY = 0,
    OWN_ENTRY = 1,
    CASTLE_ENTRY = 0,
    IMPERIAL_ENTRY = 1,
    STALE = 7777,
    NO_GAME_TECH = 99,
    LACKED_NODE = 0,
    KEPT_NODE = 1,
    OTHER_SLOT = 2,
    GAME_WOOD = 150
};

static const wchar_t GAME[] = L"build\\test_game\\steamapps\\common\\AoE2DE";
static const wchar_t NO_GAME[] = L"build\\no_game";
static const wchar_t BRITONS_TREE[] =
    L"build\\test_game\\steamapps\\common\\AoE2DE\\resources\\_common\\dat\\CivTechTrees\\BRITONS.json";
static const char GONE[] = "{\"civ_techs_buildings\":[],\"civ_techs_units\":[{\"Use Type\":\"Unit\",\"Node ID\":9,"
                           "\"Node Status\":\"NotAvailable\"}]}";
static const uint64_t EVERY_CIV = ~(uint64_t)0U;
static const float REAL_RANGE = 9.0F;
static const float SHOWN_RANGE = 5.0F;

static Catalog catalog;
static unsigned char costly[SMALL_DAT_CAPACITY];
static DatMap costly_map;

static void make_stale(void)
{
    int stat;
    int resource;

    for (stat = 0; stat < (int)UNIT_STAT_COUNT; ++stat) {
        catalog.units[OLD_ENTRY].stats[stat] = STALE;
        catalog.units[OWN_ENTRY].stats[stat] = STALE;
    }
    for (resource = 0; resource < RESOURCE_KIND_COUNT; ++resource) {
        catalog.techs[CASTLE_ENTRY].cost[resource] = STALE;
        catalog.techs[IMPERIAL_ENTRY].cost[resource] = STALE;
    }
    catalog.techs[CASTLE_ENTRY].time = STALE;
    catalog.techs[IMPERIAL_ENTRY].time = STALE;
}

static bool numbers_read(UnitRole role, int make_tech, int game_techs)
{
    DatBytes game;
    bool made;

    small.elite = ELITE_UNIT;
    made = recipe_game_make(&game);
    small.elite = 0;
    catalog = *recipe_game_catalog();
    make_stale();
    catalog.units[OWN_ENTRY].role = role;
    catalog.units[OWN_ENTRY].make_tech = make_tech;
    catalog.techs[IMPERIAL_ENTRY].effects.values[0] = NO_GAME_TECH;
    catalog.techs[IMPERIAL_ENTRY].effects.count = game_techs;
    if (made) {
        game_numbers_read(&catalog, &game, &small.written_map);
    }
    return made;
}

static int unit_stat(UnitStat stat)
{
    return numbers_read(UNIT_ROLE_VANILLA, OWN_MAKE_TECH, 1) ? catalog.units[OWN_ENTRY].stats[stat] : STALE;
}

static int stat_of(UnitRole role, int make_tech)
{
    return numbers_read(role, make_tech, 1) ? catalog.units[OWN_ENTRY].stats[UNIT_STAT_HP] : 0;
}

static const TechEntry *tech_read(int entry, int game_techs)
{
    (void)numbers_read(UNIT_ROLE_VANILLA, OWN_MAKE_TECH, game_techs);
    return &catalog.techs[entry];
}

static int time_from_folder(const wchar_t *folder)
{
    bool made = game_folder_is_made();

    catalog = *recipe_game_catalog();
    make_stale();
    return (made && forge_read_game(&catalog, folder)) ? catalog.techs[CASTLE_ENTRY].time : STALE;
}

static int shown_range(void)
{
    DatBytes game;
    DatUnit unit;
    size_t at = 0U;
    bool made;

    small.elite = ELITE_UNIT;
    made = recipe_game_make(&game) && dat_find_unit(&game, small.written_map.civ_at[0], OWN_UNIT, &at) &&
           dat_unit_read(&game, at, &unit);
    small.elite = 0;
    if (!made) {
        return STALE;
    }
    (void)memcpy(&small.written[unit.combat_at + DAT_COMBAT_RANGE_AT], &REAL_RANGE, sizeof REAL_RANGE);
    (void)memcpy(&small.written[unit.combat_at + DAT_COMBAT_SHOWN_RANGE_AT], &SHOWN_RANGE, sizeof SHOWN_RANGE);
    catalog = *recipe_game_catalog();
    game_numbers_read(&catalog, &game, &small.written_map);
    return catalog.units[OWN_ENTRY].stats[UNIT_STAT_RANGE];
}

static uint64_t civs_read(int entry, uint64_t before, const char *tree)
{
    bool made = game_folder_is_made() && ((tree == NULL) || file_write_all(BRITONS_TREE, tree, strlen(tree)));

    catalog = *recipe_game_catalog();
    catalog.appearance[catalog.appearance_count] = catalog.appearance[0];
    catalog.appearance[catalog.appearance_count].id = OTHER_SLOT;
    ++catalog.appearance_count;
    catalog.tree[entry].civs = before;
    return (made && forge_read_game(&catalog, GAME)) ? catalog.tree[entry].civs : ~before;
}

static bool has(uint64_t civs, int slot)
{
    return ((civs >> (unsigned)slot) & 1U) != 0U;
}

static int costly_tech(int resource)
{
    DatOutput output = { costly, sizeof costly, 0U };
    DatBytes made = { costly, 0U };
    DatBytes game;
    DatTech tech = { 0 };
    bool fine;

    fine = recipe_game_make(&game) && dat_tech_read(&game, small.written_map.tech_at[CASTLE_SOURCE], &tech) &&
           dat_edit_start(&small.edit, &small.written_map);
    tech.costs[0] = (DatTechCost){ WOOD, GAME_WOOD, 1 };
    catalog = *recipe_game_catalog();
    make_stale();
    catalog.techs[CASTLE_ENTRY].effects.values[0] = fine ? dat_edit_add_tech(&small.edit, &tech) : NO_GAME_TECH;
    fine = fine && dat_edit_write(&small.edit, &game, &output) && dat_map_read(costly, output.length, &costly_map);
    made.size = output.length;
    if (fine) {
        game_numbers_read(&catalog, &made, &costly_map);
    }
    return catalog.techs[CASTLE_ENTRY].cost[resource];
}

static int first_unit_hit_points(void)
{
    DatBytes game;
    bool made;

    small.elite = ELITE_UNIT;
    made = recipe_game_make(&game);
    small.elite = 0;
    catalog = *recipe_game_catalog();
    make_stale();
    catalog.units[OLD_ENTRY] = catalog.units[OWN_ENTRY];
    if (made) {
        game_numbers_read(&catalog, &game, &small.written_map);
    }
    return catalog.units[OLD_ENTRY].stats[UNIT_STAT_HP];
}

TestOutcome game_number_tests_run(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("the cost of a unique unit comes from the game's data file",
                               unit_stat(UNIT_STAT_FOOD) == SMALL_FOOD);
    case 2:
        return test_outcome_of("a resource that a unique unit does not cost in the game's data file is 0",
                               unit_stat(UNIT_STAT_WOOD) == 0);
    case 3:
        return test_outcome_of("the attack of a unique unit comes from the game's data file",
                               unit_stat(UNIT_STAT_ATTACK) == SMALL_ATTACK);
    case 4:
        return test_outcome_of("a unique unit that the game's data file lacks keeps the catalog's numbers",
                               stat_of(UNIT_ROLE_VANILLA, LACKED_TECH) == STALE);
    case 5:
        return test_outcome_of("a custom unit keeps the catalog's numbers",
                               stat_of(UNIT_ROLE_CUSTOM, OWN_MAKE_TECH) == STALE);
    case 6:
        return test_outcome_of("the cost of a unique tech comes from the game's data file",
                               costly_tech(WOOD) == GAME_WOOD);
    case 7:
        return test_outcome_of("the research time of a unique tech comes from the game's data file",
                               tech_read(CASTLE_ENTRY, 1)->time == GAME_TIME);
    case 8:
        return test_outcome_of("a unique tech whose game tech the data file lacks keeps the catalog's numbers",
                               tech_read(IMPERIAL_ENTRY, 1)->time == STALE);
    case 9:
        return test_outcome_of("a unique tech of no game tech keeps the catalog's numbers",
                               tech_read(IMPERIAL_ENTRY, 0)->cost[GOLD] == STALE);
    case 10:
        return test_outcome_of("the numbers are read from the data file in the game's folder",
                               time_from_folder(GAME) == GAME_TIME);
    case 11:
        return test_outcome_of("a folder without the game's data file leaves the catalog's numbers",
                               time_from_folder(NO_GAME) == STALE);
    case 12:
        return test_outcome_of("the range of a unique unit comes as the game shows it", shown_range() == 5);
    case 13:
        return test_outcome_of("a node that a civ's tech tree file of the game has is had by that civ",
                               has(civs_read(LACKED_NODE, 0U, NULL), SLOT));
    case 14:
        return test_outcome_of("a node that a civ's tech tree file of the game lacks is not had by that civ",
                               !has(civs_read(KEPT_NODE, EVERY_CIV, NULL), SLOT));
    case 15:
        return test_outcome_of("a node not available in a civ's tech tree file is not had by that civ",
                               !has(civs_read(LACKED_NODE, EVERY_CIV, GONE), SLOT));
    case 16:
        return test_outcome_of("a civ without a tech tree file in the game keeps the nodes of the catalog",
                               has(civs_read(LACKED_NODE, EVERY_CIV, NULL), OTHER_SLOT));
    case 17:
        return test_outcome_of("a resource that a unique tech does not cost in the game's data file is 0",
                               costly_tech(GOLD) == 0);
    case 18:
        return test_outcome_of("the first unique unit of the catalog is read from the game's data file",
                               first_unit_hit_points() != STALE);
    default:
        return test_outcome_missing();
    }
}
