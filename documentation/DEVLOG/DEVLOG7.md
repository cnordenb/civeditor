# Development log, part 7

- [2026-11-17](#2026-11-17)
- [2026-11-18](#2026-11-18)
- [2026-11-19](#2026-11-19)
- [2026-11-20](#2026-11-20)

Part 7 of the development log. It follows [part 6](DEVLOG6.md) and goes on in [part 8](DEVLOG8.md). The decisions
made in the log are listed in [DECISIONS.md](../DECISIONS.md).

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

## 2026-11-20

### 03:00:57: the hero is chosen in UU&UT, with a Hero box and Choose hero...

#### What changed

- **Why.** It was asked that the step Hero unit go, and that the hero be chosen in UU&UT instead: a box Hero left of
  Back; once it is ticked, a button Choose hero... left of the box, with the hero's name above it, Attila the Hun at
  first; Choose hero... opens a list of the heroes, Attila the Hun picked at first, and a hero picked there keeps its
  name in the game unless another is typed. The fix of
  [21:28:52](#212852-a-turn-of-the-wheel-that-cannot-scroll-the-step-does-nothing) was found to work by hand.
- **How the request was read.** Three points were asked before the work and answered: the name of the hero is typed
  in the dialog of Choose hero..., under the list; the name above Choose hero... is the name given to the hero,
  "Attila the Hun" at first; and the tests that open the window, with the mutator, could be run for this change.
- **The steps.** `STEP_HERO` is gone from `wizard.h`: the wizard has four steps, UU&UT the third and Review the
  fourth, and the menu Go, Ctrl+1 to Ctrl+4 and the window's title follow. UU&UT is done once its unique unit, a
  unique tech or the hero differs from the base civ's (`differs` in `wizard.c`), and its help says to tick Hero for a
  hero.
- **The controls.** `CONTROL_HERO_LIST` is gone. `CONTROL_HERO_NAME` is now a text, the hero's name;
  `CONTROL_HERO_CHOOSE` is the button Choose hero...; `CONTROL_HERO` is the Hero box, of the new kind
  `WIDGET_CHECKBOX`, a `BUTTON` of style `BS_AUTOCHECKBOX`. The three are `PLACE_FRAME` controls of UU&UT: `hero_box`
  in `layout.c` puts the box left of Back, Choose hero... left of the box, and the name above Choose hero... and the
  box. The status line of UU&UT ends before them (`bottom_box`), and its columns end above the name (`foot_rise`): 362
  high at 960 by 560 instead of 388. `layout_box` takes the step for this, as `reflow_box` does. In one column the
  three come after the imperial tech, in that order. `layout_names_itself` tells the controls that show their label
  themselves, a button or a box; `layout_needs_hero` the two that show only while the civ has a hero
  (`civ_design_has_hero`, new in `civ_design.c`). `controls_show` in `controls.c` shows each control by its step and
  by the hero, and `sync_details` calls it after every change, as it ticks or unticks the box to match the design.
  The box is drawn on the paper of the window, not on the colour of fields (`is_field`).
- **The hero.** Ticking the box makes the command of `hero_box_command` in `binding.c`: the hero `HERO_AT_FIRST`,
  unit 777, Attila the Hun, named as the catalog names him; unticking it, no hero. The heroes no longer have a row
  "No hero", and `LIST_NO_ITEM` is gone. They are the rows of `CONTROL_HERO_CHOOSE`, which `sync_choices` leaves
  alone, a button having no rows to show. The name above Choose hero... is the hero's name field (`details_of`).
- **Choose hero....** `dialogs_choose_hero` in `dialogs.c` lists the heroes in the dialog of `text_dialog.c`, which
  now takes an `AskQuestion` (`shell.h`): a question, its answer and, when it has rows, a list above the field. The
  civ's hero is picked at first and the field holds its name; picking a row puts the row's text, the hero's name in
  the game, in the field (`take_row`); OK picks the hero of the row with the name in the field, as one command that
  undo takes back; Cancel, or OK with the field empty, changes nothing. The list has a classic scroll bar, as every
  other. The question of the mod name is the same dialog without the list. The dialog's procedure is split into
  `answer_command` and `end_answer`. The field holds as much as the hero's name does in the design
  (`CIV_NAME_CAPACITY`): at first it held 64 characters, and a civ file whose hero had a longer name left Choose
  hero... doing nothing.
- **Tests.** 444, up from 440, and 72 redundancy tests, up from 65. Tests 356 to 359, in the new
  `tests/shell/hero_tests.c`, are new: ticking Hero names Attila the Hun above Choose hero... (356); unticking it
  hides Choose hero... (357); a hero picked in Choose hero..., Saladin, keeps his name in the game (358); a name typed
  there, Testhero, is the hero's (359). Tests 356 to 440 are now 360 to 444; the test of `TERMINOLOGY.md`'s lines,
  Test 419 before, is Test 423. Test 272 checks that unticking Hero gives the command to pick no hero, instead of the
  row "No hero"; Test 296 that a hero finishes UU&UT; Test 271 a row of Choose hero...; Test 347 fits the window to
  the Review, now the shortest step, instead of the hero step; Test 364 reads "Step 2 of 4" in the window's title.
  Redundancy test 11 unticks Hero instead of picking the row "No hero", Redundancy test 10 picks from the rows of
  Choose hero..., and Redundancy test 64, now 65, reads four steps in the menu Go. The tests that walk every control
  of every step (Tab, names, help and the frame of focus) tick Hero first, so that Choose hero... is among the
  controls they reach, and the test of the reading order reads the bottom row left to right after the columns.
  `window_fixture.c` ticks a box, clicks a button, and answers Choose hero...: a hook on the test's thread
  (`WH_CALLWNDPROCRET`) picks the row, types the name and presses OK once the dialog has started (`WM_INITDIALOG`),
  so that the real dialog runs and closes before Windows shows it.
- **Redundancy tests.** Seven are new, each for mutants that survived the tests above. Redundancy test 12: a hero
  whose name is emptied lacks a name (`civ_design_hero_lacks_name` reading the second byte of the name instead of the
  first: the bytes of an earlier name are left after the end of an emptied one). In `review_tests.c`, Choose hero...
  lists each of the 30 heroes of the catalog once (67: a row less, or an empty row more); its list is labelled
  "Heroes" (68: a list with no label, or the label's text given to the list); it opens with the focus on its list
  (69: the focus on the field); a name typed there stays when the list takes the focus again (70: every message of
  the list taking its row); its first row, Cao Cao, can be picked like any other (71: row 0 refused or left empty);
  and a name of one letter is taken (72: the second letter checked instead of the first). Redundancy tests 12 to 65
  are now 13 to 66. To see what the dialog showed, `window_fixture.c` keeps its rows, the label of its list and
  whether the list had the focus (`window_fixture_asked`), and `window_fixture_refocus` makes it give the list the
  focus again before OK.
- **Checked.** Test 296 failed before `differs` took the hero in, and passed after. Each of Tests 356 to 359
  failed alone when the code was broken on purpose: ticking Hero picking unit 929 instead of 777 (356), Choose
  hero... shown without a hero (357), the picked row not put in the field (358), the typed name left out (359); each
  break was undone. Test 359, changed for a while to open a civ file whose hero has a name of 100 characters and pick
  Saladin, failed before the field was widened and passed after; it was put back as it was, byte for byte. The
  eleven mutants the redundancy tests were written for, put in by hand one at a time, each failed only its own test,
  and each file was put back. The window was opened at 960 by 560 and pictured: on UU&UT, unticked, only the Hero box
  shows left of Back; ticked, Choose hero... shows left of it, with "Attila the Hun" above; Choose hero... opens the
  list "Heroes", from Cao Cao, with Attila the Hun picked and his name in the field under it, then OK and Cancel.
  `build.exe quick norules redundancy`: 491 tests, none failed. `build.exe`, with the analyzer: no warning, 444
  tests, and Test 423 failed, on the first line of `TERMINOLOGY.md`, as Test 419 did before.
- **Measured.** Every source compiled anew, the coverage build with the redundancy tests (before the field of
  Choose hero... was widened and before Redundancy tests 12 and 67 to 72), gcovr, then the crapper: no function at 10
  or more. The three of `text_dialog.c` at 10 or more before are now run by the tests: `answer_proc` 3.0 (56 before),
  `text_dialog_show` 4.1 (30), `put_text` 3.1 (12). Of the functions this work changed, the highest are
  `dialogs_choose_hero` 8.1, `sync_details` 8.0, `differs` 8.0, `frame_box` 7.8, `sync_on_notify` 7.2, `details_of`
  7.0 and `column_of` 7.0. The dryer: no two functions 80% alike. The mutator, with the window's tests and
  `--timeout-factor 40`, on every line this work changed in 12 files: 91 mutants killed, 47 survived, and 1 on a line
  no test runs. Of the 47, 10 change an argument that Windows does not read (`text_dialog.c` 144, 146, 158 twice, 160
  once, 174 twice; `sync.c` 210, 374 twice); 9 a start value that is always set again before it is read (`ask.c` 37
  twice; `dialogs.c` 285, 286 twice, 302 three times, 303); 7 a bound that is never reached (`text_dialog.c` 160
  twice, a row of 64 characters or no row picked, and 207, a capacity of 1; `dialogs.c` 290 three times, rows past
  the end of the list, which are not shown, and 308, `LB_GETCURSEL` giving the count of rows); 14 the places and sizes
  of the dialog (`text_dialog.c` 101 to 114, question 2); 2 the dialog without a list and Cancel (`text_dialog.c` 125,
  the count of controls of the question of the mod name, which every test answers through `ask_script`, and 213,
  Cancel taking the field, which for Choose hero... gives back the hero it had); 4 in `layout.c`: 101, a label's
  second letter, where every label is empty or longer than one letter, 139, the hero's name 16 wider into the room
  above Back, and 223 twice, the columns 8 lower, then touching the name, and those of the other steps 1 lower; and 1
  in `controls.c`, 275, the list of steps drawn on the paper instead of the colour of fields, a check older than this
  work, where no test reads colours. Not run: `dialogs.c` 309, Cancel in Choose hero..., which no test presses.
- **Documents.** `TESTS.md`: Tests 356 to 359, the numbers of Tests 360 to 444, the entries of Tests 271, 272, 296,
  347 and 364; Redundancy tests 12 and 67 to 72, the numbers of Redundancy tests 13 to 66, the entries of Redundancy
  tests 10, 11 and 65, and the lines of the tests in the files changed. `MAINTAINER.md`: four steps, the dialog with a
  list in `src/shell/`, the bottom row of UU&UT, how the hero is chosen, how a test answers Choose hero... and reads
  what it showed, `hero_tests.c`, and the dialog of Choose hero... among the tests of `review_tests.c`.
  `TERMINOLOGY.md`: step, UU&UT and wizard changed; Hero box and Choose hero... new. `README.md`: the steps, Ctrl+1 to
  Ctrl+4, and that a civ has no hero until Hero is ticked. `DECISIONS.md`: the decisions below.
  `DELETION_REQUESTS.md`: the scratch files of this work and the worker `run-2b24c3bb`.

#### Obstacles met, and how they were solved

- **A baseline that failed once.** The mutator's first run of `dialogs.c` stopped before any mutant: its baseline,
  the tests without a mutant, had 1 failure in 484, and the mutator printed only the end of the output, which does
  not name the test. The same tests passed in the baseline of the next file and in every run since, alone and eight
  side by side. Which test failed was not found.
- **Tests changed while the mutator ran.** A worker of the mutator copies only the file it mutates and links the
  rest of the project, `tests/` too. The second run of `dialogs.c` was stopped at 00:32 once the tests it builds were
  being changed, and it was run again after.
- **Mutants reported killed that no test kills.** In the second run of `text_dialog.c`, 14 mutants of the dialog's
  places and sizes were reported killed, though two of them, put in by hand, failed no test. They were the first 16
  mutants of the run, run at once by 16 workers that each ran the window's tests, and a mutant's time is limited to
  ten times the baseline, which runs alone. A mutant that runs out of time is reported killed, as one that fails a
  test is. With `--timeout-factor 40` the 14 survived. Every line this work changed was then measured again with
  `--timeout-factor 40` and `--mutate-all`. Of the mutants on those lines reported killed before, 10 of the first run
  (9 of `text_dialog.c`, 1 of `layout.c`) and 28 of the second (14 of `text_dialog.c`, 14 of `dialogs.c`) survived
  it. Five on lines this work did not change, in `civ_design.c`, `dialogs.c` and `sync.c`, were reported killed in
  the first reports and survived in the last (question 3). The eight runs side by side, all passed, showed that no
  test fails from the load alone.

#### Not done

- The 47 mutants that survive, listed under Measured (questions 2 and 3).
- Cancel in Choose hero... is pressed by no test. OK with the field empty closes the dialog and changes nothing,
  without a word (question 4).
- Whether the focus is lost when an undo unticks Hero while Choose hero... has the focus was not checked (question 5).
- The question of the mod name, the same dialog without the list, is shown by no test: every test answers it through
  `ask_script`.
- Which test failed once in the mutator's baseline of `dialogs.c` was not found.
- At 960 by 560, the text of the selected unit in UU&UT is cut after about two of its five lines. Its column is 26
  lower than before, so it shows about one line less than it did.
- In the picture of UU&UT taken after the window was made 960 by 560, the picked castle tech and imperial tech were
  half out of sight in their lists; this was not looked into.
- The crapper and the dryer ran before the field of Choose hero... was widened and before the last redundancy tests;
  the widening changes no path of a function, and the dryer passes by the tests.
- Choose hero... was not tried by hand with the mouse, the keyboard or a screen reader; the tests drive it and the
  pictures show it.
- The core has 444 tests, where Rule 31 allows about 54 (question 1 of
  [21:28:52](#212852-a-turn-of-the-wheel-that-cannot-scroll-the-step-does-nothing)).
- Test 423 still fails, on the first line of `TERMINOLOGY.md` (question 3 of 2026-11-17 10:57:53).
- `redundancy_suite.c` has 380 of the 400 lines a file may have (Rule 13).
- `sitrep&instructions/sitrep.md` was not brought up to date with this work.

#### Questions

1. **The mutator's time limit.** With its default, ten times the baseline, the mutator reported mutants killed
   that fail no test, when 16 workers ran the window's tests at once. Proposal: add `--timeout-factor 40` to the
   command of Requirement 12 in `AGENTS.md`.
2. **The places and sizes of the dialog.** No test checks the dialog of Choose hero... as the steps of the window are
   checked: that its targets are 44 by 44 and that no two of its controls overlap (Req. 3); 14 mutants of its places
   and sizes survive. Whether its controls are 44 by 44 now was not measured. Proposal: two accessibility tests
   (Rule 32) that read the dialog through `window_fixture_asked`.
3. **Mutants measured before with the default limit.** Five mutants on lines this work did not change
   (`civ_design.c` 36 and 149, `dialogs.c` 37, `sync.c` 74 and 139) were reported killed in the first reports of this
   work and survived in the last, and the files measured before may hold more. Proposal: measure every folder again
   with `--timeout-factor 40 --mutate-all`, the folders of the window when the computer can be left to it for some
   hours.
4. **OK with the name empty.** OK in Choose hero... with the field empty closes the dialog and changes nothing,
   without a word. Proposal: OK greyed out while the field is empty.
5. **The focus when Choose hero... hides.** An undo that unticks Hero hides Choose hero..., which may have the focus.
   Proposal: check what happens, and give the focus to the Hero box when Choose hero... hides while it has it.

### 03:59:19: the project named civeditor

#### What changed

- **Why.** The project was renamed from civbuilder to civeditor. Every mention of the old name that names this
  project was to follow it; the history and the names of other things were not.
- **The program.** The window's title ends in " - civeditor" (`details_window_title` in `details.c`); the dialogs and
  message boxes are titled civeditor (`text_dialog.c`, `dialogs.c`); the window's class is `civeditor.window` and the
  first mod name `civeditor_mod` (`app_window.c`); `build_plan.c` builds `build\civeditor.exe`.
- **Civ files.** A civ is saved as `<name>.civeditor.json`, and a civ without a name of its own as
  `civeditor_civ.civeditor.json` (`suggest_name` in `dialogs.c`). Open civ file... names `*.civeditor.json` and
  `*.civbuilder.json` and lists every `*.json` file, as `*.civbuilder.json;*.json` did before, so the files saved
  before the rename open as before.
- **Kept.** The format `civbuilder_v1`, in which the original wrote civ files and which `civ_file_read.c` still reads;
  the original's example civ in that format, `tests/fixtures/example.civbuilder.json`; and Krakenmeister's
  civbuilder in `README.md`, where the catalog of bonuses comes from. The sections of the log written before, their
  titles and the links to them keep the old name, as the log is not rewritten; so do the notes in `materials/`.
- **Tests.** 444 tests and 73 redundancy tests, up from 72. Tests 323, 324, 326 and 364 and Redundancy tests 58 and
  62 expect the new name, and two are renamed with it: `test_a_new_civ_is_saved_as_civeditor_civ_by_default` (Test
  323) and `test_a_mod_is_named_civeditor_mod_unless_named_otherwise` (Redundancy test 58). Redundancy test 73 saves
  a civ named "X" and expects `build\X.civeditor.json`; it and Redundancy test 62 share `saves_as` in
  `review_tests.c`.
- **Checked.** With the tests changed first, `build.exe quick norules redundancy` ran 491 tests and 6 failed, each on
  the old name: Tests 323, 324, 326 and 364, and Redundancy tests 58 and 62. With the code changed and `build.exe`
  built again, all 491 passed. Of all the lines changed, the mutator finds something to change only on line 104 of
  `dialogs.c`. With the window's tests and `--timeout-factor 40`, of its 2 mutants, `!=` made `==` was killed, and
  `name[0]` made `name[1]` survived: no test saved a civ whose name has one letter. Redundancy test 73 was written
  for it. It passed, and with that mutant put in by hand it failed, the only test to; the mutant was taken out.
  Run again, the mutator killed both. Of seven runs of all the tests, Test 390 failed in one, at "Description of
  the unique unit", and passed in the other six. `build.exe`, with the analyzer: no warning, 444 tests, and only
  Test 423 failed, on the first line of `TERMINOLOGY.md`, as before.
- **Documents.** `README.md`: the name, the links to GitHub, `.civeditor.json` files and that `.civbuilder.json`
  files open, `civeditor_civ`, `civeditor_mod`, `build\civeditor.exe` and the Acknowledgements. `TERMINOLOGY.md`:
  the ten lines that named the program. `TESTS.md`: Tests 323 and 364, Redundancy tests 58 and 62, the lines of the
  tests of `review_tests.c` after Redundancy test 62, and Redundancy test 73. `MAINTAINER.md`, `AGENTS.md`,
  `.claude/skills/rules-check/SKILL.md`, `.github/ACCESSIBILITY.md`, `.github/CONTRIBUTING.md`, `.github/SECURITY.md`
  and `.github/ISSUE_TEMPLATE/feature_request.md`: the name. `DECISIONS.md`: the decisions below, the name, and the
  example of the window's title, which still had five steps. `DELETION_REQUESTS.md`: what the build and the tests
  wrote under the old name.
- **Rules.** Rule 1 names civeditor; the change of `RULES.md` came with the rename.
- **Replaced.** The default name `civbuilder_civ.civbuilder.json`, of
  [2026-11-09 10:06:18](DEVLOG2.md#100618-rules-26-to-36-settings-that-deny-commits-and-deletions-and-version-100), and
  the first mod name `civbuilder_mod`, of
  [2026-11-18 07:24:48](#072448-civs-of-the-game-edited-and-saved-into-one-named-mod).

#### Not done

- `REQUIREMENTS.md` was changed with the rename, not by this work (Rule 2): Requirement 1 says `civeditor_civ` and
  Requirement 3 "Civ Editor", while Requirement 4 still says "Civ Builder" (question 1).
- The project's folder is still named `civbuilder`, and so is the repository on GitHub until it is renamed; the links
  in `README.md` and `.github/SECURITY.md` lead to `cnordenb/civeditor`. `.vscode/launch.json`, which Git ignores,
  names the folder in its paths.
- Why Test 390 failed once was not found; the rename changed nothing that it reads. It may be the test that failed
  once, unnamed, in a baseline of the mutator in
  [03:00:57](#030057-the-hero-is-chosen-in-uuut-with-a-hero-box-and-choose-hero).
- Open civ file... was not tried by hand with a `.civbuilder.json` file, and no test opens a file through it.
- `.github/ACCESSIBILITY.md` gives the window's title as "Testonia - Step 2 of 9: Tech tree - civeditor", from when
  the wizard had nine steps; only the name in it was changed. `AGENTS.md` and `MAINTAINER.md` name
  `sitrep&instructions/` in the main folder, which is now `materials/sitrep&instructions/`, ignored by Git
  (question 2).
- The crapper and the dryer were not run again: the code changed only in its texts, and the new function, `saves_as`,
  is in `tests/`, which they pass by.
- The questions of [03:00:57](#030057-the-hero-is-chosen-in-uuut-with-a-hero-box-and-choose-hero) are still open.

#### Questions

1. **Requirement 4.** It says that when every item of the checklist holds, "Civ Builder is considered" WCAG 2.2 AA
   compliant; Requirement 3 now says "Civ Editor". Proposal: Requirement 4 says "Civ Editor" too.
2. **Two stale pointers.** The title in `.github/ACCESSIBILITY.md` has nine steps, where the wizard has four, and
   `AGENTS.md` and `MAINTAINER.md` send the reader to `sitrep&instructions/`, now in `materials/`, which Git ignores,
   so that a clone of the project has no such folder. Proposal: the title of Test 364, "Testonia - Step 2 of 4:
   Bonuses - civeditor", in `ACCESSIBILITY.md`; and either `materials/` tracked by Git, or the pointers to it taken
   out of `AGENTS.md` and `MAINTAINER.md`.

### 04:06:25: the documents point at no file that Git ignores

#### What changed

- **Answers.** Of the questions of 03:59:19, in [the project named civeditor](#035919-the-project-named-civeditor):
  the pointers to the notes that Git ignores are taken out, and the log records what the notes asked; the title in
  `.github/ACCESSIBILITY.md` is brought up to date (2). Requirement 4 now says "Civ Editor", as question 1
  proposed; `REQUIREMENTS.md` was changed with it, not by this work (Rule 2).
- **`AGENTS.md`.** The stack names Rule 29 as its source, and the list of what to read before a change ends with
  `MAINTAINER.md`: the item that sent the reader to the instructions, which a clone of the project lacks, is taken
  out. What it said, P10, Head First Design Patterns, Clean Code, and C and Win32 only, is Rules 26 to 29.
- **`MAINTAINER.md`.** The table of files and folders has no row for the folder of the notes, which is not in the
  main folder and which Git ignores.
- **`DECISIONS.md`.** The decision that kept the notes in a folder of their own now says that the documents point at
  no file that Git ignores. Two decisions say what the notes asked without naming their files, and the decision on
  the sections written before the rename speaks of notes that Git ignores, without naming their folder.
- **`.github/ACCESSIBILITY.md`.** The example of 2.4.2 is the title of Test 364, "Testonia - Step 2 of 4: Bonuses -
  civeditor", in place of a title of nine steps, and 1.4.4 asks for a walk through the four steps, not nine.
- **Tests.** 444 tests and 73 redundancy tests, as before; only documents changed.
- **Checked.** `build.exe quick nowindow redundancy` ran 433 tests, and only the test of the first line of
  `TERMINOLOGY.md` failed (Test 423 when the window's tests run), as before. `git grep` finds the notes, their files
  and their folder named in no file Git tracks but the log and `.gitignore`.

#### Not done

- The sections of the log written before still name the notes and their files, 93 times over the seven parts: the
  log is not rewritten.
- The reading of 2.4.6 in `.github/ACCESSIBILITY.md`, dated 2026-11-10, still counts the nine titles of the steps
  of then; it records what was read that day.
