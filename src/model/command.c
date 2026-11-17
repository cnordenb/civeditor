#include "command.h"

#include "../core/text.h"

enum { NO_TEXT_FIELD = -1 };

Command command_of(CommandKind kind, int field, int value)
{
    Command command = { kind, field, value, "" };

    return command;
}

Command command_nothing(void)
{
    Command command = { COMMAND_KIND_COUNT, 0, CIV_NO_CHOICE, "" };

    return command;
}

Command command_set_text(TextField field, const char *text)
{
    Command command = command_of(COMMAND_SET_TEXT, (int)field, 0);

    if (!text_copy(command.text, sizeof command.text, text)) {
        command.kind = COMMAND_KIND_COUNT;
    }
    return command;
}

static bool apply_pick(CivDesign *design, const Command *command)
{
    if (command->kind == COMMAND_PICK_UNIT) {
        return civ_design_pick_unit(design, (UnitOrigin)command->field, command->value);
    }
    if (command->kind == COMMAND_PICK_TECH) {
        return civ_design_pick_tech(design, (TechTier)command->field, command->value);
    }
    return (command->kind == COMMAND_PICK_HERO) && civ_design_pick_hero(design, command->value);
}

static bool apply_toggle(CivDesign *design, const Command *command)
{
    if (command->kind == COMMAND_TOGGLE_BONUS) {
        return civ_design_toggle_bonus(design, command->value);
    }
    if (command->kind == COMMAND_TOGGLE_TEAM_BONUS) {
        return civ_design_toggle_team_bonus(design, command->value);
    }
    return (command->kind == COMMAND_TOGGLE_TREE) &&
           civ_design_toggle_tree(design, (TreeKind)command->field, command->value);
}

bool command_apply(CivDesign *design, const Command *command)
{
    if (command->kind == COMMAND_SET_TEXT) {
        return civ_design_set_text(design, (TextField)command->field, command->text);
    }
    if (command->kind == COMMAND_SET_CHOICE) {
        return civ_design_set_choice(design, (ChoiceField)command->field, command->value);
    }
    return apply_toggle(design, command) || apply_pick(design, command);
}

static int text_field_of(const Command *command)
{
    return (command->kind == COMMAND_SET_TEXT) ? command->field : NO_TEXT_FIELD;
}

bool command_joins(const Command *earlier, const Command *later)
{
    return (text_field_of(earlier) != NO_TEXT_FIELD) && (text_field_of(earlier) == text_field_of(later));
}
