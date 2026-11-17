# Development log, part 7

- [2026-11-17](#2026-11-17)

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
