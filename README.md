[![GitHub last commit](https://img.shields.io/github/last-commit/cnordenb/civeditor)](#)
[![language](https://img.shields.io/badge/language-C-555555)](#)
[![license](https://img.shields.io/badge/license-GPL--3.0-007FFF)](LICENSE)
[![testing](https://img.shields.io/badge/testing-yes-C3FF00)](documentation/TESTS.md)
[![maintainable](https://img.shields.io/badge/maintainable-yes-00FF72)](documentation/MAINTAINER.md)

# Civ Editor

## But why?
For a simpler, lighter app with fewer steps.

## Description

Civ Editor designs a new civilization for Age of Empires II: Definitive Edition and builds it into a mod the game
can load. It is a remake of [aoe2-empire-forge](https://github.com/napkingcole/aoe2-empire-forge) in C alone, with a
window of its own made with the Win32 API: no browser, no Python, nothing to install but the program.

A civ of the game is changed in three steps: Name and tech tree (its name, looks and tech tree), Bonuses (its civ
bonuses and team bonus) and UU&UT (its unique unit, two unique techs and, if Hero is ticked, a hero). In the fourth,
Finish, every civ changed is listed and built into one mod; nothing needs saving first. A civ can also be saved as a
civ file that the original also opens.

## Usage
0. Fetch binary from [releases](https://github.com/cnordenb/civeditor/releases).
1. Choose which civ to edit.  
2. Edit what you want.  
3. Build mod.  
4. Start the game.  
5. In lobby, choose your mod as data mod.  

## Compilation
1. Install [MinGW-w64](https://www.mingw-w64.org/).  
2. Add gcc to PATH in system environment variables.  
3. In the project's folder, make the build program (the first time only):  
   `gcc -o build.exe tools/build/*.c src/core/*.c`
4. Run it: `.\build.exe`. It compiles the code, runs the tests and leaves the binary at `build\civeditor.exe`.  

## Contributing

Anyone is welcome to open an [issue](https://github.com/cnordenb/civeditor/issues) to submit a bug report or suggest a new feature. Alternatively, you may fork or copy the repository and take it in any direction you wish.

If you wish to contribute work into this repository, please consult [CONTRIBUTING.md](.github/CONTRIBUTING.md) before any commits or pull requests.

## Maintaining

To keep Civ Editor working and up to date, see [MAINTAINER.md](documentation/MAINTAINER.md).

## Acknowledgements

Civ Editor is a modified derivative of napkingcole's [Empire Forge](https://github.com/napkingcole/aoe2-empire-forge) and subsequently licensed under [GPL-3.0](https://github.com/napkingcole/aoe2-empire-forge?tab=GPL-3.0-1-ov-file).

Development of Civ Editor was accelerated with Claude Opus 5.5.
