#include "layout.h"

#include "../core/check.h"

static const char HELP_TEXTS[CONTROL_COUNT + 1][LAYOUT_HELP_WIDTH] = {
    "The nine steps of a civ. Pick one to go to it; a step marked done has all it needs.",
    "",
    "",
    "",
    "Goes to the step before this one.",
    "Goes to the next step.",
    "The civ to start from. Picking one replaces the tree, bonuses and looks; undo takes it back.",
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
    "A name of your own for the unique unit. Leave it empty to keep the game's name.",
    "A text of your own for the unique unit, shown where the game explains the unit.",
    "Says what the picked unit costs and how strong it is.",
    "Pick the unique tech that the civ researches in the Castle Age.",
    "A name of your own for the Castle Age tech.",
    "Says what the picked tech does, what it costs and how long it takes.",
    "Pick the unique tech that the civ researches in the Imperial Age.",
    "A name of your own for the Imperial Age tech.",
    "Says what the picked tech does, what it costs and how long it takes.",
    "Pick a hero that the civ trains at its Castle in the Imperial Age, or no hero.",
    "A name of your own for the hero. A hero needs a name to be built.",
    "Reads the whole civ back to you and says what it still needs before it can be built.",
    "The civ of the game whose place yours takes in the mod.",
    "Saves the civ as a file you can open again. Ctrl+S does the same.",
    "Opens a civ file saved before. Ctrl+O does the same.",
    "Builds the civ into a mod in the game's mods folder, after asking. Ctrl+B does the same."
};

const char *layout_help(ControlId control)
{
    return HELP_TEXTS[check_index((int)control, CONTROL_COUNT, CONTROL_COUNT)];
}

static const GlossaryEntry GLOSSARY[LAYOUT_GLOSSARY_COUNT] = {
    { "civ", "a civilization: a people of the game, with units, techs and bonuses of its own" },
    { "base civ", "the civ of the game that a new civ starts as" },
    { "Generic", "a base civ with what every civ of the game has, and nothing more" },
    { "tech", "a technology: something a civ researches once to become better" },
    { "tech tree", "all the units, buildings and techs that a civ may have" },
    { "unique unit", "a unit that only one civ trains, at its Castle" },
    { "unique tech", "a tech that only one civ researches, at its Castle" },
    { "team bonus", "a bonus that a civ and all its allies get" },
    { "hero", "one strong unit with a name of its own, of which a civ has only one at a time" },
    { "mod", "a change to the game, which the game loads from its mods folder" },
    { "Castle Age", "the third of the four ages of a game" },
    { "Imperial Age", "the fourth and last of the four ages of a game" },
    { "Folwark", "the building the Poles have in place of the Mill" },
    { "Krepost", "a small castle of the Bulgarians" },
    { "Mule Cart", "a cart that takes in wood, gold and stone, in place of the camps" },
    { "HP", "hit points: how much damage a unit or a building takes before it is lost" },
    { "LOS", "line of sight: how far a unit or a building sees" },
    { "LoS", "line of sight: how far a unit or a building sees" },
    { "ATK", "attack: the damage a unit does with one hit" },
    { "TC", "Town Center: the building that trains Villagers" },
    { "ARRG", "Archery Range: the building that trains archers" },
    { "vs.", "versus: against" },
    { "vs", "versus: against" },
    { "Skirms", "Skirmishers" },
    { "vils", "Villagers" },
    { "Cav", "cavalry: units on horseback" },
    { "H.", "Heavy, at the start of a unit's name" },
    { "E.", "Elite, at the start of a unit's name" },
    { "max", "at most" },
    { "min", "a minute, as in hit points a minute" }
};

GlossaryEntry layout_glossary(int index)
{
    static const GlossaryEntry NONE = { "", "" };

    if (!CHECK(index >= 0) || !CHECK(index < LAYOUT_GLOSSARY_COUNT)) {
        return NONE;
    }
    return GLOSSARY[index];
}
