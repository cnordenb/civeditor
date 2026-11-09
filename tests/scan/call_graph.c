#include "call_graph.h"

#include "../../src/core/check.h"
#include "../../src/core/text.h"

static bool removed[GRAPH_FUNCTION_CAPACITY];
static int open_calls[GRAPH_FUNCTION_CAPACITY];

void call_graph_start(CallGraph *graph)
{
    if (!CHECK(graph != NULL) || !CHECK(GRAPH_FUNCTION_CAPACITY > 0)) {
        return;
    }
    graph->function_count = 0;
    graph->edge_count = 0;
}

static int find_function(const CallGraph *graph, const char *name)
{
    int index;

    if (!CHECK(graph != NULL) || !CHECK(name != NULL)) {
        return -1;
    }
    for (index = 0; (index < graph->function_count) && (index < GRAPH_FUNCTION_CAPACITY); ++index) {
        if (text_equal(graph->names[index], name)) {
            return index;
        }
    }
    return -1;
}

bool call_graph_add_functions(CallGraph *graph, const ScannedSource *source)
{
    int index;

    if (!CHECK(graph != NULL) || !CHECK(source != NULL)) {
        return false;
    }
    for (index = 0; (index < source->function_count) && (index < SCAN_FUNCTION_CAPACITY); ++index) {
        if ((source->functions[index].name[0] == '\0') || (find_function(graph, source->functions[index].name) >= 0)) {
            continue;
        }
        if (!CHECK(graph->function_count < GRAPH_FUNCTION_CAPACITY)) {
            return false;
        }
        (void)text_copy(graph->names[graph->function_count], SCAN_NAME_CAPACITY, source->functions[index].name);
        ++graph->function_count;
    }
    return true;
}

static int called_name_end(const ScannedSource *source, int index, int end)
{
    int cursor;

    if (!CHECK(source != NULL) || !CHECK(index < end)) {
        return -1;
    }
    for (cursor = index; (cursor < end) && (cursor < SCAN_TEXT_CAPACITY); ++cursor) {
        if (!source_is_word_letter(source->cleaned[cursor])) {
            break;
        }
    }
    return ((cursor < end) && (source->cleaned[cursor] == '(')) ? cursor : -1;
}

static bool add_edge(CallGraph *graph, int caller, int callee)
{
    if (!CHECK(graph != NULL) || !CHECK(graph->edge_count < GRAPH_EDGE_CAPACITY)) {
        return false;
    }
    graph->callers[graph->edge_count] = caller;
    graph->callees[graph->edge_count] = callee;
    ++graph->edge_count;
    return true;
}

static bool add_calls_of(CallGraph *graph, const ScannedSource *source, const ScannedFunction *function)
{
    char name[SCAN_NAME_CAPACITY];
    int caller = find_function(graph, function->name);
    int index;
    int name_end;
    int callee;

    if (!CHECK(function != NULL) || (caller < 0)) {
        return true;
    }
    for (index = function->body_start; (index < function->body_end) && (index < SCAN_TEXT_CAPACITY); ++index) {
        if (((index > 0) && source_is_word_letter(source->cleaned[index - 1])) ||
            !source_is_word_letter(source->cleaned[index])) {
            continue;
        }
        name_end = called_name_end(source, index, function->body_end);
        if ((name_end < 0) || ((name_end - index) >= SCAN_NAME_CAPACITY)) {
            continue;
        }
        (void)text_copy_span(name, sizeof name, &source->cleaned[index], (size_t)(name_end - index));
        callee = find_function(graph, name);
        if ((callee >= 0) && !add_edge(graph, caller, callee)) {
            return false;
        }
    }
    return true;
}

bool call_graph_add_calls(CallGraph *graph, const ScannedSource *source)
{
    int index;

    if (!CHECK(graph != NULL) || !CHECK(source != NULL)) {
        return false;
    }
    for (index = 0; (index < source->function_count) && (index < SCAN_FUNCTION_CAPACITY); ++index) {
        if (!add_calls_of(graph, source, &source->functions[index])) {
            return false;
        }
    }
    return true;
}

static bool remove_finished(const CallGraph *graph)
{
    bool changed = false;
    int node;
    int edge;

    if (!CHECK(graph != NULL) || !CHECK(graph->edge_count <= GRAPH_EDGE_CAPACITY)) {
        return false;
    }
    for (node = 0; (node < graph->function_count) && (node < GRAPH_FUNCTION_CAPACITY); ++node) {
        if (removed[node] || (open_calls[node] > 0)) {
            continue;
        }
        removed[node] = true;
        changed = true;
        for (edge = 0; (edge < graph->edge_count) && (edge < GRAPH_EDGE_CAPACITY); ++edge) {
            open_calls[graph->callers[edge]] -= (graph->callees[edge] == node) ? 1 : 0;
        }
    }
    return changed;
}

bool call_graph_find_cycle(const CallGraph *graph, char *culprit, size_t capacity)
{
    int node;
    int edge;
    int round;

    if (!CHECK(graph != NULL) || !CHECK(culprit != NULL)) {
        return false;
    }
    for (node = 0; (node < graph->function_count) && (node < GRAPH_FUNCTION_CAPACITY); ++node) {
        removed[node] = false;
        open_calls[node] = 0;
    }
    for (edge = 0; (edge < graph->edge_count) && (edge < GRAPH_EDGE_CAPACITY); ++edge) {
        ++open_calls[graph->callers[edge]];
    }
    for (round = 0; (round < GRAPH_FUNCTION_CAPACITY) && remove_finished(graph); ++round) {
        culprit[0] = '\0';
    }
    for (node = 0; (node < graph->function_count) && (node < GRAPH_FUNCTION_CAPACITY); ++node) {
        if (!removed[node]) {
            return text_copy(culprit, capacity, graph->names[node]);
        }
    }
    return false;
}
