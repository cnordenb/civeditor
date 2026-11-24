# Tests

This project strives to have sensible testing. The goal is not to have 100% test coverage but to have tests which _sufficiently_ ensure the functional completeness, functional correctness, maintainability, and accessibility of civeditor. For more guidelines on software quality and accessibility, see [ISO/IEC 25010:2011](https://cdn.standards.iteh.ai/samples/35733/2ca18b477b7845a5b8cae39d6de0c098/ISO-IEC-25010-2011.pdf), [ISO/IEC 25010:2023](https://cdn.standards.iteh.ai/samples/78176/13ff8ea97048443f99318920757df124/ISO-IEC-25010-2023.pdf) and [WCAG 2.2](https://www.w3.org/TR/WCAG22/).

This document lists each test, its purpose, and in which test file it resides. This document is an effort to mitigate redundant tests and ensure test quality. It is intended as an aid in the trouble-shooting of potential false positives.

Test descriptions must remain brief and parameters (eg. Purpose) may not exceed 120 characters. Tests are ordered in order of their execution.

The test report names each test by its number here, such as "Test 15 failed": a test's number is its place in the order
the tests run. So when a test is added, removed or moved, renumber this document to match.

Every test serves a requirement, or catches again a bug found before, and its purpose names the requirement, rule
or bug (Rules 5 and 6). Rule 4, which asks only that there are tests, is never a test's purpose.

The tests are in four categories, each in files of its own (Rule 34). The core tests are at most as many as the
requirements, rules and bugs together (Rule 31); the accessibility tests serve `ACCESSIBILITY.md` (Rule 32); a
regression test catches again a bug that the log names, or covers what a human asked it to; the redundancy tests are
the others that the crapper and the mutator need (Req. 10 to 12). The core and accessibility tests are numbered
together, as `Test 12`, the regression and redundancy tests each on their own.

The tests run with the build: `build.exe` (or `build.exe quick`) in the project's folder. `build\tests.exe` runs them
again without building. A file and lines given as `suite.c`, line N, is the line that names the test; the lines after
it are the code that makes the check.

## Table of contents per category

Core tests of functional completeness & correctness:

- [Test 1](#test-1): the local mods folder of a profile is found
- [Test 2](#test-2): undo brings back the civ before a change
- [Test 3](#test-3): a 25th bonus is refused
- [Test 4](#test-4): the mod's folder name is made from the mod's name
- [Test 5](#test-5): the replaced civ loses its own techs
- [Test 6](#test-6): the report counts a civ bonus the catalog lacks as skipped
- [Test 7](#test-7): a new civ is saved as civeditor civ by default
- [Test 8](#test-8): a wheel turn between the lists redraws nothing when the step fits
- [Test 9](#test-9): a dropdown opens without animation
- [Test 10](#test-10): a number of the team bonus is set in its field
- [Test 11](#test-11): a list that is erased keeps the picture of its rows

Accessibility tests:

- [Test 12](#test-12): every text contrasts at least 4.5:1 with its background
- [Test 13](#test-13): every input has a visible label
- [Test 14](#test-14): every control one acts on is at least 44 by 44
- [Test 15](#test-15): no two controls of a step overlap, even in the smallest window
- [Test 16](#test-16): the window's title names the civ and the step
- [Test 17](#test-17): the list of steps is on every step, with Back and Next wherever they lead
- [Test 18](#test-18): a window narrower than the wide layout reflows
- [Test 19](#test-19): a window shorter than the wide layout reflows
- [Test 20](#test-20): a window as large as the wide layout does not reflow
- [Test 21](#test-21): no control reaches past a window 320 wide
- [Test 22](#test-22): no two controls overlap in a window 320 wide
- [Test 23](#test-23): every control one acts on is at least 44 by 44 in a window 320 wide
- [Test 24](#test-24): a step is as high as its last control and a margin
- [Test 25](#test-25): a control below the window is scrolled up into it
- [Test 26](#test-26): a control above the window is scrolled down into it
- [Test 27](#test-27): a control in the window leaves it where it is
- [Test 28](#test-28): the window scrolls no further than its content
- [Test 29](#test-29): the window scrolls no higher than its top
- [Test 30](#test-30): a window that holds its content does not scroll
- [Test 31](#test-31): the first control of a step comes under the help
- [Test 32](#test-32): Back and Next come under the status line
- [Test 33](#test-33): a control starts a margin from the left
- [Test 34](#test-34): Next ends a margin from the right
- [Test 35](#test-35): the Tab key reaches every control of every step
- [Test 36](#test-36): the list of steps shows the current step as the chosen one
- [Test 37](#test-37): the Tab key follows the order in which a step is read
- [Test 38](#test-38): a ticked row leaves the list of bonuses where it was scrolled
- [Test 39](#test-39): every control tells a screen reader a name that begins with its label
- [Test 40](#test-40): a ticked row is told to a screen reader as chosen
- [Test 41](#test-41): moving the focus through every control changes nothing
- [Test 42](#test-42): the Tab key leaves every control
- [Test 43](#test-43): Windows is told that the window's language is English
- [Test 44](#test-44): the help key says what the control in focus is for
- [Test 45](#test-45): the status line is a live region that a screen reader is told of
- [Test 46](#test-46): the text and the layout grow with the text size of Windows, up to 225%
- [Test 47](#test-47): the control in focus has a solid frame at least two pixels thick
- [Test 48](#test-48): a ticked row is drawn with a check, not a filled box
- [Test 49](#test-49): after an undo, the base civ shown is the design's again
- [Test 50](#test-50): a narrow window places its controls in one column
- [Test 51](#test-51): the control in focus is scrolled into a short window
- [Test 52](#test-52): the window may be as small as 320 by 256
- [Test 53](#test-53): Scroll down moves a short window down a page
- [Test 54](#test-54): Scroll up takes back a Scroll down
- [Test 55](#test-55): the wheel scrolls a short window
- [Test 56](#test-56): a long row of a narrow list is higher than a short one
- [Test 57](#test-57): a short row of a list is as high as a target

Core tests of maintainability:

- [Test 58](#test-58): no comments in the code
- [Test 59](#test-59): no function over 60 lines
- [Test 60](#test-60): no line of code over 120 characters
- [Test 61](#test-61): no file over 400 lines of code
- [Test 62](#test-62): no goto, setjmp or longjmp
- [Test 63](#test-63): no folder over 7 files, the main folder 8
- [Test 64](#test-64): no two files share a name
- [Test 65](#test-65): only C code and documents in the project
- [Test 66](#test-66): every placeholder of the template is filled in
- [Test 67](#test-67): the development log keeps its form
- [Test 68](#test-68): TESTS.md numbers its tests in order
- [Test 69](#test-69): TESTS.md documents every test
- [Test 70](#test-70): every line of TERMINOLOGY.md is short

Regression tests, numbered on their own, as `Regression test 1`:

- [Regression test 1](#regression-test-1): ticking the arbalester ticks the archer
- [Regression test 2](#regression-test-2): unticking the archer unticks the arbalester
- [Regression test 3](#regression-test-3): a bonus of the game's techs taken twice doubles the changes of its copy
- [Regression test 4](#regression-test-4): the original's example civ keeps its language
- [Regression test 5](#regression-test-5): the summary labels the civ's name
- [Regression test 6](#regression-test-6): the last step shows no Next button
- [Regression test 7](#regression-test-7): the civ descriptions are written in id order
- [Regression test 8](#regression-test-8): a skipped hero's name is not written
- [Regression test 9](#regression-test-9): the civ list points at the civ's own description
- [Regression test 10](#regression-test-10): a new civ starts with the tech tree of its civ
- [Regression test 11](#regression-test-11): a building the civ lacks is not available in its tech tree
- [Regression test 12](#regression-test-12): a building the civ has is available in its tech tree
- [Regression test 13](#regression-test-13): the catalog holds every look line of its file
- [Regression test 14](#regression-test-14): a tree that swaps a node of the base civ is changed
- [Regression test 15](#regression-test-15): bonuses that swap one of the base civ are changed
- [Regression test 16](#regression-test-16): a team bonus other than the base civ's is a change
- [Regression test 17](#regression-test-17): the base civ's own team bonus is no change
- [Regression test 18](#regression-test-18): a list joined by commas is scaled as one
- [Regression test 19](#regression-test-19): a cost cut by half and slid to twice cuts it by three quarters
- [Regression test 20](#regression-test-20): no step is shown as done when the window opens
- [Regression test 21](#regression-test-21): the exit command closes a window without changes
- [Regression test 22](#regression-test-22): a list's scroll bar shows its arrows while the mouse is away
- [Regression test 23](#regression-test-23): the window's scroll bar shows its arrows while the mouse is away
- [Regression test 24](#regression-test-24): a step ends a margin from the window's scroll bar
- [Regression test 25](#regression-test-25): each wheel notch moves a list one row its way
- [Regression test 26](#regression-test-26): a press on either arrow moves a list one row its way
- [Regression test 27](#regression-test-27): a shown list scrolls without animation
- [Regression test 28](#regression-test-28): a held arrow keeps scrolling a list
- [Regression test 29](#regression-test-29): a press between a list's arrows moves it a page
- [Regression test 30](#regression-test-30): each wheel notch moves an open dropdown one row

Redundancy tests (Req. 10 to 12), run only with the word `redundancy`, as `build.exe quick redundancy`:

- [Redundancy test 1](#redundancy-test-1): a civ of the game gives the design its name
- [Redundancy test 2](#redundancy-test-2): a civ of the game names its unique unit as the game does
- [Redundancy test 3](#redundancy-test-3): a civ of the game names its Castle Age tech as the game does
- [Redundancy test 4](#redundancy-test-4): a civ of the game names its Imperial Age tech as the game does
- [Redundancy test 5](#redundancy-test-5): a civ of the game gives the design its tagline
- [Redundancy test 6](#redundancy-test-6): a civ of the game gives the design its description
- [Redundancy test 7](#redundancy-test-7): a civ of the game takes away the hero of the civ before
- [Redundancy test 8](#redundancy-test-8): picking a unique unit names it as the game does
- [Redundancy test 9](#redundancy-test-9): picking a Castle Age tech names it as the game does
- [Redundancy test 10](#redundancy-test-10): picking a hero names it as the game does
- [Redundancy test 11](#redundancy-test-11): unticking Hero takes away the hero's name
- [Redundancy test 12](#redundancy-test-12): a hero whose name is emptied lacks a name
- [Redundancy test 13](#redundancy-test-13): a civ of the game shows no step done
- [Redundancy test 14](#redundancy-test-14): Finish is done once a whole civ differs from the game's
- [Redundancy test 15](#redundancy-test-15): a changed civ is counted
- [Redundancy test 16](#redundancy-test-16): a civ as the game has it is left out of the mod
- [Redundancy test 17](#redundancy-test-17): a civ changed again holds its newer edits in the mod
- [Redundancy test 18](#redundancy-test-18): a civ made as the game has it again is taken out of the mod
- [Redundancy test 19](#redundancy-test-19): the draft of a civ is kept while another is edited
- [Redundancy test 20](#redundancy-test-20): a changed civ is marked as changed
- [Redundancy test 21](#redundancy-test-21): a civ as the game has it is not marked as changed
- [Redundancy test 22](#redundancy-test-22): the civs of the mod are given in the order of their slots
- [Redundancy test 23](#redundancy-test-23): a roster started anew holds no draft
- [Redundancy test 24](#redundancy-test-24): a civ past the last slot is left out of the mod
- [Redundancy test 25](#redundancy-test-25): a changed civ in the first slot is counted
- [Redundancy test 26](#redundancy-test-26): a changed civ in the first slot is given to the mod
- [Redundancy test 27](#redundancy-test-27): a changed civ in the first slot is marked as changed
- [Redundancy test 28](#redundancy-test-28): a roster started anew forgets a changed civ in the first slot
- [Redundancy test 29](#redundancy-test-29): a civ of another first look differs from its civ of the game
- [Redundancy test 30](#redundancy-test-30): a civ of another first cost differs from its civ of the game
- [Redundancy test 31](#redundancy-test-31): a civ given a bonus of the civ before differs from its civ of the game
- [Redundancy test 32](#redundancy-test-32): the game gives a civ its tagline
- [Redundancy test 33](#redundancy-test-33): the description of a civ starts after its tagline
- [Redundancy test 34](#redundancy-test-34): a line break of the game breaks the description
- [Redundancy test 35](#redundancy-test-35): a quote of the game is kept in the description
- [Redundancy test 36](#redundancy-test-36): a civ the game does not describe keeps no tagline
- [Redundancy test 37](#redundancy-test-37): texts are read from a game that is found
- [Redundancy test 38](#redundancy-test-38): a civ the game does not describe keeps no description
- [Redundancy test 39](#redundancy-test-39): a text without its closing quote ends with its line
- [Redundancy test 40](#redundancy-test-40): the spaces and breaks at the end of a text are cut
- [Redundancy test 41](#redundancy-test-41): a first line without civilization is the whole tagline
- [Redundancy test 42](#redundancy-test-42): a tagline longer than its room is cut to fit
- [Redundancy test 43](#redundancy-test-43): the analyzed build compiles with the analyzer
- [Redundancy test 44](#redundancy-test-44): an edit knows the first place it patched
- [Redundancy test 45](#redundancy-test-45): a job with no civ is not finished
- [Redundancy test 46](#redundancy-test-46): a build of an unfinished civ fails
- [Redundancy test 47](#redundancy-test-47): the description of a civ takes the place of the game's once
- [Redundancy test 48](#redundancy-test-48): the other descriptions of the game are kept
- [Redundancy test 49](#redundancy-test-49): a civ past the descriptions of the game gets its own
- [Redundancy test 50](#redundancy-test-50): a civ left from a bigger build gets no description
- [Redundancy test 51](#redundancy-test-51): the description of the first civ described is written once
- [Redundancy test 52](#redundancy-test-52): a tech takes no more requirements than it has room for
- [Redundancy test 53](#redundancy-test-53): the strings of a civ fail once its description passes its room
- [Redundancy test 54](#redundancy-test-54): a civ's name leaves one castle tech
- [Redundancy test 55](#redundancy-test-55): a civ's name leaves the castle tech of that civ
- [Redundancy test 56](#redundancy-test-56): a civ's name leaves the imperial tech of that civ
- [Redundancy test 57](#redundancy-test-57): a civ's name leaves the unique unit of that civ
- [Redundancy test 58](#redundancy-test-58): a civ's name leaves one unique unit
- [Redundancy test 59](#redundancy-test-59): a civ's name leaves every civ bonus of that civ
- [Redundancy test 60](#redundancy-test-60): a civ's name leaves no civ bonus of another civ
- [Redundancy test 61](#redundancy-test-61): a civ's name leaves the team bonus of that civ
- [Redundancy test 62](#redundancy-test-62): a civ's name leaves the nodes that civ has
- [Redundancy test 63](#redundancy-test-63): a civ's name leaves no node that civ lacks
- [Redundancy test 64](#redundancy-test-64): the middle of a civ's name is not its name
- [Redundancy test 65](#redundancy-test-65): a civ's name leaves no hero
- [Redundancy test 66](#redundancy-test-66): a filter still leaves the rows that hold its text
- [Redundancy test 67](#redundancy-test-67): Civ to edit leaves out a civ that is not ranked
- [Redundancy test 68](#redundancy-test-68): a civ without an unranked line is ranked
- [Redundancy test 69](#redundancy-test-69): a civ with an unranked line is not ranked
- [Redundancy test 70](#redundancy-test-70): the first filter names its list
- [Redundancy test 71](#redundancy-test-71): the last filter names its list
- [Redundancy test 72](#redundancy-test-72): a control that is no filter names no list
- [Redundancy test 73](#redundancy-test-73): a list of numbers holds its first
- [Redundancy test 74](#redundancy-test-74): a list of numbers holds its last
- [Redundancy test 75](#redundancy-test-75): a list of numbers holds no number past its count
- [Redundancy test 76](#redundancy-test-76): an empty list of numbers holds nothing
- [Redundancy test 77](#redundancy-test-77): the look choices are shown on no step
- [Redundancy test 78](#redundancy-test-78): a build with nothing skipped is whole
- [Redundancy test 79](#redundancy-test-79): a build that skipped the team bonus is not whole
- [Redundancy test 80](#redundancy-test-80): a build that skipped a multiplier is not whole
- [Redundancy test 81](#redundancy-test-81): a build that skipped a unique tech is not whole
- [Redundancy test 82](#redundancy-test-82): a build that skipped the unique unit is not whole
- [Redundancy test 83](#redundancy-test-83): a build whose tech tree overflowed is not whole
- [Redundancy test 84](#redundancy-test-84): a build that skipped the hero is not whole
- [Redundancy test 85](#redundancy-test-85): clearing the civ bonuses of a civ with one is a change
- [Redundancy test 86](#redundancy-test-86): clearing the civ bonuses of a civ with none is no change
- [Redundancy test 87](#redundancy-test-87): clearing the civ bonuses leaves none
- [Redundancy test 88](#redundancy-test-88): the built mark takes no focus
- [Redundancy test 89](#redundancy-test-89): the panel of the selected team bonus is no higher in a higher window
- [Redundancy test 90](#redundancy-test-90): each signed percent of a list that counts twice is scaled
- [Redundancy test 91](#redundancy-test-91): a sign inside a word is no sign
- [Redundancy test 92](#redundancy-test-92): a number after a space is not in the list before it
- [Redundancy test 93](#redundancy-test-93): a comma that no space follows is not in the list before it
- [Redundancy test 94](#redundancy-test-94): a bonus counted 1.5 times is read back from a civ file as 1.5
- [Redundancy test 95](#redundancy-test-95): a percent of a bonus counted 1.5 times is raised to 1.5
- [Redundancy test 96](#redundancy-test-96): a percent without a sign before cheaper counted 1.5 times is a cut raised to 1.5
- [Redundancy test 97](#redundancy-test-97): a change multiplied and counted 1.5 times is raised to 1.5
- [Redundancy test 98](#redundancy-test-98): an armor added and counted 1.5 times is rounded to the nearest
- [Redundancy test 99](#redundancy-test-99): an attack multiplied and counted 1.5 times is raised to 1.5
- [Redundancy test 100](#redundancy-test-100): a spawn counted 1.5 times makes the nearest whole number of units
- [Redundancy test 101](#redundancy-test-101): a decimal of nine places is written whole
- [Redundancy test 102](#redundancy-test-102): a decimal of ten places is refused
- [Redundancy test 103](#redundancy-test-103): a negative whole number is written with its sign
- [Redundancy test 104](#redundancy-test-104): a number as long as the room for it is read
- [Redundancy test 105](#redundancy-test-105): a number as large as ten to the eighteenth is read
- [Redundancy test 106](#redundancy-test-106): a text in the place of a number is no decimal
- [Redundancy test 107](#redundancy-test-107): a count past ten is no multiplier
- [Redundancy test 108](#redundancy-test-108): a multiplier of nothing is refused
- [Redundancy test 109](#redundancy-test-109): the percent of a chosen bonus is a number to set where it is shown
- [Redundancy test 110](#redundancy-test-110): a percent cut typed gives the count that shows it
- [Redundancy test 111](#redundancy-test-111): a power typed gives the count that shows it
- [Redundancy test 112](#redundancy-test-112): an amount typed gives the count that shows it
- [Redundancy test 113](#redundancy-test-113): a chosen bonus without numbers reads as it is
- [Redundancy test 114](#redundancy-test-114): a percent without a sign before cheaper is a number to set
- [Redundancy test 115](#redundancy-test-115): a cut of a hundred percent is not a number to set
- [Redundancy test 116](#redundancy-test-116): a percent of nothing is not a number to set
- [Redundancy test 117](#redundancy-test-117): an amount of nothing is not a number to set
- [Redundancy test 118](#redundancy-test-118): a power of one is not a number to set
- [Redundancy test 119](#redundancy-test-119): a row holds sixteen numbers to set at most
- [Redundancy test 120](#redundancy-test-120): a percent without a sign before another word is not a number to set
- [Redundancy test 121](#redundancy-test-121): unticking a node unticks the descendants listed before it
- [Redundancy test 122](#redundancy-test-122): a civ bonus not ticked counts no times
- [Redundancy test 123](#redundancy-test-123): the report counts no multiplier of a bonus of civ techs or of a bonus counted once
- [Redundancy test 124](#redundancy-test-124): ticking a node ticks the needs listed before it
- [Redundancy test 125](#redundancy-test-125): ticking a node into a full tree changes nothing
- [Redundancy test 126](#redundancy-test-126): a value typed into a cost cut by half sets the count that shows it
- [Redundancy test 127](#redundancy-test-127): a value typed with its sign and percent is taken
- [Redundancy test 128](#redundancy-test-128): the most value of a number is taken
- [Redundancy test 129](#redundancy-test-129): the value of a number is shown with its unit, as in its bonus
- [Redundancy test 130](#redundancy-test-130): the range of a number names its least and its most value
- [Redundancy test 131](#redundancy-test-131): the slider stands at the count of the bonus in hundredths
- [Redundancy test 132](#redundancy-test-132): the slider sets the count it stands at
- [Redundancy test 133](#redundancy-test-133): the field of a number holds its sign and digits without the percent
- [Redundancy test 134](#redundancy-test-134): the field of an amount holds its sign and digits
- [Redundancy test 135](#redundancy-test-135): a word too wide for its line goes on at the start of the next line
- [Redundancy test 136](#redundancy-test-136): the number of a row is a piece of its sign and digits alone
- [Redundancy test 137](#redundancy-test-137): the piece of a number is found in its row
- [Redundancy test 138](#redundancy-test-138): a row past the last chosen bonus is not shown
- [Redundancy test 139](#redundancy-test-139): the least value of a number sets its count back to 1
- [Redundancy test 140](#redundancy-test-140): a number past the last of a row has no value
- [Redundancy test 141](#redundancy-test-141): a chosen bonus not in the catalog has no number to set
- [Redundancy test 142](#redundancy-test-142): a number past the last of a row has no piece in it
- [Redundancy test 143](#redundancy-test-143): the slider's card is centered under its field
- [Redundancy test 144](#redundancy-test-144): the slider's card under a field near the left edge stays inside it
- [Redundancy test 145](#redundancy-test-145): the slider's card under a field near the right edge stays inside it
- [Redundancy test 146](#redundancy-test-146): a value with letters after it is refused
- [Redundancy test 147](#redundancy-test-147): a value past the most of a number is refused
- [Redundancy test 148](#redundancy-test-148): a place of the slider stands for its count in hundredths
- [Redundancy test 149](#redundancy-test-149): the slider shrinks to the room of a narrow window
- [Redundancy test 150](#redundancy-test-150): a row is cut into its words, spaces, numbers and units
- [Redundancy test 151](#redundancy-test-151): a row of more pieces than it has room for is cut at the room
- [Redundancy test 152](#redundancy-test-152): a word of several pieces goes on to the next line whole
- [Redundancy test 153](#redundancy-test-153): a word that would pass the end of its line goes on to the next
- [Redundancy test 154](#redundancy-test-154): a first word wider than its line stays on the first line
- [Redundancy test 155](#redundancy-test-155): a row wrapped twice takes three lines
- [Redundancy test 156](#redundancy-test-156): the slider stands after the least value on its card
- [Redundancy test 157](#redundancy-test-157): a word wider than its line is not broken between its pieces
- [Redundancy test 158](#redundancy-test-158): the last word of a row that just fits its line stays on it
- [Redundancy test 159](#redundancy-test-159): a row that ends in a space takes only its own lines
- [Redundancy test 160](#redundancy-test-160): a row of Chosen bonuses shows its bonus as it counts
- [Redundancy test 161](#redundancy-test-161): the range of an amount names its least and its most value
- [Redundancy test 162](#redundancy-test-162): a number without a sign is a piece of its digits alone
- [Redundancy test 163](#redundancy-test-163): the piece of a number at the start of its row is its first
- [Redundancy test 164](#redundancy-test-164): a number of a row laid out before has no piece in a shorter row
- [Redundancy test 165](#redundancy-test-165): the slider of a card wider than its window has no width
- [Redundancy test 166](#redundancy-test-166): the row of the selected team bonus shows it as it counts
- [Redundancy test 167](#redundancy-test-167): a value typed into the number of the team bonus sets how many times it counts
- [Redundancy test 168](#redundancy-test-168): with no team bonus selected its panel has no row
- [Redundancy test 169](#redundancy-test-169): the mod describes the team bonus as it counts
- [Redundancy test 170](#redundancy-test-170): the summary shows the team bonus as it counts
- [Redundancy test 171](#redundancy-test-171): a command to set a stat of the unique unit sets it
- [Redundancy test 172](#redundancy-test-172): undo takes back a stat set of the unique unit
- [Redundancy test 173](#redundancy-test-173): picking another unique unit clears the stats set
- [Redundancy test 174](#redundancy-test-174): a stat of the unique unit out of its range is refused
- [Redundancy test 175](#redundancy-test-175): a stat set of the unique unit is saved and opened again
- [Redundancy test 176](#redundancy-test-176): the stats of the unique unit in a civ file of the original are read
- [Redundancy test 177](#redundancy-test-177): a stat set of the unique unit changes the civ
- [Redundancy test 178](#redundancy-test-178): a cost set of the unique unit is saved and opened again
- [Redundancy test 179](#redundancy-test-179): picking the same unique unit again keeps the stats set
- [Redundancy test 180](#redundancy-test-180): a cost set of the unique unit changes the civ
- [Redundancy test 181](#redundancy-test-181): a command to set a value of a unique tech sets it
- [Redundancy test 182](#redundancy-test-182): picking another unique tech clears the values set
- [Redundancy test 183](#redundancy-test-183): a value of a unique tech out of its range is refused
- [Redundancy test 184](#redundancy-test-184): a value set of a unique tech is saved and opened again
- [Redundancy test 185](#redundancy-test-185): a value not set of a unique tech stays not set when saved and opened
- [Redundancy test 186](#redundancy-test-186): a cost of four zeros in a civ file is the game's cost
- [Redundancy test 187](#redundancy-test-187): a cost of 0 set of a unique tech is saved and opened again
- [Redundancy test 188](#redundancy-test-188): a civ bonus ticked as an effect of a unique tech is kept with it
- [Redundancy test 189](#redundancy-test-189): the multiplier of an effect of a unique tech is set
- [Redundancy test 190](#redundancy-test-190): the effects of a unique tech are saved and opened again
- [Redundancy test 191](#redundancy-test-191): a unique tech with effects is saved as the original's custom tech
- [Redundancy test 192](#redundancy-test-192): an effect of a unique tech changes the civ
- [Redundancy test 193](#redundancy-test-193): a unique tech without effects is saved as the original's vanilla tech
- [Redundancy test 194](#redundancy-test-194): a civ bonus ticked as an effect of a unique tech is found ticked
- [Redundancy test 195](#redundancy-test-195): the hit points and speed set of the unique unit are built
- [Redundancy test 196](#redundancy-test-196): the attack set of the unique unit is built
- [Redundancy test 197](#redundancy-test-197): the armours set of the unique unit are built
- [Redundancy test 198](#redundancy-test-198): the range and reload time set of the unique unit are built
- [Redundancy test 199](#redundancy-test-199): the costs set of the unique unit are built
- [Redundancy test 200](#redundancy-test-200): a cost of four resources is counted as not built
- [Redundancy test 201](#redundancy-test-201): the elite unit takes the change made to the unit
- [Redundancy test 202](#redundancy-test-202): a build says that a stat of the unique unit was not built
- [Redundancy test 203](#redundancy-test-203): a build that left out a stat of the unique unit is not whole
- [Redundancy test 204](#redundancy-test-204): a cost not set of a unique tech is built as the game's
- [Redundancy test 205](#redundancy-test-205): the research time set of a unique tech is built
- [Redundancy test 206](#redundancy-test-206): an effect of a unique tech is built into what the tech does, by its multiplier
- [Redundancy test 207](#redundancy-test-207): the mod describes a unique tech with effects by its effects as they count
- [Redundancy test 208](#redundancy-test-208): an effect of a unique tech takes the changes of the game techs of its bonus
- [Redundancy test 209](#redundancy-test-209): the elite unit is built with the change made to the unit's hit points, from its own
- [Redundancy test 210](#redundancy-test-210): the elite unit is built with the change made to the unit's reload time, from its own
- [Redundancy test 211](#redundancy-test-211): the attack set of a unit with an attack bonus is built into its attack
- [Redundancy test 212](#redundancy-test-212): hit points set to the least of their range are built
- [Redundancy test 213](#redundancy-test-213): hit points set to the most of their range are built
- [Redundancy test 214](#redundancy-test-214): a cost set of the unique unit keeps its other costs and its population
- [Redundancy test 215](#redundancy-test-215): a food cost of 1 set alone of the unique unit is built
- [Redundancy test 216](#redundancy-test-216): a cost set to 0 is taken out of the costs of the unique unit
- [Redundancy test 217](#redundancy-test-217): a cost of 1 of a unique tech is built
- [Redundancy test 218](#redundancy-test-218): an effect ticked and unticked again is not built into the unique tech
- [Redundancy test 219](#redundancy-test-219): a build whose unique unit is not in the catalog leaves out no stat
- [Redundancy test 220](#redundancy-test-220): the mod describes a unique tech with effects alike when it is built again
- [Redundancy test 221](#redundancy-test-221): the first row of the selected unit holds its name
- [Redundancy test 222](#redundancy-test-222): the cost row of the selected unit names each resource with its number
- [Redundancy test 223](#redundancy-test-223): the stats row of the selected unit names each stat with its number
- [Redundancy test 224](#redundancy-test-224): a stat set of the selected unit is shown in its row
- [Redundancy test 225](#redundancy-test-225): a stat of the selected unit that the catalog does not know is shown as a dash
- [Redundancy test 226](#redundancy-test-226): the field of a stat of the selected unit holds the number alone
- [Redundancy test 227](#redundancy-test-227): a value typed into the field of a stat sets that stat
- [Redundancy test 228](#redundancy-test-228): a value typed into the field of a cost sets that cost
- [Redundancy test 229](#redundancy-test-229): a speed typed into its field is kept in hundredths
- [Redundancy test 230](#redundancy-test-230): a value typed with a minus sets a stat below zero
- [Redundancy test 231](#redundancy-test-231): a value typed past the range of a stat is refused
- [Redundancy test 232](#redundancy-test-232): the range of a stat is named by its least and most values
- [Redundancy test 233](#redundancy-test-233): the field of a stat is named after the stat
- [Redundancy test 234](#redundancy-test-234): with no unit selected the panel holds only the row that says so
- [Redundancy test 235](#redundancy-test-235): a selected unit not in the catalog is said to be so
- [Redundancy test 236](#redundancy-test-236): the numbers of the selected unit have no slider
- [Redundancy test 237](#redundancy-test-237): the least value of the range of a stat typed into its field sets it
- [Redundancy test 238](#redundancy-test-238): the most value of the range of a stat typed into its field sets it
- [Redundancy test 239](#redundancy-test-239): the second row of a selected tech says what it does
- [Redundancy test 240](#redundancy-test-240): the cost row of a selected tech names each resource with its number
- [Redundancy test 241](#redundancy-test-241): the research time row of a selected tech says it in seconds
- [Redundancy test 242](#redundancy-test-242): a value typed into the field of a cost of a tech sets that cost
- [Redundancy test 243](#redundancy-test-243): the field of the research time is named after it
- [Redundancy test 244](#redundancy-test-244): with no tech selected the panel holds only the row that says so
- [Redundancy test 245](#redundancy-test-245): the range of a cost of a tech runs from 0 to 9999
- [Redundancy test 246](#redundancy-test-246): with an effect ticked the second row of a selected tech is its cost
- [Redundancy test 247](#redundancy-test-247): the first row of a selected tech holds its name
- [Redundancy test 248](#redundancy-test-248): an effect of a selected tech takes the place of what its game tech does
- [Redundancy test 249](#redundancy-test-249): the number of an effect of a selected tech slides
- [Redundancy test 250](#redundancy-test-250): a value typed into the number of an effect of a tech sets its multiplier
- [Redundancy test 251](#redundancy-test-251): the field of the number of an effect of a tech is named by its row
- [Redundancy test 252](#redundancy-test-252): the cost of a unique unit comes from the game's data file
- [Redundancy test 253](#redundancy-test-253): a resource that a unique unit does not cost in the game's data file is 0
- [Redundancy test 254](#redundancy-test-254): the attack of a unique unit comes from the game's data file
- [Redundancy test 255](#redundancy-test-255): a unique unit that the game's data file lacks keeps the catalog's numbers
- [Redundancy test 256](#redundancy-test-256): a custom unit keeps the catalog's numbers
- [Redundancy test 257](#redundancy-test-257): the cost of a unique tech comes from the game's data file
- [Redundancy test 258](#redundancy-test-258): the research time of a unique tech comes from the game's data file
- [Redundancy test 259](#redundancy-test-259): a unique tech whose game tech the data file lacks keeps the catalog's numbers
- [Redundancy test 260](#redundancy-test-260): a unique tech of no game tech keeps the catalog's numbers
- [Redundancy test 261](#redundancy-test-261): the numbers are read from the data file in the game's folder
- [Redundancy test 262](#redundancy-test-262): a folder without the game's data file leaves the catalog's numbers
- [Redundancy test 263](#redundancy-test-263): the range of a unique unit comes as the game shows it
- [Redundancy test 264](#redundancy-test-264): a node that a civ's tech tree file of the game has is had by that civ
- [Redundancy test 265](#redundancy-test-265): a node that a civ's tech tree file of the game lacks is not had by that civ
- [Redundancy test 266](#redundancy-test-266): a node not available in a civ's tech tree file is not had by that civ
- [Redundancy test 267](#redundancy-test-267): a civ without a tech tree file in the game keeps the nodes of the catalog
- [Redundancy test 268](#redundancy-test-268): a resource that a unique tech does not cost in the game's data file is 0
- [Redundancy test 269](#redundancy-test-269): the first unique unit of the catalog is read from the game's data file
- [Redundancy test 270](#redundancy-test-270): the window opens with the Britons to edit
- [Redundancy test 271](#redundancy-test-271): the Name field holds the name of the civ to edit
- [Redundancy test 272](#redundancy-test-272): another civ keeps the edits of the civ before
- [Redundancy test 273](#redundancy-test-273): the review counts no civ changed at first
- [Redundancy test 274](#redundancy-test-274): Finish counts a civ that was changed
- [Redundancy test 275](#redundancy-test-275): Finish counts no civ that was changed back
- [Redundancy test 276](#redundancy-test-276): the review text shows no caret when clicked
- [Redundancy test 277](#redundancy-test-277): a build with no civ changed says so
- [Redundancy test 278](#redundancy-test-278): a mod is named civeditor_mod unless named otherwise
- [Redundancy test 279](#redundancy-test-279): a filter leaves the bonuses that hold it and no other
- [Redundancy test 280](#redundancy-test-280): the categories together hold every bonus once
- [Redundancy test 281](#redundancy-test-281): an empty mod name builds no mod
- [Redundancy test 282](#redundancy-test-282): a long civ name is cut in the name of its file
- [Redundancy test 283](#redundancy-test-283): a text field shows its caret when it takes the focus
- [Redundancy test 284](#redundancy-test-284): the window opens with the focus on the civ to edit
- [Redundancy test 285](#redundancy-test-285): the menu Go names each step as its title reads
- [Redundancy test 286](#redundancy-test-286): a wheel turn on the minimized window leaves the step unscrolled
- [Redundancy test 287](#redundancy-test-287): Choose hero lists every hero of the catalog once
- [Redundancy test 288](#redundancy-test-288): the list of Choose hero is labelled Heroes
- [Redundancy test 289](#redundancy-test-289): Choose hero opens with the focus on its list
- [Redundancy test 290](#redundancy-test-290): a name typed in Choose hero stays when its list takes the focus again
- [Redundancy test 291](#redundancy-test-291): the first hero of Choose hero can be picked
- [Redundancy test 292](#redundancy-test-292): a one-letter name typed in Choose hero is taken
- [Redundancy test 293](#redundancy-test-293): a civ named with one letter is saved under that letter
- [Redundancy test 294](#redundancy-test-294): a value typed into the field of a number and left sets the count that shows it
- [Redundancy test 295](#redundancy-test-295): a value typed past the most of a number names its range
- [Redundancy test 296](#redundancy-test-296): Enter in the field of a number sets the count that shows it
- [Redundancy test 297](#redundancy-test-297): the field of a number in a chosen bonus holds the number alone
- [Redundancy test 298](#redundancy-test-298): the slider is shown just below the field of the number in focus
- [Redundancy test 299](#redundancy-test-299): the slider dragged shows in the field the value it would set
- [Redundancy test 300](#redundancy-test-300): the slider is hidden when the focus leaves the fields of numbers
- [Redundancy test 301](#redundancy-test-301): Esc in the field of a number hides its slider
- [Redundancy test 302](#redundancy-test-302): a chosen bonus is shown as a label
- [Redundancy test 303](#redundancy-test-303): Tab from the field of a number moves the focus to its slider
- [Redundancy test 304](#redundancy-test-304): the field of the number in focus is framed
- [Redundancy test 305](#redundancy-test-305): the card shows the least value of its number left of the slider
- [Redundancy test 306](#redundancy-test-306): the card shows the most value of its number right of the slider
- [Redundancy test 307](#redundancy-test-307): a field that a change takes away gives the focus to the list of civ bonuses
- [Redundancy test 308](#redundancy-test-308): a field that a change takes away leaves the status line as it was
- [Redundancy test 309](#redundancy-test-309): the field of a number stands just after the text before it
- [Redundancy test 310](#redundancy-test-310): the field of a number holds a number wider than at either end of its range
- [Redundancy test 311](#redundancy-test-311): the frame of the number in focus is drawn whole within its label
- [Redundancy test 312](#redundancy-test-312): the frame of a number on a later line of its label is drawn around it
- [Redundancy test 313](#redundancy-test-313): the field of a number is just as tall as its text needs, and at least 24 pixels
- [Redundancy test 314](#redundancy-test-314): a value typed into a field of the selected unit and left sets its stat
- [Redundancy test 315](#redundancy-test-315): a value typed past the range of a stat of the selected unit names its range
- [Redundancy test 316](#redundancy-test-316): the field of a stat of the selected unit shows no slider
- [Redundancy test 317](#redundancy-test-317): a value typed into a field of the selected imperial tech and left sets it
- [Redundancy test 318](#redundancy-test-318): a civ bonus ticked in the effects of the castle tech is shown as what it does
- [Redundancy test 319](#redundancy-test-319): a failed check names its file, line and condition
- [Redundancy test 320](#redundancy-test-320): the original's example civ keeps its name
- [Redundancy test 321](#redundancy-test-321): the original's example civ keeps its bonuses
- [Redundancy test 322](#redundancy-test-322): the original's example civ keeps its unique unit
- [Redundancy test 323](#redundancy-test-323): a saved civ loads back unchanged
- [Redundancy test 324](#redundancy-test-324): a bonus's multiplier is read back from a civ file
- [Redundancy test 325](#redundancy-test-325): typing a name is undone at once
- [Redundancy test 326](#redundancy-test-326): redo makes an undone change again
- [Redundancy test 327](#redundancy-test-327): next walks every step in order
- [Redundancy test 328](#redundancy-test-328): the summary names what is still needed
- [Redundancy test 329](#redundancy-test-329): a dynamic Huffman stream inflates
- [Redundancy test 330](#redundancy-test-330): a fixed Huffman stream inflates
- [Redundancy test 331](#redundancy-test-331): a stored stream inflates back
- [Redundancy test 332](#redundancy-test-332): packed data unpacks to the same bytes
- [Redundancy test 333](#redundancy-test-333): repeated data packs to a tenth of its size
- [Redundancy test 334](#redundancy-test-334): an unchanged dat file is written back byte for byte
- [Redundancy test 335](#redundancy-test-335): an added effect is written as the game holds it
- [Redundancy test 336](#redundancy-test-336): an added tech is written as the game holds it
- [Redundancy test 337](#redundancy-test-337): a bonus without techs is built from its changes
- [Redundancy test 338](#redundancy-test-338): a bonus taken twice is built with its changes doubled
- [Redundancy test 339](#redundancy-test-339): the name is written under the replaced civ's name id
- [Redundancy test 340](#redundancy-test-340): a mod is not built from an unfinished civ
- [Redundancy test 341](#redundancy-test-341): a unique tech is aimed at the civ's own unique unit
- [Redundancy test 342](#redundancy-test-342): a unique tech's name is written under the string id of its slot
- [Redundancy test 343](#redundancy-test-343): a unique unit's name is written under the string id of its slot
- [Redundancy test 344](#redundancy-test-344): a building takes the look of its architecture's civ
- [Redundancy test 345](#redundancy-test-345): a monk takes the look of its monk skin
- [Redundancy test 346](#redundancy-test-346): a hero trains at the castle
- [Redundancy test 347](#redundancy-test-347): a unique tech's changes of one attribute by two amounts are one target
- [Redundancy test 348](#redundancy-test-348): a unique tech's changes of two armor classes are two targets
- [Redundancy test 349](#redundancy-test-349): a hero's name is written under the string id of its slot
- [Redundancy test 350](#redundancy-test-350): the civ list names the civ's own unique unit
- [Redundancy test 351](#redundancy-test-351): a unique unit missing from the civ list takes its own icon and texts
- [Redundancy test 352](#redundancy-test-352): a unit patch changes its civ's unit
- [Redundancy test 353](#redundancy-test-353): a train line moves where its civ's unit is trained
- [Redundancy test 354](#redundancy-test-354): a train line sets the button of its civ's unit
- [Redundancy test 355](#redundancy-test-355): a unit with nowhere to train leaves its bonus skipped
- [Redundancy test 356](#redundancy-test-356): a cost line sets a cost of its civ's unit
- [Redundancy test 357](#redundancy-test-357): a cost line reaches the last field of the costs
- [Redundancy test 358](#redundancy-test-358): a value the civ keeps stays the game's in the civ list
- [Redundancy test 359](#redundancy-test-359): choosing another civ takes away a regional node
- [Redundancy test 360](#redundancy-test-360): a base civ has the nodes its civ has in the game
- [Redundancy test 361](#redundancy-test-361): a base civ gives the civ exactly its bonuses
- [Redundancy test 362](#redundancy-test-362): a base civ gives the civ its team bonus
- [Redundancy test 363](#redundancy-test-363): a base civ gives the civ its unique unit
- [Redundancy test 364](#redundancy-test-364): a civ with an empty tree keeps the game's tech tree
- [Redundancy test 365](#redundancy-test-365): a civ keeps its villagers in its tech tree
- [Redundancy test 366](#redundancy-test-366): a building the civ has keeps the research it needs in its tech tree
- [Redundancy test 367](#redundancy-test-367): only the tech of a unit the civ lacks is disabled
- [Redundancy test 368](#redundancy-test-368): only a tech the civ lacks is disabled
- [Redundancy test 369](#redundancy-test-369): a civ keeps its villagers in the dat file
- [Redundancy test 370](#redundancy-test-370): a building the replaced civ lacked takes the status most civs give it
- [Redundancy test 371](#redundancy-test-371): the unique unit is made by a tech of the civ
- [Redundancy test 372](#redundancy-test-372): a unique tech is researched at the castle in its age
- [Redundancy test 373](#redundancy-test-373): a built unique tech changes the civ's own unit
- [Redundancy test 374](#redundancy-test-374): a chosen cost takes the place of the tech's own
- [Redundancy test 375](#redundancy-test-375): the built tech tree disables the tech of a unit the civ lacks
- [Redundancy test 376](#redundancy-test-376): the report counts the unique techs built
- [Redundancy test 377](#redundancy-test-377): the built unique unit is named by the string id of its slot
- [Redundancy test 378](#redundancy-test-378): a civ bonus made of a civ's tech is copied for the civ
- [Redundancy test 379](#redundancy-test-379): a copied tech's effect keeps the amounts of its changes
- [Redundancy test 380](#redundancy-test-380): a copied effect names the copy of a copied tech
- [Redundancy test 381](#redundancy-test-381): a civ bonus made of a generic tech unlocks it
- [Redundancy test 382](#redundancy-test-382): the report counts the civ bonuses built
- [Redundancy test 383](#redundancy-test-383): a bonus with nothing to build is counted as skipped
- [Redundancy test 384](#redundancy-test-384): the report counts a multiplier of a bonus that names a generic tech
- [Redundancy test 385](#redundancy-test-385): a bonus requirement is a tech of the civ
- [Redundancy test 386](#redundancy-test-386): two techs given one requirement share its copy
- [Redundancy test 387](#redundancy-test-387): a bonus of requirements is counted as built
- [Redundancy test 388](#redundancy-test-388): a bonus whose tech has no room is skipped
- [Redundancy test 389](#redundancy-test-389): a second build keeps no requirement of the first
- [Redundancy test 390](#redundancy-test-390): a tech that no line names keeps its requirements
- [Redundancy test 391](#redundancy-test-391): the team bonus takes the changes of its effect
- [Redundancy test 392](#redundancy-test-392): the team bonus names the game's own techs
- [Redundancy test 393](#redundancy-test-393): a team bonus taken twice doubles the changes of its effect
- [Redundancy test 394](#redundancy-test-394): the elite unit's tech takes the string id of its slot
- [Redundancy test 395](#redundancy-test-395): a unique tech makes no change twice for the civ's unit
- [Redundancy test 396](#redundancy-test-396): a unique tech that enabled the old unit enables the civ's own
- [Redundancy test 397](#redundancy-test-397): a unique tech no longer upgrades the old unit
- [Redundancy test 398](#redundancy-test-398): a unit that a unique tech spawns becomes the civ's elite unit
- [Redundancy test 399](#redundancy-test-399): the tech tree unlocks before it disables
- [Redundancy test 400](#redundancy-test-400): a tech of resources that is researched nowhere stays
- [Redundancy test 401](#redundancy-test-401): the tree's changes are sorted, unlocks first, then by tech
- [Redundancy test 402](#redundancy-test-402): two disabled techs are two changes of the tree
- [Redundancy test 403](#redundancy-test-403): the report says that the unique unit was skipped
- [Redundancy test 404](#redundancy-test-404): the report counts the unique techs the catalog lacks
- [Redundancy test 405](#redundancy-test-405): a hero is hidden in the tech tree
- [Redundancy test 406](#redundancy-test-406): an empty tree disables no tech
- [Redundancy test 407](#redundancy-test-407): a team bonus without an effect is built from its changes
- [Redundancy test 408](#redundancy-test-408): a team bonus of changes taken twice doubles them
- [Redundancy test 409](#redundancy-test-409): a bonus of techs builds its lines too
- [Redundancy test 410](#redundancy-test-410): the lines of a bonus of techs count as often as the bonus
- [Redundancy test 411](#redundancy-test-411): a team bonus with nothing to build is counted as skipped
- [Redundancy test 412](#redundancy-test-412): a skipped team bonus is not counted as built
- [Redundancy test 413](#redundancy-test-413): a built team bonus is not counted as skipped
- [Redundancy test 414](#redundancy-test-414): a named hero takes the string id of its slot
- [Redundancy test 415](#redundancy-test-415): a copied tech requires the copy of a civ's tech it required
- [Redundancy test 416](#redundancy-test-416): a copied tech still requires a tech of no civ
- [Redundancy test 417](#redundancy-test-417): a chosen scout is written as the civ's scout
- [Redundancy test 418](#redundancy-test-418): a hero that the replaced civ lacks is skipped and reported
- [Redundancy test 419](#redundancy-test-419): a dat file with sounds graphics tasks and a tech tree is written back
- [Redundancy test 420](#redundancy-test-420): the tech that makes the unique unit belongs to the civ
- [Redundancy test 421](#redundancy-test-421): a unique tech asks for the age of its tier
- [Redundancy test 422](#redundancy-test-422): the report counts the techs that the tree disables
- [Redundancy test 423](#redundancy-test-423): a unique unit not built counts no techs
- [Redundancy test 424](#redundancy-test-424): a warship takes the look of its architecture's civ
- [Redundancy test 425](#redundancy-test-425): a trade cart's load takes the look of its architecture's civ
- [Redundancy test 426](#redundancy-test-426): a monk's healing takes the look of its monk skin
- [Redundancy test 427](#redundancy-test-427): a castle takes the look of the civ chosen for the castle
- [Redundancy test 428](#redundancy-test-428): a soldier keeps the look of the replaced civ
- [Redundancy test 429](#redundancy-test-429): the look of an architecture is the civ that the catalog names
- [Redundancy test 430](#redundancy-test-430): a look line of a chosen bonus gives its unit the look of its civ
- [Redundancy test 431](#redundancy-test-431): a look line of a bonus not chosen leaves its unit its look
- [Redundancy test 432](#redundancy-test-432): a look line gives its unit the attack timing of its civ
- [Redundancy test 433](#redundancy-test-433): a look line gives its unit the attack graphic of its civ
- [Redundancy test 434](#redundancy-test-434): a look line gives its unit the walking graphic of its civ
- [Redundancy test 435](#redundancy-test-435): a look line gives its unit the icon of its civ
- [Redundancy test 436](#redundancy-test-436): an added amount taken three times is tripled
- [Redundancy test 437](#redundancy-test-437): a multiplied attribute taken twice is squared
- [Redundancy test 438](#redundancy-test-438): a multiplied resource taken twice is squared
- [Redundancy test 439](#redundancy-test-439): a tech cost multiplied in mode two taken twice is squared
- [Redundancy test 440](#redundancy-test-440): a tech time multiplied in mode two taken twice is squared
- [Redundancy test 441](#redundancy-test-441): a tech cost added in mode one is kept
- [Redundancy test 442](#redundancy-test-442): a spawn taken twice makes twice the units
- [Redundancy test 443](#redundancy-test-443): a resource taken twice is doubled
- [Redundancy test 444](#redundancy-test-444): the spawn limit is kept
- [Redundancy test 445](#redundancy-test-445): the effect function is kept
- [Redundancy test 446](#redundancy-test-446): a set attribute is kept
- [Redundancy test 447](#redundancy-test-447): an armor added past 255 is split in two
- [Redundancy test 448](#redundancy-test-448): an armor taken away keeps its sign
- [Redundancy test 449](#redundancy-test-449): an attack multiplied past 255 percent is split in two
- [Redundancy test 450](#redundancy-test-450): a multiplied attack halfway is rounded to even
- [Redundancy test 451](#redundancy-test-451): a multiplied attack past a half is rounded up
- [Redundancy test 452](#redundancy-test-452): a multiplied attack below a half is rounded down
- [Redundancy test 453](#redundancy-test-453): an armor added to 255 is one change
- [Redundancy test 454](#redundancy-test-454): a cost cut by half that counts twice is cut by three quarters
- [Redundancy test 455](#redundancy-test-455): an amount added that counts twice is added twice
- [Redundancy test 456](#redundancy-test-456): each percent of a list that counts twice is scaled
- [Redundancy test 457](#redundancy-test-457): a twofold amount that counts twice becomes fourfold
- [Redundancy test 458](#redundancy-test-458): a percent that counts twice shows one decimal
- [Redundancy test 459](#redundancy-test-459): a percent without a sign before faster that counts twice is raised
- [Redundancy test 460](#redundancy-test-460): a built mod has a data folder named after the civ
- [Redundancy test 461](#redundancy-test-461): a built mod has a ui folder named after the civ
- [Redundancy test 462](#redundancy-test-462): the data mod's info names the civ
- [Redundancy test 463](#redundancy-test-463): the ui mod's info says that it is the ui mod
- [Redundancy test 464](#redundancy-test-464): the data mod holds the dat file with the civ's techs
- [Redundancy test 465](#redundancy-test-465): the data mod's list of civs names the civ's unique unit
- [Redundancy test 466](#redundancy-test-466): the data mod holds the tech tree file of the replaced civ
- [Redundancy test 467](#redundancy-test-467): the ui mod names the civ in english
- [Redundancy test 468](#redundancy-test-468): the ui mod is written for the last language of the game
- [Redundancy test 469](#redundancy-test-469): a game folder without its list of civs is reported
- [Redundancy test 470](#redundancy-test-470): a data file that is not the game's is reported
- [Redundancy test 471](#redundancy-test-471): a game without the civ's tech tree file is reported
- [Redundancy test 472](#redundancy-test-472): a civ with parts the catalog lacks is built and reported
- [Redundancy test 473](#redundancy-test-473): a line break in a text is written as an escape
- [Redundancy test 474](#redundancy-test-474): a carriage return before a line break is left out
- [Redundancy test 475](#redundancy-test-475): a quote in a text is escaped
- [Redundancy test 476](#redundancy-test-476): a quote that is escaped already is left as it is
- [Redundancy test 477](#redundancy-test-477): a unique tech's label ends its description without a full stop
- [Redundancy test 478](#redundancy-test-478): a unique tech without a description has a plain label
- [Redundancy test 479](#redundancy-test-479): a unit's tooltip carries its description
- [Redundancy test 480](#redundancy-test-480): the first tip names the first civ bonus
- [Redundancy test 481](#redundancy-test-481): the first tip names the team bonus of a civ without civ bonuses
- [Redundancy test 482](#redundancy-test-482): the first tip shows a civ bonus as it counts
- [Redundancy test 483](#redundancy-test-483): the tip of a unique tech says where it is researched
- [Redundancy test 484](#redundancy-test-484): the second tip names the unique unit
- [Redundancy test 485](#redundancy-test-485): a line break in a tip is written as a space
- [Redundancy test 486](#redundancy-test-486): the game is found in the folder of steam
- [Redundancy test 487](#redundancy-test-487): the game is found in a library that steam lists
- [Redundancy test 488](#redundancy-test-488): the game is not found where no library holds it
- [Redundancy test 489](#redundancy-test-489): a home without a profile of the game has no mods folder
- [Redundancy test 490](#redundancy-test-490): a number with a minus sign is negative
- [Redundancy test 491](#redundancy-test-491): a number with a letter in it is no number
- [Redundancy test 492](#redundancy-test-492): a number past the largest long is no number
- [Redundancy test 493](#redundancy-test-493): the largest number a long holds is read
- [Redundancy test 494](#redundancy-test-494): a letter of two bytes is widened to one wide letter
- [Redundancy test 495](#redundancy-test-495): a letter of three bytes is widened to one wide letter
- [Redundancy test 496](#redundancy-test-496): a letter beyond the basic plane is widened to two
- [Redundancy test 497](#redundancy-test-497): the first letter beyond the basic plane is widened to two
- [Redundancy test 498](#redundancy-test-498): a byte that starts no letter is not widened
- [Redundancy test 499](#redundancy-test-499): a text that starts inside a letter is not widened
- [Redundancy test 500](#redundancy-test-500): a letter cut short is not widened
- [Redundancy test 501](#redundancy-test-501): a text longer than its place is not widened
- [Redundancy test 502](#redundancy-test-502): a field that is a dash reads as empty
- [Redundancy test 503](#redundancy-test-503): a field the line lacks reads as no text
- [Redundancy test 504](#redundancy-test-504): hundredths keep two digits after the point
- [Redundancy test 505](#redundancy-test-505): hundredths of one digit after the point end in zero
- [Redundancy test 506](#redundancy-test-506): a second point makes no hundredths
- [Redundancy test 507](#redundancy-test-507): a decimal with a minus sign is negative
- [Redundancy test 508](#redundancy-test-508): a decimal with a letter in it is no decimal
- [Redundancy test 509](#redundancy-test-509): a minus sign alone is no decimal
- [Redundancy test 510](#redundancy-test-510): an escaped letter of four hex digits is read
- [Redundancy test 511](#redundancy-test-511): an escape with capital hex digits is read
- [Redundancy test 512](#redundancy-test-512): an escaped pair of surrogates is one letter
- [Redundancy test 513](#redundancy-test-513): an escape that is not hex is unreadable
- [Redundancy test 514](#redundancy-test-514): a high surrogate alone is unreadable
- [Redundancy test 515](#redundancy-test-515): a high surrogate before a plain letter is unreadable
- [Redundancy test 516](#redundancy-test-516): an escaped new line is read
- [Redundancy test 517](#redundancy-test-517): an escaped tab is read
- [Redundancy test 518](#redundancy-test-518): an escaped carriage return is read
- [Redundancy test 519](#redundancy-test-519): an escaped quote is read
- [Redundancy test 520](#redundancy-test-520): a string longer than its place is not copied
- [Redundancy test 521](#redundancy-test-521): a text that is a list is no civ file
- [Redundancy test 522](#redundancy-test-522): an object left open is unreadable
- [Redundancy test 523](#redundancy-test-523): edits made out of order are written in the file's order
- [Redundancy test 524](#redundancy-test-524): an escape with the hex digits a and f is read
- [Redundancy test 525](#redundancy-test-525): an escape with the capital hex digit f is read
- [Redundancy test 526](#redundancy-test-526): the first letter of two bytes is read
- [Redundancy test 527](#redundancy-test-527): the first letter of three bytes is read
- [Redundancy test 528](#redundancy-test-528): the first letter beyond the basic plane is read
- [Redundancy test 529](#redundancy-test-529): the last letter of unicode is read
- [Redundancy test 530](#redundancy-test-530): a stream of dynamic codes with repeated lengths inflates
- [Redundancy test 531](#redundancy-test-531): a text is not equal to a longer one that starts with it
- [Redundancy test 532](#redundancy-test-532): a text is not equal to its own beginning
- [Redundancy test 533](#redundancy-test-533): a text that fills its place is copied
- [Redundancy test 534](#redundancy-test-534): a text one letter too long for its place is not copied
- [Redundancy test 535](#redundancy-test-535): a text that does not fit is not appended
- [Redundancy test 536](#redundancy-test-536): a negative number is appended with its minus sign
- [Redundancy test 537](#redundancy-test-537): a capital z is found by a small z
- [Redundancy test 538](#redundancy-test-538): a text does not contain a letter it lacks
- [Redundancy test 539](#redundancy-test-539): two members are written with a comma between
- [Redundancy test 540](#redundancy-test-540): the first member of an object inside another has no comma
- [Redundancy test 541](#redundancy-test-541): an object left open is not a finished text
- [Redundancy test 542](#redundancy-test-542): a text too long for the writer is reported
- [Redundancy test 543](#redundancy-test-543): a line of a file is split at its tabs
- [Redundancy test 544](#redundancy-test-544): a number field of a file is read
- [Redundancy test 545](#redundancy-test-545): a text field of a file is read
- [Redundancy test 546](#redundancy-test-546): a list of numbers is read in its order
- [Redundancy test 547](#redundancy-test-547): a list of slots is read as bits
- [Redundancy test 548](#redundancy-test-548): the second line of a file follows the first
- [Redundancy test 549](#redundancy-test-549): a file ends after its last line
- [Redundancy test 550](#redundancy-test-550): a file that is missing is not opened
- [Redundancy test 551](#redundancy-test-551): the catalog holds every civ bonus of its file
- [Redundancy test 552](#redundancy-test-552): the catalog holds every team bonus of its file
- [Redundancy test 553](#redundancy-test-553): the catalog holds every unit of its file
- [Redundancy test 554](#redundancy-test-554): the catalog holds every tech of its file
- [Redundancy test 555](#redundancy-test-555): the catalog holds every node of its file
- [Redundancy test 556](#redundancy-test-556): the catalog holds every requirement line of its file
- [Redundancy test 557](#redundancy-test-557): a requirement line names its bonus, its tech and the tech required
- [Redundancy test 558](#redundancy-test-558): a train line is a unit patch placed in the train location
- [Redundancy test 559](#redundancy-test-559): a cost line is a unit patch placed in the costs
- [Redundancy test 560](#redundancy-test-560): a look line names its bonus, its unit and the civ of the look
- [Redundancy test 561](#redundancy-test-561): the first unit of the catalog has its name
- [Redundancy test 562](#redundancy-test-562): a units speed is kept in hundredths
- [Redundancy test 563](#redundancy-test-563): the first unit of the catalog is a unit of the game
- [Redundancy test 564](#redundancy-test-564): a catalog folder that is missing is not loaded
- [Redundancy test 565](#redundancy-test-565): a command to pick a unit picks it
- [Redundancy test 566](#redundancy-test-566): a command to pick a tech picks it
- [Redundancy test 567](#redundancy-test-567): a command to pick a hero picks it
- [Redundancy test 568](#redundancy-test-568): a command to tick a civ bonus ticks it
- [Redundancy test 569](#redundancy-test-569): a command to tick a team bonus ticks it
- [Redundancy test 570](#redundancy-test-570): a command to tick a node ticks it
- [Redundancy test 571](#redundancy-test-571): a command to set a choice sets it
- [Redundancy test 572](#redundancy-test-572): a command of no kind changes nothing
- [Redundancy test 573](#redundancy-test-573): a civ bonus ticked twice is no longer picked
- [Redundancy test 574](#redundancy-test-574): a team bonus ticked twice is no longer picked
- [Redundancy test 575](#redundancy-test-575): a row of the tech tree gives the command to tick its node
- [Redundancy test 576](#redundancy-test-576): a row of the civ bonuses gives the command to tick its bonus
- [Redundancy test 577](#redundancy-test-577): a row of the team bonuses gives the command to tick its bonus
- [Redundancy test 578](#redundancy-test-578): a row of a look gives the command to set that choice
- [Redundancy test 579](#redundancy-test-579): unticking the archer leaves the skirmisher
- [Redundancy test 580](#redundancy-test-580): unticking iron casting unticks blast furnace
- [Redundancy test 581](#redundancy-test-581): unticking the outpost unticks the watch tower
- [Redundancy test 582](#redundancy-test-582): unticking the archery range unticks the archer
- [Redundancy test 583](#redundancy-test-583): unticking the mill which every civ keeps leaves the farm
- [Redundancy test 584](#redundancy-test-584): the summary shows a bonus as it counts
- [Redundancy test 585](#redundancy-test-585): a row of the units gives the command to pick its unit
- [Redundancy test 586](#redundancy-test-586): a row of the heroes gives the command to pick its hero
- [Redundancy test 587](#redundancy-test-587): unticking Hero gives the command to pick no hero
- [Redundancy test 588](#redundancy-test-588): a row of the castle techs gives the command to pick its tech
- [Redundancy test 589](#redundancy-test-589): a row of the imperial techs gives the command to pick its tech
- [Redundancy test 590](#redundancy-test-590): a row of the castle techs shows its tech's name
- [Redundancy test 591](#redundancy-test-591): the summary names the unique unit and the unit it is made from
- [Redundancy test 592](#redundancy-test-592): the summary lists a civ bonus by its description
- [Redundancy test 593](#redundancy-test-593): the summary names a civ bonus the catalog lacks by its number
- [Redundancy test 594](#redundancy-test-594): the summary names a unique tech by its catalog name
- [Redundancy test 595](#redundancy-test-595): the summary says when the catalog lacks the unique unit
- [Redundancy test 596](#redundancy-test-596): the summary says when the catalog lacks a unique tech
- [Redundancy test 597](#redundancy-test-597): the build report counts the civ bonuses built and skipped
- [Redundancy test 598](#redundancy-test-598): the build report says when the hero was not built
- [Redundancy test 599](#redundancy-test-599): the build report says when a bonus counts only once
- [Redundancy test 600](#redundancy-test-600): the build report of a whole civ names the two folders
- [Redundancy test 601](#redundancy-test-601): no step of a new design is done
- [Redundancy test 602](#redundancy-test-602): the name and tech tree step is done once the civ has a name
- [Redundancy test 603](#redundancy-test-603): the name and tech tree step is done once a node is chosen
- [Redundancy test 604](#redundancy-test-604): the bonuses step is done once a bonus is ticked
- [Redundancy test 605](#redundancy-test-605): the bonuses step is done once a team bonus is picked
- [Redundancy test 606](#redundancy-test-606): the UU&UT step is done once a unit is picked
- [Redundancy test 607](#redundancy-test-607): the UU&UT step is done once a castle tech is picked
- [Redundancy test 608](#redundancy-test-608): the UU&UT step is done once an imperial tech is picked
- [Redundancy test 609](#redundancy-test-609): the UU&UT step is done once a hero is picked
- [Redundancy test 610](#redundancy-test-610): a pick finishes only its own step
- [Redundancy test 611](#redundancy-test-611): back from the second step goes to the first
- [Redundancy test 612](#redundancy-test-612): a new design has nothing to undo
- [Redundancy test 613](#redundancy-test-613): a new design has nothing to redo
- [Redundancy test 614](#redundancy-test-614): an undo that fails leaves nothing to redo
- [Redundancy test 615](#redundancy-test-615): a change that is made is reported as made
- [Redundancy test 616](#redundancy-test-616): a change after an undo leaves nothing to redo
- [Redundancy test 617](#redundancy-test-617): undo goes back as many changes as the history holds
- [Redundancy test 618](#redundancy-test-618): undo past the depth brings back the change before
- [Redundancy test 619](#redundancy-test-619): a design put in place of another can be undone
- [Redundancy test 620](#redundancy-test-620): a design put in place of another leaves nothing to redo
- [Redundancy test 621](#redundancy-test-621): a change after typing is undone by itself
- [Redundancy test 622](#redundancy-test-622): typing after an undo is a change of its own
- [Redundancy test 623](#redundancy-test-623): a ticked bonus counts once
- [Redundancy test 624](#redundancy-test-624): a picked team bonus counts once
- [Redundancy test 625](#redundancy-test-625): a multiplier past ten is refused
- [Redundancy test 626](#redundancy-test-626): a multiplier below one is refused
- [Redundancy test 627](#redundancy-test-627): a bonus ticked twice is not chosen
- [Redundancy test 628](#redundancy-test-628): a node chosen twice is not chosen
- [Redundancy test 629](#redundancy-test-629): taking a node out keeps the other nodes
- [Redundancy test 630](#redundancy-test-630): a tree takes no more nodes than it holds
- [Redundancy test 631](#redundancy-test-631): a unit of the game is saved with no custom units index
- [Redundancy test 632](#redundancy-test-632): a custom unit is saved with no id of the game
- [Redundancy test 633](#redundancy-test-633): the language of the britons is saved as index 0
- [Redundancy test 634](#redundancy-test-634): a build that left nothing out is whole
- [Redundancy test 635](#redundancy-test-635): a build that skipped a civ bonus is not whole
- [Redundancy test 636](#redundancy-test-636): the last step is titled Finish
- [Redundancy test 637](#redundancy-test-637): a named civ is saved under its name
- [Redundancy test 638](#redundancy-test-638): a save that is cancelled says that the civ was not saved
- [Redundancy test 639](#redundancy-test-639): a saved civ is opened again
- [Redundancy test 640](#redundancy-test-640): a file that is no civ file is refused with a message
- [Redundancy test 641](#redundancy-test-641): the next button goes to the next step
- [Redundancy test 642](#redundancy-test-642): a typed name is shown in the title of the window
- [Redundancy test 643](#redundancy-test-643): redo says when there is nothing to redo
- [Redundancy test 644](#redundancy-test-644): closing is stopped when the question of saving is cancelled
- [Redundancy test 645](#redundancy-test-645): closing without saving closes the window
- [Redundancy test 646](#redundancy-test-646): build is refused when the game is not found
- [Redundancy test 647](#redundancy-test-647): build writes nothing when the mod is not named
- [Redundancy test 648](#redundancy-test-648): build of an unfinished civ says that it is not finished
- [Redundancy test 649](#redundancy-test-649): a build that left something out says what in a message
- [Redundancy test 650](#redundancy-test-650): the help line is written in the muted colour
- [Redundancy test 651](#redundancy-test-651): a field is written on the colour of fields
- [Redundancy test 652](#redundancy-test-652): a row in focus is drawn with a frame in the focus colour
- [Redundancy test 653](#redundancy-test-653): picking the picked row again makes no new change
- [Redundancy test 654](#redundancy-test-654): the knob of the window's scroll bar reaches its end with the step
- [Redundancy test 655](#redundancy-test-655): a step that just fits the window shows no scroll bar
- [Redundancy test 656](#redundancy-test-656): the up arrow of the window's scroll bar moves the step up a line
- [Redundancy test 657](#redundancy-test-657): a key in a list moves its caret one row
- [Redundancy test 658](#redundancy-test-658): the room under the last row of a list is painted in the colour of fields
- [Redundancy test 659](#redundancy-test-659): a list full of rows paints nothing under itself
- [Redundancy test 660](#redundancy-test-660): ticking Hero names Attila the Hun above Choose hero
- [Redundancy test 661](#redundancy-test-661): unticking Hero hides Choose hero
- [Redundancy test 662](#redundancy-test-662): a hero picked in Choose hero keeps the game's name
- [Redundancy test 663](#redundancy-test-663): a hero renamed in Choose hero takes the new name
- [Redundancy test 664](#redundancy-test-664): a build is written without a question of its folder
- [Redundancy test 665](#redundancy-test-665): Options takes a mods folder that exists
- [Redundancy test 666](#redundancy-test-666): Options refuses a mods folder that does not exist
- [Redundancy test 667](#redundancy-test-667): closing asks nothing of a changed civ when the option says not to
- [Redundancy test 668](#redundancy-test-668): Options keeps the choice not to ask to save before exiting
- [Redundancy test 669](#redundancy-test-669): Options with the mods folder left as it is says that the options were kept
- [Redundancy test 670](#redundancy-test-670): a changed civ is asked to be saved when no option is kept
- [Redundancy test 671](#redundancy-test-671): a click ticks the tick box of a question
- [Redundancy test 672](#redundancy-test-672): a click unticks a tick box given as ticked
- [Redundancy test 673](#redundancy-test-673): the window shows the numbers it read from the game's data file
- [Redundancy test 674](#redundancy-test-674): a civ not yet changed takes its tech tree from the game's files once they are read
- [Redundancy test 675](#redundancy-test-675): a changed civ keeps its tech tree when the game's files are read
- [Redundancy test 676](#redundancy-test-676): Options cancelled leaves the choice of asking to save as it was
- [Redundancy test 677](#redundancy-test-677): the tick box of a question lies above its buttons
- [Redundancy test 678](#redundancy-test-678): the rows of a list shown are measured anew when it becomes wider
- [Redundancy test 679](#redundancy-test-679): a letter typed into the name leaves the tech tree nothing to redraw
- [Redundancy test 680](#redundancy-test-680): the longest row of the narrow list of team bonuses is higher than the shortest
- [Redundancy test 681](#redundancy-test-681): a question with a tick box takes an empty text
- [Redundancy test 682](#redundancy-test-682): Appearance opens the popup of the look choices
- [Redundancy test 683](#redundancy-test-683): the popup of the look choices is hidden until asked for
- [Redundancy test 684](#redundancy-test-684): the architecture is chosen in the popup of the look choices
- [Redundancy test 685](#redundancy-test-685): a civ outside ranked play is not offered to edit
- [Redundancy test 686](#redundancy-test-686): a changed civ is starred in Civ to edit
- [Redundancy test 687](#redundancy-test-687): a civ changed back loses its star
- [Redundancy test 688](#redundancy-test-688): Finish counts a changed civ without a save
- [Redundancy test 689](#redundancy-test-689): the filter of the team bonuses leaves the rows that hold its text
- [Redundancy test 690](#redundancy-test-690): a civ's name in the filter of the units gives its unique unit
- [Redundancy test 691](#redundancy-test-691): a civ's name in the filter of the castle techs gives its castle tech
- [Redundancy test 692](#redundancy-test-692): a civ's name in the filter of the imperial techs gives its imperial tech
- [Redundancy test 693](#redundancy-test-693): the filter of Choose hero leaves the heroes that hold its text
- [Redundancy test 694](#redundancy-test-694): Clear leaves the civ without a civ bonus
- [Redundancy test 695](#redundancy-test-695): a civ's name in the filter of the bonuses gives its civ bonuses
- [Redundancy test 696](#redundancy-test-696): a civ's name in the filter of the tech tree gives its nodes
- [Redundancy test 697](#redundancy-test-697): the sources of a folder include those of its folders
- [Redundancy test 698](#redundancy-test-698): a folder lists only its own sources
- [Redundancy test 699](#redundancy-test-699): the sources of the project are those of src, tests and tools
- [Redundancy test 700](#redundancy-test-700): the newest source is the one changed last
- [Redundancy test 701](#redundancy-test-701): the object of a source is named after it in the folder given
- [Redundancy test 702](#redundancy-test-702): a source older than its object is not built again
- [Redundancy test 703](#redundancy-test-703): a source with a newer header is built again
- [Redundancy test 704](#redundancy-test-704): a source changed when its object was made is built again
- [Redundancy test 705](#redundancy-test-705): the objects of the build tool are listed without libraries
- [Redundancy test 706](#redundancy-test-706): a coverage build is asked for by coverage
- [Redundancy test 707](#redundancy-test-707): the tests are run with the arguments given to the build
- [Redundancy test 708](#redundancy-test-708): a coverage build links the tests alone
- [Redundancy test 709](#redundancy-test-709): a coverage build links the tests with coverage
- [Redundancy test 710](#redundancy-test-710): a coverage build makes the folder of its report
- [Redundancy test 711](#redundancy-test-711): no recursion, direct or indirect
- [Redundancy test 712](#redundancy-test-712): no dynamic memory allocation
- [Redundancy test 713](#redundancy-test-713): preprocessor only includes and guards
- [Redundancy test 714](#redundancy-test-714): one level of pointer dereferencing
- [Redundancy test 715](#redundancy-test-715): no function pointers but the window procedure
- [Redundancy test 716](#redundancy-test-716): the rule against goto names the line of a goto
- [Redundancy test 717](#redundancy-test-717): the rule against allocation names the line of a malloc
- [Redundancy test 718](#redundancy-test-718): the rule of the preprocessor names a macro with arguments
- [Redundancy test 719](#redundancy-test-719): the rule of one dereference names two arrows in a row
- [Redundancy test 720](#redundancy-test-720): the rule against function pointers names one
- [Redundancy test 721](#redundancy-test-721): the rule against comments names the line of a comment
- [Redundancy test 722](#redundancy-test-722): the rule against recursion names a function that calls itself

## Test 1

### File residence:
`tests/requirements/requirement_tests.c`, lines 25-30

### Purpose:
Verify that the local mods folder of the user's profile of the game is found (Req. 2).

### Assertion expectation:
Under the home `build/test_home` with profile 7656, the folder found ends in `7656\mods\local`.

## Test 2

### File residence:
`tests/requirements/requirement_tests.c`, lines 32-39

### Purpose:
Verify that undo takes back the last change, so a slip costs nothing (Req. 5).

### Assertion expectation:
After bonus 7 is chosen and undone, undo reports success and the civ has no bonus.

## Test 3

### File residence:
`tests/requirements/requirement_tests.c`, lines 41-51

### Purpose:
Prevent a bonus past the fixed 24 places from being written beyond them (Req. 6).

### Assertion expectation:
After 24 bonuses are chosen, a 25th is refused and the civ still has 24.

## Test 4

### File residence:
`tests/requirements/requirement_tests.c`, lines 53-61

### Purpose:
Verify that the mod's folders are named from the mod's name as the original names them (Req. 7).

### Assertion expectation:
"Æsir Kings 2!" gives the folder name "_sir_kings_2_": one "_" for each character that is not a-z, 0-9, _ or -.

## Test 5

### File residence:
`tests/requirements/requirement_tests.c`, lines 63-66

### Purpose:
Verify that the techs of the replaced civ no longer belong to it in the built dat file (Req. 8).

### Assertion expectation:
The old unique unit's tech of civ 1 reads back with civ 99, the civ no player has.

## Test 6

### File residence:
`tests/requirements/requirement_tests.c`, lines 68-71

### Purpose:
Verify that the build says which civ bonus it could not build (Req. 9).

### Assertion expectation:
`bonuses_skipped` is 1 for a design with bonus 99, which the catalog lacks.

## Test 7

### File residence:
`tests/requirements/requirement_tests.c`, lines 73-76

### Purpose:
Verify that a civ the user has not named is saved as `civeditor_civ` by default (Req. 1).

### Assertion expectation:
A new civ, named as the game names it, saved in `build/` writes `build/civeditor_civ.civeditor.json` anew.

## Test 8

### File residence:
`tests/requirements/requirement_tests.c`, lines 78-90

### Purpose:
Prevent a wheel turn outside the lists from redrawing a step that fits the window (Bug 1 of 2026-11-19).

### Assertion expectation:
At the window's first size, a notch of the wheel sent to the window itself leaves it nothing to redraw.

## Test 9

### File residence:
`tests/requirements/requirement_tests.c`, lines 92-97

### Purpose:
Prevent again a dropdown sliding open, where the lists move at once (Bug 1 of 2026-11-21).

### Assertion expectation:
With the window on the screen, `CB_SHOWDROPDOWN` on Civ to edit takes less than 150 ms.

## Test 10

### File residence:
`tests/requirements/requirement_tests.c`, lines 99-112

### Purpose:
Prevent again a team bonus whose numbers the window gives no fields (Bug 1 of 2026-11-24).

### Assertion expectation:
With the Britons' team bonus, 20 typed into its field makes its row read "Archery Ranges work 20% faster".

## Test 11

### File residence:
`tests/requirements/requirement_tests.c`, lines 114-119

### Purpose:
Prevent again selected rows that blink white while a list is scrolled (Bug 2 of 2026-11-24).

### Assertion expectation:
The tech tree list, asked to erase itself into a black picture, leaves its lowest row of pixels black.

## Test 12

### File residence:
`tests/accessibility/access_tests.c`, lines 14-27

### Purpose:
Verify that all text can be read by those who see less contrast (Req. 3, WCAG 2.2 SC 1.4.3).

### Assertion expectation:
Each of the 7 pairs of text and background colors in `palette.c` contrasts at least 4.50:1 by the WCAG formula.

## Test 13

### File residence:
`tests/accessibility/access_tests.c`, lines 39-49

### Purpose:
Verify that every input says what it is for, seen and to a screen reader (Req. 3, WCAG 2.2 SC 2.5.3, 3.3.2).

### Assertion expectation:
Every text field, list, checklist, choice and report in `layout.c`, and Chosen bonuses, has a label of its own.

## Test 14

### File residence:
`tests/accessibility/access_tests.c`, lines 51-65

### Purpose:
Verify that everything one clicks is large enough to hit (Req. 3, WCAG 2.2 SC 2.5.8).

### Assertion expectation:
In the smallest window, 960 by 640, every control one can act on is at least 44 by 44, and so is a list row.

## Test 15

### File residence:
`tests/accessibility/access_tests.c`, lines 92-114

### Purpose:
Verify that no control or label hides another, so focus is never covered (Req. 3, WCAG 2.2 SC 2.4.11).

### Assertion expectation:
On each of the 9 steps at 960 by 640, no two boxes of controls and labels overlap, and all lie inside the window.

## Test 16

### File residence:
`tests/accessibility/access_tests.c`, lines 116-125

### Purpose:
Verify that the window's title says which civ is open and on which step (WCAG 2.2 SC 2.4.2; Req. 3).

### Assertion expectation:
For a civ named Testonia on the Bonuses step, the title is "Testonia - Step 2 of 4: Bonuses - civeditor".

## Test 17

### File residence:
`tests/accessibility/accessibility_suite.c`, lines 30-32; `tests/accessibility/access_tests.c`, lines 127-141

### Purpose:
Enforce that the ways from step to step keep their places on every step (Requirement 3, WCAG 2.2 SC 3.2.3).

### Assertion expectation:
On each step the list of steps is shown, Back wherever a step lies before, Next wherever one lies after.

## Test 18

### File residence:
`tests/reflow/reflow_tests.c`, lines 103-106

### Purpose:
Verify that a window too narrow for the columns gets one column (Req. 3, WCAG 1.4.10).

### Assertion expectation:
`reflow_is_needed` is true for a window of 959 by 640.

## Test 19

### File residence:
`tests/reflow/reflow_tests.c`, lines 108-111

### Purpose:
Verify that a window too short for the columns gets one column and scrolls (Req. 3, WCAG 1.4.4).

### Assertion expectation:
`reflow_is_needed` is true for a window of 960 by 559.

## Test 20

### File residence:
`tests/reflow/reflow_tests.c`, lines 113-116

### Purpose:
Prevent the one column from taking the place of the columns in a window that holds them (Req. 1).

### Assertion expectation:
`reflow_is_needed` is false for a window of 960 by 640.

## Test 21

### File residence:
`tests/reflow/reflow_tests.c`, lines 118-121

### Purpose:
Verify that no step needs scrolling sideways at a width of 320 (Req. 3, WCAG 1.4.10).

### Assertion expectation:
In every step, every control and label starts at 0 or more and ends at 320 or less, across.

## Test 22

### File residence:
`tests/reflow/reflow_tests.c`, lines 123-132

### Purpose:
Prevent a control from hiding another in the one column (Req. 3, WCAG 1.4.10).

### Assertion expectation:
In every step, no two boxes of controls and labels overlap at a width of 320.

## Test 23

### File residence:
`tests/reflow/reflow_tests.c`, lines 134-137

### Purpose:
Verify that every control one acts on stays large enough in the one column (Req. 3, WCAG 2.5.8).

### Assertion expectation:
In every step, every control one acts on is at least 44 wide and 44 high at a width of 320.

## Test 24

### File residence:
`tests/reflow/reflow_tests.c`, lines 139-144

### Purpose:
Verify that the window scrolls to the end of a step and no further (Req. 3, WCAG 1.4.10).

### Assertion expectation:
`reflow_height` of the bonuses step is the lower edge of Next plus a margin of 16.

## Test 25

### File residence:
`tests/reflow/reflow_tests.c`, lines 146-149

### Purpose:
Verify that the control in focus is never left below the window (Req. 3, WCAG 2.4.11).

### Assertion expectation:
A box at 600, 44 high, in a view of 256 scrolled to 0, gives a scroll of 600 + 44 + 16 - 256.

## Test 26

### File residence:
`tests/reflow/reflow_tests.c`, lines 151-154

### Purpose:
Verify that the control in focus is never left above the window, nor its label (Req. 3, WCAG 2.4.11).

### Assertion expectation:
A box at 100 in a view scrolled to 300 gives a scroll of 100 - 26 - 16: its label and a margin show.

## Test 27

### File residence:
`tests/reflow/reflow_tests.c`, lines 156-159

### Purpose:
Prevent the window from jumping when the focus moves to a control in view (Req. 3, WCAG 3.2.1).

### Assertion expectation:
A box at 400, 44 high, in a view of 256 scrolled to 300, gives a scroll of 300.

## Test 28

### File residence:
`tests/reflow/reflow_tests.c`, lines 161-164

### Purpose:
Prevent scrolling past the end of a step (Req. 3).

### Assertion expectation:
`reflow_scroll_within` of 900, for a content of 700 in a view of 256, gives 444.

## Test 29

### File residence:
`tests/reflow/reflow_tests.c`, lines 166-169

### Purpose:
Prevent scrolling above the start of a step (Req. 3).

### Assertion expectation:
`reflow_scroll_within` of -5 gives 0.

## Test 30

### File residence:
`tests/reflow/reflow_tests.c`, lines 171-174

### Purpose:
Prevent scrolling a step that fits the window (Req. 3).

### Assertion expectation:
`reflow_scroll_within` of 50, for a content of 200 in a view of 256, gives 0.

## Test 31

### File residence:
`tests/reflow/reflow_tests.c`, lines 307-310

### Purpose:
Verify that the one column has no gap between the help and a step's controls (Req. 3, WCAG 1.4.10).

### Assertion expectation:
At a width of 320, the Civ to edit list of the first step is at 370 from the top.

## Test 32

### File residence:
`tests/reflow/reflow_tests.c`, lines 312-317

### Purpose:
Verify that Back and Next end the one column, as they end the Tab order (Req. 3, WCAG 2.4.3).

### Assertion expectation:
At a width of 320, Back is 8 below the lower edge of the status line.

## Test 33

### File residence:
`tests/reflow/reflow_tests.c`, lines 319-322

### Purpose:
Verify that a control of the one column keeps its margin on the left (Req. 3, WCAG 1.4.10).

### Assertion expectation:
At a width of 320, the Name field starts at 16.

## Test 34

### File residence:
`tests/reflow/reflow_tests.c`, lines 324-329

### Purpose:
Verify that Back and Next together are as wide as the other controls (Req. 3, WCAG 1.4.10).

### Assertion expectation:
At a width of 320, Next ends at 304.

## Test 35

### File residence:
`tests/accessibility/accessibility_suite.c`, lines 44-46; `tests/accessibility/window_tests.c`, lines 35-51

### Purpose:
Enforce that every control can be reached with the keyboard alone (Requirement 3, WCAG 2.2 SC 2.1.1).

### Assertion expectation:
In the real window, made but not shown, the Tab order of each step holds every control of it that one acts on.

## Test 36

### File residence:
`tests/accessibility/accessibility_suite.c`, lines 47-49; `tests/accessibility/window_tests.c`, lines 199-211

### Purpose:
Enforce that the window tells which step the user is on, as it shows it (Requirement 3, WCAG 2.2 SC 1.3.1).

### Assertion expectation:
In the real window, after going to each step by its command, the list of steps has that step as its chosen row.

## Test 37

### File residence:
`tests/accessibility/accessibility_suite.c`, lines 50-52; `tests/accessibility/window_tests.c`, lines 213-230

### Purpose:
Enforce that the keyboard goes through a step as it is read (Requirement 3, WCAG 2.2 SC 2.4.3 and 1.3.2).

### Assertion expectation:
On each step, Tab goes from the list of steps down each column from left to right, then to Back and Next.

## Test 38

### File residence:
`tests/accessibility/accessibility_suite.c`, lines 53-55; `tests/accessibility/window_tests.c`, lines 301-314

### Purpose:
Catch again Bug 1 of the trial: a click on a row scrolled the list of bonuses to its top (Rule 24).

### Assertion expectation:
In the real window, with the list scrolled to row 30, ticking row 34 leaves row 30 at the top of the list.

## Test 39

### File residence:
`tests/accessibility/accessibility_suite.c`, lines 56-58; `tests/accessibility/window_tests.c`, lines 53-63

### Purpose:
Enforce that each control has a name for assistive technology (Requirement 3, WCAG 2.2 SC 4.1.2, 1.3.1, 2.5.3).

### Assertion expectation:
In the real window, the name that Windows' accessibility interface gives each control begins with its label.

## Test 40

### File residence:
`tests/accessibility/accessibility_suite.c`, lines 59-61; `tests/accessibility/window_tests.c`, lines 326-344

### Purpose:
Enforce that the check of a ticked row has a text alternative (Requirement 3, WCAG 2.2 SC 1.1.1).

### Assertion expectation:
In the real window, Windows' accessibility interface gives a ticked row of the bonuses as chosen, another row not.

## Test 41

### File residence:
`tests/accessibility/accessibility_suite.c`, lines 62-64; `tests/accessibility/window_tests.c`, lines 358-379

### Purpose:
Enforce that a control does nothing by getting the focus alone (Requirement 3, WCAG 2.2 SC 3.2.1).

### Assertion expectation:
After the focus was put on every control of every step, undo still says "There is nothing to undo."

## Test 42

### File residence:
`tests/accessibility/accessibility_suite.c`, lines 65-66; `tests/accessibility/window_tests.c`, lines 87-111

### Purpose:
Enforce that the keyboard is never trapped in a control (Requirement 3, WCAG 2.2 SC 2.1.2).

### Assertion expectation:
With the focus on each control of each step in turn, a Tab key given to the dialog manager moves the focus on.

## Test 43

### File residence:
`tests/accessibility/accessibility_suite.c`, lines 78-80; `tests/accessibility/window_tests.c`, lines 381-393

### Purpose:
Enforce that the language of the window can be found by a program (Requirement 3, WCAG 2.2 SC 3.1.1).

### Assertion expectation:
Once the window is made, the one language the process prefers for its texts, as Windows tells it, is `en-US`.

## Test 44

### File residence:
`tests/accessibility/accessibility_suite.c`, lines 81-83; `tests/accessibility/window_tests.c`, lines 113-125

### Purpose:
Enforce that every control has instructions of its own (Requirement 3, WCAG 2.2 SC 3.3.2).

### Assertion expectation:
In the real window, with the focus on each control in turn, the help command puts its help in the status line.

## Test 45

### File residence:
`tests/accessibility/accessibility_suite.c`, lines 84-86; `tests/accessibility/window_tests.c`, lines 395-405

### Purpose:
Enforce that a status message reaches assistive technology (Requirement 3, WCAG 2.2 SC 4.1.3).

### Assertion expectation:
In the real window, UI Automation gives the status line the live setting "polite".

## Test 46

### File residence:
`tests/accessibility/accessibility_suite.c`, lines 87-89; `tests/accessibility/access_tests.c`, lines 143-146

### Purpose:
Enforce that the text follows the text size set in Windows (Requirement 3, WCAG 2.2 SC 1.4.4).

### Assertion expectation:
At 96 dots an inch and a text size of 400%, the layout is scaled as at 225%, the most Windows offers: 216.

## Test 47

### File residence:
`tests/accessibility/accessibility_suite.c`, lines 90-92; `tests/accessibility/draw_tests.c`, lines 96-128

### Purpose:
Enforce that the focus can be seen (Requirement 3, WCAG 2.2 SC 2.4.7).

### Assertion expectation:
With the focus on each control in turn, the window paints 2 or more pixels of another color just left of it.

## Test 48

### File residence:
`tests/accessibility/accessibility_suite.c`, lines 93-95; `tests/accessibility/draw_tests.c`, lines 11-27

### Purpose:
Catch again Bug 2 of the trial: a ticked box showed a filled rectangle, not a check (Rule 24).

### Assertion expectation:
In the picture the window draws of a ticked row, the inside of the box holds marks, on less than two fifths of it.

## Test 49

### File residence:
`tests/accessibility/accessibility_suite.c`, lines 95-97; `tests/accessibility/choice_tests.c`, lines 11-26

### Purpose:
Enforce that a list or choice shows what the design holds, after an undo too (Requirement 5).

### Assertion expectation:
In the real window, after another civ to edit is picked and the pick undone, the choice shows the Britons again.

## Test 50

### File residence:
`tests/reflow/reflow_tests.c`, lines 218-226

### Purpose:
Verify that the real window reflows when it is made narrow (Req. 3, WCAG 1.4.10).

### Assertion expectation:
In a window 420 wide, the Architecture list starts as far left as the Name field.

## Test 51

### File residence:
`tests/reflow/reflow_tests.c`, lines 228-239

### Purpose:
Verify that the real window brings the control in focus into view (Req. 3, WCAG 2.4.11).

### Assertion expectation:
In a window 420 by 360, the Scout list lies between the window's top and bottom once it has the focus.

## Test 52

### File residence:
`tests/reflow/reflow_tests.c`, lines 241-257

### Purpose:
Verify that the window can be made as small as WCAG 1.4.10 asks to be usable (Req. 3).

### Assertion expectation:
The least size the window reports is the frame around 320 by 256, at the scale in use.

## Test 53

### File residence:
`tests/reflow/reflow_tests.c`, lines 259-262

### Purpose:
Verify that the keyboard scrolls a step: Go, Scroll down, Ctrl+Down (Req. 3, WCAG 2.1.1).

### Assertion expectation:
In a window 420 by 360, the scroll bar's place is above 0 after Scroll down.

## Test 54

### File residence:
`tests/reflow/reflow_tests.c`, lines 264-268

### Purpose:
Verify that the keyboard scrolls a step back: Go, Scroll up, Ctrl+Up (Req. 3, WCAG 2.1.1).

### Assertion expectation:
Scroll down twice and Scroll up once leave the scroll bar where one Scroll down leaves it.

## Test 55

### File residence:
`tests/reflow/reflow_tests.c`, lines 270-279

### Purpose:
Verify that the mouse wheel scrolls a step (Req. 3).

### Assertion expectation:
In a window 420 by 360, the scroll bar's place is above 0 after one notch of the wheel downwards.

## Test 56

### File residence:
`tests/reflow/reflow_tests.c`, lines 293-296

### Purpose:
Verify that a row too long for its list is shown on more lines, not cut (Req. 3, WCAG 1.4.10).

### Assertion expectation:
In a window 420 wide, the first row of the bonuses is higher than the third, "Shepherds work 25% faster".

## Test 57

### File residence:
`tests/reflow/reflow_tests.c`, lines 298-305

### Purpose:
Prevent a row of one line from becoming smaller than a target (Req. 3, WCAG 2.5.8).

### Assertion expectation:
The third row of the bonuses is 44 high at the scale in use.

## Test 58

### File residence:
`tests/maintainability/maintainability_suite.c`, lines 19-21; `tests/maintainability/code_rules.c`, lines 51-66

### Purpose:
Enforce that the code holds no comments (Rule 10).

### Assertion expectation:
No `.c` or `.h` file holds `//` or `/*` outside a string; a failure names the file and line.

## Test 59

### File residence:
`tests/maintainability/maintainability_suite.c`, lines 22-24; `tests/maintainability/code_rules.c`, lines 21-34

### Purpose:
Enforce that no function is longer than 60 lines of code (Rule 11, P10 rule 4).

### Assertion expectation:
Every function, from its opening to its closing brace, has at most 60 lines that are not blank.

## Test 60

### File residence:
`tests/maintainability/maintainability_suite.c`, lines 25-27; `tests/maintainability/code_rules.c`, lines 36-49

### Purpose:
Enforce that no line of code is longer than 120 characters (Rule 12).

### Assertion expectation:
Every line of every `.c` and `.h` file has at most 120 characters; a failure names the file and line.

## Test 61

### File residence:
`tests/maintainability/maintainability_suite.c`, lines 28-30; `tests/maintainability/code_rules.c`, lines 51-66

### Purpose:
Enforce that no file of code is longer than 400 lines of code (Rule 13).

### Assertion expectation:
Every `.c` and `.h` file has at most 400 lines that are not blank.

## Test 62

### File residence:
`tests/maintainability/maintainability_suite.c`, lines 30-32; `tests/maintainability/code_patterns.c`, lines 62-65

### Purpose:
Enforce simple control flow without jumps (Rule 26, P10 rule 1).

### Assertion expectation:
The words `goto`, `setjmp` and `longjmp` appear in no code.

## Test 63

### File residence:
`tests/maintainability/maintainability_suite.c`, line 46; `tests/rules/repo_rules.c`, lines 30-45

### Purpose:
Enforce that no folder holds more than 7 files, the main folder 8 (Rule 14).

### Assertion expectation:
Every folder of the files Git sees holds at most 7 files, the main one 8; `documentation/DEVLOG/` is not counted.

## Test 64

### File residence:
`tests/maintainability/maintainability_suite.c`, line 48; `tests/rules/repo_rules.c`, lines 82-95

### Purpose:
Enforce that no two files share a name (Rule 15).

### Assertion expectation:
No two file names are equal when case is ignored, `SKILL.md` aside.

## Test 65

### File residence:
`tests/maintainability/maintainability_suite.c`, line 50; `tests/rules/repo_rules.c`, lines 119-132

### Purpose:
Enforce that the project holds only C and documents (Rule 29).

### Assertion expectation:
Every file ends in `.c`, `.h`, `.md`, `.txt`, `.json` or `.png`, or is `LICENSE`, `.gitignore` or `.gitattributes`.

## Test 66

### File residence:
`tests/maintainability/maintainability_suite.c`, line 52; `tests/rules/repo_rules.c`, lines 168-174

### Purpose:
Enforce that the documents of repo_template are filled in (Rule 3).

### Assertion expectation:
No `.md` file holds two opening braces before a capital, or the template's start or end mark.

## Test 67

### File residence:
`tests/maintainability/maintainability_suite.c`, line 54; `tests/rules/log_rules.c`, lines 31-44

### Purpose:
Enforce the form of the development log (Rules 18 to 21).

### Assertion expectation:
`DEVLOG1.md` has its title, at most 1000 lines, dated days listed in order at its top, and times as hh:mm:ss.

## Test 68

### File residence:
`tests/maintainability/maintainability_suite.c`, line 56; `tests/rules/log_rules.c`, lines 219-234

### Purpose:
Enforce that TESTS.md numbers its tests 1, 2, 3 in order, with short fields (Rules 5, 6).

### Assertion expectation:
Headings and contents both count up from 1, agree, and no field line passes 120 characters.

## Test 69

### File residence:
`tests/maintainability/maintainability_suite.c`, line 58; `tests/rules/log_rules.c`, lines 295-307

### Purpose:
Enforce that TESTS.md documents every test and no more (Rule 6).

### Assertion expectation:
The `## Test` headings are as many as the `case` lines of the three suites.

## Test 70

### File residence:
`tests/maintainability/maintainability_suite.c`, line 60; `tests/rules/log_rules.c`, lines 331-345

### Purpose:
Enforce that every term is one short line (Rule 9).

### Assertion expectation:
No line of `TERMINOLOGY.md` passes 120 characters.

## Regression test 1

### File residence:
`tests/regression/regression_suite.c`, lines 56-57

### Purpose:
Verify that ticking a node of the tech tree ticks every node it needs, through others too (Req. 8).

### Assertion expectation:
For the Britons, with the Archer (4) unticked and so the Arbalester (492), ticking the Arbalester ticks the Archer.

## Regression test 2

### File residence:
`tests/regression/regression_suite.c`, lines 59-60

### Purpose:
Verify that unticking a node of the tech tree unticks every node that needs it, through others too (Req. 8).

### Assertion expectation:
For the Britons, unticking the Archer (4) unticks the Arbalester (492), which needs the Crossbowman.

## Regression test 3

### File residence:
`tests/regression/mod_regression.c`, lines 29-34

### Purpose:
Verify that a civ bonus made of a civ's tech and taken twice counts twice in the game (Req. 8).

### Assertion expectation:
With bonus 50 taken twice, the copy of its tech, tech 12, adds 10 where the game's tech adds 5.

## Regression test 4

### File residence:
`tests/regression/design_regression.c`, lines 37-40

### Purpose:
Prevent again a civ file's language read as the civ after it: the original counts from 0 (bug of 2026-11-09 03:45:50).

### Assertion expectation:
The example civ's language 4, the Japanese in the original's count, is read as slot 5, the Japanese's.

## Regression test 5

### File residence:
`tests/regression/design_regression.c`, lines 42-45

### Purpose:
Prevent again the summary on Finish losing its labels (bug of 2026-11-09 01:20:32).

### Assertion expectation:
For a civ named "Vikings", the summary holds the line "Name: Vikings".

## Regression test 6

### File residence:
`tests/regression/design_regression.c`, lines 47-50

### Purpose:
Prevent again a Next button on Finish, the last step (Bug 3 of 2026-11-09 10:05:44).

### Assertion expectation:
`layout_shows(CONTROL_NEXT, STEP_FINISH)` is false, so the Finish step has no Next button.

## Regression test 7

### File residence:
`tests/regression/mod_regression.c`, lines 36-48

### Purpose:
Prevent again civ descriptions written out of order (bug of 2026-11-09 02:15:04).

### Assertion expectation:
With the game's 120201 line before its 120200 line, the strings hold 120200 before 120201.

## Regression test 8

### File residence:
`tests/regression/mod_regression.c`, lines 50-53

### Purpose:
Prevent the strings of a hero that was not built from being written (bug of 2026-11-09 05:56:32).

### Assertion expectation:
With the hero skipped, the UI mod's strings have no line `44500 "Testhero"` for slot 1's hero string id.

## Regression test 9

### File residence:
`tests/regression/mod_regression.c`, lines 55-58

### Purpose:
Prevent again a civ selection that shows the old civ's bonuses (Bug 4 of 2026-11-09 10:05:44).

### Assertion expectation:
An entry whose `tech_tree_description_string_id` is 120150 is written with 990101, the own id of slot 1.

## Regression test 10

### File residence:
`tests/regression/design_regression.c`, lines 52-58

### Purpose:
Prevent again a new civ that starts with an empty tech tree (Observation 7 of 2026-11-09 10:05:44).

### Assertion expectation:
A civ of the game gets its one node of the catalog, the Barracks, and not the row past the catalog's count.

## Regression test 11

### File residence:
`tests/regression/mod_regression.c`, lines 60-63

### Purpose:
Prevent again a tech tree screen that shows units the civ lacks (Bug 6 of 2026-11-09 10:05:44).

### Assertion expectation:
A Stable that the game's tree has as ResearchedCompleted is written NotAvailable for a design without a Stable.

## Regression test 12

### File residence:
`tests/regression/mod_regression.c`, lines 65-68

### Purpose:
Prevent again a tech tree screen that hides what the civ has (bug of 2026-11-09 10:40:46).

### Assertion expectation:
A Stable that the game's tree has as NotAvailable is written ResearchedCompleted for a design with a Stable.

## Regression test 13

### File residence:
`tests/regression/mod_regression.c`, lines 70-74

### Purpose:
Prevent again the look lines read anew at each load of the catalog (bug of 2026-11-17 06:49:31).

### Assertion expectation:
The catalog holds 2 look lines.

## Regression test 14

### File residence:
`tests/regression/design_regression.c`, lines 60-65

### Purpose:
Prevent a tree as large as the base civ's but not the same from being shown as not done (Bug 1 of 2026-11-16).

### Assertion expectation:
With unit 1 in the design and unit 2 in the base, `wizard_step_is_done` is true for the Name and tech tree step.

## Regression test 15

### File residence:
`tests/regression/design_regression.c`, lines 67-72

### Purpose:
Prevent as many bonuses as the base civ's but not the same from being shown as not done (Bug 1 of 2026-11-16).

### Assertion expectation:
With bonus 4 in the design and bonus 5 in the base, `wizard_step_is_done` is true for the Bonuses step.

## Regression test 16

### File residence:
`tests/regression/design_regression.c`, lines 74-79

### Purpose:
Verify that a team bonus other than the base civ's marks the Bonuses step done (Bug 1 of 2026-11-16).

### Assertion expectation:
With team bonus 8 in the design and 9 in the base, `wizard_step_is_done` is true for the Bonuses step.

## Regression test 17

### File residence:
`tests/regression/design_regression.c`, lines 81-86

### Purpose:
Prevent the base civ's own team bonus from marking the Bonuses step done (Bug 1 of 2026-11-16).

### Assertion expectation:
With team bonus 8 in both the design and the base, `wizard_step_is_done` is false for the Bonuses step.

## Regression test 18

### File residence:
`tests/regression/design_regression.c`, lines 88-94

### Purpose:
Prevent again a list of amounts joined by commas from being scaled as a lone number (bug of 2026-11-22 02:10:47).

### Assertion expectation:
"Military units cost -10, 15, 20, 25% food ..." counted twice reads "Military units cost -19, 27.8, 36, 43.8% food ...".

## Regression test 19

### File residence:
`tests/regression/window_regression.c`, lines 30-46

### Purpose:
Verify that the slider, moved in the window, sets the count of the civ bonus whose field has the focus (Req. 8).

### Assertion expectation:
The slider of the field "-50" of "Town Centers cost -50% wood", ended at 200, makes it "Town Centers cost -75% wood".

## Regression test 20

### File residence:
`tests/regression/window_regression.c`, lines 48-63

### Purpose:
Prevent a step from being shown as done when the window opens, before the user did anything (Bug 1 of 2026-11-16).

### Assertion expectation:
In a new window, made but not shown, no row of the list of steps holds "(done)", though Generic fills the tree.

## Regression test 21

### File residence:
`tests/regression/window_regression.c`, lines 65-68

### Purpose:
Verify that Exit closes a window without changes, without a question (fault 1 of 2026-11-15 22:56:15).

### Assertion expectation:
In a new window the Exit command closes the window; no question about unsaved changes is asked.

## Regression test 22

### File residence:
`tests/regression/window_regression.c`, lines 193-198

### Purpose:
Verify that a list's scroll bar shows its arrows and knob without the mouse over it (Bug 2 of 2026-11-16).

### Assertion expectation:
The up arrow of the shown Tech tree list, drawn by `WM_PRINT`, holds at least 4 colours; the thin bar holds 2.

## Regression test 23

### File residence:
`tests/regression/window_regression.c`, lines 200-207

### Purpose:
Verify that the window's scroll bar shows its arrows and knob without the mouse over it (Bug 2 of 2026-11-16).

### Assertion expectation:
In a window of 480 by 360 on the first step, the top right corner of its inside holds at least 4 colours.

## Regression test 24

### File residence:
`tests/regression/window_regression.c`, lines 209-219

### Purpose:
Prevent the window's scroll bar from taking the step's margin on the right (Bug 2 of 2026-11-16).

### Assertion expectation:
In a window of 480 by 360, Next ends as far from the scroll bar as Back starts from the left, to a pixel.

## Regression test 25

### File residence:
`tests/regression/window_regression.c`, lines 221-230

### Purpose:
Verify that each notch of the mouse wheel moves a list one row, up or down (Bug 4 of 2026-11-16).

### Assertion expectation:
From row 2 of the Tech tree list, a notch up and two down, run through `app_run_messages`, leave row 3 on top.

## Regression test 26

### File residence:
`tests/regression/window_regression.c`, lines 232-241

### Purpose:
Verify that a press on either arrow of a list's scroll bar moves it one row, up or down (Bug 4 of 2026-11-16).

### Assertion expectation:
From row 2, a click on the up arrow, one on the down arrow and a double click on it, each released, leave row 3.

## Regression test 27

### File residence:
`tests/regression/window_regression.c`, lines 253-269

### Purpose:
Prevent a list on the screen from sliding when its wheel or arrows scroll it (Bug 3 of 2026-11-16).

### Assertion expectation:
Two notches, a click and a double click on the up arrow each take less than 150 ms; sliding takes 190 to 530 ms.

## Regression test 28

### File residence:
`tests/regression/window_regression.c`, lines 271-279

### Purpose:
Verify that holding a list's arrow keeps moving it a row at a time, as a normal scroll bar does (Bug 4 of 2026-11-16).

### Assertion expectation:
A press on the down arrow, still held, and one tick of `LIST_REPEAT_TIMER` leave row 2 on top.

## Regression test 29

### File residence:
`tests/regression/window_regression.c`, lines 281-290

### Purpose:
Prevent the arrows of a list's scroll bar from taking a press on the bar between them (Bug 4 of 2026-11-16).

### Assertion expectation:
With the list at its end, a press just under the up arrow moves its top row up by more than one row.

## Regression test 30

### File residence:
`tests/regression/window_regression.c`, lines 292-299

### Purpose:
Prevent again an open dropdown sliding under the wheel: a notch moves it one row (Bug 1 of 2026-11-21).

### Assertion expectation:
With Civ to edit open, one notch down leaves row 1 at the top of the dropdown's rows.

## Redundancy test 1

### File residence:
`tests/redundancy/civ_tests.c`, lines 104-108

### Purpose:
Verify that a civ of the game gives the design the game's name for it (Req. 12).

### Assertion expectation:
The Britons of a small catalog give the design the name "Britons".

## Redundancy test 2

### File residence:
`tests/redundancy/civ_tests.c`, lines 110-114

### Purpose:
Verify that a civ of the game names its unique unit as the game does (Req. 12).

### Assertion expectation:
The Britons give the unique unit's name field "Longbowman".

## Redundancy test 3

### File residence:
`tests/redundancy/civ_tests.c`, lines 116-120

### Purpose:
Verify that a civ of the game names its Castle Age tech as the game does (Req. 12).

### Assertion expectation:
The Franks give the Castle Age tech's name field "Bearded Axe".

## Redundancy test 4

### File residence:
`tests/redundancy/civ_tests.c`, lines 122-126

### Purpose:
Verify that a civ of the game names its Imperial Age tech as the game does (Req. 12).

### Assertion expectation:
The Britons give the Imperial Age tech's name field "Warwolf".

## Redundancy test 5

### File residence:
`tests/redundancy/civ_tests.c`, lines 128-132

### Purpose:
Verify that a civ of the game gives the design the tagline the catalog holds for it (Req. 12).

### Assertion expectation:
The Britons give the tagline field "Foot Archer".

## Redundancy test 6

### File residence:
`tests/redundancy/civ_tests.c`, lines 134-138

### Purpose:
Verify that a civ of the game gives the design the description the catalog holds for it (Req. 12).

### Assertion expectation:
The Britons give the description field "An archer civilization".

## Redundancy test 7

### File residence:
`tests/redundancy/civ_tests.c`, lines 140-145

### Purpose:
Verify that choosing a civ of the game takes away the hero of the civ before (Req. 12).

### Assertion expectation:
A design with Joan of Arc as its hero has no hero once the Britons are applied.

## Redundancy test 8

### File residence:
`tests/redundancy/civ_tests.c`, lines 147-150

### Purpose:
Verify that picking a unique unit gives its name field the game's name (Req. 12).

### Assertion expectation:
Picking the Throwing Axeman's row names the unique unit "Throwing Axeman".

## Redundancy test 9

### File residence:
`tests/redundancy/civ_tests.c`, lines 152-156

### Purpose:
Verify that picking a Castle Age tech gives its name field the game's name (Req. 12).

### Assertion expectation:
Picking the Bearded Axe's row names the Castle Age tech "Bearded Axe".

## Redundancy test 10

### File residence:
`tests/redundancy/civ_tests.c`, lines 158-161

### Purpose:
Verify that picking a hero gives the hero's name field the game's name (Req. 12).

### Assertion expectation:
Picking Joan of Arc's row names the hero "Joan of Arc".

## Redundancy test 11

### File residence:
`tests/redundancy/civ_tests.c`, lines 163-170

### Purpose:
Verify that unticking the Hero box empties the hero's name (Req. 12).

### Assertion expectation:
After Joan of Arc is picked, unticking the Hero box leaves the hero's name empty.

## Redundancy test 12

### File residence:
`tests/redundancy/civ_tests.c`, lines 172-177

### Purpose:
Verify that a hero whose name is emptied is told as lacking a name (Req. 12).

### Assertion expectation:
After Joan of Arc is picked and her name is set to "", `civ_design_hero_lacks_name` is true.

## Redundancy test 13

### File residence:
`tests/redundancy/civ_tests.c`, lines 179-189

### Purpose:
Verify that a civ as the game has it marks no step done (Req. 12).

### Assertion expectation:
The Britons compared with the Britons give no step as done.

## Redundancy test 14

### File residence:
`tests/redundancy/civ_tests.c`, lines 191-196

### Purpose:
Verify that the Finish step is done once a whole civ differs from the game's (Req. 12).

### Assertion expectation:
The Britons renamed Testonia mark the Finish step done.

## Redundancy test 15

### File residence:
`tests/redundancy/civ_tests.c`, lines 206-210

### Purpose:
Verify that a changed civ is counted among the civs changed (Req. 12).

### Assertion expectation:
After the Britons renamed Testonia are kept, the roster counts 1 civ.

## Redundancy test 16

### File residence:
`tests/redundancy/civ_tests.c`, lines 212-216

### Purpose:
Verify that a civ as the game has it is not put in the mod (Req. 12).

### Assertion expectation:
Keeping the Britons unchanged flips no mark, and the roster counts 0 civs.

## Redundancy test 17

### File residence:
`tests/redundancy/civ_tests.c`, lines 218-224

### Purpose:
Verify that a civ changed again is in the mod with its newer edits (Req. 12).

### Assertion expectation:
Testonia kept, renamed Newer and kept again: no mark flips and the mod's first civ is named Newer.

## Redundancy test 18

### File residence:
`tests/redundancy/civ_tests.c`, lines 226-230

### Purpose:
Verify that a civ saved as the game has it again is taken out of the mod (Req. 12).

### Assertion expectation:
Testonia kept, then kept as the game has it: the mark flips and the roster counts 0 civs.

## Redundancy test 19

### File residence:
`tests/redundancy/civ_tests.c`, lines 232-238

### Purpose:
Verify that the edits of a civ are kept while another civ is edited (Req. 12).

### Assertion expectation:
After the Franks are kept, the draft of the Britons is still named "Testonia".

## Redundancy test 20

### File residence:
`tests/redundancy/civ_tests.c`, lines 240-243

### Purpose:
Verify that the roster marks a civ that differs from the game's as changed (Req. 12).

### Assertion expectation:
Keeping the Britons renamed Testonia flips their mark, and `roster_is_changed` is true for them.

## Redundancy test 21

### File residence:
`tests/redundancy/civ_tests.c`, lines 245-250

### Purpose:
Verify that the roster marks no civ as changed that is as the game has it (Req. 12).

### Assertion expectation:
The Britons kept unchanged are not marked, and neither is a slot past the last.

## Redundancy test 22

### File residence:
`tests/redundancy/civ_tests.c`, lines 284-291

### Purpose:
Verify that the civs of the mod come in the order of the slots they take (Req. 12).

### Assertion expectation:
With the Franks saved before the Britons, the mod's first civ is the Britons, slot 1.

## Redundancy test 23

### File residence:
`tests/redundancy/civ_tests.c`, lines 293-298

### Purpose:
Verify that a window opened anew holds no draft kept before it (Req. 12).

### Assertion expectation:
After a draft of the Britons is kept and the civs are started anew, no draft of the Britons is found.

## Redundancy test 24

### File residence:
`tests/redundancy/civ_tests.c`, lines 300-305

### Purpose:
Verify that a civ replacing a slot past the last one is left out of the mod (Req. 12).

### Assertion expectation:
A civ whose slot is `ROSTER_SLOT_LIMIT` flips no mark, and the roster counts 0 civs.

## Redundancy test 25

### File residence:
`tests/redundancy/civ_tests.c`, lines 259-263

### Purpose:
Verify that the roster counts a changed civ in slot 0, the first (Req. 12).

### Assertion expectation:
A changed civ kept with slot 0 makes the roster count 1 civ.

## Redundancy test 26

### File residence:
`tests/redundancy/civ_tests.c`, lines 265-269

### Purpose:
Verify that the roster gives the mod a changed civ in slot 0, the first (Req. 12).

### Assertion expectation:
With a changed civ kept in slot 0, `roster_civ` gives a civ at index 0.

## Redundancy test 27

### File residence:
`tests/redundancy/civ_tests.c`, lines 271-275

### Purpose:
Verify that the roster marks a changed civ in slot 0, the first (Req. 12).

### Assertion expectation:
With a changed civ kept in slot 0, `roster_is_changed` is true for slot 0.

## Redundancy test 28

### File residence:
`tests/redundancy/civ_tests.c`, lines 277-282

### Purpose:
Verify that a roster started anew forgets a changed civ in slot 0, the first (Req. 12).

### Assertion expectation:
A changed civ kept in slot 0, then `roster_start`: the roster counts 0 civs.

## Redundancy test 29

### File residence:
`tests/redundancy/civ_tests.c`, lines 307-312

### Purpose:
Verify that a civ with another first look, its architecture, is not the civ of the game (Req. 12).

### Assertion expectation:
The Britons with the next architecture are not the same as the Britons of the game.

## Redundancy test 30

### File residence:
`tests/redundancy/civ_tests.c`, lines 314-319

### Purpose:
Verify that a civ whose Castle Age tech costs more of the first resource is not the civ of the game (Req. 12).

### Assertion expectation:
The Britons whose Castle Age tech costs 1 more of the first resource are not the Britons of the game.

## Redundancy test 31

### File residence:
`tests/redundancy/civ_tests.c`, lines 321-328

### Purpose:
Verify that a civ given a bonus of the civ edited before it is not taken for its civ of the game (Req. 12).

### Assertion expectation:
The Britons, after the Franks, with their bonus swapped for the Franks' second, are not the Britons of the game.

## Redundancy test 32

### File residence:
`tests/redundancy/game_tests.c`, lines 53-56

### Purpose:
Verify that the game's description of a civ gives it its tagline (Req. 12).

### Assertion expectation:
"Foot Archer civilization" on the first line of the description gives the tagline "Foot Archer".

## Redundancy test 33

### File residence:
`tests/redundancy/game_tests.c`, lines 58-61

### Purpose:
Verify that a civ's description is the game's text after its first line (Req. 12).

### Assertion expectation:
The Britons' description starts with "- Shepherds", the text after the empty line.

## Redundancy test 34

### File residence:
`tests/redundancy/game_tests.c`, lines 63-66

### Purpose:
Verify that a line break written as \n by the game is a line break of the field (Req. 12).

### Assertion expectation:
The description holds "faster", a carriage return, a line feed and "Towers".

## Redundancy test 35

### File residence:
`tests/redundancy/game_tests.c`, lines 68-71

### Purpose:
Verify that a quote escaped by the game is a plain quote in the description (Req. 12).

### Assertion expectation:
The description holds "shoot" between two plain quotes.

## Redundancy test 36

### File residence:
`tests/redundancy/game_tests.c`, lines 73-76

### Purpose:
Verify that a civ the game does not describe gets no tagline, not one from before (Req. 12).

### Assertion expectation:
The Franks, whose description is not in the strings, have their stale tagline emptied.

## Redundancy test 37

### File residence:
`tests/redundancy/game_tests.c`, lines 78-82

### Purpose:
Verify that reading the texts of a game that is found succeeds (Req. 12).

### Assertion expectation:
Reading the small described game answers true.

## Redundancy test 38

### File residence:
`tests/redundancy/game_tests.c`, lines 84-87

### Purpose:
Verify that a civ the game does not describe gets no description, not one from before (Req. 12).

### Assertion expectation:
The Franks, whose description is not in the strings, have their stale description emptied.

## Redundancy test 39

### File residence:
`tests/redundancy/game_tests.c`, lines 89-92

### Purpose:
Verify that a text of the game without its closing quote ends where its line ends (Req. 12).

### Assertion expectation:
An unclosed description followed by another line gives the description "Shepherds" alone.

## Redundancy test 40

### File residence:
`tests/redundancy/game_tests.c`, lines 94-97

### Purpose:
Verify that the spaces and line breaks at the end of a text of the game are cut (Req. 12).

### Assertion expectation:
A description that ends in a space and a \n gives the description "Shepherds".

## Redundancy test 41

### File residence:
`tests/redundancy/game_tests.c`, lines 99-102

### Purpose:
Verify that a first line not ending in " civilization" is the whole tagline (Req. 12).

### Assertion expectation:
The first line "Archers of the north" gives the tagline "Archers of the north".

## Redundancy test 42

### File residence:
`tests/redundancy/game_tests.c`, lines 104-115

### Purpose:
Verify that a tagline longer than its field holds is cut to fit (Req. 12).

### Assertion expectation:
A first line of 300 letters gives a tagline of 255 letters, all that its field of 256 holds.

## Redundancy test 43

### File residence:
`tests/redundancy/tools/plan_tests.c`, lines 8-15

### Purpose:
Verify that the analyzed build compiles each source with gcc's static analyzer (Rule 26, Req. 10).

### Assertion expectation:
The compile command of `src/core/check.c` in the analyzed build holds " -fanalyzer -c src/core/check.c -o ".

## Redundancy test 44

### File residence:
`tests/redundancy/dat/patch_tests.c`, lines 9-13

### Purpose:
Verify that the first patch of the dat file is found, so that no civ patches its place twice (Req. 12).

### Assertion expectation:
After a patch of the first byte of a small dat file, that byte is said to be patched.

## Redundancy test 45

### File residence:
`tests/redundancy/mod/built_mod_tests.c`, lines 48-55

### Purpose:
Verify that a build with no civ in the mod is refused before anything is built (Req. 9, 12).

### Assertion expectation:
The civs of a build of none are answered as not finished.

## Redundancy test 46

### File residence:
`tests/redundancy/mod/built_mod_tests.c`, lines 57-66

### Purpose:
Verify that building a mod of an unfinished civ fails (Req. 9, 12).

### Assertion expectation:
Building a mod of one civ just reset answers false.

## Redundancy test 47

### File residence:
`tests/redundancy/mod/built_mod_tests.c`, lines 68-75

### Purpose:
Verify that a civ's description takes the place of the game's description of its civ, once (Req. 12).

### Assertion expectation:
A civ named by the Jurchens' string id leaves no 120200 "Jurchens" in the mod's strings, and 120200 once.

## Redundancy test 48

### File residence:
`tests/redundancy/mod/built_mod_tests.c`, lines 77-80

### Purpose:
Verify that the game's descriptions of the civs not in the mod are kept in its strings (Req. 12).

### Assertion expectation:
The mod's strings of a civ for the Britons still hold the game's 120200 "Jurchens".

## Redundancy test 49

### File residence:
`tests/redundancy/mod/built_mod_tests.c`, lines 82-85

### Purpose:
Verify that a civ whose description id lies past the game's descriptions still gets one (Req. 12).

### Assertion expectation:
A civ named by string id 10399 gets its description as 120278 in the mod's strings.

## Redundancy test 50

### File residence:
`tests/redundancy/mod/built_mod_tests.c`, lines 87-90

### Purpose:
Verify that a civ left from a bigger build, past the count of the mod, gets no description (Req. 12).

### Assertion expectation:
With the Franks left past a count of one civ, the mod's strings hold no line 120151 for them.

## Redundancy test 51

### File residence:
`tests/redundancy/mod/built_mod_tests.c`, lines 141-146

### Purpose:
Verify that the description of the Britons, the first id of the game's descriptions, is written once (Req. 12).

### Assertion expectation:
A civ named by the Britons' string id leaves one line 120150 in the mod's strings, not two.

## Redundancy test 52

### File residence:
`tests/redundancy/mod/built_mod_tests.c`, lines 100-112

### Purpose:
Verify that a tech of the game takes the requirements of a mod's civs only while it has room (Req. 9, 12).

### Assertion expectation:
With the 4 free places of a tech's requirements taken by 4 civs, a fifth civ's requirement is refused.

## Redundancy test 53

### File residence:
`tests/redundancy/mod/built_mod_tests.c`, lines 132-139

### Purpose:
Verify that a civ whose description passes the 8192 bytes of its room fails the mod's strings (Req. 9, 12).

### Assertion expectation:
With 24 bonuses of 300 letters, the strings are written for the description "Archers", not for 1535 letters.

## Redundancy test 54

### File residence:
`tests/redundancy/present/filter_tests.c`, lines 144-145

### Purpose:
Verify that a civ's name in a filter leaves that civ's castle tech alone (Req. 12).

### Assertion expectation:
In a small catalog, the filter "brit" leaves 1 row of the castle techs.

## Redundancy test 55

### File residence:
`tests/redundancy/present/filter_tests.c`, lines 146-148

### Purpose:
Verify that a civ's name in a filter leaves the castle tech of that civ, not another's (Req. 12).

### Assertion expectation:
The filter "franks" leaves Bearded Axe, row 2, first of the castle techs.

## Redundancy test 56

### File residence:
`tests/redundancy/present/filter_tests.c`, lines 149-151

### Purpose:
Verify that a civ's name in a filter leaves the imperial tech of that civ (Req. 12).

### Assertion expectation:
The filter "franks" leaves Chivalry, row 3, first of the imperial techs.

## Redundancy test 57

### File residence:
`tests/redundancy/present/filter_tests.c`, lines 152-154

### Purpose:
Verify that a civ's name in a filter leaves the unique unit of that civ (Req. 12).

### Assertion expectation:
The filter "franks" leaves the Throwing Axeman, row 1, first of the unique units.

## Redundancy test 58

### File residence:
`tests/redundancy/present/filter_tests.c`, lines 155-156

### Purpose:
Verify that a civ's name in a filter leaves no unique unit of another civ (Req. 12).

### Assertion expectation:
The filter "brit" leaves 1 row of the unique units.

## Redundancy test 59

### File residence:
`tests/redundancy/present/filter_tests.c`, lines 157-159

### Purpose:
Verify that a civ's name in a filter leaves every civ bonus of that civ (Req. 12).

### Assertion expectation:
The filter "franks" leaves 2 rows of the civ bonuses, the two of the Franks.

## Redundancy test 60

### File residence:
`tests/redundancy/present/filter_tests.c`, lines 160-162

### Purpose:
Verify that a civ's name in a filter leaves no civ bonus of another civ (Req. 12).

### Assertion expectation:
The filter "brit" leaves 1 row of the civ bonuses.

## Redundancy test 61

### File residence:
`tests/redundancy/present/filter_tests.c`, lines 163-165

### Purpose:
Verify that a civ's name in a filter leaves the team bonus of that civ (Req. 12).

### Assertion expectation:
The filter "franks" leaves the team bonus of the Franks, row 1, first.

## Redundancy test 62

### File residence:
`tests/redundancy/present/filter_tests.c`, lines 166-167

### Purpose:
Verify that a civ's name in a filter leaves the nodes that civ has (Req. 12).

### Assertion expectation:
The filter "franks" leaves 2 rows of the tech tree.

## Redundancy test 63

### File residence:
`tests/redundancy/present/filter_tests.c`, lines 168-169

### Purpose:
Verify that a civ's name in a filter leaves no node that civ lacks (Req. 12).

### Assertion expectation:
The filter "brit" leaves 1 row of the tech tree.

## Redundancy test 64

### File residence:
`tests/redundancy/present/filter_tests.c`, lines 170-172

### Purpose:
Verify that a filter is a civ's name only from the name's start (Req. 12).

### Assertion expectation:
The filter "ritons" leaves 0 rows of the castle techs.

## Redundancy test 65

### File residence:
`tests/redundancy/present/filter_tests.c`, lines 173-174

### Purpose:
Verify that a civ's name in the filter of the heroes leaves none, as no civ has a hero (Req. 12).

### Assertion expectation:
The filter "franks" leaves 0 rows of the heroes.

## Redundancy test 66

### File residence:
`tests/redundancy/present/filter_tests.c`, lines 183-185

### Purpose:
Verify that a filter still leaves the rows that hold its text (Req. 12).

### Assertion expectation:
The filter "bearded" leaves Bearded Axe, row 2, first of the castle techs.

## Redundancy test 67

### File residence:
`tests/redundancy/present/filter_tests.c`, lines 186-187

### Purpose:
Verify that Civ to edit leaves out a civ that is not a ranked civ (Req. 12).

### Assertion expectation:
Of the Britons, the Franks and the unranked Spartans, Civ to edit has 2 rows.

## Redundancy test 68

### File residence:
`tests/redundancy/present/filter_tests.c`, lines 188-189

### Purpose:
Verify that a civ without an `unranked` line is a ranked civ (Req. 12).

### Assertion expectation:
`catalog_is_ranked` is true for the Britons, slot 1.

## Redundancy test 69

### File residence:
`tests/redundancy/present/filter_tests.c`, lines 190-192

### Purpose:
Verify that a civ with an `unranked` line is not a ranked civ (Req. 12).

### Assertion expectation:
`catalog_is_ranked` is false for the Spartans, slot 48.

## Redundancy test 70

### File residence:
`tests/redundancy/present/filter_tests.c`, lines 193-195

### Purpose:
Verify that the first filter of the layout names the list it narrows (Req. 12).

### Assertion expectation:
`binding_filtered_list(CONTROL_TREE_FILTER)` is `CONTROL_TREE_LIST`.

## Redundancy test 71

### File residence:
`tests/redundancy/present/filter_tests.c`, lines 196-198

### Purpose:
Verify that the last filter of the layout names the list it narrows (Req. 12).

### Assertion expectation:
`binding_filtered_list(CONTROL_IMPERIAL_TECH_FILTER)` is `CONTROL_IMPERIAL_TECH_LIST`.

## Redundancy test 72

### File residence:
`tests/redundancy/present/filter_tests.c`, lines 199-201

### Purpose:
Verify that a control that is no filter narrows no list (Req. 12).

### Assertion expectation:
`binding_filtered_list(CONTROL_NAME)` is `CONTROL_COUNT`.

## Redundancy test 73

### File residence:
`tests/redundancy/present/filter_tests.c`, lines 202-203

### Purpose:
Verify that a list of numbers holds its first number (Req. 12).

### Assertion expectation:
`tsv_list_holds` is true for 100 in the list 100, 101, 102.

## Redundancy test 74

### File residence:
`tests/redundancy/present/filter_tests.c`, lines 204-205

### Purpose:
Verify that a list of numbers holds its last number (Req. 12).

### Assertion expectation:
`tsv_list_holds` is true for 102 in the list 100, 101, 102.

## Redundancy test 75

### File residence:
`tests/redundancy/present/filter_tests.c`, lines 206-207

### Purpose:
Verify that a list of numbers holds no number past its count (Req. 12).

### Assertion expectation:
`tsv_list_holds` is false for 102 in the list 100, 101, 102 counted as 2.

## Redundancy test 76

### File residence:
`tests/redundancy/present/filter_tests.c`, lines 208-209

### Purpose:
Verify that an empty list of numbers holds nothing (Req. 12).

### Assertion expectation:
`tsv_list_holds` is false for 100 in a list counted as 0.

## Redundancy test 77

### File residence:
`tests/redundancy/present/filter_tests.c`, lines 210-212

### Purpose:
Verify that a look choice, held by the Appearance popup, is shown on no step (Req. 12).

### Assertion expectation:
`layout_shows(CONTROL_ARCHITECTURE, STEP_NAME_AND_TREE)` is false.

## Redundancy test 78

### File residence:
`tests/redundancy/present/filter_tests.c`, lines 226-227

### Purpose:
Verify that a build with nothing skipped is whole (Req. 12).

### Assertion expectation:
`summary_build_is_whole` is true for a report with nothing skipped.

## Redundancy test 79

### File residence:
`tests/redundancy/present/filter_tests.c`, lines 228-229

### Purpose:
Verify that a build that skipped the team bonus is not whole (Req. 12).

### Assertion expectation:
`summary_build_is_whole` is false with `team_bonuses_skipped` 1.

## Redundancy test 80

### File residence:
`tests/redundancy/present/filter_tests.c`, lines 230-231

### Purpose:
Verify that a build that skipped a multiplier is not whole (Req. 12).

### Assertion expectation:
`summary_build_is_whole` is false with `multipliers_skipped` 1.

## Redundancy test 81

### File residence:
`tests/redundancy/present/filter_tests.c`, lines 232-233

### Purpose:
Verify that a build that skipped a unique tech is not whole (Req. 12).

### Assertion expectation:
`summary_build_is_whole` is false with `unique_techs_skipped` 1.

## Redundancy test 82

### File residence:
`tests/redundancy/present/filter_tests.c`, lines 234-235

### Purpose:
Verify that a build that skipped the unique unit is not whole (Req. 12).

### Assertion expectation:
`summary_build_is_whole` is false with `unit_skipped` true.

## Redundancy test 83

### File residence:
`tests/redundancy/present/filter_tests.c`, lines 236-237

### Purpose:
Verify that a build whose tech tree overflowed is not whole (Req. 12).

### Assertion expectation:
`summary_build_is_whole` is false with `tree_overflowed` true.

## Redundancy test 84

### File residence:
`tests/redundancy/present/filter_tests.c`, lines 238-239

### Purpose:
Verify that a build that skipped the hero is not whole (Req. 12).

### Assertion expectation:
`summary_build_is_whole` is false with `hero_skipped` true.

## Redundancy test 85

### File residence:
`tests/redundancy/present/filter_tests.c`, lines 240-241

### Purpose:
Verify that clearing the civ bonuses of a civ that has one is a change (Req. 12).

### Assertion expectation:
`civ_design_clear_bonuses` is true for a design with one civ bonus.

## Redundancy test 86

### File residence:
`tests/redundancy/present/filter_tests.c`, lines 242-243

### Purpose:
Verify that clearing the civ bonuses of a civ without any is no change (Req. 12).

### Assertion expectation:
`civ_design_clear_bonuses` is false for a new design.

## Redundancy test 87

### File residence:
`tests/redundancy/present/filter_tests.c`, lines 244-246

### Purpose:
Verify that clearing the civ bonuses leaves the civ none (Req. 12).

### Assertion expectation:
After `civ_design_clear_bonuses`, the design no longer has the civ bonus it had.

## Redundancy test 88

### File residence:
`tests/redundancy/present/filter_tests.c`, lines 247-248

### Purpose:
Verify that the built mark, which only tells, takes no focus (Req. 12).

### Assertion expectation:
`layout_is_focusable(CONTROL_BUILT)` is false.

## Redundancy test 89

### File residence:
`tests/redundancy/present/filter_tests.c`, lines 249-251

### Purpose:
Verify that the panel of the selected team bonus keeps its height as the window grows (Req. 12).

### Assertion expectation:
`layout_box` gives `CONTROL_TEAM_INFO` the same height in a window 640 high and in one 2000 high.

## Redundancy test 90

### File residence:
`tests/redundancy/bonus/scaled_text_tests.c`, lines 20-22

### Purpose:
Verify that the percents of a list of signed amounts in a bonus's text are scaled as one list (Req. 10).

### Assertion expectation:
"Cavalry +20/+30% hit points" counted twice reads "Cavalry +44/+69% hit points", not "+40/+69%".

## Redundancy test 91

### File residence:
`tests/redundancy/bonus/scaled_text_tests.c`, lines 23-25

### Purpose:
Verify that a sign joined to the word before it is no sign in a bonus's text (Req. 12).

### Assertion expectation:
"Elite-2 units" counted twice reads "Elite-2 units": the 2 is left as it is.

## Redundancy test 92

### File residence:
`tests/redundancy/bonus/scaled_text_tests.c`, lines 26-28

### Purpose:
Verify that a space alone does not join a number to the list before it in a bonus's text (Req. 12).

### Assertion expectation:
"+5 10 food" counted twice reads "+10 10 food": the 10 is left as it is.

## Redundancy test 93

### File residence:
`tests/redundancy/bonus/scaled_text_tests.c`, lines 29-31

### Purpose:
Verify that a comma that no space follows does not join a number to the list before it in a bonus's text (Req. 12).

### Assertion expectation:
"+5,10 food" counted twice reads "+10,10 food": the 10 is left as it is.

## Redundancy test 94

### File residence:
`tests/redundancy/bonus/between_tests.c`, lines 141-142

### Purpose:
Verify that a count that is not whole is saved in a civ file and opened again unchanged (Req. 12).

### Assertion expectation:
A civ bonus counted 1.5 times, composed and interpreted again, counts 1500000 millionths.

## Redundancy test 95

### File residence:
`tests/redundancy/bonus/between_tests.c`, lines 143-145

### Purpose:
Verify that a percent is raised to a count that is not whole (Req. 12).

### Assertion expectation:
"Cavalry +20% hit points" counted 1.5 times reads "Cavalry +31.5% hit points".

## Redundancy test 96

### File residence:
`tests/redundancy/bonus/between_tests.c`, lines 146-148

### Purpose:
Verify that a percent without a sign before "cheaper" counted 1.5 times is a cut raised to 1.5 (Req. 12).

### Assertion expectation:
"Siege Workshop units 15% cheaper" counted 1.5 times reads "Siege Workshop units 21.6% cheaper".

## Redundancy test 97

### File residence:
`tests/redundancy/bonus/between_tests.c`, lines 149-151

### Purpose:
Verify that a change that multiplies is raised to a count that is not whole (Req. 12).

### Assertion expectation:
A speed multiplied by 1.21 and counted 1.5 times is multiplied by 1.331.

## Redundancy test 98

### File residence:
`tests/redundancy/bonus/between_tests.c`, lines 152-155

### Purpose:
Verify that an armor added at a count that is not whole is rounded to the nearest whole (Req. 12).

### Assertion expectation:
A pierce armor of +5 counted 1.5 times adds 8.

## Redundancy test 99

### File residence:
`tests/redundancy/bonus/between_tests.c`, lines 156-159

### Purpose:
Verify that an attack multiplied at a count that is not whole is raised to that count (Req. 12).

### Assertion expectation:
A pierce attack multiplied by 121 hundredths and counted 1.5 times is multiplied by 133 hundredths.

## Redundancy test 100

### File residence:
`tests/redundancy/bonus/between_tests.c`, lines 160-161

### Purpose:
Verify that a spawn at a count that is not whole makes the nearest whole number of units (Req. 12).

### Assertion expectation:
5 militia spawned at the barracks and counted 1.5 times become 8.

## Redundancy test 101

### File residence:
`tests/redundancy/bonus/between_tests.c`, lines 114-115

### Purpose:
Verify that a decimal is written with as many as nine places (Req. 12).

### Assertion expectation:
5 written with nine places reads "0.000000005".

## Redundancy test 102

### File residence:
`tests/redundancy/bonus/between_tests.c`, lines 116-117

### Purpose:
Verify that a decimal with more places than its writer allows is refused, not written (Req. 12).

### Assertion expectation:
5 with ten places is not written, and the text stays empty.

## Redundancy test 103

### File residence:
`tests/redundancy/bonus/between_tests.c`, lines 118-119

### Purpose:
Verify that a whole number below zero is written with its sign (Req. 12).

### Assertion expectation:
-5 written with no places reads "-5".

## Redundancy test 104

### File residence:
`tests/redundancy/bonus/between_tests.c`, lines 120-122

### Purpose:
Verify that a number of 24 letters, the most a number may have, is read (Req. 12).

### Assertion expectation:
"000000000000000000000001" is read as 1.

## Redundancy test 105

### File residence:
`tests/redundancy/bonus/between_tests.c`, lines 123-125

### Purpose:
Verify that a number is read whole up to the limit of its digits (Req. 12).

### Assertion expectation:
"1000000000000000000" is read as 1000000000000000000.

## Redundancy test 106

### File residence:
`tests/redundancy/bonus/between_tests.c`, lines 126-127

### Purpose:
Verify that a text where a number is wanted is not read as a decimal (Req. 12).

### Assertion expectation:
In `{"multiplier": "2"}`, "multiplier" gives no decimal.

## Redundancy test 107

### File residence:
`tests/redundancy/bonus/between_tests.c`, lines 128-130

### Purpose:
Verify that a count past the most makes no multiplier (Req. 12).

### Assertion expectation:
The count 11 makes a multiplier of 0 millionths.

## Redundancy test 108

### File residence:
`tests/redundancy/bonus/between_tests.c`, lines 131-132

### Purpose:
Verify that a civ bonus refuses a multiplier below 1, and says so (Req. 12).

### Assertion expectation:
Setting the multiplier of a ticked civ bonus to 0 millionths returns false.

## Redundancy test 109

### File residence:
`tests/redundancy/bonus/number_tests.c`, lines 47-49

### Purpose:
Verify that a percent in a civ bonus is a number to set, at its place in the text (Req. 12).

### Assertion expectation:
"Cavalry +20% hit points" holds one number to set, whose text is "20".

## Redundancy test 110

### File residence:
`tests/redundancy/bonus/number_tests.c`, lines 50-52

### Purpose:
Verify that a value typed for a percent cut gives the count that shows it (Req. 12).

### Assertion expectation:
A cut of 50% typed as 75 gives the count 2.

## Redundancy test 111

### File residence:
`tests/redundancy/bonus/number_tests.c`, lines 53-55

### Purpose:
Verify that a value typed for a power gives the count that shows it (Req. 12).

### Assertion expectation:
A power of 2 typed as 8 gives the count 3.

## Redundancy test 112

### File residence:
`tests/redundancy/bonus/number_tests.c`, lines 56-58

### Purpose:
Verify that a value typed for an amount gives the count that shows it (Req. 12).

### Assertion expectation:
An amount of 5 typed as 7.5 gives the count 1.5.

## Redundancy test 113

### File residence:
`tests/redundancy/bonus/number_tests.c`, lines 59-61

### Purpose:
Verify that a civ bonus without numbers is shown as it is, with no count (Req. 12).

### Assertion expectation:
"Monks double healing range" reads "Monks double healing range".

## Redundancy test 114

### File residence:
`tests/redundancy/bonus/number_tests.c`, lines 62-64

### Purpose:
Verify that a percent without a sign before "cheaper" is a number to set (Req. 12).

### Assertion expectation:
"Siege Workshop units 15% cheaper" holds one number to set, whose text is "15".

## Redundancy test 115

### File residence:
`tests/redundancy/bonus/number_tests.c`, lines 65-67

### Purpose:
Verify that a cut of 100%, which no count changes, is not a number to set (Req. 12).

### Assertion expectation:
"Farms cost -100% wood" holds no number to set.

## Redundancy test 116

### File residence:
`tests/redundancy/bonus/number_tests.c`, lines 68-70

### Purpose:
Verify that a percent of 0%, which no count changes, is not a number to set (Req. 12).

### Assertion expectation:
"Cavalry +0% hit points" holds no number to set.

## Redundancy test 117

### File residence:
`tests/redundancy/bonus/number_tests.c`, lines 71-73

### Purpose:
Verify that an amount of 0, which no count changes, is not a number to set (Req. 12).

### Assertion expectation:
"Villagers carry +0 food" holds no number to set.

## Redundancy test 118

### File residence:
`tests/redundancy/bonus/number_tests.c`, lines 74-75

### Purpose:
Verify that a power of 1, which no count changes, is not a number to set (Req. 12).

### Assertion expectation:
"Monks heal 1x as fast" holds no number to set.

## Redundancy test 119

### File residence:
`tests/redundancy/bonus/number_tests.c`, lines 76-77

### Purpose:
Verify that a row records no more numbers to set than it has room for (Req. 12).

### Assertion expectation:
Seventeen times "+1", then "food", hold 16 numbers to set, the most a row has room for.

## Redundancy test 120

### File residence:
`tests/redundancy/bonus/number_tests.c`, lines 78-80

### Purpose:
Verify that a percent without a sign before another word is kept as it is, not a number to set (Req. 12).

### Assertion expectation:
"Loot 25% of the resource cost of each destroyed building" holds no number to set.

## Redundancy test 121

### File residence:
`tests/redundancy/present/edit_tests.c`, lines 92-93

### Purpose:
Verify that unticking a node unticks the descendants that the catalog lists before it (Req. 12).

### Assertion expectation:
Of a unit's three descendants in a catalog of five nodes out of order, none is ticked after the unit is unticked.

## Redundancy test 122

### File residence:
`tests/redundancy/present/edit_tests.c`, lines 94-95

### Purpose:
Verify that a civ bonus that is not ticked counts no times (Req. 12).

### Assertion expectation:
`civ_design_multiplier` of a civ bonus that a reset design lacks is 0.

## Redundancy test 123

### File residence:
`tests/redundancy/present/edit_tests.c`, lines 96-98

### Purpose:
Verify that the report counts no multiplier that the build carried out in full (Req. 12).

### Assertion expectation:
`multipliers_skipped` is 0 with a bonus of civ techs counted twice and a bonus of a generic tech counted once.

## Redundancy test 124

### File residence:
`tests/redundancy/present/edit_tests.c`, lines 99-101

### Purpose:
Verify that ticking a node ticks the needs that the catalog lists before it (Req. 12).

### Assertion expectation:
Of a unit's three needs in a catalog of five nodes out of order, all three are ticked after the unit is ticked.

## Redundancy test 125

### File residence:
`tests/redundancy/present/edit_tests.c`, lines 102-103

### Purpose:
Verify that ticking a node whose needs do not all fit the design's tree changes nothing (Req. 12).

### Assertion expectation:
With 400 units ticked, the command that ticks one more and its needs is not applied: `command_apply` is false.

## Redundancy test 126

### File residence:
`tests/redundancy/present/number_edit_tests.c`, lines 273-275

### Purpose:
Verify that a value typed for a number gives the command that sets the count that shows it (Req. 12).

### Assertion expectation:
75 typed for "Town Centers cost -50% wood" sets its count to 2, 2000000 millionths.

## Redundancy test 127

### File residence:
`tests/redundancy/present/number_edit_tests.c`, lines 276-278

### Purpose:
Verify that a value typed with the sign and the percent of its number is taken (Req. 12).

### Assertion expectation:
"-75%" typed for "Town Centers cost -50% wood" sets its count to 2.

## Redundancy test 128

### File residence:
`tests/redundancy/present/number_edit_tests.c`, lines 279-280

### Purpose:
Verify that the most value of a number, its value at the count 10, can be typed (Req. 12).

### Assertion expectation:
99.9 typed for "Town Centers cost -50% wood" is taken, and the count is above 9.

## Redundancy test 129

### File residence:
`tests/redundancy/present/number_edit_tests.c`, lines 281-283

### Purpose:
Verify that the value of a number, with its unit, is written as its civ bonus shows it (Req. 12).

### Assertion expectation:
For "Cavalry +20% hit points" counted 1.5 times, the value with its unit reads "+31.5%".

## Redundancy test 130

### File residence:
`tests/redundancy/present/number_edit_tests.c`, lines 284-286

### Purpose:
Verify that the range of a number names its values at the count 1 and at the count 10 (Req. 12).

### Assertion expectation:
For "Town Centers cost -50% wood", the range reads "Slide from -50% to -99.9%".

## Redundancy test 131

### File residence:
`tests/redundancy/present/number_edit_tests.c`, lines 287-289

### Purpose:
Verify that the slider stands at the count of the civ bonus in focus, in hundredths (Req. 12).

### Assertion expectation:
For a civ bonus counted 1.5 times, the slider stands at 150.

## Redundancy test 132

### File residence:
`tests/redundancy/present/number_edit_tests.c`, lines 290-291

### Purpose:
Verify that the slider gives the command that sets the count it stands at (Req. 12).

### Assertion expectation:
The slider at 250 sets the count of "Cavalry +20% hit points" to 2.5, 2500000 millionths.

## Redundancy test 133

### File residence:
`tests/redundancy/present/number_edit_tests.c`, lines 292-294

### Purpose:
Verify that the field of a percent holds its sign and digits, its percent left beside it (Req. 12).

### Assertion expectation:
For "Cavalry +20% hit points" counted 1.5 times, the text of the field reads "+31.5".

## Redundancy test 134

### File residence:
`tests/redundancy/present/number_edit_tests.c`, lines 303-305

### Purpose:
Verify that the field of an amount holds its sign and digits (Req. 12).

### Assertion expectation:
For "Foot archers +1 range" counted 1.5 times, the text of the field reads "+1.5".

## Redundancy test 135

### File residence:
`tests/redundancy/present/number_edit_tests.c`, lines 306-307

### Purpose:
Verify that a row of Chosen bonuses wraps at the word that does not fit on its line (Req. 12).

### Assertion expectation:
Cut to 12 letters a line, "Town Centers cost -50% wood" starts its second line with "cost".

## Redundancy test 136

### File residence:
`tests/redundancy/present/number_edit_tests.c`, lines 308-311

### Purpose:
Verify that a number to set is cut as one piece of its sign and digits, without its unit (Req. 12).

### Assertion expectation:
In "Town Centers cost -50% wood", piece 6 is number 0 and three letters long: "-50".

## Redundancy test 137

### File residence:
`tests/redundancy/present/number_edit_tests.c`, lines 312-314

### Purpose:
Verify that the piece of a number to set is found among the pieces of its row (Req. 12).

### Assertion expectation:
In "+1/+2 food", number 1, "+2", is piece 2.

## Redundancy test 138

### File residence:
`tests/redundancy/present/number_edit_tests.c`, lines 315-316

### Purpose:
Verify that a row is shown only for a chosen civ bonus (Req. 12).

### Assertion expectation:
With one civ bonus ticked, the row of bonus 1, past the last, is not shown.

## Redundancy test 139

### File residence:
`tests/redundancy/present/number_edit_tests.c`, lines 317-318

### Purpose:
Verify that the least value typed for a number makes its civ bonus count once again (Req. 12).

### Assertion expectation:
50 typed for "Town Centers cost -50% wood" counted twice sets its count to 1, 1000000 millionths.

## Redundancy test 140

### File residence:
`tests/redundancy/present/number_edit_tests.c`, lines 319-320

### Purpose:
Verify that a number past the last of its row has no value to show (Req. 12).

### Assertion expectation:
For "Cavalry +20% hit points", number 1 has no value with its unit.

## Redundancy test 141

### File residence:
`tests/redundancy/present/number_edit_tests.c`, lines 321-322

### Purpose:
Verify that the row of a civ bonus missing from the catalog holds no number to set (Req. 12).

### Assertion expectation:
The row of bonus 99, which the catalog lacks, holds 0 numbers to set.

## Redundancy test 142

### File residence:
`tests/redundancy/present/number_edit_tests.c`, lines 331-333

### Purpose:
Verify that a number past the last of a row has no piece in it (Req. 12).

### Assertion expectation:
In "+1/+2 food", number 2 has no piece: `NUMBER_NONE`.

## Redundancy test 143

### File residence:
`tests/redundancy/present/number_edit_tests.c`, lines 334-336

### Purpose:
Verify that the card of the slider is centered under the field of its number (Req. 12).

### Assertion expectation:
Under a field centered at 200 in 400, a card 260 wide starts at 70.

## Redundancy test 144

### File residence:
`tests/redundancy/present/number_edit_tests.c`, lines 337-339

### Purpose:
Verify that the card under a field near the left edge of Chosen bonuses stays inside it (Req. 12).

### Assertion expectation:
Under a field centered at 10 in 400, the card starts at 0.

## Redundancy test 145

### File residence:
`tests/redundancy/present/number_edit_tests.c`, lines 340-342

### Purpose:
Verify that the card under a field near the right edge of Chosen bonuses stays inside it (Req. 12).

### Assertion expectation:
Under a field centered at 390 in 400, a card 260 wide starts at 140.

## Redundancy test 146

### File residence:
`tests/redundancy/present/number_edit_tests.c`, lines 343-344

### Purpose:
Verify that a value typed with letters after it sets nothing (Req. 12).

### Assertion expectation:
"75abc" typed for "Town Centers cost -50% wood" gives no command.

## Redundancy test 147

### File residence:
`tests/redundancy/present/number_edit_tests.c`, lines 345-346

### Purpose:
Verify that a value typed past the most of its number sets nothing (Req. 12).

### Assertion expectation:
100 typed for "Town Centers cost -50% wood", whose most is 99.9, gives no command.

## Redundancy test 148

### File residence:
`tests/redundancy/present/number_edit_tests.c`, lines 347-349

### Purpose:
Verify that each place of the slider stands for a count in hundredths (Req. 12).

### Assertion expectation:
The slider at 250 stands for the count 2.5.

## Redundancy test 149

### File residence:
`tests/redundancy/present/number_edit_tests.c`, lines 350-352

### Purpose:
Verify that the slider shrinks when Chosen bonuses is too narrow for the whole card (Req. 12).

### Assertion expectation:
In 200, with ends 40 wide and gaps of 5, the slider is 100 wide, not 160.

## Redundancy test 150

### File residence:
`tests/redundancy/present/number_edit_tests.c`, lines 361-363

### Purpose:
Verify that a row of Chosen bonuses is cut into its words, spaces, numbers and units (Req. 12).

### Assertion expectation:
"Town Centers cost -50% wood" is cut into 10 pieces.

## Redundancy test 151

### File residence:
`tests/redundancy/present/number_edit_tests.c`, lines 364-366

### Purpose:
Verify that a row of more pieces than its layout holds is cut where the room ends (Req. 12).

### Assertion expectation:
100 times "a " is cut into 192 pieces, the room of a layout.

## Redundancy test 152

### File residence:
`tests/redundancy/present/number_edit_tests.c`, lines 367-369

### Purpose:
Verify that a word made of several pieces, as "+1/+2", wraps as one (Req. 12).

### Assertion expectation:
Cut to 6 letters a line, "aaaa +1/+2" starts its second line with "+1".

## Redundancy test 153

### File residence:
`tests/redundancy/present/number_edit_tests.c`, lines 370-372

### Purpose:
Verify that a word that would pass the end of its line wraps to the next (Req. 12).

### Assertion expectation:
Cut to 10 letters a line, "Town Centers" starts its second line with "Centers".

## Redundancy test 154

### File residence:
`tests/redundancy/present/number_edit_tests.c`, lines 373-375

### Purpose:
Verify that the first word of a row stays on the first line however wide it is (Req. 12).

### Assertion expectation:
Cut to 3 letters a line, "Centers" stays on line 0.

## Redundancy test 155

### File residence:
`tests/redundancy/present/number_edit_tests.c`, lines 376-378

### Purpose:
Verify that a row counts the lines it takes (Req. 12).

### Assertion expectation:
Cut to 12 letters a line, "Town Centers cost -50% wood" takes 3 lines.

## Redundancy test 156

### File residence:
`tests/redundancy/present/number_edit_tests.c`, lines 379-381

### Purpose:
Verify that the slider stands after the least value, with a gap on each side of it (Req. 12).

### Assertion expectation:
On a card that starts at 70, with a least value 40 wide and gaps of 5, the slider starts at 120.

## Redundancy test 157

### File residence:
`tests/redundancy/present/number_edit_tests.c`, lines 382-384

### Purpose:
Verify that a word of several pieces too wide for its line is not broken where its pieces meet (Req. 12).

### Assertion expectation:
Cut to 4 letters a line, "+1/+2" keeps "/", its second piece, on line 0.

## Redundancy test 158

### File residence:
`tests/redundancy/present/number_edit_tests.c`, lines 393-395

### Purpose:
Verify that the last word of a row is measured without the pieces of the row laid out before it (Req. 12).

### Assertion expectation:
After "aaa a/+1/b", "+2 b" cut to 4 letters a line keeps "b" on line 0.

## Redundancy test 159

### File residence:
`tests/redundancy/present/number_edit_tests.c`, lines 396-398

### Purpose:
Verify that a row ending in a space takes no line for the pieces of the row laid out before it (Req. 12).

### Assertion expectation:
After "aaa a/+1/b", "+2 b " cut to 4 letters a line takes 1 line.

## Redundancy test 160

### File residence:
`tests/redundancy/present/number_edit_tests.c`, lines 399-400

### Purpose:
Verify that a row of Chosen bonuses is the text of its civ bonus as it counts (Req. 12).

### Assertion expectation:
"Town Centers cost -50% wood" counted twice is the row "Town Centers cost -75% wood".

## Redundancy test 161

### File residence:
`tests/redundancy/present/number_edit_tests.c`, lines 401-403

### Purpose:
Verify that the range of an amount names its values at the count 1 and at the count 10, with no unit (Req. 12).

### Assertion expectation:
For "Foot archers +1 range", the range reads "Slide from +1 to +10".

## Redundancy test 162

### File residence:
`tests/redundancy/present/number_edit_tests.c`, lines 404-407

### Purpose:
Verify that a number to set without a sign is cut as one piece of its digits, without its unit (Req. 12).

### Assertion expectation:
In "Shepherds work 25% faster", piece 4 is number 0 and two letters long: "25".

## Redundancy test 163

### File residence:
`tests/redundancy/present/number_edit_tests.c`, lines 408-410

### Purpose:
Verify that the piece of a number that starts its row is found as the first piece (Req. 12).

### Assertion expectation:
In "+1/+2 food", number 0, "+1", is piece 0.

## Redundancy test 164

### File residence:
`tests/redundancy/present/number_edit_tests.c`, lines 411-413

### Purpose:
Verify that a row laid out after a longer one finds no piece of the longer row's numbers (Req. 12).

### Assertion expectation:
After "Town Centers cost -50% wood", "Town Centers cost " has no piece of number 0: `NUMBER_NONE`.

## Redundancy test 165

### File residence:
`tests/redundancy/present/number_edit_tests.c`, lines 414-416

### Purpose:
Verify that the slider of a card whose ends do not fit its window has no width, never less (Req. 12).

### Assertion expectation:
In 80, with ends 40 wide and gaps of 5, the slider is 0 wide.

## Redundancy test 166

### File residence:
`tests/redundancy/present/number_edit_tests.c`, lines 417-418

### Purpose:
Verify that the panel of the team bonus shows its row scaled by its multiplier (Req. 12).

### Assertion expectation:
"Knights +2 LOS" taken twice reads "Knights +4 LOS" in row 0 of the panel of the team bonus.

## Redundancy test 167

### File residence:
`tests/redundancy/present/number_edit_tests.c`, lines 419-421

### Purpose:
Verify that a value typed for the team bonus becomes the command that sets its multiplier (Req. 12).

### Assertion expectation:
"4" typed for "Knights +2 LOS" gives `COMMAND_SET_TEAM_MULTIPLIER` of bonus 7 with 2 000 000.

## Redundancy test 168

### File residence:
`tests/redundancy/present/number_edit_tests.c`, lines 422-423

### Purpose:
Verify that the panel of the team bonus is empty while the design has none (Req. 12).

### Assertion expectation:
A design without a team bonus gives 0 rows for `NUMBER_PANEL_TEAM`.

## Redundancy test 169

### File residence:
`tests/redundancy/present/number_edit_tests.c`, lines 424-425

### Purpose:
Verify that the mod's description scales the team bonus by its multiplier (Req. 12).

### Assertion expectation:
The description of a civ with "Knights +2 LOS" taken twice holds "Knights +4 LOS".

## Redundancy test 170

### File residence:
`tests/redundancy/present/number_edit_tests.c`, lines 426-427

### Purpose:
Verify that the summary of Finish scales the team bonus by its multiplier (Req. 12).

### Assertion expectation:
The summary of a civ with "Knights +2 LOS" taken twice holds "Team bonus: Knights +4 LOS".

## Redundancy test 171

### File residence:
`tests/redundancy/unique/stat_tests.c`, lines 308-309

### Purpose:
Verify that the command to set a stat of the unique unit sets it in the design (Req. 5).

### Assertion expectation:
After the Longbowman, unit 8, is picked, the command to set its hit points to 100 sets them, at 100.

## Redundancy test 172

### File residence:
`tests/redundancy/unique/stat_tests.c`, lines 310-311

### Purpose:
Verify that undo takes back a stat set of the unique unit (Req. 5).

### Assertion expectation:
After the Longbowman's hit points are set to 100 and undone, undo succeeds and no hit points are set.

## Redundancy test 173

### File residence:
`tests/redundancy/unique/stat_tests.c`, lines 312-313

### Purpose:
Verify that picking another unique unit leaves none of the stats set for the one before (Req. 12).

### Assertion expectation:
After the Longbowman's hit points are set to 100, picking the Mangudai, unit 11, leaves no hit points set.

## Redundancy test 174

### File residence:
`tests/redundancy/unique/stat_tests.c`, lines 314-315

### Purpose:
Prevent a stat of the unique unit from being set out of its range (Req. 12).

### Assertion expectation:
With the Longbowman picked, the command to set its hit points to 0, below their least of 1, is refused.

## Redundancy test 175

### File residence:
`tests/redundancy/unique/stat_tests.c`, lines 316-317

### Purpose:
Verify that a stat set of the unique unit is saved in the civ file and opened again (Req. 1).

### Assertion expectation:
The Longbowman's speed set to 1.10, held as 110, is 110 again after its civ file is written and read back.

## Redundancy test 176

### File residence:
`tests/redundancy/unique/stat_tests.c`, lines 318-320

### Purpose:
Verify that a civ file of the original opens with the stats of its unique unit and elite unit (Req. 1).

### Assertion expectation:
`tests/fixtures/example.civbuilder.json` opens with the unit's speed 1.1, as 110, and the elite unit's hit points 110.

## Redundancy test 177

### File residence:
`tests/redundancy/unique/stat_tests.c`, lines 321-322

### Purpose:
Verify that a civ whose unique unit has a stat set is not the civ it was before (Req. 12).

### Assertion expectation:
Setting the Longbowman's hit points to 100 makes the design differ, by `civ_design_same`, from the one before.

## Redundancy test 178

### File residence:
`tests/redundancy/unique/stat_tests.c`, lines 323-324

### Purpose:
Verify that a cost set of the unique unit is kept by saving and opening the civ file (Req. 1).

### Assertion expectation:
The Longbowman's food cost set to 45, saved and read back, is 45.

## Redundancy test 179

### File residence:
`tests/redundancy/unique/stat_tests.c`, lines 325-326

### Purpose:
Verify that picking the unique unit already picked keeps the stats set of it (Req. 12).

### Assertion expectation:
After the Longbowman's hit points are set to 100, picking the Longbowman again leaves them set.

## Redundancy test 180

### File residence:
`tests/redundancy/unique/stat_tests.c`, lines 327-328

### Purpose:
Verify that a civ whose unique unit has a cost set is not the civ it was before (Req. 12).

### Assertion expectation:
Setting the Longbowman's food cost to 45 makes the design differ, by `civ_design_same`, from the one before.

## Redundancy test 181

### File residence:
`tests/redundancy/unique/stat_tests.c`, lines 282-283

### Purpose:
Verify that the command to set a tech value of a unique tech sets it in the design (Req. 5).

### Assertion expectation:
After Yeomen, imperial tech 3, is picked, the command to set its wood cost to 600 sets it, at 600.

## Redundancy test 182

### File residence:
`tests/redundancy/unique/stat_tests.c`, lines 284-285

### Purpose:
Verify that picking another unique tech leaves none of the tech values set for the one before (Req. 12).

### Assertion expectation:
After Yeomen's wood cost is set to 600, picking Warwolf, imperial tech 4, leaves no wood cost set.

## Redundancy test 183

### File residence:
`tests/redundancy/unique/stat_tests.c`, lines 286-288

### Purpose:
Prevent a tech value of a unique tech from being set out of its range (Req. 12).

### Assertion expectation:
With Yeomen picked, the command to set its research time to 0, below its least of 1, is refused.

## Redundancy test 184

### File residence:
`tests/redundancy/unique/stat_tests.c`, lines 289-291

### Purpose:
Verify that a tech value set of a unique tech is saved in the civ file and opened again (Req. 1).

### Assertion expectation:
Yeomen's wood cost set to 600 is 600 again after its civ file is written and read back.

## Redundancy test 185

### File residence:
`tests/redundancy/unique/stat_tests.c`, lines 292-294

### Purpose:
Prevent a tech value not set from coming back set when its civ file is opened again (Req. 1).

### Assertion expectation:
With only Yeomen's wood cost set, its food cost is still not set after the civ file is written and read back.

## Redundancy test 186

### File residence:
`tests/redundancy/unique/stat_tests.c`, lines 295-296

### Purpose:
Verify that a cost of four zeros in a civ file is read as not set, so the tech keeps the game's (Req. 1).

### Assertion expectation:
A civ file of imperial tech 3 costing 0 food, 0 wood, 0 stone and 0 gold opens with tech 3 and no wood cost set.

## Redundancy test 187

### File residence:
`tests/redundancy/unique/stat_tests.c`, lines 297-299

### Purpose:
Verify that a cost of 0 set of a unique tech is kept by saving and opening the civ file (Req. 1).

### Assertion expectation:
Yeomen's food cost set to 0, saved alone in the civ file and read back, is still set.

## Redundancy test 188

### File residence:
`tests/redundancy/unique/stat_tests.c`, lines 256-257

### Purpose:
Verify that the command to tick a civ bonus as a tech effect keeps it with its unique tech (Req. 5).

### Assertion expectation:
After bonus 81 is ticked as a tech effect of Yeomen, the imperial tech has 1 tech effect, bonus 81.

## Redundancy test 189

### File residence:
`tests/redundancy/unique/stat_tests.c`, lines 258-259

### Purpose:
Verify that the command to set the multiplier of a tech effect sets it (Req. 5).

### Assertion expectation:
Bonus 81, a tech effect of Yeomen, set to count twice, has the multiplier 2000000 millionths.

## Redundancy test 190

### File residence:
`tests/redundancy/unique/stat_tests.c`, lines 260-262

### Purpose:
Verify that the tech effects of a unique tech, with their multipliers, are saved and opened again (Req. 1).

### Assertion expectation:
Written and read back, Yeomen's first tech effect is bonus 81, with the multiplier 2000000 millionths.

## Redundancy test 191

### File residence:
`tests/redundancy/unique/stat_tests.c`, lines 263-265

### Purpose:
Verify that a unique tech with tech effects is saved as the original saves its custom tech (Req. 1).

### Assertion expectation:
With bonus 81 ticked as a tech effect of Yeomen, the text of the civ file holds `"custom"`, the tech's mode.

## Redundancy test 192

### File residence:
`tests/redundancy/unique/stat_tests.c`, lines 266-267

### Purpose:
Verify that a civ whose unique tech has a tech effect is not the civ it was before (Req. 12).

### Assertion expectation:
Ticking bonus 81 as a tech effect of Yeomen makes the design differ, by `civ_design_same`, from the one before.

## Redundancy test 193

### File residence:
`tests/redundancy/unique/stat_tests.c`, lines 268-270

### Purpose:
Verify that a unique tech without tech effects is saved as the original's game tech (Req. 1).

### Assertion expectation:
The civ file of Yeomen with no tech effect holds "vanilla".

## Redundancy test 194

### File residence:
`tests/redundancy/unique/stat_tests.c`, lines 271-273

### Purpose:
Verify that a bonus ticked as a tech effect is found ticked, as Choose imperial effects... asks (Req. 12).

### Assertion expectation:
After bonus 81 is ticked as a tech effect of Yeomen, `civ_design_has_tech_effect` finds it.

## Redundancy test 195

### File residence:
`tests/redundancy/unique/stat_build_tests.c`, lines 309-310

### Purpose:
Verify that the hit points and speed set of the unique unit are written into the built unit (Req. 8).

### Assertion expectation:
Built from `STATS_CIV`, unit 6 of civ 1 of the dat file has 100 hit points and the speed 1.10.

## Redundancy test 196

### File residence:
`tests/redundancy/unique/stat_build_tests.c`, lines 311-312

### Purpose:
Verify that the attack set of the unique unit is written into the built unit, and shown (Req. 8).

### Assertion expectation:
Built from `STATS_CIV`, unit 6 of civ 1 has the attack 9 of class 4, melee, and shows the attack 9.

## Redundancy test 197

### File residence:
`tests/redundancy/unique/stat_build_tests.c`, lines 313-314

### Purpose:
Verify that the melee and pierce armors set of the unique unit are written into the built unit, and shown (Req. 8).

### Assertion expectation:
Built from `STATS_CIV`, unit 6 of civ 1 has the armor 3 of class 4 and 4 of class 3, and shows 3 and 4.

## Redundancy test 198

### File residence:
`tests/redundancy/unique/stat_build_tests.c`, lines 315-317

### Purpose:
Verify that the range and reload time set of the unique unit are written into the built unit, and shown (Req. 8).

### Assertion expectation:
Built from `STATS_CIV`, unit 6 of civ 1 has the range 2 and the reload time 2.50, and shows both.

## Redundancy test 199

### File residence:
`tests/redundancy/unique/stat_build_tests.c`, lines 318-319

### Purpose:
Verify that the costs set of the unique unit are written into the cost slots of the built unit (Req. 8).

### Assertion expectation:
Built from `STATS_CIV`, unit 6 of civ 1 costs 50 food, 20 wood and 10 stone in its three cost slots, each paid.

## Redundancy test 200

### File residence:
`tests/redundancy/unique/stat_build_tests.c`, lines 320-322

### Purpose:
Verify that the build's report counts a cost in all four resources, which three slots cannot hold (Req. 9).

### Assertion expectation:
Built from `COSTLY_CIV`, its unit set to cost 50 food, 20 wood, 10 stone and 5 gold, `unit_stats_skipped` is 1.

## Redundancy test 201

### File residence:
`tests/redundancy/unique/stat_build_tests.c`, lines 323-324

### Purpose:
Verify that the elite unit takes the change made to a stat of the unique unit, when it has none of its own (Req. 8).

### Assertion expectation:
The unit's hit points 45 set to 100, the elite unit's 60 are wanted as 115: 60 + (100 - 45).

## Redundancy test 202

### File residence:
`tests/redundancy/unique/stat_build_tests.c`, lines 325-326

### Purpose:
Verify that Build says so when a stat or cost of the unique unit could not be built (Req. 9).

### Assertion expectation:
The summary of the build of `COSTLY_CIV` holds "A changed stat or cost of the unique unit is not built."

## Redundancy test 203

### File residence:
`tests/redundancy/unique/stat_build_tests.c`, lines 327-328

### Purpose:
Verify that a build that left out a stat of the unique unit is not whole, so that a message says so (Req. 9).

### Assertion expectation:
`summary_build_is_whole` is false for a report with nothing skipped but `unit_stats_skipped` 1.

## Redundancy test 204

### File residence:
`tests/redundancy/unique/stat_build_tests.c`, lines 329-330

### Purpose:
Verify that a cost of a unique tech not set is built as the game's, beside the cost set (Req. 8).

### Assertion expectation:
Castle tech 15, its wood cost set to 300 and its gold not, has the catalog's 200 gold as its second cost.

## Redundancy test 205

### File residence:
`tests/redundancy/unique/stat_build_tests.c`, lines 331-333

### Purpose:
Verify that the research time set of a unique tech is built in place of the game's (Req. 8).

### Assertion expectation:
Castle tech 15, its research time set to 45, has the research time 45 at its first research location.

## Redundancy test 206

### File residence:
`tests/redundancy/unique/stat_build_tests.c`, lines 334-337

### Purpose:
Verify that a tech effect is built into the effect of its unique tech, scaled by its multiplier (Req. 8).

### Assertion expectation:
The last change of the effect of castle tech 15 has the amount 18: bonus 56's 9, its tech effect counted twice.

## Redundancy test 207

### File residence:
`tests/redundancy/unique/stat_build_tests.c`, lines 338-340

### Purpose:
Verify that the mod describes a unique tech with tech effects by them, as they count (Req. 8).

### Assertion expectation:
The mod's text of what the castle tech does is "Infantry +18 armor": bonus 56's "Infantry +9 armor" twice.

## Redundancy test 208

### File residence:
`tests/redundancy/unique/stat_build_tests.c`, lines 341-343

### Purpose:
Verify that a tech effect brings the changes of the game techs that its bonus names (Req. 8).

### Assertion expectation:
The effect of castle tech 15 enables unit 4 once, as game tech 10, which bonus 56 names, does.

## Redundancy test 209

### File residence:
`tests/redundancy/unique/stat_build_tests.c`, lines 264-266

### Purpose:
Verify that the elite unit's hit points are its own plus the change made to the unit's (Req. 8).

### Assertion expectation:
In the small game with an elite, unit 11 is built with 115 hit points: its 15 and the unit's change of 100.

## Redundancy test 210

### File residence:
`tests/redundancy/unique/stat_build_tests.c`, lines 267-269

### Purpose:
Verify that the elite unit's reload time is its own plus the change made to the unit's (Req. 8).

### Assertion expectation:
Unit 11 is built with the reload time 4.50: its 2.00 and the unit's change of 2.50.

## Redundancy test 211

### File residence:
`tests/redundancy/unique/stat_build_tests.c`, lines 270-273

### Purpose:
Verify that the attack set is written into the unit's attack, not into a larger attack bonus (Req. 8).

### Assertion expectation:
Unit 6, with a bonus of 22 against class 8 before its melee attack of 7, is built with the melee attack 9.

## Redundancy test 212

### File residence:
`tests/redundancy/unique/stat_build_tests.c`, lines 274-276

### Purpose:
Verify that a stat set to the least of its range is built (Req. 8).

### Assertion expectation:
Unit 6 with its hit points set to 1 is built with 1 hit point.

## Redundancy test 213

### File residence:
`tests/redundancy/unique/stat_build_tests.c`, lines 277-279

### Purpose:
Verify that a stat set to the most of its range is built (Req. 8).

### Assertion expectation:
Unit 6 with its hit points set to 32767 is built with 32767 hit points.

## Redundancy test 214

### File residence:
`tests/redundancy/unique/stat_build_tests.c`, lines 280-282

### Purpose:
Verify that setting a cost of the unique unit keeps the game's other costs and its population (Req. 8).

### Assertion expectation:
Unit 6 of 60 food, 20 gold and 1 population, with gold set to 5: slots food 60, gold 5, population 1 unpaid.

## Redundancy test 215

### File residence:
`tests/redundancy/unique/stat_build_tests.c`, lines 283-286

### Purpose:
Verify that the least cost kept, 1, set as the only cost of the unique unit, is built (Req. 8).

### Assertion expectation:
Unit 6 with only its food cost set, to 1, has food 1 in its first cost slot.

## Redundancy test 216

### File residence:
`tests/redundancy/unique/stat_build_tests.c`, lines 287-289

### Purpose:
Verify that a cost set to 0 is taken out of the cost slots of the unique unit (Req. 8).

### Assertion expectation:
Unit 6 with its gold set to 0 keeps food and population, and its third slot holds no resource, 0, unpaid.

## Redundancy test 217

### File residence:
`tests/redundancy/unique/stat_build_tests.c`, lines 290-291

### Purpose:
Verify that the least cost kept, 1, of a unique tech is built (Req. 8).

### Assertion expectation:
The castle tech of the full civ with its food cost set to 1 is built with food 1 in its first cost slot.

## Redundancy test 218

### File residence:
`tests/redundancy/unique/stat_build_tests.c`, lines 292-294

### Purpose:
Verify that a tech effect unticked is left out of what the built unique tech does (Req. 8).

### Assertion expectation:
With bonus 56 ticked and 50 ticked and unticked, the castle tech's effect has the changes of 56 alone.

## Redundancy test 219

### File residence:
`tests/redundancy/unique/stat_build_tests.c`, lines 295-297

### Purpose:
Verify that no stat counts as left out when the unique unit is not in the catalog (Req. 9).

### Assertion expectation:
The report of `BARE_CIV`, whose unit is missing from the catalog, has `unit_stats_skipped` 0.

## Redundancy test 220

### File residence:
`tests/redundancy/unique/stat_build_tests.c`, lines 298-300

### Purpose:
Verify that the text of a unique tech with tech effects keeps nothing of an earlier build (Req. 8).

### Assertion expectation:
The mod's text of what the castle tech does is "Infantry +18 armor" on the second of two builds as well.

## Redundancy test 221

### File residence:
`tests/redundancy/unique/unit_number_tests.c`, lines 280-282

### Purpose:
Verify that the first row of the number panel of the selected unit holds the unit's name (Req. 12).

### Assertion expectation:
With the Longbowman, unit 0 of the catalog, selected, row 0 reads "Longbowman".

## Redundancy test 222

### File residence:
`tests/redundancy/unique/unit_number_tests.c`, lines 283-285

### Purpose:
Verify that the cost row of the selected unit names each resource with its number (Req. 12).

### Assertion expectation:
For the Longbowman, row 1 reads "Cost: 0 food, 35 wood, 0 stone, 40 gold".

## Redundancy test 223

### File residence:
`tests/redundancy/unique/unit_number_tests.c`, lines 286-290

### Purpose:
Verify that the stats row of the selected unit names each stat with its number (Req. 12).

### Assertion expectation:
Row 2 of unit 0 reads "Hit points 35, attack 6, melee armor 0, pierce armor 0, range 5, speed 0.96, reload time 2".

## Redundancy test 224

### File residence:
`tests/redundancy/unique/unit_number_tests.c`, lines 291-292

### Purpose:
Verify that an override of the selected unit is shown in its row in place of the game's stat (Req. 12).

### Assertion expectation:
With the Longbowman's hit points set to 100, its stats row starts "Hit points 100, attack 6".

## Redundancy test 225

### File residence:
`tests/redundancy/unique/unit_number_tests.c`, lines 293-297

### Purpose:
Verify that a stat the catalog does not know is shown as a dash, not as a number (Req. 12).

### Assertion expectation:
Row 2 of unit 97, no stats: "Hit points -, attack -, melee armor -, pierce armor -, range -, speed -, reload time -".

## Redundancy test 226

### File residence:
`tests/redundancy/unique/unit_number_tests.c`, lines 298-300

### Purpose:
Verify that the field of a stat of the selected unit holds the number alone, without its name (Req. 12).

### Assertion expectation:
The field of the Longbowman's speed, number 5 of row 2, holds "0.96".

## Redundancy test 227

### File residence:
`tests/redundancy/unique/unit_number_tests.c`, lines 301-303

### Purpose:
Verify that a value typed into the field of a stat gives the command that sets that stat (Req. 12).

### Assertion expectation:
"100" typed into the field of the Longbowman's hit points, number 0 of row 2, sets them, at 100.

## Redundancy test 228

### File residence:
`tests/redundancy/unique/unit_number_tests.c`, lines 304-306

### Purpose:
Verify that a value typed into the field of a cost of the selected unit sets that cost (Req. 12).

### Assertion expectation:
"60" typed into the field of the Longbowman's wood cost, number 1 of row 1, sets it, at 60.

## Redundancy test 229

### File residence:
`tests/redundancy/unique/unit_number_tests.c`, lines 307-309

### Purpose:
Verify that a speed typed into its field is kept in hundredths, as the design holds it (Req. 12).

### Assertion expectation:
"1.5" typed into the field of the Longbowman's speed sets it to 150, in hundredths.

## Redundancy test 230

### File residence:
`tests/redundancy/unique/unit_number_tests.c`, lines 310-312

### Purpose:
Verify that a value typed with a minus sets a stat below zero, as an armor may be (Req. 12).

### Assertion expectation:
" -2 " typed into the field of the Longbowman's melee armor, number 2 of row 2, sets it to -2.

## Redundancy test 231

### File residence:
`tests/redundancy/unique/unit_number_tests.c`, lines 313-314

### Purpose:
Prevent a value typed past the range of a stat from giving a command (Req. 12).

### Assertion expectation:
"0" typed for the Longbowman's hit points, whose least is 1, gives no command: its kind is `COMMAND_KIND_COUNT`.

## Redundancy test 232

### File residence:
`tests/redundancy/unique/unit_number_tests.c`, lines 315-316

### Purpose:
Verify that the range of a stat is named by its least and most values (Req. 12).

### Assertion expectation:
The range of the Longbowman's speed reads "Type a value from 0.01 to 9.99".

## Redundancy test 233

### File residence:
`tests/redundancy/unique/unit_number_tests.c`, lines 317-318

### Purpose:
Verify that the field of a stat is named after the stat (Req. 12).

### Assertion expectation:
The field of number 1 of the Longbowman's stats row is named "Attack".

## Redundancy test 234

### File residence:
`tests/redundancy/unique/unit_number_tests.c`, lines 319-320

### Purpose:
Verify that with no unit selected the number panel holds one row, which says so (Req. 12).

### Assertion expectation:
With no unit picked, row 0 reads "No unit is selected yet." and the panel has 1 row.

## Redundancy test 235

### File residence:
`tests/redundancy/unique/unit_number_tests.c`, lines 321-323

### Purpose:
Verify that the number panel says so when the selected unit is not in the catalog (Req. 12).

### Assertion expectation:
With unit 99, which the catalog lacks, picked, row 0 reads "The selected unit is not in the catalog."

## Redundancy test 236

### File residence:
`tests/redundancy/unique/unit_number_tests.c`, lines 324-326

### Purpose:
Verify that the numbers of the selected unit have no slider, as no multiplier scales them (Req. 12).

### Assertion expectation:
`number_slides` is false for the Longbowman's hit points, number 0 of row 2.

## Redundancy test 237

### File residence:
`tests/redundancy/unique/unit_number_tests.c`, lines 327-329

### Purpose:
Verify that the least value of a stat's range typed into its field is taken (Req. 12).

### Assertion expectation:
"1" typed into the field of the Longbowman's hit points sets them, at 1.

## Redundancy test 238

### File residence:
`tests/redundancy/unique/unit_number_tests.c`, lines 330-332

### Purpose:
Verify that the most value of a stat's range typed into its field is taken (Req. 12).

### Assertion expectation:
"32767" typed into the field of the Longbowman's hit points sets them, at 32767.

## Redundancy test 239

### File residence:
`tests/redundancy/unique/unit_number_tests.c`, lines 249-251

### Purpose:
Verify that the second row of the number panel of a selected tech says what the tech does (Req. 12).

### Assertion expectation:
With Yeomen, imperial tech 3, selected, row 1 reads "+1 foot archer range".

## Redundancy test 240

### File residence:
`tests/redundancy/unique/unit_number_tests.c`, lines 252-254

### Purpose:
Verify that the cost row of a selected tech names each resource with its number (Req. 12).

### Assertion expectation:
For Yeomen, row 2 reads "Cost: 0 food, 750 wood, 0 stone, 450 gold".

## Redundancy test 241

### File residence:
`tests/redundancy/unique/unit_number_tests.c`, lines 255-257

### Purpose:
Verify that the research time row of a selected tech says the time in seconds (Req. 12).

### Assertion expectation:
For Yeomen, row 3 reads "Research time: 60 seconds".

## Redundancy test 242

### File residence:
`tests/redundancy/unique/unit_number_tests.c`, lines 258-259

### Purpose:
Verify that a value typed into the field of a cost of a tech sets that tech value (Req. 12).

### Assertion expectation:
"600" typed into the field of Yeomen's wood cost, number 1 of row 2, sets it, at 600.

## Redundancy test 243

### File residence:
`tests/redundancy/unique/unit_number_tests.c`, lines 260-261

### Purpose:
Verify that the field of the research time of a tech is named after it (Req. 12).

### Assertion expectation:
The field of Yeomen's row 3, its research time, is named "Research time".

## Redundancy test 244

### File residence:
`tests/redundancy/unique/unit_number_tests.c`, lines 262-264

### Purpose:
Verify that with no tech selected the number panel holds one row, which says so (Req. 12).

### Assertion expectation:
With no imperial tech picked, row 0 reads "No tech is selected yet." and the panel has 1 row.

## Redundancy test 245

### File residence:
`tests/redundancy/unique/unit_number_tests.c`, lines 265-266

### Purpose:
Verify that the range told for a cost of a tech is that of a cost, not of the research time (Req. 12).

### Assertion expectation:
The range of Yeomen's wood cost reads "Type a value from 0 to 9999".

## Redundancy test 246

### File residence:
`tests/redundancy/unique/unit_number_tests.c`, lines 267-269

### Purpose:
Verify that a tech with a tech effect drops the row of what its game tech does (Req. 12).

### Assertion expectation:
With bonus 81 ticked for Yeomen, row 1 starts "Cost: ", not "+1 foot archer range".

## Redundancy test 247

### File residence:
`tests/redundancy/unique/unit_number_tests.c`, lines 270-271

### Purpose:
Verify that the sheet of a selected tech starts with the tech's name (Req. 12).

### Assertion expectation:
Row 0 of the panel of the castle tech, with Yeomen selected, reads "Yeomen".

## Redundancy test 248

### File residence:
`tests/redundancy/unique/unit_number_tests.c`, lines 230-232

### Purpose:
Verify that the tech effects of a selected tech take the place of the row of what its game tech does (Req. 12).

### Assertion expectation:
With bonus 81 ticked for Yeomen, row 3, after its name, cost and time, reads "Villagers carry +3 food".

## Redundancy test 249

### File residence:
`tests/redundancy/unique/unit_number_tests.c`, lines 233-234

### Purpose:
Verify that the number of a tech effect has a slider, as its multiplier scales it (Req. 12).

### Assertion expectation:
`number_slides` is true for row 3 of Yeomen, its tech effect "Villagers carry +3 food".

## Redundancy test 250

### File residence:
`tests/redundancy/unique/unit_number_tests.c`, lines 235-237

### Purpose:
Verify that a value typed into the number of a tech effect sets its multiplier (Req. 12).

### Assertion expectation:
"6" typed for "Villagers carry +3 food" sets the tech effect's multiplier to 2, 2000000 millionths.

## Redundancy test 251

### File residence:
`tests/redundancy/unique/unit_number_tests.c`, lines 238-240

### Purpose:
Verify that the field of a tech effect's number is named by its row, for a screen reader (Req. 12).

### Assertion expectation:
The field of bonus 81's number, ticked for Yeomen, is named "Villagers carry +3 food".

## Redundancy test 252

### File residence:
`tests/redundancy/dat/game_number_tests.c`, lines 180-182

### Purpose:
Verify that a unique unit's cost in the catalog is read from the game's data (Req. 12).

### Assertion expectation:
The food of the catalog's unit made by tech 4 becomes 60, the food of unit 6 in the test game.

## Redundancy test 253

### File residence:
`tests/redundancy/dat/game_number_tests.c`, lines 183-185

### Purpose:
Verify that a resource absent from a unit's costs in the game is read as 0 (Req. 12).

### Assertion expectation:
The wood of the catalog's unit, 7777 before, is 0 after reading the test game.

## Redundancy test 254

### File residence:
`tests/redundancy/dat/game_number_tests.c`, lines 186-188

### Purpose:
Verify that a stat is read as the game holds it, not as it shows it (Req. 12).

### Assertion expectation:
The attack of the catalog's unit becomes 7, the melee attack of unit 6 in the test game.

## Redundancy test 255

### File residence:
`tests/redundancy/dat/game_number_tests.c`, lines 189-191

### Purpose:
Verify that a unit absent from the game's data keeps what the catalog says (Req. 12).

### Assertion expectation:
With its make tech 5, whose unit 9 the test game lacks, the unit's hit points stay 7777.

## Redundancy test 256

### File residence:
`tests/redundancy/dat/game_number_tests.c`, lines 192-194

### Purpose:
Verify that only the game's own unique units are read from the game's data (Req. 12).

### Assertion expectation:
With the role `UNIT_ROLE_CUSTOM`, the unit's hit points stay 7777.

## Redundancy test 257

### File residence:
`tests/redundancy/dat/game_number_tests.c`, lines 195-197

### Purpose:
Verify that a unique tech's cost in the catalog is read from its game tech (Req. 12).

### Assertion expectation:
The wood of the castle tech, 7777 before, is 150, the cost of the tech added to the test game.

## Redundancy test 258

### File residence:
`tests/redundancy/dat/game_number_tests.c`, lines 198-200

### Purpose:
Verify that a unique tech's research time is read from its game tech (Req. 12).

### Assertion expectation:
The time of the castle tech, 7777 before, is 25, the research time of tech 2 in the test game.

## Redundancy test 259

### File residence:
`tests/redundancy/dat/game_number_tests.c`, lines 201-203

### Purpose:
Verify that a tech absent from the game's data keeps what the catalog says (Req. 12).

### Assertion expectation:
With game tech 99, which the test game lacks, the imperial tech's time stays 7777.

## Redundancy test 260

### File residence:
`tests/redundancy/dat/game_number_tests.c`, lines 204-206

### Purpose:
Verify that a tech the catalog copies from no game tech keeps its numbers (Req. 12).

### Assertion expectation:
With an empty list of game techs, the imperial tech's gold stays 7777.

## Redundancy test 261

### File residence:
`tests/redundancy/dat/game_number_tests.c`, lines 207-209

### Purpose:
Verify that `forge_read_game` unpacks the data file of a game folder (Req. 12).

### Assertion expectation:
With the test game's folder, the castle tech's time becomes 25.

## Redundancy test 262

### File residence:
`tests/redundancy/dat/game_number_tests.c`, lines 210-212

### Purpose:
Verify that a folder that holds no game leaves the catalog as it was (Req. 12).

### Assertion expectation:
With `build\no_game`, `forge_read_game` fails and the castle tech's time stays 7777.

## Redundancy test 263

### File residence:
`tests/redundancy/dat/game_number_tests.c`, lines 213-214

### Purpose:
Verify that the range is read from the field the game shows, not the one it shoots by (Req. 12).

### Assertion expectation:
With 9 held and 5 shown for unit 6, the range of the catalog's unit is 5.

## Redundancy test 264

### File residence:
`tests/redundancy/dat/game_number_tests.c`, lines 215-217

### Purpose:
Verify that a node available in a civ's tech tree file is marked for that civ (Req. 12).

### Assertion expectation:
Unit 9, of no civ before, is of slot 1 after `BRITONS.json` of the test game is read.

## Redundancy test 265

### File residence:
`tests/redundancy/dat/game_number_tests.c`, lines 218-220

### Purpose:
Verify that a node absent from a civ's tech tree file is taken from that civ (Req. 12).

### Assertion expectation:
Unit 4, of every civ before, is no longer of slot 1 after the test game is read.

## Redundancy test 266

### File residence:
`tests/redundancy/dat/game_number_tests.c`, lines 221-223

### Purpose:
Verify that a node with the status NotAvailable is taken from that civ (Req. 12).

### Assertion expectation:
With unit 9 written as "NotAvailable" in `BRITONS.json`, it is no longer of slot 1.

## Redundancy test 267

### File residence:
`tests/redundancy/dat/game_number_tests.c`, lines 224-226

### Purpose:
Verify that a civ the game's list of civs lacks keeps its nodes (Req. 12).

### Assertion expectation:
Unit 9 stays of slot 2, which `civilizations.json` of the test game does not list.

## Redundancy test 268

### File residence:
`tests/redundancy/dat/game_number_tests.c`, lines 227-229

### Purpose:
Verify that a resource absent from a tech's costs in the game is read as 0 (Req. 12).

### Assertion expectation:
The gold of the castle tech, 7777 before, is 0 after the tech that costs only wood is read.

## Redundancy test 269

### File residence:
`tests/redundancy/dat/game_number_tests.c`, lines 230-232

### Purpose:
Verify that the read of the game's data starts at the catalog's first unit (Req. 12).

### Assertion expectation:
With the unit of the test game put first in the catalog, its hit points are no longer 7777.

## Redundancy test 270

### File residence:
`tests/redundancy/review_tests.c`, lines 93-104

### Purpose:
Verify that the window opens with the Britons as the civ to edit (Req. 12).

### Assertion expectation:
The row chosen in the civ to edit reads "Britons" when the window opens.

## Redundancy test 271

### File residence:
`tests/redundancy/review_tests.c`, lines 106-111

### Purpose:
Verify that the Name field holds the name of the civ to edit when the window opens (Req. 12).

### Assertion expectation:
The Name field reads "Britons" when the window opens.

## Redundancy test 272

### File residence:
`tests/redundancy/review_tests.c`, lines 113-121

### Purpose:
Verify that choosing another civ to edit and back keeps the edits made before (Req. 12).

### Assertion expectation:
"Testonia" typed for the Britons is back in the Name field after the Franks and the Britons are chosen.

## Redundancy test 273

### File residence:
`tests/redundancy/review_tests.c`, lines 123-126

### Purpose:
Verify that Finish says that no civ is changed at first (Req. 12).

### Assertion expectation:
The label at the top right of Finish reads "0 civs changed".

## Redundancy test 274

### File residence:
`tests/redundancy/review_tests.c`, lines 128-134

### Purpose:
Verify that Finish counts a civ once it is changed (Req. 12).

### Assertion expectation:
After "Testonia" is typed, the label of Finish reads "1 civ changed".

## Redundancy test 275

### File residence:
`tests/redundancy/review_tests.c`, lines 136-143

### Purpose:
Verify that Finish no longer counts a civ changed back to the game's (Req. 12).

### Assertion expectation:
After "Testonia" and then "Britons" are typed, the label of Finish reads "0 civs changed".

## Redundancy test 276

### File residence:
`tests/redundancy/review_tests.c`, lines 145-156

### Purpose:
Verify that the text of Finish, which cannot be typed in, shows no blinking caret in focus (Req. 12).

### Assertion expectation:
With the text of Finish in focus, the thread's caret is not shown.

## Redundancy test 277

### File residence:
`tests/redundancy/review_tests.c`, lines 171-177

### Purpose:
Verify that Build with no civ changed says so (Req. 9, 12).

### Assertion expectation:
Build in a new window gives a message that holds "No civ is changed yet".

## Redundancy test 278

### File residence:
`tests/redundancy/review_tests.c`, lines 187-194

### Purpose:
Verify that the mod's name is civeditor_mod until another is given (Req. 7, 12).

### Assertion expectation:
A saved civ built with the name question answered as offered is built as "civeditor_mod-data".

## Redundancy test 279

### File residence:
`tests/redundancy/review_tests.c`, lines 250-258

### Purpose:
Verify that the filter of the bonuses leaves the bonuses that hold its text and no other (Req. 10, 12).

### Assertion expectation:
After "food" is typed in the filter, the list holds as many rows as held "food" before, and each holds it.

## Redundancy test 280

### File residence:
`tests/redundancy/review_tests.c`, lines 260-273

### Purpose:
Verify that each category of bonuses lists its own bonuses, so that together they list each once (Req. 10, 12).

### Assertion expectation:
The rows of the categories, chosen one by one, add up to the rows of all bonuses, with two categories or more.

## Redundancy test 281

### File residence:
`tests/redundancy/review_tests.c`, lines 196-203

### Purpose:
Verify that Build mod with an empty mod name builds no mod (Req. 7, 12).

### Assertion expectation:
With a whole civ changed and an empty mod name given, the status line reads "No mod was built."

## Redundancy test 282

### File residence:
`tests/redundancy/review_tests.c`, lines 217-220

### Purpose:
Verify that the civ file of a civ with a long name is named after the first 48 letters of it (Req. 12).

### Assertion expectation:
A civ named with 60 letters is saved, by Save civ file..., as its first 48 letters and `.civeditor.json`.

## Redundancy test 283

### File residence:
`tests/redundancy/review_tests.c`, lines 158-169

### Purpose:
Verify that a field that can be typed in shows its caret in focus, unlike the text of Finish (Req. 12).

### Assertion expectation:
With the Name field in focus, the thread's caret is shown blinking.

## Redundancy test 284

### File residence:
`tests/redundancy/review_tests.c`, lines 275-280

### Purpose:
Verify that the window opens with the first control of the first step in focus (Req. 12).

### Assertion expectation:
Right after the window opens, the focus is on Civ to edit.

## Redundancy test 285

### File residence:
`tests/redundancy/review_tests.c`, lines 282-297

### Purpose:
Verify that the menu Go lists every step by its number and its title as written, "&" too (Req. 12).

### Assertion expectation:
The items after the separator read "&1 Name and tech tree\tCtrl+1" to "&4 Finish\tCtrl+4", "&3 UU&&UT\tCtrl+3" third.

## Redundancy test 286

### File residence:
`tests/redundancy/review_tests.c`, lines 299-309

### Purpose:
Verify that a wheel turn while the window is minimized does not scroll the step it shows (Req. 12).

### Assertion expectation:
At 480 by 360, minimized, turned a notch down and shown again, the window's scroll bar is at 0.

## Redundancy test 287

### File residence:
`tests/redundancy/review_tests.c`, lines 336-341

### Purpose:
Verify that Choose hero... lists every hero of the catalog, each once (Req. 12).

### Assertion expectation:
The list of Choose hero... holds as many rows as the catalog of `data/` has heroes, 30.

## Redundancy test 288

### File residence:
`tests/redundancy/review_tests.c`, lines 343-348

### Purpose:
Verify that the list of Choose hero... is labelled, so that a screen reader names it (Req. 12).

### Assertion expectation:
The text just before the list in the dialog of Choose hero... reads "Heroes".

## Redundancy test 289

### File residence:
`tests/redundancy/review_tests.c`, lines 350-355

### Purpose:
Verify that Choose hero... opens with the focus on its list of heroes (Req. 12).

### Assertion expectation:
When the dialog of Choose hero... has started, the control in focus is its list.

## Redundancy test 290

### File residence:
`tests/redundancy/review_tests.c`, lines 364-368

### Purpose:
Verify that a name typed in Choose hero... stays when its list takes the focus again (Req. 12).

### Assertion expectation:
With Saladin picked, "Testhero" typed and the focus moved to the field and back to the list, the name is "Testhero".

## Redundancy test 291

### File residence:
`tests/redundancy/review_tests.c`, lines 387-395

### Purpose:
Verify that the first row of Choose hero... can be picked like any other (Req. 12).

### Assertion expectation:
With Saladin picked, then the catalog's first hero, Cao Cao, the name shown is "Cao Cao".

## Redundancy test 292

### File residence:
`tests/redundancy/review_tests.c`, lines 397-400

### Purpose:
Verify that a hero's name of one letter, typed in Choose hero..., is taken (Req. 12).

### Assertion expectation:
With Saladin picked and "X" typed as his name, the name shown is "X".

## Redundancy test 293

### File residence:
`tests/redundancy/review_tests.c`, lines 222-225

### Purpose:
Verify that a civ named with one letter is saved under that letter (Req. 1, 12).

### Assertion expectation:
With the name "X" typed, Save civ file... writes `build/X.civeditor.json` anew, over a stale file.

## Redundancy test 294

### File residence:
`tests/redundancy/chosen/chosen_tests.c`, lines 351-353

### Purpose:
Verify that a value typed into the field of a number in the window sets its count as it is left (Req. 12).

### Assertion expectation:
"75" typed into the field of "Town Centers cost -50% wood" makes its label read "-75% wood".

## Redundancy test 295

### File residence:
`tests/redundancy/chosen/chosen_tests.c`, lines 354-355

### Purpose:
Verify that a value past the range of a number is refused, and the range named (Req. 12).

### Assertion expectation:
100 typed for "Town Centers cost -50% wood" makes the status line read "Type a value from -50% to -99.9%.".

## Redundancy test 296

### File residence:
`tests/redundancy/chosen/chosen_tests.c`, lines 356-357

### Purpose:
Verify that Enter in the field of a number sets the count that shows the value typed (Req. 12).

### Assertion expectation:
"75" and Enter in the field of "Town Centers cost -50% wood" make its label read "-75% wood".

## Redundancy test 297

### File residence:
`tests/redundancy/chosen/chosen_tests.c`, lines 358-359

### Purpose:
Verify that the field of a number holds the number itself, not a count of its bonus (Req. 12).

### Assertion expectation:
The field of "Town Centers cost -50% wood" reads "-50".

## Redundancy test 298

### File residence:
`tests/redundancy/chosen/chosen_tests.c`, lines 360-361

### Purpose:
Verify that the card of the slider is shown just below the field in focus (Req. 12).

### Assertion expectation:
With the focus on "-50", the card is shown below the field, less than its height under it, and across it.

## Redundancy test 299

### File residence:
`tests/redundancy/chosen/chosen_tests.c`, lines 362-363

### Purpose:
Verify that the slider, while dragged, shows in the field the value it would set (Req. 12).

### Assertion expectation:
The slider dragged to 200 on "Town Centers cost -50% wood" makes its field read "-75".

## Redundancy test 300

### File residence:
`tests/redundancy/chosen/chosen_tests.c`, lines 364-365

### Purpose:
Verify that the card of the slider is hidden once the focus leaves the fields of numbers (Req. 12).

### Assertion expectation:
With the focus moved from "-50" to the filter of the bonuses, the card has no `WS_VISIBLE`.

## Redundancy test 301

### File residence:
`tests/redundancy/chosen/chosen_tests.c`, lines 366-367

### Purpose:
Verify that Esc hides the card of the slider and leaves the focus on its field (Req. 12).

### Assertion expectation:
After Esc in the field "-50", the card has no `WS_VISIBLE`, and the field keeps the focus.

## Redundancy test 302

### File residence:
`tests/redundancy/chosen/chosen_tests.c`, lines 368-369

### Purpose:
Verify that a chosen bonus is shown as a label that holds its fields, not as a field (Req. 12).

### Assertion expectation:
The window of "Town Centers cost -50% wood" in Chosen bonuses is of the class "Static".

## Redundancy test 303

### File residence:
`tests/redundancy/chosen/chosen_tests.c`, lines 370-371

### Purpose:
Verify that Tab goes from the field in focus to its slider (Req. 12).

### Assertion expectation:
Tab in the field "-50" of "Town Centers cost -50% wood" puts the focus on the slider.

## Redundancy test 304

### File residence:
`tests/redundancy/chosen/chosen_tests.c`, lines 372-373

### Purpose:
Verify that the field of the number in focus has the focus frame around it (Req. 12).

### Assertion expectation:
Printed, the label of "Town Centers cost -50% wood" has the colour of the frame 3 pixels left of the field.

## Redundancy test 305

### File residence:
`tests/redundancy/chosen/chosen_tests.c`, lines 374-375

### Purpose:
Verify that the card shows the least value of the number in focus at the left end of its slider (Req. 10).

### Assertion expectation:
Printed, the card of "-50" of "Town Centers cost -50% wood" has marks of text left of its slider.

## Redundancy test 306

### File residence:
`tests/redundancy/chosen/chosen_tests.c`, lines 376-377

### Purpose:
Verify that the card shows the most value of the number in focus at the right end of its slider (Req. 10).

### Assertion expectation:
Printed, the card of "-50" of "Town Centers cost -50% wood" has marks of text right of its slider.

## Redundancy test 307

### File residence:
`tests/redundancy/chosen/chosen_tests.c`, lines 378-380

### Purpose:
Verify that the focus leaves a field whose number a change takes away, for the list of civ bonuses (Req. 12).

### Assertion expectation:
With the focus on the field "-50" of "Town Centers cost -50% wood", Clear leaves the focus on the list of civ bonuses.

## Redundancy test 308

### File residence:
`tests/redundancy/chosen/chosen_tests.c`, lines 382-382

### Purpose:
Verify that a field whose number a change takes away sets nothing as it loses the focus (Req. 12).

### Assertion expectation:
After "100" and Enter in the field "-50", then Clear, the status line still reads "Type a value from -50% to -99.9%.".

## Redundancy test 309

### File residence:
`tests/redundancy/chosen/chosen_tests.c`, lines 384-384

### Purpose:
Verify that the field of a number stands in its label just after the text before it (Req. 12).

### Assertion expectation:
The field of "-50" of "Town Centers cost -50% wood" starts the inset and the width of "Town Centers cost " in.

## Redundancy test 310

### File residence:
`tests/redundancy/chosen/chosen_tests.c`, lines 386-387

### Purpose:
Verify that the field of a number holds the whole of a number wider than either end of its range (Req. 12).

### Assertion expectation:
With "5.5" typed for "+1" of "Foot archers (except skirmishers) +1 range", "+5.5" fits the text area of its field.

## Redundancy test 311

### File residence:
`tests/redundancy/chosen/chosen_tests.c`, lines 389-390

### Purpose:
Verify that the frame of the number in focus is drawn whole, inside its label (Req. 12).

### Assertion expectation:
Printed, the label of "Shepherds work 25% faster" has the colour of the frame 3 pixels below the middle of the field.

## Redundancy test 312

### File residence:
`tests/redundancy/chosen/chosen_tests.c`, lines 392-393

### Purpose:
Verify that the frame of a number on a later line of its label is drawn around that number (Req. 12).

### Assertion expectation:
Printed, the label of the foot archers has the colour of the frame 3 pixels left of "+2", on its third line.

## Redundancy test 313

### File residence:
`tests/redundancy/chosen/chosen_tests.c`, lines 395-396

### Purpose:
Verify that the field of a number is just as tall as its text needs, and at least 24 pixels high (Req. 12).

### Assertion expectation:
The field of "-50" of "Town Centers cost -50% wood" is as high as its font + 4 pixels, and at least 24 at 96 dpi.

## Redundancy test 314

### File residence:
`tests/redundancy/chosen/unit_panel_tests.c`, lines 101-102

### Purpose:
Verify that a value typed into a field of the selected unit in the window sets its stat as it is left (Req. 12).

### Assertion expectation:
"100" typed into the hit points field of the first unit listed, then left, makes a row start "Hit points 100,".

## Redundancy test 315

### File residence:
`tests/redundancy/chosen/unit_panel_tests.c`, lines 103-105

### Purpose:
Verify that a value past the range of a stat is refused in the window, and the range named (Req. 12).

### Assertion expectation:
"0" typed into the hit points field and left makes the status line read "Type a value from 1 to 32767."

## Redundancy test 316

### File residence:
`tests/redundancy/chosen/unit_panel_tests.c`, lines 106-107

### Purpose:
Verify that the field of a stat of the selected unit, in focus, shows no card of a slider (Req. 12).

### Assertion expectation:
With the focus on the hit points field of the first unit listed, the panel has no card, or one without `WS_VISIBLE`.

## Redundancy test 317

### File residence:
`tests/redundancy/chosen/unit_panel_tests.c`, lines 108-110

### Purpose:
Verify that a value typed into a field of the selected imperial tech in the window sets it as it is left (Req. 12).

### Assertion expectation:
"600" typed into the food field of the first imperial tech listed, then left, makes a row start "Cost: 600 food,".

## Redundancy test 318

### File residence:
`tests/redundancy/chosen/unit_panel_tests.c`, lines 111-113

### Purpose:
Verify that a civ bonus ticked as a tech effect in the window is shown in the castle tech's number panel (Req. 12).

### Assertion expectation:
"Shepherds work 25% faster" ticked with Choose castle effects... is a row of "What the castle tech does".

## Redundancy test 319

### File residence:
`tests/functional/core_tests.c`, lines 6-13

### Purpose:
Verify that a failed check tells where it failed and what, so a fault can be found (Rule 26, P10 rule 5).

### Assertion expectation:
After `CHECK(width > 5)` and `CHECK(width > 4)` fail, the last failure is the file and `:11: check failed: width > 4`.

## Redundancy test 320

### File residence:
`tests/functional/civ_file_tests.c`, lines 16-19

### Purpose:
Verify that a civ file of the original opens, with its name (Req. 1).

### Assertion expectation:
`tests/fixtures/example.civbuilder.json` loads, and the design's name is "Vanguards".

## Redundancy test 321

### File residence:
`tests/functional/civ_file_tests.c`, lines 21-25

### Purpose:
Verify that a civ file of the original opens with its bonuses (Req. 1).

### Assertion expectation:
The example civ has exactly two bonuses, 9 and 29.

## Redundancy test 322

### File residence:
`tests/functional/civ_file_tests.c`, lines 27-31

### Purpose:
Verify that a civ file of the original opens with its unique unit (Req. 1).

### Assertion expectation:
The example civ's unique unit is the custom unit with index 5.

## Redundancy test 323

### File residence:
`tests/functional/civ_file_tests.c`, lines 91-96

### Purpose:
Verify that saving and opening a civ loses nothing (Req. 1).

### Assertion expectation:
A design written as JSON and read back keeps its texts, quotes and line breaks included, and is written as before.

## Redundancy test 324

### File residence:
`tests/functional/civ_file_tests.c`, lines 98-104

### Purpose:
Verify that a civ file keeps how many times each civ bonus counts, as the original's files do (Req. 8).

### Assertion expectation:
A design whose bonus 17 counts 10 times is written, then read back with a `multiplier` of 10.

## Redundancy test 325

### File residence:
`tests/functional/model_tests.c`, lines 21-28

### Purpose:
Verify that a name typed letter by letter is undone as one change, not a letter at a time (Req. 5).

### Assertion expectation:
After "V", "Vi" and "Vik" are typed as the name, one undo leaves the name empty.

## Redundancy test 326

### File residence:
`tests/functional/model_tests.c`, lines 30-38

### Purpose:
Verify that redo makes again a change that was undone (Req. 5).

### Assertion expectation:
After unit 1263 is picked, undone and redone, redo reports success and the unique unit is 1263 again.

## Redundancy test 327

### File residence:
`tests/functional/model_tests.c`, lines 40-52

### Purpose:
Verify that Next leads through the steps in their order and stays on Finish (Req. 3).

### Assertion expectation:
From the first step, each Next gives the step after it, and Next on Finish gives Finish.

## Redundancy test 328

### File residence:
`tests/functional/model_tests.c`, lines 64-67

### Purpose:
Verify that Finish names what the original's steps would not let pass yet (Req. 3).

### Assertion expectation:
For a civ with only a name, the summary reads "Still needed before a mod can be built: a unique unit; a castle tech;".

## Redundancy test 329

### File residence:
`tests/pack/pack_tests.c`, lines 44-49

### Purpose:
Verify that a stream with its own Huffman codes inflates, as the game's dat file is packed (Req. 8).

### Assertion expectation:
The 91 bytes gzip made of a 119-character text inflate to exactly that text.

## Redundancy test 330

### File residence:
`tests/pack/pack_tests.c`, lines 51-56

### Purpose:
Verify that a stream with the fixed Huffman codes of RFC 1951 inflates (Req. 8).

### Assertion expectation:
The 7 bytes `cb 48 cd c9 c9 07 00` inflate to "hello".

## Redundancy test 331

### File residence:
`tests/pack/pack_tests.c`, lines 58-76

### Purpose:
Verify that a dat file packed for a mod in stored blocks reads back unchanged (Req. 8).

### Assertion expectation:
140 000 bytes, packed by `deflate_store` in three blocks and inflated again, are the same 140 000 bytes.

## Redundancy test 332

### File residence:
`tests/pack/pack_tests.c`, lines 119-134

### Purpose:
Verify that a dat file compressed for a mod reads back unchanged (Req. 8).

### Assertion expectation:
140 000 bytes of copied runs and random bytes, packed by `deflate_pack` and inflated again, are the same 140 000 bytes.

## Redundancy test 333

### File residence:
`tests/pack/pack_tests.c`, lines 136-146

### Purpose:
Verify that a mod's dat file is compressed, as the original compresses it, not only stored (Req. 8).

### Assertion expectation:
140 000 bytes that repeat every 251 bytes are packed by `deflate_pack` into fewer than 14 000 bytes.

## Redundancy test 334

### File residence:
`tests/dat/dat_tests.c`, lines 66-74

### Purpose:
Verify that a mod's dat file keeps the game's data wherever nothing is edited (Req. 8).

### Assertion expectation:
A minimal valid dat file of 82 678 bytes, mapped and written with an empty edit, is the same 82 678 bytes.

## Redundancy test 335

### File residence:
`tests/dat/dat_tests.c`, lines 76-91

### Purpose:
Verify that an effect added for the civ is written as the game holds an effect (Req. 8).

### Assertion expectation:
Loom's effect with its 3 changes, added to an empty dat file, is written as the 43 bytes the game's own file holds.

## Redundancy test 336

### File residence:
`tests/dat/dat_tests.c`, lines 93-102

### Purpose:
Verify that a tech added for the civ is written as the game holds a tech (Req. 8).

### Assertion expectation:
The tech Loom, added to an empty dat file, is written as the 75 bytes the game's own file holds.

## Redundancy test 337

### File residence:
`tests/dat/dat_tests.c`, lines 131-149

### Purpose:
Verify that a civ bonus that the catalog makes from changes, not techs, is built for the civ (Req. 8).

### Assertion expectation:
Two changes of bonus 55 make one tech of civ 1 that needs tech 102 and fires a new effect with the 2 changes.

## Redundancy test 338

### File residence:
`tests/dat/dat_tests.c`, lines 151-167

### Purpose:
Verify that a civ bonus made of changes and taken twice counts twice in the game (Req. 8).

### Assertion expectation:
Bonus 55 taken twice is built with its +1 pierce armor made +2: the change's `d` is 770.

## Redundancy test 339

### File residence:
`tests/mod/mod_tests.c`, lines 36-44

### Purpose:
Verify that the civ's name replaces the name of the civ it replaces in the UI mod's strings (Req. 8).

### Assertion expectation:
For "Testonia" in the slot of the Britons, the strings start with the line `10271 "Testonia"`.

## Redundancy test 340

### File residence:
`tests/mod/mod_tests.c`, lines 46-60

### Purpose:
Verify that no mod is built from a civ Finish still finds unfinished (Req. 9).

### Assertion expectation:
A civ with a name but no unique unit or techs is refused with a problem that says it is "not finished".

## Redundancy test 341

### File residence:
`tests/mod/mod_tests.c`, lines 74-81

### Purpose:
Verify that a unique tech taken from another civ works on the civ's own unique unit (Req. 8).

### Assertion expectation:
A change of +1 range for Throwing Axeman 281, the other civ's unique unit, is aimed at the civ's Longbowman 8.

## Redundancy test 342

### File residence:
`tests/mod/mod_tests.c`, lines 132-141

### Purpose:
Verify that the name given to a unique tech is the name the game shows for it (Req. 8).

### Assertion expectation:
A castle tech named "Testing Axes" in the slot of the Britons gives the line `44008 "Testing Axes"` in the strings.

## Redundancy test 343

### File residence:
`tests/mod/mod_tests.c`, lines 147-153

### Purpose:
Verify that the name given to a unique unit of the game is the name the game shows for it (Req. 8).

### Assertion expectation:
The Longbowman named "Testbowman" in the slot of the Britons gives the line `69802 "Testbowman"` in the strings.

## Redundancy test 344

### File residence:
`tests/dat/dat_tests.c`, lines 171-183

### Purpose:
Verify that the civ's buildings look as those of the civ its architecture names (Req. 8).

### Assertion expectation:
A building of graphic 101 in slot 1 is given graphic 100, that of the same building of civ 0, the architecture's civ.

## Redundancy test 345

### File residence:
`tests/dat/dat_tests.c`, lines 187-201

### Purpose:
Verify that the civ's monks look as those of the monk skin chosen for it (Req. 8).

### Assertion expectation:
A monk, unit 125, of graphic 101 in slot 1 is given graphic 100, that of the monk of civ 0, whose skin is chosen.

## Redundancy test 346

### File residence:
`tests/dat/dat_tests.c`, lines 213-233

### Purpose:
Verify that the hero chosen for the civ is trained at the Castle, as in the original (Req. 8).

### Assertion expectation:
Unit 5 of slot 1, trained nowhere in the small dat file, is given building 82, the Castle, as its train location.

## Redundancy test 347

### File residence:
`tests/mod/mod_tests.c`, lines 102-105

### Purpose:
Prevent a unique tech from making two changes of one attribute of the civ's unit that differ only in amount (Req. 8).

### Assertion expectation:
`unique_same_target` says that +10 and +300 hit points for the Longbowman 8 change one target.

## Redundancy test 348

### File residence:
`tests/mod/mod_tests.c`, lines 107-110

### Purpose:
Verify that a unique tech keeps its changes of the armor of two classes, as each changes its own (Req. 8).

### Assertion expectation:
`unique_same_target` says that +5 pierce armor (class 3) and +5 melee armor (class 4) are two targets.

## Redundancy test 349

### File residence:
`tests/mod/mod_tests.c`, lines 181-184

### Purpose:
Verify that the name given to the hero is the name the game shows for it (Req. 8).

### Assertion expectation:
Liu Bei, unit 1966, named "Testhero" in the slot of the Britons gives the line `44500 "Testhero"` in the strings.

## Redundancy test 350

### File residence:
`tests/mod/mod_tests.c`, lines 216-219

### Purpose:
Verify that the civ list names the civ's own unique unit, as the original's does (Req. 8).

### Assertion expectation:
Slot 1 given the Longbowman, unit 8, takes the id 8 and the line -276 of the entry that has it; all else is kept.

## Redundancy test 351

### File residence:
`tests/mod/mod_tests.c`, lines 234-237

### Purpose:
Verify that a unique unit no civ of the game has shows its own icon and texts in the civ list (Req. 8).

### Assertion expectation:
Unit 9, absent from the list, makes slot 1's icon `007_50730.png` and its string ids 5200 and 26200; all else is kept.

## Redundancy test 352

### File residence:
`tests/dat/dat_tests.c`, lines 240-254

### Purpose:
Verify that a bonus's unit patch sets the field of the unit of its own civ (Req. 8).

### Assertion expectation:
A patch of bonus 330 sets collision size x, at +29, of unit 0 of civ 1 to 1.0; the bonus counts as built.

## Redundancy test 353

### File residence:
`tests/dat/dat_tests.c`, lines 302-308

### Purpose:
Verify that a train line sets the building where a unit of the civ is trained, as bonus 222 needs (Req. 8).

### Assertion expectation:
With unit 5 of slot 1 creatable, the two lines of bonus 222 build 2, and its first train location names building 68.

## Redundancy test 354

### File residence:
`tests/dat/dat_tests.c`, lines 310-316

### Purpose:
Verify that a train line sets the button that trains a unit of the civ, as bonus 222 needs (Req. 8).

### Assertion expectation:
With unit 5 of slot 1 creatable, the two lines of bonus 222 build 2, and its first train location has button 3.

## Redundancy test 355

### File residence:
`tests/dat/dat_tests.c`, lines 318-323

### Purpose:
Verify that a train line for a unit with no train location fails, so Build names its bonus (Req. 9).

### Assertion expectation:
With unit 5 of slot 1 a building, which has no train location, the lines of bonus 222 give BONUS_BUILD_FAILED.

## Redundancy test 356

### File residence:
`tests/dat/dat_tests.c`, lines 325-330

### Purpose:
Verify that a cost line sets a field of the costs of a unit of the civ, as bonus 460 needs (Req. 8).

### Assertion expectation:
With unit 5 of slot 1 creatable, the one cost line of bonus 460 builds 1, and its first cost has an amount of 300.

## Redundancy test 357

### File residence:
`tests/dat/dat_tests.c`, lines 332-337

### Purpose:
Verify that a cost line may set the last field of a unit's three costs, the third cost's flag (Req. 8).

### Assertion expectation:
A cost line at place 16, of 2 bytes, is built, and the flag of the third cost of the civ's unit becomes 1.

## Redundancy test 358

### File residence:
`tests/mod/mod_tests.c`, lines 245-248

### Purpose:
Prevent the list of civs from changing a value that the civ keeps from the replaced civ (Req. 8).

### Assertion expectation:
`civ_list_write` with every value `CIV_LIST_KEEP` writes the list unchanged: the description id stays 120150.

## Redundancy test 359

### File residence:
`tests/base/base_tests.c`, lines 46-50

### Purpose:
Verify that choosing another civ takes away the regional nodes of the civ before (Req. 8; order of 11-09).

### Assertion expectation:
A design given civ 1 and then civ 2 no longer has the regional unit that only civ 1 has.

## Redundancy test 360

### File residence:
`tests/base/base_tests.c`, lines 52-55

### Purpose:
Verify that a base civ gives the design the nodes its civ has in the game (Req. 8; base civ order of 2026-11-09).

### Assertion expectation:
Base civ 1 gives the design the regional unit whose list of civs holds civ 1.

## Redundancy test 361

### File residence:
`tests/base/base_tests.c`, lines 57-60

### Purpose:
Verify that a base civ gives the design its own civ bonuses and no others (Req. 8; base civ order of 2026-11-09).

### Assertion expectation:
Base civ 1, with bonus 3 alone in its list, gives the design exactly one bonus: bonus 3.

## Redundancy test 362

### File residence:
`tests/base/base_tests.c`, lines 62-65

### Purpose:
Verify that a base civ gives the design its team bonus (Req. 8; base civ order of 2026-11-09).

### Assertion expectation:
Base civ 1, whose team bonus is 0, gives the design team bonus 0.

## Redundancy test 363

### File residence:
`tests/base/base_tests.c`, lines 67-71

### Purpose:
Verify that a base civ gives the design its unique unit, as the game has it (Req. 8; base civ order of 2026-11-09).

### Assertion expectation:
Base civ 1, whose unique unit is 0, gives the design unit 0 of the game, of vanilla origin.

## Redundancy test 364

### File residence:
`tests/mod/mod_tests.c`, lines 277-280

### Purpose:
Verify that a civ with an empty tree keeps the game's tech tree screen, as its dat keeps every tech (Req. 8).

### Assertion expectation:
With nothing ticked, a Stable that the game's tree has as ResearchedCompleted is written unchanged.

## Redundancy test 365

### File residence:
`tests/mod/mod_tests.c`, lines 282-285

### Purpose:
Prevent a tech tree screen without the Villager, Town Centers, Mill and camps, which the dat never disables (Req. 8).

### Assertion expectation:
A Villager that the game's tree has as ResearchedCompleted is written unchanged for a design without it.

## Redundancy test 366

### File residence:
`tests/mod/mod_tests.c`, lines 287-290

### Purpose:
Verify that a building the civ has still needs the research the game asks for it, as the Keep does (Req. 8).

### Assertion expectation:
A Keep that the game's tree has as ResearchRequired is written unchanged for a design with a Keep.

## Redundancy test 367

### File residence:
`tests/dat/dat_tests.c`, lines 389-392

### Purpose:
Verify that the dat file disables the tech of a unit the civ lacks, and not that of a unit it has (Req. 8).

### Assertion expectation:
Of two techs that enable units 4 and 7, for a design with unit 4 only, tech 1 alone gets a change of type 102.

## Redundancy test 368

### File residence:
`tests/dat/dat_tests.c`, lines 394-397

### Purpose:
Verify that the dat file disables a tech of the tech tree the civ lacks, and not one it has (Req. 8).

### Assertion expectation:
Of techs 0 and 1, both in the catalog, for a design with tech 0 only, tech 1 alone gets a change of type 102.

## Redundancy test 369

### File residence:
`tests/dat/dat_tests.c`, lines 399-405

### Purpose:
Prevent a dat file that disables the Villager, which every civ keeps in the dat file and the tech tree file (Req. 8).

### Assertion expectation:
Of the techs that enable the Villager and unit 7, for a design with neither, only the second gets a change of type 102.

## Redundancy test 370

### File residence:
`tests/functional/functional_suite.c`, lines 247-249; `tests/mod/mod_tests.c`, lines 292-303

### Purpose:
Enforce that the tech tree screen shows a node as the game does for most civs (Requirement 8).

### Assertion expectation:
A Keep the replaced civ lacked, which most civs must research, is written `ResearchRequired` when the design has it.

## Redundancy test 371

### File residence:
`tests/dat/recipe_tests.c`, lines 41-44

### Purpose:
Verify that the tech that makes the chosen unique unit is copied for the replaced civ (Req. 8).

### Assertion expectation:
The effect of tech 14 of the built dat file enables unit 6, once.

## Redundancy test 372

### File residence:
`tests/dat/recipe_tests.c`, lines 46-49

### Purpose:
Verify that a built unique tech is researched at the Castle and asks for its age (Req. 8).

### Assertion expectation:
Tech 15, the castle tech, has research location 82, the Castle.

## Redundancy test 373

### File residence:
`tests/dat/recipe_tests.c`, lines 51-54

### Purpose:
Verify that a unique tech's change of its old civ's unique unit is aimed at the civ's own (Req. 8).

### Assertion expectation:
The effect of tech 16, the imperial tech, adds to an attribute of unit 6, once, where the game's adds to unit 5.

## Redundancy test 374

### File residence:
`tests/dat/recipe_tests.c`, lines 56-59

### Purpose:
Verify that the cost chosen for a unique tech is written in place of the catalog's (Req. 8).

### Assertion expectation:
The first cost of tech 15 has the amount 300.

## Redundancy test 375

### File residence:
`tests/dat/recipe_tests.c`, lines 61-64

### Purpose:
Verify that the civ's tech tree effect disables the tech of a node the design lacks (Req. 8).

### Assertion expectation:
The effect that civ 1's tech tree id names disables tech 5, once.

## Redundancy test 376

### File residence:
`tests/dat/recipe_tests.c`, lines 66-69

### Purpose:
Verify that the build's report counts the unique techs it built (Req. 9).

### Assertion expectation:
`unique_techs_built` is 2 for a design with both unique techs in the catalog.

## Redundancy test 377

### File residence:
`tests/dat/recipe_tests.c`, lines 71-74

### Purpose:
Verify that the civ's unique unit takes the name string id of the replaced civ's slot (Req. 8).

### Assertion expectation:
Unit 6 of civ 1 has name string id 111003 in the built dat file.

## Redundancy test 378

### File residence:
`tests/dat/recipe_tests.c`, lines 76-79

### Purpose:
Verify that a civ bonus whose tech belongs to another civ is copied for the replaced civ (Req. 8).

### Assertion expectation:
Tech 12, the copy of the bonus's tech 6, has civ 1.

## Redundancy test 379

### File residence:
`tests/dat/recipe_tests.c`, lines 81-84

### Purpose:
Verify that a tech copied for the civ does what the game's tech does, its changes counted once (Req. 8).

### Assertion expectation:
The copy of bonus tech 6, whose effect adds 5 to infantry, adds 5: the change's `d` is 5.

## Redundancy test 380

### File residence:
`tests/dat/recipe_tests.c`, lines 86-89

### Purpose:
Verify that a copied effect that changes a tech the recipe also copies changes the civ's copy of it (Req. 8).

### Assertion expectation:
The copy of bonus tech 6 lowers the cost of tech 13, the copy of tech 8, which the game's tech 6 lowers.

## Redundancy test 381

### File residence:
`tests/dat/recipe_tests.c`, lines 91-94

### Purpose:
Verify that a civ bonus whose tech belongs to no civ is unlocked in the civ's tech tree (Req. 8).

### Assertion expectation:
The civ's tech tree effect unlocks tech 10, once.

## Redundancy test 382

### File residence:
`tests/dat/recipe_tests.c`, lines 96-99

### Purpose:
Verify that the build's report counts the civ bonuses built in each of the three ways (Req. 9).

### Assertion expectation:
`bonuses_built` is 3: one of a civ's tech, one of a generic tech, one of the catalog's changes.

## Redundancy test 383

### File residence:
`tests/dat/recipe_tests.c`, lines 101-104

### Purpose:
Verify that Build says it skipped a civ bonus the catalog has but gives neither techs nor changes (Req. 9).

### Assertion expectation:
A civ with bonus 53, which the catalog lists with no techs and no changes, has 1 civ bonus skipped.

## Redundancy test 384

### File residence:
`tests/dat/recipe_tests.c`, lines 138-141

### Purpose:
Verify that Build counts each bonus taken more than once that names a tech every civ shares (Req. 9).

### Assertion expectation:
With bonus 56, which names the generic tech 10, taken twice, the report's `multipliers_skipped` is 1.

## Redundancy test 385

### File residence:
`tests/dat/recipe_tests.c`, lines 106-109

### Purpose:
Verify that a requirement tech of another civ is copied for the civ and added to the game's tech (Req. 8).

### Assertion expectation:
The tech named in the third slot of bonus tech 6's requirements, free in the game, belongs to civ 1.

## Redundancy test 386

### File residence:
`tests/dat/recipe_tests.c`, lines 111-115

### Purpose:
Verify that two techs given one requirement tech share one copy of it, as the original does for bonus 81 (Req. 8).

### Assertion expectation:
The first slot of tech 10's requirements and the third of tech 6's name the same tech.

## Redundancy test 387

### File residence:
`tests/dat/recipe_tests.c`, lines 117-120

### Purpose:
Verify that Build counts a bonus made of requirement lines as built (Req. 9).

### Assertion expectation:
The civ with bonuses 54 and 55 has 1 civ bonus built.

## Redundancy test 388

### File residence:
`tests/dat/recipe_tests.c`, lines 122-125

### Purpose:
Verify that Build says it skipped a bonus whose game tech has no free slot for a requirement (Req. 9).

### Assertion expectation:
The civ with bonus 55, whose tech 7 has all six slots of its requirements taken, has 1 civ bonus skipped.

## Redundancy test 389

### File residence:
`tests/dat/recipe_tests.c`, lines 127-131

### Purpose:
Prevent a build from adding the requirement techs of the build before it (Req. 8).

### Assertion expectation:
After a build with bonus 54, a build of the bare civ leaves the first slot of tech 10's requirements at -1.

## Redundancy test 390

### File residence:
`tests/dat/recipe_tests.c`, lines 133-136

### Purpose:
Prevent the build from giving a requirement to a tech of the game that no requirement line names (Req. 8).

### Assertion expectation:
With bonuses 54 and 55, the first slot of tech 0's requirements stays at -1.

## Redundancy test 391

### File residence:
`tests/dat/recipe_tests.c`, lines 143-146

### Purpose:
Verify that the civ's team bonus effect holds the changes of the chosen team bonus (Req. 8).

### Assertion expectation:
The last change of the effect that civ 1's team bonus id names is of attribute 5.

## Redundancy test 392

### File residence:
`tests/dat/recipe_tests.c`, lines 148-151

### Purpose:
Verify that the team bonus, which allies take too, changes the game's own techs, not the civ's copies (Req. 8).

### Assertion expectation:
The team bonus copied from effect 9 lowers the cost of tech 8, as the game's effect does, not of its copy 13.

## Redundancy test 393

### File residence:
`tests/dat/recipe_tests.c`, lines 153-156

### Purpose:
Verify that a team bonus copied from the game's effect and taken twice counts twice (Req. 8).

### Assertion expectation:
Team bonus 60 taken twice writes its effect's +1 as +2.

## Redundancy test 394

### File residence:
`tests/dat/recipe_tests.c`, lines 158-161

### Purpose:
Verify that the tech of the elite unique unit is named by the slot's string id (Req. 8).

### Assertion expectation:
Tech 13, the copy of the elite tech, has name string id 111004.

## Redundancy test 395

### File residence:
`tests/dat/recipe_tests.c`, lines 163-166

### Purpose:
Prevent a unique tech from changing the civ's unit twice when its old and new unit are both named (Req. 8).

### Assertion expectation:
The castle tech's effect has 4 changes: of 7 in the game, 2 are dropped and 1 is already made.

## Redundancy test 396

### File residence:
`tests/dat/recipe_tests.c`, lines 168-171

### Purpose:
Verify that a unique tech that enabled its old civ's unique unit enables the civ's own (Req. 8).

### Assertion expectation:
The castle tech's effect enables unit 6, once.

## Redundancy test 397

### File residence:
`tests/dat/recipe_tests.c`, lines 173-176

### Purpose:
Verify that a unique tech's upgrade of its old civ's unique unit is left out (Req. 8).

### Assertion expectation:
The castle tech's effect has no upgrade of unit 5.

## Redundancy test 398

### File residence:
`tests/dat/recipe_tests.c`, lines 178-181

### Purpose:
Verify that a unit spawned by a unique tech is the civ's elite unique unit (Req. 8).

### Assertion expectation:
The castle tech's effect spawns unit 11, once.

## Redundancy test 399

### File residence:
`tests/dat/recipe_tests.c`, lines 183-186

### Purpose:
Verify that the civ's tech tree effect lists its unlocks before its disables, as the game's do (Req. 8).

### Assertion expectation:
The last change of the tree effect is a disable.

## Redundancy test 400

### File residence:
`tests/dat/recipe_tests.c`, lines 188-191

### Purpose:
Prevent the tech tree from disabling a tech that only sets resources and has no research place (Req. 8).

### Assertion expectation:
The tree effect does not disable tech 7.

## Redundancy test 401

### File residence:
`tests/dat/recipe_tests.c`, lines 319-328

### Purpose:
Verify that the tech tree effect lists its unlocks first, then each kind of change by tech, as the game's do (Req. 8).

### Assertion expectation:
A disable of tech 30, an unlock of tech 40 and a disable of tech 20, sorted, come as the unlock, then 20, then 30.

## Redundancy test 402

### File residence:
`tests/dat/recipe_tests.c`, lines 330-336

### Purpose:
Prevent the tech tree from keeping one of two techs it disables, since a disable names its tech in `d` (Req. 8).

### Assertion expectation:
After a disable of tech 20 and one of tech 30 are added, the tree has 2 changes.

## Redundancy test 403

### File residence:
`tests/dat/recipe_tests.c`, lines 193-196

### Purpose:
Verify that the build says when it could not build the unique unit (Req. 9).

### Assertion expectation:
`unit_skipped` is true for a design whose unique unit the catalog lacks.

## Redundancy test 404

### File residence:
`tests/dat/recipe_tests.c`, lines 198-201

### Purpose:
Verify that the build says which unique techs it could not build (Req. 9).

### Assertion expectation:
`unique_techs_skipped` is 2 for a design whose two unique techs the catalog lacks.

## Redundancy test 405

### File residence:
`tests/dat/recipe_tests.c`, lines 203-206

### Purpose:
Verify that the civ's tech tree effect hides the hero until its own tech enables it (Req. 8).

### Assertion expectation:
The tree effect has one change that enables unit 6 with mode 0, off.

## Redundancy test 406

### File residence:
`tests/dat/recipe_tests.c`, lines 208-211

### Purpose:
Verify that a design without a tech tree leaves the game's techs as they are (Req. 8).

### Assertion expectation:
`tree_disabled` is 0 for the bare civ, which has no tree.

## Redundancy test 407

### File residence:
`tests/dat/recipe_tests.c`, lines 213-216

### Purpose:
Verify that a team bonus the game has no effect for is built from the catalog's changes (Req. 9).

### Assertion expectation:
`team_bonuses_built` is 1 for team bonus 61, which has one change and no effect.

## Redundancy test 408

### File residence:
`tests/dat/recipe_tests.c`, lines 233-236

### Purpose:
Verify that a team bonus made of changes and taken twice counts twice (Req. 8).

### Assertion expectation:
Team bonus 61 taken twice writes its change of +3 as +6.

## Redundancy test 409

### File residence:
`tests/dat/recipe_tests.c`, lines 238-241

### Purpose:
Verify that a civ bonus that names techs also gets its lines of changes, as bonus 356 needs (Req. 8).

### Assertion expectation:
The bare civ with bonus 56, which names tech 10 and has one line, has 1 change adding to infantry in tech 11.

## Redundancy test 410

### File residence:
`tests/dat/recipe_tests.c`, lines 243-248

### Purpose:
Verify that the lines of a bonus that names techs count as often as the bonus is taken (Req. 8).

### Assertion expectation:
Bonus 56, taken twice, adds 18 to infantry in tech 11, twice the 9 its line gives.

## Redundancy test 411

### File residence:
`tests/dat/recipe_tests.c`, lines 218-221

### Purpose:
Verify that Build says it skipped a team bonus with neither an effect nor changes (Req. 9).

### Assertion expectation:
A civ with team bonus 62, which the catalog lists with no effect and no changes, has 1 team bonus skipped.

## Redundancy test 412

### File residence:
`tests/dat/recipe_tests.c`, lines 223-226

### Purpose:
Prevent Build from saying it built a team bonus it skipped (Req. 9).

### Assertion expectation:
A civ with team bonus 62, which has nothing to build, has 0 team bonuses built.

## Redundancy test 413

### File residence:
`tests/dat/recipe_tests.c`, lines 228-231

### Purpose:
Prevent Build from saying it skipped a team bonus it built (Req. 9).

### Assertion expectation:
The full civ, whose team bonus 60 is copied from the game's effect, has 0 team bonuses skipped.

## Redundancy test 414

### File residence:
`tests/dat/recipe_tests.c`, lines 250-253

### Purpose:
Verify that a hero with a name takes the hero's string id of the replaced civ's slot (Req. 8).

### Assertion expectation:
Unit 6 of civ 1 has name string id 111005 in the built dat file.

## Redundancy test 415

### File residence:
`tests/dat/recipe_tests.c`, lines 255-258

### Purpose:
Verify that a tech copied for the civ requires the civ's copy of a tech it required (Req. 8).

### Assertion expectation:
Tech 12, the copy of tech 6, requires tech 13, the copy of tech 8, which belonged to another civ.

## Redundancy test 416

### File residence:
`tests/dat/recipe_tests.c`, lines 260-263

### Purpose:
Verify that a tech copied for the civ keeps a requirement that every civ shares (Req. 8).

### Assertion expectation:
The second required tech of tech 12 is tech 5, as in the game.

## Redundancy test 417

### File residence:
`tests/dat/recipe_tests.c`, lines 265-268

### Purpose:
Verify that the scout chosen for the civ is written into the civ's resources (Req. 8).

### Assertion expectation:
With scout 448 chosen, resource 263 of civ 1 is 448 in the built dat file.

## Redundancy test 418

### File residence:
`tests/dat/recipe_tests.c`, lines 270-273

### Purpose:
Verify that the build says so when the hero is a unit the replaced civ does not have (Req. 9).

### Assertion expectation:
With unit 4 as the hero, which civ 1 of the small game lacks, the civ is built and `hero_skipped` is true.

## Redundancy test 419

### File residence:
`tests/dat/recipe_tests.c`, lines 275-284

### Purpose:
Verify that the dat file is read past its sounds, graphics, unit tasks and tech tree nodes (Req. 8).

### Assertion expectation:
A small dat with one sound, two graphic slots, two unit headers and four tree nodes is written back byte for byte.

## Redundancy test 420

### File residence:
`tests/dat/recipe_tests.c`, lines 286-289

### Purpose:
Verify that the tech that makes the unique unit is the replaced civ's own (Req. 8).

### Assertion expectation:
Tech 14 of the built dat file, the copy of the unit's tech, has civ 1.

## Redundancy test 421

### File residence:
`tests/dat/recipe_tests.c`, lines 291-294

### Purpose:
Verify that a built unique tech can be researched only in the age of its tier (Req. 8).

### Assertion expectation:
The first required tech of tech 15, the castle tech, is 102, the Castle Age.

## Redundancy test 422

### File residence:
`tests/dat/recipe_tests.c`, lines 296-299

### Purpose:
Verify that the build's report counts the techs its tech tree disables (Req. 9).

### Assertion expectation:
`tree_disabled` is 1 for the full civ, whose tree lacks one unit of the catalog.

## Redundancy test 423

### File residence:
`tests/dat/recipe_tests.c`, lines 301-304

### Purpose:
Prevent Build from counting the techs of a unique unit it did not build (Req. 9).

### Assertion expectation:
The bare civ, whose unique unit the catalog lacks, has 0 techs of its unit built.

## Redundancy test 424

### File residence:
`tests/look/look_tests.c`, lines 56-59

### Purpose:
Verify that a ship of the civ takes the look of the civ of its architecture (Req. 8).

### Assertion expectation:
A unit of class 22 in civ 1 has the standing graphic 100 of civ 0 after the build.

## Redundancy test 425

### File residence:
`tests/look/look_tests.c`, lines 61-64

### Purpose:
Verify that a trade cart's loaded look is taken with its architecture (Req. 8).

### Assertion expectation:
The carrying graphic of the first task of a unit of class 19 in civ 1 is 100, that of civ 0.

## Redundancy test 426

### File residence:
`tests/look/look_tests.c`, lines 66-73

### Purpose:
Verify that a monk's working look is taken with its monk skin (Req. 8).

### Assertion expectation:
The proceeding graphic of the first task of unit 125 in civ 1 is 100, that of the monk skin's civ 0.

## Redundancy test 427

### File residence:
`tests/look/look_tests.c`, lines 75-81

### Purpose:
Verify that the Castle takes the look chosen for it and not that of the architecture (Req. 8).

### Assertion expectation:
With architecture civ 0 and castle civ 2, unit 82 of civ 1 has the standing graphic 102.

## Redundancy test 428

### File residence:
`tests/look/look_tests.c`, lines 83-86

### Purpose:
Prevent a unit that is neither a building nor a ship, monk, cart or king from changing its look (Req. 8).

### Assertion expectation:
A unit of class 6 in civ 1 keeps its standing graphic 101 after the build.

## Redundancy test 429

### File residence:
`tests/look/look_tests.c`, lines 88-94

### Purpose:
Verify that the architecture chosen in the design is looked up in the catalog for its civ (Req. 8).

### Assertion expectation:
For architecture 2, which the catalog gives the look of civ 0, `look_sources_of` names civ 0.

## Redundancy test 430

### File residence:
`tests/look/look_tests.c`, lines 111-114

### Purpose:
Verify that a look line of a bonus in the design dresses its unit in the look of the civ it names (Req. 8, 9).

### Assertion expectation:
The infantry unit of the civ built, a look line naming civ 0 and its bonus chosen, has the graphics of civ 0.

## Redundancy test 431

### File residence:
`tests/look/look_tests.c`, lines 116-119

### Purpose:
Prevent a look line from dressing a unit when the design lacks its bonus (Req. 8, 9).

### Assertion expectation:
The infantry unit of the civ built, a look line naming civ 0 but its bonus not chosen, keeps the graphics of civ 1.

## Redundancy test 432

### File residence:
`tests/look/look_tests.c`, lines 168-171

### Purpose:
Verify that a look line gives its unit the frame and the place its attack is let go from, with the look (Req. 8, 9).

### Assertion expectation:
The unit of the civ built, its look copied from civ 0 whose frame delay is 7, has a frame delay of 7.

## Redundancy test 433

### File residence:
`tests/look/look_tests.c`, lines 173-176

### Purpose:
Verify that a look line gives its unit the attack graphic of the unit it copies (Req. 8, 9).

### Assertion expectation:
The unit of the civ built, its look copied from civ 0 whose attack graphic is 7, has an attack graphic of 7.

## Redundancy test 434

### File residence:
`tests/look/look_tests.c`, lines 178-181

### Purpose:
Verify that a look line gives its unit the walking graphic of the unit it copies (Req. 8, 9).

### Assertion expectation:
The unit of the civ built, its look copied from civ 0 whose walking graphic is 7, has a walking graphic of 7.

## Redundancy test 435

### File residence:
`tests/look/look_tests.c`, lines 183-186

### Purpose:
Verify that a look line gives its unit the icon of the unit it copies (Req. 8, 9).

### Assertion expectation:
The unit of the civ built, its look copied from civ 0 whose icon is 7, has an icon of 7.

## Redundancy test 436

### File residence:
`tests/bonus/scale_tests.c`, lines 55-58

### Purpose:
Verify that an amount a bonus adds grows with the times the bonus counts, as the original scales it (Req. 8).

### Assertion expectation:
+15 hit points taken 3 times is one change of +45.

## Redundancy test 437

### File residence:
`tests/bonus/scale_tests.c`, lines 60-63

### Purpose:
Verify that the factor a bonus multiplies an attribute by is raised to the times it counts (Req. 8).

### Assertion expectation:
Speed times 1.5, taken twice, is speed times 2.25.

## Redundancy test 438

### File residence:
`tests/bonus/scale_tests.c`, lines 65-68

### Purpose:
Verify that the factor a bonus multiplies a resource by is raised to the times it counts (Req. 8).

### Assertion expectation:
Food times 1.5, taken twice, is food times 2.25.

## Redundancy test 439

### File residence:
`tests/bonus/scale_tests.c`, lines 70-73

### Purpose:
Verify that a tech's cost multiplied in mode 2 is raised to the times the bonus counts (Req. 8).

### Assertion expectation:
Loom's food cost times 0.5 in mode 2, taken twice, is times 0.25.

## Redundancy test 440

### File residence:
`tests/bonus/scale_tests.c`, lines 75-78

### Purpose:
Verify that a tech's time multiplied in mode 2 is raised to the times the bonus counts (Req. 8).

### Assertion expectation:
Loom's time times 0.5 in mode 2, taken twice, is times 0.25.

## Redundancy test 441

### File residence:
`tests/bonus/scale_tests.c`, lines 80-83

### Purpose:
Verify that a tech's cost changed in a mode other than 2 is kept, as the original keeps it (Req. 8).

### Assertion expectation:
Loom's food cost -10 in mode 1, taken twice, stays -10.

## Redundancy test 442

### File residence:
`tests/bonus/scale_tests.c`, lines 85-90

### Purpose:
Verify that a bonus that spawns units spawns more as it counts more times (Req. 8).

### Assertion expectation:
3 militia spawned at the barracks, taken twice, are 6: the change's `c` is 6.

## Redundancy test 443

### File residence:
`tests/bonus/scale_tests.c`, lines 92-95

### Purpose:
Verify that a resource a bonus gives grows with the times the bonus counts (Req. 8).

### Assertion expectation:
+100 food taken twice is +200.

## Redundancy test 444

### File residence:
`tests/bonus/scale_tests.c`, lines 97-100

### Purpose:
Verify that the spawn limit, resource 234, is kept, as the original keeps it (Req. 8).

### Assertion expectation:
Resource 234 +5, taken twice, stays +5.

## Redundancy test 445

### File residence:
`tests/bonus/scale_tests.c`, lines 102-105

### Purpose:
Verify that resource 33, which names a function of the game, not an amount, is kept (Req. 8).

### Assertion expectation:
Resource 33 set to 7, taken twice, stays 7.

## Redundancy test 446

### File residence:
`tests/bonus/scale_tests.c`, lines 107-110

### Purpose:
Verify that a change that sets an attribute is kept, since setting it twice sets it once (Req. 8).

### Assertion expectation:
Hit points set to 100, taken twice, stay 100.

## Redundancy test 447

### File residence:
`tests/bonus/scale_tests.c`, lines 112-116

### Purpose:
Verify that armor added past 255, the most that one change holds, is split into two changes (Req. 8).

### Assertion expectation:
Pierce armor +200 taken twice is +255 and +145: the second change's `d` is 913.

## Redundancy test 448

### File residence:
`tests/bonus/scale_tests.c`, lines 118-122

### Purpose:
Verify that armor taken away stays taken away as the bonus counts more times (Req. 8).

### Assertion expectation:
Pierce armor -2 taken twice is -4: the change's `d` is -772.

## Redundancy test 449

### File residence:
`tests/bonus/scale_tests.c`, lines 124-128

### Purpose:
Verify that an attack multiplied past 255%, the most that one change holds, is split into two (Req. 8).

### Assertion expectation:
Attack times 150% taken 3 times is 255% and 132%: the second change's `d` is 900.

## Redundancy test 450

### File residence:
`tests/bonus/scale_tests.c`, lines 130-134

### Purpose:
Verify that a percent halfway between two is rounded to the even one, as the original rounds it (Req. 8).

### Assertion expectation:
Attack times 50% taken 3 times is 12.5%, written as 12%: the change's `d` is 780.

## Redundancy test 451

### File residence:
`tests/bonus/scale_tests.c`, lines 136-140

### Purpose:
Verify that a percent past a half is rounded up, as the original rounds it (Req. 8).

### Assertion expectation:
Attack times 109% taken twice is 118.81%, written as 119%: the change's `d` is 887.

## Redundancy test 452

### File residence:
`tests/bonus/scale_tests.c`, lines 142-146

### Purpose:
Verify that a percent below a half is rounded down, odd or even, as the original rounds it (Req. 8).

### Assertion expectation:
Attack times 111% taken twice is 123.21%, written as 123%: the change's `d` is 891.

## Redundancy test 453

### File residence:
`tests/bonus/scale_tests.c`, lines 148-153

### Purpose:
Verify that armor that grows to exactly 255, the most one change holds, stays one change (Req. 8).

### Assertion expectation:
Pierce armor +85 taken 3 times is one change of +255.

## Redundancy test 454

### File residence:
`tests/bonus/scale_tests.c`, lines 162-165

### Purpose:
Verify that a bonus's text shows a cost cut as the game counts it when the bonus counts twice (Req. 8).

### Assertion expectation:
"Town Centers cost -50% wood" counted twice reads "Town Centers cost -75% wood".

## Redundancy test 455

### File residence:
`tests/bonus/scale_tests.c`, lines 167-170

### Purpose:
Verify that a bonus's text shows an amount added as often as the bonus counts (Req. 8).

### Assertion expectation:
"Infantry +1 attack" counted twice reads "Infantry +2 attack".

## Redundancy test 456

### File residence:
`tests/bonus/scale_tests.c`, lines 172-175

### Purpose:
Verify that each number of a list in a bonus's text is scaled as the bonus counts (Req. 8).

### Assertion expectation:
"Cavalry +20/30% hit points" counted twice reads "Cavalry +44/69% hit points".

## Redundancy test 457

### File residence:
`tests/bonus/scale_tests.c`, lines 177-180

### Purpose:
Verify that a factor in a bonus's text is raised to the times the bonus counts (Req. 8).

### Assertion expectation:
"Farms 2x HP" counted twice reads "Farms 4x HP".

## Redundancy test 458

### File residence:
`tests/bonus/scale_tests.c`, lines 182-185

### Purpose:
Verify that a percent scaled in a bonus's text keeps one decimal, as the game counts it (Req. 8).

### Assertion expectation:
"Monks +5% faster" counted twice reads "Monks +10.3% faster", since 1.05 squared is 1.1025.

## Redundancy test 459

### File residence:
`tests/bonus/scale_tests.c`, lines 187-190

### Purpose:
Verify that a percent without a sign before "faster" is scaled as a gain, as the bonus counts (Req. 8).

### Assertion expectation:
"Cavalry 20% faster" counted twice reads "Cavalry 44% faster".

## Redundancy test 460

### File residence:
`tests/mod/forge_tests.c`, lines 134-137

### Purpose:
Verify that a built mod has the folder `<name>-data` in the mods folder (Req. 2, 7).

### Assertion expectation:
For the civ "Testonia" and a small game in `build/test_game`, the result's data mod ends in `testonia-data`.

## Redundancy test 461

### File residence:
`tests/mod/forge_tests.c`, lines 139-142

### Purpose:
Verify that a built mod has the folder `<name>-ui` in the mods folder (Req. 2, 7).

### Assertion expectation:
For the civ "Testonia", the result's UI mod ends in `testonia-ui`.

## Redundancy test 462

### File residence:
`tests/mod/forge_tests.c`, lines 144-147

### Purpose:
Verify that the data mod's `info.json` carries the civ's name as its title (Req. 7).

### Assertion expectation:
`build/test_mods/testonia-data/info.json` holds "Testonia".

## Redundancy test 463

### File residence:
`tests/mod/forge_tests.c`, lines 149-152

### Purpose:
Verify that the UI mod's `info.json` is told apart from the data mod's by its title (Req. 7).

### Assertion expectation:
`build/test_mods/testonia-ui/info.json` holds "Testonia (UI)".

## Redundancy test 464

### File residence:
`tests/mod/forge_tests.c`, lines 154-165

### Purpose:
Verify that the data mod holds the game's dat file with the civ built in, packed as the game reads it (Req. 8).

### Assertion expectation:
The mod's `empires2_x2_p1.dat` unpacks and has 17 techs, the game's 11 and the civ's 6.

## Redundancy test 465

### File residence:
`tests/mod/forge_tests.c`, lines 167-170

### Purpose:
Verify that the data mod's `civilizations.json` names the civ's unique unit for the replaced civ (Req. 8).

### Assertion expectation:
The mod's list of civs holds `"unique_unit_id":6,` where the game's holds 5.

## Redundancy test 466

### File residence:
`tests/mod/forge_tests.c`, lines 172-175

### Purpose:
Verify that the data mod holds the tech tree file under the replaced civ's name (Req. 8).

### Assertion expectation:
`CivTechTrees/BRITONS.json` of the data mod holds `civ_techs_units`.

## Redundancy test 467

### File residence:
`tests/mod/forge_tests.c`, lines 177-180

### Purpose:
Verify that the UI mod writes the civ's name under the replaced civ's name id (Req. 8).

### Assertion expectation:
The UI mod's strings of `en` hold `10271 "Testonia"`.

## Redundancy test 468

### File residence:
`tests/mod/forge_tests.c`, lines 182-185

### Purpose:
Verify that the UI mod is written for every language of the game, to the last (Req. 7).

### Assertion expectation:
The UI mod's strings of `zh`, the fifteenth language, hold `10271 "Testonia"`.

## Redundancy test 469

### File residence:
`tests/mod/forge_tests.c`, lines 187-190

### Purpose:
Verify that Build says so when the game's list of civs cannot be read (Req. 9).

### Assertion expectation:
For the game folder `build/no_game` the build fails with a problem that names the "list of civs".

## Redundancy test 470

### File residence:
`tests/mod/forge_tests.c`, lines 192-195

### Purpose:
Verify that Build says so when the game's data file cannot be unpacked (Req. 9).

### Assertion expectation:
With 19 bytes of text as the dat file, the build fails with "read or unpacked" in its problem.

## Redundancy test 471

### File residence:
`tests/mod/forge_tests.c`, lines 197-200

### Purpose:
Verify that Build says so when the replaced civ's tech tree file is missing (Req. 9).

### Assertion expectation:
With a list of civs that names the tree `NOBODY`, the build fails with "tech tree" in its problem.

## Redundancy test 472

### File residence:
`tests/mod/forge_tests.c`, lines 202-205

### Purpose:
Verify that a civ is built without what the catalog lacks, and that the report says so (Req. 9).

### Assertion expectation:
The bare civ is built, and the result's report has `unit_skipped`.

## Redundancy test 473

### File residence:
`tests/mod/strings_tests.c`, lines 35-38

### Purpose:
Verify that a text of several lines is written to the game's strings on one line (Req. 8).

### Assertion expectation:
The tagline "North", a line break, "South" is written `"North\nSouth civilization`.

## Redundancy test 474

### File residence:
`tests/mod/strings_tests.c`, lines 40-43

### Purpose:
Prevent a line break typed in Windows from being written as two line breaks (Req. 8).

### Assertion expectation:
The tagline "North", a carriage return and a line break, "South" is written with one `\n`.

## Redundancy test 475

### File residence:
`tests/mod/strings_tests.c`, lines 45-48

### Purpose:
Prevent a quote in a text from ending its line of the game's strings early (Req. 8).

### Assertion expectation:
The tagline `The "best"` is written `"The \"best\" civilization`.

## Redundancy test 476

### File residence:
`tests/mod/strings_tests.c`, lines 50-53

### Purpose:
Prevent a quote that the user escaped from being escaped twice (Req. 8).

### Assertion expectation:
The tagline `The \"best` is written `"The \"best civilization`.

## Redundancy test 477

### File residence:
`tests/mod/strings_tests.c`, lines 55-58

### Purpose:
Verify that a unique tech's label holds its description in brackets, without its full stop (Req. 8).

### Assertion expectation:
The tech "Bulwark", "Walls are stronger.", is written `"Research Bulwark (Walls are stronger)"`.

## Redundancy test 478

### File residence:
`tests/mod/strings_tests.c`, lines 60-63

### Purpose:
Verify that a unique tech without a description gets a label of its name alone (Req. 8).

### Assertion expectation:
The imperial tech named "Zeal" is written `"Research Zeal"`.

## Redundancy test 479

### File residence:
`tests/mod/strings_tests.c`, lines 65-69

### Purpose:
Verify that the unique unit's tooltip holds the description the user gave it (Req. 8).

### Assertion expectation:
The unit "Tester", "It tests.", is written `"Create <b>Tester<b> (<cost>)\nIt tests."`.

## Redundancy test 480

### File residence:
`tests/mod/strings_tests.c`, lines 71-75

### Purpose:
Verify that the first loading tip of the civ is its first civ bonus (Req. 8).

### Assertion expectation:
`IDS_CIVTIPS_1_0` is "Civilization bonus: Infantry +1 attack.", with a full stop added.

## Redundancy test 481

### File residence:
`tests/mod/strings_tests.c`, lines 77-80

### Purpose:
Verify that a civ without civ bonuses gets its team bonus as its first tip (Req. 8).

### Assertion expectation:
For the bare civ, `IDS_CIVTIPS_1_0` is "Team bonus: Allies are faster."

## Redundancy test 482

### File residence:
`tests/mod/strings_tests.c`, lines 82-86

### Purpose:
Verify that the first tip in the game shows a civ bonus as it counts, its numbers scaled (Req. 8).

### Assertion expectation:
With bonus 50 counted twice, `IDS_CIVTIPS_1_0` is "Civilization bonus: Infantry +2 attack."

## Redundancy test 483

### File residence:
`tests/mod/strings_tests.c`, lines 88-92

### Purpose:
Verify that the third tip names the castle tech, its place and what it does (Req. 8).

### Assertion expectation:
`IDS_CIVTIPS_1_2` is "Research Bulwark in your Castle: Walls are stronger."

## Redundancy test 484

### File residence:
`tests/mod/strings_tests.c`, lines 94-97

### Purpose:
Verify that the second tip names the civ's unique unit (Req. 8).

### Assertion expectation:
`IDS_CIVTIPS_1_1` is "Your unique unit is the Tester."

## Redundancy test 485

### File residence:
`tests/mod/strings_tests.c`, lines 99-103

### Purpose:
Verify that a tip, which the game shows on one line, takes a line break of the civ's texts as a space (Req. 8).

### Assertion expectation:
With the unit named "Test", a line break and "er", `IDS_CIVTIPS_1_1` is "Your unique unit is the Test er."

## Redundancy test 486

### File residence:
`tests/places/places_tests.c`, lines 51-55

### Purpose:
Verify that the game is found when it is installed in Steam's own folder (Req. 2).

### Assertion expectation:
With a dat file under `build/test_steam`, the game folder found ends in `test_steam\steamapps\common\AoE2DE`.

## Redundancy test 487

### File residence:
`tests/places/places_tests.c`, lines 57-60

### Purpose:
Verify that the game is found in another library that Steam's list names (Req. 2).

### Assertion expectation:
With a list that names `build\\test_library`, the game folder found is in `build\test_library`.

## Redundancy test 488

### File residence:
`tests/places/places_tests.c`, lines 62-66

### Purpose:
Prevent a folder without the game's data file from being taken for the game (Req. 2).

### Assertion expectation:
With a list that names only `build\\nowhere`, no game folder is found.

## Redundancy test 489

### File residence:
`tests/places/places_tests.c`, lines 68-71

### Purpose:
Prevent a mods folder from being named where the game has no profile (Req. 2).

### Assertion expectation:
Under the home `build/no_home`, which does not exist, no mods folder is found.

## Redundancy test 490

### File residence:
`tests/core/read_tests.c`, lines 99-104

### Purpose:
Verify that a number of the catalog or a civ file is read with its sign (Req. 1).

### Assertion expectation:
`text_to_number` reads "-12" as valid and -12.

## Redundancy test 491

### File residence:
`tests/core/read_tests.c`, lines 106-109

### Purpose:
Prevent a text with a letter among its digits from being read as a number (Req. 1).

### Assertion expectation:
`text_to_number` says that "1x" is not valid.

## Redundancy test 492

### File residence:
`tests/core/read_tests.c`, lines 111-114

### Purpose:
Prevent a number too large to hold from being read as another number (Req. 1).

### Assertion expectation:
`text_to_number` says that "2147483648", one more than the largest `long`, is not valid.

## Redundancy test 493

### File residence:
`tests/core/read_tests.c`, lines 116-121

### Purpose:
Verify that the largest number a `long` holds is still read, now that a larger one is refused (Req. 1).

### Assertion expectation:
`text_to_number` reads "2147483647" as valid and equal to `LONG_MAX`.

## Redundancy test 494

### File residence:
`tests/core/read_tests.c`, lines 123-126

### Purpose:
Verify that a path or text with an accented letter reaches Windows whole (Req. 1, 2).

### Assertion expectation:
`text_widen` makes the two bytes C3 A9 the one wide letter 00E9.

## Redundancy test 495

### File residence:
`tests/core/read_tests.c`, lines 128-131

### Purpose:
Verify that a letter of three bytes in UTF-8 reaches Windows whole (Req. 1, 2).

### Assertion expectation:
`text_widen` makes the three bytes E2 82 AC the one wide letter 20AC.

## Redundancy test 496

### File residence:
`tests/core/read_tests.c`, lines 133-136

### Purpose:
Verify that a letter of four bytes in UTF-8 becomes a pair of wide letters (Req. 1, 2).

### Assertion expectation:
`text_widen` makes F0 9F 98 80 the two wide letters D83D DE00.

## Redundancy test 497

### File residence:
`tests/core/read_tests.c`, lines 138-141

### Purpose:
Verify that U+10000, the first letter that needs a pair of wide letters, is widened to a pair (Req. 1, 2).

### Assertion expectation:
`text_widen` makes F0 90 80 80 the two wide letters D800 DC00.

## Redundancy test 498

### File residence:
`tests/core/read_tests.c`, lines 143-146

### Purpose:
Prevent a text that is not UTF-8 from becoming a wrong path (Req. 1, 2).

### Assertion expectation:
`text_widen` fails on the byte FF.

## Redundancy test 499

### File residence:
`tests/core/read_tests.c`, lines 148-151

### Purpose:
Prevent a text that begins with the middle of a letter from being widened (Req. 1, 2).

### Assertion expectation:
`text_widen` fails on the byte 80.

## Redundancy test 500

### File residence:
`tests/core/read_tests.c`, lines 153-156

### Purpose:
Prevent a letter whose second byte is missing from being widened (Req. 1, 2).

### Assertion expectation:
`text_widen` fails on C3 followed by "A".

## Redundancy test 501

### File residence:
`tests/core/read_tests.c`, lines 158-161

### Purpose:
Prevent a widened text from being cut where its place ends (Req. 1, 2).

### Assertion expectation:
`text_widen` fails on "abc" for a place of 2 wide letters.

## Redundancy test 502

### File residence:
`tests/core/read_tests.c`, lines 163-169

### Purpose:
Verify that a field of the catalog that holds only a dash is read as an empty text (Req. 8).

### Assertion expectation:
`tsv_text` succeeds on "-" and leaves an empty text.

## Redundancy test 503

### File residence:
`tests/core/read_tests.c`, lines 171-177

### Purpose:
Prevent a field beyond the end of a catalog line from being read (Req. 8).

### Assertion expectation:
`tsv_text` fails for field 3 of a line of one field and leaves an empty text.

## Redundancy test 504

### File residence:
`tests/core/read_tests.c`, lines 191-194

### Purpose:
Verify that a decimal of the catalog is read in hundredths, without rounding (Req. 8).

### Assertion expectation:
`tsv_hundredths` reads "1.257" as 125.

## Redundancy test 505

### File residence:
`tests/core/read_tests.c`, lines 196-199

### Purpose:
Verify that a decimal with one digit after the point is read in hundredths (Req. 8).

### Assertion expectation:
`tsv_hundredths` reads "1.5" as 150.

## Redundancy test 506

### File residence:
`tests/core/read_tests.c`, lines 201-204

### Purpose:
Prevent a field with two points from being read as a number (Req. 8).

### Assertion expectation:
`tsv_hundredths` gives `TSV_MISSING` for "1.2.3".

## Redundancy test 507

### File residence:
`tests/core/read_tests.c`, lines 206-209

### Purpose:
Verify that a change of the catalog with a negative amount is read with its sign (Req. 9).

### Assertion expectation:
`tsv_decimal` reads "-1.5" as -1.5.

## Redundancy test 508

### File residence:
`tests/core/read_tests.c`, lines 211-214

### Purpose:
Prevent a field with a letter among its digits from being read as a decimal (Req. 8).

### Assertion expectation:
`tsv_decimal` fails on "1x".

## Redundancy test 509

### File residence:
`tests/core/read_tests.c`, lines 216-219

### Purpose:
Prevent a field with a sign and no digit from being read as zero (Req. 8).

### Assertion expectation:
`tsv_decimal` fails on "-".

## Redundancy test 510

### File residence:
`tests/core/read_tests.c`, lines 221-224

### Purpose:
Verify that a civ file's text with an escaped letter is opened as that letter (Req. 1).

### Assertion expectation:
The string `é` of a civ file is copied as the bytes C3 A9.

## Redundancy test 511

### File residence:
`tests/core/read_tests.c`, lines 226-229

### Purpose:
Verify that an escaped letter written with capital hex digits is opened (Req. 1).

### Assertion expectation:
The string `€` is copied as the bytes E2 82 AC.

## Redundancy test 512

### File residence:
`tests/core/read_tests.c`, lines 231-234

### Purpose:
Verify that an escaped pair of surrogates is opened as one letter (Req. 1).

### Assertion expectation:
The string `😀` is copied as the bytes F0 9F 98 80.

## Redundancy test 513

### File residence:
`tests/core/read_tests.c`, lines 236-239

### Purpose:
Prevent a broken escape of a civ file from being opened as some letter (Req. 1).

### Assertion expectation:
The string `\u00zz` is not copied.

## Redundancy test 514

### File residence:
`tests/core/read_tests.c`, lines 241-244

### Purpose:
Prevent half a pair of surrogates from being opened as a letter (Req. 1).

### Assertion expectation:
The string `\ud83d` followed by plain letters is not copied.

## Redundancy test 515

### File residence:
`tests/core/read_tests.c`, lines 246-249

### Purpose:
Prevent a high surrogate followed by another escape from being opened (Req. 1).

### Assertion expectation:
The string `\ud83dA` is not copied.

## Redundancy test 516

### File residence:
`tests/core/read_tests.c`, lines 251-254

### Purpose:
Verify that a description of several lines is opened with its line breaks (Req. 1).

### Assertion expectation:
The string `x\ny` is copied as "x", a new line and "y".

## Redundancy test 517

### File residence:
`tests/core/read_tests.c`, lines 256-259

### Purpose:
Verify that an escaped tab of a civ file is opened as a tab (Req. 1).

### Assertion expectation:
The string `x\ty` is copied as "x", a tab and "y".

## Redundancy test 518

### File residence:
`tests/core/read_tests.c`, lines 261-264

### Purpose:
Verify that an escaped carriage return of a civ file is opened as one (Req. 1).

### Assertion expectation:
The string `x\ry` is copied as "x", a carriage return and "y".

## Redundancy test 519

### File residence:
`tests/core/read_tests.c`, lines 266-269

### Purpose:
Verify that a quote inside a text of a civ file is opened as a quote (Req. 1).

### Assertion expectation:
The string `x\"y` is copied as "x", a quote and "y".

## Redundancy test 520

### File residence:
`tests/core/read_tests.c`, lines 271-274

### Purpose:
Prevent a text of a civ file from being cut where its place ends (Req. 1).

### Assertion expectation:
The string "abc" is not copied into a place of 2 bytes.

## Redundancy test 521

### File residence:
`tests/core/read_tests.c`, lines 276-279

### Purpose:
Prevent a file whose outermost value is not an object from being opened as a civ file (Req. 1).

### Assertion expectation:
`json_parse` fails on `[1]`, after reading its 2 tokens.

## Redundancy test 522

### File residence:
`tests/core/read_tests.c`, lines 281-284

### Purpose:
Prevent a civ file that ends before its object closes from being opened (Req. 1).

### Assertion expectation:
`json_parse` fails on `{"a":1`, after reading its 3 tokens.

## Redundancy test 523

### File residence:
`tests/core/read_tests.c`, lines 286-295

### Purpose:
Verify that the game's files are patched rightly whatever the order of the edits (Req. 8).

### Assertion expectation:
`{"a":1,"b":2}` with "b" set to 20 and then "a" to 10 is written `{"a":10,"b":20}`.

## Redundancy test 524

### File residence:
`tests/core/read_tests.c`, lines 297-300

### Purpose:
Verify that an escape is read with each small hex digit, the first and the last among them (Req. 1).

### Assertion expectation:
`"\u00af"` reads as the two bytes C2 AF.

## Redundancy test 525

### File residence:
`tests/core/read_tests.c`, lines 302-305

### Purpose:
Verify that an escape is read with the last capital hex digit (Req. 1).

### Assertion expectation:
`"\u00AF"` reads as the two bytes C2 AF.

## Redundancy test 526

### File residence:
`tests/core/read_tests.c`, lines 307-310

### Purpose:
Verify that a letter is written in two bytes from U+0080 on (Req. 1).

### Assertion expectation:
`"\u0080"` reads as the bytes C2 80.

## Redundancy test 527

### File residence:
`tests/core/read_tests.c`, lines 312-315

### Purpose:
Verify that a letter is written in three bytes from U+0800 on (Req. 1).

### Assertion expectation:
`"\u0800"` reads as the bytes E0 A0 80.

## Redundancy test 528

### File residence:
`tests/core/read_tests.c`, lines 317-320

### Purpose:
Verify that the first pair of surrogates is read as one letter of four bytes (Req. 1).

### Assertion expectation:
`"\ud800\udc00"` reads as the bytes F0 90 80 80.

## Redundancy test 529

### File residence:
`tests/core/read_tests.c`, lines 322-325

### Purpose:
Verify that the last pair of surrogates is read as one letter (Req. 1).

### Assertion expectation:
`"\udbff\udfff"` reads as the bytes F4 8F BF BF.

## Redundancy test 530

### File residence:
`tests/pack/dynamic_tests.c`, lines 47-58

### Purpose:
Verify that a dat file packed by another program, with repeated code lengths, is unpacked (Req. 8).

### Assertion expectation:
236 bytes packed by the deflater of Windows unpack to the 532 letters they were made from.

## Redundancy test 531

### File residence:
`tests/core/text_tests.c`, lines 10-13

### Purpose:
Prevent a name from being taken for a longer name that starts with it (Req. 1).

### Assertion expectation:
`text_equal` of "civ" and "civics" is false.

## Redundancy test 532

### File residence:
`tests/core/text_tests.c`, lines 15-18

### Purpose:
Prevent a name from being taken for the start of itself (Req. 1).

### Assertion expectation:
`text_equal` of "civics" and "civ" is false.

## Redundancy test 533

### File residence:
`tests/core/text_tests.c`, lines 20-23

### Purpose:
Verify that a text as long as its place allows is kept whole (Req. 1).

### Assertion expectation:
"abc" is copied into a place of 4 bytes and reads back as "abc".

## Redundancy test 534

### File residence:
`tests/core/text_tests.c`, lines 25-28

### Purpose:
Prevent a text from being written past the end of its place (Rule 26).

### Assertion expectation:
`text_copy` of "abcd" into a place of 4 bytes gives false.

## Redundancy test 535

### File residence:
`tests/core/text_tests.c`, lines 30-34

### Purpose:
Prevent an appended text from being written past the end of its place (Rule 26).

### Assertion expectation:
`text_append` of "cd" to "ab" in a place of 4 bytes gives false.

## Redundancy test 536

### File residence:
`tests/core/text_tests.c`, lines 36-41

### Purpose:
Verify that a negative number is written as one, such as a missing id of -1 (Req. 1).

### Assertion expectation:
`text_append_number` of -12 gives "-12".

## Redundancy test 537

### File residence:
`tests/core/text_tests.c`, lines 43-46

### Purpose:
Verify that the filters find a row whatever the case of its letters, Z among them (Req. 1).

### Assertion expectation:
`text_contains_folded` of "Zulu" and "z" is true.

## Redundancy test 538

### File residence:
`tests/core/text_tests.c`, lines 48-51

### Purpose:
Prevent a filter from keeping rows that do not hold what was typed (Req. 1).

### Assertion expectation:
`text_contains_folded` of "civ" and "x" is false.

## Redundancy test 539

### File residence:
`tests/core/writer_tests.c`, lines 18-24

### Purpose:
Verify that a civ file is JSON that the original can read: members are split by commas (Req. 1).

### Assertion expectation:
An object of `a` 1 and `b` 2 is written as `{`, `  "a": 1,`, `  "b": 2` and `}` on four lines.

## Redundancy test 540

### File residence:
`tests/core/writer_tests.c`, lines 26-34

### Purpose:
Prevent a comma before the first member of an object inside another (Req. 1).

### Assertion expectation:
An object `in` after `a` is written as `"in": {`, its member `b` on a line of its own, and `}`.

## Redundancy test 541

### File residence:
`tests/core/writer_tests.c`, lines 36-40

### Purpose:
Prevent an unfinished text from being saved as a civ file (Req. 1).

### Assertion expectation:
`json_writer_succeeded` is false while an object is not ended.

## Redundancy test 542

### File residence:
`tests/core/writer_tests.c`, lines 42-57

### Purpose:
Prevent a civ file from being cut short without a word (Req. 1).

### Assertion expectation:
After 133 texts of 1000 letters, more than the writer's 131072 bytes, `json_writer_succeeded` is false.

## Redundancy test 543

### File residence:
`tests/data/lines_tests.c`, lines 32-35

### Purpose:
Verify that a line of a catalog file is split into its fields (Req. 1).

### Assertion expectation:
The line `7`, `word`, `-`, `3,5,8`, `1,63`, split by tabs, has 5 fields.

## Redundancy test 544

### File residence:
`tests/data/lines_tests.c`, lines 37-40

### Purpose:
Verify that an id of a catalog file is read as a number (Req. 1).

### Assertion expectation:
The first field of that line reads as 7.

## Redundancy test 545

### File residence:
`tests/data/lines_tests.c`, lines 42-47

### Purpose:
Verify that a name of a catalog file is read as its text (Req. 1).

### Assertion expectation:
The second field of that line reads as "word".

## Redundancy test 546

### File residence:
`tests/data/lines_tests.c`, lines 49-57

### Purpose:
Verify that a list of techs or effects of a catalog file keeps its order (Req. 1).

### Assertion expectation:
The field `3,5,8` reads as 3 numbers, the first 3 and the last 8.

## Redundancy test 547

### File residence:
`tests/data/lines_tests.c`, lines 59-62

### Purpose:
Verify that the civs that have a node are read as a set, slot 63 among them (Req. 1).

### Assertion expectation:
The field `1,63` reads as bits 1 and 63.

## Redundancy test 548

### File residence:
`tests/data/lines_tests.c`, lines 64-69

### Purpose:
Verify that a file with the line ends of Windows is read line by line (Req. 1).

### Assertion expectation:
After a line ended by CR LF, the next line's first field reads as "second".

## Redundancy test 549

### File residence:
`tests/data/lines_tests.c`, lines 71-74

### Purpose:
Prevent a line from being read past the end of a file (Rule 26).

### Assertion expectation:
In a file of two lines, the third `tsv_next` gives false.

## Redundancy test 550

### File residence:
`tests/data/lines_tests.c`, lines 76-79

### Purpose:
Verify that a missing catalog file is told, not read as empty (Req. 1).

### Assertion expectation:
`tsv_open` of `build\no_such_lines.txt` gives false.

## Redundancy test 551

### File residence:
`tests/data/catalog_tests.c`, lines 22-25

### Purpose:
Verify that every line of `bonuses.txt` becomes a bonus of the catalog (Req. 1).

### Assertion expectation:
After `catalog_load` of `data`, the catalog holds 394 bonuses.

## Redundancy test 552

### File residence:
`tests/data/catalog_tests.c`, lines 27-30

### Purpose:
Verify that every line of `team_bonuses.txt` becomes a team bonus (Req. 1).

### Assertion expectation:
The catalog holds 80 team bonuses.

## Redundancy test 553

### File residence:
`tests/data/catalog_tests.c`, lines 32-35

### Purpose:
Verify that every line of `units.txt` becomes a unit (Req. 1).

### Assertion expectation:
The catalog holds 146 units.

## Redundancy test 554

### File residence:
`tests/data/catalog_tests.c`, lines 37-40

### Purpose:
Verify that every line of `techs.txt` becomes a unique tech (Req. 1).

### Assertion expectation:
The catalog holds 120 techs.

## Redundancy test 555

### File residence:
`tests/data/catalog_tests.c`, lines 42-45

### Purpose:
Verify that every line of `tree.txt` becomes a node (Req. 1).

### Assertion expectation:
The catalog holds 205 nodes.

## Redundancy test 556

### File residence:
`tests/data/catalog_tests.c`, lines 47-50

### Purpose:
Verify that the catalog reads every requirement line of `bonus_effects.txt` (Req. 8).

### Assertion expectation:
The catalog holds 15 requirement lines.

## Redundancy test 557

### File residence:
`tests/data/catalog_tests.c`, lines 52-57

### Purpose:
Verify that a requirement line is read whole: its bonus, the game's tech and the tech it requires (Req. 8).

### Assertion expectation:
The fourth requirement line is bonus 103, tech 209, the Cavalier, and tech 768.

## Redundancy test 558

### File residence:
`tests/data/catalog_tests.c`, lines 59-65

### Purpose:
Verify that the catalog reads a train line of its file as a unit patch in the train location (Req. 8).

### Assertion expectation:
Unit patch 173 is placed in the train location: bonus 222, unit 705, place 2, value 68.

## Redundancy test 559

### File residence:
`tests/data/catalog_tests.c`, lines 67-73

### Purpose:
Verify that the catalog reads a cost line of its file as a unit patch in the costs (Req. 8).

### Assertion expectation:
Unit patch 178 is placed in the costs: bonus 460, unit 82, place 2, value 300.

## Redundancy test 560

### File residence:
`tests/data/catalog_tests.c`, lines 75-80

### Purpose:
Verify that the catalog reads a look line of its file: its bonus, its unit and the civ of the look (Req. 8).

### Assertion expectation:
The first look line is of bonus 457, unit 42, civ 47.

## Redundancy test 561

### File residence:
`tests/data/catalog_tests.c`, lines 82-85

### Purpose:
Verify that a unit's name is read from the third field of its line (Req. 1).

### Assertion expectation:
The first unit of the catalog is named "Longbowman".

## Redundancy test 562

### File residence:
`tests/data/catalog_tests.c`, lines 87-90

### Purpose:
Verify that a unit's speed, a decimal in the file, is kept exactly (Req. 1).

### Assertion expectation:
The speed of the first unit, 0.96 in `units.txt`, is 96.

## Redundancy test 563

### File residence:
`tests/data/catalog_tests.c`, lines 92-95

### Purpose:
Verify that the letter V of a line of `units.txt` makes a unit of the game, not a hero (Req. 1).

### Assertion expectation:
The role of the first unit of the catalog is `UNIT_ROLE_VANILLA`.

## Redundancy test 564

### File residence:
`tests/data/catalog_tests.c`, lines 97-100

### Purpose:
Verify that a missing `data` folder is told, so that the window can say so (Req. 1).

### Assertion expectation:
`catalog_load` of `build\no_data` gives false.

## Redundancy test 565

### File residence:
`tests/model/present_tests.c`, lines 44-47

### Purpose:
Verify that the command a pick in the list of units gives changes the design (Req. 5).

### Assertion expectation:
After the command to pick unit 8, the design's unique unit is 8.

## Redundancy test 566

### File residence:
`tests/model/present_tests.c`, lines 49-52

### Purpose:
Verify that the command to pick a unique tech changes the design (Req. 5).

### Assertion expectation:
After the command to pick imperial tech 12, the design's imperial tech is 12.

## Redundancy test 567

### File residence:
`tests/model/present_tests.c`, lines 54-57

### Purpose:
Verify that the command to pick a hero changes the design (Req. 5).

### Assertion expectation:
After the command to pick hero 6, the design's hero is unit 6.

## Redundancy test 568

### File residence:
`tests/model/present_tests.c`, lines 59-62

### Purpose:
Verify that the command to tick a civ bonus changes the design (Req. 5, 6).

### Assertion expectation:
After the command to tick civ bonus 5, the design has it.

## Redundancy test 569

### File residence:
`tests/model/present_tests.c`, lines 64-67

### Purpose:
Verify that the command to tick a team bonus changes the design (Req. 5).

### Assertion expectation:
After the command to tick team bonus 5, the design has it.

## Redundancy test 570

### File residence:
`tests/model/present_tests.c`, lines 69-79

### Purpose:
Verify that the command to tick a node of the tech tree changes the design (Req. 5).

### Assertion expectation:
After the command made from a tree with building 12 ticked, the design's tree has it.

## Redundancy test 571

### File residence:
`tests/model/present_tests.c`, lines 81-84

### Purpose:
Verify that the command to set a look changes the design (Req. 5).

### Assertion expectation:
After the command to set the wonder to 12, the design's wonder is 12.

## Redundancy test 572

### File residence:
`tests/model/present_tests.c`, lines 86-92

### Purpose:
Prevent a command without a kind from changing the design (Req. 5).

### Assertion expectation:
`command_apply` refuses `command_nothing()`, and the design still has no hero.

## Redundancy test 573

### File residence:
`tests/model/present_tests.c`, lines 94-100

### Purpose:
Verify that ticking a civ bonus again takes it out and keeps the others (Req. 6).

### Assertion expectation:
With bonuses 5 and 9 ticked and 5 ticked again, the design lacks 5 and has 9.

## Redundancy test 574

### File residence:
`tests/model/present_tests.c`, lines 102-107

### Purpose:
Verify that ticking the team bonus again takes it out (Req. 8).

### Assertion expectation:
With team bonus 5 ticked twice, the design has no team bonus 5.

## Redundancy test 575

### File residence:
`tests/model/present_tests.c`, lines 109-117

### Purpose:
Verify that a row of the tech tree list stands for its node of the catalog (Req. 8).

### Assertion expectation:
The command of the first row of the tree list, applied to an empty design, ticks unit 9.

## Redundancy test 576

### File residence:
`tests/model/present_tests.c`, lines 119-122

### Purpose:
Verify that a row of the civ bonus list stands for its bonus of the catalog (Req. 6).

### Assertion expectation:
The second row of the bonus list gives the command to tick civ bonus 51.

## Redundancy test 577

### File residence:
`tests/model/present_tests.c`, lines 124-127

### Purpose:
Verify that a row of the team bonus list stands for its bonus of the catalog (Req. 8).

### Assertion expectation:
The second row of the team bonus list gives the command to tick team bonus 61.

## Redundancy test 578

### File residence:
`tests/model/present_tests.c`, lines 129-133

### Purpose:
Verify that a row of a choice of looks stands for its entry of the catalog (Req. 8).

### Assertion expectation:
The second entry, shown for Architecture, gives the command to set the architecture to 2.

## Redundancy test 579

### File residence:
`tests/model/tree_tests.c`, lines 55-58

### Purpose:
Verify that unticking a node leaves ticked the nodes of its building that do not need it (Req. 8).

### Assertion expectation:
For the Britons, unticking the Archer (4) leaves the Skirmisher (7) ticked.

## Redundancy test 580

### File residence:
`tests/model/tree_tests.c`, lines 60-63

### Purpose:
Verify that unticking a tech unticks the techs that need it (Req. 8).

### Assertion expectation:
For the Britons, unticking Iron Casting (68) unticks Blast Furnace (75).

## Redundancy test 581

### File residence:
`tests/model/tree_tests.c`, lines 65-68

### Purpose:
Verify that unticking a building unticks the building that needs it, though shown in another column (Req. 8).

### Assertion expectation:
For the Britons, unticking the Outpost (598) unticks the Watch Tower (79).

## Redundancy test 582

### File residence:
`tests/model/tree_tests.c`, lines 70-73

### Purpose:
Verify that unticking a building unticks the units trained in it (Req. 8).

### Assertion expectation:
For the Britons, unticking the Archery Range (87) unticks the Archer (4).

## Redundancy test 583

### File residence:
`tests/model/tree_tests.c`, lines 75-78

### Purpose:
Verify that a node every civ keeps is unticked alone, without the nodes that need it (Req. 8).

### Assertion expectation:
For the Britons, unticking the Mill (68), which every civ keeps, leaves the Farm (50) ticked.

## Redundancy test 584

### File residence:
`tests/model/present_tests.c`, lines 260-263

### Purpose:
Verify that the summary of Finish shows each civ bonus's text as the bonus counts, its numbers scaled (Req. 8).

### Assertion expectation:
With bonus 50 counted twice, the summary holds "- Infantry +2 attack".

## Redundancy test 585

### File residence:
`tests/model/present_tests.c`, lines 135-138

### Purpose:
Verify that a row of the list of units stands for its unit of the catalog (Req. 8).

### Assertion expectation:
The second row of the unit list gives the command to pick the game's unit 8.

## Redundancy test 586

### File residence:
`tests/model/present_tests.c`, lines 140-143

### Purpose:
Verify that a row of the heroes of Choose hero... stands for its hero of the catalog (Req. 8).

### Assertion expectation:
The row of the catalog's third unit, a hero, gives the command to pick hero 6.

## Redundancy test 587

### File residence:
`tests/model/present_tests.c`, lines 145-150

### Purpose:
Verify that unticking the Hero box takes the hero out of the design (Req. 8).

### Assertion expectation:
Unticking the Hero box gives the command to pick hero -1, no choice.

## Redundancy test 588

### File residence:
`tests/model/present_tests.c`, lines 152-155

### Purpose:
Verify that a row of the castle techs stands for its tech, of the castle tier (Req. 8).

### Assertion expectation:
The first row gives the command to pick tech 0 for the castle tier.

## Redundancy test 589

### File residence:
`tests/model/present_tests.c`, lines 157-160

### Purpose:
Verify that a row of the imperial techs stands for its tech, of the imperial tier (Req. 8).

### Assertion expectation:
The second row gives the command to pick tech 0 for the imperial tier.

## Redundancy test 590

### File residence:
`tests/model/present_tests.c`, lines 162-167

### Purpose:
Verify that a row of the castle techs shows the name of its tech, not of a unit (Req. 8).

### Assertion expectation:
The first row of the castle techs reads "Bulwark", the name of the catalog's first tech.

## Redundancy test 591

### File residence:
`tests/model/present_tests.c`, lines 169-172

### Purpose:
Verify that Finish names the unique unit by its own name and its base unit (Req. 3, WCAG 3.3.4).

### Assertion expectation:
The summary holds "Unique unit: Tester, from Own".

## Redundancy test 592

### File residence:
`tests/model/present_tests.c`, lines 174-177

### Purpose:
Verify that Finish lists each civ bonus by its description (Req. 3, WCAG 3.3.4).

### Assertion expectation:
The summary holds "- Infantry +1 attack".

## Redundancy test 593

### File residence:
`tests/model/present_tests.c`, lines 179-182

### Purpose:
Verify that Finish names a civ bonus that the catalog lacks, by its number (Req. 9).

### Assertion expectation:
The summary holds "- bonus #99".

## Redundancy test 594

### File residence:
`tests/model/present_tests.c`, lines 184-187

### Purpose:
Verify that Finish names a unique tech without a name of its own by the catalog's (Req. 3, WCAG 3.3.4).

### Assertion expectation:
The summary holds "Castle tech: Bulwark".

## Redundancy test 595

### File residence:
`tests/model/present_tests.c`, lines 189-192

### Purpose:
Verify that Finish says so when the picked unit is not in the catalog (Req. 9).

### Assertion expectation:
For the bare civ the summary holds "Unique unit: an unknown unit".

## Redundancy test 596

### File residence:
`tests/model/present_tests.c`, lines 194-197

### Purpose:
Verify that Finish says so when a picked tech is not in the catalog (Req. 9).

### Assertion expectation:
For the bare civ the summary holds "Castle tech: an unknown tech".

## Redundancy test 597

### File residence:
`tests/model/present_tests.c`, lines 221-224

### Purpose:
Verify that Build says how many civ bonuses it built and that others were left out (Req. 9).

### Assertion expectation:
A report of 3 built and 1 skipped reads "Civ bonuses built: 3 of 4; the others are not built yet."

## Redundancy test 598

### File residence:
`tests/model/present_tests.c`, lines 226-229

### Purpose:
Verify that Build says so when it could not build the hero (Req. 9).

### Assertion expectation:
A report with `hero_skipped` holds "The hero is not built: the civ lacks its unit."

## Redundancy test 599

### File residence:
`tests/model/present_tests.c`, lines 231-234

### Purpose:
Verify that Build says when a bonus counts fewer times than its multiplier asks (Req. 9).

### Assertion expectation:
A report with one multiplier not applied says "A tech that every civ of the game shares counts only once in a bonus."

## Redundancy test 600

### File residence:
`tests/model/present_tests.c`, lines 236-240

### Purpose:
Verify that Build names the two folders of the mod and nothing as left out (Req. 7, 9).

### Assertion expectation:
A report without anything skipped names `testonia-data`, `testonia-ui`, the civ and "2 of 2.", then the advice.

## Redundancy test 601

### File residence:
`tests/model/wizard_tests.c`, lines 35-39

### Purpose:
Verify that the list of steps marks no step as done before anything is chosen (Req. 1).

### Assertion expectation:
Of the steps of a design just reset, `wizard_step_is_done` is true for none.

## Redundancy test 602

### File residence:
`tests/model/wizard_tests.c`, lines 41-44

### Purpose:
Verify that the Name and tech tree step is marked done when the civ has a name (Req. 1).

### Assertion expectation:
After the name "Testonia" is set, `wizard_step_is_done` is true for the Name and tech tree step.

## Redundancy test 603

### File residence:
`tests/model/wizard_tests.c`, lines 46-49

### Purpose:
Verify that the Name and tech tree step is marked done when a node is chosen (Req. 1).

### Assertion expectation:
After building 12 is chosen, `wizard_step_is_done` is true for the Name and tech tree step.

## Redundancy test 604

### File residence:
`tests/model/wizard_tests.c`, lines 51-54

### Purpose:
Verify that the Bonuses step is marked done when a bonus is ticked (Req. 1).

### Assertion expectation:
After bonus 4 is ticked, `wizard_step_is_done` is true for the Bonuses step.

## Redundancy test 605

### File residence:
`tests/model/wizard_tests.c`, lines 56-59

### Purpose:
Verify that the Bonuses step is marked done when a team bonus is picked (Req. 1).

### Assertion expectation:
After team bonus 8 is picked, `wizard_step_is_done` is true for the Bonuses step.

## Redundancy test 606

### File residence:
`tests/model/wizard_tests.c`, lines 61-64

### Purpose:
Verify that the UU&UT step is marked done when a unit is picked (Req. 1).

### Assertion expectation:
After unit 1 of the game is picked, `wizard_step_is_done` is true for the UU&UT step.

## Redundancy test 607

### File residence:
`tests/model/wizard_tests.c`, lines 66-69

### Purpose:
Verify that the UU&UT step is marked done when a castle tech is picked (Req. 1).

### Assertion expectation:
After castle tech 3 is picked, `wizard_step_is_done` is true for the UU&UT step.

## Redundancy test 608

### File residence:
`tests/model/wizard_tests.c`, lines 71-75

### Purpose:
Verify that the UU&UT step is marked done when an imperial tech is picked (Req. 1).

### Assertion expectation:
After imperial tech 3 is picked, `wizard_step_is_done` is true for the UU&UT step.

## Redundancy test 609

### File residence:
`tests/model/wizard_tests.c`, lines 77-80

### Purpose:
Verify that the UU&UT step is marked done when a hero is picked (Req. 1).

### Assertion expectation:
After hero 1911 is picked, `wizard_step_is_done` is true for the UU&UT step.

## Redundancy test 610

### File residence:
`tests/model/wizard_tests.c`, lines 82-86

### Purpose:
Prevent a choice in one step from marking another step as done (Req. 1).

### Assertion expectation:
After a unique unit is picked on a new design, exactly one step is done.

## Redundancy test 611

### File residence:
`tests/model/wizard_tests.c`, lines 88-91

### Purpose:
Verify that Back leads from the second step to the first (Req. 1).

### Assertion expectation:
`wizard_move` of the Bonuses step by -1 gives the Name and tech tree step.

## Redundancy test 612

### File residence:
`tests/model/history_tests.c`, lines 50-54

### Purpose:
Verify that undo does nothing before the first change (Req. 1).

### Assertion expectation:
After `history_start`, `history_step` back gives false.

## Redundancy test 613

### File residence:
`tests/model/history_tests.c`, lines 56-60

### Purpose:
Verify that redo does nothing before the first undo (Req. 1).

### Assertion expectation:
After `history_start`, `history_step` forward gives false.

## Redundancy test 614

### File residence:
`tests/model/history_tests.c`, lines 62-67

### Purpose:
Prevent an undo with nothing to undo from giving redo something to do (Req. 1).

### Assertion expectation:
After an undo on a new design, `history_step` forward gives false.

## Redundancy test 615

### File residence:
`tests/model/history_tests.c`, lines 69-73

### Purpose:
Verify that the window is told when a change was made, so that it marks the civ unsaved (Req. 1).

### Assertion expectation:
`history_do` of ticking bonus 7 on a new design gives true.

## Redundancy test 616

### File residence:
`tests/model/history_tests.c`, lines 75-81

### Purpose:
Verify that a new change ends what could be made again (Req. 1).

### Assertion expectation:
After a change, an undo and another change, `history_step` forward gives false.

## Redundancy test 617

### File residence:
`tests/model/history_tests.c`, lines 83-87

### Purpose:
Verify that undo goes back 32 changes and no further (Req. 1).

### Assertion expectation:
After 33 changes, undo succeeds 32 times, `HISTORY_DEPTH`, and then fails.

## Redundancy test 618

### File residence:
`tests/model/history_tests.c`, lines 101-106

### Purpose:
Verify that the newest changes are the ones kept once the history is full (Req. 1).

### Assertion expectation:
After changes numbered 1 to 33, two undos leave the design at the change numbered 31.

## Redundancy test 619

### File residence:
`tests/model/history_tests.c`, lines 108-113

### Purpose:
Verify that opening a civ file or picking a base civ can be taken back (Req. 1).

### Assertion expectation:
After `history_replace`, an undo brings back the design with its one bonus.

## Redundancy test 620

### File residence:
`tests/model/history_tests.c`, lines 115-122

### Purpose:
Verify that a design put in place of another ends what could be made again (Req. 1).

### Assertion expectation:
After a change, an undo and `history_replace`, `history_step` forward gives false.

## Redundancy test 621

### File residence:
`tests/model/history_tests.c`, lines 124-130

### Purpose:
Prevent a tick from joining the typing before it into one undo (Req. 1).

### Assertion expectation:
After typing "V" and ticking a bonus, one undo leaves the name "V".

## Redundancy test 622

### File residence:
`tests/model/history_tests.c`, lines 132-140

### Purpose:
Prevent typing after an undo from joining the typing that was undone (Req. 1).

### Assertion expectation:
After typing "V" and "Vi", an undo and typing "X", an undo succeeds and leaves the name empty.

## Redundancy test 623

### File residence:
`tests/design/design_tests.c`, lines 29-32

### Purpose:
Verify that a bonus is saved with the multiplier 1, as the original's civ files have it (Req. 1).

### Assertion expectation:
After bonus 7 is ticked, the multiplier of the design's first bonus is 1.

## Redundancy test 624

### File residence:
`tests/design/design_tests.c`, lines 34-38

### Purpose:
Verify that a team bonus is saved with the multiplier 1 (Req. 1).

### Assertion expectation:
After team bonus 3 is picked, the multiplier of the design's team bonus is 1.

## Redundancy test 625

### File residence:
`tests/design/design_tests.c`, lines 40-45

### Purpose:
Prevent a civ file from taking a bonus more than 10 times, the most that Build makes (Req. 8).

### Assertion expectation:
A multiplier of 11 is refused, and the bonus counts once.

## Redundancy test 626

### File residence:
`tests/design/design_tests.c`, lines 47-52

### Purpose:
Prevent a civ file from taking a bonus less than once (Req. 8).

### Assertion expectation:
A multiplier of 0 is refused, and the bonus counts once.

## Redundancy test 627

### File residence:
`tests/design/design_tests.c`, lines 54-58

### Purpose:
Verify that unticking a bonus takes it out of the design (Req. 1).

### Assertion expectation:
After bonus 7 is ticked twice, the design has 0 bonuses.

## Redundancy test 628

### File residence:
`tests/design/design_tests.c`, lines 60-65

### Purpose:
Verify that unticking a node takes it out of the tech tree (Req. 1).

### Assertion expectation:
After unit 5 is chosen twice, `civ_design_tree_has` is false for it.

## Redundancy test 629

### File residence:
`tests/design/design_tests.c`, lines 67-73

### Purpose:
Prevent a node that is taken out from taking another with it (Req. 1).

### Assertion expectation:
After units 5 and 9 are chosen and 5 is taken out, `civ_design_tree_has` is true for 9.

## Redundancy test 630

### File residence:
`tests/design/design_tests.c`, lines 75-79

### Purpose:
Prevent a node from being written past the end of the tree's list (Rule 26).

### Assertion expectation:
After 400 units are chosen, `CIV_TREE_CAPACITY`, choosing unit 401 gives false.

## Redundancy test 631

### File residence:
`tests/design/saved_tests.c`, lines 21-26

### Purpose:
Verify that a civ file names a unit of the game as the original's files do (Req. 1).

### Assertion expectation:
With unit 1263 of the game picked, the saved text holds `"km_idx": null`.

## Redundancy test 632

### File residence:
`tests/design/saved_tests.c`, lines 28-33

### Purpose:
Verify that a civ file names a custom unit as the original's files do (Req. 1).

### Assertion expectation:
With custom unit 5 picked, the saved text holds `"vanilla_id": null`.

## Redundancy test 633

### File residence:
`tests/design/saved_tests.c`, lines 35-40

### Purpose:
Verify that a civ file counts the civ of its language from 0, as the original does (Req. 1).

### Assertion expectation:
With the language of slot 1, the Britons, the saved text holds `"language": 0`.

## Redundancy test 634

### File residence:
`tests/model/present_tests.c`, lines 250-253

### Purpose:
Verify that a build that left nothing out is whole, so that no message is needed (Req. 9).

### Assertion expectation:
`summary_build_is_whole` is true for the report of a civ built with nothing skipped.

## Redundancy test 635

### File residence:
`tests/model/present_tests.c`, lines 255-258

### Purpose:
Verify that a build that skipped a civ bonus is not whole, so that a message says so (Req. 9).

### Assertion expectation:
`summary_build_is_whole` is false for a report with one civ bonus skipped.

## Redundancy test 636

### File residence:
`tests/model/wizard_tests.c`, lines 93-96

### Purpose:
Verify that the last step is titled Finish (Req. 3).

### Assertion expectation:
`wizard_page(STEP_FINISH)` has the title "Finish".

## Redundancy test 637

### File residence:
`tests/shell/dialog_tests.c`, lines 232-235

### Purpose:
Verify that a civ the user named is saved under that name (Req. 1).

### Assertion expectation:
With the name "Testonia" typed, Save writes `build/Testonia.civeditor.json` anew, over a stale file.

## Redundancy test 638

### File residence:
`tests/shell/dialog_tests.c`, lines 237-240

### Purpose:
Verify that a cancelled Save is said in the status line (Req. 3, WCAG 3.3.1).

### Assertion expectation:
With the Save dialog cancelled, the status line holds "The civ was not saved."

## Redundancy test 639

### File residence:
`tests/shell/dialog_tests.c`, lines 242-245

### Purpose:
Verify that a civ saved by the window is opened by it again (Req. 1).

### Assertion expectation:
"Testonia" is saved, the name is changed to "Other", the file is opened: the title names "Testonia" again.

## Redundancy test 640

### File residence:
`tests/shell/dialog_tests.c`, lines 247-250

### Purpose:
Verify that a file that is no civ file is refused in words (Req. 1, WCAG 3.3.1).

### Assertion expectation:
Opening `README.md` gives a message that holds "not a civ file".

## Redundancy test 641

### File residence:
`tests/shell/dialog_tests.c`, lines 252-255

### Purpose:
Verify that the button Next goes to the next step (Req. 3, WCAG 2.1.1).

### Assertion expectation:
After a click of Next on step 1, the list of steps has its second row chosen.

## Redundancy test 642

### File residence:
`tests/shell/dialog_tests.c`, lines 257-260

### Purpose:
Verify that a name typed into the window reaches the design and the title (Req. 3, WCAG 2.4.2).

### Assertion expectation:
With "Testonia" typed into Name, the window's title holds "Testonia - Step 1".

## Redundancy test 643

### File residence:
`tests/shell/dialog_tests.c`, lines 262-265

### Purpose:
Verify that Redo answers in the status line when nothing was undone (Req. 5).

### Assertion expectation:
In a new window the Redo command leaves "There is nothing to redo." in the status line.

## Redundancy test 644

### File residence:
`tests/shell/dialog_tests.c`, lines 267-270

### Purpose:
Prevent unsaved changes from being lost when closing is cancelled (Req. 3, WCAG 3.3.4).

### Assertion expectation:
With a name typed and the question answered Cancel, the window is still there after a close.

## Redundancy test 645

### File residence:
`tests/shell/dialog_tests.c`, lines 272-275

### Purpose:
Verify that the window closes when the user chooses not to save (Req. 3, WCAG 3.3.4).

### Assertion expectation:
With a name typed and the question answered No, the window is gone after a close.

## Redundancy test 646

### File residence:
`tests/shell/dialog_tests.c`, lines 358-361

### Purpose:
Verify that Build says so when the game is not found on the computer (Req. 9).

### Assertion expectation:
With Steam's folder named as `build/no_steam`, Build gives a message that holds "cannot be built".

## Redundancy test 647

### File residence:
`tests/shell/dialog_tests.c`, lines 363-369

### Purpose:
Verify that Build writes no mod when the question of the mod's name is cancelled (Req. 9).

### Assertion expectation:
With a changed civ and the questions answered Cancel, the status line holds "No mod was built."

## Redundancy test 648

### File residence:
`tests/shell/dialog_tests.c`, lines 371-377

### Purpose:
Verify that Build of a civ that is not finished says what is wrong (Req. 9).

### Assertion expectation:
A civ whose name is emptied, then built, gives a message that holds "not finished".

## Redundancy test 649

### File residence:
`tests/shell/dialog_tests.c`, lines 390-395

### Purpose:
Verify that a build that left something out says what in a message (Req. 9).

### Assertion expectation:
A civ built from a game that lacks its unit and techs gives a message that holds "not built yet".

## Redundancy test 650

### File residence:
`tests/shell/dialog_tests.c`, lines 416-419

### Purpose:
Verify that the help line under the title is drawn in the muted colour of the palette (Req. 3, WCAG 1.4.3).

### Assertion expectation:
Asked for its colours, the window gives the help line the text colour of `COLOR_ROLE_MUTED`.

## Redundancy test 651

### File residence:
`tests/shell/dialog_tests.c`, lines 421-424

### Purpose:
Verify that a field one types in is drawn on the field colour of the palette (Req. 3, WCAG 1.4.3).

### Assertion expectation:
Asked for its colours, the window gives the field Name the background of `COLOR_ROLE_FIELD`.

## Redundancy test 652

### File residence:
`tests/shell/dialog_tests.c`, lines 426-446

### Purpose:
Verify that the row in focus inside a list has a frame in the focus colour (Req. 3, WCAG 2.4.7).

### Assertion expectation:
A row drawn with the focus has the colour of `COLOR_ROLE_FOCUS` at its left edge.

## Redundancy test 653

### File residence:
`tests/shell/dialog_tests.c`, lines 448-457

### Purpose:
Prevent a pick of the row that is picked already from making a change to undo (Req. 5).

### Assertion expectation:
With the first unit picked twice, the second Undo says "There is nothing to undo."

## Redundancy test 654

### File residence:
`tests/shell/scroll_tests.c`, lines 78-94

### Purpose:
Verify that the knob of the window's scroll bar reaches the bar's end at the end of a step (Req. 3, WCAG 1.4.10).

### Assertion expectation:
After 16 pages down on the first step in a window of 480 by 360, position and page of the bar add up to its range.

## Redundancy test 655

### File residence:
`tests/shell/scroll_tests.c`, lines 96-106

### Purpose:
Prevent the window's scroll bar from showing when the step fits the window exactly (Req. 3, WCAG 1.4.10).

### Assertion expectation:
A window 480 wide whose inside is exactly as high as Finish in one column has its scroll bar hidden.

## Redundancy test 656

### File residence:
`tests/shell/scroll_tests.c`, lines 108-121

### Purpose:
Verify that the up arrow of the window's scroll bar moves the step up a line (Req. 3, WCAG 1.4.10).

### Assertion expectation:
On the first step at 480 by 360, a page down and then the up arrow leave the bar 44 units above the page.

## Redundancy test 657

### File residence:
`tests/shell/scroll_tests.c`, lines 123-131

### Purpose:
Verify that the loop of messages hands a key pressed in a list to the list once (Req. 3, WCAG 2.1.1).

### Assertion expectation:
With the caret on row 0 of the Tech tree list, the key Down, run through `app_run_messages`, puts it on row 1.

## Redundancy test 658

### File residence:
`tests/shell/scroll_tests.c`, lines 133-140

### Purpose:
Verify that a list paints the room that its rows leave free (Req. 12).

### Assertion expectation:
The list of steps, erased into a black picture, has its lowest row of pixels in the colour of fields.

## Redundancy test 659

### File residence:
`tests/shell/scroll_tests.c`, lines 142-147

### Purpose:
Verify that a list whose rows fill it paints no room beyond its own (Req. 12).

### Assertion expectation:
The tech tree list, erased into a picture one pixel higher than itself, leaves that row of pixels black.

## Redundancy test 660

### File residence:
`tests/shell/hero_tests.c`, lines 41-44

### Purpose:
Verify that ticking Hero in UU&UT gives the civ Attila the Hun, named as the game names him (Req. 8).

### Assertion expectation:
After the Hero box of UU&UT is ticked, the name shown above Choose hero... is "Attila the Hun".

## Redundancy test 661

### File residence:
`tests/shell/hero_tests.c`, lines 46-56

### Purpose:
Verify that Choose hero... shows only while the civ has a hero (Req. 8).

### Assertion expectation:
After the Hero box is ticked and unticked again, Choose hero... is hidden.

## Redundancy test 662

### File residence:
`tests/shell/hero_tests.c`, lines 58-65

### Purpose:
Verify that a hero picked in Choose hero... takes the game's name without one being typed (Req. 8).

### Assertion expectation:
With Saladin picked in the list of Choose hero... and OK pressed, the name shown above it is "Saladin".

## Redundancy test 663

### File residence:
`tests/shell/hero_tests.c`, lines 67-74

### Purpose:
Verify that the name typed in Choose hero... becomes the hero's name (Req. 8).

### Assertion expectation:
With Saladin picked, "Testhero" typed as his name and OK pressed, the name shown is "Testhero".

## Redundancy test 664

### File residence:
`tests/shell/dialog_tests.c`, lines 397-402

### Purpose:
Verify that Build writes the mod without asking whether its folder is right (Req. 9).

### Assertion expectation:
With only the mod's name answered, the status line holds "The mod was built as testonia-data".

## Redundancy test 665

### File residence:
`tests/shell/dialog_tests.c`, lines 404-407

### Purpose:
Verify that Options in the menu File takes a mods folder that exists (Req. 9).

### Assertion expectation:
With the folder `build` given, the status line holds "now writes into the mods folder given".

## Redundancy test 666

### File residence:
`tests/shell/dialog_tests.c`, lines 409-414

### Purpose:
Verify that Options refuses a mods folder that does not exist (Req. 9).

### Assertion expectation:
With the folder `build\no_such_folder` given, a message holds "does not exist".

## Redundancy test 667

### File residence:
`tests/shell/dialog_tests.c`, lines 277-287

### Purpose:
Verify that the option not to ask lets a changed civ close without a question (Req. 12).

### Assertion expectation:
With the option set to false, `WM_CLOSE` sent to a window with a typed name destroys it.

## Redundancy test 668

### File residence:
`tests/shell/dialog_tests.c`, lines 316-327

### Purpose:
Verify that Options saves the tick box of asking to save before exiting (Req. 12).

### Assertion expectation:
Options answered OK with the box unticked leaves `options_ask_to_save()` false.

## Redundancy test 669

### File residence:
`tests/shell/dialog_tests.c`, lines 342-345

### Purpose:
Verify that Options answered without another folder reports the options kept (Req. 12).

### Assertion expectation:
After Options answered OK the status line holds "The options were kept."

## Redundancy test 670

### File residence:
`tests/shell/dialog_tests.c`, lines 347-356

### Purpose:
Verify that without a file of options the program asks before exiting (Req. 12).

### Assertion expectation:
With a home folder that does not exist, `options_ask_to_save()` is true.

## Redundancy test 671

### File residence:
`tests/shell/hero_tests.c`, lines 102-105

### Purpose:
Verify that the tick box of a question dialog gives back a tick made in it (Req. 12).

### Assertion expectation:
A question shown with an unticked box, ticked and answered OK, leaves its `check` true.

## Redundancy test 672

### File residence:
`tests/shell/hero_tests.c`, lines 107-110

### Purpose:
Verify that the tick box of a question dialog starts as it is given (Req. 12).

### Assertion expectation:
A question shown with a ticked box, clicked once and answered OK, leaves its `check` false.

## Redundancy test 673

### File residence:
`tests/shell/dialog_tests.c`, lines 289-296

### Purpose:
Verify that the window shows the catalog anew after reading the game (Req. 12).

### Assertion expectation:
After `APP_GAME_WANTED` with the test game, the castle tech panel holds "Research time: 25 seconds".

## Redundancy test 674

### File residence:
`tests/shell/dialog_tests.c`, lines 306-309

### Purpose:
Verify that an unchanged civ is made its base civ anew once the game is read (Req. 12).

### Assertion expectation:
After `APP_GAME_WANTED` with the test game, row 0 of the tech tree list is no longer ticked.

## Redundancy test 675

### File residence:
`tests/shell/dialog_tests.c`, lines 311-314

### Purpose:
Verify that reading the game leaves a civ with unsaved changes as it is (Req. 12).

### Assertion expectation:
With a name typed first, row 0 of the tech tree list stays ticked after `APP_GAME_WANTED`.

## Redundancy test 676

### File residence:
`tests/shell/dialog_tests.c`, lines 329-340

### Purpose:
Verify that Options answered Cancel keeps the tick box as the options file has it (Req. 12).

### Assertion expectation:
Options cancelled with the box unticked leaves `options_ask_to_save()` true.

## Redundancy test 677

### File residence:
`tests/shell/hero_tests.c`, lines 112-116

### Purpose:
Verify that the buttons of a question dialog move down to make room for its tick box (Req. 12).

### Assertion expectation:
In the dialog shown, the bottom of the tick box is not below the top of the OK button.

## Redundancy test 678

### File residence:
`tests/shell/change_tests.c`, lines 101-114

### Purpose:
Verify that the kept heights of a list's rows are dropped when the list changes width (Req. 12).

### Assertion expectation:
The rows of the list of bonuses add up to less in the window made 420 wide, where the list is wider.

## Redundancy test 679

### File residence:
`tests/shell/change_tests.c`, lines 116-128

### Purpose:
Verify that a list is redrawn only when one of its ticks changed (Req. 12).

### Assertion expectation:
After "T" typed into the name of a painted window, the tech tree list has no update region.

## Redundancy test 680

### File residence:
`tests/shell/change_tests.c`, lines 153-156

### Purpose:
Verify that the rows of a list without check boxes are as high as their text needs (Req. 12).

### Assertion expectation:
In a window 420 wide the row of the longest team bonus is higher than that of the shortest.

## Redundancy test 681

### File residence:
`tests/shell/hero_tests.c`, lines 95-100

### Purpose:
Verify that a question with a tick box is answered by OK though its text is empty (Req. 12).

### Assertion expectation:
The dialog shown with a tick box and an empty text, answered OK, gives true.

## Redundancy test 682

### File residence:
`tests/shell/change_tests.c`, lines 70-79

### Purpose:
Verify that the button Appearance opens the Appearance popup (Req. 3).

### Assertion expectation:
After a click on Appearance, the window of the class `civeditor.popup` is visible.

## Redundancy test 683

### File residence:
`tests/shell/change_tests.c`, lines 158-165

### Purpose:
Verify that the Appearance popup is hidden until Appearance is pressed (Req. 3).

### Assertion expectation:
In a new window, the window of the class `civeditor.popup` exists and is not visible.

## Redundancy test 684

### File residence:
`tests/shell/change_tests.c`, lines 167-174

### Purpose:
Verify that a look choice of the first step is held by the Appearance popup (Req. 3).

### Assertion expectation:
The Appearance popup has a child with the id of `CONTROL_ARCHITECTURE`.

## Redundancy test 685

### File residence:
`tests/shell/change_tests.c`, lines 176-183

### Purpose:
Verify that Civ to edit offers no civ outside ranked play (Req. 3).

### Assertion expectation:
No row of Civ to edit starts with "Spartans".

## Redundancy test 686

### File residence:
`tests/shell/change_tests.c`, lines 185-188

### Purpose:
Verify that a changed civ is marked with an asterisk in Civ to edit (Req. 3).

### Assertion expectation:
After "Testonia" is typed as the name, the first row of Civ to edit reads "Britons*".

## Redundancy test 687

### File residence:
`tests/shell/change_tests.c`, lines 190-193

### Purpose:
Verify that a civ changed back to the game's loses its asterisk in Civ to edit (Req. 3).

### Assertion expectation:
After "Testonia" and then "Britons" are typed, the first row of Civ to edit reads "Britons".

## Redundancy test 688

### File residence:
`tests/shell/change_tests.c`, lines 195-205

### Purpose:
Verify that a change puts the civ in the mod at once, with no button to save it (Req. 3, 9).

### Assertion expectation:
After "Testonia" is typed and Finish is opened, its label holds "1 civ changed".

## Redundancy test 689

### File residence:
`tests/shell/change_tests.c`, lines 207-216

### Purpose:
Verify that the filter of the team bonuses narrows their list (Req. 3).

### Assertion expectation:
With "no team bonus says this" typed in the filter, the list of team bonuses has 0 rows.

## Redundancy test 690

### File residence:
`tests/shell/change_tests.c`, lines 239-242

### Purpose:
Verify that the filter of the unique units takes a civ's name (Req. 3).

### Assertion expectation:
With "britons" typed in the filter, the first row of the unique units starts with "Longbowman".

## Redundancy test 691

### File residence:
`tests/shell/change_tests.c`, lines 244-247

### Purpose:
Verify that the filter of the Castle Age techs takes a civ's name (Req. 3).

### Assertion expectation:
With "britons" typed in the filter, the first row of the Castle Age techs starts with "Yeomen".

## Redundancy test 692

### File residence:
`tests/shell/change_tests.c`, lines 249-252

### Purpose:
Verify that the filter of the Imperial Age techs takes a civ's name (Req. 3).

### Assertion expectation:
With "britons" typed in the filter, the first row of the Imperial Age techs starts with "Warwolf".

## Redundancy test 693

### File residence:
`tests/shell/change_tests.c`, lines 266-277

### Purpose:
Verify that the filter of Choose hero narrows the list of heroes (Req. 3).

### Assertion expectation:
With "saladin" typed in the filter of the dialog, its list has 1 row.

## Redundancy test 694

### File residence:
`tests/shell/change_tests.c`, lines 254-264

### Purpose:
Verify that Clear unticks every civ bonus (Req. 3).

### Assertion expectation:
After a click on Clear, the summary of Finish holds "Civ bonuses: 0".

## Redundancy test 695

### File residence:
`tests/shell/change_tests.c`, lines 229-232

### Purpose:
Verify that the filter of the civ bonuses takes a civ's name (Req. 3).

### Assertion expectation:
With "britons" typed in the filter, the list of civ bonuses has at least 1 row.

## Redundancy test 696

### File residence:
`tests/shell/change_tests.c`, lines 234-237

### Purpose:
Verify that the filter of the tech tree takes a civ's name (Req. 3).

### Assertion expectation:
With "britons" typed in the filter, the list of the tech tree has at least 1 row.

## Redundancy test 697

### File residence:
`tests/rules/tool_tests.c`, lines 82-85

### Purpose:
Verify that the build reads the sources of a folder and of the folders in it (Rule 3).

### Assertion expectation:
`source_list_collect` of `tests` and `.txt` lists `tests/fixtures/broken_rules.txt`.

## Redundancy test 698

### File residence:
`tests/rules/tool_tests.c`, lines 89-102

### Purpose:
Verify that the build lists a folder's sources and nothing before them or beside them (Rule 3).

### Assertion expectation:
Every file that `source_list_collect` lists of `tests` and `.txt` lies in `tests/`, and there is at least one.

## Redundancy test 699

### File residence:
`tests/rules/tool_tests.c`, lines 111-116

### Purpose:
Verify that the build reads the sources of `src`, `tests` and `tools`, each once (Rule 3).

### Assertion expectation:
`source_list_collect_all` of `.h` lists as many files as `source_list_collect` of `src`, `tests` and `tools` together.

## Redundancy test 700

### File residence:
`tests/rules/tool_tests.c`, lines 118-124

### Purpose:
Verify that a header changed after an object makes the build compile again (Rule 3).

### Assertion expectation:
Of two files changed at 9 and at 5, `source_list_newest` gives 9.

## Redundancy test 701

### File residence:
`tests/rules/tool_tests.c`, lines 126-132

### Purpose:
Verify that each program finds the object of a source where the build put it (Rule 3).

### Assertion expectation:
`source_built_path` of `build/obj_quick`, `src/core/text.c` and `.o` gives `build/obj_quick/text.o`.

## Redundancy test 702

### File residence:
`tests/rules/tool_tests.c`, lines 134-137

### Purpose:
Verify that a build compiles only what changed (Rule 3).

### Assertion expectation:
A source changed at 0, with headers changed at 0, is fresh against an object that exists.

## Redundancy test 703

### File residence:
`tests/rules/tool_tests.c`, lines 139-142

### Purpose:
Prevent a stale object after a header changed (Rule 3).

### Assertion expectation:
A source is not fresh when a header is changed one tick after its object.

## Redundancy test 704

### File residence:
`tests/rules/tool_tests.c`, lines 144-147

### Purpose:
Prevent a stale object when a source is saved in the tick its object was made (Rule 3).

### Assertion expectation:
A source changed at the very time of its object is not fresh.

## Redundancy test 705

### File residence:
`tests/rules/tool_tests.c`, lines 149-152

### Purpose:
Verify that the build tool is linked from its own sources and `src/core/` only (Rule 3).

### Assertion expectation:
For the build tool, `src/core/text.c` and `tools/build/build.c` give `obj/text.o` and `obj/build.o`, and no library.

## Redundancy test 706

### File residence:
`tests/rules/tool_tests.c`, lines 154-157

### Purpose:
Verify that `build.exe coverage` builds the tests to measure what they run (Req. 10).

### Assertion expectation:
`build_mode_of` of "build.exe coverage" is `BUILD_MODE_COVERAGE`.

## Redundancy test 707

### File residence:
`tests/rules/tool_tests.c`, lines 159-163

### Purpose:
Verify that `nowindow`, `norules` and `redundancy` given to the build are passed on to the tests (Req. 12).

### Assertion expectation:
`build_test_arguments` of "build.exe quick nowindow norules redundancy" is " nowindow norules redundancy".

## Redundancy test 708

### File residence:
`tests/rules/tool_tests.c`, lines 165-170

### Purpose:
Verify that a coverage build links the tests and no other program (Req. 10).

### Assertion expectation:
`build_programs_of(BUILD_MODE_COVERAGE)` starts at `SOURCE_PROGRAM_TESTS`, ends before `SOURCE_PROGRAM_BUILD_TOOL`.

## Redundancy test 709

### File residence:
`tests/rules/tool_tests.c`, lines 172-176

### Purpose:
Verify that a coverage build links the tests with gcc's coverage (Req. 10).

### Assertion expectation:
The link command of the tests in a coverage build holds "gcc -g --coverage -o build/tests.exe".

## Redundancy test 710

### File residence:
`tests/rules/tool_tests.c`, lines 178-182

### Purpose:
Verify that a coverage build makes the folder that its report is written to (Req. 10).

### Assertion expectation:
The last folder of `BUILD_MODE_COVERAGE` is "target/coverage/c".

## Redundancy test 711

### File residence:
`tests/redundancy/rules/rule_redundancy.c`, line 99; `tests/redundancy/rules/recursion_rule.c`, lines 34-50

### Purpose:
Enforce that no function calls itself, directly or through others (Rule 26, P10 rule 1).

### Assertion expectation:
The graph of calls between the functions of all `.c` files has no cycle; a failure names a function in one.

## Redundancy test 712

### File residence:
`tests/redundancy/rules/rule_redundancy.c`, line 101; `tests/redundancy/rules/p10_patterns.c`, lines 20-23

### Purpose:
Enforce that no memory is allocated while the program runs (Rule 26, P10 rule 3).

### Assertion expectation:
None of `malloc`, `calloc`, `realloc`, `free`, `alloca`, `strdup` or the Win32 allocators appears in the code.

## Redundancy test 713

### File residence:
`tests/redundancy/rules/rule_redundancy.c`, line 103; `tests/redundancy/rules/p10_patterns.c`, lines 25-34

### Purpose:
Enforce that the preprocessor only includes files and guards headers (Rule 26, P10 rule 8).

### Assertion expectation:
Only `#include`, `#ifndef`, `#endif` and `#define` of a `_H` guard or of `CHECK(` appear.

## Redundancy test 714

### File residence:
`tests/redundancy/rules/rule_redundancy.c`, line 105; `tests/redundancy/rules/p10_patterns.c`, lines 83-96

### Purpose:
Enforce at most one level of dereferencing (Rule 26, P10 rule 9).

### Assertion expectation:
No `**`, no `*const *` and no chain of two `->` in the code.

## Redundancy test 715

### File residence:
`tests/redundancy/rules/rule_redundancy.c`, lines 107-108; `tests/redundancy/rules/p10_patterns.c`, lines 127-136

### Purpose:
Enforce that no function pointer is used but the one window procedure Win32 needs (Rule 26, P10 rule 9).

### Assertion expectation:
No `(*name)(`, no callback API; `lpfnWndProc` only in `src/win/app_window.c`.

## Redundancy test 716

### File residence:
`tests/rules/gate_tests.c`, lines 30-33

### Purpose:
Prevent the test of simple control flow from passing a `goto` unseen (Rule 26).

### Assertion expectation:
In `tests/fixtures/broken_rules.txt`, `pattern_forbidden_flow_line` gives 5, the line of `goto out;`.

## Redundancy test 717

### File residence:
`tests/rules/gate_tests.c`, lines 35-38

### Purpose:
Prevent the test of dynamic memory from passing an allocation unseen (Rule 26).

### Assertion expectation:
In `tests/fixtures/broken_rules.txt`, `pattern_allocation_line` gives 12, the line of `malloc(4)`.

## Redundancy test 718

### File residence:
`tests/rules/gate_tests.c`, lines 40-43

### Purpose:
Prevent the test of the preprocessor from passing a macro unseen (Rule 26).

### Assertion expectation:
In `tests/fixtures/broken_rules.txt`, `pattern_complex_directive_line` gives 23, the line of `#define TWICE(x)`.

## Redundancy test 719

### File residence:
`tests/rules/gate_tests.c`, lines 45-48

### Purpose:
Prevent the test of pointer depth from passing two dereferences in a row unseen (Rule 26).

### Assertion expectation:
In `tests/fixtures/broken_rules.txt`, `pattern_deep_pointer_line` gives 27, the line of `outer->inner->value`.

## Redundancy test 720

### File residence:
`tests/rules/gate_tests.c`, lines 50-53

### Purpose:
Prevent the test of function pointers from passing one unseen (Rule 26).

### Assertion expectation:
In `tests/fixtures/broken_rules.txt`, `pattern_function_pointer_line` gives 30, the line of `(*callback)(int)`.

## Redundancy test 721

### File residence:
`tests/rules/gate_tests.c`, lines 55-58

### Purpose:
Prevent the test of comments from passing a comment unseen (Rule 10).

### Assertion expectation:
After `source_scan` of `tests/fixtures/broken_rules.txt`, `comment_line` is 34, the line of its one comment.

## Redundancy test 722

### File residence:
`tests/rules/gate_tests.c`, lines 60-67

### Purpose:
Prevent the test of recursion from passing a function that calls itself unseen (Rule 26).

### Assertion expectation:
The call graph of `tests/fixtures/broken_rules.txt` has a cycle, and the function named is `again`.
