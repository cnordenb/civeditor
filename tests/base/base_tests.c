#include "base_tests.h"

#include <stdint.h>

#include "../../src/base/base_civ.h"

enum { BARRACKS = 12, STALE_NODE = 99, REGIONAL_UNIT = 5, BASE_CIV = 1, BASE_BONUS = 3 };
enum { BASE_TEAM_ID = 0, BASE_UNIT_ID = 0 };

static Catalog bases;
static CivDesign based;

static void make_bases(void)
{
    BaseCiv base = { BASE_CIV, { { BASE_BONUS }, 1 }, { BASE_TEAM_ID, BASE_UNIT_ID, 0, 0, 0, 0, 0 } };

    bases.tree[0].kind = TREE_BUILDINGS;
    bases.tree[0].id = BARRACKS;
    bases.tree[0].universal = true;
    bases.tree[0].civs = 0U;
    bases.tree[1].kind = TREE_UNITS;
    bases.tree[1].id = REGIONAL_UNIT;
    bases.tree[1].universal = false;
    bases.tree[1].civs = (uint64_t)1U << (unsigned)BASE_CIV;
    bases.tree[2] = bases.tree[0];
    bases.tree[2].id = STALE_NODE;
    bases.tree_count = 2;
    bases.bases[0].id = CIV_GENERIC_BASE;
    bases.bases[1] = base;
    bases.base_count = 2;
    civ_design_reset(&based);
}

static bool apply_base(int base_id)
{
    make_bases();
    return base_civ_apply(&based, &bases, base_id);
}

bool test_the_generic_base_civ_has_exactly_the_universal_nodes(void)
{
    return apply_base(CIV_GENERIC_BASE) && (based.tree[TREE_BUILDINGS].count == 1) &&
           civ_design_tree_has(&based, TREE_BUILDINGS, BARRACKS);
}

bool test_choosing_generic_takes_away_a_regional_node(void)
{
    (void)apply_base(BASE_CIV);
    return base_civ_apply(&based, &bases, CIV_GENERIC_BASE) && !civ_design_tree_has(&based, TREE_UNITS, REGIONAL_UNIT);
}

bool test_a_base_civ_has_the_nodes_its_civ_has_in_the_game(void)
{
    return apply_base(BASE_CIV) && civ_design_tree_has(&based, TREE_UNITS, REGIONAL_UNIT);
}

bool test_a_base_civ_gives_the_civ_exactly_its_bonuses(void)
{
    return apply_base(BASE_CIV) && (based.bonus_count == 1) && civ_design_has_bonus(&based, BASE_BONUS);
}

bool test_a_base_civ_gives_the_civ_its_team_bonus(void)
{
    return apply_base(BASE_CIV) && civ_design_has_team_bonus(&based, BASE_TEAM_ID);
}

bool test_a_base_civ_gives_the_civ_its_unique_unit(void)
{
    return apply_base(BASE_CIV) && (based.unique_unit.origin == UNIT_ORIGIN_VANILLA) &&
           (based.unique_unit.id == BASE_UNIT_ID);
}
