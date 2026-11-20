clanker - non-human agent, tool, large language model (LLM), artificial intelligence (AI), AI agent (eg. Claude, Claude Code, Codex, Gemini, Grok, etc.)
requirement - a numbered statement in REQUIREMENTS.md of what the project must do, e.g. "Requirement 4"
rule - a numbered statement in RULES.md of how the project must be made and kept, e.g. "Rule 12"
chief developer - the person who alone writes the requirements, named in Rule 2
test - a check of the project, run on every push and listed by its number in TESTS.md, e.g. "Test 15"
log - the development log in documentation/DEVLOG/: what changed in the project, why, and what is left
part - one file of the log, DEVLOG1.md, DEVLOG2.md and so on, each of at most 1000 lines
day - the entry of the log for one date, titled by it, e.g. "2026-11-02", never split between two parts
section - one change recorded under its day, titled by its time if it was the first written in that minute
decision - a choice made in a section and listed by subject in DECISIONS.md, with a link to that section
question - a numbered point for the chief developer at the end of a section, open until a later section answers it
deletion request - a file or folder that should be deleted, listed in DELETION_REQUESTS.md for a human to delete
original - aoe2-empire-forge, the Python web application that civbuilder remakes in C
game - Age of Empires II: Definitive Edition, for which civbuilder makes civs
civ - a civilization of the game: its name, looks, bonuses, tech tree, unique unit, unique techs and hero
tagline - the line that says what kind of civ it is, e.g. "Foot Archer"; the game gives one to each of its civs
design - the civ being made in civbuilder, kept in CivDesign in src/model/civ_design.c
look - a choice of how a civ looks and sounds: its architecture, language, wonder, castle, monk or scout
monk skin - the look of a civ's monks, taken from another civ or from a priest of the scenario editor
bonus - an advantage of a civ, e.g. "Villagers carry +3 food"; a civ has up to 24
team bonus - the one bonus a civ gives to itself and to its allies
unique unit - the unit only a civ can train, trained at its castle
elite unit - the stronger form of a unique unit, which a tech of its civ upgrades it to, e.g. the Elite Longbowman
unique tech - one of the two techs only a civ can research: the castle tech and the imperial tech
hero unit - a single strong unit a civ may have, picked from the heroes of the game; the hero, for short
tech tree - the units, buildings and techs a civ may have; each one in it is a node
node - one unit, building or tech of the tech tree, known by its id
universal node - a node the game shows in the tech tree of every civ, had or not, e.g. the Archery Range
regional node - a node that only some civs of the game have, e.g. the Camel Rider
unique node - a node that only one civ of the game has, e.g. the Folwark
step - one page of the wizard, e.g. "Bonuses"; there are four
UU&UT - the step of the unique unit, the two unique techs and the hero, each picked and named
wizard - the four steps in which a design is made: name and tech tree, bonuses, UU&UT and review
base civ - a civ of the game as civbuilder holds it: its tech tree, bonuses, looks, names, tagline and description
civ to edit - the civ of the game that the steps show and change, chosen in the first step; the Britons at first
draft - the design of a civ to edit as it was last left, kept while another civ is edited
catalog - the lists civbuilder offers in the steps, read from data/*.txt: looks, bonuses, units, techs, nodes
control - one part of a step that the user reads or acts on, e.g. the list of bonuses; each has a visible label
checklist - a control whose rows are ticked and unticked, e.g. the bonuses; in a plain list one row is picked
Hero box - the control of UU&UT ticked to give the civ a hero, Attila the Hun at first, and unticked to take it away
Choose hero... - the button left of the Hero box that lists the heroes; picking one fills in its name in the game
notch - one step of the mouse wheel; a notch over a list moves it one row
smooth scrolling - Windows sliding a list as it scrolls; civbuilder moves its lists at once instead
knob - the part of a scroll bar that shows how far it is scrolled and can be dragged; Windows calls it the thumb
classic scroll bar - a scroll bar that shows its arrows and knob at all times, as every scroll bar of civbuilder does
command - one change to a design, e.g. ticking a bonus, made through history_do in src/model/history.c
undo - taking back the last command, with Ctrl+Z; redo makes it again, with Ctrl+Y
civ file - a design saved as JSON in the format empireforge_v2 of the original; civbuilder_v1 files also open
effect - a list of changes the game applies, e.g. "+3 food carried by villagers"; a bonus has one or more
dat file - empires2_x2_p1.dat, the game's data: units, techs, effects and civs, compressed with deflate
graphic - a picture of the game, still or moving, known by its id in the dat file, e.g. a castle standing
train location - where a unit of the dat file is trained: a building, its button, a hotkey and a time, e.g. the castle
slot - the number of a civ in the dat file, e.g. 1 for the Britons; 0 is Gaia, the civ of the map's animals
change - one entry of an effect: a type and four values, e.g. type 102 disables the tech its last value names
patch - up to 8 bytes of the dat file written over at an offset, e.g. the team bonus of a civ
copy - bytes of the dat file written over others of the same size, e.g. the graphics of another civ's castle
unit patch - a patch that a bonus makes to one field of a unit of its civ in the dat file, e.g. a farm's size
train line - a unit patch placed in the first train location of its unit, e.g. the cow of bonus 222 trained at the Mill
cost line - a unit patch placed in the three costs of its unit, e.g. the 300 stone of the Castle of bonus 460
look line - a line of a bonus giving a unit of its civ the look of that unit of another civ, e.g. the Palintonon of 457
requirement tech - a tech of one civ, researched by itself, that a tech of every civ requires, e.g. 768 for the Cavalier
multiplier - how many times a bonus counts, 1 to 10, kept in the civ file; each change of the bonus is scaled to it
mod - a folder the game loads from its local mods folder; civbuilder makes a data mod and a UI mod
mods folder - the game's local mods folder, mods\local in the user's newest profile, where a mod is built
data mod - the mod with a changed dat file that holds the new civ
civ list - civilizations.json, the game's list of civs, their names, unique techs and unique unit; the data mod has one
tech tree file - the game's file of one civ's tech tree screen, e.g. CivTechTrees\BRITONS.json; the data mod has one
UI mod - the mod with the texts of the new civ for the game's screens
string id - the number of a text of the game, e.g. 10271 for "Britons"; the UI mod gives some of them new texts
description id - the string id of a civ's description, named by the civ list; the design's is its own, 990100 + its slot
mod name - the name the user gives a mod when Build mod asks for it, e.g. "Testonia"; civbuilder_mod at first
prefix - the start of both mod folders' names, made from the mod name, e.g. "testonia" in "testonia-data"
review - the last step: the whole design, and what it still needs before a mod can be built from it
Save civ - the button of the review that puts the design, as it is, in the mod; a civ as the game has it is left out
civs changed - the count of civs Save civ has put in the mod, shown at the top right of the review, e.g. "1 civ changed"
roster - what civbuilder keeps of each slot while it runs: the draft of its civ, and the design Save civ put in the mod
job - what Build mod puts in one mod: every design that Save civ put in it, up to 64, held in ForgeJob
replaced civ - the civ of the game whose slot a design takes in the mod: the civ to edit it was made from
recipe - what is done to the dat file to put a design in the slot of its replaced civ, in src/forge/recipe.c
aim - to turn what a unique tech does to another civ's unique unit to the design's own, as the recipe does
check - a condition the code asserts with CHECK where data enters it; a failed check is counted and named
build program - build.exe, made from tools/build/, which compiles, links and runs the tests in one step
analyzed build - the build that also runs the static analyzer of gcc: build.exe
quick build - the build without the static analyzer: build.exe quick
coverage build - the tests built with gcc's --coverage and run, so that gcovr can tell what ran: build.exe coverage
build plan - what the build program does, from its command line: analyzed, quick or coverage build, folders, commands
complexity - one more than the branches of a function: each if, loop, case, default, ?:, && and ||, a check's too
CRAP score - complexity squared times the cube of the share of branches no test takes, plus complexity (Req. 10)
crapper - ub-mode's program for Requirement 10, in ..\crapper: the CRAP score of each function it reads
shape - what is left of a function without its names and values: its calls, its operators and how it is built
dryer - ub-mode's program for Requirement 11, in ..\dryer: finds every two functions whose shapes are 80% alike or more
mutant - the code with one operator or constant changed, which a test should catch (Req. 12)
mutator - ub-mode's program for Requirement 12, in ..\mutator: makes each mutant of src/ and runs the tests on it
redundancy test - a test that only the crapper or the mutator needs, kept in tests/redundancy/ and run only when asked
suite - the tests of one kind or subject, run in order by one file, e.g. tests/suites/mod_suite.c
group - at most eight tests of a suite, held in one switch, e.g. run_gate_test
fixture - a file or a small game that a test reads or builds, e.g. tests/fixtures/broken_rules.txt
gate test - a test that shows the test of a rule a fixture that breaks the rule, and asks which line it names
script - the answers a test gives in the user's place to the dialogs of the window, set with ask_script
wide layout - a step beside the list of steps, its controls in up to three columns, in a window of 960 by 560 or more
reflow - to show a step in one column that scrolls up and down, when the window does not hold the wide layout
