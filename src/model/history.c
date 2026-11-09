#include "history.h"

#include "../core/check.h"

static void stack_push(DesignStack *stack, const CivDesign *design)
{
    int slot;

    if (!CHECK(stack != NULL) || !CHECK(design != NULL)) {
        return;
    }
    if (stack->count >= HISTORY_DEPTH) {
        stack->first = (stack->first + 1) % HISTORY_DEPTH;
        stack->count = HISTORY_DEPTH - 1;
    }
    slot = (stack->first + stack->count) % HISTORY_DEPTH;
    stack->states[slot] = *design;
    ++stack->count;
}

static bool stack_pop(DesignStack *stack, CivDesign *design)
{
    if (!CHECK(stack != NULL) || !CHECK(design != NULL) || (stack->count <= 0)) {
        return false;
    }
    --stack->count;
    *design = stack->states[(stack->first + stack->count) % HISTORY_DEPTH];
    return true;
}

void history_start(History *history)
{
    if (!CHECK(history != NULL) || !CHECK(HISTORY_DEPTH > 1)) {
        return;
    }
    civ_design_reset(&history->present);
    history->past.first = 0;
    history->past.count = 0;
    history->future.first = 0;
    history->future.count = 0;
    history->joinable = false;
    history->last = command_of(COMMAND_PICK_HERO, 0, CIV_NO_CHOICE);
}

bool history_do(History *history, const Command *command)
{
    static CivDesign before;

    if (!CHECK(history != NULL) || !CHECK(command != NULL)) {
        return false;
    }
    before = history->present;
    if (!command_apply(&history->present, command)) {
        history->present = before;
        return false;
    }
    if (!history->joinable || !command_joins(&history->last, command)) {
        stack_push(&history->past, &before);
    }
    history->future.count = 0;
    history->last = *command;
    history->joinable = true;
    return true;
}

bool history_replace(History *history, const CivDesign *design)
{
    if (!CHECK(history != NULL) || !CHECK(design != NULL)) {
        return false;
    }
    stack_push(&history->past, &history->present);
    history->present = *design;
    history->future.count = 0;
    history->joinable = false;
    return true;
}

bool history_undo(History *history)
{
    if (!CHECK(history != NULL) || !CHECK(history->past.count <= HISTORY_DEPTH) || (history->past.count <= 0)) {
        return false;
    }
    stack_push(&history->future, &history->present);
    history->joinable = false;
    return stack_pop(&history->past, &history->present);
}

bool history_redo(History *history)
{
    if (!CHECK(history != NULL) || !CHECK(history->future.count <= HISTORY_DEPTH) || (history->future.count <= 0)) {
        return false;
    }
    stack_push(&history->past, &history->present);
    history->joinable = false;
    return stack_pop(&history->future, &history->present);
}

bool history_can_undo(const History *history)
{
    if (!CHECK(history != NULL) || !CHECK(history->past.count <= HISTORY_DEPTH)) {
        return false;
    }
    return history->past.count > 0;
}

bool history_can_redo(const History *history)
{
    if (!CHECK(history != NULL) || !CHECK(history->future.count <= HISTORY_DEPTH)) {
        return false;
    }
    return history->future.count > 0;
}
