#include "layout.h"

#include "../core/check.h"

static const char HELP_TEXTS[CONTROL_COUNT + 1][LAYOUT_HELP_WIDTH] = {
    "The steps of a civ. Pick one to go to it; a step marked done has all it needs.",
    "",
    "",
    "",
    "Goes to the step before this one.",
    "Goes to the next step.",
    "The civ of the game to edit. Edits of other civs are kept; a civ that was changed is marked with *.",
    "The name of the civ, as the game will show it. A civ needs a name to be built.",
    "A short line about the civ, shown where the game lets you pick a civ.",
    "Opens the looks of the civ: its architecture, language, wonder, castle, monk and scout.",
    "The civ of the game whose buildings yours will look like.",
    "The civ of the game whose language your units speak.",
    "The civ of the game whose wonder yours builds.",
    "The civ of the game whose castle yours builds.",
    "How the monks of the civ look.",
    "The unit the civ starts with to explore the map.",
    "Closes the looks of the civ. Each choice was kept as it was made.",
    "Type a part of a name, or the name of a civ, to show only what holds it or what that civ has.",
    "Tick what the civ may have. Unticking one unticks what needs it. Space ticks or unticks the row in focus.",
    "Says how many units, buildings and techs are ticked.",
    "Type a part of a text, or the name of a civ, to show only the bonuses that hold it or are that civ's.",
    "Shows only the bonuses of one kind, such as economy or military.",
    "Tick up to 24 bonuses for the civ. Space ticks or unticks the row in focus.",
    "Unticks every bonus of the civ at once. Undo brings them back.",
    "The ticked bonuses. Type a value into the box of a number and press Enter, or move the slider under it.",
    "Type a part of a text, or the name of a civ, to show only the team bonuses that hold it or are its.",
    "Pick the one bonus that the civ and its allies get.",
    "The picked team bonus. Type a value into the box of a number and press Enter, or move the slider under it.",
    "Type a part of a name, or the name of a civ, to show only the units that hold it or are that civ's.",
    "Pick the unit that only this civ trains, at its Castle.",
    "The name of the unique unit. Picking a unit gives it the game's name, which you may change.",
    "A text of your own for the unique unit, shown where the game explains the unit.",
    "What the picked unit costs and how strong it is. Type a value into the box of a number and press Enter.",
    "Type a part of a name, or the name of a civ, to show only the castle techs that hold it or are its.",
    "Pick the unique tech that the civ researches in the Castle Age.",
    "The name of the Castle Age tech. Picking a tech gives it the game's name, which you may change.",
    "Opens the civ bonuses to tick what the castle tech does. With none ticked, it does what the game's tech does.",
    "What the picked tech does, costs and takes. Type a value into the box of a number and press Enter.",
    "Type a part of a name, or the name of a civ, to show only the imperial techs that hold it or are its.",
    "Pick the unique tech that the civ researches in the Imperial Age.",
    "The name of the Imperial Age tech. Picking a tech gives it the game's name, which you may change.",
    "Opens the civ bonuses to tick what the imperial tech does. With none ticked, it does what the game's tech does.",
    "What the picked tech does, costs and takes. Type a value into the box of a number and press Enter.",
    "The name the hero will have in the game. Choose hero... picks another hero or changes the name.",
    "Opens the heroes of the game to pick another. Picking one gives it the game's name, which you may change.",
    "Tick to give the civ a hero, Attila the Hun at first, trained at its Castle in the Imperial Age.",
    "Reads the whole civ back to you and says what it still needs before it can be built.",
    "Says how many civs the mod changes: every civ that differs from the game's.",
    "Builds every civ changed into one mod, under a name you give, in the game's mods folder. Ctrl+B too.",
    "Says that the last build made the whole mod."
};

const char *layout_help(ControlId control)
{
    return HELP_TEXTS[check_index((int)control, CONTROL_COUNT, CONTROL_COUNT)];
}
