[![GitHub last commit](https://img.shields.io/github/last-commit/cnordenb/civbuilder)](#)
[![language](https://img.shields.io/badge/language-C-FFFF00)](#)
[![license](https://img.shields.io/badge/license-GPL--3.0-007FFF)](LICENSE)
[![testing](https://img.shields.io/badge/testing-yes-C3FF00)](documentation/TESTS.md)
[![maintainable](https://img.shields.io/badge/maintainable-yes-00FF72)](documentation/MAINTAINER.md)

# civbuilder

## Description

civbuilder designs a new civilization for Age of Empires II: Definitive Edition and builds it into a mod the game
can load. It is a remake of [aoe2-empire-forge](https://github.com/napkingcole/aoe2-empire-forge) in C alone, with a
window of its own made with the Win32 API: no browser, no Python, nothing to install but the program.

A civ is made in nine steps: its name and looks, its tech tree, its bonuses, its team bonus, its unique unit, its two
unique techs and its hero. Then it is reviewed, saved as a civ file that the original also opens, and built into a
mod.

## Usage

1. **Build it.** With [MinGW-w64](https://www.mingw-w64.org/) gcc on the `PATH`, in the project's folder, once:
   `gcc -o build.exe tools/build/*.c src/core/*.c`. Then `build.exe` builds civbuilder and runs the tests.
2. **Start it.** Run `build\civbuilder.exe`.
3. **Pick a base civ.** The first control of the first step is Base civ. The new civ takes the tech tree, the
   bonuses, the unique unit and techs and the looks of the civ picked there, and every later step can change them.
   At first it is Generic: every unit, building and tech that all civs of the game share, and nothing else.
4. **Make a civ.** Go through the steps with the mouse or the keyboard: Tab moves between the controls, the arrow
   keys pick in a list, and Space ticks or unticks in a list of bonuses or of the tech tree. Ctrl+Page Down and
   Ctrl+Page Up go to the next and the previous step, Ctrl+1 to Ctrl+9 to a step. F1 says in the status line what the control in focus is
   for, and Glossary in the menu Help explains the game's words and shortened forms. Ctrl+Z undoes a change, Ctrl+Y
   redoes it. Every command is also in the menu (Alt).
5. **Save it.** Save the civ as a `.civbuilder.json` file with Ctrl+S, or open one saved before with Ctrl+O. A civ
   without a name is offered the name `civbuilder_civ`. The last step shows what the civ still needs before a mod
   can be built from it.
6. **Build the mod.** On the last step, pick the civ of the game that the new civ replaces: at first the base civ,
   or the Britons for Generic. Then press Build mod... or Ctrl+B. civbuilder asks before it writes two folders,
   `<name>-data` and `<name>-ui`, into the game's `mods\local` folder, and then says what it built. In the game,
   turn on the UI mod in the mod manager and pick the data mod in the lobby. The game must be installed through
   Steam; civbuilder reads its data file and never changes it. Turn the UI mod off before you play a campaign: the
   names of the unique techs, the unique unit and the hero take the place of a few lines of the game's campaigns.
7. **Give the UI mod the highest priority.** Another mod that changes the game's texts, such as a mod of tooltips,
   can cover the civ's texts if the mod manager gives it a higher priority than the UI mod. The civ's description
   has a string id of its own, which other mods do not use, but its name and the names of its unique unit, unique
   techs and hero keep the game's ids.

## Contributing

Anyone is welcome to open an [issue](https://github.com/cnordenb/civbuilder/issues) to submit a bug report or suggest a new feature. Alternatively, you may fork or copy the repository and take it in any direction you wish.

If you wish to contribute work into this repository, please consult [CONTRIBUTING.md](.github/CONTRIBUTING.md) before any commits or pull requests.

## Maintaining

To keep civbuilder working and up to date, see [MAINTAINER.md](documentation/MAINTAINER.md).

## Acknowledgements

civbuilder remakes aoe2-empire-forge by napkingcole, whose catalog of bonuses comes from Krakenmeister's civbuilder.

Development of civbuilder was accelerated with Claude Opus 5.5.
