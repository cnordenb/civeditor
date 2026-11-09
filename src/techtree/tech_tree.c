#include "tech_tree.h"

#include "../core/check.h"
#include "../core/text.h"

enum { NAME_CAPACITY = 32, NODE_LIST_COUNT = 2, NODE_LIMIT = 4096 };

static const char USE_TYPES[TREE_KIND_COUNT][NAME_CAPACITY] = { "Unit", "Building", "Tech" };
static const char NODE_LISTS[NODE_LIST_COUNT][NAME_CAPACITY] = { "civ_techs_buildings", "civ_techs_units" };
static JsonPatch patch;

static bool string_is(const JsonDocument *tree, int token, const char *expected)
{
    char text[NAME_CAPACITY] = "";

    if (!CHECK(tree != NULL) || !CHECK(expected != NULL) || (token == JSON_NONE)) {
        return false;
    }
    return json_copy_string(tree, token, text, sizeof text) && text_equal(text, expected);
}

static int node_kind(const JsonDocument *tree, int node)
{
    int use;
    int kind;

    if (!CHECK(tree != NULL) || !CHECK(node != JSON_NONE)) {
        return JSON_NONE;
    }
    use = json_member(tree, node, "Use Type");
    for (kind = 0; kind < (int)TREE_KIND_COUNT; ++kind) {
        if (string_is(tree, use, USE_TYPES[kind])) {
            return kind;
        }
    }
    return JSON_NONE;
}

static void set_status(const TechTreeJob *job, int node)
{
    const JsonDocument *tree;
    int kind;
    int status;
    TextNumber id;

    if (!CHECK(job != NULL) || !CHECK(job->tree != NULL)) {
        return;
    }
    tree = job->tree;
    kind = node_kind(tree, node);
    id = json_number(tree, json_member(tree, node, "Node ID"));
    status = json_member(tree, node, "Node Status");
    if ((kind == JSON_NONE) || !id.valid || (status == JSON_NONE) ||
        (catalog_find_tree(job->catalog, (TreeKind)kind, (int)id.value) == CATALOG_MISSING)) {
        return;
    }
    if (!civ_design_tree_keeps(job->design, (TreeKind)kind, (int)id.value)) {
        json_patch_replace(&patch, status, "\"NotAvailable\"");
    } else if (civ_design_tree_has(job->design, (TreeKind)kind, (int)id.value) &&
               string_is(tree, status, "NotAvailable")) {
        json_patch_replace(&patch, status, "\"ResearchedCompleted\"");
    }
}

static void set_statuses(const TechTreeJob *job, int list)
{
    const JsonDocument *tree;
    int node;
    int index;

    if (!CHECK(job != NULL) || !CHECK(job->tree != NULL)) {
        return;
    }
    tree = job->tree;
    node = json_is_kind(tree, list, JSON_KIND_ARRAY) ? json_first_child(tree, list) : JSON_NONE;
    for (index = 0; (index < NODE_LIMIT) && (node != JSON_NONE); ++index) {
        set_status(job, node);
        node = json_next_sibling(tree, list, node);
    }
}

bool tech_tree_write(const TechTreeJob *job, JsonPatchOutput *output)
{
    int list;

    if (!CHECK(job != NULL) || !CHECK(output != NULL) || !CHECK(job->tree != NULL)) {
        return false;
    }
    json_patch_start(&patch, job->tree);
    for (list = 0; (list < NODE_LIST_COUNT) && !civ_design_tree_is_empty(job->design); ++list) {
        set_statuses(job, json_member(job->tree, 0, NODE_LISTS[list]));
    }
    return json_patch_write(&patch, output);
}
