#include "recursion_rule.h"

#include "../../../src/core/check.h"
#include "../../scan/call_graph.h"
#include "../../scan/repo_files.h"
#include "../../scan/source_scan.h"
#include "../../test_outcome.h"

static RepoFiles files;
static ScannedSource source;
static CallGraph graph;

static bool add_to_graph(bool calls)
{
    int index;

    if (!CHECK(files.count > 0) || !CHECK(files.count <= REPO_FILE_CAPACITY)) {
        return false;
    }
    for (index = 0; (index < files.count) && (index < REPO_FILE_CAPACITY); ++index) {
        if (!repo_path_is_code(files.paths[index]) || !repo_path_has_suffix(files.paths[index], ".c")) {
            continue;
        }
        if (!source_scan(&source, files.paths[index])) {
            return false;
        }
        if (calls ? !call_graph_add_calls(&graph, &source) : !call_graph_add_functions(&graph, &source)) {
            return false;
        }
    }
    return true;
}

bool code_has_no_recursion(void)
{
    char culprit[SCAN_NAME_CAPACITY] = "";

    if (!repo_files_list(&files) || !CHECK(files.count > 0)) {
        return false;
    }
    call_graph_start(&graph);
    if (!add_to_graph(false) || !add_to_graph(true)) {
        return false;
    }
    if (call_graph_find_cycle(&graph, culprit, sizeof culprit)) {
        test_note(culprit);
        return false;
    }
    return true;
}
