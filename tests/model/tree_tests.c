#include "present_tests.h"

#include "../../src/base/base_civ.h"
#include "../../src/core/check.h"
#include "../../src/model/history.h"
#include "../../src/present/binding.h"

enum {
    BRITONS = 1,
    ARCHER = 4,
    SKIRMISHER = 7,
    ARCHERY_RANGE = 87,
    IRON_CASTING = 68,
    BLAST_FURNACE = 75,
    OUTPOST = 598,
    WATCH_TOWER = 79,
    MILL = 68,
    FARM = 50
};

static Catalog catalog;
static History history;

static bool britons_ready(void)
{
    static CivDesign britons;
    static bool loaded = false;

    loaded = loaded || CHECK(catalog_load(&catalog, L"data"));
    history_start(&history);
    return loaded && CHECK(base_civ_apply(&britons, &catalog, BRITONS)) && history_replace(&history, &britons);
}

static bool untick(TreeKind kind, int node_id)
{
    ListItem item = { CONTROL_TREE_LIST, CATALOG_MISSING };
    Command command;

    if (!britons_ready()) {
        return false;
    }
    item.item = catalog_find(&catalog, CATALOG_TREE, (int)kind, node_id);
    if (!CHECK(item.item != CATALOG_MISSING) || !CHECK(civ_design_tree_has(&history.present, kind, node_id))) {
        return false;
    }
    command = list_item_command(&catalog, &history.present, item);
    return history_do(&history, &command);
}

static bool has_after(TreeKind kind, int node_id, TreeKind unticked_kind, int unticked_id)
{
    return untick(unticked_kind, unticked_id) && civ_design_tree_has(&history.present, kind, node_id);
}

bool test_unticking_the_archer_leaves_the_skirmisher(void)
{
    return has_after(TREE_UNITS, SKIRMISHER, TREE_UNITS, ARCHER);
}

bool test_unticking_iron_casting_unticks_blast_furnace(void)
{
    return untick(TREE_TECHS, IRON_CASTING) && !civ_design_tree_has(&history.present, TREE_TECHS, BLAST_FURNACE);
}

bool test_unticking_the_outpost_unticks_the_watch_tower(void)
{
    return untick(TREE_BUILDINGS, OUTPOST) && !civ_design_tree_has(&history.present, TREE_BUILDINGS, WATCH_TOWER);
}

bool test_unticking_the_archery_range_unticks_the_archer(void)
{
    return untick(TREE_BUILDINGS, ARCHERY_RANGE) && !civ_design_tree_has(&history.present, TREE_UNITS, ARCHER);
}

bool test_unticking_the_mill_which_every_civ_keeps_leaves_the_farm(void)
{
    return has_after(TREE_BUILDINGS, FARM, TREE_BUILDINGS, MILL);
}
