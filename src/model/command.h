#ifndef COMMAND_H
#define COMMAND_H

#include <stdbool.h>

#include "civ_design.h"

typedef enum {
    COMMAND_SET_TEXT,
    COMMAND_SET_CHOICE,
    COMMAND_TOGGLE_BONUS,
    COMMAND_TOGGLE_TEAM_BONUS,
    COMMAND_TOGGLE_TREE,
    COMMAND_PICK_UNIT,
    COMMAND_PICK_TECH,
    COMMAND_PICK_HERO,
    COMMAND_KIND_COUNT
} CommandKind;

typedef struct {
    CommandKind kind;
    int field;
    int value;
    char text[CIV_DESCRIPTION_CAPACITY];
} Command;

Command command_of(CommandKind kind, int field, int value);
Command command_nothing(void);
Command command_set_text(TextField field, const char *text);
bool command_apply(CivDesign *design, const Command *command);
bool command_joins(const Command *earlier, const Command *later);

#endif
