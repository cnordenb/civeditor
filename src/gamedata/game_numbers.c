#include "game_numbers.h"

#include "../forge/recipe_parts.h"
#include "../unique/unique_aim.h"

static int civ_of(const RecipeInput *input, int make_tech)
{
    const DatMap *map = input->map;
    DatTech tech;

    if (!recipe_read_tech(input, make_tech, &tech) || (tech.civ < 0) || (tech.civ >= map->civ_count)) {
        return 0;
    }
    return tech.civ;
}

static void read_unit(const RecipeInput *input, UnitEntry *entry)
{
    DatTarget target = { input->dat, input->map, NULL, 0 };
    UniqueUnit unit;

    if (entry->role != UNIT_ROLE_VANILLA) {
        return;
    }
    unit = unique_unit_of_techs(input, entry->make_tech, entry->elite_tech);
    target.civ = civ_of(input, entry->make_tech);
    (void)unique_stats_read(&target, unit.base, entry->stats);
}

static void read_tech(const RecipeInput *input, TechEntry *entry)
{
    DatTech tech;
    int slot;

    if ((entry->effects.count < 1) || !recipe_read_tech(input, entry->effects.values[0], &tech)) {
        return;
    }
    for (slot = 0; slot < RESOURCE_KIND_COUNT; ++slot) {
        entry->cost[slot] = 0;
    }
    for (slot = 0; slot < DAT_TECH_COST_SLOTS; ++slot) {
        if ((tech.costs[slot].type >= 0) && (tech.costs[slot].type < RESOURCE_KIND_COUNT)) {
            entry->cost[tech.costs[slot].type] = tech.costs[slot].amount;
        }
    }
    if (tech.research_location_count > 0) {
        entry->time = tech.research_locations[0].research_time;
    }
}

void game_numbers_read(Catalog *catalog, const DatBytes *dat, const DatMap *map)
{
    RecipeInput input = { NULL, catalog, dat, map, 0 };
    int entry;

    for (entry = 0; (entry < catalog->unit_count) && (entry < CATALOG_UNIT_CAPACITY); ++entry) {
        read_unit(&input, &catalog->units[entry]);
    }
    for (entry = 0; (entry < catalog->tech_count) && (entry < CATALOG_TECH_CAPACITY); ++entry) {
        read_tech(&input, &catalog->techs[entry]);
    }
}
