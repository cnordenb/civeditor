# Development log, part 3

- [2026-11-10](#2026-11-10)

Part 3 of the development log. It follows [part 2](DEVLOG2.md). The decisions made in the log are listed in
[DECISIONS.md](../DECISIONS.md).

## 2026-11-10

### 04:21:27: the instructions in a folder of their own, pictures as documents, and a night stopped by the usage limit

#### What changed

- **Why.** The task was taken up again on 2026-11-09 at 22:12, with a new deadline, 2026-11-10 08:00, and the order
  to go on until it is done. `instructions.md`, `instructions_2.md`, `sitrep.md` and the four pictures of the trial
  had been moved from the main folder into a new folder, `sitrep&instructions/`, which holds 7 files; the main folder
  holds 6. This settles question 1 of
  [the questions of part 1 answered](DEVLOG2.md#100544-the-questions-of-part-1-answered) in another way than proposed: the files
  stay in the project, in a folder of their own. ub-mode is now a skill, `.claude/skills/ub-mode/`, and it names the
  crapper, the dryer and the mutator as tools to use.
- **Pictures are documents (Test 73).** `build.exe quick` at 22:20 ran 78 tests, and one failed: "Test 73 FAILED:
  only C code and documents in the project", at `sitrep&instructions/image-1.png`. The test's list of kinds of file,
  `ALLOWED_SUFFIXES` in `tests/rules/repo_rules.c`, had no pictures. `.png` was added to it at 22:27, and all 78
  tests passed. A picture is not code in any language, so Rule 29 holds; the pictures are the evidence of the trial.
- **A skip that nothing reached.** `repo_placeholders_are_filled` passed over `instructions.md` by its old place in
  the main folder, where no file is any more. The skip was taken out; the test reads the file where it is now and
  passes.
- **Parallel streams, tried and stopped.** At 22:30 what is left of the task was started as six streams side by
  side, each in a copy of the working tree in the session's scratchpad, so that an unfinished change of one could not
  break the build of another: the 22 civ bonuses that Build skips with the multiplier and questions 5, 8 and 9 of
  `sitrep.md`; the hero's description, flags and overrides with language and custom unique units; the criteria of the
  accessibility checklist that need code; tests that drive the real window; the crapper and the mutator; and the
  dryer. Three readings of the project were started beside them: the original's features that civbuilder lacks, Bugs
  4 to 6 of the trial, and the rules.
- **The obstacle.** The account's usage limit of five hours was reached at 22:35, five minutes after the streams
  began. It was lifted at 23:10, the streams began anew, and it was reached again at 23:24. Nothing could be done
  from then until 04:10. At 04:11 the streams were stopped, as ordered, and the work went on in the working tree
  alone, one change at a time. The streams left next to nothing: one of them had changed 32 lines of
  `src/win/app_window.c` in its copy, unfinished and not taken over, and no reading had written down what it found.
- **Documents.** `AGENTS.md` and `MAINTAINER.md`: the instructions and the sitrep are in `sitrep&instructions/`.
  `TESTS.md`: Test 73 allows pictures. `DELETION_REQUESTS.md`: the copies and scratch folders of the night.
  `DECISIONS.md`.

#### Not done

- Everything the streams were to do but the crapper, which the next section gives: the 22 civ bonuses and the Royal
  tiers, a bonus's multiplier, the hero's description, flags and overrides, language and voices, custom unique units,
  the proposals of questions 5, 7, 8 and 9 of `sitrep.md`, every unticked box of the accessibility checklist, the
  dryer and the mutator.
- Bugs 4 to 6 were not looked at again, and the mod was not tried in the game.
- The three readings gave no result that could be kept.

#### Questions

1. **Pictures in Rule 25.** Rule 25 names `*.md`, `*.json` and `*.txt` as not code. The pictures of the trial are in
   the project now, and Test 73 lets `.png` pass as a document. Proposal: add `*.png` to Rule 25.
2. **Work side by side.** One period of the usage limit carried about fourteen minutes of ten pieces of work running
   at once, and two periods were spent before any of it was finished. Proposal: the work is done one change at a
   time, and streams side by side are used again only when the limit is known to allow them.
3. **The questions of `sitrep.md`.** Questions 2 to 9 of 2026-11-09 10:51 are still open, with their proposals; none
   of them was carried out in the night.

### 04:22:28: the crapper in C, and 369 functions at CRAP 10 or above

#### What changed

- **Why.** Rule 32 asks that every function's CRAP score be below 10, and nothing measured it: ub-mode's crapper
  reads no C. The answer to question 3 of part 1 allows the tools to be written in C, and the skill says to use the
  crapper.
- **The crapper.** `build\crapper.exe`, which `build.exe` makes from the new folder `tools/crap/`, scores every
  function of `src/`, `tests/` and `tools/`. `cover_build.c` compiles the test program once more into `build/cover/`
  with gcc's `--coverage`, empties the counts of the run before (each `.gcda` file is written anew with nothing in
  it, since nothing is deleted) and runs the tests. `crap_cover.c` asks `gcov -b -f` for each source file and reads,
  function by function, how many branches were taken at least once; for a function without branches, how many lines
  ran. `crap_score.c` counts a function's complexity, 1 and one more for each `if`, `for`, `while`, `case`, `&&`,
  `||` and `?` of its body, in the text that the scanner of the maintainability tests has cleaned of strings and
  comments, and works out the score in whole thousandths. `crapper.c` writes every function to `build/crap.txt`,
  the worst first, prints those at 10 or above with their count, and ends with 1 when there is one. A function that
  the test program does not hold, all of `src/win/` and of `tools/` but the two files below, counts as not run.
- **gcov is gcc's.** It comes with gcc from MinGW-w64, the project's compiler, so no dependency is added (Rule 29).
- **One list of folders, one rule for what the tests hold.** `source_list_collect_all` and `source_in_tests` in
  `tools/build/source_list.c` say which folders hold code (`src`, `tests`, `tools`) and which files make the test
  program; `build_steps.c` and the crapper both ask them (Rule 31). The test program now holds `crap_score.c` and
  `crap_cover.c`, the files of `tools/crap/` whose names begin with `crap_`.
- **Tests.** 80, up from 78. Test 79, "the CRAP score is complexity squared times the uncovered share cubed, plus
  complexity", and Test 80, "the complexity of a function counts its branches", in a new group, `run_tool_test`, of
  `maintainability_suite.c`.
- **Checked.** `build.exe`, with the analyzer: no warning, no finding, 80 tests passed. The analyzer first found a
  file left open on an early return in `cover_build.c` and in `crapper.c`; both were mended. Five mutants of
  `crap_score.c` were made one at a time and undone: the cube made a square lived, since Test 79 then asked for 1
  branch of 2, where the square and the cube are the same; the test now asks for 1 of 3 and kills it. `+` made
  `-` was killed by Test 79; a `!` taken away and 1 made 0 by Test 80; `||` made `&&` did not compile.
- **Measured.** 847 functions, in 17 seconds; 369 are at 10 or above, 285 of the 604 in `src/`. By folder:
  `src/win` 72, `src/forge` 67, `tools/build` 28, `src/data` 27, `src/mod` 22, `tools/crap` 22, `src/present` 21,
  `src/json` 12, `tests/maintainability` 12, `src/dat` 11, `src/look` 9, `tests/scan` 9, `src/core` 8, `src/model`
  7, `tests/rules` 7, `src/unique` 6, `src/pack` 6, `tests/functional` 4, `src/hero`, `src/view`, `src/bonus` and
  `src/techtree` 3 each, `src/civlist` and `src/deflate` 2 each, `src/persist`, `tests/dat` and `tests/runner.c` 1
  each. The worst are `describe_build` in `dialogs.c`, 552 (complexity 23, not run), `window_procedure`, 380,
  `recipe_copy_unique_effect`, 342 (complexity 18, none of 18 branches taken), `sync_on_notify`, 342, and
  `tsv_hundredths`, `tsv_decimal` and `unit_in_effect`, 306 each.
- **What the numbers say.** Of the 369, 276 are functions of which no test takes a branch, and 93 are run in part.
  A function that no test runs scores its complexity squared plus its complexity, so it passes only with a
  complexity of 2 or less; the two checks and the doubled bound of a loop that P10 asks for are branches too, so a
  function with three checks and one loop already has a complexity of 9. A function that is run in full passes up
  to a complexity of 9; 115 functions have 10 or more.
- **Documents.** `TESTS.md`: Tests 79 and 80, and the lines of the 20 tests above them in
  `maintainability_suite.c`, one lower for its new `#include`. `MAINTAINER.md`: `tools/crap/`, how to run the
  crapper and read `build/crap.txt`, and Rule 32 among the rules for changing the code. `TERMINOLOGY.md`:
  complexity, CRAP score and crapper. `AGENTS.md`: the crapper among the commands. `DECISIONS.md`.

#### Not done

- Rule 32 does not hold: 369 functions are at 10 or above, and none was brought down.
- The dryer and the mutator are not written, so Rules 31 and 34 are still not measured.
- `build.exe` makes the crapper but does not run it, since it would fail every build today.
- Tests 79 and 80 were written after the code they guard, not before it (Rule 33); they were then shown to fail
  on the mutants above.
- No test runs the crapper's own reading of gcov or its coverage build; `crap_cover_take_line` could have one.

#### Questions

1. **Code that no test runs.** `src/win/`, the only Win32 code, is outside the test program, and so are the tools:
   122 of the 369 functions are theirs. They pass Rule 32 only if tests run them or if each is cut down to a
   complexity of 2. Proposal: the 20 tests still free go first to tests that drive the real window without showing
   it, which the accessibility checklist needs as well.
2. **Checks as branches.** The crapper counts the branches of `CHECK` like any other, since a check that fails takes
   another way through the function; no test makes a check fail, so such a branch is never taken. Proposal: keep
   counting them, and bring a function down by splitting it.
3. **A gate.** Should `build.exe` run the crapper and fail when a function is at 10 or above? Proposal: yes, from
   the day the count is 0; until then it is run by hand.

### 04:27:23: the dryer in C, and 62 pairs of functions alike

#### What changed

- **Why.** Rule 31 asks that two functions whose normalised forms are 80% alike or more be made one, and nothing
  measured it: ub-mode's dryer reads no C.
- **The dryer.** `build\dryer.exe`, which `build.exe` makes from the new folder `tools/dry/`. `dry_shape.c` takes the
  shape of a function from its body, as the scanner of the maintainability tests has cleaned it: a word followed by
  `(` keeps its name, so a call stays what it calls and `if`, `for`, `while` and `switch` stay themselves; `return`,
  `else`, `break`, `continue`, `case` and `default` keep theirs; every other word is a name, whatever it is called;
  a word that begins with a digit is a literal; every other character stands for itself. The shape is the set of
  all runs of four such tokens, each kept once as a number. Two functions are as alike as the runs they share
  among all the runs of either, in thousandths. `dryer.c` compares every two functions of `src/`, `tests/` and
  `tools/` that have 4 lines and 20 tokens or more, prints the pairs at 0.8 or above with their count, writes those
  at 0.6 or above to `build/dry.txt`, and ends with 1 when a pair is at 0.8 or above.
- **Where it differs from ub-mode's dryer.** That one normalises a syntax tree and compares its subtrees; this one
  has no tree, and runs of four tokens stand for the subtrees. Its smallest function, 4 lines, is kept, and its 20
  nodes are 20 tokens. The limit is Rule 31's 0.8, not its 0.82. The tests are compared too, which it leaves out,
  since Rule 31 speaks of any two functions.
- **The build.** `shared_by_tools` in `build_steps.c` names what the crapper and the dryer both link: `src/core/`,
  `tests/scan/` and `source_list.c`. The test program holds `dry_shape.c`, by the rule `source_in_tests`.
- **Tests.** 81, up from 80. Test 81, "two functions that differ only in their names are alike", in
  `run_tool_test` of `maintainability_suite.c`.
- **Checked.** `build.exe`, with the analyzer: no warning, no finding, 81 tests passed. Six mutants of
  `dry_shape.c` were made one at a time and undone. Test 81 killed four: a name and a call's name swapped, `>=`
  made `>` for a literal, `-` made `+` and `==` made `!=` in the count of what is shared. Two lived: `<=` made
  `<` in the sorting of the runs, which sorts the same, and `!=` made `==` where repeated runs are dropped, after
  which the two functions of the test are still alike.
- **Measured.** 849 functions, in a third of a second: 62 pairs at 0.8 or above, 28 of them at 1.000; 48 pairs lie
  within `src/`, 8 within `tests/`, and 6 cross between `src/`, `tests/` and `tools/`. The largest families: the six
  `catalog_find_...` of `catalog_lookup.c`, 15 pairs; `dat_edit_patch_i16`, `_i32` and `_f32`, and `dat_edit_patch`
  with `dat_edit_copy`; `json_begin_object` and `json_end_object` with their array twins; `history_undo` with
  `history_redo` and `history_can_undo` with `history_can_redo`; `text_equal` with `text_starts_with`;
  `recipe_read_tech` with `recipe_read_effect`; `sync_all` with `sync_model`; and three tests of the civ list in
  `mod_tests.c`.
- **Documents.** `TESTS.md`: Test 81, and the lines of the tests above it in `maintainability_suite.c`.
  `MAINTAINER.md`: `tools/dry/`, and Rule 31 among the rules for changing the code. `TERMINOLOGY.md`: shape and
  dryer. `AGENTS.md`: the dryer among the commands. `sitrep.md`. `DECISIONS.md`.

#### Not done

- Rule 31 does not hold: none of the 62 pairs was made one.
- The mutator is not written, so Rule 34 is still kept by hand.
- `build.exe` makes the dryer but does not run it, since it would fail every build today.
- The mutant that lived in the dropping of repeated runs has no test; Test 81 was written after the code (Rule 33).
- The pairs are printed in the order of the files, not the most alike first.

#### Questions

1. **Small functions alike by their form.** Some pairs are alike only because P10's form is: two checks and one
   return, such as `layout_spec` with `palette_color`, or `is_building_class` with `needs_label` of a test. Are
   they one decision in two places? Proposal: keep the smallest function at 4 lines and 20 tokens, as ub-mode has
   it, make one of each pair that says the same thing, and answer here for the pairs that only look the same.
2. **A gate.** Proposal: `build.exe` runs the dryer and fails on it from the day no pair is left, as proposed for
   the crapper.

### 04:37:03: the mutator in C, tried on one folder

#### What changed

- **Why.** Rule 34 asks that every mutant of the code the tests run be killed by a test. Until now mutants were
  made by hand, a few for each change; ub-mode's mutator reads no C.
- **The mutator.** `build\mutator.exe`, which `build.exe` makes from the new folder `tools/mutate/`. It is given a
  folder or a file of `src/`, such as `build\mutator.exe src/model/`, or nothing for all of `src/`.
  `mutant_sites.c` finds each place of a function's body where a mutant can be made, in the text that the scanner
  has cleaned of strings and comments: `==` and `!=`, `&&` and `||`, `<` and `<=`, `>` and `>=` swapped; `+` and
  `-` swapped between two operands; a `!` or a unary `-` taken away; `true` and `false`, `0` and `1` swapped.
  `<<`, `>>`, `++`, `--`, `+=`, `-=` and `->` are passed over. `mutator.c` first builds and runs the tests with
  coverage, as the crapper does, and stops if a test fails. Then it starts itself twelve times as workers, and each
  worker takes every twelfth place. A place on a line that no test ran is listed and no mutant is made of it. For
  any other, `mutation_run.c` writes the file with that one place changed to `build/mutate/w<n>/mutant.c`, over
  the mutant before it, compiles it without warnings, links the test program of the last build with it in place of
  the file's own object, and runs the tests, ending them after 30 seconds. Each mutant is one line of
  `build/mutants.txt`: `K` killed, `S` survived, `N` not compiled, `T` ended by the time limit, `U` on a line no
  test runs. The survivors are printed with the counts, and the mutator ends with 1 when a mutant survived.
- **Where it differs from Rule 34's list.** `>` and `>=` are swapped too, as the mutants made by hand were. Only the
  files of `src/` that the test program holds are mutated, so not `src/win/`, the tests or the tools.
- **Shared, not written twice (Rule 31).** `source_span_is` in `tests/scan/source_scan.c` compares a word of a text
  for the dryer and the mutator; `open_gcov` in `crap_cover.c` starts gcov for the crapper's reading of functions
  and the mutator's reading of lines, `crap_cover_lines`; `cover_build_tests_passed` says whether the tests passed.
- **Line numbers from 1.** The crapper and the dryer printed the lines of a function counted from 0, as the scanner
  counts them; they now print them from 1, as an editor shows them.
- **Tests.** 82, up from 81. Test 82, "the mutants of a line change each operator and constant once", in
  `run_tool_test` of `maintainability_suite.c`.
- **Checked.** `build.exe`, with the analyzer: no warning, no finding, 82 tests passed. Six mutants of
  `mutant_sites.c` were made by hand, since the mutator mutates `src/` only: Test 82 killed a pair swapped for
  itself and `+=` made `-=` where the search moves on; two did not compile under the build's warnings; `||` made
  `&&` where a passed pair is told from a single sign lived, until the line of the test was given an arrow,
  `item->count`, and then Test 82 killed it.
- **Tried.** `build\mutator.exe src/base/`: 53 mutants in 30 seconds, the coverage build among them; 35 killed, 18
  survived, and 3 more places were on lines no test runs. Among the survivors are the second bound of a loop,
  `<` made `<=`, and six mutants of line 53 of `base_civ.c`.
- **Documents.** `TESTS.md`: Test 82 and the lines of the tests above it in `maintainability_suite.c`.
  `MAINTAINER.md`: `tools/mutate/`, and how to run the mutator, in place of "No tool does this yet".
  `TERMINOLOGY.md`: mutant and mutator. `AGENTS.md`. `sitrep.md`. `DECISIONS.md`.

#### Not done

- Rule 34 does not hold: mutants survive, and none was killed by a new test.
- The whole of `src/` was started and had not ended when this was written; a later section gives its counts.
- No test runs `mutation_run.c` or `mutator.c`, which start other programs; Test 82 was written after the code.
- The tests and the tools themselves are not mutated.

#### Questions

1. **Mutants no test can kill.** P10 asks for two checks in a function and a second bound in a loop. While the
   checks hold, `!CHECK(a) || !CHECK(b)` goes the same way with `&&`, and `index < CAPACITY` with `<=`; no test of
   behaviour tells them apart. Proposal: such mutants are listed apart as "of a check or a bound" once the mutator
   can tell them, and Rule 34 is read as about the others.
2. **Rule 5 and Rule 34.** 18 tests are free of the 100, each of one check, and the mutants that survive are many
   more than 18. Proposal: decide which gives way; until then the tests go where a mutant shows behaviour that no
   test holds, as on line 53 of `base_civ.c`.

### 04:57:44: the mutants of all of src/ counted

#### What changed

- **Measured.** `build\mutator.exe`, with no folder named, from 04:36 to 04:56: 4 370 mutants were made of the lines
  of `src/` that the tests run. 2 324 were killed, 2 032 survived, 8 were ended by the limit of 30 seconds and 6
  did not compile; 2 524 more places are on lines that no test runs. No mutant was made of `src/win/`.
- **By folder,** killed and survived, then the places on lines not run: `base` 35 and 18, 3; `bonus` 55 and 49,
  19; `civlist` 61 and 38, 2; `core` 115 and 80, 112; `dat` 518 and 303, 121; `data` 31 and 87, 455; `deflate` 106
  and 106, 7; `forge` 102 and 106, 877; `hero` 55 and 41, 21; `json` 294 and 205, 111; `look` 106 and 123, 74;
  `mod` 168 and 244, 192; `model` 178 and 160, 88; `pack` 190 and 115, 34; `persist` 94 and 100, 17; `present` 38
  and 86, 196; `techtree` 35 and 16, 23; `unique` 16 and 6, 96; `view` 127 and 149, 76.
- **By kind,** the survivors: `||` made `&&` 549, `<` made `<=` 282, `0` made `1` 272, `&&` made `||` 204, `>=`
  made `>` 122, a `!` taken away 109, `!=` made `==` 93, `<=` made `<` 90, `>` made `>=` 75, `+` made `-` 61,
  `-` made `+` 55, `==` made `!=` 49, `1` made `0` 36, `false` made `true` 19, `true` made `false` 12, a unary
  `-` taken away 4. The files with most are `mod_text.c` 135, `look_copy.c` 123, `civ_design.c` 114, `layout.c`
  113, `dat_edit.c` 111 and `mod_strings.c` 109.
- **What the numbers say.** A little more than half of the mutants of tested lines are killed. The largest kind
  of survivor, `||` made `&&`, is what joins two checks, or a check and a condition, at the head of a function;
  the second, `<` made `<=`, is the bound of a loop. `src/forge/`, which builds the mod, has 877 places on lines
  that no test runs against 208 mutants on lines that one does, and `src/data/` 455 against 118.
- **Documents.** `sitrep.md`.

#### Not done

- No survivor was killed and no line that no test runs was given a test.
- The survivors were counted by kind, not read one by one; how many are of a check or a bound is not known.

### 05:01:56: two boxes of the accessibility checklist ticked, 48 of 86

#### What changed

- **Why.** Requirements 3 and 4: the checklist in `.github/ACCESSIBILITY.md` had 46 of 86 boxes ticked. Two of the
  unticked ones could be settled by the code that the tests already measure, without a window.
- **3.2.3 Consistent Navigation.** A new test, "the list of steps is on every step, with Back and Next wherever
  they lead": on each of the nine steps `layout_shows` shows the list of steps, Back when a step lies before and
  Next when one lies after. The places themselves cannot differ between steps, since `layout_box` places a control
  from the size of the window alone.
- **1.4.11 Non-text Contrast.** No new test: the test "every text contrasts at least 7:1 with its background"
  goes through every pair of colors of `palette.c`, and `controls_draw_item` in `controls.c` draws the frame of a
  box, the check and the focus frame in those pairs only: ink on a field, the chosen ink on the chosen color, and
  the focus color on a field or on paper. Each is above 7:1, where 3:1 is asked.
- **Tests.** 83, up from 82. The new one is Test 59, the sixth accessibility test (`access_tests.c`,
  `accessibility_suite.c`); the maintainability tests are now Tests 60 to 83.
- **Checked.** `build.exe`, with the analyzer: no warning, no finding, 83 tests passed. The test was written
  before any change of the code, and no change was needed: the code already did what the criterion asks, so the
  test was not seen to fail first. It was shown honest with the mutator, `build\mutator.exe src/view/layout.c`:
  all 13 mutants of lines 79 to 83, the conditions of `layout_shows`, were killed, and of the mutants of
  `layout.c` 104 survive where 113 did before.
- **Documents.** `.github/ACCESSIBILITY.md`: 1.4.11 and 3.2.3 ticked, 48 of 86. `TESTS.md`: Test 59, and the
  numbers of Tests 60 to 83. `sitrep.md`. `DECISIONS.md`.

#### Not done

- 38 boxes are unticked. Most need tests that drive the real window and read what a screen reader reads; the
  criteria known not to be met (1.4.4, 1.4.10, 2.4.13, 3.1.1, 3.1.3, 3.1.4, 3.3.5, 4.1.3) are as they were.
- Four mutants of the two checks of `layout_shows`, line 76, survive.

### 05:03:39: the window's texts read against the checklist, 55 of 86

#### What changed

- **Why.** Seven unticked boxes of `.github/ACCESSIBILITY.md` asked for a reading of the window's texts, not for a
  screen reader: 1.3.3, 1.3.5, 1.4.12, 2.4.6, 2.4.10, 3.1.6 and 3.2.4.
- **The reading.** All 144 texts that `src/view/`, `src/present/` and `src/win/` can show were listed and read:
  the nine titles and their help lines, the 36 labels, the menu, the status messages, the message boxes and the
  words of the review. None points at a shape, a color, a place or a sound (1.3.3); no field asks for a fact about
  the user (1.3.5); no word's meaning depends on how it is said (3.1.6); every step has its title (2.4.10).
- **Three names mended.** The menu Go said "Previous step" where the button says "Back": the menu now says "Next"
  and "Back", as the buttons do (3.2.4, `app_window.c`). Two labels leaned on the label before them: "Its
  description" is now "Description of the unique unit" and "Its base unit" is "The selected unit", as "The
  selected team bonus" is (2.4.6, `SPECS` in `layout.c`).
- **1.4.12 Text Spacing.** The criterion speaks of content made in a markup language that lets the spacing of text
  be set; the window is made of the controls of Windows, so it has nothing to apply to.
- **A third way to tick a box.** Besides "Test" and "Code", the checklist now has "Read": every text the window
  can show was read on the day named. A text added later must be read in the same way.
- **Tests.** None was added; the tests of the layout passed with the longer label.
- **Checked.** `build.exe`, with the analyzer: no warning, no finding, 83 tests passed.
- **Documents.** `.github/ACCESSIBILITY.md`: seven boxes ticked, 55 of 86. `sitrep.md`. `DECISIONS.md`.

#### Not done

- 31 boxes are unticked: the eight criteria known not to be met, and those that need the keyboard and a screen
  reader, or tests that drive the real window.
- The 394 descriptions of the catalog's bonuses were not read; 3.1.5 Reading Level stays unticked.
- The changed names were not looked at in the running window.

#### Questions

1. **"Read" as evidence.** Requirement 4 asks that every item be evaluated as true. Proposal: a reading of every
   text on a named day counts, as written into the checklist, until a test can take its place.

### 05:09:30: a test that drives the real window, and the log's third part

#### What changed

- **Why.** No test ran `src/win/`, the only Win32 code: all 72 of its functions scored 10 or above with the
  crapper, and most unticked boxes of the accessibility checklist waited for a check "by hand". Question 1 of
  [the crapper's section](#042228-the-crapper-in-c-and-369-functions-at-crap-10-or-above) proposed tests that drive
  the real window without showing it; this is the first.
- **A window that is made but not shown.** `app_open` in `app_window.c` makes the window with its controls, its
  catalog and its first step, and returns it; `app_run` calls it, then shows the window and runs the messages, as
  before. A test calls `app_open` alone: nothing appears on the screen, and the keyboard and the mouse are left
  alone.
- **The test program holds the window's code.** `source_in_tests` in `source_list.c` takes all of `src/`, and
  `source_test_libraries` names the four libraries of Windows that the window needs; the build, the crapper's
  coverage build and, through the build's list of objects, the mutator all link them.
- **The test.** "The Tab key reaches every control of every step" (`tests/accessibility/window_tests.c`): it opens
  the window, goes to each of the nine steps by the command of the menu Go, and asks Windows for the Tab order
  (`GetNextDlgTabItem`, what the Tab key itself follows); every control that the layout shows on the step and that
  one acts on must be in it. Then it destroys the window.
- **Tests.** 84, up from 83. The new one is Test 60, the seventh accessibility test; the maintainability tests are
  now Tests 61 to 84.
- **Checked.** `build.exe`, with the analyzer: no warning, no finding, 84 tests passed; the tests still run in 1.6
  seconds. The test passed at once, since the controls had their tab stops; it was shown honest by taking the tab
  stop from the buttons, where it failed at "Next >", and from the text fields, where it failed at "Name"; both
  were undone. The crapper, run again: 909 functions, 342 at 10 or above, and 36 of the 73 functions of
  `src/win/`, where all were before; 225 functions are still not run by any test.
- **The checklist.** 2.1.1 Keyboard and 2.1.3 Keyboard (No Exception) are ticked: 57 of 86.
- **The log's third part.** The day 2026-11-10 was moved whole from `DEVLOG2.md` to this part, `DEVLOG3.md`, before
  part 2 reached 1 000 lines (Rules 18 and 22); the links of `DECISIONS.md` to its sections follow it.
- **Documents.** `TESTS.md`: Test 60 and the numbers of Tests 61 to 84. `.github/ACCESSIBILITY.md`. `MAINTAINER.md`:
  the tests of the window. `sitrep.md`. `DECISIONS.md`.

#### Not done

- The test does not press keys: it does not show that Tab leaves a field of several lines (2.1.2), in what order
  the controls come (2.4.3), or what a screen reader is told (4.1.2).
- Bugs 1 and 2 of the trial still have no test; with the window in the test program they can have one.
- 16 tests are free of the 100.

#### Questions

1. **A window in the tests.** The tests now make a real window, hidden, on the machine that runs them; on a
   machine without a desktop the test would fail. Proposal: keep it, since the tests are run by hand before a push.

### 05:13:51: two more tests of the real window, 62 of 86

#### What changed

- **Why.** With the window in the test program, the criteria about where the user is and in what order the
  keyboard goes can be checked by tests instead of by hand (Requirement 3).
- **The tests.** "The list of steps shows the current step as the chosen one": after the command of each of the
  nine steps, the row of that step is the chosen row of the list (2.4.8). "The Tab key follows the order in which
  a step is read": on each step the Tab order goes from the list of steps down each column of the layout, from the
  left, and then to Back and Next; the place of each control is taken from `layout_box` (2.4.3, 1.3.2).
- **A window opened more than once.** `register_class` in `app_window.c` takes a window class that is already
  there as made, so each test opens and destroys a window of its own; `close_window` in `window_tests.c` destroys
  it and empties the messages it leaves.
- **Tests.** 86, up from 84: Tests 61 and 62, the eighth and ninth accessibility tests; the maintainability tests
  are now Tests 63 to 86.
- **Checked.** `build.exe`, with the analyzer: no warning, no finding, 86 tests passed. Both tests passed at once,
  the code being right already; each was shown to fail on a fault of the kind it guards, made and undone: with the
  controls of a step created in the opposite order, Test 62 failed; with the step never changed, Tests 60 and 61
  failed. The crapper and the dryer, run again: 914 functions, 342 at 10 or above, 225 not run at all; 64 pairs
  of functions alike, two more than before, from the night's own code.
- **The checklist.** Ticked: 1.3.2, 2.4.1, 2.4.3, 2.4.5 and 2.4.8, by these tests and the code they name. 62 of 86.
- **Documents.** `TESTS.md`: Tests 61 and 62, the numbers of Tests 63 to 86, and a heading that the change before
  had joined to the line after it. `.github/ACCESSIBILITY.md`. `sitrep.md`.

#### Not done

- The two pairs of alike functions that the night's code added were not made one.
- 24 boxes are unticked, and 14 tests are free of the 100.

### 05:16:33: a test that catches Bug 1 again

#### What changed

- **Why.** Bug 1 of the trial, a click on a row that scrolled the list of bonuses to its top, was mended on
  2026-11-09 without a test, since no test ran the window (Rule 24 allows a test for a bug found before).
- **The test.** "A ticked row leaves the list of bonuses where it was scrolled" (`window_tests.c`): in the real
  window, not shown, the list of civ bonuses is scrolled to row 30, row 34 is ticked as a click ticks it, and row
  30 must still be at the top.
- **Tests.** 87, up from 86. Test 63, the tenth test of the accessibility suite, where the tests of the window
  are; the maintainability tests are now Tests 64 to 87.
- **Checked.** `build.exe`, with the analyzer: no warning, no finding, 87 tests passed. With `tick_rows` in
  `sync.c` putting the list back at its top row 0, made and undone, Test 63 failed. With the putting back taken
  out altogether, Test 63 still passed: the list stays by itself now that only the rows whose tick changed are set
  again, which is the other half of the mending.
- **Documents.** `TESTS.md`: Test 63, the numbers of Tests 64 to 87, and the lines of Tests 60 to 62 in
  `window_tests.c`. `sitrep.md`. `DECISIONS.md`.

#### Not done

- Bug 2, the check of a ticked box, still has no test: a test would have to judge a drawing by its pixels.
- The test does not send a click of the mouse; it does to the list what a click does.
- 13 tests are free of the 100.

### 05:21:07: the names a screen reader is told, asked of Windows, 66 of 86

#### What changed

- **Why.** 4.1.2, 1.3.1 and 2.5.3 of the checklist waited for a screen reader. What a screen reader is told can be
  asked of Windows directly: its accessibility interface (MSAA, `oleacc`) gives each control a name.
- **The test.** "Every control tells a screen reader a name that begins with its label" (`window_tests.c`): on each
  step of the real window, not shown, `AccessibleObjectFromWindow` is asked for each control one acts on, and the
  name it gives must begin with the control's label of `layout.c`.
- **Obstacle: a wrong interface id.** The call first answered "no such interface" for every control. The id of
  `IAccessible` that the compiler's library gives, `IID_IAccessible`, begins 617a25ff, where the interface's id
  begins 618736e0. The test holds the right id itself.
- **The build.** The code is compiled with `COBJMACROS` defined, so a call of an interface is one macro and the
  code keeps one level of dereferencing; the flag is in the build's, the crapper's and the mutator's command. The
  test program links `oleacc`, `ole32` and `oleaut32` as well (`source_test_libraries`).
- **Tests.** 88, up from 87. Test 64, the eleventh of the accessibility suite; the maintainability tests are now
  Tests 65 to 88.
- **Checked.** `build.exe`, with the analyzer: no warning, no finding, 88 tests passed. With the labels made
  without their text, made and undone, Test 64 failed at "Steps"; with the buttons left without theirs, at
  "Next >".
- **The checklist.** Ticked: 1.3.1, 2.5.3 and 4.1.2 by the test, and 1.3.6, which is about content made in a
  markup language. 66 of 86.
- **Documents.** `TESTS.md`: Test 64, the numbers of Tests 65 to 88, and the lines of Tests 60 to 63.
  `.github/ACCESSIBILITY.md`. `sitrep.md`. `DECISIONS.md`.

#### Not done

- No screen reader was tried; the test asks the interface a screen reader asks, not the screen reader.
- The roles, values and states are Windows' own and are not tested; 1.1.1 stays unticked until a test asks for the
  state of a ticked row.
- The compile flags are written in three places: `build_steps.c`, `cover_build.c` and `mutation_run.c` (Rule 31).
- 12 tests are free of the 100; 20 boxes are unticked.

#### Questions

1. **The flags in three places.** The build, the crapper and the mutator each hold their own list of compile flags.
   Proposal: one list in `source_list.c`, as the libraries have, when the tools are next changed.

### 05:37:00: a keyboard trap found by a test and mended, 71 of 86

#### What changed

- **Why.** 2.1.2 No Keyboard Trap, 3.2.1 On Focus and 1.1.1 Non-text Content of the checklist waited for a check
  by hand; the real window in the test program can answer them.
- **A fault found: Tab did not leave a field of several lines.** The test "the Tab key leaves every control" puts
  the focus on each control of each step in turn and gives a Tab key to the dialog manager, as the program's own
  loop of messages does. It failed at "Description": a field of several lines keeps the Tab key and asks its
  parent to move the focus on (`WM_NEXTDLGCTL`), which only a dialog of Windows answers by itself; civbuilder's
  window is no such dialog, so the focus stayed, in "Description" and in "Description of the unique unit".
- **The mending.** `move_focus` in `app_window.c` answers that request: the focus goes to the next control of the
  Tab order, or to the one before with Shift, or to the control the request names. The test then passed.
- **Two more tests.** "A ticked row is told to a screen reader as chosen": after a row of the bonuses is ticked,
  Windows' accessibility interface gives that row the state "selected" and another row not, so the check that is
  drawn has its words (1.1.1). "Moving the focus through every control changes nothing": after the focus was on
  every control of every step, undo still answers "There is nothing to undo." (3.2.1).
- **Tests.** 91, up from 88: Tests 65, 66 and 67 of the accessibility suite; the maintainability tests are now
  Tests 68 to 91.
- **Checked.** `build.exe`, with the analyzer: no warning, no finding, 91 tests passed. Test 67 failed first, at
  "Description", for the reason expected, and passed with `move_focus`; with the request read the wrong way round
  (`!=` made `==`), it failed again. Test 65 failed when a ticked row no longer reached the design (`!=` made
  `==` in `on_checklist_change`). Test 66 failed when a text field took getting the focus for a change (`==`
  made `!=` for `EN_CHANGE`); two other faults of that kind, for a choice and for a list, changed nothing in the
  design and so were not caught, rightly.
- **The checklist.** Ticked: 1.1.1, 2.1.2 and 3.2.1 by these tests; 3.3.1 and 3.3.3 by the test "the summary
  names what is still needed" and the reading of the texts. 71 of 86.
- **A slip.** One fault made to try Test 66 let the list of steps call Save when it got the focus: the test
  program opened a Save dialog on the desktop and waited in it, for about ten minutes, until the run was stopped
  at 05:36. Nothing was saved, and the file was put back as it was. A fault made on purpose must not reach a
  command that opens a dialog.
- **Documents.** `TESTS.md`: Tests 65 to 67 and the numbers of Tests 68 to 91. `.github/ACCESSIBILITY.md`.
  `sitrep.md`. `DECISIONS.md`.

#### Not done

- The fields were not tried with the keyboard by hand, nor with Shift+Tab in a test.
- 15 boxes are unticked: 1.4.1, 1.4.4, 1.4.8, 1.4.10, 2.4.7, 2.4.13, 2.5.2, 3.1.1, 3.1.3, 3.1.4, 3.1.5, 3.2.2,
  3.2.5, 3.3.5 and 4.1.3. Nine tests are free of the 100.

### 05:41:23: the night's own code made dry

#### What changed

- **Why.** The dryer, run on the whole project, ranked functions of the night's own code as alike (Rule 31): three
  loops over the controls of a step in `window_tests.c`, at 0.84 to 0.93, the tests that used them, and
  `name_start` in `mutation_run.c` with `repo_path_name_start` in `tests/scan/repo_files.c`, at 0.82.
- **One loop, three questions.** `every_control_passes` in `window_tests.c` opens the window, goes through every
  control of every step that one acts on, and asks one question of it, a `ControlQuestion`: does the Tab key reach
  it, is it named by its label, does the Tab key leave it. `control_passes` answers with one `switch`, as the
  project builds a choice among kinds. `go_to` sends the command of a step and `tick_a_bonus` ticks a row for the
  two tests that need one.
- **One finder of a file's name.** `mutation_run.c` asks `repo_path_name_start` and holds no copy of it.
- **Checked.** The behaviour was pinned before and shown unchanged after (Rule 36): `build.exe`, with the analyzer,
  no warning, no finding, 91 tests passed; and the four faults that Tests 60, 63, 64 and 67 were shown to catch
  were made again, one at a time, and undone: each still failed its test, at "Next >", at the list's top row, at
  "Steps" and at "Description". The dryer: 63 pairs at 0.8 or above, where 68 were, and none of them in
  `window_tests.c` or `tools/mutate/`. The crapper: 926 functions, 333 at 10 or above.
- **Documents.** `TESTS.md`: the lines of Tests 60 to 67 in `window_tests.c`.

#### Not done

- 63 pairs are left, 62 of them older than the night or alike only by P10's form.
- The scanner of the maintainability tests does not find the name of a function that returns a pointer, such as
  `reader_of`: the crapper and the dryer list it without a name.

### 05:43:36: Windows told that the window speaks English, 72 of 86

#### What changed

- **Why.** 3.1.1 Language of Page was not met: every text is English, but the program did not say so to Windows,
  and the usual way, a resource of the program, is not C (Rule 29).
- **Said in C.** `app_open` in `app_window.c` calls `SetProcessPreferredUILanguages` with `en-US` before the window
  is made, so a program that asks Windows which language this process shows its texts in is told English.
- **Tests.** 92, up from 91. Test 68, "Windows is told that the window's language is English"
  (`window_tests.c`); the maintainability tests are now Tests 69 to 92.
- **Checked.** The test was written first and failed, since the process then preferred no language of its own;
  with the call it passed. `build.exe`, with the analyzer: no warning, no finding, 92 tests passed.
- **The checklist.** 3.1.1 ticked: 72 of 86.
- **Documents.** `TESTS.md`: Test 68 and the numbers of Tests 69 to 92. `.github/ACCESSIBILITY.md`. `sitrep.md`.
  `DECISIONS.md`.

#### Not done

- Whether Narrator or another screen reader takes its voice from this was not tried; the language of a single
  control is not told apart, and need not be while every text is English (3.1.2).
- 14 boxes are unticked; 8 tests are free of the 100.

#### Questions

1. **Is the process's language enough for 3.1.1?** It is what a program can find out; a screen reader mostly
   reads in the voice the user chose. Proposal: keep the tick, and try it with Narrator set to another language.

### 05:44:28: four boxes ticked from what the tests and the code already show, 76 of 86

#### What changed

- **Why.** Four unticked boxes of the checklist asked for a judgment that the tests and the code of the night can
  now carry (Requirements 3 and 4).
- **3.2.2 On Input.** The help line of the first step now says, before a base civ is chosen, what choosing one
  does: "Choose a base civ first: it sets the tree, bonuses and looks. Then name and describe the civ."
  (`HELP` in `wizard.c`). The status line says it after, as before.
- **1.4.1 Use of Color.** A ticked row has a check; the chosen row of a plain list differs from the others by
  lightness, more than 7:1, the pair of the focus color on a field that the test of contrast holds.
- **2.5.2 Pointer Cancellation.** The buttons are Windows' own and act when the mouse button is let go; a pick in
  a list is taken back by undo, which a test holds.
- **3.2.5 Change on Request.** A dialog opens only in a command (`run_action`), a step changes only by a command
  or a pick in the list of steps, there is no timer, and the test "moving the focus through every control changes
  nothing" holds the rest.
- **Tests.** None was added.
- **Checked.** `build.exe`, with the analyzer: no warning, no finding, 92 tests passed, the tests of the layout
  among them with the new help line.
- **Documents.** `.github/ACCESSIBILITY.md`: 76 of 86. `sitrep.md`.

#### Not done

- Ten boxes are unticked: 1.4.4 and 1.4.10 (the text size of Windows, reflow), 1.4.8, 2.4.7 and 2.4.13 (the focus
  frame, which must be looked at), 3.1.3 and 3.1.4 (a glossary), 3.1.5 (the catalog's 394 descriptions were not
  read), 3.3.5 (help for each control) and 4.1.3 (status messages told to a screen reader).
- The high contrast of Windows was not looked at.

### 05:48:07: help for every control, 77 of 86

#### What changed

- **Why.** 3.3.5 Help was not met: no control had help of its own beyond its label.
- **Help in the status line.** F1, or "Help for this control" in a new menu Help, puts one or two plain sentences
  about the control that has the focus into the status line: what it is for and what it allows. With the focus
  nowhere, the help line of the step is given. `show_help` in `app_window.c` does it, as the action `ACTION_HELP`.
- **The texts are data.** `HELP_TEXTS` in a new file, `src/view/control_help.c`, holds one text for each control,
  in the order of `ControlId`; `layout_help` gives it. They are beside the table of the controls, not in it, since
  a line of `SPECS` with its help would pass 120 characters.
- **Tests.** 93, up from 92. Test 69, "the help key says what the control in focus is for" (`window_tests.c`): in
  the real window, with the focus on each control of each step in turn, the help command must put that control's
  help, which must not be empty, into the status line. It is a fourth question of `every_control_passes`. The
  maintainability tests are now Tests 70 to 93.
- **Checked.** The test was written first, against a `layout_help` that gave nothing, and failed at "Steps"; with
  the texts and the command it passed. `build.exe`, with the analyzer: no warning, no finding, 93 tests passed.
- **The checklist.** 3.3.5 ticked, and 3.2.6 Consistent Help now says where the help is: 77 of 86.
- **Documents.** `TESTS.md`: Test 69, the numbers of Tests 70 to 93 and the lines of Tests 60 to 68.
  `.github/ACCESSIBILITY.md`. `README.md`: F1. `MAINTAINER.md`: `control_help.c` and the help of a new control.
  `sitrep.md`. `DECISIONS.md`.

#### Not done

- The help was not read in the running window; a long text may be cut in a narrow window's status line.
- `app_window.c` has 436 lines with its blank lines and is near the 400 lines of code that Rule 13 allows.
- Nine boxes are unticked: 1.4.4, 1.4.8, 1.4.10, 2.4.7, 2.4.13, 3.1.3, 3.1.4, 3.1.5 and 4.1.3. Seven tests are free.

### 05:51:35: status messages told to a screen reader, 78 of 86

#### What changed

- **Why.** 4.1.3 Status Messages was not met: the status line changed its text, but assistive technology was not
  told. The help of a control is shown there now, so it matters more.
- **A live region.** `mark_live` in `controls.c` marks the status line as a polite live region, through the
  property service of Windows' accessibility (`IAccPropServices`), once the controls are made. `dialogs_status` in
  `dialogs.c` raises the event of Windows for a changed live region after every change of its text. `app_open`
  starts COM for the window's thread, which the property service needs.
- **Obstacle: a number the compiler's headers lack.** `EVENT_OBJECT_LIVEREGIONCHANGED` is not in the headers of
  MinGW-w64; its number, 0x8019, is named `LIVE_REGION_CHANGED` in `dialogs.c`. The ids of the interfaces are
  written out in the code, since the compiler's library gave a wrong one before (05:21:07).
- **One list of libraries.** The window's program and the test program are linked with the same libraries, from
  `source_test_libraries`, through their lists of objects; the window's own list in `build_steps.c` is gone.
- **Tests.** 94, up from 93. Test 70, "the status line is a live region that a screen reader is told of"
  (`window_tests.c`): UI Automation, asked as a screen reader asks, gives the status line of the real window the
  live setting "polite". The maintainability tests are now Tests 71 to 94.
- **Checked.** The test was written first and failed; with `mark_live` it passed. `build.exe`, with the analyzer:
  no warning, no finding, 94 tests passed, and the window's program links.
- **The checklist.** 4.1.3 ticked: 78 of 86.
- **Documents.** `TESTS.md`: Test 70, the numbers of Tests 71 to 94 and the lines of Tests 60 to 69.
  `.github/ACCESSIBILITY.md`. `sitrep.md`. `DECISIONS.md`.

#### Not done

- No test hears the event itself: catching it needs a function that Windows calls back, and the code has no
  function pointers but the window procedure (P10). The setting is tested; the event is one line of code.
- No screen reader was tried.
- Eight boxes are unticked: 1.4.4, 1.4.8, 1.4.10, 2.4.7, 2.4.13, 3.1.3, 3.1.4 and 3.1.5. Six tests are free.

### 05:56:10: the text size of Windows followed, and the accessibility suite in two groups

#### What changed

- **Why.** 1.4.4 Resize Text: the window followed the scale of the display, but not the text size that Windows'
  accessibility settings offer, from 100% to 225%.
- **One scale for both.** `layout_text_dpi` in `layout.c` multiplies the display's dots an inch by the text size,
  held between 100% and 225%. `assist_dpi` in a new file, `src/win/assist.c`, reads the display's scale and the
  text size of Windows (`TextScaleFactor` under `Software\Microsoft\Accessibility` of the user's registry) and asks
  it. The window keeps the result as its scale, so the fonts, the layout and the smallest window all grow with the
  setting. `controls_refresh_style` makes the fonts anew and places the controls again when the scale changes,
  which Windows tells the window while it runs.
- **The assistive code in one file.** `assist.c` also holds `assist_mark_live`, moved from `controls.c`, which
  had reached 409 lines.
- **The suite in two groups.** `accessibility_suite_run` had passed 60 lines (Rule 11). Its tests are now two
  groups, `run_layout_test`, the six tests without a window, and `run_window_test`; every test keeps its number.
- **Tests.** 95, up from 94. Test 71, "the text and the layout grow with the text size of Windows, up to 225%"
  (`access_tests.c`): 96 dots an inch at 400% give 216, as at 225%. The maintainability tests are now Tests 72 to
  95.
- **Checked.** The test was written first, against a `layout_text_dpi` that did not scale, and failed; then it
  passed. `build.exe`, with the analyzer: no warning, no finding, 95 tests passed.
- **The checklist.** 1.4.4 stays unticked and says why: at 200% the smallest window is 1920 by 1120 pixels, more
  than many screens hold; it needs the reflow of 1.4.10. 78 of 86.
- **Documents.** `TESTS.md`: Test 71, the numbers of Tests 72 to 95, and the lines of the accessibility suite.
  `.github/ACCESSIBILITY.md`. `MAINTAINER.md`: `assist.c`. `sitrep.md`. `DECISIONS.md`.

#### Not done

- The setting was not changed in Windows to see the window grow; the test holds the arithmetic, not the reading
  of the registry.
- The lower bound of the text size, 100%, has no test of its own.
- Reflow (1.4.10) is not built. Five tests are free of the 100.

### 06:00:59: a glossary of the game's words and of the catalog's shortened forms, 80 of 86

#### What changed

- **Why.** 3.1.3 Unusual Words and 3.1.4 Abbreviations were not met: words of the game had no explanation, and the
  catalog shortens words ("HP", "LOS", "vs.", "H. Mounted Crossbowman"). Question 9 of `sitrep.md` proposed to
  spell the shortened forms out in `data/`; the criterion asks only that their meaning can be found, which a
  glossary gives without changing the catalog's texts.
- **The glossary.** "Glossary..." in the menu Help shows, in a message box, 30 words with their meanings
  (`dialogs_glossary` in `dialogs.c`, the action `ACTION_GLOSSARY`): the words of the game that the window's own
  texts use, civ, base civ, Generic, tech, tech tree, unique unit, unique tech, team bonus, hero, mod, Castle Age
  and Imperial Age; Folwark, Krepost and Mule Cart; and the shortened forms of the catalog, HP, LOS, LoS, ATK, TC,
  ARRG, vs., vs, Skirms, vils, Cav, H., E., max and min. They are `GLOSSARY` in `control_help.c`, beside the help
  of the controls. ARRG is the game's inner name of the Archery Range, as the original's data shows.
- **Tests.** 96, up from 95. Test 72, "the glossary explains every shortened form in capitals of the catalog"
  (`access_tests.c`): every word of two to five capitals in `bonuses.txt`, `team_bonuses.txt`, `techs.txt`,
  `units.txt` and `tree.txt` must be a word of the glossary with a meaning; FREE and OFF, plain words in
  capitals, are let pass. The maintainability tests are now Tests 73 to 96.
- **Checked.** The test was written first, against an empty glossary, and failed at "HP"; with the glossary it
  passed, and with "ATK" taken out, made and undone, it failed at "ATK". `build.exe`, with the analyzer: no
  warning, no finding, 96 tests passed.
- **The checklist.** 3.1.3 and 3.1.4 ticked: 80 of 86.
- **Documents.** `TESTS.md`: Test 72, the numbers of Tests 73 to 96, and the lines of the tests of
  `access_tests.c`. `.github/ACCESSIBILITY.md`. `README.md`. `MAINTAINER.md`. `TERMINOLOGY.md`: glossary.
  `sitrep.md`. `DECISIONS.md`.

#### Not done

- The message box was not opened: a test must not open a dialog, and the window was not run by hand.
- The shortened forms that are not in capitals were found by reading, and no test finds a new one.
- "Settlement", a building of `tree.txt`, has no meaning in the glossary: what it is was not known.
- Six boxes are unticked: 1.4.4, 1.4.8, 1.4.10, 2.4.7, 2.4.13 and 3.1.5. Four tests are free of the 100.

#### Questions

1. **Question 9, answered another way.** The shortened forms stay in `data/`, as the original has them, and the
   glossary explains them. Proposal: keep this; spell them out in `data/` as well only if the texts should read
   better in the game itself.
2. **How far the glossary goes.** The names of the game's 205 units, buildings and techs are taken as proper
   names. Proposal: keep that reading, and add a word to `GLOSSARY` whenever one is found unclear.

### 06:04:17: the glossary's own code made dry and simple

#### What changed

- **Why.** The dryer and the crapper, run after the glossary, named its code: `layout_glossary_word` and
  `layout_glossary_meaning` were alike at 1.000 (Rule 31), and `capitals_are_explained` of its test scored 11
  (Rule 32).
- **One lookup.** `layout_glossary` in `control_help.c` gives a whole `GlossaryEntry`, a word with its meaning;
  the type is in `layout.h`. `dialogs_glossary` and the test ask it.
- **A simpler helper.** `is_letter_between` in `access_tests.c` says whether a letter lies in a range, in place of
  conditions written out four times.
- **Checked.** The behaviour was pinned and shown unchanged (Rule 36): `build.exe`, with the analyzer, no warning,
  no finding, 96 tests passed; with "ATK" out of the glossary, made and undone, Test 72 failed at "ATK" as
  before. The crapper: 947 functions, 335 at 10 or above. The dryer: 925 functions, 66 pairs at 0.8 or above.
- **Documents.** `TESTS.md`: the lines of Test 72.

#### Not done

- `app_window.c` holds exactly the 400 lines of code that Rule 13 allows; the next change to it must move
  something out, such as the menu.
- Pairs alike only by the form P10 gives a small function, such as `layout_help` with `wizard_title`, are left
  for the answer to the question of [the dryer's section](#042723-the-dryer-in-c-and-62-pairs-of-functions-alike).

### 06:09:12: a solid focus frame, drawn by the window, 82 of 86

#### What changed

- **Why.** 2.4.13 Focus Appearance was not met: the focus was shown by Windows' own marks, a dotted line of one
  pixel or a text cursor; and 2.4.7 Focus Visible waited for a look by hand.
- **The frame.** The window draws a solid frame around whichever control has the focus, in the focus color: three
  pixels out from the control and two pixels thick at 100%, both scaled (`assist_draw_focus` in `assist.c`,
  called where the window paints its paper). The frame lies in the gap around a control, so it covers nothing.
  `assist_follow_focus`, called on every notification of a control, has the old and the new frame painted anew
  when the focus has moved; the buttons now tell when they get the focus (`BS_NOTIFY`).
- **Room in `app_window.c`.** `move_focus` and `show_help` moved to `assist.c` as `assist_move_focus` and
  `assist_show_help`; `app_window.c` has 379 lines of code, where it had the 400 that Rule 13 allows.
- **Tests.** 97, up from 96. Test 73, "the control in focus has a solid frame at least two pixels thick"
  (`window_tests.c`), a fifth question of `every_control_passes`: with the focus on each control in turn, the
  window paints its paper into a picture in memory, and two or more pixels of another color must lie just left of
  the control. The maintainability tests are now Tests 74 to 97.
- **Checked.** The test was written first and failed at "Steps"; with the frame it passed; with the frame one
  pixel thick, made and undone, it failed again. `build.exe`, with the analyzer: no warning, no finding, 97 tests
  passed. The tests that the Tab key leaves every control and that help is given still pass with the moved code.
- **The checklist.** 2.4.7 and 2.4.13 ticked: 82 of 86.
- **Documents.** `TESTS.md`: Test 73, the numbers of Tests 74 to 97, and the lines of Tests 60 to 70.
  `.github/ACCESSIBILITY.md`. `sitrep.md`. `DECISIONS.md`.

#### Not done

- The frame was not looked at in the running window: whether it sits well beside a label or a neighbour is for
  the next trial. The test reads the picture the window paints, not the screen.
- The frame's color is taken as the focus color because both come from the same brush; the test tells only that
  it is not paper.
- Four boxes are unticked: 1.4.4 and 1.4.10, which need the layout to reflow, 1.4.8 and 3.1.5. Three tests are
  free of the 100.

### 06:12:47: a node the replaced civ lacked takes the status most civs give it

#### What changed

- **Why.** Question 8 of `sitrep.md`, from [2026-11-09 10:47:03](DEVLOG2.md#104703-the-tech-tree-file-of-two-built-mods-and-the-games-statuses-counted):
  in the tech tree file, a node that the design has and the replaced civ lacked was written `ResearchedCompleted`,
  though the game marks some nodes `ResearchRequired` for nearly every civ that has them. The question was not
  answered; its proposal is carried out as a proposal.
- **The usual status, from the game's own files.** The statuses of the game's 62 `CivTechTrees` files, counted on
  2026-11-09 for each `Use Type` and `Node ID`, were set against the 205 nodes of `tree.txt`. Thirteen are
  `ResearchRequired` for most civs that have them: the Hand Cannoneer (23 of 24), Bombard Cannon (24 of 26), Fire
  Ship, Fast Fire Ship, War Galley, Galleon, War Hulk, Carrack and Cannon Galleon, and the Guard Tower, Keep,
  Bombard Tower and Fortified Wall. The other 192 are `ResearchedCompleted` for most.
- **A tenth column of `tree.txt`.** Each node's line ends in `required` or `completed`. `add_tree_node` in
  `catalog.c` reads it into `needs_research` of the node's `TreeEntry`; a line without it reads as completed.
- **The tech tree file.** `set_status` in `tech_tree.c` writes `ResearchRequired` for a node that the design has,
  the replaced civ lacked and the catalog says needs research, and `ResearchedCompleted` for any other such node.
- **Tests.** 98, up from 97. Test 54, "a building the replaced civ lacked takes the status most civs give it"
  (`tests/mod/mod_tests.c`), the last of the functional tests; the accessibility tests are now Tests 55 to 74 and
  the maintainability tests 75 to 98.
- **Checked.** The test was written first and failed, the Keep being written `ResearchedCompleted`; with the
  change it passed, and the five older tests of the tech tree file passed unchanged. `build.exe`, with the
  analyzer: no warning, no finding, 98 tests passed, the tests of the real window among them, which read the
  catalog with its new column.
- **Documents.** `TESTS.md`: Test 54 and the numbers of Tests 55 to 98. `MAINTAINER.md`: the tenth field of
  `tree.txt`, and the status in "How a mod is built". `sitrep.md`. `DECISIONS.md`.

#### Not done

- The mod was not tried in the game.
- No test reads the new column from `tree.txt` itself; the test sets `needs_research` by hand.
- A game update that changes a node's usual status leaves the column out of date.
- Two tests are free of the 100.

#### Questions

1. **Question 8 stays open** until it is answered: the proposal is carried out, and can be taken back by making
   every line of `tree.txt` end in `completed`.

### 06:27:55: the mutator mended after the tests of the window slowed it

#### What changed

- **A fault of the night's own making.** With the tests of the real window in the test program, the mutator, which
  runs twelve test programs at once, no longer worked: one test program alone takes 3.0 seconds, but six at once
  took 16.6, since the tests that ask Windows' accessibility interfaces wait on one another across programs. On
  `src/techtree/`, 36 of 53 mutants were "ended by the time limit" of 30 seconds, which counts them as killed
  though no test had judged them.
- **The mending.** The test program takes one argument, `nowindow`: `test_window_is_left_out` in `runner.c` says
  so, and `accessibility_suite_run` then ends before the tests of the window. The mutator runs the tests that way,
  both for its coverage (`cover_build_run` takes the arguments of the test run) and for each mutant, and it
  leaves `src/win/` out (`is_target` in `mutator.c`), whose mutants only the tests of the window could judge and
  which could open a dialog on the desktop. The crapper still runs every test.
- **Checked.** `build.exe`, with the analyzer: no warning, no finding, 98 tests passed; `build\tests.exe nowindow`
  runs 84 tests in 1.4 seconds. `build\mutator.exe src/techtree/`: 53 mutants in 39 seconds, 37 killed, 16
  survived, none ended by the time limit, 23 more places on lines no test runs. The crapper: 953 functions, 334 at
  10 or above. The dryer: 930 functions, 66 pairs at 0.8 or above.
- **Documents.** `MAINTAINER.md`: what the mutator leaves out. `sitrep.md`. `DECISIONS.md`.

#### Not done

- The mutants of `src/win/` are not made at all now; what the tests of the window catch there was shown by hand
  only, for the faults named in the sections above.
- A line of `src/` that only a test of the window runs counts as not run for the mutator.
- Why the accessibility interfaces of several programs wait on one another was not found out.
- The whole of `src/` was not mutated again: the counts of 04:57:44 are from before the night's later changes.

### 06:49:55: the mutants of all of src/ counted again

#### What changed

- **Measured.** `build\mutator.exe`, with no folder named, from 06:29 to 06:49, with the mended mutator: 4372
  mutants of the lines of `src/` that the tests without a window run. 2334 were killed, 2018 survived, 8 were ended
  by the time limit and 12 did not compile; 2547 more places are on lines those tests do not run. `src/win/` is left
  out.
- **Against 04:57:44.** Then: 4 370 mutants, 2 324 killed, 2 032 survived, 2 524 places not run. The night's later
  changes moved little: `view` has 136 killed and 140 survived where it had 127 and 149, with the new texts and
  the scale of the text; `techtree` 37 and 16 where it had 35 and 16; `civlist`, `data`, `dat` and `model` differ
  by a few.
- **By folder,** killed, survived and places not run: base 35, 18, 3; bonus 55, 49, 19; civlist 63, 39, 2; core 115,
  80, 112; dat 512, 300, 121; data 33, 85, 458; deflate 106, 106, 7; forge 102, 106, 877; hero 55, 41, 21; json 294,
  205, 111; look 106, 123, 74; mod 168, 244, 192; model 179, 159, 88; pack 190, 115, 34; persist 94, 100, 17;
  present 38, 86, 196; techtree 37, 16, 23; unique 16, 6, 96; view 136, 140, 96.
- **Checked.** After the run, `build\tests.exe`: 98 tests run, 0 failed.
- **Documents.** `sitrep.md`.

#### Not done

- Rule 34 does not hold: about half of the mutants of tested lines survive, and no test was written to kill one.
  Two tests are free of the 100.

### 06:54:15: a test that catches Bug 2 again, and the program started

#### What changed

- **Why.** Bug 2 of the trial, a ticked box that showed a filled rectangle where a check should be, was mended on
  2026-11-09 without a test, and at 05:16:33 it was left without one because a test "would have to judge
  pixels". The test of the focus frame showed how: the window can be asked to draw into a picture in memory.
- **The test.** "A ticked row is drawn with a check, not a filled box" (`tests/accessibility/draw_tests.c`, a new
  file): the real window, not shown, is asked to draw a ticked row of the bonuses into a picture; the box is found
  in it by its frame, and the inside of the box must hold marks, on less than two fifths of it. A filled box
  holds marks all over, and a box without a check holds none.
- **A header for the window's tests.** `window_fixture.h` names `window_fixture_close`, which `window_tests.c`
  defines and `draw_tests.c` uses, so that the tests of the window fit in two files under 400 lines of code.
- **Tests.** 99, up from 98. Test 75, the last of the accessibility suite; the maintainability tests are now Tests
  76 to 99.
- **Checked.** The test passed at once, the check being drawn rightly; with the box filled after its check, made
  and undone, it failed, and with the check drawn for the rows that are not ticked, made and undone, it failed.
  `build.exe`, with the analyzer: no warning, no finding, 99 tests passed.
- **The program started.** `build\civbuilder.exe` was started once at 06:51: its window opened with the title "A
  new civ - Step 1 of 9: Identity - civbuilder", was closed by the close command, and the program ended with 0.
- **Documents.** `TESTS.md`: Test 75, the numbers of Tests 76 to 99, and the lines of the tests of
  `window_tests.c`. `MAINTAINER.md`. `sitrep.md`. `DECISIONS.md`.

#### Not done

- The window was started, not used: nothing was clicked, typed or looked at beyond its title.
- One test is free of the 100.

### 06:55:28: the last measurements of the night

#### What changed

- **A helper brought under 10.** `box_holds_a_check` in `draw_tests.c` scored 10.045 with the crapper; one of its
  two checks, which could not fail, was taken out, and it scores below 10.
- **Measured.** `build.exe`, with the analyzer: no warning, no finding, 99 tests passed. The crapper: 958 functions, 330 at 10 or above,
  where the first run of the night had 369 of 847. The dryer: 935 functions, 66 pairs at 0.8 or above, where the first run had 62 of 849
  functions. The mutator, at 06:49:55: 4 372 mutants of `src/` without `src/win/`, 2 334 killed and 2 018 survived.
- **The checklist.** 82 of 86 boxes are ticked, where 46 were.
- **Documents.** `sitrep.md`.

#### Not done

- Rules 31, 32 and 34 are measured and do not hold; the tools' numbers above say by how much.
- The features of the original that `sitrep.md` lists as missing are still missing, and the mod was not tried in
  the game.
- One test is free of the 100.

### 06:57:47: a function of the window brought under CRAP 10

#### What changed

- **Why.** Rule 32, and the skill's "Then make it right": with tests of the real window, a function of `src/win/`
  can be brought down by splitting it while tests hold what it does. `controls_draw_item` in `controls.c` scored
  240 when no test ran it, and 15.2 once the tests of drawing did, with a complexity of 15.
- **Three parts taken out.** `draw_box` draws a row's box and its check and gives the box's right edge;
  `is_checklist` says whether a control is a checklist; `draw_row_focus` draws the frame of the row in focus.
  `controls_draw_item` keeps the filling, the text and the calls.
- **Checked.** The behaviour was pinned and shown unchanged (Rule 36): `build.exe`, with the analyzer, no warning,
  no finding, 99 tests passed; with the check drawn for unticked rows, and with a checklist told wrongly, each made
  and undone, Test 75 failed. The crapper: `controls_draw_item` scores below 10, as do its three parts;
  961 functions, 329 at 10 or above. The dryer: 938 functions, 66 pairs at 0.8 or above.
- **Documents.** `sitrep.md`.

#### Not done

- `window_procedure` (complexity 20), `on_command` (15) and `run_action` (13) of `app_window.c` are still at 10
  or above: each is one `switch` or one chain of choices, and `app_window.c` has little room left under Rule 13.
- 330 functions or so are still at 10 or above; this was one of them.

### 07:02:10: two functions split, a mutant that lived, and the hundredth test

#### What changed

- **Why.** Rule 32: `select_rows` in `sync.c` and `list_model_fill` in `binding.c` scored 10.000 and 10.036, each
  run in full by the tests of the window and one branch too many.
- **The splits.** `first_chosen_row` in `sync.c` finds the first row of a list that the design has chosen;
  `add_row` in `binding.c` adds a row to a list's model, also the row "No hero".
- **A mutant lived.** With `first_chosen_row` taking the first row that is not chosen, made by hand, all 99 tests
  passed: no test held which row a list or a choice shows. So the split of `select_rows` was not pinned (Rule 36).
- **The hundredth test.** Test 76, "after an undo, the base civ shown is the design's again"
  (`tests/accessibility/choice_tests.c`): in the real window another base civ is picked, the pick is undone, and
  the choice must show Generic again. It kills that mutant, and it is the first test to run a base civ picked in
  the window and taken back (Requirement 5). The maintainability tests are now Tests 77 to 100.
- **Checked.** `build.exe`, with the analyzer: no warning, no finding, 100 tests passed. With rows no longer added
  to a list's model, made and undone, Tests 64, 66 and 76 failed. The crapper: 964 functions, 323 at 10 or above;
  `select_rows`, `list_model_fill`, `on_pick` and `apply_base` are below 10 now. The dryer: 66 pairs.
- **Documents.** `TESTS.md`: Test 76 and the numbers of Tests 77 to 100. `sitrep.md`.

#### Not done

- No test is free of the 100 (Rule 5): the next test must take the place of another.
- The mutator cannot judge `binding.c`: only the tests of the window run it, and the mutator leaves those out.

#### Questions

1. **The last test.** All 100 tests are used, 22 of them for the tools and the window in this night. Proposal:
   keep them, and when a test is needed, join two that ask the same of every control, such as "the Tab key reaches
   every control" with "the Tab key leaves every control".
### 08:27:52: answers recorded, CHECK left out of the tools, the first merges; unfinished

Answers to the questions of 07:02:10 and before: `*.png` is in Rule 25; Rule 5 no longer limits the tests to 100;
Rules 26 to 28 now yield to ub-mode. Decided: the branches of `CHECK` do not count for CRAP or mutation; functions
that differ only in a literal are merged; a checklist box that no test can settle is flagged "human review needed";
`build.exe` gets a ratchet on the counts of the crapper and the dryer.

- Done: `crap_cover.c` reads `gcov -b -c -t` by line and leaves lines with `CHECK(` out; `crap_complexity` takes
  one off for each `CHECK`; `mutant_next` passes over `!CHECK(...)` and a `||` before it. Tests 101 to 103, written
  after the code and not seen to fail first. Crapper: 276 of 966 at CRAP 10 or above (was 323).
- Merged: `json_begin`/`json_end` (four functions before), `write_empty`, `history_step` (undo and redo;
  `history_can_undo`/`_redo` were unused and are gone), `text_equal`/`text_starts_with` over `matches`,
  `dat_edit_patch_sunk`, `wizard_move` and `is_step`, `controls_row_height` over `scaled`, `sync_from_design`,
  `in_range`, `binding_of`, `huffman_build_fixed`, `catalog_find_bonus` with a `team` flag.
  Dryer: 47 pairs before the last two merges (was 66).
- State: the build fails on Test 95 only, because Tests 101 to 103 are not in `TESTS.md`.
- Not done: the alike tests and tool functions; `dat_edit_patch_i16/_i32/_f32` (0.849); the flags in
  `ACCESSIBILITY.md`; the ratchet; DECISIONS, TESTS, MAINTAINER and the places that still say "at most 100 tests"
  (AGENTS, TESTS, MAINTAINER, two skills); the mutator was not run again.
- Known: a line that mixes a `CHECK` with another condition loses all its branches from the coverage.
- Slips: a `python -` typed by mistake ran nothing and was stopped; `gc.exe` was left in the Temp folder of the
  user and awaits a line in `DELETION_REQUESTS.md`.

#### Questions

1. **Lookups over different types.** `catalog_find_unit`, `_tech`, `_tree` and `_base` are alike in form only.
   Proposal: give the entries of the catalog one shared key structure first, as a change of its own.

The day continues in `DEVLOG4.md`: this part is full.
