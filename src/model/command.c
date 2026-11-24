#include "command.h"

#include <string.h>

#include "../core/text.h"

enum { NO_TEXT_FIELD = -1, TIER_STRIDE = 100000 };

Command command_of(CommandKind kind, int field, int value)
{
    Command command = { kind, field, value, "", { { { 0 }, 0 } } };

    return command;
}

Command command_nothing(void)
{
    return command_of(COMMAND_KIND_COUNT, 0, CIV_NO_CHOICE);
}

Command command_set_tree(const CivDesign *source)
{
    Command command = command_of(COMMAND_SET_TREE, 0, 0);

    (void)memcpy(command.tree, source->tree, sizeof command.tree);
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

Command command_pick(CommandKind kind, int field, int value, const char *name)
{
    Command command = command_of(kind, field, value);

    if (!text_copy(command.text, sizeof command.text, name)) {
        command.kind = COMMAND_KIND_COUNT;
    }
    return command;
}

static Command tier_command(CommandKind kind, TechTier tier, int item, int value)
{
    return command_of(kind, ((int)tier * TIER_STRIDE) + item, value);
}

Command command_set_tech_value(TechTier tier, TechValue value, int amount)
{
    return tier_command(COMMAND_SET_TECH_VALUE, tier, (int)value, amount);
}

Command command_toggle_tech_effect(TechTier tier, int bonus_id)
{
    return tier_command(COMMAND_TOGGLE_TECH_EFFECT, tier, bonus_id, 0);
}

Command command_set_tech_effect(TechTier tier, int bonus_id, int millionths)
{
    return tier_command(COMMAND_SET_TECH_EFFECT, tier, bonus_id, millionths);
}

static const TextField TECH_NAMES[TECH_TIER_COUNT] = { TEXT_FIELD_CASTLE_TECH_NAME, TEXT_FIELD_IMPERIAL_TECH_NAME };

static bool apply_pick(CivDesign *design, const Command *command)
{
    if (command->kind == COMMAND_PICK_UNIT) {
        return civ_design_pick_unit(design, (UnitOrigin)command->field, command->value) &&
               civ_design_set_text(design, TEXT_FIELD_UNIT_NAME, command->text);
    }
    if (command->kind == COMMAND_PICK_TECH) {
        return civ_design_pick_tech(design, (TechTier)command->field, command->value) &&
               civ_design_set_text(design, TECH_NAMES[command->field], command->text);
    }
    return (command->kind == COMMAND_PICK_HERO) && civ_design_pick_hero(design, command->value) &&
           civ_design_set_text(design, TEXT_FIELD_HERO_NAME, command->text);
}

static bool apply_toggle(CivDesign *design, const Command *command)
{
    if (command->kind == COMMAND_TOGGLE_BONUS) {
        return civ_design_toggle_bonus(design, command->value);
    }
    if (command->kind == COMMAND_TOGGLE_TEAM_BONUS) {
        return civ_design_toggle_team_bonus(design, command->value);
    }
    return (command->kind == COMMAND_CLEAR_BONUSES) && civ_design_clear_bonuses(design);
}

static bool apply_number(CivDesign *design, const Command *command)
{
    bool team = command->kind == COMMAND_SET_TEAM_MULTIPLIER;

    if (team || (command->kind == COMMAND_SET_MULTIPLIER)) {
        Multiplier multiplier = { command->value };

        return civ_design_set_multiplier(design, team, command->field, multiplier);
    }
    return (command->kind == COMMAND_SET_UNIT_STAT) &&
           civ_design_set_unit_stat(design, UNIT_RANK_BASE, (UnitStat)command->field, command->value);
}

static bool apply_tech(CivDesign *design, const Command *command)
{
    TechTier tier = (TechTier)(command->field / TIER_STRIDE);
    int item = command->field % TIER_STRIDE;
    Multiplier multiplier = { command->value };

    if (command->kind == COMMAND_SET_TECH_VALUE) {
        return civ_design_set_tech_value(design, tier, (TechValue)item, command->value);
    }
    if (command->kind == COMMAND_TOGGLE_TECH_EFFECT) {
        return civ_design_toggle_tech_effect(design, tier, item);
    }
    return (command->kind == COMMAND_SET_TECH_EFFECT) && civ_design_set_tech_effect(design, tier, item, multiplier);
}

static bool apply_tree(CivDesign *design, const Command *command)
{
    (void)memcpy(design->tree, command->tree, sizeof design->tree);
    return true;
}

bool command_apply(CivDesign *design, const Command *command)
{
    if (command->kind == COMMAND_SET_TEXT) {
        return civ_design_set_text(design, (TextField)command->field, command->text);
    }
    if (command->kind == COMMAND_SET_CHOICE) {
        return civ_design_set_choice(design, (ChoiceField)command->field, command->value);
    }
    if (command->kind == COMMAND_SET_TREE) {
        return apply_tree(design, command);
    }
    return apply_toggle(design, command) || apply_number(design, command) || apply_tech(design, command) ||
           apply_pick(design, command);
}

static int text_field_of(const Command *command)
{
    return (command->kind == COMMAND_SET_TEXT) ? command->field : NO_TEXT_FIELD;
}

bool command_joins(const Command *earlier, const Command *later)
{
    return (text_field_of(earlier) != NO_TEXT_FIELD) && (text_field_of(earlier) == text_field_of(later));
}
