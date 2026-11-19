# Development log, part 7

- [2026-11-17](#2026-11-17)
- [2026-11-18](#2026-11-18)
- [2026-11-19](#2026-11-19)

Part 7 of the development log. It follows [part 6](DEVLOG6.md). The decisions made in the log are listed in
[DECISIONS.md](../DECISIONS.md).

## 2026-11-17

The day goes on from [part 6](DEVLOG6.md#2026-11-17), which holds its sections up to 07:05:37.

### 07:15:16: a cost line on the last field of the costs is built

#### What changed

- **Why.** In runs 16 and 17 of the mutator, two mutants of the bound of `patch_fits` in `bonus_build.c` survived. One
  of them, `<=` made `<`, refuses a unit patch on the last field of its part, and no test had such a patch.
- **Tests.** 457, up from 456. Test 47 checks, on the small dat, that a cost line on the last field of the costs, the
  flag of the third cost at place 16, is built and sets that flag. Tests 47 to 456 became 48 to 457.
- **Checked.** `build.exe quick`: 457 tests, every one passed. With the bound made `<`, Test 47 failed, and only it; the
  file was put back from its copy. The mutator on line 87 of `bonus_build.c`, run 21: `<=` made `<` was killed; `+` made
  `-` survived, since only a patch past its part shows it, and no test gives the build such a line, as question 6 of the
  sitrep asks about. No code of `src/` changed. `build.exe` with the analyzer: 457 tests, every one passed, no warning.
- **Documents.** `TESTS.md`: Test 47, and the numbers of the tests after it. `DECISIONS.md`: the number of the test of
  sliding. The sitrep: the numbers of the tests it names. `DEVLOG7.md`: this part. Part 6 holds this day only and ends
  at 995 lines, so the day goes on here, as Rule 22 allows for a part of fewer than two days.

#### Not done

- The survivor `+` made `-` of line 87 of `bonus_build.c` stands, with the others at checks of outside data.

### 07:20:34: tests of the icon, walking and attack graphics that a look line copies

#### What changed

- **Why.** `copy_unit_look` in `look_copy.c` copies a unit's icon and its walking and attack graphics, for a look line
  and for the monk skin, but the small dat writes those fields as 0 in every civ, so no test failed when one of the
  copies was left out.
- **Tests.** 460, up from 457. The helper of Test 128 (formerly 127) in `look_tests.c` now takes the place in the unit
  to mark (`LookPlace`): it marks that place in civ 0's unit, copies the look, and reads it in the civ built. Tests 129,
  130 and 131 check that a look line gives its unit the attack graphic, the walking graphic and the icon of the unit it
  copies. Tests 129 to 457 became 132 to 460.
- **Checked.** `build.exe quick`: 460 tests, every one passed. With the copy of the icon, of the walking graphic or of
  the attack graphic skipped in turn, Test 131, 130 or 129 failed, and only it; the file was put back from its copy each
  time. No code of `src/` changed, so the crapper, the dryer and the mutator were not run again. `build.exe` with the
  analyzer: 460 tests, every one passed, no warning.
- **Documents.** `TESTS.md`: Tests 128 to 131, and the numbers of the tests after them. `DECISIONS.md`: the number of
  the test of sliding. The sitrep: the numbers of the tests it names.

### 07:23:28: every mutant of the copy of a unit's look killed

#### What changed

- **Why.** The section of 07:20:34 did not run the mutator, since no code of `src/` changed; its new tests protect
  `copy_unit_look` in `look_copy.c`, so that function's lines were measured.
- **Tests.** 460, as before. Nothing changed in the project but this log and the sitrep.
- **Checked.** Coverage, then the mutator on lines 296 to 299 of `look_copy.c`, run 22: the 5 mutants of
  `copy_unit_look` were killed. The run also reported lines near them: 7 more were killed, the guard of `timing_of` at
  line 309 survived as in run 20, and 5 lie on lines that no test runs: the guards against a copy outside the data file,
  a part that one of two units lacks, a civ number out of range and a monk's civ that the data file lacks, and the copy
  of the damage graphics, which no unit of the small dat has.

### 10:57:53: no test names Rule 4 alone: the 21 that did are removed

#### What changed

- **Answers.** Of the questions of 21:03:39 of 2026-11-16, in [the answers recorded: no cap on tests, P10's bounds and
  check count given up, AA only, coverage
  measured](DEVLOG5.md#210339-the-answers-recorded-no-cap-on-tests-p10s-bounds-and-check-count-given-up-aa-only-coverage-measured),
  asked again as question 2 of the sitrep: no. A test that names only Rule 4 is the kind of test that Rule 5 forbids
  (2). Every test whose purpose named Rule 4 alone was to be removed, and no test is to be written for now.
- **The tests removed.** 21, by their old numbers: 276 and 277 (a filter and a category of a list), 284 to 289 (the
  details of a unit and of a unique tech), 336 (Glossary in the menu Help), 338 and 340 (a filter and a category in the
  window), 354 (the loop of messages ends with the code of the message that ends it), 402 (the glossary explains the
  catalog's shortened forms), 447, 449, 450, 453, 454, 455 and 459 (the build program's objects of the tests, its modes,
  its arguments, its verdict, the analyzer's command and the command that runs the tests), and 460 (every test defined
  is run). Test 402, formerly 415, names rule 4 of P10, not the project's Rule 4, and stays.
- **What went with them.** In the tests: `rows_kept` and `unit_details_hold` in `present_tests.c`; the check of the
  glossary's words in `access_tests.c`, with its list of catalog files and of plain words in capitals; the typing and
  choosing of `rows_after` in `dialog_tests.c`, which is now `step_after_next` and only presses Next; and
  `test_run_before` and `test_rules_are_left_out` in `runner.c`, which only Test 460 read. In the suites: the groups of
  the filter and of the details in `model_suite.c` (`MODEL_SUITE_SIZE` 75 to 67), and the cases and group ends in
  `shell_suite.c`, `accessibility_suite.c` and `maintainability_suite.c`. No code of `src/` or `tools/build/` changed,
  and no file was left empty.
- **Tests.** 439, down from 460. The tests after each one removed were renumbered.
- **Checked.** `build.exe quick`: 439 tests, every one passed. `src/` had not changed, so the coverage build did not
  compile it again, and gcc adds the counts of a run to those of the runs before while the object is the same; the
  sources were touched so that every object was built again and its counts written afresh. The crapper on that coverage:
  912 functions, the highest 9.6, and every function covered as before, since the tests of the window still run the
  code. The dryer was not run again: no code it reads changed. The mutator, run 23, on every line of the six files whose
  tests were removed, with the window for the two of the window: 441 mutants, 220 killed, 189 survived, 32 on lines that
  no test runs; `details.c` 33 survived, `binding.c` 56, `build_plan.c` none, `source_list.c` 27, `control_help.c` 4
  (the guard of the glossary's index) and `app_window.c` 69. The runs before measured only lines that had changed, so
  how many of these mutants the removed tests killed was not measured. `build.exe` with the analyzer: 439 tests, Test
  418 failed, on the first line of `TERMINOLOGY.md`, which is longer than 120 characters; that line was added while this
  section was written, and was left as it is (question 3).
- **Documents.** `TESTS.md`: the 21 entries removed and the rest renumbered, the lines of Test 328, and that Rule 4 is
  never a test's purpose. `MAINTAINER.md`: the first two steps of adding a test. `DECISIONS.md`: two decisions replaced,
  one added, and the number of the test of sliding. The sitrep.
- **Replaced.** "A test of code that no requirement names names Requirement 10", of 21:03:39 of 2026-11-16; "The last
  test checks that every test defined is run", of
  [02:41:02](DEVLOG6.md#024102-the-build-plan-apart-from-the-steps-that-run-gcc-and-a-test-that-every-test-defined-is-run).

#### Not done

- The mutants that now survive have no test, since none may be written: 189 in the six files of run 23, the lists of
  which are in the scratchpad of the run.
- A wrong size of a suite or start of a group again leaves tests out without a failure.

#### Questions

1. **The code whose tests were removed.** No requirement names the details beside the lists, the filter and the
   categories of a list, the glossary of the menu Help, the loop of messages or the modes of the build program. The
   details, the filter and the categories are the original's; the glossary was made for two criteria of level AAA, which
   are gone. Proposal: keep the original's features, and let a requirement name what civbuilder takes from the original,
   so that their tests can name it when tests may be written again; take the glossary out.
2. **Test 460 caught a bug found before.** Twice a suite's size was left as it was when cases were added, and tests were
   left out of the run without a failure (2026-11-16 22:12:27, and 2026-11-17). Test 460 caught that again, but named
   Rule 4. Proposal: when tests may be written again, it comes back naming that bug.
3. **The first line of `TERMINOLOGY.md`.** "clanker" is defined in 152 characters, and Rule 9 allows 120, so Test 418
   fails and `build.exe` does not pass. Proposal: "clanker - a non-human agent: a tool, a large language model (LLM) or
   an AI agent, e.g. Claude, Codex, Gemini".
4. **Rules 31 to 34.** The tests are now 393 of the core (354 functional, 39 of maintainability) and 46 of
   accessibility, in three categories that are not those of Rule 34. Rule 31 allows as many core tests as there are
   rules, requirements and bugs, about 53; Rule 32 allows 55 of accessibility, and keeps the tests that only kill
   mutants apart, run for Requirement 12 alone. No `BUGS.md` exists (Rule 33). Proposal: the core keeps one test per
   rule, requirement and bug, the tests that kill mutants move to a redundancy suite that `build.exe redundancy` runs,
   and `BUGS.md` lists the bugs found in testing, Bugs 1 and 2 of 2026-11-10 and 1 to 4 of 2026-11-16; this when tests
   may be changed again.

## 2026-11-18

### 07:24:48: civs of the game edited and saved into one named mod

#### What changed

- **Why.** The window was to start from the Britons instead of Generic, each field holding what the game gives; the
  first step was to say that it chooses which civ to edit; the review was to have Save civ and, at its top right, how
  many civs are changed, with Save civ file... and Open civ file... in the menu only; the review's text was to show no
  caret; and Build mod was to ask for the mod's name, which names the mod's folders instead of the civ's name.
- **Answers.** Four questions were put while the work was done, and answered. A mod changes every civ that was
  changed and saved, each in its own slot, so the review's choice of the civ it replaces goes. Save civ puts the civ
  in the mod and writes nothing to the disk; only the civs saved so are built and counted. The tagline and the
  description are read from the game when the window opens, empty without the game, and no text of the game is kept
  in the project. Tests may be written again, and a test that only ub-mode needs is a redundancy test; that answers
  question 4 of [10:57:53](#105753-no-test-names-rule-4-alone-the-21-that-did-are-removed) in part.
- **Civ to edit.** Generic, base 0, is gone from `appearance.txt` and the catalog (`CATALOG_GENERIC_BASE`). The first
  list of the step Identity is Civ to edit (`CONTROL_CIV`, before `CONTROL_BASE`), the Britons at first, and the step
  says "Choose which civ to edit. Every step then shows it as the game has it, ready to change." `base_civ_apply` now
  also gives the name of the civ's `base` line, the unique unit and techs named as the catalog names them
  (`command_pick`), no hero, and the tagline and description the game gives. Choosing another civ (`choose_civ` in
  `sync.c`) shows that civ's draft, or its base civ the first time, as one change that undo takes back.
- **Drafts and the civs of the mod.** The new `src/roster/civ_roster.c` keeps, for each of 64 slots, the draft of its
  civ, the design last saved in the mod and whether the mod holds it. The design shown is kept as its civ's draft each
  time the window shows it anew. `roster_save` puts a design in the mod only when it differs from its civ of the game
  (`civ_design_same`, in the new `src/model/civ_compare.c` with the comparisons of each step).
- **The review.** Its right column holds the count of civs changed (`CONTROL_CHANGED`: "1 civ changed", "2 civs
  changed", from `details_changed`), Save civ (`CONTROL_ADD`, `sync_save_civ`) and Build mod... . The choice of the
  civ it replaces and the buttons Save civ file... and Open civ file... are gone from it; the menu File keeps the two.
  A step is done once the design differs there from its civ of the game, and the review once the mod holds the civ as
  it is and the civ is complete (`wizard_step_is_done`). The review's text hides the caret when it takes the focus
  (`hide_report_caret` in `app_window.c`).
- **The game's texts.** `game_text_read`, in the new `src/gametext/game_text.c`, reads when the window opens each
  civ's `tech_tree_description_string_id` from the game's `civilizations.json`, and that string from its English
  strings: the first line, without " civilization", is the tagline, the rest the description. `civ_list_read` moved
  from `forge.c` to `civ_list.c`, so that both use it.
- **The mod's name.** Build mod... asks for it (`ask_text` in `ask.c`, in a dialog made in memory by the new
  `src/shell/text_dialog.c`): `civbuilder_mod` at first, then the name given last. The folders' prefix is made from
  it as it was from the civ's name (`forge_prefix`), and both `info.json` take it as their title, the UI mod's with
  " (UI)". A civ that keeps the name of its civ of the game is offered `civbuilder_civ` when saved as a civ file.
- **Several civs in one mod.** `ForgeJob` holds up to 64 designs. `forge_build` applies the recipe of each to one
  `DatEdit`, so the techs a civ adds are numbered after those of the civs before it, and a requirement that
  `add_requirement` in `recipe_copy.c` adds takes a slot that no civ before has patched (`dat_edit_is_patched`); when
  none is left, the build fails (question 11). The civ list is written civ by civ (`chain_civ_list`), each civ gets
  its tech tree file, and `mod_strings_write` writes the names and pickers of all civs first, then the descriptions,
  then each civ's other texts. The report of a build names each civ (`BuiltCivs` in `summary.h`). The room for
  patches, copies and strings was raised for 64 civs.
- **Redundancy tests.** A fourth category of tests, in `tests/redundancy/`, run after the others only with the word
  `redundancy` (`tests/runner.c`; `build_test_arguments` passes it on), and numbered on their own, as "Redundancy test
  3". `TESTS.md` lists them in a section of their own, which its checks of the other tests do not count. Each group
  of `redundancy_suite.c` holds at most eight tests, as `DECISIONS.md` has it since 2026-11-15: the four groups of 13,
  11, 9 and 9 were divided where their subjects change, and the tests kept their order, but for the newest,
  Redundancy test 46, which went to the descriptions, before the two tests of room.
- **Made right.** After the crapper and the dryer, `civ_design_same`, `game_text_read`, `read_quoted` and
  `mod_strings_write` were split into smaller functions; `civ_design_same_bonuses` takes the list it compares
  (`BonusList`), where a second function compared the team bonus alike; and `source_write_objects` in
  `tools/build/source_list.c` writes each object through `write_object`.
- **The mutator's survivors.** Of those on lines this work wrote, the checks that no caller can fail were taken out:
  the capacity checked by `ask_text`, the range of the control in `hide_report_caret`, which `layout_spec` checks
  itself, and the check of the message of `may_build_into`, which always fits (at most 612 bytes in 2048). So were the
  second bounds of the loops of `civs_are_ready`, `report_build`, `summary_write_build` and `dat_edit_is_patched`,
  whose counts never pass their arrays (64 each). `ask_text` and the default mod name copy with `lstrcpynW`, which
  ends the copy within its room, in place of `wcsncpy` and an end put after it. The texts of `summary_write_build`,
  `details_changed` and `choose_civ` start from `text_copy` or an empty initializer, and the `BaseCiv` of `add_base`
  from an empty tagline, in place of a first letter emptied: a mutant of that letter kept the start of an older text.
  In a second round the same was done to the check of more than 64 civs in `forge_civs_are_finished`, since a job
  counts the roster's civs, and to the range of the slot in `roster_draft`, since `add_base` refuses a civ id outside
  0 to 63; to the second bounds of the four loops of `forge.c` over a job's civs, of the four of `mod_strings.c`, whose
  count `mod_strings_write` no longer checks, and of `want_descriptions` and `base_wanting`; and to the 8192 letters
  of `read_quoted`, the 400000 lines of `read_lines` and the nine digits of `read_line`, whose texts always end, since
  `file_read_all` ends what it reads. `read_line` no longer asks for a digit, nor for a space after the digits, which
  the game writes as a tab on 4331 of its 19682 numbered lines, though on none of its 62 descriptions: a line without
  a digit gives the id 0, and the game's description ids run from 120150 to 120211. So `read_lines` stops at the end
  of the text alone, an empty line being one without a digit. `add_requirement` takes a slot holding
  `NO_REQUIREMENT`, -1, the game's empty slot, in place of any number below 0. `mod_strings_write` and
  `build_test_arguments` start their texts from `text_copy`, and `write_object` writes its line with `fputs` and
  `fputc` in place of `fprintf`. The limits that the checks taken out rested on are stated where they meet, by two
  `_Static_assert`s: the roster's slots and the catalog's civ ids in `sync.c`, a mod's civs and the roster's slots in
  `dialogs.c`. The check of more than 64 civs had come after `civs_are_ready` filled the job, too late to guard it.
  The check of the civs in `forge_build` stays: without it Test 29 failed, since a caller may build without asking
  `forge_civs_are_finished` first. The others got redundancy tests, below.
- **The mutator's uncovered mutants.** Those on lines this work wrote, which no test ran, were counted after the
  third round of survivors: 58, of which 47 in `text_dialog.c`, which no test opens (question 3). Of the other 11,
  two were on lines that a user reaches: `add_requirement` in `recipe_copy.c` when the civs built before have taken
  the free places of a tech's requirements, as several civs of one mod with the same bonus can, and
  `put_description` in `mod_strings.c` when a civ's description passes its 8192 bytes, as 24 long bonuses and a long
  description do. They got Redundancy tests 47 and 48. One was on a check that cannot fail: the picker's text of
  `put_name`, "Click to play as " and a name, always fits, so its result is cast to `(void)` and a `_Static_assert`
  states the sizes. The other eight are under Not done. The mutator, run again on the two files, found a survivor
  that the round before had killed: the lower bound of the descriptions that `mod_strings_write` writes past the
  game's, which only the Britons' id, 120150, the first, tells apart, while Redundancy test 42 had since been given
  the Jurchens'. Redundancy test 46 writes the Britons' description and finds it once.
- **Tests.** 439, as before, and 63 redundancy tests, new. Changed: Tests 50 and 51 test a civ of the game instead of
  Generic, under new names; Test 28 the folders named from the mod's name; Test 285 a report that names the civ; Test
  323 a new civ that keeps the game's name; Tests 336 to 338 a civ saved in the mod before Build, and the mod's name;
  Test 392 the Britons after an undo; Test 436 `redundancy` passed on; and the fixtures that used Generic,
  `CONTROL_BASE` or a job of one design. Redundancy tests 1 to 37 test the civ of the game, picks, the steps done, the
  roster, how a civ is told from its civ of the game, and the game's texts; 38 the analyzer in the compile command; 39
  the first patch of the dat file found again; 40 to 48 the builds refused, the descriptions in the mod's strings and
  the first of them written once, the room of a tech for the requirements of several civs and the room of a
  description; 49 to 63 the real window: the Britons at first, a draft kept, the count of civs changed, Save civ, the
  caret of the review's text, Build, the mod's name, the glossary, the filter and the categories of the bonuses, an
  empty mod name, the name of the civ file of a long civ name and the caret of the Name field. In the catalog of the
  tests the Britons now have one bonus and the Franks two.
- **Checked.** `build.exe quick redundancy`: 502 tests, 439 and 63, every one passed but Test 418, which failed before
  this work too. `build.exe`, the analyzed build: no warning and no finding of the analyzer, and of its 439 tests
  only Test 418 failed. The tests of the game's texts and of the caret of the review's text were seen to fail before
  their code was written; the other redundancy tests were written after the code they test. Redundancy tests 38, 39
  and 58 to 62 were shown honest by breaking the code: the analyzer taken out of the analyzed build, the search of the
  patches begun at the second, the glossary's loop cut, the filter made to match every row, the category's comparison
  turned round, the answer of the mod's name taken when either of its two halves held, and the cut of a long civ name
  doubled; each failed alone, and Redundancy test 61 failed alone again when a mod name not given counted as given.
  The seventeen of the later rounds were shown honest too. Thirteen breaks at once made Redundancy tests 22, 24, 25,
  33 to 37, 40 to 42, 44 and 63 fail, and 56 with them: a roster started anew holding every draft, the looks and the
  costs compared from the second, a stale description left, a line's end not ending a text, a space at the end kept,
  the tail cut from any first line long enough, a tagline let fill its whole room, the civs of the descriptions sought
  from the second, a description id at the end of the game's span taken as inside it, a job of no civ taken as
  finished, `forge_build` answering true for an unfinished civ, and the caret of every field hidden. Redundancy test
  43 failed, with 42, when a civ's description took every other id, and Redundancy test 23 alone when slot 64 was
  taken as a slot. Then Redundancy test 42 was given the Jurchens' civ, in the middle of the game's descriptions, in
  place of the Britons', the first: it failed, with Redundancy test 26, when the descriptions were sought below 120150
  in place of above it and `multiplier_of` looked one pick past its count; Redundancy test 45 failed alone when
  `civ_described_by` looked one civ past its count. Redundancy test 47 failed alone when `add_requirement` answered
  true with no place free, and again when its loop read one place past the six; Redundancy test 48 alone when a
  description too long for its room left the strings unfailed; Redundancy test 46 alone when the descriptions past
  the game's were written from 120150 on; Redundancy tests 52 and 53 alone when line 211 of `sync.c` gave the count
  of civs changed to every other text of the step and their details to the count. Each break was undone, and the
  diff was then as before it.
- **Measured.** gcc adds the counts of a run to those of the runs before while an object stays the same, so a measure
  after a build that compiled only some files counts lines that no test runs any more; the highest score of 9.6 at
  10:57:53 was such a measure. With every source compiled anew, the glossary, the filter of the lists and two
  functions of the build program scored 10 or more; they now have redundancy tests. The crapper after each round of
  survivors and after the tests of the uncovered mutants, every source compiled anew: 977 functions. Each time only
  the three of `text_dialog.c`, which no test runs (question 3), scored 10 or more: `answer_proc` 56.0,
  `text_dialog_show` 30.0 and `put_text` 12.0. Every other function was below 10, the highest `on_text_change`, 8.3,
  the first time, and `bottom_box`, 9.6, after that: in those runs no test reached the `return` under its checks of
  the size or its `default:`. The dryer, each time: no two functions 80% alike. The mutator ran one file at a time on
  each source this work changed, after the last change to it, the files of `src/win/`, `src/shell/` and
  `src/present/` with the window's tests. Eleven runs stopped on an error of Windows (question 9), and each such file
  was run again until a run ended. On lines of this work the run after the first round of survivors left 84, the run
  after the second 28 and the third 24, each read; after the tests of the uncovered mutants, the runs of the two files
  they test left none in `recipe_copy.c` and one in `mod_strings.c`, which Redundancy test 46 killed in a last run.
  Those left, and why, are under Not done. The survivors on older lines were left as they were.
- **Documents.** `TESTS.md`: the entries of the tests changed, the lines of the tests in the files changed, and the
  63 redundancy tests in a section of their own. `TERMINOLOGY.md`: Generic taken out; base civ, replaced civ and
  prefix changed; tagline, civ to edit, draft, mod name, Save civ, civs changed, roster, job and redundancy test added.
  `MAINTAINER.md`: the table of files, the diagram, "Civs to edit and the civs of the mod" in place of "Base civs",
  how a mod of several civs is built and named, the three limits of a civ's slot that must stay equal, the redundancy
  tests and where a new one goes, and the mutator's command. `AGENTS.md`: the measure of Requirement 10 touches every
  source first and runs `build.exe coverage redundancy`, and the mutator's command passes `redundancy`. `README.md`:
  the description and the steps of the usage. `DELETION_REQUESTS.md`, which Git now ignores (`.gitignore`): the
  workers that the mutator's runs left, in place of six workers already gone from the disk, and the files of this work
  outside the project. `DECISIONS.md`.
- **Replaced.** "Generic at first", "A design starts from a base civ" and "A base civ of the game is also the replaced
  civ", of
  [2026-11-09 10:07:36](DEVLOG2.md#100736-a-design-begun-from-a-base-civ-generic-at-first);
  "A mod takes the slot of one civ", of
  [2026-11-09 02:15:04](DEVLOG1.md#021504-a-mod-built-from-the-civ-from-the-catalog-and-the-games-own-data-file);
  "No test is written for now", of
  [2026-11-17 10:57:53](#105753-no-test-names-rule-4-alone-the-21-that-did-are-removed).

#### Not done

- Requirement 8 still speaks of the one civ that a mod replaces (question 1).
- No test opens the dialog that asks for the mod's name, and it was not tried by hand, so the three functions of
  `text_dialog.c` stay at CRAP 10 or above (question 3).
- No test builds a mod of two civs or more, and none was tried in the game: the small game of the tests has slots for
  Gaia and the Britons only. Redundancy test 47 commits the requirements of five civs onto one data file, but builds
  no mod.
- The window was not tried by hand.
- Most redundancy tests were written after the code they test, against the order of ub-mode; those not broken by hand
  rest on the mutator's verdict.
- Closing the window asks to save only the civ shown; the drafts of other civs and the civs saved in the mod are lost
  without a question (question 5).
- The count of core tests still breaks Rules 31 and 34, and `BUGS.md` does not exist (Rule 33), as at 10:57:53.
- `bottom_box` in `layout.c`, which this work did not change, checks the size again after `frame_box`, its only
  caller, has checked it, and its `default:` serves no control; both were left as they were.
- The mutator's 23 survivors on lines of this work were read and left, since no user of the game meets the state
  that tells them from the code: `forge.c` 108, a name id of 0, where the game's run from 10102 to 10332; 115, a civ
  list written to exactly 524288 bytes, the room of the one read; 206, a build of two civs (above); `game_text.c` 54
  and 131, a civ read past the last one, which nothing shows; 97, a first line that is " civilization" alone, and
  105, a backslash that ends a line, neither in the game's 62 descriptions; `civ_roster.c`, eleven on slot 0, Gaia's,
  which Save civ refuses as no civ of the game, and one on line 84, past the 64 slots, which only a civ asked for past
  the count reaches, and `civs_are_ready` never asks so; `build_plan.c` 55, a fourth word read past the three and not
  used; `dialogs.c` 200 and 233, a civ copied past the count and not read; and `app_window.c` 84 (question 8).
- Eight uncovered mutants on lines of this work were left, besides the 47 of `text_dialog.c`: two on line 13 of
  `ask.c` and one on line 51 of `build_plan.c`, declarations that hold no code to run; `sync.c` 211, which runs, since
  Redundancy tests 52 and 53 fail when it is broken, but whose condition gcov counts on the lines below it;
  `civ_list.c` 19 and `game_text.c` 32, a game folder too long for a path of 520 characters, which no installed game
  has; `game_text.c` 173, no game installed, where the mutant changes only the result of `game_text_read`, which the
  window does not use; and `forge.c` 116, a civ list written past 524288 bytes, where the game's holds 58763.
- Eight groups of the tests outside `tests/redundancy/` hold more than eight tests, as they did before this work, and
  were left: `run_mod_test` (9) and `run_civ_list_test` (12) of `tests/functional/functional_suite.c`,
  `run_recipe_bonus_test` (14), `run_recipe_bare_test` (12), `run_recipe_plan_test` (9), `run_look_test` (12) and
  `run_bonus_scale_test` (18) of `tests/suites/build_suite.c`, and `run_catalog_test` (11) of
  `tests/suites/core_suite.c`. The crapper does not measure the tests, so no measure shows them.
- Nine runs of the mutator that stopped left their workers in `target/mutation-workers/`; `DELETION_REQUESTS.md`
  lists them to be deleted.
- `sitrep&instructions/sitrep.md` was not brought up to date with this work.

#### Questions

1. **Requirement 8 and several civs.** It speaks of the civ that the mod replaces. It was read as each civ saved in
   the mod replacing its own civ of the game. Proposal: "The mod changes each civ saved in it, in that civ's slot."
2. **Requirement 1 for a civ of the game.** No civ is without a name any more, so the default name `civbuilder_civ`
   was read as the one offered for a civ that keeps the name of its civ of the game. Proposal: word Requirement 1 so.
3. **A test of the dialog of the mod's name.** The tests answer every dialog through `ask_script` and never open one,
   so `text_dialog.c` runs in no test. A test could make the dialog without showing it (`CreateDialogIndirectParamW`
   with no `WS_VISIBLE`), type in it and press OK by messages, as the tests do with the window. Proposal: allow that
   for this dialog, so that its CRAP scores fall below 10.
4. **The mod's name as a folder name.** The name is made into the folders' prefix as the civ's name was: lower case,
   every character but a-z, 0-9, `_` and `-` made `_`, so "My Mod" builds `my_mod-data`; `info.json` keeps the name
   as typed. Proposal: keep it, since the folders must be names that Windows and the game both take.
5. **Closing with several civs.** Proposal: ask also before closing when a civ saved in the mod has not been built
   since, or when a draft of another civ holds edits that no civ file has.
6. **Redundancy tests run after the core.** Rule 32 says they are "only run separately". They run only when
   `redundancy` is given, after the core tests in the same run, so that one command of the mutator runs both.
   Proposal: keep that, or add a word that runs them alone.
7. **Window tests and the installed game.** A window that a test opens without a script reads the tagline and the
   description of an installed game, so they hold the game's text on a computer with the game and nothing on one
   without. No test reads them. Proposal: keep it, or let the tests open the window with a script that names no game.
8. **The chain of actions.** `run_file_action`, `run_other_action`, `run_history_action` and `run_scroll_action` in
   `app_window.c` each say whether they took the action, and the next one is asked only when none did. A `return
   true` made false shows the user nothing, since the action then falls through to `go_to_step`, whose check refuses
   it; so the mutator's survivors there stay, the new one of Save civ and the old ones beside it. Proposal: one
   `switch` on the action, split by kind so that no function reaches CRAP 10, which takes those results out.
9. **The mutator's workers.** Eleven runs stopped on an error of Windows: nine with "being used by another process"
   while the mutator deleted a worker's copy of a file at the end, each leaving its workers, and two with "Permission
   denied" while it wrote a mutant into a worker's copy. Which program held the files was not found. Proposal: keep
   `target/` out of the editor's watcher and indexer and out of the virus scanner, and see whether the runs then end.
10. **The replaced civ of a civ file.** A civ file may name any number from -1 to 99999 as the civ it replaces
    (`is_choice_value`), while the roster has 64 slots. Save civ answers "That civ is not in the catalog." for a civ
    outside them, but the window keeps the draft of the civ shown and asks whether the mod holds it at each refresh
    (`roster_keep` and `roster_holds` in `sync.c`), and there `slot_of`'s check fails, a check meant for faults, not
    for data. Proposal: Open civ file... refuses a file whose replaced civ is not a civ of the catalog, and `slot_of`
    then trusts the slot it is given.
11. **A tech full of the requirements of several civs.** When the civs of one mod with a bonus that adds a
    requirement to the same tech have taken its free places, four in the game of the tests, the next such civ fails
    the whole build with "The civs could not be written into the game data.", while a civ whose tech has no free
    place in the game gets that bonus skipped and counted in the report. A civ's plan reads the tech from the game
    alone, and the places that the civs before it took are seen only when the plan is committed. Proposal: let the
    plan see them, so that the bonus is skipped and the report names it (Req. 9).

## 2026-11-19

### 20:19:34: five steps, and no field for the civ's description

#### What changed

- **Why.** The step Identity was to lose its field for the description, and the nine steps were to become five:
  Identity and Tech tree as "Name and tech tree", Civ bonuses and Team bonus as "Bonuses", and Unique unit, Castle
  tech and Imperial tech as "UU&UT". Hero unit and Review stay as they were.
- **The steps.** `WizardStep` in `wizard.h` is `STEP_NAME_AND_TREE`, `STEP_BONUSES`, `STEP_UU_UT`, `STEP_HERO` and
  `STEP_REVIEW`, with their titles and help lines in `PAGES` of `wizard.c`. A merged step is done once any part of it
  differs from the civ of the game: `differs` in `wizard.c` asks `civ_design_same_identity` and
  `civ_design_same_tree`, both lists of `civ_design_same_bonuses`, and `civ_design_same_unit` and both
  `civ_design_same_tech`; it replaces `differs_early` and `differs_late`. The window opens on the first step
  (`on_create` in `app_window.c`).
- **No field for the description.** `CONTROL_DESCRIPTION` is gone from `ControlId` in `layout.h`, with its line in
  `SPECS`, its help in `HELP_TEXTS` of `control_help.c` and its binding in `BINDINGS` of `binding.c`. The description
  itself stays in the design: the game's is still read into it, a civ file keeps it, the review shows it, and the
  mod's description starts with it when the tagline is empty (`head_text` in `mod_text.c`), as before (question 1).
- **The columns.** At 960 by 560 a column is 388 high, and a field or choice with its label takes 70 of it. Name and
  tech tree: Civ to edit, Name, Tagline, Architecture and Language on the left, which take 382; Wonder, Castle, Monk
  and Scout in the middle; the tech tree's filter, list and "In the tree" on the right. Bonuses: the filter, the
  category and the bonuses on the left, the chosen bonuses in the middle, the team bonuses and the selected one on the
  right. UU&UT: the unique unit, the castle tech and the imperial tech each in a column, each with its list, its name
  and what it is or does, and the unit's description under its name. So that every list keeps 44 or more there, the
  unit's description is 88 high in place of 132 (`PARAGRAPH_HEIGHT`), and "In the tree" and the selected team bonus
  66 (`NOTE_HEIGHT`) in place of stretching. At 960 by 560 the lists are then 184 high for the tree, 206 for the
  bonuses, 262 for the team bonuses, 64 for the units and 125 for each unique tech; at the window's first size, 1200
  by 760, 384, 406, 462, 164 and 225. The texts of the two unique techs, both "What it does" before, are labelled
  "What the castle tech does" and "What the imperial tech does", since they now share a step.
- **The count of steps.** The title, "Step 2 of 5: Bonuses", takes the count from `STEP_COUNT` (`details_title` in
  `details.c`) in place of a written "of 9"; the help of the list of steps says "The steps of a civ." without a count.
- **The menu Go.** In a menu's text a `&` underlines the letter after it, so `append_title` in `menu.c` writes the
  `&` of "UU&UT" twice: the item shows "3 UU&UT", Ctrl+3. Ctrl+1 to Ctrl+5 go to the steps.
- **Tests.** 439, as before, and 64 redundancy tests, up from 62. Tests 287, 288, 290, 293, 294 and 295 test the
  merged steps, under new names: "the name and tech tree step is done once the civ has a name" and "... once a node is
  chosen", "the bonuses step is done once a team bonus is picked", and "the UU&UT step is done once a unit is picked",
  "... a castle tech ..." and "... an imperial tech is picked". Tests 291, 299 and 300 ask the step that now holds the
  tree or the team bonus; Test 298 goes back from Bonuses to Name and tech tree; Test 359 reads "Testonia - Step 2 of
  5: Bonuses - civbuilder"; Tests 12 and 374 to 377 start from the first step; the scroll tests of the tech tree's
  list go to the step that shows it (`layout_spec(CONTROL_TREE_LIST).step`), and Tests 344 to 346 and 348 use the
  first step. Redundancy test 63 checks that the window opens with Civ to edit in focus, and Redundancy test 64 that
  the items of the menu Go read "&1 Name and tech tree\tCtrl+1" to "&5 Review\tCtrl+5", "&3 UU&&UT\tCtrl+3" among
  them; both in `review_tests.c`, in a group of their own after the eight of `run_build_test`.
- **Checked.** `build.exe quick` before the work: 439 tests, only Test 418 failed. The tests were changed first, and
  did not build before the five steps existed. With the code done, the tree, the team bonus and both unique techs
  were taken out of `differs` and the count of the title written as 9: Tests 288, 290, 291, 294, 295, 299 and 359
  failed, and no other but Test 418. With the window opened without its focus and the `&` of a title written once,
  Redundancy tests 63 and 64 failed, and no other but Test 418. Each break was undone, and the diff was then as
  before it. The window was looked at, by pictures of each step at 1200 by 760 and at 960 by 560: every control is in
  its column and none overlaps another; at 960 by 560 the list of units shows a row and a half, and the text of the
  selected unit is cut after its third line (question 2). `build.exe`, the analyzed build: no warning and no finding
  of the analyzer; of its 439 tests only Test 418 failed, which failed before this work too.
- **Measured.** Every source compiled anew, the coverage build with the redundancy tests (before Redundancy tests 63
  and 64), gcovr, then the crapper: 924 functions, and only the three of `text_dialog.c` at 10 or more, as on
  2026-11-18; `differs` 8.0, `append_title` 4.1. The dryer: no two functions 80% alike. The mutator, on the lines
  this work wrote: `wizard.c`, without the window's tests, 22 mutants, all killed; `layout.c`, `binding.c` and
  `control_help.c` hold no mutant on their changed lines, which are tables; `details.c`, with the window's tests, 6
  of the 8 of `details_title` killed, the 2 left on its check of the step's range on line 154, which this work did not
  write; `menu.c` 14 mutants, all survived, since no test read the menu; `app_window.c` line 223, the focus given to
  the first step as the window opens, survived. Redundancy tests 63 and 64 were written for them; the mutator then
  killed 26 of the 28 of `menu.c` and the mutant of line 223, and 2 of the 4 of `focus_step`. Those left are under
  Not done.
- **A run that took the computer.** The mutator runs a worker for each core, and with the window's tests each worker
  opens the real window again and again. One such run, on `menu.c`, left the computer unusable, and was ended by
  shutting the computer down; its workers are in `DELETION_REQUESTS.md`. It was made again when the computer could be
  given over to it. A run of the mutator with the window's tests, or a build without `nowindow`, is now started only
  when the computer is free for it. A first run on `menu.c` had stopped on a baseline that failed one test in a
  worker; the same tests then passed in the project's folder.
- **Documents.** `TESTS.md`: the entries of the tests changed, the lines of Redundancy tests 49 to 62, which moved
  down by three, and Redundancy tests 63 and 64. `TERMINOLOGY.md`: step and wizard changed, UU&UT added.
  `MAINTAINER.md`: the five steps in the table of files, the order of a step's controls and the room of a column in
  "Adding a control to a step", the first step's name, that no step has a field for the description, and the tests
  of `review_tests.c`. `README.md`: the steps, the tagline alone where the tagline and description were named,
  and Ctrl+1 to Ctrl+5. `DELETION_REQUESTS.md`: the workers of the run that was stopped, and one more found unlisted.
  `AGENTS.md`: to ask before a run that opens the window. `DECISIONS.md`.
- **Replaced.** The nine steps, of
  [2026-11-09 01:20:32](DEVLOG1.md#012032-the-nine-steps-in-a-window-with-undo-a-review-and-accessibility-tests);
  "Identity" as the first step, with a field for the description, of
  [2026-11-18 07:24:48](#072448-civs-of-the-game-edited-and-saved-into-one-named-mod); the steps that a change
  marks done, of
  [2026-11-16 21:18:51](DEVLOG5.md#211851-no-step-shown-as-done-before-the-user-changed-it); the title "Step 2 of 9:
  Tech tree", of
  [2026-11-09 10:27:22](DEVLOG2.md#102722-a-windows-title-that-names-the-civ-and-the-step-and-the-base-civ-tried-in-the-window).

#### Not done

- At 960 by 560 the list of units is 64 high, a row and a half, and the text of the selected unit, 64 high, shows
  three of the five lines of the Longbowman; at 1200 by 760 both are 164 and show it whole (question 2).
- The description stays in the design, the civ file and the review, though no field shows it (question 1).
- The mutants left on lines of this work cannot be told from the code by anything the user sees: the bound of 24
  letters of `append_title` made 25, which no title reaches; the id of the separator of the menu Go made 1, which
  Windows does not read; and in `focus_step`, the loop begun at the second control or run one past the last, since
  the first control is the list of steps, which no step holds, and every step has a control that takes the focus.
  The 2 of `details_title` are on its older check of the step's range.
- The window was seen in pictures, not used by hand, and not at a text size of 200%.
- Test 418 still fails, on the first line of `TERMINOLOGY.md` (question 3 of 2026-11-17 10:57:53).
- The count of core tests still breaks Rules 31 and 34, as before.
- `sitrep&instructions/sitrep.md` was not brought up to date with this work.

#### Questions

1. **The description of a civ.** Only its field is gone. The design still takes the game's description, a civ file
   keeps it, the review shows it under "Description:", and when the tagline is empty the mod's description starts
   with it, so a text the user cannot see in any step can reach the game. Proposal: take the description out of the
   design too: the review no longer shows it, a civ file is written with an empty one and its own is not read, the
   game's is no longer read, and an empty tagline gives the mod's description the civ's name.
2. **UU&UT at 960 by 560.** Three lists, three names, the unit's description and three texts share three columns of
   388: the list of units gets a row and a half, and the text of the selected unit is cut. Proposal: let the texts
   beside the lists scroll, as the review's text does, so that nothing is cut at any size; or let the wide layout
   need 680 in height in place of 560, below which the step shows in one column and scrolls.

### 21:28:52: a turn of the wheel that cannot scroll the step does nothing

#### What changed

- **Why.** A bug was reported: a turn of the wheel outside the lists, such as between two of them, made the whole
  window flicker and blink without end, where nothing at all was to happen. Windows sends such a turn to the window
  itself, and `assist_scroll` in `assist.c` placed every control again for each notch (`controls_place`), whether the
  step moved or not. Placing them wraps the rows of every list again and has the whole window erased and drawn again
  (`assist_placed`). A step that fits the window, as every step does in the wide layout, cannot scroll at all, so
  each notch only drew the window again; a fine wheel or a touch pad sends many notches a second.
- **The scroll.** `scroll_to`, new in `assist.c`, keeps the scroll within the step (`reflow_scroll_within`, with the
  step's height as the window's scroll bar holds it, read by `page_height`) and places the controls again only when
  the scroll changes. `assist_scroll` hands it the scroll of the wheel, of the window's scroll bar, and of Scroll up
  and Scroll down of the menu Go; `show_focus` hands it the scroll that shows the control in focus, which it placed
  itself before. Where the step fits, or is already at the end the wheel turns to, nothing happens.
- **BUGS.md.** `documentation/BUGS.md` is new (Rule 33): the table of the bugs found by a human, with this one,
  discovered 2026-11-19 21:10:01, the time it was recorded, and "Covered by test 349". The bugs of 2026-11-10 and
  2026-11-16 are not in it, since question 4 of
  [2026-11-17 10:57:53](#105753-no-test-names-rule-4-alone-the-21-that-did-are-removed) is still open.
  `documentation/` holds 7 files in Git with it; `DELETION_REQUESTS.md`, which Git leaves out, is an eighth on the
  disk.
- **Tests.** 440, up from 439, and 65 redundancy tests, up from 64. Test 349, in `scroll_tests.c`, is new: at the
  window's first size, a notch of the wheel sent to the window leaves it nothing to redraw (Bug 1 of 2026-11-19). It
  shows the window far off the screen, as the scroll tests before it do, since Windows keeps nothing to redraw for a
  hidden window. Tests 349 to 439 are now 350 to 440; the test of `TERMINOLOGY.md`'s lines, Test 418 before, is Test
  419. Redundancy test 65, in `review_tests.c`, is new: a notch of the wheel while the window is minimized, at 480 by
  360, leaves the step unscrolled once the window is shown again.
- **Checked.** Test 349 was written first and failed, the window having all of it to redraw after the notch; with
  `scroll_to` it passed. With the check of the view in `assist_scroll` made `view >= 0` by hand, Redundancy test 65
  failed, and no other test but Test 419; the check was put back from a copy. `build.exe quick redundancy`: 505
  tests, only Test 419 failed, which failed before this work too, as Test 418. `build.exe`, the analyzed build: no
  warning and no finding of the analyzer; of its 440 tests only Test 419 failed.
- **Measured.** Every source compiled anew, the coverage build with the redundancy tests (before Redundancy test 65),
  gcovr, then the crapper: 926 functions, only the three of `text_dialog.c` at 10 or more, as before; `scroll_to`
  2.0, `page_height` 1.0, `assist_scroll` 3.1, `show_focus` 4.1. The dryer: no two functions 80% alike. The mutator,
  with the window's tests, on the lines of `assist.c` this work changed: `page_height` 2 mutants, `scroll_to` 1, both
  killed; `assist_scroll` 4 of 6 killed, the 2 left on its check of the view, `CHECK(view > 0)`, which this work moved
  but did not write: `>` made `>=`, and `0` made `1`. Redundancy test 65 was written for the first, and the mutator
  then killed it. The second cannot be told from the code: it differs only in a window 1 unit high, and the window is
  never less high than `REFLOW_MIN_HEIGHT`, nor less than 0 when minimized.
- **Documents.** `TESTS.md`: Test 349, the numbers of Tests 350 to 440, and Redundancy test 65. `BUGS.md`, new.
  `MAINTAINER.md`: what `assist_scroll` and `scroll_to` do, `BUGS.md` in the table of files, that a test of a bug
  found by a human has its row in `BUGS.md`, that a test of the window shows it only far off the screen and only when
  what it checks needs that, and the test of the minimized window among those of `review_tests.c`. `DECISIONS.md`:
  the decisions below, and Test 351 named as Test 352. `DELETION_REQUESTS.md`: the scratch files of this work; and
  the worker `run-a373bbe6`, listed at 20:19:34, taken out, since it is no longer on the disk.

#### Not done

- The fix was not tried by hand with a mouse wheel or a touch pad; Test 349 sends the notch to the window as Windows
  does.
- In one column, below 960 by 560, a notch that moves the step still places every control again, wraps the rows of
  every list again and has the whole window erased and drawn again, as every notch did before; whether the step
  blinks as it scrolls there was not looked at (question 2).
- `BUGS.md` holds only this bug (question 4 of 2026-11-17 10:57:53).
- The core has 440 tests, where Rule 31 allows about 54 (question 1).
- Test 419 still fails, on the first line of `TERMINOLOGY.md` (question 3 of 2026-11-17 10:57:53).
- `sitrep&instructions/sitrep.md` was not brought up to date with this work.

#### Questions

1. **A test of a bug reported from now on.** The core already holds far more tests than Rule 31 allows. Test 349 was
   added to the core, since the new bug raises what Rule 31 allows by one, and Rule 34's category Regression is for
   tests ordered to cover further what a test covers already. Should a test of a newly reported bug go to the core,
   as here, or to Regression? Proposal: the core, as here.
2. **A step that scrolls.** In one column the wheel still has the whole window erased and drawn again at each notch
   that moves the step. Proposal: let the window leave its controls alone when it is erased (`WS_CLIPCHILDREN`), and
   wrap the rows of the lists again only when the window's width changes, not when the step scrolls.
