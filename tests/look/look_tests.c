#include "look_tests.h"

#include "../../src/core/check.h"
#include "../../src/look/look.h"
#include "../dat/recipe_tests.h"
#include "../dat/small_dat.h"

enum {
    TWO_CIVS = 2,
    THREE_CIVS = 3,
    ARCHITECTURE_CIV = 0,
    CASTLE_CIV = 2,
    FIRST_UNIT = 0,
    MONK = 125,
    MONK_CLASS = 18,
    TRADE_CART_CLASS = 19,
    WARSHIP_CLASS = 22,
    TASK_AT = 261,
    ARCHITECTURE_LOOK = SMALL_FIRST_GRAPHIC + ARCHITECTURE_CIV,
    OWN_LOOK = SMALL_FIRST_GRAPHIC + 1
};

static DatUnitList written_units;

static int looked(SmallUnit unit, int civ_count, const LookSources *sources, size_t field_at)
{
    const SmallUnit building = small.unit;
    DatTarget target = { &small.bytes, &small.map, &small.edit, SLOT };
    DatBytes result;
    bool built;

    if (!CHECK(sources != NULL) || !CHECK(unit.index >= 0)) {
        return -1;
    }
    small.unit = unit;
    built = small_dat_start(civ_count) && look_build(&target, sources) && small_dat_write(&result) &&
            dat_list_units(&result, small.written_map.civ_at[SLOT], &written_units);
    small.unit = building;
    return built ? (int)small.written[written_units.at[unit.index] + field_at] : -1;
}

static int look_of(int unit_class, int tasks, size_t field_at)
{
    const SmallUnit unit = { FIRST_UNIT, unit_class, SMALL_CREATABLE_TYPE, tasks };
    const LookSources sources = { ARCHITECTURE_CIV, LOOK_NONE, LOOK_NONE, { LOOK_NONE, { 0, 0, 0 } } };

    return CHECK(unit_class > 0) ? looked(unit, TWO_CIVS, &sources, field_at) : -1;
}

bool test_a_warship_takes_the_look_of_its_architectures_civ(void)
{
    return look_of(WARSHIP_CLASS, 0, DAT_UNIT_GRAPHICS_AT) == ARCHITECTURE_LOOK;
}

bool test_a_trade_carts_load_takes_the_look_of_its_architectures_civ(void)
{
    return look_of(TRADE_CART_CLASS, 1, TASK_AT + DAT_TASK_CARRYING_GRAPHIC_AT) == ARCHITECTURE_LOOK;
}

bool test_a_monks_healing_takes_the_look_of_its_monk_skin(void)
{
    const SmallUnit monk = { MONK, MONK_CLASS, SMALL_CREATABLE_TYPE, 1 };
    const LookSources sources = { LOOK_NONE, LOOK_NONE, LOOK_NONE, { ARCHITECTURE_CIV, { MONK, MONK, MONK } } };

    return looked(monk, TWO_CIVS, &sources, TASK_AT + DAT_TASK_PROCEEDING_GRAPHIC_AT) ==
           SMALL_FIRST_GRAPHIC + ARCHITECTURE_CIV;
}

bool test_a_castle_takes_the_look_of_the_civ_chosen_for_the_castle(void)
{
    const SmallUnit castle = { CASTLE, SMALL_BUILDING_CLASS, SMALL_BUILDING_TYPE, 0 };
    const LookSources sources = { ARCHITECTURE_CIV, CASTLE_CIV, LOOK_NONE, { LOOK_NONE, { 0, 0, 0 } } };

    return looked(castle, THREE_CIVS, &sources, DAT_UNIT_GRAPHICS_AT) == SMALL_FIRST_GRAPHIC + CASTLE_CIV;
}

bool test_a_soldier_keeps_the_look_of_the_replaced_civ(void)
{
    return look_of(INFANTRY_CLASS, 0, DAT_UNIT_GRAPHICS_AT) == OWN_LOOK;
}

bool test_the_look_of_an_architecture_is_the_civ_that_the_catalog_names(void)
{
    LookSources sources = { LOOK_NONE, LOOK_NONE, LOOK_NONE, { LOOK_NONE, { 0, 0, 0 } } };

    return look_sources_of(recipe_game_catalog(), recipe_game_design(FULL_CIV), &sources) &&
           (sources.architecture == ARCHITECTURE_CIV);
}
