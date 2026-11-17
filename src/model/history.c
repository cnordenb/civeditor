#include "history.h"

#include "../core/check.h"

static void stack_push(DesignStack *stack, const CivDesign *design)
{
    int slot;

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
    if (stack->count <= 0) {
        return false;
    }
    --stack->count;
    *design = stack->states[(stack->first + stack->count) % HISTORY_DEPTH];
    return true;
}

void history_start(History *history)
{
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
    stack_push(&history->past, &history->present);
    history->present = *design;
    history->future.count = 0;
    history->joinable = false;
    return true;
}

bool history_step(History *history, bool back)
{
    DesignStack *from;
    DesignStack *to;

    if (!CHECK(history->past.count <= HISTORY_DEPTH)) {
        return false;
    }
    from = back ? &history->past : &history->future;
    to = back ? &history->future : &history->past;
    if (from->count <= 0) {
        return false;
    }
    stack_push(to, &history->present);
    history->joinable = false;
    return stack_pop(from, &history->present);
}
