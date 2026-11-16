# Sitrep, 2026-11-16 05:45

Nothing is committed and nothing is deleted by the run. `build.exe` runs the analyzer, which finds nothing, and 363
tests: all pass but Test 344, which fails only because there are more than 100 tests. The task is not finished: see
"Not done". The log is part 4, `documentation/DEVLOG/DEVLOG4.md`, the days 2026-11-15 and 2026-11-16.

## What is new

- **ub-mode's own tools are the only measure.** Rules 31, 32 and 34 are measured by `..\dryer`, `..\crapper` and
  `..\mutator`. The tools in C (`tools/crap/`, `tools/dry/`, `tools/mutate/`) are gone from the tree, with their 13
  tests. The three programs read `src/` only.
- **Rule 31:** `..\dryer\dryer.cmd --threshold 0.8` finds no duplicate candidates (it found 50 at first).
- **Rule 32:** `..\crapper\crapper.cmd --no-coverage` gives 814 functions, none with a complexity of 10 or more (29
  at first). The CRAP score itself is not measured yet: see question 1.
- **Rule 34:** the mutator runs, on tests without those of the documents (`build.exe quick nowindow norules`), so
  the cap of 100 tests no longer stops it. It made 8955 mutants of `src/`; 4754 were killed and 4201 survived
  (53%). About seven survivors in ten are in a `CHECK`, the `return` under one or a loop's fixed bound: see
  question 2. 70 tests were written for survivors that showed a real gap.
- **Requirement 3 at level AA:** `.github/ACCESSIBILITY.md` has 55 boxes, all ticked; 31 of them ask for a human's
  review. A window too small for the columns shows a step in one column and scrolls, and the rows of the lists
  wrap: make the window narrow to see it.
- **Tests:** 363. New since the last sitrep: the recipe on a small game, a whole mod built under `build/`, the
  window driven through its dialogs, the rule tests shown a text that breaks each rule, the narrow window, the
  steps that are done, undo and redo, the text helpers, the civ file's text, JSON, and the catalog files.

## Not done

- 4201 mutants survive. Those of `src/win/` (1128) and `src/shell/` (101) were judged without the tests of the
  real window, which are too slow for a whole run.
- The CRAP scores (question 1).
- The window was not tried at a text size of 200% or a scale of 400%, nor with a screen reader.
- The features of the original that Build skips, as listed under "Known gaps you will see" below.
- The program was started and looked at in a narrow and a wide window only; its dialogs were not tried by hand, nor
  the mod in the game.

## Questions to answer now

1. **gcovr.** The crapper reads coverage from `target\coverage\c\lcov.info`, which gcovr makes. It is not installed,
   and the run was not permitted to install it. Will you run
   `..\crapper\.venv\Scripts\python.exe -m pip install gcovr`?
2. **P10 or the mutator.** A `CHECK` fails only on arguments that no caller passes, and a loop's fixed bound is
   never the one that ends it, so their mutants cannot be killed by a test that uses the code as its user does.
   Rule 26 puts ub-mode before P10. Should the checks and bounds stay, and Rule 34 count the mutants outside them;
   or should they go, with the tests "at least two checks per function" and "every loop has a fixed upper bound"?
   Proposal: keep P10 and read Rule 34 so.
3. **The cap of 100 tests.** `TEST_LIMIT` in `tests/rules/log_rules.c` is 100; no rule names a number any more.
   While Test 344 fails, `build.exe` cannot pass. The run was not permitted to change it. Will you raise it or
   take it out?
4. **Requirement 4 still says AAA,** while Requirement 3 says AA. The 31 AAA criteria are kept in a closing section
   of the checklist, without boxes. Is that what you want?
5. **Pairs alike by form only.** Where the dryer paired two functions that share no decision, one of them was
   written another way, or its callers make the call themselves. Accept?
6. **`tests/` and `tools/build/` are outside the measure,** since the three programs pass them by. Accept?

## How to test what is new

1. `gcc -o build.exe tools/build/*.c src/core/*.c`, then `build.exe`: 363 tests, one failed (Test 344).
2. `..\dryer\dryer.cmd --threshold 0.8`: `No duplicate candidates found.`
3. `..\crapper\crapper.cmd --no-coverage`: the highest complexity in the table is 9.
4. `..\mutator\mutator.cmd --no-coverage --test-command ".\build.exe quick nowindow norules" src/view/wizard.c`:
   about a minute; 42 killed and 13 survived when it was last run.
5. `build\civbuilder.exe`: drag the window narrow, then make a civ and build it, as in the sitrep below; the steps
   in the game are unchanged.

# Sitrep, 2026-11-10 07:03

Nothing is committed and nothing is deleted. `build.exe` runs the analyzer, which finds nothing, and 100 tests (54
functional, 22 accessibility, 24 maintainability): all pass. The task is not finished: see "Not done".

## What happened in the night

- **The usage limit took most of it.** At 22:30 the remaining work was started as six parallel streams in private
  copies of the tree. The 5-hour usage limit was reached at 22:35, and again at 23:24 after it reset at 23:10.
  Nothing could run until 04:10. The streams were then stopped, as you ordered, and the work went on alone, one
  change at a time. The streams left nothing worth keeping.
- Everything below was done between 04:11 and 07:03. The log of it is the day 2026-11-10 in
  `documentation/DEVLOG/DEVLOG3.md`, a new part.

## What is new

1. **The three tools of ub-mode exist, in C,** built by `build.exe` and run by hand after it:
   - `build\crapper.exe` (Rule 32, with gcc's own gcov): 323 of 964 functions are at CRAP 10 or above, about 200 of
     them not run by any test. The worst is `describe_build` in `dialogs.c`, at 552. `build\crap.txt` lists them all.
   - `build\dryer.exe` (Rule 31): 66 pairs of functions are 80% alike or more, most of them wholly, such as the six
     `catalog_find_...` functions. `build\dry.txt` lists the pairs from 60%.
   - `build\mutator.exe src/<folder>/` (Rule 34), twelve mutants at a time: on all of `src/` but `src/win/`, in 20
     minutes, 4372 mutants, 2334 killed, 2018 survived, and 2547 more places on lines that the tests without
     a window do not run (877 in `src/forge/`). It runs the tests without those of the real window, which are
     slow when twelve run at once.
   - So Rules 31, 32 and 34 are measured now, and none of them holds.
2. **The real window is tested, without being shown.** `app_open` makes it; Tests 61 to 71 and 74 to 76 drive it by
   messages and ask Windows what the keyboard and a screen reader would get. They never take the keyboard or the mouse.
3. **A bug found and mended: the Tab key was trapped** in the fields of several lines (Description, and the
   description of the unique unit). Test 68 found it; the window now moves the focus on.
4. **Bugs 1 and 2 of your trial have tests** (Tests 64 and 75): a ticked row leaves the list where it was
   scrolled, and it is drawn with a check, not a filled box.
5. **Every control has help:** F1, or Help in the menu, says in the status line what the control in focus is for.
   The status line is told to a screen reader as it changes (a polite live region). The text and the layout
   follow the text size of Windows' accessibility settings, as well as the display's scale. Glossary in the menu
   Help explains the game's words and the catalog's shortened forms (HP, LOS, vs. and the like). The control in
   focus has a solid frame of two pixels, drawn by the window.
6. **The accessibility checklist has 82 of 86 ticked,** up from 46: most by the new tests, seven by a reading
   of all 144 texts of the window, two because they are about markup. The menu's "Previous step" became "Back", and
   two labels were made to stand alone ("Description of the unique unit", "The selected unit").
7. **Question 8 of 10:51 is carried out as proposed:** in the tech tree file, a node the replaced civ lacked
   takes the status most civs give it; 13 nodes, such as the Hand Cannoneer and the Keep, are written as still to be
   researched. Question 9 is answered another way: the glossary explains the shortened forms.
8. The test of what the project holds lets the pictures of the trial pass (`.png` counts as a document), and
   the paths of the moved files are mended in `AGENTS.md`, `MAINTAINER.md` and `TESTS.md`.

## Not done

- **The original's features that were missing are still missing:** the 22 civ bonuses that Build skips, the Royal
  tiers, a bonus's multiplier, custom unique units, language and voices, the hero's description, flags and
  overrides. Of the nine questions of 10:51 below, only 8 was carried out as proposed; 5 (the six civs with gaps)
  and 7 (reflow) were not.
- **Bugs 4 to 6 are still untried in the game,** and their mending was not read again.
- No function was brought below CRAP 10, no pair of alike functions was made one, and no surviving mutant was
  killed on purpose: the tools measure, and what they found is still to be mended.
- 4 boxes of the checklist are unticked: reflow in a narrow window (1.4.10) and, with it, the text size at 200% on
  a small screen (1.4.4); the look of blocks of text (1.4.8); and the reading level of the catalog's 394
  descriptions (3.1.5).
- No screen reader was tried, and the window was not tried by hand after the changes.

## Questions to answer now

The nine questions of 10:51 below are still open. New, each with a proposal:

1. **Pictures in Rule 25.** Proposal: add `*.png` to Rule 25, as its test now allows it.
2. **Rule 5 against Rules 32 and 34.** All 100 tests are used, each of one check; 323 functions are at CRAP
   10 or above and 2018 mutants survive. Proposal: say which gives way.
3. **Checks as branches.** The crapper counts the branches of `CHECK`, and the mutator mutates them; with P10's two
   checks and doubled loop bound a small function has a complexity of 9, and such mutants cannot be killed while
   the checks hold. Proposal: keep counting them in the crapper, split functions, and let the mutator list mutants
   "of a check or a bound" apart.
4. **A gate.** Proposal: `build.exe` runs the crapper and the dryer and fails on them from the day each finds
   nothing.
5. **Parallel work.** One usage period carried about fourteen minutes of ten pieces of work at once. Proposal: one
   change at a time, unless you know the limit allows more.
6. **"Read" as evidence.** Seven boxes are ticked because every text was read on 2026-11-10. Proposal: that counts
   until a test can take its place.
7. **A window in the tests.** The tests make a real window, hidden; on a machine without a desktop they would fail.
   Proposal: keep it, since the tests are run by hand before a push.
8. **Small functions alike by their form.** Many of the 66 pairs are thin functions of two checks and one call,
   such as `json_begin_object` and `json_begin_array`. Proposal: say whether those are one decision in two places.

## How to test what is new

1. `build.exe`, then `build\crapper.exe`, `build\dryer.exe` and `build\mutator.exe src/base/` (half a minute).
   Do not build while the mutator runs.
2. In `build\civbuilder.exe`: on "Identity", put the cursor in "Description" and press Tab: the focus must move
   on, and Shift+Tab must move it back. The control in focus should have a dark frame around it; press F1 and read
   the status line; open Help, Glossary. The menu Go says "Next" and "Back". On "Unique unit" the labels read
   "Description of the unique unit" and "The selected unit".
3. With Narrator on, each control should be read with its label, and a ticked row as selected; Test 64 and Test
   65 say that Windows reports it so.
4. In the game: everything under "In the game" of 10:51 below still waits.
---

# Sitrep, 2026-11-09 10:51

Nothing is committed and nothing is deleted; the work is in the working tree for review. The answers in
`instructions_2.md` to the sitrep of 08:06, which is kept below, are recorded in the log and in `DECISIONS.md`.
`build.exe` runs the analyzer, which finds nothing, and 78 tests (53 functional, 5 accessibility, 20
maintainability): 76 pass. Tests 71 and 73 fail only on the main folder's files (question 1).

## What changed since 08:06

- **Bugs 1 to 3.** A list keeps its place when a row is ticked; a ticked row shows a check; the last step has no
  Next button.
- **Bugs 4 and 5.** The civ's description is written under a description id of its own, 990 100 plus its slot.
- **Bug 6.** The data mod holds the civ's tech tree file, `resources\_common\dat\CivTechTrees\<NAME>.json`: the
  game's file of the replaced civ, with each node set to the design. An empty tree keeps the game's file, and every
  civ keeps the Villager, both Town Centers, the Mill and the two camps, as the dat file does.
- **The base civ.** "Base civ" is the first choice on the step "Identity". Picking a civ sets its tech tree,
  bonuses, team bonus, unique unit, unique techs and looks, each still editable; undo takes it back. A new civ
  starts from Generic: every universal node, no regional or unique node, no bonus.
- **Accessibility.** The window's title names the civ and the step (WCAG 2.4.2). `.github/ACCESSIBILITY.md` is the
  checklist of Requirement 4: 46 of its 86 criteria are ticked.
- **Rules.** Rules 26 to 36 are in `RULES.md`; `.claude/settings.json` denies commits, pushes and deletions; the
  version is 1.0.0.

## Questions to answer now

Each has a proposal; "yes" accepts it. Each is also in the log, under the Questions of its section.

1. **Files in the main folder.** `instructions_2.md` and `image.png` to `image-3.png` make Tests 71 and 73 fail: 13
   files where Rule 14 allows 8, and pictures, which are neither C nor documents. Proposal: move them into
   `materials/`, which Git ignores.
2. **The default name.** "Saved and opened as `civbuilder_civ`" (Requirement 1) is read as the file name offered,
   `civbuilder_civ.civbuilder.json`; the format inside stays `empireforge_v2`, so the original still opens the file.
   Proposal: keep this reading.
3. **The priority of the UI mod.** A text mod of higher priority can still cover the civ's names, which keep the
   game's string ids. Proposal: keep the advice in `README.md` to give the UI mod the highest priority.
4. **The base civ as a requirement.** The order is in `instructions_2.md`, not in `REQUIREMENTS.md`. Proposal: a
   Requirement 10, "A civ starts as its base civ, picked first; at first Generic, with every universal node and
   nothing else."
5. **Six civs with gaps.** The Achaemenids, Athenians, Spartans, Macedonians, Thracians and Puru have no castle or
   imperial tech and no monk skin in the catalog, and some no civ or team bonus. Proposal: add them from the game's
   dat file.
6. **"Evaluated as true" (Requirement 4).** A criterion with nothing to apply to, such as captions without video,
   is ticked as met. Proposal: keep this.
7. **Reflow (1.4.10).** A step needs a window of 960 by 560, where reflow asks for 320 pixels wide at 400%.
   Proposal: lay a step out in one column below 960 pixels, with a test.
8. **The status of a node the replaced civ lacked.** Such a node is written `ResearchedCompleted`. For a Generic
   civ that replaces the Britons, four of them are `ResearchRequired` for nearly every civ that has them: the
   Bombard Tower, Carrack, Hand Cannoneer and Bombard Cannon. Proposal: a column of `tree.txt` with each node's usual
   status, read once from the game's files.
9. **Abbreviations (3.1.4).** The catalog's texts shorten words: "HP" 42 times, "vs" 41, "LOS" 17 and "TC" once,
   besides "Skirms", "stone def" and the like; two nodes are "H. Mounted Crossbowman" and "E. Varangian Guard". The
   game itself says "HP" 236 times, but "line of sight" 74 times and "LOS" twice. Proposal: spell them out in
   `data/` ("hit points", "line of sight", "against"), the two names as the game names them, with a test.

## How to test what is new

### In the application

1. Run `build.exe`, then `build\civbuilder.exe`. The title reads "A new civ - Step 1 of 9: Identity - civbuilder".
2. On "Identity", "Base civ" shows "Generic". On "Tech tree", 54 units, 29 buildings and 83 techs are ticked.
3. Pick a base civ, such as the Franks: the looks, bonuses and tree change, the steps it fills are marked done,
   and the status line says so. Ctrl+Z brings Generic back.
4. On "Civ bonuses", scroll down and tick a row: the list stays where it was, and the row shows a check.
5. The last step has no Next button.

### In the game

All of "In the game" in the sitrep of 08:06 below still holds, and none of it has been tried. In addition:

1. **The tech tree screen (Bug 6).** Open the civ's tech tree: it should show the design's tree, not the replaced
   civ's. Look at the Villager, the Town Center, the Mill and the camps, which are always there; at the Keep, which
   still needs its research; and at the Hand Cannoneer and the Bombard Cannon, which may look researched already
   (question 8).
2. **The description (Bugs 4 and 5).** The civ selection screen and the tech tree's help show the new civ's
   description, not the replaced civ's. With another text mod on, give the UI mod the highest priority (question 3).
3. **A base civ.** A mod built from the Franks, with nothing changed, should play as the Franks in the replaced slot.

### Known gaps

- Not built: custom unique units, language and voices, the hero's description, flags and overrides, the Royal
  tiers of bonuses 155 and 261, and the 22 civ bonuses listed below.
- No screen reader has been tried.

---

# Sitrep, 2026-11-09 08:06 (answered in `instructions_2.md`)

Nothing is committed and nothing is deleted. The work is in the working tree for review. All 61 tests pass in the
analyzed build (37 functional, 4 accessibility, 20 maintainability). The game has never been started with a mod
from civbuilder, so testing it in the game is the most valuable check now.

## 1. Questions to answer

Each question has a proposal. Answering "yes" means the proposal is accepted.

### Requirements (REQUIREMENTS.md holds only the template, and only you may write it)

1. Accept these as requirements? Until then, the tests cover them as proposals.
   - The civ is designed in nine steps.
   - It is saved and opened as `empireforge_v2`.
   - The mod goes into the local mods folder.
   - Every step works by keyboard alone.
   - Contrast is at least 7:1.
   - Undo and redo (Ctrl+Z, Ctrl+Y).
   - A civ has at most 24 civ bonuses.
   - The mod is two folders, `<name>-data` and `<name>-ui`.
   - The mod replaces one civ of the game.
   - Build says what it could not build.

### Rules and repository

2. **License:** keep GPL-3.0, since the catalog in `data/` comes from the GPL original? Or replace the data so the
   code can be MIT? Proposal: keep GPL-3.0.
3. **Rules from instructions.md:** add P10, Head First Design Patterns, Clean Code, "C and Win32 only" and "no
   deletion" as Rules 27–31? Proposal: yes.
4. **CI:** may one `.github/workflows/checks.yml` run `build.exe` on a Windows runner on every push? Without it,
   Rule 4 holds only partly. Proposal: yes.
5. **Settings:** write `.claude/settings.json` to deny commit, push and edits of REQUIREMENTS.md? Proposal: yes.
6. **Rule 24:** is the gap between Rules 23 and 25 intended, or is a rule missing?
7. **Line 16 of instructions.md:** are `DeleteObject` and `DestroyWindow` allowed? They free GDI objects and
   windows, not files. Proposal: allowed.
8. **Log length:** Rules 18 and 22 together cap a day at 1000 lines. DEVLOG1.md is at 990 lines, nearly all of
   them 2026-11-09. Proposal: a day may continue in the next part under a heading marked "continued", and Rule 22
   says so.
9. **This file:** with it, the root has 9 files, one over Rule 14's limit. Remove it once read, or say where it
   belongs.

### Design

10. **Themed controls:** these need a manifest, which is XML, not C. Proposal: keep the classic controls.
11. **Compressor:** the dat file is packed by deflate written in C, with fixed Huffman codes, to 13.5 MB. Proposal:
    keep it; dynamic codes would only make the file smaller.
12. **Where the mod goes:** two folders in `mods\local`, rather than a zip. Proposal: keep the folders.
13. **Version:** the civ file keeps `builder_version` "2.7.0", the original's. Proposal: keep.
14. **Wording:** "change" names one entry of an effect, where the original says "command". Proposal: keep
    "change".
15. **Campaign lines:** the names of the unique techs, the unique unit and the hero go under string ids that a few
    lines of the game's campaigns also use. Proposal: keep this, with the warning in the README to turn off the UI
    mod before playing a campaign.

## 2. How to test

### Without the game

1. In the project's folder, once: `gcc -o build.exe tools/build/*.c src/core/*.c`.
2. Run `build.exe`. It runs the static analyzer, then the 61 tests, and must end with every test passed. For a
   faster run without the analyzer, use `build.exe quick`.
3. To see what each test checks, read `documentation/TESTS.md`.

### The application

1. Run `build\civbuilder.exe`.
2. Open `tests\fixtures\example.civbuilder.json` (Ctrl+O). This is the original's example civ, "Vanguards"; it
   should load with its name, its 2 bonuses, its unique unit and its language. Or make a civ from scratch.
3. Walk the nine steps by mouse and by keyboard (Tab; arrows; Space; Ctrl+Page Down / Ctrl+Page Up; Ctrl+1 to
   Ctrl+9). Try undo and redo (Ctrl+Z, Ctrl+Y), and the menu (Alt).
4. Save it (Ctrl+S), reopen it and compare. The last step lists what the civ still needs.
5. On the last step, pick the civ of the game to replace, then press Build mod... (Ctrl+B). Read the report; it
   lists every bonus that could not be built.

### In the game (the check that has never been made)

Before you start: the game must be installed through Steam. civbuilder only reads the game's data file and never
changes it. It writes `<name>-data` and `<name>-ui` into the game's `mods\local` folder, usually
`%USERPROFILE%\Games\Age of Empires 2 DE\<number>\mods\local`.

1. **Loading:** in the mod manager, turn on `<name>-ui`. In a skirmish lobby, pick `<name>-data` as the data mod
   and the replaced civ for a player. The first risk is that the game rejects or crashes on the dat file, which
   civbuilder wrote and compressed itself. If it does, note the error and the step where it happened. That is the
   most important report.
2. **Civ selection:** the replaced civ's slot shows the new name, description and unique unit (from the
   `civilizations.json` the mod writes).
3. **Bonuses:** check a few civ bonuses with a visible effect, such as costs, starting resources or speeds. Also
   check the team bonus on an allied player. These are worth picking because they were built in the last stretch:
   - 330: farms of 2×2 tiles;
   - 339: food stored at military buildings, the Harbor and the Krepost;
   - 458: buildings can be placed on any elevation (the Town Center keeps its own rule).
4. **Unique unit and techs:** the Castle trains the chosen unique unit under its new name. The Castle Age and
   Imperial Age unique techs have their new names and change that unit.
5. **Looks:** the buildings show the chosen architecture. The Castle and the Wonder show their chosen looks, and
   Monks show the chosen monk skin.
6. **Hero:** if one was chosen, the hero trains at the Castle from the Imperial Age, under its new name.
7. **Afterwards:** turn off the UI mod before playing a campaign (question 15).

### Known gaps you will see

- 22 of the 394 civ bonuses are not built: 81, 103, 221, 222, 239, 247, 283, 286, 308, 309, 310, 332, 352, 356,
  400, 451, 454, 455, 456, 457, 459 and 460. The build's report names the ones it skipped. All 80 team bonuses are
  built.
- These are not built at all:
  - custom unique units (only the game's own units can be picked);
  - language and voices;
  - the tech tree screen, which still shows the replaced civ's tree (`civTechTrees` is not written);
  - the hero's description, flags and overrides;
  - the Royal tiers of bonuses 155 and 261.
- A bonus's multiplier in a civ file is not applied.
- For a unique unit with no entry of its own, the civ list keeps the replaced civ's `unique_unit_line`.

### Afterwards

Test mods are folders in `mods\local`. Delete them by hand when done. The scratch folders and files listed in
`documentation/DELETION_REQUESTS.md` also wait for a human to delete them.
