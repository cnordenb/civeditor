#ifndef COMMAND_H
#define COMMAND_H

#include <stdbool.h>

#include "civ_design.h"

typedef enum {
    COMMAND_SET_TEXT,
    COMMAND_SET_CHOICE,
    COMMAND_TOGGLE_BONUS,
    COMMAND_TOGGLE_TEAM_BONUS,
    COMMAND_SET_TREE,
    COMMAND_CLEAR_BONUSES,
    COMMAND_PICK_UNIT,
    COMMAND_PICK_TECH,
    COMMAND_PICK_HERO,
    COMMAND_SET_MULTIPLIER,
    COMMAND_SET_TEAM_MULTIPLIER,
    COMMAND_SET_UNIT_STAT,
    COMMAND_SET_TECH_VALUE,
    COMMAND_TOGGLE_TECH_EFFECT,
    COMMAND_SET_TECH_EFFECT,
    COMMAND_KIND_COUNT
} CommandKind;

typedef struct {
    CommandKind kind;
    int field;
    int value;
    char text[CIV_DESCRIPTION_CAPACITY];
    IdSet tree[TREE_KIND_COUNT];
} Command;

Command command_of(CommandKind kind, int field, int value);
Command command_nothing(void);
Command command_set_text(TextField field, const char *text);
Command command_set_tree(const CivDesign *source);
Command command_pick(CommandKind kind, int field, int value, const char *name);
Command command_set_tech_value(TechTier tier, TechValue value, int amount);
Command command_toggle_tech_effect(TechTier tier, int bonus_id);
Command command_set_tech_effect(TechTier tier, int bonus_id, int millionths);
bool command_apply(CivDesign *design, const Command *command);
bool command_joins(const Command *earlier, const Command *later);

#endif
