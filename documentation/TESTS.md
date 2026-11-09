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

Accessibility tests:

- [Test 54](#test-54): every text contrasts at least 7:1 with its background
- [Test 55](#test-55): every input has a visible label
- [Test 56](#test-56): every control one acts on is at least 44 by 44
- [Test 57](#test-57): no two controls of a step overlap, even in the smallest window
- [Test 58](#test-58): the window's title names the civ and the step

Maintainability tests:

- [Test 59](#test-59): no comments in the code
- [Test 60](#test-60): no function over 60 lines
- [Test 61](#test-61): no line of code over 120 characters
- [Test 62](#test-62): no file over 400 lines of code
- [Test 63](#test-63): no goto, setjmp or longjmp
- [Test 64](#test-64): no recursion, direct or indirect
- [Test 65](#test-65): every loop has a fixed upper bound
- [Test 66](#test-66): no dynamic memory allocation
- [Test 67](#test-67): at least two checks per function
- [Test 68](#test-68): preprocessor only includes and guards
- [Test 69](#test-69): one level of pointer dereferencing
- [Test 70](#test-70): no function pointers but the window procedure
- [Test 71](#test-71): no folder over 7 files, the main folder 8
- [Test 72](#test-72): no two files share a name
- [Test 73](#test-73): only C code and documents in the project
- [Test 74](#test-74): every placeholder of the template is filled in
- [Test 75](#test-75): the development log keeps its form
- [Test 76](#test-76): TESTS.md numbers its tests in order
- [Test 77](#test-77): TESTS.md documents every test
- [Test 78](#test-78): every line of TERMINOLOGY.md is short

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
`tests/accessibility/access_tests.c`, lines 13-26

### Purpose:
Verify that all text can be read by those who see less contrast (WCAG 2.2 SC 1.4.6; Req. 3).

### Assertion expectation:
Each of the 7 pairs of text and background colors in `palette.c` contrasts at least 7.00:1 by the WCAG formula.

## Test 55

### File residence:
`tests/accessibility/access_tests.c`, lines 34-44

### Purpose:
Verify that every input says what it is for, seen and to a screen reader (WCAG 2.2 SC 2.5.3, 3.3.2).

### Assertion expectation:
Every text field, list, checklist, choice and report in `layout.c` has a label of its own.

## Test 56

### File residence:
`tests/accessibility/access_tests.c`, lines 46-60

### Purpose:
Verify that everything one clicks is large enough to hit (WCAG 2.2 SC 2.5.5).

### Assertion expectation:
In the smallest window, 960 by 560, every control one can act on is at least 44 by 44, and so is a list row.

## Test 57

### File residence:
`tests/accessibility/access_tests.c`, lines 87-109

### Purpose:
Verify that no control or label hides another, so focus is never covered (WCAG 2.2 SC 2.4.12).

### Assertion expectation:
On each of the 9 steps at 960 by 560, no two boxes of controls and labels overlap, and all lie inside the window.

## Test 58

### File residence:
`tests/accessibility/access_tests.c`, lines 111-120

### Purpose:
Verify that the window's title says which civ is open and on which step (WCAG 2.2 SC 2.4.2; Req. 3).

### Assertion expectation:
For a civ named Testonia on the tech tree step, the title is "Testonia - Step 2 of 9: Tech tree - civbuilder".

## Test 59

### File residence:
`tests/maintainability/maintainability_suite.c`, line 14; `tests/maintainability/code_rules.c`, lines 55-85

### Purpose:
Enforce that the code holds no comments (Rule 10).

### Assertion expectation:
No `.c` or `.h` file holds `//` or `/*` outside a string; a failure names the file and line.

## Test 60

### File residence:
`tests/maintainability/maintainability_suite.c`, line 16; `tests/maintainability/code_rules.c`, lines 25-38

### Purpose:
Enforce that no function is longer than 60 lines of code (Rule 11, P10 rule 4).

### Assertion expectation:
Every function, from its opening to its closing brace, has at most 60 lines that are not blank.

## Test 61

### File residence:
`tests/maintainability/maintainability_suite.c`, line 18; `tests/maintainability/code_rules.c`, lines 40-53

### Purpose:
Enforce that no line of code is longer than 120 characters (Rule 12).

### Assertion expectation:
Every line of every `.c` and `.h` file has at most 120 characters; a failure names the file and line.

## Test 62

### File residence:
`tests/maintainability/maintainability_suite.c`, line 20; `tests/maintainability/code_rules.c`, lines 55-85

### Purpose:
Enforce that no file of code is longer than 400 lines of code (Rule 13).

### Assertion expectation:
Every `.c` and `.h` file has at most 400 lines that are not blank.

## Test 63

### File residence:
`tests/maintainability/maintainability_suite.c`, line 22; `tests/maintainability/code_patterns.c`, lines 80-86

### Purpose:
Enforce simple control flow without jumps (P10 rule 1).

### Assertion expectation:
The words `goto`, `setjmp` and `longjmp` appear in no code.

## Test 64

### File residence:
`tests/maintainability/maintainability_suite.c`, line 24; `tests/maintainability/code_rules.c`, lines 146-162

### Purpose:
Enforce that no function calls itself, directly or through others (P10 rule 1).

### Assertion expectation:
The graph of calls between the functions of all `.c` files has no cycle; a failure names a function in one.

## Test 65

### File residence:
`tests/maintainability/maintainability_suite.c`, line 26; `tests/maintainability/code_patterns.c`, lines 151-171

### Purpose:
Enforce that every loop has a fixed upper bound (P10 rule 2).

### Assertion expectation:
No `while` or `do`; the condition of every `for` names a constant in capitals.

## Test 66

### File residence:
`tests/maintainability/maintainability_suite.c`, line 28; `tests/maintainability/code_patterns.c`, lines 88-94

### Purpose:
Enforce that no memory is allocated while the program runs (P10 rule 3).

### Assertion expectation:
None of `malloc`, `calloc`, `realloc`, `free`, `alloca`, `strdup` or the Win32 allocators appears in the code.

## Test 67

### File residence:
`tests/maintainability/maintainability_suite.c`, line 30; `tests/maintainability/code_rules.c`, lines 178-200

### Purpose:
Enforce at least two checks per function on average outside the tests (P10 rule 5).

### Assertion expectation:
The `CHECK(` in the functions of `src/` and `tools/`, `check.c` aside, are at least twice the functions.

## Test 68

### File residence:
`tests/maintainability/maintainability_suite.c`, line 32; `tests/maintainability/code_patterns.c`, lines 215-230

### Purpose:
Enforce that the preprocessor only includes files and guards headers (P10 rule 8).

### Assertion expectation:
Only `#include`, `#ifndef`, `#endif` and `#define` of a `_H` guard or of `CHECK(` appear.

## Test 69

### File residence:
`tests/maintainability/maintainability_suite.c`, line 34; `tests/maintainability/code_patterns.c`, lines 232-257

### Purpose:
Enforce at most one level of dereferencing (P10 rule 9).

### Assertion expectation:
No `**`, no `*const *` and no chain of two `->` in the code.

## Test 70

### File residence:
`tests/maintainability/maintainability_suite.c`, line 36; `tests/maintainability/code_patterns.c`, lines 274-292

### Purpose:
Enforce that no function pointer is used but the one window procedure Win32 needs (P10 rule 9).

### Assertion expectation:
No `(*name)(`, no callback API; `lpfnWndProc` only in `src/win/app_window.c`.

## Test 71

### File residence:
`tests/maintainability/maintainability_suite.c`, line 50; `tests/rules/repo_rules.c`, lines 56-75

### Purpose:
Enforce that no folder holds more than 7 files, the main folder 8 (Rule 14).

### Assertion expectation:
Every folder of the files Git sees holds at most 7 files, the main one 8; `documentation/DEVLOG/` is not counted.

## Test 72

### File residence:
`tests/maintainability/maintainability_suite.c`, line 52; `tests/rules/repo_rules.c`, lines 114-128

### Purpose:
Enforce that no two files share a name (Rule 15).

### Assertion expectation:
No two file names are equal when case is ignored, `SKILL.md` aside.

## Test 73

### File residence:
`tests/maintainability/maintainability_suite.c`, line 54; `tests/rules/repo_rules.c`, lines 145-159

### Purpose:
Enforce that the project holds only C and documents (instructions.md).

### Assertion expectation:
Every file ends in `.c`, `.h`, `.md`, `.txt` or `.json`, or is `LICENSE`, `.gitignore` or `.gitattributes`.

## Test 74

### File residence:
`tests/maintainability/maintainability_suite.c`, line 56; `tests/rules/repo_rules.c`, lines 200-217

### Purpose:
Enforce that the documents of repo_template are filled in (Rule 3).

### Assertion expectation:
No `.md` file but `instructions.md` holds two opening braces before a capital, or the template's start or end mark.

## Test 75

### File residence:
`tests/maintainability/maintainability_suite.c`, line 58; `tests/rules/log_rules.c`, lines 176-193

### Purpose:
Enforce the form of the development log (Rules 18 to 21).

### Assertion expectation:
`DEVLOG1.md` has its title, at most 1000 lines, dated days listed in order at its top, and times as hh:mm:ss.

## Test 76

### File residence:
`tests/maintainability/maintainability_suite.c`, line 60; `tests/rules/log_rules.c`, lines 221-248

### Purpose:
Enforce that TESTS.md numbers its tests 1, 2, 3 in order, with short fields (Rules 5, 6).

### Assertion expectation:
Headings and contents both count up from 1, agree, stay at most 100, and no field line passes 120 characters.

## Test 77

### File residence:
`tests/maintainability/maintainability_suite.c`, line 62; `tests/rules/log_rules.c`, lines 264-279

### Purpose:
Enforce that TESTS.md documents every test and no more (Rule 6).

### Assertion expectation:
The `## Test` headings are as many as the `case` lines of the three suites.

## Test 78

### File residence:
`tests/maintainability/maintainability_suite.c`, line 64; `tests/rules/log_rules.c`, lines 281-295

### Purpose:
Enforce that every term is one short line (Rule 9).

### Assertion expectation:
No line of `TERMINOLOGY.md` passes 120 characters.
