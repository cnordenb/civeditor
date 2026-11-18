#include "layout.h"

#include "../core/check.h"

static const char HELP_TEXTS[CONTROL_COUNT + 1][LAYOUT_HELP_WIDTH] = {
    "The nine steps of a civ. Pick one to go to it; a step marked done has all it needs.",
    "",
    "",
    "",
    "Goes to the step before this one.",
    "Goes to the next step.",
    "The civ of the game to edit. Every field then shows it as the game has it; edits of other civs are kept.",
    "The name of the civ, as the game will show it. A civ needs a name to be built.",
    "A short line about the civ, shown where the game lets you pick a civ.",
    "The story of the civ, in as many lines as you like. Tab leaves the field.",
    "The civ of the game whose buildings yours will look like.",
    "The civ of the game whose language your units speak.",
    "The civ of the game whose wonder yours builds.",
    "The civ of the game whose castle yours builds.",
    "How the monks of the civ look.",
    "The unit the civ starts with to explore the map.",
    "Type a part of a name to show only the units, buildings and techs that hold it.",
    "Tick what the civ may train, build and research. Space ticks or unticks the row in focus.",
    "Says how many units, buildings and techs are ticked.",
    "Type a part of a text to show only the bonuses that hold it.",
    "Shows only the bonuses of one kind, such as economy or military.",
    "Tick up to 24 bonuses for the civ. Space ticks or unticks the row in focus.",
    "Lists the bonuses that are ticked, and how many of the 24.",
    "Pick the one bonus that the civ and its allies get.",
    "Says what the picked team bonus does.",
    "Pick the unit that only this civ trains, at its Castle.",
    "The name of the unique unit. Picking a unit gives it the game's name, which you may change.",
    "A text of your own for the unique unit, shown where the game explains the unit.",
    "Says what the picked unit costs and how strong it is.",
    "Pick the unique tech that the civ researches in the Castle Age.",
    "The name of the Castle Age tech. Picking a tech gives it the game's name, which you may change.",
    "Says what the picked tech does, what it costs and how long it takes.",
    "Pick the unique tech that the civ researches in the Imperial Age.",
    "The name of the Imperial Age tech. Picking a tech gives it the game's name, which you may change.",
    "Says what the picked tech does, what it costs and how long it takes.",
    "Pick a hero that the civ trains at its Castle in the Imperial Age, or no hero.",
    "The name of the hero. Picking a hero gives it the game's name. A hero needs a name to be built.",
    "Reads the whole civ back to you and says what it still needs before it can be built.",
    "Says how many civs the mod changes: the civs that Save civ has put in it.",
    "Puts the civ, as it is now, in the mod. Edits made after are built only once it is saved again.",
    "Builds every civ saved into one mod, under a name you give, in the game's mods folder. Ctrl+B too."
};

const char *layout_help(ControlId control)
{
    return HELP_TEXTS[check_index((int)control, CONTROL_COUNT, CONTROL_COUNT)];
}
