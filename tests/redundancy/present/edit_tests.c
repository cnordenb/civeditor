#include "edit_tests.h"

#include "../../../src/core/check.h"
#include "../../../src/present/binding.h"
#include "../../dat/recipe_tests.h"

enum { BUILDING = 100, ROOT = 1, CHILD = 2, GRANDCHILD = 3, GREAT_GRANDCHILD = 4, NODES = 5 };
enum { GREAT_GRANDCHILD_NODE = 1, ROOT_NODE = 3, FIRST_ID_NOT_LISTED = 1000 };

static Catalog shuffled;
static CivDesign design;

static void add_node(int index, TreeKind kind, int id, int parent)
{
    shuffled.tree[index].kind = kind;
    shuffled.tree[index].id = id;
    shuffled.tree[index].building = (kind == TREE_BUILDINGS) ? 0 : BUILDING;
    shuffled.tree[index].parent = parent;
    (void)CHECK(civ_design_toggle_tree(&design, kind, id));
}

static void shuffle_tree(void)
{
    civ_design_reset(&design);
    add_node(0, TREE_UNITS, CHILD, ROOT);
    add_node(GREAT_GRANDCHILD_NODE, TREE_UNITS, GREAT_GRANDCHILD, GRANDCHILD);
    add_node(2, TREE_UNITS, GRANDCHILD, CHILD);
    add_node(ROOT_NODE, TREE_UNITS, ROOT, CATALOG_MISSING);
    add_node(NODES - 1, TREE_BUILDINGS, BUILDING, CATALOG_MISSING);
    shuffled.tree_count = NODES;
}

static Command click(int node)
{
    const ListItem item = { CONTROL_TREE_LIST, node };

    return list_item_command(&shuffled, &design, item);
}

static int ticked_after_click(int node, int first_id, int last_id)
{
    Command command = click(node);
    int ticked = 0;
    int id;

    (void)CHECK(command_apply(&design, &command));
    for (id = first_id; id <= last_id; ++id) {
        ticked += civ_design_tree_has(&design, TREE_UNITS, id) ? 1 : 0;
    }
    return ticked;
}

static int descendants_left(void)
{
    shuffle_tree();
    return ticked_after_click(ROOT_NODE, CHILD, GREAT_GRANDCHILD);
}

static int needs_ticked(void)
{
    shuffle_tree();
    civ_design_reset(&design);
    return ticked_after_click(GREAT_GRANDCHILD_NODE, ROOT, GRANDCHILD);
}

static bool is_refused(Command command)
{
    return !command_apply(&design, &command);
}

static bool counts_no_times(void)
{
    civ_design_reset(&design);
    return civ_design_multiplier(&design, TECH_BONUS).millionths == 0;
}

static bool full_tree_refuses(void)
{
    int id;

    shuffle_tree();
    civ_design_reset(&design);
    for (id = FIRST_ID_NOT_LISTED; id < FIRST_ID_NOT_LISTED + CIV_TREE_CAPACITY; ++id) {
        (void)CHECK(civ_design_toggle_tree(&design, TREE_UNITS, id));
    }
    return is_refused(click(GREAT_GRANDCHILD_NODE));
}

TestOutcome edit_tests_run(int index)
{
    switch (index) {
    case 1:
        return test_outcome_of("unticking a node unticks the descendants listed before it", descendants_left() == 0);
    case 2:
        return test_outcome_of("a civ bonus not ticked counts no times", counts_no_times());
    case 3:
        return test_outcome_of("the report counts no multiplier of a bonus of civ techs or of a bonus counted once",
                               recipe_game_report(MULTIPLIED_CIV).multipliers_skipped == 0);
    case 4:
        return test_outcome_of("ticking a node ticks the needs listed before it",
                               needs_ticked() == (GRANDCHILD - ROOT) + 1);
    case 5:
        return test_outcome_of("ticking a node into a full tree changes nothing", full_tree_refuses());
    default:
        return (index <= EDIT_OWN_COUNT + NUMBER_EDIT_TEST_COUNT) ? number_edit_tests_run(index - EDIT_OWN_COUNT)
                                                                  : stat_tests_run(index - EDIT_OWN_COUNT -
                                                                                   NUMBER_EDIT_TEST_COUNT);
    }
}
