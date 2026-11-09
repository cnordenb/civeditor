#ifndef CALL_GRAPH_H
#define CALL_GRAPH_H

#include <stdbool.h>
#include <stddef.h>

#include "source_scan.h"

enum { GRAPH_FUNCTION_CAPACITY = 1600, GRAPH_EDGE_CAPACITY = 16000 };

typedef struct {
    char names[GRAPH_FUNCTION_CAPACITY][SCAN_NAME_CAPACITY];
    int function_count;
    int callers[GRAPH_EDGE_CAPACITY];
    int callees[GRAPH_EDGE_CAPACITY];
    int edge_count;
} CallGraph;

void call_graph_start(CallGraph *graph);
bool call_graph_add_functions(CallGraph *graph, const ScannedSource *source);
bool call_graph_add_calls(CallGraph *graph, const ScannedSource *source);
bool call_graph_find_cycle(const CallGraph *graph, char *culprit, size_t capacity);

#endif
