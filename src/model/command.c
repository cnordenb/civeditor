#include "command.h"

#include "../core/check.h"
#include "../core/text.h"

Command command_of(CommandKind kind, int field, int value)
{
    Command command = { COMMAND_KIND_COUNT, 0, CIV_NO_CHOICE, "" };

    if (!CHECK((int)kind >= 0) || !CHECK(kind < COMMAND_KIND_COUNT)) {
        return command;
    }
    command.kind = kind;
    command.field = field;
    command.value = value;
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

    if (!CHECK(text != NULL) || !CHECK(((int)field >= 0) && (field < TEXT_FIELD_COUNT)) ||
        !text_copy(command.text, sizeof command.text, text)) {
        command.kind = COMMAND_KIND_COUNT;
    }
    return command;
}

static bool apply_pick(CivDesign *design, const Command *command)
{
    if (!CHECK(design != NULL) || !CHECK(command != NULL)) {
        return false;
    }
    switch (command->kind) {
    case COMMAND_PICK_UNIT:
        return civ_design_pick_unit(design, (UnitOrigin)command->field, command->value);
    case COMMAND_PICK_TECH:
        return civ_design_pick_tech(design, (TechTier)command->field, command->value);
    case COMMAND_PICK_HERO:
        return civ_design_pick_hero(design, command->value);
    case COMMAND_SET_TEXT:
    case COMMAND_SET_CHOICE:
    case COMMAND_TOGGLE_BONUS:
    case COMMAND_TOGGLE_TEAM_BONUS:
    case COMMAND_TOGGLE_TREE:
    case COMMAND_KIND_COUNT:
    default:
        return false;
    }
}

bool command_apply(CivDesign *design, const Command *command)
{
    if (!CHECK(design != NULL) || !CHECK(command != NULL)) {
        return false;
    }
    switch (command->kind) {
    case COMMAND_SET_TEXT:
        return civ_design_set_text(design, (TextField)command->field, command->text);
    case COMMAND_SET_CHOICE:
        return civ_design_set_choice(design, (ChoiceField)command->field, command->value);
    case COMMAND_TOGGLE_BONUS:
        return civ_design_toggle_bonus(design, command->value);
    case COMMAND_TOGGLE_TEAM_BONUS:
        return civ_design_toggle_team_bonus(design, command->value);
    case COMMAND_TOGGLE_TREE:
        return civ_design_toggle_tree(design, (TreeKind)command->field, command->value);
    case COMMAND_PICK_UNIT:
    case COMMAND_PICK_TECH:
    case COMMAND_PICK_HERO:
        return apply_pick(design, command);
    case COMMAND_KIND_COUNT:
    default:
        return false;
    }
}

bool command_joins(const Command *earlier, const Command *later)
{
    if (!CHECK(earlier != NULL) || !CHECK(later != NULL)) {
        return false;
    }
    return (earlier->kind == COMMAND_SET_TEXT) && (later->kind == COMMAND_SET_TEXT) &&
           (earlier->field == later->field);
}
