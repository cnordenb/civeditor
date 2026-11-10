#ifndef HISTORY_H
#define HISTORY_H

#include <stdbool.h>

#include "civ_design.h"
#include "command.h"

enum { HISTORY_DEPTH = 32 };

typedef struct {
    CivDesign states[HISTORY_DEPTH];
    int first;
    int count;
} DesignStack;

typedef struct {
    CivDesign present;
    DesignStack past;
    DesignStack future;
    Command last;
    bool joinable;
} History;

void history_start(History *history);
bool history_do(History *history, const Command *command);
bool history_replace(History *history, const CivDesign *design);
bool history_step(History *history, bool back);

#endif
