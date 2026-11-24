#include "binding.h"

#include <string.h>

static CivDesign after;
static bool marked[CATALOG_TREE_CAPACITY];

static bool is_always_kept(const Catalog *catalog, int node)
{
    return civ_design_always_keeps(catalog->tree[node].kind, catalog->tree[node].id);
}

static bool is_marked(int node)
{
    return (node != CATALOG_MISSING) && marked[node];
}

static bool mark(int node)
{
    if ((node == CATALOG_MISSING) || marked[node]) {
        return false;
    }
    marked[node] = true;
    return true;
}

static bool mark_needs(const Catalog *catalog)
{
    TreeNeeds needs;
    bool marked_one = false;
    int node;

    for (node = 0; node < catalog->tree_count; ++node) {
        if (marked[node]) {
            needs = catalog_tree_needs(catalog, node);
            marked_one = mark(needs.parent) || marked_one;
            marked_one = mark(needs.building) || marked_one;
        }
    }
    return marked_one;
}

static bool mark_needers(const Catalog *catalog)
{
    TreeNeeds needs;
    bool marked_one = false;
    int node;

    for (node = 0; node < catalog->tree_count; ++node) {
        needs = catalog_tree_needs(catalog, node);
        if (!is_always_kept(catalog, node) && (is_marked(needs.parent) || is_marked(needs.building))) {
            marked_one = mark(node) || marked_one;
        }
    }
    return marked_one;
}

static void mark_from(const Catalog *catalog, int root, bool ticking)
{
    bool marked_one = ticking || !is_always_kept(catalog, root);

    (void)memset(marked, 0, sizeof marked);
    marked[root] = true;
    while (marked_one) {
        marked_one = ticking ? mark_needs(catalog) : mark_needers(catalog);
    }
}

static bool set_marked(const Catalog *catalog, bool ticking)
{
    const TreeEntry *entry;
    bool fits = true;
    int node;

    for (node = 0; node < catalog->tree_count; ++node) {
        entry = &catalog->tree[node];
        if (marked[node] && (civ_design_tree_has(&after, entry->kind, entry->id) != ticking)) {
            fits = civ_design_toggle_tree(&after, entry->kind, entry->id) && fits;
        }
    }
    return fits;
}

Command tree_command(const Catalog *catalog, const CivDesign *design, int node)
{
    const TreeEntry *entry = &catalog->tree[node];
    bool ticking = !civ_design_tree_has(design, entry->kind, entry->id);

    after = *design;
    mark_from(catalog, node, ticking);
    return set_marked(catalog, ticking) ? command_set_tree(&after) : command_nothing();
}
