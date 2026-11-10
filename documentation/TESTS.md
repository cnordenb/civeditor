# Tests

This project strives to have sensible testing. The goal is not to have 100% test coverage but to have tests which _sufficiently_ ensure the functional completeness, functional correctness, maintainability, and accessibility of civbuilder. For more guidelines on software quality and accessibility, see [ISO/IEC 25010:2011](https://cdn.standards.iteh.ai/samples/35733/2ca18b477b7845a5b8cae39d6de0c098/ISO-IEC-25010-2011.pdf), [ISO/IEC 25010:2023](https://cdn.standards.iteh.ai/samples/78176/13ff8ea97048443f99318920757df124/ISO-IEC-25010-2023.pdf) and [WCAG 2.2](https://www.w3.org/TR/WCAG22/).

This document lists each test, its purpose, and in which test file it resides. This document is an effort to mitigate redundant tests and ensure test quality. It is intended as an aid in the trouble-shooting of potential false positives.

Test descriptions must remain brief and parameters (eg. Purpose) may not exceed 120 characters. Tests are ordered in order of their execution.

The test report names each test by its number here, such as "Test 15 failed": a test's number is its place in the order
the tests run. So when a test is added, removed or moved, renumber this document to match.

There may be at most 100 tests (Rule 5).

The tests run with the build: `build.exe` (or `build.exe quick`) in the project's folder. `build\tests.exe` runs them
again without building. A file and lines given as `suite.c`, line N, is the line that names the test; the lines after
it are the code that makes the check.

## Table of contents per category

Functional completeness & correctness tests:

- [Test 1](#test-1): a failed check names its file, line and condition
- [Test 2](#test-2): the original's example civ keeps its name
- [Test 3](#test-3): the original's example civ keeps its bonuses
- [Test 4](#test-4): the original's example civ keeps its unique unit
- [Test 5](#test-5): the original's example civ keeps its language
- [Test 6](#test-6): a saved civ loads back unchanged
- [Test 7](#test-7): undo brings back the civ before a change
- [Test 8](#test-8): typing a name is undone at once
- [Test 9](#test-9): redo makes an undone change again
- [Test 10](#test-10): a 25th bonus is refused
- [Test 11](#test-11): next walks every step in order
- [Test 12](#test-12): the summary names what is still needed
- [Test 13](#test-13): the summary labels the civ's name
- [Test 14](#test-14): the last step shows no Next button
- [Test 15](#test-15): a dynamic Huffman stream inflates
- [Test 16](#test-16): a fixed Huffman stream inflates
- [Test 17](#test-17): a stored stream inflates back
- [Test 18](#test-18): packed data unpacks to the same bytes
- [Test 19](#test-19): repeated data packs to a tenth of its size
- [Test 20](#test-20): an unchanged dat file is written back byte for byte
- [Test 21](#test-21): an added effect is written as the game holds it
- [Test 22](#test-22): an added tech is written as the game holds it
- [Test 23](#test-23): a bonus without techs is built from its changes
- [Test 24](#test-24): the name is written under the replaced civ's name id
- [Test 25](#test-25): the civ descriptions are written in id order
- [Test 26](#test-26): the mod's folder name is made from the civ's name
- [Test 27](#test-27): a mod is not built from an unfinished civ
- [Test 28](#test-28): a unique tech is aimed at the civ's own unique unit
- [Test 29](#test-29): a unique tech's name is written under the string id of its slot
- [Test 30](#test-30): a unique unit's name is written under the string id of its slot
- [Test 31](#test-31): a building takes the look of its architecture's civ
- [Test 32](#test-32): a monk takes the look of its monk skin
- [Test 33](#test-33): a hero trains at the castle
- [Test 34](#test-34): a hero's name is written under the string id of its slot
- [Test 35](#test-35): a skipped hero's name is not written
- [Test 36](#test-36): the civ list names the civ's own unique unit
- [Test 37](#test-37): a unique unit missing from the civ list takes its own icon and texts
- [Test 38](#test-38): a unit patch changes its civ's unit
- [Test 39](#test-39): the civ list points at the civ's own description
- [Test 40](#test-40): the Generic base civ has exactly the universal nodes
- [Test 41](#test-41): choosing Generic takes away a regional node
- [Test 42](#test-42): a base civ has the nodes its civ has in the game
- [Test 43](#test-43): a base civ gives the civ exactly its bonuses
- [Test 44](#test-44): a base civ gives the civ its team bonus
- [Test 45](#test-45): a base civ gives the civ its unique unit
- [Test 46](#test-46): a building the civ lacks is not available in its tech tree
- [Test 47](#test-47): a building the civ has is available in its tech tree
- [Test 48](#test-48): a civ with an empty tree keeps the game's tech tree
- [Test 49](#test-49): a civ keeps its villagers in its tech tree
- [Test 50](#test-50): a building the civ has keeps the research it needs in its tech tree
- [Test 51](#test-51): only the tech of a unit the civ lacks is disabled
- [Test 52](#test-52): only a tech the civ lacks is disabled
- [Test 53](#test-53): a civ keeps its villagers in the dat file
- [Test 54](#test-54): a building the replaced civ lacked takes the status most civs give it

Accessibility tests:

- [Test 55](#test-55): every text contrasts at least 7:1 with its background
- [Test 56](#test-56): every input has a visible label
- [Test 57](#test-57): every control one acts on is at least 44 by 44
- [Test 58](#test-58): no two controls of a step overlap, even in the smallest window
- [Test 59](#test-59): the window's title names the civ and the step
- [Test 60](#test-60): the list of steps is on every step, with Back and Next wherever they lead
- [Test 61](#test-61): the Tab key reaches every control of every step
- [Test 62](#test-62): the list of steps shows the current step as the chosen one
- [Test 63](#test-63): the Tab key follows the order in which a step is read
- [Test 64](#test-64): a ticked row leaves the list of bonuses where it was scrolled
- [Test 65](#test-65): every control tells a screen reader a name that begins with its label
- [Test 66](#test-66): a ticked row is told to a screen reader as chosen
- [Test 67](#test-67): moving the focus through every control changes nothing
- [Test 68](#test-68): the Tab key leaves every control
- [Test 69](#test-69): Windows is told that the window's language is English
- [Test 70](#test-70): the help key says what the control in focus is for
- [Test 71](#test-71): the status line is a live region that a screen reader is told of
- [Test 72](#test-72): the text and the layout grow with the text size of Windows, up to 225%
- [Test 73](#test-73): the glossary explains every shortened form in capitals of the catalog
- [Test 74](#test-74): the control in focus has a solid frame at least two pixels thick
- [Test 75](#test-75): a ticked row is drawn with a check, not a filled box
- [Test 76](#test-76): after an undo, the base civ shown is the design's again

Maintainability tests:

- [Test 77](#test-77): no comments in the code
- [Test 78](#test-78): no function over 60 lines
- [Test 79](#test-79): no line of code over 120 characters
- [Test 80](#test-80): no file over 400 lines of code
- [Test 81](#test-81): no goto, setjmp or longjmp
- [Test 82](#test-82): no recursion, direct or indirect
- [Test 83](#test-83): every loop has a fixed upper bound
- [Test 84](#test-84): no dynamic memory allocation
- [Test 85](#test-85): at least two checks per function
- [Test 86](#test-86): preprocessor only includes and guards
- [Test 87](#test-87): one level of pointer dereferencing
- [Test 88](#test-88): no function pointers but the window procedure
- [Test 89](#test-89): no folder over 7 files, the main folder 8
- [Test 90](#test-90): no two files share a name
- [Test 91](#test-91): only C code and documents in the project
- [Test 92](#test-92): every placeholder of the template is filled in
- [Test 93](#test-93): the development log keeps its form
- [Test 94](#test-94): TESTS.md numbers its tests in order
- [Test 95](#test-95): TESTS.md documents every test
- [Test 96](#test-96): every line of TERMINOLOGY.md is short
- [Test 97](#test-97): the CRAP score is complexity squared times the uncovered share cubed, plus complexity
- [Test 98](#test-98): the complexity of a function counts its branches
- [Test 99](#test-99): two functions that differ only in their names are alike
- [Test 100](#test-100): the mutants of a line change each operator and constant once

## Test 1

### File residence:
`tests/functional/core_tests.c`, lines 6-12

### Purpose:
Verify that a failed check tells where it failed and what, so a fault can be found (P10 rule 5).

### Assertion expectation:
After `CHECK(width > 4)` fails, the last failure reads `core_tests.c:10: check failed: width > 4`.

## Test 2

### File residence:
`tests/functional/civ_file_tests.c`, lines 16-19

### Purpose:
Verify that a civ file of the original opens, with its name (Req. 1).

### Assertion expectation:
`tests/fixtures/example.civbuilder.json` loads, and the design's name is "Vanguards".

## Test 3

### File residence:
`tests/functional/civ_file_tests.c`, lines 21-25

### Purpose:
Verify that a civ file of the original opens with its bonuses (Req. 1).

### Assertion expectation:
The example civ has exactly two bonuses, 9 and 29.

## Test 4

### File residence:
`tests/functional/civ_file_tests.c`, lines 27-31

### Purpose:
Verify that a civ file of the original opens with its unique unit (Req. 1).

### Assertion expectation:
The example civ's unique unit is the custom unit with index 5.

## Test 5

### File residence:
`tests/functional/civ_file_tests.c`, lines 33-36

### Purpose:
Prevent again a civ file's language from being read as the civ after it: the original counts from 0 (Req. 1).

### Assertion expectation:
The example civ's language 4, the Japanese in the original's count, is read as slot 5, the Japanese's.

## Test 6

### File residence:
`tests/functional/civ_file_tests.c`, lines 54-66

### Purpose:
Verify that saving and opening a civ loses nothing (Req. 1).

### Assertion expectation:
A design written as JSON, read back and written again gives the same text, quotes in its name included.

## Test 7

### File residence:
`tests/functional/model_tests.c`, lines 22-29

### Purpose:
Verify that undo takes back the last change, so a slip costs nothing (WCAG 2.2 SC 3.3.6; Req. 5).

### Assertion expectation:
After bonus 7 is chosen and undone, undo reports success and the civ has no bonus.

## Test 8

### File residence:
`tests/functional/model_tests.c`, lines 31-38

### Purpose:
Verify that a name typed letter by letter is undone as one change, not a letter at a time (Req. 5).

### Assertion expectation:
After "V", "Vi" and "Vik" are typed as the name, one undo leaves the name empty.

## Test 9

### File residence:
`tests/functional/model_tests.c`, lines 40-48

### Purpose:
Verify that redo makes again a change that was undone (Req. 5).

### Assertion expectation:
After unit 1263 is picked, undone and redone, redo reports success and the unique unit is 1263 again.

## Test 10

### File residence:
`tests/functional/model_tests.c`, lines 50-60

### Purpose:
Prevent a bonus past the fixed 24 places from being written beyond them (P10 rules 2 and 3).

### Assertion expectation:
After 24 bonuses are chosen, a 25th is refused and the civ still has 24.

## Test 11

### File residence:
`tests/functional/model_tests.c`, lines 62-74

### Purpose:
Verify that Next leads through the nine steps in their order and stays on the review (Req. 3).

### Assertion expectation:
From the identity step, each Next gives the step after it, and Next on the review gives the review.

## Test 12

### File residence:
`tests/functional/model_tests.c`, lines 76-84

### Purpose:
Verify that the review names what the original's steps would not let pass yet (Req. 3).

### Assertion expectation:
For a civ with only a name, the summary reads "Still needed before a mod can be built: a unique unit; a castle tech;".

## Test 13

### File residence:
`tests/functional/model_tests.c`, lines 86-93

### Purpose:
Prevent again the review losing its labels, a bug found on 2026-11-09 (Rule 24, a regression test).

### Assertion expectation:
For a civ named "Vikings", the summary holds the line "Name: Vikings".

## Test 14

### File residence:
`tests/functional/model_tests.c`, lines 95-98

### Purpose:
Prevent again a Next button on the review, the last step, a bug found on 2026-11-09 (Rule 24, a regression test).

### Assertion expectation:
`layout_shows(CONTROL_NEXT, STEP_REVIEW)` is false, so the review step has no Next button.

## Test 15

### File residence:
`tests/pack/pack_tests.c`, lines 44-49

### Purpose:
Verify that a stream with its own Huffman codes inflates, as the game's dat file is packed (Req. 8).

### Assertion expectation:
The 91 bytes gzip made of a 119-character text inflate to exactly that text.

## Test 16

### File residence:
`tests/pack/pack_tests.c`, lines 51-56

### Purpose:
Verify that a stream with the fixed Huffman codes of RFC 1951 inflates (Req. 8).

### Assertion expectation:
The 7 bytes `cb 48 cd c9 c9 07 00` inflate to "hello".

## Test 17

### File residence:
`tests/pack/pack_tests.c`, lines 58-76

### Purpose:
Verify that a dat file packed for a mod in stored blocks reads back unchanged (Req. 8).

### Assertion expectation:
140 000 bytes, packed by `deflate_store` in three blocks and inflated again, are the same 140 000 bytes.

## Test 18

### File residence:
`tests/pack/pack_tests.c`, lines 119-134

### Purpose:
Verify that a dat file compressed for a mod reads back unchanged (Req. 8).

### Assertion expectation:
140 000 bytes of copied runs and random bytes, packed by `deflate_pack` and inflated again, are the same 140 000 bytes.

## Test 19

### File residence:
`tests/pack/pack_tests.c`, lines 136-146

### Purpose:
Verify that a mod's dat file is compressed, as the original compresses it, not only stored (Req. 8).

### Assertion expectation:
140 000 bytes that repeat every 251 bytes are packed by `deflate_pack` into fewer than 14 000 bytes.

## Test 20

### File residence:
`tests/dat/dat_tests.c`, lines 212-220

### Purpose:
Verify that a mod's dat file keeps the game's data wherever nothing is edited (Req. 8).

### Assertion expectation:
A minimal valid dat file of 82 678 bytes, mapped and written with an empty edit, is the same 82 678 bytes.

## Test 21

### File residence:
`tests/dat/dat_tests.c`, lines 222-237

### Purpose:
Verify that an effect added for the civ is written as the game holds an effect (Req. 8).

### Assertion expectation:
Loom's effect with its 3 changes, added to an empty dat file, is written as the 43 bytes the game's own file holds.

## Test 22

### File residence:
`tests/dat/dat_tests.c`, lines 239-248

### Purpose:
Verify that a tech added for the civ is written as the game holds a tech (Req. 8).

### Assertion expectation:
The tech Loom, added to an empty dat file, is written as the 75 bytes the game's own file holds.

## Test 23

### File residence:
`tests/dat/dat_tests.c`, lines 276-293

### Purpose:
Verify that a civ bonus that the catalog makes from changes, not techs, is built for the civ (Req. 8).

### Assertion expectation:
Two changes of bonus 55 make one tech of civ 1 that needs tech 102 and fires a new effect with the 2 changes.

## Test 24

### File residence:
`tests/mod/mod_tests.c`, lines 36-44

### Purpose:
Verify that the civ's name replaces the name of the civ it replaces in the UI mod's strings (Req. 8).

### Assertion expectation:
For "Testonia" in the slot of the Britons, the strings start with the line `10271 "Testonia"`.

## Test 25

### File residence:
`tests/mod/mod_tests.c`, lines 46-58

### Purpose:
Prevent again civ descriptions written out of order, a bug found on 2026-11-09 (Rule 24, a regression test).

### Assertion expectation:
With the game's 120201 line before its 120200 line, the strings hold 120200 before 120201.

## Test 26

### File residence:
`tests/mod/mod_tests.c`, lines 60-68

### Purpose:
Verify that the mod's folders are named from the civ's name as the original names them (Req. 7).

### Assertion expectation:
"Æsir Kings 2!" gives the folder name "_sir_kings_2_": one "_" for each character that is not a-z, 0-9, _ or -.

## Test 27

### File residence:
`tests/mod/mod_tests.c`, lines 70-82

### Purpose:
Verify that no mod is built from a civ the review still finds unfinished (Req. 9).

### Assertion expectation:
A civ with a name but no unique unit or techs is refused with a problem that says it is "not finished".

## Test 28

### File residence:
`tests/mod/mod_tests.c`, lines 96-103

### Purpose:
Verify that a unique tech taken from another civ works on the civ's own unique unit (Req. 8).

### Assertion expectation:
A change of +1 range for Throwing Axeman 281, the other civ's unique unit, is aimed at the civ's Longbowman 8.

## Test 29

### File residence:
`tests/mod/mod_tests.c`, lines 109-128

### Purpose:
Verify that the name given to a unique tech is the name the game shows for it (Req. 8).

### Assertion expectation:
A castle tech named "Testing Axes" in the slot of the Britons gives the line `44008 "Testing Axes"` in the strings.

## Test 30

### File residence:
`tests/mod/mod_tests.c`, lines 134-148

### Purpose:
Verify that the name given to a unique unit of the game is the name the game shows for it (Req. 8).

### Assertion expectation:
The Longbowman named "Testbowman" in the slot of the Britons gives the line `69802 "Testbowman"` in the strings.

## Test 31

### File residence:
`tests/dat/dat_tests.c`, lines 297-309

### Purpose:
Verify that the civ's buildings look as those of the civ its architecture names (Req. 8).

### Assertion expectation:
A building of graphic 101 in slot 1 is given graphic 100, that of the same building of civ 0, the architecture's civ.

## Test 32

### File residence:
`tests/dat/dat_tests.c`, lines 313-327

### Purpose:
Verify that the civ's monks look as those of the monk skin chosen for it (Req. 8).

### Assertion expectation:
A monk, unit 125, of graphic 101 in slot 1 is given graphic 100, that of the monk of civ 0, whose skin is chosen.

## Test 33

### File residence:
`tests/dat/dat_tests.c`, lines 339-359

### Purpose:
Verify that the hero chosen for the civ is trained at the Castle, as in the original (Req. 8).

### Assertion expectation:
Unit 5 of slot 1, trained nowhere in the small dat file, is given building 82, the Castle, as its train location.

## Test 34

### File residence:
`tests/mod/mod_tests.c`, lines 171-174

### Purpose:
Verify that the name given to the hero is the name the game shows for it (Req. 8).

### Assertion expectation:
Liu Bei, unit 1966, named "Testhero" in the slot of the Britons gives the line `44500 "Testhero"` in the strings.

## Test 35

### File residence:
`tests/mod/mod_tests.c`, lines 176-179

### Purpose:
Prevent the strings of a hero that was not built from being written, as they were before (regression test).

### Assertion expectation:
With the hero skipped, the UI mod's strings have no line `44500 "Testhero"` for slot 1's hero string id.

## Test 36

### File residence:
`tests/mod/mod_tests.c`, lines 207-213

### Purpose:
Verify that the civ list names the civ's own unique unit, as the original's does (Req. 8).

### Assertion expectation:
Slot 1 given the Longbowman, unit 8, takes the id 8 and the line -276 of the entry that has it; all else is kept.

## Test 37

### File residence:
`tests/mod/mod_tests.c`, lines 224-230

### Purpose:
Verify that a unique unit no civ of the game has shows its own icon and texts in the civ list (Req. 8).

### Assertion expectation:
Unit 9, absent from the list, makes slot 1's icon `007_50730.png` and its string ids 5200 and 26200; all else is kept.

## Test 38

### File residence:
`tests/dat/dat_tests.c`, lines 366-379

### Purpose:
Verify that a bonus's unit patch sets the field of the unit of its own civ (Req. 8).

### Assertion expectation:
A patch of bonus 330 sets collision size x, at +29, of unit 0 of civ 1 to 1.0; the bonus counts as built.

## Test 39

### File residence:
`tests/mod/mod_tests.c`, lines 237-243

### Purpose:
Prevent again a civ selection that shows the old civ's bonuses, a bug found on 2026-11-09 (Rule 24, a regression test).

### Assertion expectation:
An entry whose `tech_tree_description_string_id` is 120150 is written with 990101, the own id of slot 1.

## Test 40

### File residence:
`tests/base/base_tests.c`, lines 40-44

### Purpose:
Prevent again a new civ that starts with an empty tech tree, a bug found on 2026-11-09 (Rule 24, a regression test).

### Assertion expectation:
Generic gives the one universal node of the catalog, the Barracks, and not a universal row past the catalog's count.

## Test 41

### File residence:
`tests/base/base_tests.c`, lines 46-50

### Purpose:
Verify that choosing Generic after another base civ takes that civ's regional nodes away (base civ order, 2026-11-09).

### Assertion expectation:
A design given base civ 1 and then Generic no longer has the regional unit that civ 1 has.

## Test 42

### File residence:
`tests/base/base_tests.c`, lines 52-55

### Purpose:
Verify that a base civ gives the design the nodes its civ has in the game (base civ order of 2026-11-09).

### Assertion expectation:
Base civ 1 gives the design the regional unit whose list of civs holds civ 1.

## Test 43

### File residence:
`tests/base/base_tests.c`, lines 57-60

### Purpose:
Verify that a base civ gives the design its own civ bonuses and no others (base civ order of 2026-11-09).

### Assertion expectation:
Base civ 1, with bonus 3 alone in its list, gives the design exactly one bonus: bonus 3.

## Test 44

### File residence:
`tests/base/base_tests.c`, lines 62-65

### Purpose:
Verify that a base civ gives the design its team bonus (base civ order of 2026-11-09).

### Assertion expectation:
Base civ 1, whose team bonus is 0, gives the design team bonus 0.

## Test 45

### File residence:
`tests/base/base_tests.c`, lines 67-71

### Purpose:
Verify that a base civ gives the design its unique unit, as the game has it (base civ order of 2026-11-09).

### Assertion expectation:
Base civ 1, whose unique unit is 0, gives the design unit 0 of the game, of vanilla origin.

## Test 46

### File residence:
`tests/mod/mod_tests.c`, lines 274-277

### Purpose:
Prevent again a tech tree screen that shows units the civ lacks, a bug found on 2026-11-09 (Rule 24, a regression test).

### Assertion expectation:
A Stable that the game's tree has as ResearchedCompleted is written NotAvailable for a design without a Stable.

## Test 47

### File residence:
`tests/mod/mod_tests.c`, lines 279-282

### Purpose:
Prevent again a tech tree screen that hides what the civ has, a bug found on 2026-11-09 (Rule 24, a regression test).

### Assertion expectation:
A Stable that the game's tree has as NotAvailable is written ResearchedCompleted for a design with a Stable.

## Test 48

### File residence:
`tests/mod/mod_tests.c`, lines 284-287

### Purpose:
Verify that a civ with an empty tree keeps the game's tech tree screen, as its dat keeps every tech (Req. 8).

### Assertion expectation:
With nothing ticked, a Stable that the game's tree has as ResearchedCompleted is written unchanged.

## Test 49

### File residence:
`tests/mod/mod_tests.c`, lines 289-292

### Purpose:
Prevent a tech tree screen without the Villager, Town Centers, Mill and camps, which the dat never disables (Req. 8).

### Assertion expectation:
A Villager that the game's tree has as ResearchedCompleted is written unchanged for a design without it.

## Test 50

### File residence:
`tests/mod/mod_tests.c`, lines 294-297

### Purpose:
Verify that a building the civ has still needs the research the game asks for it, as the Keep does (Req. 8).

### Assertion expectation:
A Keep that the game's tree has as ResearchRequired is written unchanged for a design with a Keep.

## Test 51

### File residence:
`tests/dat/dat_tests.c`, lines 424-428

### Purpose:
Verify that the dat file disables the tech of a unit the civ lacks, and not that of a unit it has (Req. 8).

### Assertion expectation:
Of two techs that enable units 4 and 7, for a design with unit 4 only, tech 1 alone gets a change of type 102.

## Test 52

### File residence:
`tests/dat/dat_tests.c`, lines 430-434

### Purpose:
Verify that the dat file disables a tech of the tech tree the civ lacks, and not one it has (Req. 8).

### Assertion expectation:
Of techs 0 and 1, both in the catalog, for a design with tech 0 only, tech 1 alone gets a change of type 102.

## Test 53

### File residence:
`tests/dat/dat_tests.c`, lines 436-442

### Purpose:
Prevent a dat file that disables the Villager, which every civ keeps in the dat file and the tech tree file (Req. 8).

### Assertion expectation:
Of the techs that enable the Villager and unit 7, for a design with neither, only the second gets a change of type 102.

## Test 54

### File residence:
`tests/functional/functional_suite.c`, lines 222-224; `tests/mod/mod_tests.c`, lines 299-310

### Purpose:
Enforce that the tech tree screen shows a node as the game does for most civs (Requirement 8).

### Assertion expectation:
A Keep the replaced civ lacked, which most civs must research, is written `ResearchRequired` when the design has it.

## Test 55

### File residence:
`tests/accessibility/access_tests.c`, lines 21-34

### Purpose:
Verify that all text can be read by those who see less contrast (WCAG 2.2 SC 1.4.6; Req. 3).

### Assertion expectation:
Each of the 7 pairs of text and background colors in `palette.c` contrasts at least 7.00:1 by the WCAG formula.

## Test 56

### File residence:
`tests/accessibility/access_tests.c`, lines 36-52

### Purpose:
Verify that every input says what it is for, seen and to a screen reader (WCAG 2.2 SC 2.5.3, 3.3.2).

### Assertion expectation:
Every text field, list, checklist, choice and report in `layout.c` has a label of its own.

## Test 57

### File residence:
`tests/accessibility/access_tests.c`, lines 54-68

### Purpose:
Verify that everything one clicks is large enough to hit (WCAG 2.2 SC 2.5.5).

### Assertion expectation:
In the smallest window, 960 by 560, every control one can act on is at least 44 by 44, and so is a list row.

## Test 58

### File residence:
`tests/accessibility/access_tests.c`, lines 70-117

### Purpose:
Verify that no control or label hides another, so focus is never covered (WCAG 2.2 SC 2.4.12).

### Assertion expectation:
On each of the 9 steps at 960 by 560, no two boxes of controls and labels overlap, and all lie inside the window.

## Test 59

### File residence:
`tests/accessibility/access_tests.c`, lines 119-128

### Purpose:
Verify that the window's title says which civ is open and on which step (WCAG 2.2 SC 2.4.2; Req. 3).

### Assertion expectation:
For a civ named Testonia on the tech tree step, the title is "Testonia - Step 2 of 9: Tech tree - civbuilder".

## Test 60

### File residence:
`tests/accessibility/accessibility_suite.c`, lines 27-29; `tests/accessibility/access_tests.c`, lines 130-144

### Purpose:
Enforce that the ways from step to step keep their places on every step (Requirement 3, WCAG 2.2 SC 3.2.3).

### Assertion expectation:
On each of the nine steps the list of steps is shown, Back wherever a step lies before, Next wherever one lies after.

## Test 61

### File residence:
`tests/accessibility/accessibility_suite.c`, lines 41-43; `tests/accessibility/window_tests.c`, lines 55-226

### Purpose:
Enforce that every control can be reached with the keyboard alone (Requirement 3, WCAG 2.2 SC 2.1.1 and 2.1.3).

### Assertion expectation:
In the real window, made but not shown, the Tab order of each step holds every control of it that one acts on.

## Test 62

### File residence:
`tests/accessibility/accessibility_suite.c`, lines 44-46; `tests/accessibility/window_tests.c`, lines 251-263

### Purpose:
Enforce that the window shows where the user is among the steps (Requirement 3, WCAG 2.2 SC 2.4.8).

### Assertion expectation:
In the real window, after going to each step by its command, the list of steps has that step as its chosen row.

## Test 63

### File residence:
`tests/accessibility/accessibility_suite.c`, lines 47-49; `tests/accessibility/window_tests.c`, lines 265-318

### Purpose:
Enforce that the keyboard goes through a step as it is read (Requirement 3, WCAG 2.2 SC 2.4.3 and 1.3.2).

### Assertion expectation:
On each step, Tab goes from the list of steps down each column from left to right, then to Back and Next.

## Test 64

### File residence:
`tests/accessibility/accessibility_suite.c`, lines 50-52; `tests/accessibility/window_tests.c`, lines 320-343

### Purpose:
Catch again Bug 1 of the trial: a click on a row scrolled the list of bonuses to its top (Rule 24).

### Assertion expectation:
In the real window, with the list scrolled to row 30, ticking row 34 leaves row 30 at the top of the list.

## Test 65

### File residence:
`tests/accessibility/accessibility_suite.c`, lines 53-55; `tests/accessibility/window_tests.c`, lines 72-234

### Purpose:
Enforce that each control has a name for assistive technology (Requirement 3, WCAG 2.2 SC 4.1.2, 1.3.1, 2.5.3).

### Assertion expectation:
In the real window, the name that Windows' accessibility interface gives each control begins with its label.

## Test 66

### File residence:
`tests/accessibility/accessibility_suite.c`, lines 56-58; `tests/accessibility/window_tests.c`, lines 345-375

### Purpose:
Enforce that the check of a ticked row has a text alternative (Requirement 3, WCAG 2.2 SC 1.1.1).

### Assertion expectation:
In the real window, Windows' accessibility interface gives a ticked row of the bonuses as chosen, another row not.

## Test 67

### File residence:
`tests/accessibility/accessibility_suite.c`, lines 59-61; `tests/accessibility/window_tests.c`, lines 377-398

### Purpose:
Enforce that a control does nothing by getting the focus alone (Requirement 3, WCAG 2.2 SC 3.2.1).

### Assertion expectation:
After the focus was put on every control of every step, undo still says "There is nothing to undo."

## Test 68

### File residence:
`tests/accessibility/accessibility_suite.c`, lines 62-63; `tests/accessibility/window_tests.c`, lines 106-239

### Purpose:
Enforce that the keyboard is never trapped in a control (Requirement 3, WCAG 2.2 SC 2.1.2).

### Assertion expectation:
With the focus on each control of each step in turn, a Tab key given to the dialog manager moves the focus on.

## Test 69

### File residence:
`tests/accessibility/accessibility_suite.c`, lines 64-66; `tests/accessibility/window_tests.c`, lines 400-412

### Purpose:
Enforce that the language of the window can be found by a program (Requirement 3, WCAG 2.2 SC 3.1.1).

### Assertion expectation:
Once the window is made, the one language the process prefers for its texts, as Windows tells it, is `en-US`.

## Test 70

### File residence:
`tests/accessibility/accessibility_suite.c`, lines 67-69; `tests/accessibility/window_tests.c`, lines 132-244

### Purpose:
Enforce that every control has help of its own (Requirement 3, WCAG 2.2 SC 3.3.5).

### Assertion expectation:
In the real window, with the focus on each control in turn, the help command puts its help in the status line.

## Test 71

### File residence:
`tests/accessibility/accessibility_suite.c`, lines 70-72; `tests/accessibility/window_tests.c`, lines 414-451

### Purpose:
Enforce that a status message reaches assistive technology (Requirement 3, WCAG 2.2 SC 4.1.3).

### Assertion expectation:
In the real window, UI Automation gives the status line the live setting "polite".

## Test 72

### File residence:
`tests/accessibility/accessibility_suite.c`, lines 73-75; `tests/accessibility/access_tests.c`, lines 146-149

### Purpose:
Enforce that the text follows the text size set in Windows (Requirement 3, WCAG 2.2 SC 1.4.4).

### Assertion expectation:
At 96 dots an inch and a text size of 400%, the layout is scaled as at 225%, the most Windows offers: 216.

## Test 73

### File residence:
`tests/accessibility/accessibility_suite.c`, lines 76-78; `tests/accessibility/access_tests.c`, lines 151-214

### Purpose:
Enforce that a shortened form can be looked up (Requirement 3, WCAG 2.2 SC 3.1.4).

### Assertion expectation:
Every word of 2 to 5 capitals in five files of `data/` is a word of the glossary, but FREE and OFF.

## Test 74

### File residence:
`tests/accessibility/accessibility_suite.c`, lines 79-81; `tests/accessibility/window_tests.c`, lines 146-178

### Purpose:
Enforce that the focus can be seen (Requirement 3, WCAG 2.2 SC 2.4.7 and 2.4.13).

### Assertion expectation:
With the focus on each control in turn, the window paints 2 or more pixels of another color just left of it.

## Test 75

### File residence:
`tests/accessibility/accessibility_suite.c`, lines 82-84; `tests/accessibility/draw_tests.c`, lines 11-72

### Purpose:
Catch again Bug 2 of the trial: a ticked box showed a filled rectangle, not a check (Rule 24).

### Assertion expectation:
In the picture the window draws of a ticked row, the inside of the box holds marks, on less than two fifths of it.

## Test 76

### File residence:
`tests/accessibility/accessibility_suite.c`, lines 85-87; `tests/accessibility/choice_tests.c`, lines 11-26

### Purpose:
Enforce that a list or choice shows what the design holds, after an undo too (Requirement 5).

### Assertion expectation:
In the real window, after another base civ is picked and the pick undone, the choice shows Generic again.

## Test 77

### File residence:
`tests/maintainability/maintainability_suite.c`, line 18; `tests/maintainability/code_rules.c`, lines 55-85

### Purpose:
Enforce that the code holds no comments (Rule 10).

### Assertion expectation:
No `.c` or `.h` file holds `//` or `/*` outside a string; a failure names the file and line.

## Test 78

### File residence:
`tests/maintainability/maintainability_suite.c`, line 20; `tests/maintainability/code_rules.c`, lines 25-38

### Purpose:
Enforce that no function is longer than 60 lines of code (Rule 11, P10 rule 4).

### Assertion expectation:
Every function, from its opening to its closing brace, has at most 60 lines that are not blank.

## Test 79

### File residence:
`tests/maintainability/maintainability_suite.c`, line 22; `tests/maintainability/code_rules.c`, lines 40-53

### Purpose:
Enforce that no line of code is longer than 120 characters (Rule 12).

### Assertion expectation:
Every line of every `.c` and `.h` file has at most 120 characters; a failure names the file and line.

## Test 80

### File residence:
`tests/maintainability/maintainability_suite.c`, line 24; `tests/maintainability/code_rules.c`, lines 55-85

### Purpose:
Enforce that no file of code is longer than 400 lines of code (Rule 13).

### Assertion expectation:
Every `.c` and `.h` file has at most 400 lines that are not blank.

## Test 81

### File residence:
`tests/maintainability/maintainability_suite.c`, line 26; `tests/maintainability/code_patterns.c`, lines 80-86

### Purpose:
Enforce simple control flow without jumps (P10 rule 1).

### Assertion expectation:
The words `goto`, `setjmp` and `longjmp` appear in no code.

## Test 82

### File residence:
`tests/maintainability/maintainability_suite.c`, line 28; `tests/maintainability/code_rules.c`, lines 146-162

### Purpose:
Enforce that no function calls itself, directly or through others (P10 rule 1).

### Assertion expectation:
The graph of calls between the functions of all `.c` files has no cycle; a failure names a function in one.

## Test 83

### File residence:
`tests/maintainability/maintainability_suite.c`, line 30; `tests/maintainability/code_patterns.c`, lines 151-171

### Purpose:
Enforce that every loop has a fixed upper bound (P10 rule 2).

### Assertion expectation:
No `while` or `do`; the condition of every `for` names a constant in capitals.

## Test 84

### File residence:
`tests/maintainability/maintainability_suite.c`, line 32; `tests/maintainability/code_patterns.c`, lines 88-94

### Purpose:
Enforce that no memory is allocated while the program runs (P10 rule 3).

### Assertion expectation:
None of `malloc`, `calloc`, `realloc`, `free`, `alloca`, `strdup` or the Win32 allocators appears in the code.

## Test 85

### File residence:
`tests/maintainability/maintainability_suite.c`, line 34; `tests/maintainability/code_rules.c`, lines 178-200

### Purpose:
Enforce at least two checks per function on average outside the tests (P10 rule 5).

### Assertion expectation:
The `CHECK(` in the functions of `src/` and `tools/`, `check.c` aside, are at least twice the functions.

## Test 86

### File residence:
`tests/maintainability/maintainability_suite.c`, line 36; `tests/maintainability/code_patterns.c`, lines 215-230

### Purpose:
Enforce that the preprocessor only includes files and guards headers (P10 rule 8).

### Assertion expectation:
Only `#include`, `#ifndef`, `#endif` and `#define` of a `_H` guard or of `CHECK(` appear.

## Test 87

### File residence:
`tests/maintainability/maintainability_suite.c`, line 38; `tests/maintainability/code_patterns.c`, lines 232-257

### Purpose:
Enforce at most one level of dereferencing (P10 rule 9).

### Assertion expectation:
No `**`, no `*const *` and no chain of two `->` in the code.

## Test 88

### File residence:
`tests/maintainability/maintainability_suite.c`, line 40; `tests/maintainability/code_patterns.c`, lines 274-292

### Purpose:
Enforce that no function pointer is used but the one window procedure Win32 needs (P10 rule 9).

### Assertion expectation:
No `(*name)(`, no callback API; `lpfnWndProc` only in `src/win/app_window.c`.

## Test 89

### File residence:
`tests/maintainability/maintainability_suite.c`, line 54; `tests/rules/repo_rules.c`, lines 56-75

### Purpose:
Enforce that no folder holds more than 7 files, the main folder 8 (Rule 14).

### Assertion expectation:
Every folder of the files Git sees holds at most 7 files, the main one 8; `documentation/DEVLOG/` is not counted.

## Test 90

### File residence:
`tests/maintainability/maintainability_suite.c`, line 56; `tests/rules/repo_rules.c`, lines 114-128

### Purpose:
Enforce that no two files share a name (Rule 15).

### Assertion expectation:
No two file names are equal when case is ignored, `SKILL.md` aside.

## Test 91

### File residence:
`tests/maintainability/maintainability_suite.c`, line 58; `tests/rules/repo_rules.c`, lines 145-159

### Purpose:
Enforce that the project holds only C and documents (Rule 29).

### Assertion expectation:
Every file ends in `.c`, `.h`, `.md`, `.txt`, `.json` or `.png`, or is `LICENSE`, `.gitignore` or `.gitattributes`.

## Test 92

### File residence:
`tests/maintainability/maintainability_suite.c`, line 60; `tests/rules/repo_rules.c`, lines 200-217

### Purpose:
Enforce that the documents of repo_template are filled in (Rule 3).

### Assertion expectation:
No `.md` file holds two opening braces before a capital, or the template's start or end mark.

## Test 93

### File residence:
`tests/maintainability/maintainability_suite.c`, line 62; `tests/rules/log_rules.c`, lines 176-193

### Purpose:
Enforce the form of the development log (Rules 18 to 21).

### Assertion expectation:
`DEVLOG1.md` has its title, at most 1000 lines, dated days listed in order at its top, and times as hh:mm:ss.

## Test 94

### File residence:
`tests/maintainability/maintainability_suite.c`, line 64; `tests/rules/log_rules.c`, lines 221-248

### Purpose:
Enforce that TESTS.md numbers its tests 1, 2, 3 in order, with short fields (Rules 5, 6).

### Assertion expectation:
Headings and contents both count up from 1, agree, stay at most 100, and no field line passes 120 characters.

## Test 95

### File residence:
`tests/maintainability/maintainability_suite.c`, line 66; `tests/rules/log_rules.c`, lines 264-279

### Purpose:
Enforce that TESTS.md documents every test and no more (Rule 6).

### Assertion expectation:
The `## Test` headings are as many as the `case` lines of the three suites.

## Test 96

### File residence:
`tests/maintainability/maintainability_suite.c`, line 68; `tests/rules/log_rules.c`, lines 281-295

### Purpose:
Enforce that every term is one short line (Rule 9).

### Assertion expectation:
No line of `TERMINOLOGY.md` passes 120 characters.

## Test 97

### File residence:
`tests/maintainability/maintainability_suite.c`, lines 123-125; `tools/crap/crap_score.c`, lines 13-34

### Purpose:
Enforce that the crapper scores a function as Rule 32 defines the CRAP score.

### Assertion expectation:
A function of complexity 4 with 1 of 3 branches run scores 8.740: 16 times 8/27, plus 4.

## Test 98

### File residence:
`tests/maintainability/maintainability_suite.c`, lines 126-128; `tools/crap/crap_score.c`, lines 36-87

### Purpose:
Enforce that the crapper counts a function's complexity as one plus its branches (Rule 32).

### Assertion expectation:
A text with `if`, `&&`, `||`, `for`, `while`, `case` and `?` has complexity 8; "gift" and "notify" add nothing.

## Test 99

### File residence:
`tests/maintainability/maintainability_suite.c`, lines 74-89 and 129-131; `tools/dry/dry_shape.c`, lines 163-219

### Purpose:
Enforce that the dryer ranks two functions by their shape, not by their names (Rule 31).

### Assertion expectation:
Two loops that add up prices, with other names for every local and another number, rank 1.000.

## Test 100

### File residence:
`tests/maintainability/maintainability_suite.c`, lines 91-112 and 132-134; `tools/mutate/mutant_sites.c`, lines 19-168

### Purpose:
Enforce that the mutator makes the mutants Rule 34 names, one for each operator and constant.

### Assertion expectation:
`if (!ready && (item->count <= limit - 1)) { return false; }` gives six: no `!`, `||`, `<`, `+`, `0`, `true`.
