# Decisions

- [Rules, and how they are read](#rules-and-how-they-are-read)
- [Requirements, and how they are read](#requirements-and-how-they-are-read)
- [Code and files](#code-and-files)
- [Tests](#tests)
- [Documents](#documents)

The decisions recorded in the development log in [DEVLOG/](DEVLOG/DEVLOG1.md), by subject (Rule 23). Each ends with the
day it was made, and the time where the log has one, as a link to its entry in the log. Where a later decision replaced
an earlier one, the earlier one is named after it, with its own link.

## Rules, and how they are read

- **Rule 1 names civeditor,** the project's name. The change of `RULES.md` came with the rename.
  [2026-11-20 03:59:19](DEVLOG/DEVLOG7.md#035919-the-project-named-civeditor)
- **The rules of `RULES.md` hold, Rules 0 to 30.** Rules 0 to 25 are repo_template's, as amended outside the log,
  and Rules 26 to 30 hold what the project's instructions ask; Rules 10 to 13 apply to code only (Rule 25).
  Requirements 10 to 12 and ub-mode's skill ask for what Rules 31 to 36 asked. Replaced: Rules 0 to 23, 25 and 26 of repo_template;
  Rules 31 to 36, derived from ub-mode.
  [2026-11-09 00:44:18](DEVLOG/DEVLOG1.md#004418-civbuilder-begun-in-c-with-its-own-build-and-rule-tests)
  [2026-11-09 10:06:18](DEVLOG/DEVLOG2.md#100618-rules-26-to-36-settings-that-deny-commits-and-deletions-and-version-100)
  [2026-11-16 21:03:39](DEVLOG/DEVLOG5.md#210339-the-answers-recorded-no-cap-on-tests-p10s-bounds-and-check-count-given-up-aa-only-coverage-measured)
- **ub-mode as its skill says, as far as Rules 0 and 30 allow.** Its steps that commit, push or make worktrees are
  left out. Replaced: its rules as Rules 31 to 36, kept by breaking the code by hand.
  [2026-11-09 10:06:18](DEVLOG/DEVLOG2.md#100618-rules-26-to-36-settings-that-deny-commits-and-deletions-and-version-100)
  [2026-11-16 21:03:39](DEVLOG/DEVLOG5.md#210339-the-answers-recorded-no-cap-on-tests-p10s-bounds-and-check-count-given-up-aa-only-coverage-measured)
- **The crapper measures Rule 32.** `build\crapper.exe`, written in C in `tools/crap/`, scores every function:
  complexity is 1 and one more for each `if`, `for`, `while`, `case`, `&&`, `||` and `?`, the branches of `CHECK`
  among them; coverage is the share of branches that gcov, which comes with gcc, saw taken at least once by the
  tests, or of lines for a function without branches; a function outside the test program counts as not run.
  `build.exe` makes the crapper and does not run it until no function is at 10 or above.
  [2026-11-10 04:22:28](DEVLOG/DEVLOG3.md#042228-the-crapper-in-c-and-369-functions-at-crap-10-or-above)
- **The dryer measures Rule 31.** `build\dryer.exe`, written in C in `tools/dry/`, takes a function's shape as the
  set of its runs of four tokens, in which a call keeps its name and every other name and literal loses it, and
  ranks two functions by the runs they share among all runs of either. It compares functions of 4 lines and 20
  tokens or more, the tests among them, and reports pairs at 0.8 or above. `build.exe` makes it and does not run
  it until no pair is left.
  [2026-11-10 04:27:23](DEVLOG/DEVLOG3.md#042723-the-dryer-in-c-and-62-pairs-of-functions-alike)
- **The mutator measures Rule 34.** `build\mutator.exe`, written in C in `tools/mutate/`, makes each mutant of the
  lines of `src/` that the tests run, one operator or constant changed, links the test program with it and runs the
  tests, twelve at a time, each ended after 30 seconds. `>` and `>=` are swapped besides the swaps Rule 34 names.
  Replaced: mutation by hand.
  [2026-11-10 04:37:03](DEVLOG/DEVLOG3.md#043703-the-mutator-in-c-tried-on-one-folder)
- **The mutator leaves the window out.** It runs the tests with `nowindow`, which ends the accessibility tests before
  those of the real window, and makes no mutant of `src/win/`: twelve test programs at once wait on one another in
  Windows' accessibility interfaces, and a mutant of the window could open a dialog.
  [2026-11-10 06:27:55](DEVLOG/DEVLOG3.md#062755-the-mutator-mended-after-the-tests-of-the-window-slowed-it)
- **Pictures are documents.** A `.png` is not code in any language (Rule 29), so the test of what the project holds
  lets it pass, as it does `.md`, `.txt` and `.json`.
  [2026-11-10 04:21:27](DEVLOG/DEVLOG3.md#042127-the-instructions-in-a-folder-of-their-own-pictures-as-documents-and-a-night-stopped-by-the-usage-limit)
- **One change at a time.** The work is not split into streams side by side: the usage limit ended two such
  attempts before any change was finished.
  [2026-11-10 04:21:27](DEVLOG/DEVLOG3.md#042127-the-instructions-in-a-folder-of-their-own-pictures-as-documents-and-a-night-stopped-by-the-usage-limit)
- **The settings deny what the rules forbid.** `.claude/settings.json` denies commits, pushes, merges, rebases,
  cherry-picks and pull requests, the commands that delete, and every edit of `REQUIREMENTS.md`.
  [2026-11-09 10:06:18](DEVLOG/DEVLOG2.md#100618-rules-26-to-36-settings-that-deny-commits-and-deletions-and-version-100)
- **No workflow runs the tests.** `.github/` holds none; the tests are run by hand before a push (Rule 4).
  [2026-11-09 10:05:44](DEVLOG/DEVLOG2.md#100544-the-questions-of-part-1-answered)
- **A day moves whole to a new part.** The day 2026-11-10 was moved from `DEVLOG2.md` to `DEVLOG3.md` before part 2
  reached 1 000 lines (Rules 18 and 22).
  [2026-11-10 05:09:30](DEVLOG/DEVLOG3.md#050930-a-test-that-drives-the-real-window-and-the-logs-third-part)
- **A day may go on in the next part.** When a part holds fewer than two days, its day goes on in the next part,
  and each notes it, as 2026-11-09 goes on from `DEVLOG1.md` in `DEVLOG2.md` (Rule 22).
  [2026-11-09 10:05:44](DEVLOG/DEVLOG2.md#100544-the-questions-of-part-1-answered)
- **An order is read as a requirement to come.** The tests of the base civ cover an order given with the answers to
  the first questions, which `REQUIREMENTS.md` does not hold yet (Rule 24).
  [2026-11-09 10:07:36](DEVLOG/DEVLOG2.md#100736-a-design-begun-from-a-base-civ-generic-at-first)
- **P10 is read rule by rule, and gives way to ub-mode** (Rule 26). No `goto` or recursion, no allocation, 60 lines
  a function, one level of dereferencing, no function pointers, all warnings. A loop is bounded by what it walks, and
  data is checked where it enters; P10's fixed loop bounds and two checks a function (its rules 2 and 5) are given
  up, since a check no caller can fail and a bound that never ends a loop make mutants no test can kill. Replaced:
  `for` loops bounded by a named constant, two checks a function on average, and `check.c` left out of the count.
  [2026-11-09 00:44:18](DEVLOG/DEVLOG1.md#004418-civbuilder-begun-in-c-with-its-own-build-and-rule-tests)
  [2026-11-16 21:03:39](DEVLOG/DEVLOG5.md#210339-the-answers-recorded-no-cap-on-tests-p10s-bounds-and-check-count-given-up-aa-only-coverage-measured)
- **One function pointer.** The window procedure, in `src/win/app_window.c` only, since Win32 needs it.
  [2026-11-09 00:44:18](DEVLOG/DEVLOG1.md#004418-civbuilder-begun-in-c-with-its-own-build-and-rule-tests)
- **Patterns without objects.** Head First Design Patterns' patterns are built with `enum` and `switch`, one `switch`
  for each choice, since P10 forbids function pointers.
  [2026-11-09 00:44:18](DEVLOG/DEVLOG1.md#004418-civbuilder-begun-in-c-with-its-own-build-and-rule-tests)
- **No shell and no YAML.** `check-rules.sh` and `checks.yml` of repo_template are not copied; their checks are C
  tests. [2026-11-09 00:44:18](DEVLOG/DEVLOG1.md#004418-civbuilder-begun-in-c-with-its-own-build-and-rule-tests)
- **Nothing is deleted.** What should go is listed in `DELETION_REQUESTS.md` (Rule 30).
  [2026-11-09 00:44:18](DEVLOG/DEVLOG1.md#004418-civbuilder-begun-in-c-with-its-own-build-and-rule-tests)
  [2026-11-09 10:06:18](DEVLOG/DEVLOG2.md#100618-rules-26-to-36-settings-that-deny-commits-and-deletions-and-version-100)
- **The ban on deletion is about files.** `DeleteObject` and `DestroyWindow` give back what Windows made, and are used.
  [2026-11-09 01:20:32](DEVLOG/DEVLOG1.md#012032-the-nine-steps-in-a-window-with-undo-a-review-and-accessibility-tests)
  [2026-11-09 10:05:44](DEVLOG/DEVLOG2.md#100544-the-questions-of-part-1-answered)
- **The message loop's bound.** `MESSAGE_LIMIT`, 2 147 483 647 messages, bounds the one loop that runs while the
  window is open (P10 rule 2).
  [2026-11-09 01:20:32](DEVLOG/DEVLOG1.md#012032-the-nine-steps-in-a-window-with-undo-a-review-and-accessibility-tests)
- **A `CHECK` is not a branch for the tools.** The crapper leaves each `CHECK` out of a function's complexity and
  the branches of a line with a `CHECK` out of its coverage; the mutator makes no mutant of a `CHECK` or of the
  `||` before one. Replaced: the branches of `CHECK` are counted like any other.
  [2026-11-10 08:27:52](DEVLOG/DEVLOG3.md#082752-answers-recorded-check-left-out-of-the-tools-the-first-merges-unfinished)
  [2026-11-10 04:22:28](DEVLOG/DEVLOG3.md#042228-the-crapper-in-c-and-369-functions-at-crap-10-or-above)
- **Functions that differ only in a literal are made one** (Rule 31), such as `json_begin` for an object and an
  array.
  [2026-11-10 08:27:52](DEVLOG/DEVLOG3.md#082752-answers-recorded-check-left-out-of-the-tools-the-first-merges-unfinished)
- **The core tests are at most one of each requirement, rule and bug,** and so at most as many as these together
  (Rules 31 and 34); the accessibility tests are at most 55 (Rule 32), and the regression and redundancy tests are not
  counted. Of a requirement, the core test is the one that checks most nearly what its words say. Each test covers a
  requirement or catches again a bug found before, and its purpose names the requirement, rule or bug (Rules 5 and
  6). Replaced: no cap on the number of tests; before it, at most 100 tests, then a cap of 100 in a test, left for a
  human to raise.
  [2026-11-10 08:27:52](DEVLOG/DEVLOG3.md#082752-answers-recorded-check-left-out-of-the-tools-the-first-merges-unfinished)
  [2026-11-15 21:14:09](DEVLOG/DEVLOG4.md#211409-tests-101-to-103-documented-and-a-cap-of-100-tests-that-still-stops-the-build-and-the-mutator)
  [2026-11-16 21:03:39](DEVLOG/DEVLOG5.md#210339-the-answers-recorded-no-cap-on-tests-p10s-bounds-and-check-count-given-up-aa-only-coverage-measured)
  [2026-11-22 23:08:30](DEVLOG/DEVLOG8.md#230830-the-core-tests-within-rule-31-and-a-list-joined-by-commas-scaled-as-one)
- **Requirements 10, 11 and 12 are measured by the original tools only:** the crapper, the dryer and the mutator
  that lie beside the project, in `..\crapper`, `..\dryer` and `..\mutator`. The project holds no copy of them. The
  crapper and the mutator read the coverage of `build.exe coverage`, made into LCOV by gcovr. Replaced: the three
  tools written in C in `tools/`, and with them the reading that a `CHECK` is no branch; Rules 31, 32 and 34.
  [2026-11-16 02:13:51](DEVLOG/DEVLOG5.md#021351-rules-31-32-and-34-measured-by-the-original-tools-only-and-no-pair-alike-in-src)
  [2026-11-16 21:03:39](DEVLOG/DEVLOG5.md#210339-the-answers-recorded-no-cap-on-tests-p10s-bounds-and-check-count-given-up-aa-only-coverage-measured)
- **The dryer is run as `..\dryer\dryer --threshold 0.8`,** since Requirement 11 says 0.8 and the tool's own limit is
  0.82. It and the crapper read `src/` only; `.metrics/` and `target/`, which they write, are ignored by Git.
  [2026-11-16 02:13:51](DEVLOG/DEVLOG5.md#021351-rules-31-32-and-34-measured-by-the-original-tools-only-and-no-pair-alike-in-src)
- **A function of one line that only names a call with other values is not kept** when the dryer pairs it with
  another: its callers make the call, or one function takes what differed.
  [2026-11-16 02:13:51](DEVLOG/DEVLOG5.md#021351-rules-31-32-and-34-measured-by-the-original-tools-only-and-no-pair-alike-in-src)
  [2026-11-16 21:03:39](DEVLOG/DEVLOG5.md#210339-the-answers-recorded-no-cap-on-tests-p10s-bounds-and-check-count-given-up-aa-only-coverage-measured)
- **A `CHECK` is a branch for the original crapper.** A guard of two checks adds 2 to a function's complexity
  and two branches that no test runs, so a function keeps at most four checks, and a longer list of them is a
  function of its own that is a chain of checks.
  [2026-11-16 02:23:39](DEVLOG/DEVLOG5.md#022339-no-function-of-src-with-a-complexity-of-10-or-more-by-the-original-crapper)
- **The mutator runs the tests without those of the rules and the documents:** `.\build.exe quick nowindow
  norules`, or `.\build.exe quick norules` for the folders of the window. A worker's copy of the tree has no files
  that Git lists, and the tests of the documents judge no mutant. The tests of the build program run, so that the
  mutator judges `tools/build/`. Replaced: `norules` left out every test of maintainability.
  [2026-11-16 02:52:16](DEVLOG/DEVLOG5.md#025216-the-analyzed-build-clean-and-ub-modes-mutator-able-to-run-on-tests-without-the-rules)
  [2026-11-17 03:54:37](DEVLOG/DEVLOG6.md#035437-the-last-checks-no-caller-can-fail-taken-out-and-ten-tests-of-what-no-test-saw)
- **Rule 9 asks a line for each of the project's terms, not for the game's words.** "tech", "unit", "Castle" and
  "Villager" are the game's; a line of `TERMINOLOGY.md` uses only the project's terms defined above it and the
  game's words.
  [2026-11-17 05:47:59](DEVLOG/DEVLOG6.md#054759-compliance-with-rulesmd-and-requirementsmd-after-the-bonuses-built-from-the-games-techs)

## Requirements, and how they are read

- **`REQUIREMENTS.md` holds the requirements,** nine, made from the proposals of part 1 with changes; the original
  settles what they leave open. Replaced: the original was the requirement until `REQUIREMENTS.md` was written.
  [2026-11-09 00:44:18](DEVLOG/DEVLOG1.md#004418-civbuilder-begun-in-c-with-its-own-build-and-rule-tests)
  [2026-11-09 10:05:44](DEVLOG/DEVLOG2.md#100544-the-questions-of-part-1-answered)
- **The default name is `civeditor_civ`.** Requirement 1 is read as the name offered for a civ that has none, or
  that keeps the name of its civ of the game, `civeditor_civ.civeditor.json`; the format inside stays
  `empireforge_v2`. Replaced: `civbuilder_civ.civbuilder.json`.
  [2026-11-09 10:06:18](DEVLOG/DEVLOG2.md#100618-rules-26-to-36-settings-that-deny-commits-and-deletions-and-version-100)
  [2026-11-18 07:24:48](DEVLOG/DEVLOG7.md#072448-civs-of-the-game-edited-and-saved-into-one-named-mod)
  [2026-11-20 03:59:19](DEVLOG/DEVLOG7.md#035919-the-project-named-civeditor)
- **A design is a civ of the game, edited.** The first list of the step "Name and tech tree" is the civ to edit, the
  Britons at first; every step shows that civ as the game has it, its names and tagline in their fields, ready to
  change; choosing another can be undone. Replaced: a design started from a base civ, Generic at first, with every
  universal node and nothing else; a base civ of the game was the replaced civ until another was picked on the review
  step; the first step was "Identity", with a field for the description.
  [2026-11-09 10:07:36](DEVLOG/DEVLOG2.md#100736-a-design-begun-from-a-base-civ-generic-at-first)
  [2026-11-18 07:24:48](DEVLOG/DEVLOG7.md#072448-civs-of-the-game-edited-and-saved-into-one-named-mod)
  [2026-11-19 20:19:34](DEVLOG/DEVLOG7.md#201934-five-steps-and-no-field-for-the-civs-description)
- **Five steps.** Name and tech tree (the civ to edit, its name, tagline, looks and tech tree), Bonuses (the civ
  bonuses and the team bonus), UU&UT (the unique unit and both unique techs, each with its name), Hero unit and
  Review. Replaced: nine steps, Identity, Tech tree, Civ bonuses, Team bonus, Unique unit, Castle tech, Imperial
  tech, Hero unit and Review.
  [2026-11-09 01:20:32](DEVLOG/DEVLOG1.md#012032-the-nine-steps-in-a-window-with-undo-a-review-and-accessibility-tests)
  [2026-11-19 20:19:34](DEVLOG/DEVLOG7.md#201934-five-steps-and-no-field-for-the-civs-description)
- **No field for the civ's description.** The description is still read from the game, kept in the design and the
  civ file, and shown in the review; no step lets the user change it.
  [2026-11-19 20:19:34](DEVLOG/DEVLOG7.md#201934-five-steps-and-no-field-for-the-civs-description)
- **Each civ edited keeps its edits.** Choosing another civ to edit keeps the design shown as the draft of its civ,
  and shows the draft again when that civ is chosen again, until the window closes.
  [2026-11-18 07:24:48](DEVLOG/DEVLOG7.md#072448-civs-of-the-game-edited-and-saved-into-one-named-mod)
- **A step is done once the user has made it differ from the civ of the game:** Name and tech tree once its name,
  tagline, description, a look or its tree does; Bonuses once its civ bonuses or its team bonus do; UU&UT once its
  unique unit or a unique tech does; Hero unit once its hero does; the review once the mod holds the civ as it is and
  the civ is complete. Nothing is loaded at the start. Replaced: Identity when the civ had a name, Review when the
  civ was complete; before that, a step done as soon as it held anything.
  [2026-11-16 21:18:51](DEVLOG/DEVLOG5.md#211851-no-step-shown-as-done-before-the-user-changed-it)
  [2026-11-18 07:24:48](DEVLOG/DEVLOG7.md#072448-civs-of-the-game-edited-and-saved-into-one-named-mod)
  [2026-11-19 20:19:34](DEVLOG/DEVLOG7.md#201934-five-steps-and-no-field-for-the-civs-description)
- **The review asks what the original's steps ask.** A name, a unique unit, a castle tech, an imperial tech and, for
  a chosen hero, its name; not a civ bonus. An architecture is chosen from the start.
  [2026-11-09 01:20:32](DEVLOG/DEVLOG1.md#012032-the-nine-steps-in-a-window-with-undo-a-review-and-accessibility-tests)
- **Undo and redo** (Requirement 5). Every change can be undone and redone, 32 deep; letters typed into one field
  join one change.
  [2026-11-09 01:20:32](DEVLOG/DEVLOG1.md#012032-the-nine-steps-in-a-window-with-undo-a-review-and-accessibility-tests)
  [2026-11-09 10:05:44](DEVLOG/DEVLOG2.md#100544-the-questions-of-part-1-answered)
- **At most 24 civ bonuses** (Requirement 6). The list is fixed, as P10 rule 3 asks; a 25th is refused.
  [2026-11-09 01:20:32](DEVLOG/DEVLOG1.md#012032-the-nine-steps-in-a-window-with-undo-a-review-and-accessibility-tests)
  [2026-11-09 10:05:44](DEVLOG/DEVLOG2.md#100544-the-questions-of-part-1-answered)
- **WCAG 2.2 AA, read for a window** (Requirements 3 and 4). Its success criteria are listed in
  `.github/ACCESSIBILITY.md`, read as WCAG2ICT reads them for software; a box is ticked only when a test checks the
  criterion or when civeditor has nothing it applies to, which counts as met. Contrast of 4.5:1, targets of 44 by
  44, a visible label for every input, and the colors of Windows in high contrast. Replaced: level AAA, 7:1.
  [2026-11-09 01:20:32](DEVLOG/DEVLOG1.md#012032-the-nine-steps-in-a-window-with-undo-a-review-and-accessibility-tests)
  [2026-11-09 10:08:11](DEVLOG/DEVLOG2.md#100811-a-checklist-of-wcag-22-for-requirement-4)
  [2026-11-16 21:03:39](DEVLOG/DEVLOG5.md#210339-the-answers-recorded-no-cap-on-tests-p10s-bounds-and-check-count-given-up-aa-only-coverage-measured)
- **A drawn mark is checked with the text's contrast.** The frame of a box, the check and the focus frame are drawn
  only in pairs of colors of `palette.c`, which the test of contrast holds at 7:1; so 1.4.11 is ticked by that test.
  3.2.3 is ticked by the test that the list of steps, Back and Next are on every step.
  [2026-11-10 05:01:56](DEVLOG/DEVLOG3.md#050156-two-boxes-of-the-accessibility-checklist-ticked-48-of-86)
- **A box of the checklist may be ticked by reading.** "Read" stands beside "Test" and "Code": every text the
  window can show was read on a named day against what the criterion asks of words. A command has one name in the
  menu and on its button, and a label says what its control is for without the label before it.
  [2026-11-10 05:03:39](DEVLOG/DEVLOG3.md#050339-the-windows-texts-read-against-the-checklist-55-of-86)
- **The window's title names the civ and the step** (Requirement 3, WCAG 2.2 SC 2.4.2), as "Testonia - Step 2 of
  4: Bonuses - civeditor", with "A new civ" before the civ has a name; the count is `STEP_COUNT`.
  [2026-11-09 10:27:22](DEVLOG/DEVLOG2.md#102722-a-windows-title-that-names-the-civ-and-the-step-and-the-base-civ-tried-in-the-window)
  [2026-11-19 20:19:34](DEVLOG/DEVLOG7.md#201934-five-steps-and-no-field-for-the-civs-description)
- **A mod changes every civ saved in it, each in its own slot** (Requirement 8, read for several civs). The
  replaced civ is the civ to edit that the design was made from, kept in the civ file as `replace_civ`. Replaced: a
  mod took the slot of one civ, picked on the review step, the Britons at first.
  [2026-11-09 02:15:04](DEVLOG/DEVLOG1.md#021504-a-mod-built-from-the-civ-from-the-catalog-and-the-games-own-data-file)
  [2026-11-09 10:05:44](DEVLOG/DEVLOG2.md#100544-the-questions-of-part-1-answered)
  [2026-11-18 07:24:48](DEVLOG/DEVLOG7.md#072448-civs-of-the-game-edited-and-saved-into-one-named-mod)
- **A changed civ is in the mod at once.** Nothing saves it: a civ is in the mod while its draft differs from its
  base civ, and out of it when it is as the game has it again. The label at the top right of Finish counts the
  changed civs, and Build mod builds them as they are. Replaced: the button Save civ, which put the civ in the mod
  as it was when pressed.
  [2026-11-18 07:24:48](DEVLOG/DEVLOG7.md#072448-civs-of-the-game-edited-and-saved-into-one-named-mod)
  [2026-11-21 04:26:03](DEVLOG/DEVLOG8.md#042603-changes-kept-at-once-the-look-choices-in-a-popup-a-filter-on-every-list-a-build-without-questions)
- **Save civ file... and Open civ file... are in the menu only.** The only button of Finish is Build mod... .
  Replaced: the review's buttons Save civ and Build mod... .
  [2026-11-18 07:24:48](DEVLOG/DEVLOG7.md#072448-civs-of-the-game-edited-and-saved-into-one-named-mod)
  [2026-11-21 04:26:03](DEVLOG/DEVLOG8.md#042603-changes-kept-at-once-the-look-choices-in-a-popup-a-filter-on-every-list-a-build-without-questions)
- **The mod is named when Build mod is pressed:** `civeditor_mod` at first, then the name given last. The folders'
  prefix is made from it as it was from the civ's name, and both `info.json` take it as their title. Replaced:
  `civbuilder_mod` at first.
  [2026-11-18 07:24:48](DEVLOG/DEVLOG7.md#072448-civs-of-the-game-edited-and-saved-into-one-named-mod)
  [2026-11-20 03:59:19](DEVLOG/DEVLOG7.md#035919-the-project-named-civeditor)
- **The texts of a civ of the game are the game's.** Its tagline and description are read from the English strings
  of the installed game when the window opens, and are empty without the game; no text of the game is kept here.
  [2026-11-18 07:24:48](DEVLOG/DEVLOG7.md#072448-civs-of-the-game-edited-and-saved-into-one-named-mod)
- **The review's text takes no caret.** It cannot be typed in, so it shows no blinking caret when it has the focus.
  [2026-11-18 07:24:48](DEVLOG/DEVLOG7.md#072448-civs-of-the-game-edited-and-saved-into-one-named-mod)
- **Two folders, written without asking** (Requirements 2 and 7). Build writes `<prefix>-data` and `<prefix>-ui`
  into the mods folder it finds, or the one given in Options... of the menu File, where the original gives a zip to
  unpack by hand. Only the mod's name is asked for. Replaced: a question whether the folder is right before writing.
  [2026-11-09 02:15:04](DEVLOG/DEVLOG1.md#021504-a-mod-built-from-the-civ-from-the-catalog-and-the-games-own-data-file)
  [2026-11-09 10:05:44](DEVLOG/DEVLOG2.md#100544-the-questions-of-part-1-answered)
  [2026-11-21 04:26:03](DEVLOG/DEVLOG8.md#042603-changes-kept-at-once-the-look-choices-in-a-popup-a-filter-on-every-list-a-build-without-questions)
- **What cannot be built is skipped and told** (Requirement 9). Civ bonuses with neither techs nor changes in the
  catalog, custom unique units and heroes, and what does not fit the fixed room are not built; Build says so.
  Replaced: every civ bonus without techs and every team bonus without an effect in the game was skipped.
  [2026-11-09 02:15:04](DEVLOG/DEVLOG1.md#021504-a-mod-built-from-the-civ-from-the-catalog-and-the-games-own-data-file)
  [2026-11-09 02:45:35](DEVLOG/DEVLOG1.md#024535-bonuses-built-from-the-originals-lists-of-changes)
- **Bonuses from the original's lists of changes.** `data/bonus_effects.txt` holds the original's `ec_list` and
  `team_ec_list`. A civ bonus is built from its techs if the catalog names any, else each entry of its changes becomes
  a new effect and a free tech of the civ that fires it, as the original's `_apply_ec_list_entry` does. A team bonus
  whose effect in the game is missing or empty gets its changes.
  [2026-11-09 02:45:35](DEVLOG/DEVLOG1.md#024535-bonuses-built-from-the-originals-lists-of-changes)
- **A bonus's multiplier scales each of its changes, as the original's `_scale_ec_for_multiplier` does:** an amount
  added is multiplied by it, a factor raised to it, a spawn's count multiplied; armor and attack past 255 are split
  into several changes, a percent halfway rounded to the even one; a value set, the spawn limit and the effect function
  are kept. It applies to the civ bonuses built from changes, to the team bonus and to the copies of the civ techs
  that a civ bonus names. A tech that every civ shares is only unlocked, so it counts once, and Build says so
  (Requirement 9). Replaced: a civ bonus made of the game's techs counts once.
  [2026-11-17 04:37:52](DEVLOG/DEVLOG6.md#043752-a-bonuss-multiplier-applied-to-the-bonuses-built-from-changes-and-to-the-team-bonus)
  [2026-11-21 21:19:41](DEVLOG/DEVLOG8.md#211941-unticking-a-node-unticks-its-descendants-and-stronger-and-weaker-edit-a-bonus)
- **A multiplier is from 1 to 10, and may lie between,** as 1.5: `Multiplier` keeps it in millionths, and the civ
  file writes it as a decimal. Read from a civ file, it is checked where it enters (`civ_design_set_multiplier`); any
  other value leaves the bonus at 1. The original has no bound. A factor is raised to a count that is not whole, and
  armor, attack and the units of a spawn are rounded to the nearest whole. Replaced: a whole number from 1 to 10.
  [2026-11-17 04:37:52](DEVLOG/DEVLOG6.md#043752-a-bonuss-multiplier-applied-to-the-bonuses-built-from-changes-and-to-the-team-bonus)
  [2026-11-23 03:53:22](DEVLOG/DEVLOG9.md#035322-the-numbers-of-chosen-bonuses-typed-or-slid-in-place-of-stronger-and-weaker)
- **A bonus that brings a tech of the game an age early is built as the game builds its own civs' bonuses.** Bonuses
  81, 103, 247, 283 and 352 are `require` lines of `bonus_effects.txt`: the requirement tech that the game gives the
  civ with the bonus, such as 768 for the Cavalier, is copied for the civ, and the copy goes in the first free slot of
  the requirements of the game's tech. The original copies the Cavalier, the Paladin and Parthian Tactics for 103 and
  247 and disables the game's own in the civ's tree; the game's way copies no tech that the tree shows, and a tech
  that the design's tree disables stays disabled.
  [2026-11-17 05:17:56](DEVLOG/DEVLOG6.md#051756-bonuses-81-103-247-283-and-352-built-from-the-games-requirement-techs)
- **A tech with no free slot for a requirement leaves its bonus skipped,** and Build says so (Requirement 9). A bonus
  of requirements counts once whatever its multiplier, as in the original: a tech cannot come two ages early.
  [2026-11-17 05:17:56](DEVLOG/DEVLOG6.md#051756-bonuses-81-103-247-283-and-352-built-from-the-games-requirement-techs)
- **Bonuses 221 and 286 are built from the game's own techs.** 221 is five `require` lines, which give the civ the
  requirement techs 952 to 956 of civ 44 for the upgrades of the Militia and the Spearman; the original copies those
  upgrades and disables the game's own. 286 names the game's Houfnice, 787, copied as every tech that a bonus names;
  the Imperial Age that the original adds to its requirements is not added, since 787 already needs it.
  [2026-11-17 05:27:22](DEVLOG/DEVLOG6.md#052722-bonuses-221-and-286-built-from-the-games-techs)
- **A civ bonus that names techs builds its lines too, and they count as often as the bonus, as its techs do.**
  Bonus 356 copies the game's pasture techs 1008, 1012, 1013 and 1014, and its line disables the Farms (216) and their
  upgrades at the Mill (14, 13 and 12), as the game disables techs from the effect of a tech of one civ. The original
  copies only the three upgrades, and puts the pasture and the disabled techs in the civ's tech tree. Replaced: the
  lines count once.
  [2026-11-17 05:41:10](DEVLOG/DEVLOG6.md#054110-bonus-356-built-from-the-games-pasture-techs-and-a-line-that-disables-the-farms)
  [2026-11-21 21:19:41](DEVLOG/DEVLOG8.md#211941-unticking-a-node-unticks-its-descendants-and-stronger-and-weaker-edit-a-bonus)
- **A bonus that moves where a unit is trained is a `train` line: a unit patch placed in the unit's first train
  location.** Bonus 222 moves the cow from the Town Center to the Mill, as the original does; a unit with no train
  location leaves the bonus skipped. The `train` line takes the columns of a `unit` line, so the catalog keeps one list
  of unit patches.
  [2026-11-17 06:03:49](DEVLOG/DEVLOG6.md#060349-bonus-222-built-the-cow-trained-at-the-mill-by-a-train-line)
- **A unit patch names the part of the unit its place counts from: its start, its costs or its first train
  location.** Bonus 460 sets the Castle's costs by `cost` lines, its build time by a `train` line and its hit points by
  a `unit` line, with the original's numbers. The Wooden Fort's look that the original also gives it is not built:
  its graphics lie in parts that no line reaches. Replaced: the flag of the train location.
  [2026-11-17 06:14:20](DEVLOG/DEVLOG6.md#061420-bonus-460-built-the-castles-costs-set-by-cost-lines)
- **Bonuses 451 and 454 name the game's techs of the Phalangite and the Sannahya,** copied for the civ as every
  tech a bonus names; the `unit` lines of 454 give the Sannahya the original's hit points, 250 and 300.
  [2026-11-17 06:18:15](DEVLOG/DEVLOG6.md#061815-bonuses-451-and-454-built-from-the-games-techs-of-the-chronicles-unique-units)
- **Bonuses 455 and 456 are `require` lines, as the game builds its own civs' bonuses.** The game's techs of the
  Hoplite (1136) and the Scythian Horse Archer (1336) need 2 techs, one of the Chronicles only; the civ's copy of 638
  in their free slot meets it, and the Castle Age the other. A `civ` line of 456 disables the Heavy Cavalry Archer
  (218), as the original takes that line out of the tree. The original copies the techs with the standard ages.
  [2026-11-17 06:23:23](DEVLOG/DEVLOG6.md#062323-bonuses-455-and-456-built-the-chronicles-requirements-met-by-a-requirement-tech-of-the-civ)
- **Bonus 457 multiplies the Trebuchet's attacks, as the game writes an attack bonus:** a change of type 5, c 9, for
  each attack, its d the class times 256 plus 115. The Palintonon's look is left out.
  [2026-11-17 06:27:31](DEVLOG/DEVLOG6.md#062731-bonus-457-built-the-trebuchets-attacks-multiplied-by-civ-lines)
- **A look line gives one unit of the civ the look of the same unit of another civ:** its graphics, icon and walking
  and attack graphics, copied after the other looks, for each bonus of the design that has one; bonus 457 dresses the
  Trebuchets as the Athenians'. A `require` line and a `look` line are read by one function, `add_id_line`.
  [2026-11-17 06:49:31](DEVLOG/DEVLOG6.md#064931-the-palintonons-look-of-bonus-457-given-by-look-lines)
- **Bonus 455 sets the cost of the Hoplite's tech (1136) to 0 for the civ, as the game's tech 1261 does:** a tech
  researched nowhere that costs something never fires. This corrects the decision on 455 and 456 above.
  [2026-11-17 06:55:32](DEVLOG/DEVLOG6.md#065532-the-hoplites-tech-of-bonus-455-freed-of-its-cost)
- **A look line gives its unit the timing of its attack too:** the frame at which it lets go and the place it lets go
  from, as the original copies them for the Palintonon; the monk's copy is as before.
  [2026-11-17 07:05:37](DEVLOG/DEVLOG6.md#070537-a-look-line-gives-its-unit-the-timing-of-its-attack-too)
- **What the original's code fixes is data here.** The bonuses that unlock a unit (405 to 417, 428) name in
  `bonuses.txt` the techs the original copies for them. Bonus 404, and bonuses 123, 125, 131, 133, 137, 138, 155,
  195, 209, 234, 258, 261, 277, 278, 279, 290, 325, 330, 339, 401 and 458, whose code reads the game's data, have in
  `bonus_effects.txt` the changes and unit patches that code makes, worked out once from the game's data file; a
  condition the code tests on the data is written as what it found there. Replaced: bonuses whose code reads the
  game's data stay unbuilt.
  [2026-11-09 02:49:33](DEVLOG/DEVLOG1.md#024933-351-civ-bonuses-built-with-the-unit-unlocking-bonuses-and-bonus-404)
  [2026-11-09 05:20:00](DEVLOG/DEVLOG1.md#052000-fourteen-more-civ-bonuses-built-from-the-changes-the-originals-code-makes)
  [2026-11-09 05:22:54](DEVLOG/DEVLOG1.md#052254-bonus-195-built-and-room-for-4-096-lines-of-changes)
  [2026-11-09 05:26:39](DEVLOG/DEVLOG1.md#052639-bonuses-155-261-and-277-built)
  [2026-11-09 05:46:17](DEVLOG/DEVLOG1.md#054617-bonuses-330-339-and-458-built-from-unit-patches)
- **A unique tech is aimed at the civ's own unique unit.** What a copied unique tech does to the other civ's unique
  unit is done to the civ's, as the original's `_build_ut_effect_cmds` does; the units are read from the effects of
  the techs `units.txt` names. What would need a change to a unit itself, such as the free units of Cuman
  Mercenaries (type 12), is left out.
  [2026-11-09 03:06:22](DEVLOG/DEVLOG1.md#030622-unique-techs-aimed-at-the-civs-own-unique-unit)
- **Unique techs and unit named through the game's own string ids.** Each slot has the original's four string ids,
  on its `civ` line of `appearance.txt`. The copied unique techs point at two; a unique unit of the game that the civ
  names or describes, its elite unit and the copied elite tech, in the slot's copies only, at the other two. The UI
  mod gives them the civ's names.
  [2026-11-09 03:19:07](DEVLOG/DEVLOG1.md#031907-unique-techs-shown-in-the-game-by-the-names-the-civ-gives-them)
  [2026-11-09 03:31:22](DEVLOG/DEVLOG1.md#033122-the-unique-unit-shown-in-the-game-by-the-name-the-civ-gives-it)
  [2026-11-09 10:05:44](DEVLOG/DEVLOG2.md#100544-the-questions-of-part-1-answered)
- **Looks copied as the original's `_copy_architecture` does.** The units of the slot take the graphics of the same
  units of the civ that the architecture's line of `appearance.txt` names, by the classes the original names; the
  castle and the wonder take the looks of the civ chosen for them, else of the architecture's civ, and not the whole
  unit, since in the game's file nothing else of them differs between civs.
  [2026-11-09 04:04:36](DEVLOG/DEVLOG1.md#040436-the-civ-given-the-looks-of-its-architecture-castle-and-wonder)
- **Monk skins from the catalog.** A `monk` line of `appearance.txt` names the civ and the three units its skin copies,
  so a priest of the scenario editor is a line like the others; without a skin, the monks are the architecture's.
  [2026-11-09 04:12:16](DEVLOG/DEVLOG1.md#041216-the-civs-monks-given-the-look-of-the-monk-skin-chosen-for-it)
- **The hero made as the original's `_place_hero` and `_apply_hero_unit` make it.** It trains at the Castle on button
  4, hotkey 16730, in 60 seconds, for 500 food and 500 gold, with the hero's status and at least 300 hit points, and a
  free tech of the civ makes it available, one at a time, once the Imperial Age is reached.
  [2026-11-09 04:35:41](DEVLOG/DEVLOG1.md#043541-the-civs-hero-trained-at-the-castle-from-the-imperial-age)
- **The hero named through the game's own string ids.** Each `civ` line of `appearance.txt` ends in the slot's hero
  string id, the original's at `HERO_POOL_OFFSET` plus the slot.
  [2026-11-09 04:35:41](DEVLOG/DEVLOG1.md#043541-the-civs-hero-trained-at-the-castle-from-the-imperial-age)
- **A hero the civ lacks is not built.** The build's message says so, and the UI mod has no strings for it.
  [2026-11-09 04:35:41](DEVLOG/DEVLOG1.md#043541-the-civs-hero-trained-at-the-castle-from-the-imperial-age)
  [2026-11-09 05:56:32](DEVLOG/DEVLOG1.md#055632-no-strings-for-a-hero-that-is-not-built)
- **The civ list in the data mod, as the original's `patch_civilizations_list` makes it.** The slot's entry names the
  copied unique techs, the unique unit, its elite unit and elite tech, and the line, icons and string ids of the
  game's entry that has the unit.
  [2026-11-09 05:05:16](DEVLOG/DEVLOG1.md#050516-the-civ-list-in-the-data-mod-naming-the-civs-unique-techs-and-unique-unit)
- **A unique unit the civ list lacks is shown by its own icon and texts,** from the dat file: its icon, its name id
  and its help id less 79 000. Its line stays the replaced civ's, as in the original.
  [2026-11-09 05:05:16](DEVLOG/DEVLOG1.md#050516-the-civ-list-in-the-data-mod-naming-the-civs-unique-techs-and-unique-unit)
- **A named unique unit's entry points at the slot's unit string id and that id plus 21 000,** the tooltip's, as the
  game's own entries do.
  [2026-11-09 05:05:16](DEVLOG/DEVLOG1.md#050516-the-civ-list-in-the-data-mod-naming-the-civs-unique-techs-and-unique-unit)
- **An empty tech tree disables nothing.** With nothing ticked in the tech tree step, the civ keeps the game's whole
  tree, as in the original. The ages, other civs' techs and Villager, Town Center and the camps are never disabled.
  The tech tree file follows the same rule.
  [2026-11-09 02:15:04](DEVLOG/DEVLOG1.md#021504-a-mod-built-from-the-civ-from-the-catalog-and-the-games-own-data-file)
  [2026-11-09 10:40:46](DEVLOG/DEVLOG2.md#104046-one-rule-for-what-a-civ-keeps-in-the-dat-file-and-the-tech-tree-file-alike)
- **The accessibility checklist is level AA** (Requirement 3): the 55 criteria of levels A and AA of WCAG 2.2 are
  its boxes, and no criterion of level AAA is on it. A test names the AA criterion it serves. Replaced: all 86
  criteria as boxes; then the 31 of level AAA in a last section without boxes.
  [2026-11-15 21:39:11](DEVLOG/DEVLOG4.md#213911-the-accessibility-checklist-at-level-aa-53-of-55-boxes-ticked)
  [2026-11-09 10:08:11](DEVLOG/DEVLOG2.md#100811-a-checklist-of-wcag-22-for-requirement-4)
  [2026-11-16 21:03:39](DEVLOG/DEVLOG5.md#210339-the-answers-recorded-no-cap-on-tests-p10s-bounds-and-check-count-given-up-aa-only-coverage-measured)
- **A ticked box that no test settles is flagged "Human review needed."** in the checklist: one that rests on
  "Code" or "Read", or on a test with something left that was not looked at, heard or tried.
  [2026-11-10 08:27:52](DEVLOG/DEVLOG3.md#082752-answers-recorded-check-left-out-of-the-tools-the-first-merges-unfinished)
  [2026-11-15 21:39:11](DEVLOG/DEVLOG4.md#213911-the-accessibility-checklist-at-level-aa-53-of-55-boxes-ticked)
- **A window that does not hold the wide layout shows the step in one column and scrolls** (WCAG 1.4.4 and
  1.4.10): narrower than 960 units or shorter than 640, so that UU&UT holds a filter above each list. The least
  window is 320 by 256 units. The one column is made in `src/reflow/`, a folder of its own, since `src/view/` and
  `src/win/` hold 7 files each (Rule 14). Replaced: shorter than 560; before it, a least window of 960 by 560.
  [2026-11-16 02:45:16](DEVLOG/DEVLOG5.md#024516-a-window-too-small-for-the-columns-shows-a-step-in-one-column-and-scrolls)
  [2026-11-21 04:26:03](DEVLOG/DEVLOG8.md#042603-changes-kept-at-once-the-look-choices-in-a-popup-a-filter-on-every-list-a-build-without-questions)
- **Every list of the wide layout is 44 high or more at 960 by 560.** So that the merged steps fit, the unique
  unit's description is 88 high, "In the tree" and the selected team bonus 66, and the other lists and texts share
  what is left of their column. Controls of one step that do alike are labelled apart, as "What the castle tech
  does" and "What the imperial tech does".
  [2026-11-19 20:19:34](DEVLOG/DEVLOG7.md#201934-five-steps-and-no-field-for-the-civs-description)
- **A `&` in a step's title is written twice in the menu Go,** so that the item shows it: "3 UU&UT".
  [2026-11-19 20:19:34](DEVLOG/DEVLOG7.md#201934-five-steps-and-no-field-for-the-civs-description)
- **The rows of a list wrap** and take the height their text needs; none is cut with three dots.
  [2026-11-16 02:45:16](DEVLOG/DEVLOG5.md#024516-a-window-too-small-for-the-columns-shows-a-step-in-one-column-and-scrolls)
- **1.4.4 and 1.4.10 are ticked on their tests and on a look at 100%,** each marked for a human's review at 200%
  and 400%.
  [2026-11-16 02:45:16](DEVLOG/DEVLOG5.md#024516-a-window-too-small-for-the-columns-shows-a-step-in-one-column-and-scrolls)
- **A list moves one row a notch of the wheel and one row a press on an arrow of its scroll bar, and never slides**
  (Bugs 3 and 4 of 2026-11-16). A held arrow moves it again after 400 ms, then every 50 ms. A press between the
  arrows still moves a page and the knob still follows the mouse, as Windows does. Replaced: Windows' three rows a
  notch and smooth scrolling.
  [2026-11-16 23:56:00](DEVLOG/DEVLOG5.md#235600-lists-scroll-a-row-a-notch-and-a-row-an-arrow-without-sliding-under-classic-scroll-bars)
- **Every control made with a vertical scroll bar has a classic scroll bar,** its arrows and knob shown at all
  times (Bug 2 of 2026-11-16), by turning the visual styles off for that control alone. The window's own scroll bar
  is not changed that way, since the window's title bar would then look like that of Windows 2000.
  [2026-11-16 23:56:00](DEVLOG/DEVLOG5.md#235600-lists-scroll-a-row-a-notch-and-a-row-an-arrow-without-sliding-under-classic-scroll-bars)
- **The window's scroll bar is a scroll bar control, classic like the lists',** its arrows and knob shown at all
  times (Bug 2 of 2026-11-16), and the window keeps the visual styles and the title bar of Windows 11. Replaced: the
  window's own scroll bar left as it was.
  [2026-11-17 00:25:45](DEVLOG/DEVLOG6.md#002545-the-windows-scroll-bar-a-control-of-its-own-classic-like-the-lists)
- **The last step is named Finish.** Replaced: Review.
  [2026-11-21 04:26:03](DEVLOG/DEVLOG8.md#042603-changes-kept-at-once-the-look-choices-in-a-popup-a-filter-on-every-list-a-build-without-questions)
- **The look choices are in the Appearance popup,** opened by the button Appearance of the first step and closed by
  Close, Esc or its X. Each choice changes the design at once, as one change that can be undone.
  [2026-11-21 04:26:03](DEVLOG/DEVLOG8.md#042603-changes-kept-at-once-the-look-choices-in-a-popup-a-filter-on-every-list-a-build-without-questions)
- **Civ to edit offers ranked civs only.** The six civs of Chronicles, the Achaemenids, Athenians, Spartans,
  Macedonians, Thracians and Puru, have an `unranked` line in `appearance.txt` and are left out; their bonuses,
  units and techs are still offered.
  [2026-11-21 04:26:03](DEVLOG/DEVLOG8.md#042603-changes-kept-at-once-the-look-choices-in-a-popup-a-filter-on-every-list-a-build-without-questions)
- **A changed civ ends in an asterisk in Civ to edit,** as "Britons*", and loses it when changed back.
  [2026-11-21 04:26:03](DEVLOG/DEVLOG8.md#042603-changes-kept-at-once-the-look-choices-in-a-popup-a-filter-on-every-list-a-build-without-questions)
- **Clear unticks every civ bonus as one change.** With none ticked it changes nothing and the status line says so.
  [2026-11-21 04:26:03](DEVLOG/DEVLOG8.md#042603-changes-kept-at-once-the-look-choices-in-a-popup-a-filter-on-every-list-a-build-without-questions)
- **Unticking a node of the tech tree unticks its descendants, and ticking a node ticks its needs, as one
  change.** A node needs its parent in `tree.txt`, the node of that id in the same building or else the building of
  that id, and a unit or a tech also the building it is made in. Ticking ticks the needs of its needs too, a node
  that every civ keeps among them. Replaced: ticking a node ticks only it.
  [2026-11-21 21:19:41](DEVLOG/DEVLOG8.md#211941-unticking-a-node-unticks-its-descendants-and-stronger-and-weaker-edit-a-bonus)
  [2026-11-22 02:10:47](DEVLOG/DEVLOG8.md#021047-ticking-a-node-ticks-its-needs-in-the-first-regression-test)
- **A node that every civ keeps is unticked alone.** The Villager, both Town Centers, the Mill and the two camps
  keep their descendants when unticked, and no other node unticks them, since the build keeps them anyway.
  [2026-11-21 21:19:41](DEVLOG/DEVLOG8.md#211941-unticking-a-node-unticks-its-descendants-and-stronger-and-weaker-edit-a-bonus)
- **The building column of a building in `tree.txt` is the column of the screen,** not a node it needs, as the
  Wonder under the House and the towers under the Outpost; a building needs only its parent.
  [2026-11-21 21:19:41](DEVLOG/DEVLOG8.md#211941-unticking-a-node-unticks-its-descendants-and-stronger-and-weaker-edit-a-bonus)
- **A bonus's number is typed where it is read.** Each ticked civ bonus is a label in Chosen bonuses, and each
  number to set in it is a field that holds the number, as "Shepherds work [25]% faster"; the value wanted is typed
  there, or set by the slider just below it, from 1 to 10 in hundredths. The multiplier is worked out from the
  value, so "-50%" typed as "-60%" counts 1.32 times. Replaced: Stronger and Weaker, which raised or lowered the
  multiplier by one, so that no value between two counts could be set; and one value field and one slider under the
  list, to which a click on an underlined number moved the focus.
  [2026-11-21 21:19:41](DEVLOG/DEVLOG8.md#211941-unticking-a-node-unticks-its-descendants-and-stronger-and-weaker-edit-a-bonus)
  [2026-11-23 03:53:22](DEVLOG/DEVLOG9.md#035322-the-numbers-of-chosen-bonuses-typed-or-slid-in-place-of-stronger-and-weaker)
  [2026-11-23 14:02:39](DEVLOG/DEVLOG9.md#140239-the-numbers-of-chosen-bonuses-typed-in-their-own-fields-the-slider-just-below)
- **A value typed is taken within the range of its number,** its values at the multipliers 1 and 10, compared in
  tenths as they are shown; otherwise the status line names the range, as "Type a value from -50% to -99.9%.". A
  value may be typed with its sign, its percent or its "x", or without them.
  [2026-11-23 03:53:22](DEVLOG/DEVLOG9.md#035322-the-numbers-of-chosen-bonuses-typed-or-slid-in-place-of-stronger-and-weaker)
- **The slider is on a card just below the field of the number in focus,** with the least and the most value of the
  number at its ends. The card is shown while the field or its slider has the focus, and Esc hides it, the focus
  left on the field. Dragged, the slider shows in the field the value it would set; let go, it sets it. Replaced:
  one slider under the value field, with a label that named the range.
  [2026-11-23 03:53:22](DEVLOG/DEVLOG9.md#035322-the-numbers-of-chosen-bonuses-typed-or-slid-in-place-of-stronger-and-weaker)
  [2026-11-23 14:02:39](DEVLOG/DEVLOG9.md#140239-the-numbers-of-chosen-bonuses-typed-in-their-own-fields-the-slider-just-below)
- **A change that takes away the number in focus gives the focus to the list of civ bonuses,** as Clear or an undo
  can, and the field it leaves sets nothing.
  [2026-11-23 14:02:39](DEVLOG/DEVLOG9.md#140239-the-numbers-of-chosen-bonuses-typed-in-their-own-fields-the-slider-just-below)
- **The field of a number holds its sign and digits, and its unit stays in the label after it,** as "[-50]% wood";
  the ends of the slider and the status line give the number with its unit. A number that no count changes is none:
  a cut of 100% or more, a power of 1 or less, an amount of 0. Replaced: a number to set that took in its "%" or "x",
  so that the whole of "-50%" was underlined, framed and found by a click.
  [2026-11-23 03:53:22](DEVLOG/DEVLOG9.md#035322-the-numbers-of-chosen-bonuses-typed-or-slid-in-place-of-stronger-and-weaker)
  [2026-11-23 14:02:39](DEVLOG/DEVLOG9.md#140239-the-numbers-of-chosen-bonuses-typed-in-their-own-fields-the-slider-just-below)
- **No text says how often a bonus counts.** Chosen bonuses, the summary and the game's texts never add "(counts 2
  times)" or "(counts 1 time)": a bonus's count is set through its numbers, and a bonus with no number to set has
  no field. All three scale a bonus's text in one way, at a count of 1 too. Replaced: "(counts 2 times)" after a
  text with a number kept, or none; "(counts 1 time)" in Chosen bonuses, there to be clicked; and the text of a
  bonus counted once copied as it is for the summary and the game's texts.
  [2026-11-23 03:53:22](DEVLOG/DEVLOG9.md#035322-the-numbers-of-chosen-bonuses-typed-or-slid-in-place-of-stronger-and-weaker)
  [2026-11-23 14:02:39](DEVLOG/DEVLOG9.md#140239-the-numbers-of-chosen-bonuses-typed-in-their-own-fields-the-slider-just-below)
- **A field of a number is at least 24 by 24,** where the other controls are at least 44 by 44: it stands in the text
  of a bonus, which WCAG 2.2 SC 2.5.8 leaves out as inline; see question 6 of the section.
  [2026-11-23 14:02:39](DEVLOG/DEVLOG9.md#140239-the-numbers-of-chosen-bonuses-typed-in-their-own-fields-the-slider-just-below)
- **The team bonus's numbers are set in fields, as a civ bonus's.** The picked team bonus is shown under its list in
  a number panel of one row, with a field for each number to set and the card with the slider; Finish and the mod's
  texts show it as it counts. Replaced: the team bonus had no number to set.
  [2026-11-23 03:53:22](DEVLOG/DEVLOG9.md#035322-the-numbers-of-chosen-bonuses-typed-or-slid-in-place-of-stronger-and-weaker)
  [2026-11-23 14:02:39](DEVLOG/DEVLOG9.md#140239-the-numbers-of-chosen-bonuses-typed-in-their-own-fields-the-slider-just-below)
  [2026-11-24 06:15:02](DEVLOG/DEVLOG10.md#061502-the-team-bonuss-numbers-set-in-fields-as-a-civ-bonuss)
- **A bonus's text shows its numbers as often as the bonus counts,** in Chosen bonuses, the summary and the civ's
  texts in the game: an amount with a sign is multiplied, a percent with a sign is a factor raised to the multiplier
  and "2x" is raised, each number shown with one decimal at most. A sign counts only at the start of a word, and
  numbers joined by a slash, or by a comma and a space, are one list, as "+20/30%" and "-10, 15, 20, 25%". A percent
  without a sign before "faster", "more" or "longer" is a gain, and one before "less" or "cheaper" a cut, a number to
  set shown without a sign: "Cavalry 20% faster" counted twice is "Cavalry 44% faster". Any other number without a
  sign outside a list is left as it is. The team bonus's text is shown as it counts too.
  Replaced: only a slash joining numbers into a list; every number without a sign outside a list left as it is; and
  the team bonus's text shown as it is.
  [2026-11-21 21:19:41](DEVLOG/DEVLOG8.md#211941-unticking-a-node-unticks-its-descendants-and-stronger-and-weaker-edit-a-bonus)
  [2026-11-24 06:15:02](DEVLOG/DEVLOG10.md#061502-the-team-bonuss-numbers-set-in-fields-as-a-civ-bonuss)
  [2026-11-22 23:08:30](DEVLOG/DEVLOG8.md#230830-the-core-tests-within-rule-31-and-a-list-joined-by-commas-scaled-as-one)
  [2026-11-23 03:53:22](DEVLOG/DEVLOG9.md#035322-the-numbers-of-chosen-bonuses-typed-or-slid-in-place-of-stronger-and-weaker)
  [2026-11-23 14:02:39](DEVLOG/DEVLOG9.md#140239-the-numbers-of-chosen-bonuses-typed-in-their-own-fields-the-slider-just-below)
- **Every list of a step has a filter, and a filter takes a civ's name.** A row stays when it holds the filter's
  text, or when the text starts the name of a civ that has the row's node, civ bonus, team bonus, unique unit or
  unique tech. The filter of Choose hero searches the heroes' names only.
  [2026-11-21 04:26:03](DEVLOG/DEVLOG8.md#042603-changes-kept-at-once-the-look-choices-in-a-popup-a-filter-on-every-list-a-build-without-questions)
- **A build that left nothing out shows no message.** The status line names the mod and Finish shows the built
  mark, a green check mark with "Successfully built mod", until the next change. A build that left something out,
  such as a bonus not built yet, still says what in a message, and a build that failed says why.
  [2026-11-21 04:26:03](DEVLOG/DEVLOG8.md#042603-changes-kept-at-once-the-look-choices-in-a-popup-a-filter-on-every-list-a-build-without-questions)
- **Options... shows and corrects the mods folder.** It takes only a folder that exists, and keeps it until
  civeditor closes.
  [2026-11-21 04:26:03](DEVLOG/DEVLOG8.md#042603-changes-kept-at-once-the-look-choices-in-a-popup-a-filter-on-every-list-a-build-without-questions)
- **The unique unit and techs are edited in place, in number panels.** The panels under the lists of UU&UT, The
  selected unit, What the castle tech does and What the imperial tech does, show the selected unit's or tech's name,
  the game's description, costs, stats and research time, each stat and tech value in a field of a number, as in
  Chosen bonuses. A stat or tech value not set is the game's, and the panel shows it.
  [2026-11-24 06:14:27](DEVLOG/DEVLOG10.md#061427-the-unique-units-stats-and-the-unique-techs-tech-values-and-tech-effects-set-in-uuut)
- **What a unique tech does is a set of civ bonuses, its tech effects.** Choose castle effects... and Choose imperial
  effects... list the civ bonuses, and OK makes the ticked ones the tech effects, each counted by its own multiplier,
  set in its fields and on its slider as in Chosen bonuses. A tech with tech effects does them instead of what its
  game tech does, and with none ticked it does what its game tech does.
  [2026-11-24 06:14:27](DEVLOG/DEVLOG10.md#061427-the-unique-units-stats-and-the-unique-techs-tech-values-and-tech-effects-set-in-uuut)
- **The elite unit takes the same change as the unit.** A stat set of the unique unit changes its elite unit by as
  much: a Longbowman of 35 hit points set to 40 gives the Elite Longbowman 45. A stat that a civ file sets for the
  elite unit itself is kept.
  [2026-11-24 06:14:27](DEVLOG/DEVLOG10.md#061427-the-unique-units-stats-and-the-unique-techs-tech-values-and-tech-effects-set-in-uuut)
- **Picking another unique unit or unique tech clears its overrides and tech effects.**
  [2026-11-24 06:14:27](DEVLOG/DEVLOG10.md#061427-the-unique-units-stats-and-the-unique-techs-tech-values-and-tech-effects-set-in-uuut)
- **A stat or cost that the build leaves out is reported** (Requirement 9): a cost in all four resources, which the
  three slots of a unit's costs cannot hold, and a stat of the elite unit that the same change takes past its range.
  The summary of the build says "A changed stat or cost of the unique unit is not built.", and the build is not whole.
  [2026-11-24 06:14:27](DEVLOG/DEVLOG10.md#061427-the-unique-units-stats-and-the-unique-techs-tech-values-and-tech-effects-set-in-uuut)
- **Options... says whether civeditor asks to save the civ before exiting,** with a tick box under the mods folder,
  ticked at first. Unticked, exiting asks nothing of a civ with unsaved changes; Open civ... still asks.
  [2026-11-24](DEVLOG/DEVLOG10.md#the-choice-of-asking-to-save-before-exiting-kept-by-options)
- **What the game installed says replaces what the catalog files say,** as far as it has been done: the stats of the
  game's unique units, the costs and research time of the unique techs, and the civs that have each node. The lists
  themselves, the bonuses and the structure of the tech tree still come from `data/`.
  [2026-11-24](DEVLOG/DEVLOG10.md#the-catalogs-numbers-and-nodes-read-from-the-game)
- **The files of `data/` stay as the fallback.** A unit, a tech or a civ that the game lacks keeps what they say, and
  everything does when no game is found, so civeditor opens without the game.
  [2026-11-24](DEVLOG/DEVLOG10.md#the-catalogs-numbers-and-nodes-read-from-the-game)
- **A unit's range is read as the game shows it,** and every other stat as the data file holds it: the Fire Archer
  shoots 9 tiles and shows 5, and the user knows the 5.
  [2026-11-24](DEVLOG/DEVLOG10.md#the-catalogs-numbers-and-nodes-read-from-the-game)

## Code and files

- **The project is named civeditor.** The window's title, the title of its dialogs and message boxes, the
  window's class (`civeditor.window`) and the program (`build\civeditor.exe`) say civeditor, and so do the
  README, the guides and the links to GitHub, which lead to `cnordenb/civeditor`. Replaced: civbuilder.
  [2026-11-20 03:59:19](DEVLOG/DEVLOG7.md#035919-the-project-named-civeditor)
- **Civ files are saved as `.civeditor.json`.** Open civ file... names `.civeditor.json` and `.civbuilder.json`
  and lists every `.json` file, so files saved before the rename open as before. Replaced: `.civbuilder.json`.
  [2026-11-20 03:59:19](DEVLOG/DEVLOG7.md#035919-the-project-named-civeditor)
- **Names of other things keep "civbuilder":** the format `civbuilder_v1`, in which civ files of the original
  are written; the original's example civ, `tests/fixtures/example.civbuilder.json`; and Krakenmeister's
  civbuilder, where the catalog of bonuses comes from.
  [2026-11-20 03:59:19](DEVLOG/DEVLOG7.md#035919-the-project-named-civeditor)
- **The hero is chosen in UU&UT,** not in a step of its own: the wizard has four steps. The Hero box is left of
  Back, Choose hero... left of the box, and the hero's name above Choose hero...; the status line ends before them
  and the columns of UU&UT end above the name.
  [2026-11-20 03:00:57](DEVLOG/DEVLOG7.md#030057-the-hero-is-chosen-in-uuut-with-a-hero-box-and-choose-hero)
- **Ticking the Hero box picks Attila the Hun,** unit 777 (`HERO_AT_FIRST`), named as the game names him;
  unticking it picks no hero. The heroes have no row "No hero" any longer.
  [2026-11-20 03:00:57](DEVLOG/DEVLOG7.md#030057-the-hero-is-chosen-in-uuut-with-a-hero-box-and-choose-hero)
- **Choose hero... and the hero's name show only while the civ has a hero** (`layout_needs_hero`).
  [2026-11-20 03:00:57](DEVLOG/DEVLOG7.md#030057-the-hero-is-chosen-in-uuut-with-a-hero-box-and-choose-hero)
- **Choose hero... opens the dialog of `text_dialog.c` with a list:** the heroes, the civ's picked at first, and
  the hero's name in a field under them. Picking a row fills in the hero's name in the game, which may be changed;
  OK picks the hero with the name in the field, as one change. The question of the mod name is the same dialog
  without the list.
  [2026-11-20 03:00:57](DEVLOG/DEVLOG7.md#030057-the-hero-is-chosen-in-uuut-with-a-hero-box-and-choose-hero)
- **The name above Choose hero... is the name given to the hero,** "Attila the Hun" at first, not the name of
  the hero it is made from.
  [2026-11-20 03:00:57](DEVLOG/DEVLOG7.md#030057-the-hero-is-chosen-in-uuut-with-a-hero-box-and-choose-hero)
- **The name in Choose hero... holds as much as the hero's name in the design,** `CIV_NAME_CAPACITY`, so that a
  name read from a civ file is never too long for the dialog.
  [2026-11-20 03:00:57](DEVLOG/DEVLOG7.md#030057-the-hero-is-chosen-in-uuut-with-a-hero-box-and-choose-hero)
- **`layout_box` takes the step,** as `reflow_box` does: the status line of UU&UT ends before the hero's
  controls.
  [2026-11-20 03:00:57](DEVLOG/DEVLOG7.md#030057-the-hero-is-chosen-in-uuut-with-a-hero-box-and-choose-hero)
- **A scroll that does not move the step does nothing.** `scroll_to` in `assist.c` keeps the scroll within the
  step and places the controls again only when it changes, since placing them redraws the whole window. The
  wheel, the window's scroll bar, Scroll up and Scroll down of the menu Go, and the focus all scroll through it.
  [2026-11-19 21:28:52](DEVLOG/DEVLOG7.md#212852-a-turn-of-the-wheel-that-cannot-scroll-the-step-does-nothing)
- **C11 and Win32 only.** Built by gcc with `-std=c11 -pedantic-errors` and every warning as an error.
  [2026-11-09 00:44:18](DEVLOG/DEVLOG1.md#004418-civbuilder-begun-in-c-with-its-own-build-and-rule-tests)
- **One step builds and tests.** `build.exe`, made from `tools/build/`, compiles what changed, links and runs the
  tests. [2026-11-09 00:44:18](DEVLOG/DEVLOG1.md#004418-civbuilder-begun-in-c-with-its-own-build-and-rule-tests)
- **One list of the folders of code, one rule for what the tests hold.** `source_list_collect_all` and
  `source_in_tests` in `tools/build/source_list.c` say them for the build program and the crapper alike; the test
  program holds the files of `tools/crap/` whose names begin with `crap_`.
  [2026-11-10 04:22:28](DEVLOG/DEVLOG3.md#042228-the-crapper-in-c-and-369-functions-at-crap-10-or-above)
- **The types of change are named once, in `dat_map.h`** (`DAT_CHANGE_...`), beside `DatChange`, for the bonuses and
  the recipe alike.
  [2026-11-17 04:37:52](DEVLOG/DEVLOG6.md#043752-a-bonuss-multiplier-applied-to-the-bonuses-built-from-changes-and-to-the-team-bonus)
- **Wide paths.** Every path is a wide string, since the user's folder may hold letters outside ASCII.
  [2026-11-09 00:44:18](DEVLOG/DEVLOG1.md#004418-civbuilder-begun-in-c-with-its-own-build-and-rule-tests)
- **Civ files as the original writes them.** `empireforge_v2` is written; it and `civbuilder_v1` are read.
  [2026-11-09 00:44:18](DEVLOG/DEVLOG1.md#004418-civbuilder-begun-in-c-with-its-own-build-and-rule-tests)
- **A civ file counts the civs of its looks from 0.** Its language, wonder and castle are the original's place of a
  civ, 0 for the Britons; the design keeps slots, and a new design speaks as the Britons.
  [2026-11-09 03:45:50](DEVLOG/DEVLOG1.md#034550-a-civ-files-language-wonder-and-castle-counted-as-the-original-counts-them)
- **The catalog is text.** Looks, bonuses, units, techs and nodes are tab-separated files in `data/`.
  [2026-11-09 00:44:18](DEVLOG/DEVLOG1.md#004418-civbuilder-begun-in-c-with-its-own-build-and-rule-tests)
- **GPL-3.0.** The project takes the original's license, since its catalog comes from the original.
  [2026-11-09 00:44:18](DEVLOG/DEVLOG1.md#004418-civbuilder-begun-in-c-with-its-own-build-and-rule-tests)
  [2026-11-09 10:05:44](DEVLOG/DEVLOG2.md#100544-the-questions-of-part-1-answered)
- **Model, view and window apart.** `src/model/` holds the design, `src/view/` and `src/present/` what is shown, without
  Win32, and `src/win/` all the Win32 code, so all but the window can be tested.
  [2026-11-09 01:20:32](DEVLOG/DEVLOG1.md#012032-the-nine-steps-in-a-window-with-undo-a-review-and-accessibility-tests)
- **Commands and copies.** Every change is a `Command` given to `history_do`, which keeps the design before it (the
  Command and Memento patterns).
  [2026-11-09 01:20:32](DEVLOG/DEVLOG1.md#012032-the-nine-steps-in-a-window-with-undo-a-review-and-accessibility-tests)
- **A table places the controls.** `SPECS` in `layout.c` gives each control its step, kind, column, height and label.
  [2026-11-09 01:20:32](DEVLOG/DEVLOG1.md#012032-the-nine-steps-in-a-window-with-undo-a-review-and-accessibility-tests)
- **Classic controls.** No manifest, since it is XML; the controls keep the classic look of Windows.
  [2026-11-09 01:20:32](DEVLOG/DEVLOG1.md#012032-the-nine-steps-in-a-window-with-undo-a-review-and-accessibility-tests)
  [2026-11-09 10:05:44](DEVLOG/DEVLOG2.md#100544-the-questions-of-part-1-answered)
- **The window moves the focus on when a control asks it to.** A field of several lines keeps the Tab key and asks
  its parent to move the focus; `move_focus` in `app_window.c` answers, so no control traps the keyboard (WCAG 2.2
  SC 2.1.2).
  [2026-11-10 05:37:00](DEVLOG/DEVLOG3.md#053700-a-keyboard-trap-found-by-a-test-and-mended-71-of-86)
- **The language is told to Windows in C.** `app_open` sets the process's preferred language for its texts to
  `en-US`, in place of a resource of the program, which is not C (WCAG 2.2 SC 3.1.1, Rule 29).
  [2026-11-10 05:43:36](DEVLOG/DEVLOG3.md#054336-windows-told-that-the-window-speaks-english-72-of-86)
- **Help is a line in the status line.** F1 and the menu Help put the help of the control in focus there, from
  `HELP_TEXTS` in `control_help.c`, one text for each control; nothing opens and nothing appears by itself (WCAG
  2.2 SC 3.3.5, 3.2.6, 1.4.13).
  [2026-11-10 05:48:07](DEVLOG/DEVLOG3.md#054807-help-for-every-control-77-of-86)
- **The status line is a live region.** It is marked polite for assistive technology, and every change of its text
  raises Windows' event for a changed live region, so a status message is read without the focus moving (WCAG 2.2
  SC 4.1.3). The window's program and the tests link the same libraries, named once.
  [2026-11-10 05:51:35](DEVLOG/DEVLOG3.md#055135-status-messages-told-to-a-screen-reader-78-of-86)
- **One scale for the display and the text size.** The window's scale is the display's dots an inch times the text
  size of Windows' accessibility settings, 100% to 225% (`layout_text_dpi`, `assist_dpi`); fonts, layout and the
  smallest window follow it (WCAG 2.2 SC 1.4.4).
  [2026-11-10 05:56:10](DEVLOG/DEVLOG3.md#055610-the-text-size-of-windows-followed-and-the-accessibility-suite-in-two-groups)
- **A glossary explains the game's words and the catalog's shortened forms.** Glossary in the menu Help lists them
  with their meanings, from `GLOSSARY` in `control_help.c`; the catalog's texts stay as the original has them
  (WCAG 2.2 SC 3.1.3, 3.1.4). The names of the game's units, buildings and techs are taken as proper names.
  [2026-11-10 06:00:59](DEVLOG/DEVLOG3.md#060059-a-glossary-of-the-games-words-and-of-the-catalogs-shortened-forms-80-of-86)
- **The window draws the focus frame itself.** A solid frame in the focus color, two pixels thick at 100% and
  scaled, is painted on the window's paper around the control in focus, and painted anew when the focus moves
  (WCAG 2.2 SC 2.4.7, 2.4.13).
  [2026-11-10 06:09:12](DEVLOG/DEVLOG3.md#060912-a-solid-focus-frame-drawn-by-the-window-82-of-86)
- **A checklist keeps its place.** After a command, only the rows whose tick changed are set again, and the list's
  top row is kept.
  [2026-11-09 10:06:34](DEVLOG/DEVLOG2.md#100634-a-list-that-keeps-its-place-a-check-in-a-ticked-box-and-no-next-on-the-last-step)
- **Next and Back only where they lead.** The last step has no Next button and the first no Back button.
  [2026-11-09 10:06:34](DEVLOG/DEVLOG2.md#100634-a-list-that-keeps-its-place-a-check-in-a-ticked-box-and-no-next-on-the-last-step)
- **A Unicode build.** `UNICODE` and `_UNICODE` are defined, so every Win32 call takes wide text.
  [2026-11-09 01:20:32](DEVLOG/DEVLOG1.md#012032-the-nine-steps-in-a-window-with-undo-a-review-and-accessibility-tests)
- **The game's data file is read, never changed.** It is found through Steam and unpacked in memory; the data mod
  holds the changed copy.
  [2026-11-09 02:15:04](DEVLOG/DEVLOG1.md#021504-a-mod-built-from-the-civ-from-the-catalog-and-the-games-own-data-file)
- **Only `VER 8.9`.** `dat_map.c` refuses a data file of another version, or one whose walk does not end at its last
  byte.
  [2026-11-09 02:15:04](DEVLOG/DEVLOG1.md#021504-a-mod-built-from-the-civ-from-the-catalog-and-the-games-own-data-file)
- **Old ids keep their numbers.** New effects and techs are written after the old ones, and the rest is changed by
  patches of up to 8 bytes.
  [2026-11-09 02:15:04](DEVLOG/DEVLOG1.md#021504-a-mod-built-from-the-civ-from-the-catalog-and-the-games-own-data-file)
- **Looks are copies of the same size.** A look is bytes of the game's own file written over others as long, so no
  unit moves or grows; damage graphics of another count are left as they are. Copies go before the patches, and an
  edit holds 2 048, four times the most an architecture needed.
  [2026-11-09 04:04:36](DEVLOG/DEVLOG1.md#040436-the-civ-given-the-looks-of-its-architecture-castle-and-wonder)
- **One target for the slot's units.** `DatTarget` names the file, its map, the edit and the slot, for the looks, the
  hero and the bonuses alike.
  [2026-11-09 04:35:41](DEVLOG/DEVLOG1.md#043541-the-civs-hero-trained-at-the-castle-from-the-imperial-age)
  [2026-11-09 05:46:17](DEVLOG/DEVLOG1.md#054617-bonuses-330-339-and-458-built-from-unit-patches)
- **Unit patches share the file of changes.** A field of a unit that a bonus sets is a `unit` line of
  `bonus_effects.txt`, not a file of its own, since `data/` holds the 7 files Rule 14 allows.
  [2026-11-09 05:46:17](DEVLOG/DEVLOG1.md#054617-bonuses-330-339-and-458-built-from-unit-patches)
- **The civ list changed in place.** Only the values that change are written anew; the rest of the game's file is
  copied as it is, so its entries keep their order and form.
  [2026-11-09 05:05:16](DEVLOG/DEVLOG1.md#050516-the-civ-list-in-the-data-mod-naming-the-civs-unique-techs-and-unique-unit)
- **The replaced civ's techs are put out of reach, not removed.** Each gets civ 99, which no civ is, and no research
  building.
  [2026-11-09 02:15:04](DEVLOG/DEVLOG1.md#021504-a-mod-built-from-the-civ-from-the-catalog-and-the-games-own-data-file)
- **Fixed room.** Two static buffers of 100 MiB, 185 changes in the new tech tree, 96 copied techs, and 4 096 lines of
  bonus changes and 512 unit patches in the catalog, as P10 rule 3 asks.
  [2026-11-09 02:15:04](DEVLOG/DEVLOG1.md#021504-a-mod-built-from-the-civ-from-the-catalog-and-the-games-own-data-file)
  [2026-11-09 05:22:54](DEVLOG/DEVLOG1.md#052254-bonus-195-built-and-room-for-4-096-lines-of-changes)
  [2026-11-09 05:46:17](DEVLOG/DEVLOG1.md#054617-bonuses-330-339-and-458-built-from-unit-patches)
- **Compressed with deflate's fixed codes.** The data mod's file is one block of fixed Huffman codes, 13.5 MB of 93
  MB, and stored blocks only if it would not fit the buffer. Replaced: stored blocks until a compressor was written.
  [2026-11-09 02:15:04](DEVLOG/DEVLOG1.md#021504-a-mod-built-from-the-civ-from-the-catalog-and-the-games-own-data-file)
  [2026-11-09 02:32:41](DEVLOG/DEVLOG1.md#023241-the-mods-dat-file-compressed-135-mb-in-place-of-93-mb)
  [2026-11-09 10:05:44](DEVLOG/DEVLOG2.md#100544-the-questions-of-part-1-answered)
- **One set of deflate's tables.** The bases and extra bits of lengths and distances are defined once, in
  `inflate.c`, and declared in `inflate.h` for `deflate.c`.
  [2026-11-09 02:32:41](DEVLOG/DEVLOG1.md#023241-the-mods-dat-file-compressed-135-mb-in-place-of-93-mb)
- **The unique units' techs are in the catalog.** `units.txt` gives each of the game's unique units its
  make-available and elite techs, from the original's `_KM_UU_TECHS`.
  [2026-11-09 02:15:04](DEVLOG/DEVLOG1.md#021504-a-mod-built-from-the-civ-from-the-catalog-and-the-games-own-data-file)
- **The civ descriptions in id order.** The UI mod's strings hold them from 120150 to 120277 in the order of their
  ids, whatever order the game's strings have.
  [2026-11-09 02:15:04](DEVLOG/DEVLOG1.md#021504-a-mod-built-from-the-civ-from-the-catalog-and-the-games-own-data-file)
- **`info.json` as the original writes it,** with `builder_version` "1.0.0", civeditor's version, until it is said
  otherwise. Replaced: "2.7.0", the original's version.
  [2026-11-09 02:15:04](DEVLOG/DEVLOG1.md#021504-a-mod-built-from-the-civ-from-the-catalog-and-the-games-own-data-file)
  [2026-11-09 10:06:18](DEVLOG/DEVLOG2.md#100618-rules-26-to-36-settings-that-deny-commits-and-deletions-and-version-100)
- **"Change" for an entry of an effect,** where the original says command, since a command is a change to a design.
  [2026-11-09 02:15:04](DEVLOG/DEVLOG1.md#021504-a-mod-built-from-the-civ-from-the-catalog-and-the-games-own-data-file)
  [2026-11-09 10:05:44](DEVLOG/DEVLOG2.md#100544-the-questions-of-part-1-answered)
- **The civ's description has a description id of its own,** 990100 plus its slot, which the game's strings do not
  use. The UI mod writes the description under it and under the game's id, and the civ list points at it.
  [2026-11-09 10:06:47](DEVLOG/DEVLOG2.md#100647-the-civs-description-under-a-description-id-of-its-own)
- **A base civ is a line of the catalog.** A `base` line of `appearance.txt` gives a civ's name, bonuses, team
  bonus, unique unit, unique techs and looks by the catalog's ids, under the civ's slot. Replaced: Generic was base 0.
  [2026-11-09 10:07:36](DEVLOG/DEVLOG2.md#100736-a-design-begun-from-a-base-civ-generic-at-first)
  [2026-11-18 07:24:48](DEVLOG/DEVLOG7.md#072448-civs-of-the-game-edited-and-saved-into-one-named-mod)
- **The civs of a session are a roster,** `src/roster/civ_roster.c`: for each of 64 slots a draft, the design saved
  in the mod and whether the mod holds it.
  [2026-11-18 07:24:48](DEVLOG/DEVLOG7.md#072448-civs-of-the-game-edited-and-saved-into-one-named-mod)
- **Several civs, one data file.** The recipes of the civs saved are applied one after another to one `DatEdit`: the
  techs each adds are numbered after those of the civs before it, a requirement added to a tech of the game takes a
  slot that no civ before has patched, the civ list is written civ by civ, and one strings file holds every civ.
  [2026-11-18 07:24:48](DEVLOG/DEVLOG7.md#072448-civs-of-the-game-edited-and-saved-into-one-named-mod)
- **A question answered with a text is a dialog made in memory** (`text_dialog.c`): its template is built while the
  program runs, since a resource file would be a language other than C (Rule 29).
  [2026-11-18 07:24:48](DEVLOG/DEVLOG7.md#072448-civs-of-the-game-edited-and-saved-into-one-named-mod)
- **What is universal is the game's word.** The region of a node, and the civs that have it, are read once from the
  game's own `CivTechTrees` files into the last two columns of `tree.txt`; the civs are bits of one 64-bit number.
  [2026-11-09 10:07:36](DEVLOG/DEVLOG2.md#100736-a-design-begun-from-a-base-civ-generic-at-first)
- **The data mod holds the civ's tech tree file,** the game's `CivTechTrees\<name>.json` of the replaced civ with each
  node of `tree.txt` set to the design: `NotAvailable` when the civ does not keep it; when the design has it and the
  replaced civ had not, the status most civs of the game give it, `ResearchRequired` for 13 nodes and
  `ResearchedCompleted` for the rest, from the last column of `tree.txt`; a node the design has keeps any other
  status. Nodes `tree.txt` does not hold stay as they were. Replaced: `ResearchedCompleted` for every node the
  replaced civ lacked, and before that `NotAvailable` for every node the design lacks.
  [2026-11-10 06:12:47](DEVLOG/DEVLOG3.md#061247-a-node-the-replaced-civ-lacked-takes-the-status-most-civs-give-it)
  [2026-11-09 10:18:41](DEVLOG/DEVLOG2.md#101841-a-tech-tree-screen-that-shows-the-designs-tree-and-all-71-tests-listed)
  [2026-11-09 10:40:46](DEVLOG/DEVLOG2.md#104046-one-rule-for-what-a-civ-keeps-in-the-dat-file-and-the-tech-tree-file-alike)
- **What a civ keeps is decided in one place,** `civ_design_tree_keeps` in `civ_design.c`: the whole tree when the
  design's is empty, and otherwise each node it has and the Villager, both Town Centers, the Mill and the two camps.
  The dat file and the tech tree file both follow it.
  [2026-11-09 10:40:46](DEVLOG/DEVLOG2.md#104046-one-rule-for-what-a-civ-keeps-in-the-dat-file-and-the-tech-tree-file-alike)
- **One way to change a JSON text.** `json_patch.c` replaces values one by one and copies every other byte; the
  civ list and the tech tree file are both changed with it.
  [2026-11-09 10:18:41](DEVLOG/DEVLOG2.md#101841-a-tech-tree-screen-that-shows-the-designs-tree-and-all-71-tests-listed)
- **Dialogs are asked for through a seam.** `ask.c` in `src/shell/` shows every message box and file dialog and
  finds the game's folders; a test gives it a script of answers, so that the window's commands run without a
  dialog on the desktop. The program never sets a script.
  [2026-11-15 22:56:15](DEVLOG/DEVLOG4.md#225615-every-function-of-src-under-crap-10-with-the-window-driven-through-its-dialogs-by-tests)
- **A window that opens starts as saved,** with no file path, whatever a window before it left.
  [2026-11-15 22:56:15](DEVLOG/DEVLOG4.md#225615-every-function-of-src-under-crap-10-with-the-window-driven-through-its-dialogs-by-tests)
- **A tech that cannot be planned leaves no trace in the plan,** so a unique unit whose tech the game lacks is
  skipped and reported, and the rest of the civ is built.
  [2026-11-15 22:56:15](DEVLOG/DEVLOG4.md#225615-every-function-of-src-under-crap-10-with-the-window-driven-through-its-dialogs-by-tests)
- **Membership and a checked index are in `check.c`.** `check_is_among` and `check_index` take the place of
  conditions and bounds written out in each function; a list of values or a table with a fallback names what
  is allowed.
  [2026-11-15 23:21:05](DEVLOG/DEVLOG4.md#232105-no-two-functions-80-alike-any-more)
- **The catalog is searched by one function,** `find_key`, for every table. Replaced: a shared key structure
  in the entries, proposed on 2026-11-10 08:27:52.
  [2026-11-15 23:21:05](DEVLOG/DEVLOG4.md#232105-no-two-functions-80-alike-any-more)
- **The build program makes the coverage.** `build.exe coverage` builds the tests with `--coverage` into
  `build/obj_coverage`, runs them and makes `target/coverage/c/`; gcovr, beside the crapper, is run by hand after it,
  so that the build program starts no program but gcc and the tests.
  [2026-11-16 21:03:39](DEVLOG/DEVLOG5.md#210339-the-answers-recorded-no-cap-on-tests-p10s-bounds-and-check-count-given-up-aa-only-coverage-measured)
- **A program that is running is renamed, not stopped,** when the build must write over it:
  `build\civeditor.exe` becomes `build\civeditor_in_use.exe`, and the window the user has open stays open.
  [2026-11-16 21:03:39](DEVLOG/DEVLOG5.md#210339-the-answers-recorded-no-cap-on-tests-p10s-bounds-and-check-count-given-up-aa-only-coverage-measured)
- **A list's wheel and arrows are taken in the loop of messages,** before Windows handles them, not in a window
  procedure of the list's own: that would need a second function pointer, which the test of P10 rule 9 forbids. A
  list is known by its class, `LISTBOX`; keys and clicks are left to Windows, which slides no list of civeditor on
  them.
  [2026-11-16 23:56:00](DEVLOG/DEVLOG5.md#235600-lists-scroll-a-row-a-notch-and-a-row-an-arrow-without-sliding-under-classic-scroll-bars)
- **A list whose arrow is pressed holds the mouse until the button is let go,** as the scroll bar of Windows does,
  and its arrow repeats only while it holds it.
  [2026-11-16 23:56:00](DEVLOG/DEVLOG5.md#235600-lists-scroll-a-row-a-notch-and-a-row-an-arrow-without-sliding-under-classic-scroll-bars)
- **The room of a step is the window's inside less its scroll bar when the bar is shown,** given by `assist_view`;
  the bar, `app->bar`, is placed and shown by `assist_scroll_range` alone.
  [2026-11-17 00:25:45](DEVLOG/DEVLOG6.md#002545-the-windows-scroll-bar-a-control-of-its-own-classic-like-the-lists)
- **A pointer is not checked for `NULL` inside the code.** Only the result of a function that can give a null
  pointer, of Win32 or of the code, is compared with `NULL`, by its caller and without `CHECK`.
  [2026-11-17 02:39:14](DEVLOG/DEVLOG6.md#023914-every-function-of-src-under-crap-10-the-checks-no-caller-can-fail-taken-out)
- **A condition on constants alone is a `_Static_assert`,** which the compiler checks once, not a `CHECK` at each call.
  [2026-11-17 02:39:14](DEVLOG/DEVLOG6.md#023914-every-function-of-src-under-crap-10-the-checks-no-caller-can-fail-taken-out)
- **A line of the game's strings is read by the digits it starts with and its first quote,** whatever lies between
  them: the game writes a space there on most lines and a tab on 4331, and a line without digits is passed by.
  [2026-11-18 07:24:48](DEVLOG/DEVLOG7.md#072448-civs-of-the-game-edited-and-saved-into-one-named-mod)
- **A file of the game cut short is data, not a fault.** `dat_read_unit` returns whether its cursor is still fine,
  without `CHECK`.
  [2026-11-17 02:39:14](DEVLOG/DEVLOG6.md#023914-every-function-of-src-under-crap-10-the-checks-no-caller-can-fail-taken-out)
- **The catalog reads a row apart from adding it.** `read_tech`, `read_unit_patch` and `read_bonus_change` check the
  fields of a row; the `add_` functions check the room and count it in.
  [2026-11-17 02:39:14](DEVLOG/DEVLOG6.md#023914-every-function-of-src-under-crap-10-the-checks-no-caller-can-fail-taken-out)
- **Data from outside is checked where it enters, and its survivors stand** until a test gives the program such data
  damaged: the game's file, the catalog, a civ file and what Windows gives. A value made from those checks, or from
  constants, is not checked again.
  [2026-11-17 02:40:05](DEVLOG/DEVLOG6.md#024005-a-number-past-the-largest-long-refused-and-the-checks-the-mutator-showed-always-true-taken-out)
- **A number is read up to the largest `long`,** and one past it is no number. Ten digits up to a billion were read
  before, and let a `long` of 32 bits wrap round.
  [2026-11-17 02:40:05](DEVLOG/DEVLOG6.md#024005-a-number-past-the-largest-long-refused-and-the-checks-the-mutator-showed-always-true-taken-out)
- **The decisions of the build program are in `build_plan.c`,** tables and small functions that the tests check;
  `build_steps.c` only makes folders, runs gcc and runs the tests with them, each of its functions one decision at
  most, so that the crapper scores it under 10 though no test runs it.
  [2026-11-17 02:41:02](DEVLOG/DEVLOG6.md#024102-the-build-plan-apart-from-the-steps-that-run-gcc-and-a-test-that-every-test-defined-is-run)
- **The build compiles and links everything before it fails,** counting failures instead of stopping at the first,
  so that one build shows the errors of every source.
  [2026-11-17 02:41:02](DEVLOG/DEVLOG6.md#024102-the-build-plan-apart-from-the-steps-that-run-gcc-and-a-test-that-every-test-defined-is-run)
- **The Appearance popup is `src/popup/`, and its class is registered in `app_window.c`.** `src/win/` holds 7 files
  (Rule 14), and the rule against function pointers allows a window procedure to be named in `app_window.c` only.
  Its controls are controls of the layout, placed `PLACE_POPUP`, so their bindings, help and commands are the same.
  [2026-11-21 04:26:03](DEVLOG/DEVLOG8.md#042603-changes-kept-at-once-the-look-choices-in-a-popup-a-filter-on-every-list-a-build-without-questions)
- **Lists scroll in `src/shell/list_scroll.c`.** The window's loop of messages and the loop of the dialog of
  `text_dialog.c`, which is no longer run by Windows, both hand their messages to `list_scroll_takes`.
  [2026-11-21 04:26:03](DEVLOG/DEVLOG8.md#042603-changes-kept-at-once-the-look-choices-in-a-popup-a-filter-on-every-list-a-build-without-questions)
- **The roster keeps one design a slot.** The draft is the civ of the mod; `roster_keep` marks the slot changed or
  not by comparing the draft with its base civ. Replaced: a second copy of each design that Save civ had saved.
  [2026-11-21 04:26:03](DEVLOG/DEVLOG8.md#042603-changes-kept-at-once-the-look-choices-in-a-popup-a-filter-on-every-list-a-build-without-questions)
- **A dropdown opens and scrolls as a list does:** at once, and one row a notch of the wheel. Its rows are shown
  before Windows would slide them open (`list_scroll_open`), and its wheel goes through `list_scroll.c`. The arrows
  of its scroll bar are left to Windows.
  [2026-11-21 15:13:46](DEVLOG/DEVLOG8.md#151346-a-dropdown-opens-and-scrolls-at-once-as-the-lists-do)
- **A change of the tech tree is one command that carries the whole tree.** `COMMAND_SET_TREE` holds the tree as it
  is after the change, made by `tree_command`, so one undo brings back every node that was unticked with another.
  [2026-11-21 21:19:41](DEVLOG/DEVLOG8.md#211941-unticking-a-node-unticks-its-descendants-and-stronger-and-weaker-edit-a-bonus)
- **On a chosen row, a list draws in the chosen ink:** its text, its check box and its frames of focus;
  `controls_row_color` in `controls.c` holds the rule for every list.
  [2026-11-23 03:53:22](DEVLOG/DEVLOG9.md#035322-the-numbers-of-chosen-bonuses-typed-or-slid-in-place-of-stronger-and-weaker)
- **Chosen bonuses is a panel of its own class,** `CHOSEN_CLASS_NAME`, that holds a label (`STATIC`) for each ticked
  civ bonus, a field (`EDIT`) for each number to set and one card with the slider. They are made when first needed
  and kept, hidden while not needed, until the window closes, so that no field is destroyed in its own message. A
  window that is not a control of the main window is named for a screen reader through Windows' annotations
  (`assist_annotate`), and its focus frame is drawn by the window around it (`assist_draw_ring`).
  [2026-11-23 14:02:39](DEVLOG/DEVLOG9.md#140239-the-numbers-of-chosen-bonuses-typed-in-their-own-fields-the-slider-just-below)
- **A field of Chosen bonuses counts 2 pixels a side for the `EDIT` itself** (`CHOSEN_FIELD_INSET`): its border and
  the pixel it keeps free inside the border. A field is as wide as the widest of its number's values, now and at the
  multipliers 1 and 10, with its margins and that inset, and as tall as its text with the inset above and below, at
  least `CHOSEN_FIELD_LEAST`.
  [2026-11-23 16:03:28](DEVLOG/DEVLOG9.md#160328-the-fields-of-chosen-bonuses-wide-enough-for-their-widest-value-and-their-drawing-tested)
- **A function's name is used in one file only,** even a static one, since the test of recursion joins the calls of
  all files by name: `scroll_to` of `chosen_window.c` became `scroll_panel_to`, and `control_of` of `assist.c`
  became `holding_control`.
  [2026-11-23 14:02:39](DEVLOG/DEVLOG9.md#140239-the-numbers-of-chosen-bonuses-typed-in-their-own-fields-the-slider-just-below)
- **Window code that `src/win/` has no room for goes in a folder of its own** that includes `win_parts.h`, as
  `src/popup/` and `src/chosen/` do (Rule 14); what needs no Win32 goes in a folder apart, as `src/numbers/`.
  [2026-11-23 03:53:22](DEVLOG/DEVLOG9.md#035322-the-numbers-of-chosen-bonuses-typed-or-slid-in-place-of-stronger-and-weaker)
- **An override is a value and whether it is set** (`Override` in `civ_design.h`), for each stat of each rank of the
  unique unit and each tech value of each unique tech; `civ_override_or` gives the game's value of one not set.
  [2026-11-24 06:14:27](DEVLOG/DEVLOG10.md#061427-the-unique-units-stats-and-the-unique-techs-tech-values-and-tech-effects-set-in-uuut)
- **A stat and a tech value lie in a fixed range,** typed in its field or read from a civ file: a cost from 0 to 9999,
  hit points from 1 to 32767, attack from 0 to 999, an armor from -99 to 999, range from 0 to 99, speed from 0.01 to
  9.99 and reload time from 0.01 to 99.99, kept in hundredths, and research time from 1 to 9999 seconds. A value typed
  past it is refused, and the status line names the range.
  [2026-11-24 06:14:27](DEVLOG/DEVLOG10.md#061427-the-unique-units-stats-and-the-unique-techs-tech-values-and-tech-effects-set-in-uuut)
- **A unique tech has at most 6 tech effects** (`CIV_TECH_EFFECT_CAPACITY`). Ticking more says "The tech has no room
  for more effects; the rest were not ticked.", and Choose castle effects... before a tech is picked says "A tech must
  be picked before its effects are chosen."
  [2026-11-24 06:14:27](DEVLOG/DEVLOG10.md#061427-the-unique-units-stats-and-the-unique-techs-tech-values-and-tech-effects-set-in-uuut)
- **A command of a unique tech carries its tier in its field,** as the tier times `TIER_STRIDE`, 100000, plus the tech
  value or the id of the tech effect.
  [2026-11-24 06:14:27](DEVLOG/DEVLOG10.md#061427-the-unique-units-stats-and-the-unique-techs-tech-values-and-tech-effects-set-in-uuut)
- **Choose castle effects... is one change** (`history_replace`), which one undo takes back whole.
  [2026-11-24 06:14:27](DEVLOG/DEVLOG10.md#061427-the-unique-units-stats-and-the-unique-techs-tech-values-and-tech-effects-set-in-uuut)
- **The dialog of a question can tick rows** (`ticks` of `AskQuestion`): its list then takes many rows, it has no
  field for an answer, it is 450 dialog units wide, and OK hands back which rows are ticked. A row of a dialog holds
  `ASK_ROW_WIDTH`, 320, characters, at least `CATALOG_TEXT_CAPACITY`, so that a bonus's text fits.
  [2026-11-24 06:14:27](DEVLOG/DEVLOG10.md#061427-the-unique-units-stats-and-the-unique-techs-tech-values-and-tech-effects-set-in-uuut)
- **A unique tech with tech effects is built as one new effect,** "Unique tech effects": for each tech effect, the
  changes of the game techs that its bonus names, then its lines in `bonus_effects.txt` (`bonus_build_lines`), each
  scaled by its multiplier. A bonus's unit patches are not built for a tech effect, and its lines lose their ages.
  [2026-11-24 06:14:27](DEVLOG/DEVLOG10.md#061427-the-unique-units-stats-and-the-unique-techs-tech-values-and-tech-effects-set-in-uuut)
- **The mod describes a unique tech with tech effects by them:** their bonuses' texts, scaled by their multipliers,
  joined by "; ". Its sheet shows no description of the game's tech.
  [2026-11-24 06:14:27](DEVLOG/DEVLOG10.md#061427-the-unique-units-stats-and-the-unique-techs-tech-values-and-tech-effects-set-in-uuut)
- **In the civ file a value not set is `null`,** and a tech's cost of four zeros is read as not set, the game's cost.
  A tech with tech effects is written with `mode` `custom`, its `vanilla_km_idx` and its `effects` as `id` and
  `multiplier`, the form of the original's custom tech.
  [2026-11-24 06:14:27](DEVLOG/DEVLOG10.md#061427-the-unique-units-stats-and-the-unique-techs-tech-values-and-tech-effects-set-in-uuut)
- **The lines of any bonus are built by `bonus_build_lines`,** a team bonus's or not, in place of `bonus_build_team`.
  [2026-11-24 06:14:27](DEVLOG/DEVLOG10.md#061427-the-unique-units-stats-and-the-unique-techs-tech-values-and-tech-effects-set-in-uuut)
- **The model keeps its check of the range of a stat or tech value,** though every caller fits the value to the range
  first: a command is the model's way in, and the tests of a command out of range hold it. `range_holds` is that one
  check, for the model and for the value the build gives the elite unit.
  [2026-11-24 06:14:27](DEVLOG/DEVLOG10.md#061427-the-unique-units-stats-and-the-unique-techs-tech-values-and-tech-effects-set-in-uuut)
- **The team bonus's panel is a fifth number panel,** `NUMBER_PANEL_TEAM`, and a value set in it a command of its
  own, `COMMAND_SET_TEAM_MULTIPLIER`, so that it is undone as any other change.
  [2026-11-24 06:15:02](DEVLOG/DEVLOG10.md#061502-the-team-bonuss-numbers-set-in-fields-as-a-civ-bonuss)
- **The choice is kept in the options file,** `civeditor_options.txt` in the user's application data, as one
  character; without the file the program asks. The mods folder is still kept only until civeditor closes.
  [2026-11-24](DEVLOG/DEVLOG10.md#the-choice-of-asking-to-save-before-exiting-kept-by-options)
- **A question may carry a tick box** (`check` and `check_label` of `AskQuestion`), which `text_dialog.c` shows only
  then; Options... is one dialog, not two.
  [2026-11-24](DEVLOG/DEVLOG10.md#the-choice-of-asking-to-save-before-exiting-kept-by-options)
- **A question with a tick box may be answered with an empty text,** so that Options... takes the tick while no mods
  folder is known.
  [2026-11-24](DEVLOG/DEVLOG10.md#the-choice-of-asking-to-save-before-exiting-kept-by-options)
- **The height of a list's row is kept for as long as the list keeps its width,** and measured only for an item not
  yet measured; only the lists of the step shown are wrapped. Measuring rows was nearly all the time of a refill.
  [2026-11-24](DEVLOG/DEVLOG10.md#rows-measured-once-a-width-and-less-redrawn)
- **Every control is still placed at every placing,** the hidden ones too: a hidden list left unplaced kept a width
  of 0, which its rows cannot be measured for.
  [2026-11-24](DEVLOG/DEVLOG10.md#rows-measured-once-a-width-and-less-redrawn)
- **The game is read once the window is shown,** by a message that `app_run` posts (`APP_GAME_WANTED`), not while the
  window is made: the read takes most of a second.
  [2026-11-24](DEVLOG/DEVLOG10.md#the-catalogs-numbers-and-nodes-read-from-the-game)
- **A civ to edit with no unsaved change and no file is made its base civ anew after the read,** so that its tech
  tree is the game's; a civ already changed is left as it is.
  [2026-11-24](DEVLOG/DEVLOG10.md#the-catalogs-numbers-and-nodes-read-from-the-game)
- **The numbers are read with the places the build writes them to** (`unique_stats_read` beside
  `unique_build_stats`), and a unit is found as a build finds it, by its make-available tech.
  [2026-11-24](DEVLOG/DEVLOG10.md#the-catalogs-numbers-and-nodes-read-from-the-game)
- **A list is erased with a brush that paints nothing,** and `list_scroll_brush` paints the room under its last row
  when that row ends inside the list, so no row is white before it is drawn, however the list is scrolled. Not
  chosen: drawing without erasing in `move_list` alone, which leaves the room under the last row unpainted at the end
  of a list, since Windows paints nothing there; `WS_EX_COMPOSITED`, with which a list drew no row; a window
  procedure of the list's own, which the project's reading of P10 forbids.
  [2026-11-24 11:26:10](DEVLOG/DEVLOG10.md#112610-a-list-never-paints-its-rows-white-the-team-bonuss-panel-lower-an-error-box-traced-to-a-mutant)
- **The panel of the selected team bonus is 124 high and does not stretch** (`TEAM_PANEL_HEIGHT`): the room of two
  lines of text and the card under them. The list of team bonuses takes the rest of the column, and a longer text
  scrolls in the panel. Replaced: the panel stretched and shared the column with the list. Whether 124 is right is
  an open question.
  [2026-11-24 11:26:10](DEVLOG/DEVLOG10.md#112610-a-list-never-paints-its-rows-white-the-team-bonuss-panel-lower-an-error-box-traced-to-a-mutant)

## Tests

- **A test answers Choose hero... with a hook on its thread** (`window_fixture_answer`), which picks the row,
  types the name and presses OK when the dialog starts, so that the real dialog runs and is never shown.
  [2026-11-20 03:00:57](DEVLOG/DEVLOG7.md#030057-the-hero-is-chosen-in-uuut-with-a-hero-box-and-choose-hero)
- **The tests that walk every control of every step tick Hero first,** so that Choose hero... is among them.
  [2026-11-20 03:00:57](DEVLOG/DEVLOG7.md#030057-the-hero-is-chosen-in-uuut-with-a-hero-box-and-choose-hero)
- **The fixture keeps what the dialog of Choose hero... showed** (`window_fixture_asked`): its rows, the label of
  its list and whether the list had the focus. A test does not pass the fixture a function of its own to look at
  the dialog, since the window procedure is the only function pointer allowed.
  [2026-11-20 03:00:57](DEVLOG/DEVLOG7.md#030057-the-hero-is-chosen-in-uuut-with-a-hero-box-and-choose-hero)
- **Mutants of the window's files are measured with `--timeout-factor 40`.** With the mutator's default, ten
  times the baseline, mutants that fail no test were reported killed when many workers ran the window's tests at
  once; `--mutate-all` measures again what such a run reported killed.
  [2026-11-20 03:00:57](DEVLOG/DEVLOG7.md#030057-the-hero-is-chosen-in-uuut-with-a-hero-box-and-choose-hero)
- **A test of a bug that a human found is a core test,** in the group of its subject: the bug raises Rule 31's
  count by one. A test of what a core test covers already is a regression test when the log names a bug that breaks
  what it covers, and a redundancy test otherwise; a test past the limit with no such bug is added only when its
  reason, with how plausibly what it tests could fail, has been accepted. Replaced: Rule 34's category Regression
  kept for tests ordered to cover what a test covers already.
  [2026-11-19 21:28:52](DEVLOG/DEVLOG7.md#212852-a-turn-of-the-wheel-that-cannot-scroll-the-step-does-nothing)
  [2026-11-22 23:08:30](DEVLOG/DEVLOG8.md#230830-the-core-tests-within-rule-31-and-a-list-joined-by-commas-scaled-as-one)
- **A test may read what Windows has left to redraw** (`GetUpdateRect`) when what the user saw was the window
  drawn again, as Test 8 does; the window is then shown far off the screen, since a hidden one has nothing to
  redraw.
  [2026-11-19 21:28:52](DEVLOG/DEVLOG7.md#212852-a-turn-of-the-wheel-that-cannot-scroll-the-step-does-nothing)
- **A run that opens the window waits until the computer is free.** With the window's tests the mutator runs a
  worker for each core, each opening the real window again and again, which leaves the computer unusable; such a
  run, and a build without `nowindow`, is started only when the computer can be given over to it.
  [2026-11-19 20:19:34](DEVLOG/DEVLOG7.md#201934-five-steps-and-no-field-for-the-civs-description)
- **The tests number themselves.** `tests/runner.c` runs the core tests of the requirements and bugs, the
  accessibility tests and the core tests of the rules, numbering them together as they run, then the regression tests
  and, with `redundancy`, the redundancy tests, each numbered on their own. Replaced: the functional, accessibility
  and maintainability tests, in that order.
  [2026-11-09 00:44:18](DEVLOG/DEVLOG1.md#004418-civbuilder-begun-in-c-with-its-own-build-and-rule-tests)
  [2026-11-22 23:08:30](DEVLOG/DEVLOG8.md#230830-the-core-tests-within-rule-31-and-a-list-joined-by-commas-scaled-as-one)
- **Mutation by hand.** ub-mode's mutator does not read C, so a new test is shown to fail by breaking the code
  (Rules 33 and 34).
  [2026-11-09 00:44:18](DEVLOG/DEVLOG1.md#004418-civbuilder-begun-in-c-with-its-own-build-and-rule-tests)
  [2026-11-09 10:06:18](DEVLOG/DEVLOG2.md#100618-rules-26-to-36-settings-that-deny-commits-and-deletions-and-version-100)
- **Scrolling and drawing are tested in the real window.** Bug 1 is caught again by a test that scrolls and ticks
  the list; Bug 2 by a test that has the window draw a ticked row into a picture in memory and looks for the check
  in its box. Replaced: no test of drawing or scrolling; then: scrolling has a test, drawing has none.
  [2026-11-10 06:54:15](DEVLOG/DEVLOG3.md#065415-a-test-that-catches-bug-2-again-and-the-program-started)
  [2026-11-10 05:16:33](DEVLOG/DEVLOG3.md#051633-a-test-that-catches-bug-1-again)
  [2026-11-09 10:06:34](DEVLOG/DEVLOG2.md#100634-a-list-that-keeps-its-place-a-check-in-a-ticked-box-and-no-next-on-the-last-step)
- **The test of the log reads every part,** while `DEVLOG<n>.md` exists, and lets a part begin with the day the
  part before it ended with only when that part holds fewer than two days.
  [2026-11-09 10:06:18](DEVLOG/DEVLOG2.md#100618-rules-26-to-36-settings-that-deny-commits-and-deletions-and-version-100)
- **The real window is tested without being shown.** `app_open` makes the window and its controls; a test drives
  it by messages and asks Windows what the keyboard would do, and never takes the keyboard or the mouse. The test
  program holds `src/win/` and links the libraries the window needs. Replaced: the tests do not run `src/win/`.
  [2026-11-10 05:09:30](DEVLOG/DEVLOG3.md#050930-a-test-that-drives-the-real-window-and-the-logs-third-part)
- **What a screen reader is told is asked of Windows.** A test asks Windows' accessibility interface (MSAA) for the
  name of each control, as a screen reader does; the code is compiled with `COBJMACROS` so that such a call keeps
  one level of dereferencing, and the test holds the id of `IAccessible` itself, since the compiler's library gives
  a wrong one.
  [2026-11-10 05:21:07](DEVLOG/DEVLOG3.md#052107-the-names-a-screen-reader-is-told-asked-of-windows-66-of-86)
- **The layout is tested without a window.** The accessibility tests measure `layout.c` and `palette.c` at the
  smallest window, 960 by 560, so they run in the build without Win32.
  [2026-11-09 01:20:32](DEVLOG/DEVLOG1.md#012032-the-nine-steps-in-a-window-with-undo-a-review-and-accessibility-tests)
- **No test needs the game.** The tests make small data files in memory; the installed game was read only by check
  programs outside the project. A window that a test opens without a script reads the texts of an installed game,
  which no test reads.
  [2026-11-09 02:15:04](DEVLOG/DEVLOG1.md#021504-a-mod-built-from-the-civ-from-the-catalog-and-the-games-own-data-file)
  [2026-11-18 07:24:48](DEVLOG/DEVLOG7.md#072448-civs-of-the-game-edited-and-saved-into-one-named-mod)
- **A new group before 60 lines.** When a suite's `switch` would pass 60 lines (Rule 11), its later cases move to a
  new group, and every test keeps its number.
  [2026-11-09 05:46:17](DEVLOG/DEVLOG1.md#054617-bonuses-330-339-and-458-built-from-unit-patches)
  [2026-11-09 10:18:41](DEVLOG/DEVLOG2.md#101841-a-tech-tree-screen-that-shows-the-designs-tree-and-all-71-tests-listed)
- **TESTS.md cites the requirements by their numbers.** The proposals of the first questions became Requirements 1
  to 9, and each test's purpose names the one it serves; a test of a bug found before cites Rule 24.
  [2026-11-09 10:18:41](DEVLOG/DEVLOG2.md#101841-a-tech-tree-screen-that-shows-the-designs-tree-and-all-71-tests-listed)
- **The recipe is tested on a small game.** `tests/dat/recipe_game.c` makes a dat file, a catalog and a design, in
  a full and a bare scenario, runs `recipe_apply` and reads the built dat file back; each test of
  `recipe_tests.c` asks one thing of it. The small dat of `tests/dat/small_dat.c` is shared with the dat tests.
  [2026-11-15](DEVLOG/DEVLOG4.md#the-recipe-run-by-tests-on-a-small-game-237-functions-at-crap-10-or-above)
- **While the mutator cannot run, a new test is shown honest by mutants made by hand,** one at a time, each undone,
  and the log names those that survived.
  [2026-11-15](DEVLOG/DEVLOG4.md#the-recipe-run-by-tests-on-a-small-game-237-functions-at-crap-10-or-above)
- **A new test is named in the suite of its category,** in groups of at most eight tests, so that no group's
  complexity reaches 10: `requirement_suite.c` for the core tests of the requirements and bugs,
  `maintainability_suite.c` for those of the rules, `regression_suite.c` for the regression tests, and
  `redundancy_suite.c`, `functional_suite.c` with the suites of `tests/suites/`, `shell_suite.c` or
  `rule_redundancy.c` for the redundancy tests. The test "TESTS.md documents every test" counts the cases of the
  suites of the core and accessibility tests. Replaced: every new test named in `tests/suites/`, after the first 54
  of `functional_suite.c`.
  [2026-11-15 22:14:14](DEVLOG/DEVLOG4.md#221414-a-mod-built-end-to-end-by-tests-166-functions-at-crap-10-or-above)
  [2026-11-22 23:08:30](DEVLOG/DEVLOG8.md#230830-the-core-tests-within-rule-31-and-a-list-joined-by-commas-scaled-as-one)
- **The test runner tries up to 1000 tests of a category, and a category that reaches them fails the run.**
  `CATEGORY_TEST_LIMIT` in `tests/runner.c` bounds a loop (P10) and is no rule of how many tests there may be; the
  run fails with a line that names it, since at 400 the 401st functional test was left out without a word.
  Replaced: 100; 400, reached without a failure.
  [2026-11-15 22:14:14](DEVLOG/DEVLOG4.md#221414-a-mod-built-end-to-end-by-tests-166-functions-at-crap-10-or-above)
  [2026-11-22 02:10:47](DEVLOG/DEVLOG8.md#021047-ticking-a-node-ticks-its-needs-in-the-first-regression-test)
- **A whole mod is built by the tests,** from a small game written to `build/test_game` into `build/test_mods`;
  both are build output and are written over by each run.
  [2026-11-15 22:14:14](DEVLOG/DEVLOG4.md#221414-a-mod-built-end-to-end-by-tests-166-functions-at-crap-10-or-above)
- **A step of a test's fixture that must succeed is a `CHECK`,** so that it adds no branch that the tests can
  never take; what a test asks stays a plain condition.
  [2026-11-15 22:14:14](DEVLOG/DEVLOG4.md#221414-a-mod-built-end-to-end-by-tests-166-functions-at-crap-10-or-above)
- **Steam's folder and the user's home can be named.** `mod_find_game_under` and `mod_find_mods_under` take
  them, so that tests find the game and the mods folder under `build/`; the program passes the real ones.
  [2026-11-15 22:36:08](DEVLOG/DEVLOG4.md#223608-all-of-src-but-the-window-under-crap-10-110-functions-left)
- **A fixture of bytes may be made once with a tool of Windows.** The dynamic stream of `dynamic_tests.c` was
  packed with PowerShell's deflater and is kept as bytes in the test; no tool is run by the tests or the build.
  [2026-11-15 22:36:08](DEVLOG/DEVLOG4.md#223608-all-of-src-but-the-window-under-crap-10-110-functions-left)
- **A test that asks the same of other values is one line** that calls a shared helper; the helper gives a value
  or takes the values to ask about. So tests of one check (Rule 24) are not alike for the dryer (Rule 31).
  [2026-11-15 23:21:05](DEVLOG/DEVLOG4.md#232105-no-two-functions-80-alike-any-more)
- **A rule test that is restructured is shown to fail on a fault made on purpose,** which is then undone; the
  log names the faults.
  [2026-11-15 23:21:05](DEVLOG/DEVLOG4.md#232105-no-two-functions-80-alike-any-more)
- **Each scanner of a rule is shown a fault.** `tests/fixtures/broken_rules.txt` breaks eight rules, a line each,
  and a gate test asks each scanner for that line; the text is a document, so the rule tests pass it by.
  [2026-11-15 23:36:40](DEVLOG/DEVLOG4.md#233640-tests-and-src-under-crap-10-and-eight-tests-that-show-the-rule-tests-see-a-fault)
- **A group of tests holds at most eight,** so that the function that runs it stays under CRAP 10; a larger
  group is divided where its subject changes, and the order of the tests is kept.
  [2026-11-15 23:36:40](DEVLOG/DEVLOG4.md#233640-tests-and-src-under-crap-10-and-eight-tests-that-show-the-rule-tests-see-a-fault)
- **A tool is a part that takes its sources and its output, and a `main` that gives them.** So a test runs
  the dryer and the crapper on a fixture; `tests/rules/tool_tests.c` holds the tests of the tools.
  [2026-11-15 23:56:59](DEVLOG/DEVLOG4.md#235659-the-dryer-the-crapper-and-the-builds-plan-run-by-tests-27-functions-of-the-tools-left)
- **A function that starts the compiler holds no decision of its own;** what it decides is asked of a part
  that tests run, and a step that fails only when a text does not fit its place is a `CHECK`.
  [2026-11-15 23:56:59](DEVLOG/DEVLOG4.md#235659-the-dryer-the-crapper-and-the-builds-plan-run-by-tests-27-functions-of-the-tools-left)
- **The mutator's program is not restructured while it cannot run** (Rule 36).
  [2026-11-15 23:56:59](DEVLOG/DEVLOG4.md#235659-the-dryer-the-crapper-and-the-builds-plan-run-by-tests-27-functions-of-the-tools-left)
- **What each program is built from is one table,** `PROGRAM_PREFIXES` in `source_list.c`, read by
  `source_in_program`.
  [2026-11-15 23:56:59](DEVLOG/DEVLOG4.md#235659-the-dryer-the-crapper-and-the-builds-plan-run-by-tests-27-functions-of-the-tools-left)
- **A test names a requirement or a bug found before, never Rule 4 alone** (Rules 4 to 6, 24). Rule 4 asks only
  that there are tests; it is no test's purpose. The tests that named Rule 4 alone were removed. Replaced: a test of code
  that no requirement names names Requirement 10.
  [2026-11-16 21:03:39](DEVLOG/DEVLOG5.md#210339-the-answers-recorded-no-cap-on-tests-p10s-bounds-and-check-count-given-up-aa-only-coverage-measured)
  [2026-11-17 10:57:53](DEVLOG/DEVLOG7.md#105753-no-test-names-rule-4-alone-the-21-that-did-are-removed)
- **Tests may be written again; a test that only ub-mode needs is a redundancy test.** It is in a file that no test
  of another category shares (Rule 34), in `tests/redundancy/` or in the folder of its subject, runs after the other
  tests only when `redundancy` is given, as the measures of Requirements 10 and 12 give it, and is numbered on its
  own, "Redundancy test 3". Replaced: no test is written for now; then, kept in `tests/redundancy/` alone.
  [2026-11-17 10:57:53](DEVLOG/DEVLOG7.md#105753-no-test-names-rule-4-alone-the-21-that-did-are-removed)
  [2026-11-18 07:24:48](DEVLOG/DEVLOG7.md#072448-civs-of-the-game-edited-and-saved-into-one-named-mod)
  [2026-11-22 23:08:30](DEVLOG/DEVLOG8.md#230830-the-core-tests-within-rule-31-and-a-list-joined-by-commas-scaled-as-one)
- **The crapper measures a coverage built anew.** Every `.c` file is touched before `build.exe coverage redundancy`,
  since gcc adds the counts of a run to those of the runs before while an object stays the same.
  [2026-11-18 07:24:48](DEVLOG/DEVLOG7.md#072448-civs-of-the-game-edited-and-saved-into-one-named-mod)
- **A survivor of the mutator is read before a test is written for it.** One on a check that no caller can fail,
  on the `return` under such a check or on a loop's second bound is taken out with the check or the bound; one
  elsewhere gets a test that calls the code as its user does. Replaced: such survivors left until P10 or the
  mutator gave way.
  [2026-11-16 04:39:06](DEVLOG/DEVLOG5.md#043906-ub-modes-mutator-run-and-read-three-survivors-in-four-are-made-by-p10s-checks-and-bounds)
  [2026-11-16 21:03:39](DEVLOG/DEVLOG5.md#210339-the-answers-recorded-no-cap-on-tests-p10s-bounds-and-check-count-given-up-aa-only-coverage-measured)
- **A survivor that no user of the game and no caller meets is left,** named in the log with the state that would
  tell it from the code, such as slot 0 or a civ list of exactly 524288 bytes; where a shape of the code makes it, as a
  `return true` that only stops the chain of actions, that shape is put as a question.
  [2026-11-18 07:24:48](DEVLOG/DEVLOG7.md#072448-civs-of-the-game-edited-and-saved-into-one-named-mod)
- **An uncovered mutant is read as a survivor is.** One on a line that a user reaches, such as a tech whose free
  places for requirements the civs before have taken, gets a test; one on a line that no user reaches is named in
  the log with the state that would reach it; one on a declaration, which holds no code to run, is named alone.
  [2026-11-18 07:24:48](DEVLOG/DEVLOG7.md#072448-civs-of-the-game-edited-and-saved-into-one-named-mod)
- **A result that the sizes of its texts make certain is cast to `(void)`,** and a `_Static_assert` states the
  sizes, as for the picker's text of a civ's name in `put_name`; a check that could never fail is not kept.
  [2026-11-18 07:24:48](DEVLOG/DEVLOG7.md#072448-civs-of-the-game-edited-and-saved-into-one-named-mod)
- **A text that is built up starts from `text_copy` or from an empty initializer,** not from its first letter
  emptied before the appends: a mutant of that letter keeps the start of an older text, which no test sees.
  [2026-11-18 07:24:48](DEVLOG/DEVLOG7.md#072448-civs-of-the-game-edited-and-saved-into-one-named-mod)
- **The mutator runs one file at a time** while its cleanup can stop a run with "being used by another process": a
  run that stops loses only its file, which is run again.
  [2026-11-18 07:24:48](DEVLOG/DEVLOG7.md#072448-civs-of-the-game-edited-and-saved-into-one-named-mod)
- **A test of an order uses values that differ,** such as numbered changes, not one value put on and off.
  [2026-11-16 04:39:06](DEVLOG/DEVLOG5.md#043906-ub-modes-mutator-run-and-read-three-survivors-in-four-are-made-by-p10s-checks-and-bounds)
- **A test may measure time when what it checks is a time,** as Regression test 27 does for sliding: each move must
  take less than 150 ms, where sliding takes 190 ms or more. It shows the window on the screen for a moment, since
  Windows slides only what can be seen.
  [2026-11-16 23:56:00](DEVLOG/DEVLOG5.md#235600-lists-scroll-a-row-a-notch-and-a-row-an-arrow-without-sliding-under-classic-scroll-bars)
- **A test of writing and reading back compares what is read,** not only two writings, since two writings by the same
  writer agree even when both lose something, as Redundancy test 492 does.
  [2026-11-17 02:40:05](DEVLOG/DEVLOG6.md#024005-a-number-past-the-largest-long-refused-and-the-checks-the-mutator-showed-always-true-taken-out)
- **No test checks that every test defined is run.** The test that compared the tests run with the `case`s of the
  suites named Rule 4 alone and was removed, so a wrong size of a suite or start of a group leaves tests out without
  a failure. Replaced: the last test checks that every test defined is run.
  [2026-11-17 02:41:02](DEVLOG/DEVLOG6.md#024102-the-build-plan-apart-from-the-steps-that-run-gcc-and-a-test-that-every-test-defined-is-run)
  [2026-11-17 10:57:53](DEVLOG/DEVLOG7.md#105753-no-test-names-rule-4-alone-the-21-that-did-are-removed)
- **Redundancy tests of `src/present/` are in `tests/redundancy/present/`,** with their own list of names in
  `filter_tests.c`, since `tests/redundancy/` holds 7 files and its suite 400 lines (Rules 13 and 14).
  [2026-11-21 04:26:03](DEVLOG/DEVLOG8.md#042603-changes-kept-at-once-the-look-choices-in-a-popup-a-filter-on-every-list-a-build-without-questions)
- **A test of the window that needs a whole build is not written.** The game of the tests builds no civ whole, so
  the built mark is tested through `summary_build_is_whole` only.
  [2026-11-21 04:26:03](DEVLOG/DEVLOG8.md#042603-changes-kept-at-once-the-look-choices-in-a-popup-a-filter-on-every-list-a-build-without-questions)
- **A bug reported with a request is listed in `BUGS.md`,** with the time of the report, and its tests name it
  (Rules 6 and 33): the dropdowns that slid open are Bug 1 of 2026-11-21.
  [2026-11-21 15:13:46](DEVLOG/DEVLOG8.md#151346-a-dropdown-opens-and-scrolls-at-once-as-the-lists-do)
- **A test that passed before its fix is not kept.** The time of a wheel turn over an open dropdown was under 150 ms
  before the change too, so only the row it moves to is tested.
  [2026-11-21 15:13:46](DEVLOG/DEVLOG8.md#151346-a-dropdown-opens-and-scrolls-at-once-as-the-lists-do)
- **Regression tests are in `tests/regression/`, run every time and numbered on their own.** `tests/runner.c` runs
  them after the core tests of the rules, as "Regression test 1". Each catches again a bug that the log names, and
  cites it, or covers what the chief developer asked for; a new one is written only at the chief developer's
  direction (Rule 34).
  [2026-11-22 02:10:47](DEVLOG/DEVLOG8.md#021047-ticking-a-node-ticks-its-needs-in-the-first-regression-test)
  [2026-11-22 23:08:30](DEVLOG/DEVLOG8.md#230830-the-core-tests-within-rule-31-and-a-list-joined-by-commas-scaled-as-one)
- **Tests 278, 81 and 401 are regression tests of Requirement 8,** Regression tests 2, 3 and 19; the other 16 tests
  that the core category was given at 2026-11-21 21:19:41 are redundancy tests.
  [2026-11-22 23:08:30](DEVLOG/DEVLOG8.md#230830-the-core-tests-within-rule-31-and-a-list-joined-by-commas-scaled-as-one)
- **A helper that a test of another category needs stays in its file,** named in a header of its folder, its name
  starting with its file's subject, as `mod_tests_named_civ`. A helper that only the tests moved used moves with
  them, and a constant or a helper of a few lines is copied.
  [2026-11-22 23:08:30](DEVLOG/DEVLOG8.md#230830-the-core-tests-within-rule-31-and-a-list-joined-by-commas-scaled-as-one)
- **Of the checks of P10, only that of goto is a core test** (Test 62). The other five and the gate tests are
  redundancy tests while questions 1 and 2 of 2026-11-22 23:08:30 are open.
  [2026-11-22 23:08:30](DEVLOG/DEVLOG8.md#230830-the-core-tests-within-rule-31-and-a-list-joined-by-commas-scaled-as-one)
- **The test of Tab moves the focus to each control it passes,** as Tab does, since a control can be shown by the
  focus, as the slider is, and a control is reached when the focus is on a window inside it, as on a field of
  Chosen bonuses.
  [2026-11-23 03:53:22](DEVLOG/DEVLOG9.md#035322-the-numbers-of-chosen-bonuses-typed-or-slid-in-place-of-stronger-and-weaker)
  [2026-11-23 14:02:39](DEVLOG/DEVLOG9.md#140239-the-numbers-of-chosen-bonuses-typed-in-their-own-fields-the-slider-just-below)
- **Chosen bonuses is an input that needs a label of its own** (Test 13), as the text fields and the lists do; its
  fields are named by the text of their bonus and its slider by its range. Replaced: the slider as an input of its
  own, with a label in `layout.c`.
  [2026-11-23 03:53:22](DEVLOG/DEVLOG9.md#035322-the-numbers-of-chosen-bonuses-typed-or-slid-in-place-of-stronger-and-weaker)
  [2026-11-23 14:02:39](DEVLOG/DEVLOG9.md#140239-the-numbers-of-chosen-bonuses-typed-in-their-own-fields-the-slider-just-below)
- **The mutator runs a folder of the window with one worker** (`--max-workers 1`): with a worker for each core, the
  windows of their runs got in each other's way, and a test of the window failed before the first mutant.
  [2026-11-23 03:53:22](DEVLOG/DEVLOG9.md#035322-the-numbers-of-chosen-bonuses-typed-or-slid-in-place-of-stronger-and-weaker)
- **A mutant that a new test kills is checked by hand when the mutator cannot run it:** it is written into the
  file, the tests are run with the window and seen to fail at the new test, and the file is put back and compared
  with its copy, one build at a time.
  [2026-11-23 16:03:28](DEVLOG/DEVLOG9.md#160328-the-fields-of-chosen-bonuses-wide-enough-for-their-widest-value-and-their-drawing-tested)
- **A test build that opens the window may run when a test needs it,** one at a time, in the foreground. A run of the
  mutator with the window still needs to be agreed first.
  [2026-11-24 06:14:27](DEVLOG/DEVLOG10.md#061427-the-unique-units-stats-and-the-unique-techs-tech-values-and-tech-effects-set-in-uuut)
- **gcovr reads the coverage with `--gcov-ignore-parse-errors=suspicious_hits.warn`,** since the counts of
  `build/obj_coverage` add up over the runs and no agent may delete them (Rule 30).
  [2026-11-24 06:14:27](DEVLOG/DEVLOG10.md#061427-the-unique-units-stats-and-the-unique-techs-tech-values-and-tech-effects-set-in-uuut)
- **The civ of stats of the test game also has a castle tech effect,** `MIXED_BONUS` counted twice, so that the tests
  of tech effects need no civ of their own while `recipe_game.c` is at 400 lines (Rule 13).
  [2026-11-24 06:14:27](DEVLOG/DEVLOG10.md#061427-the-unique-units-stats-and-the-unique-techs-tech-values-and-tech-effects-set-in-uuut)
- **The call graph of the tests holds 3200 functions and 32000 calls,** since the code passed 2400 functions.
  [2026-11-24 06:14:27](DEVLOG/DEVLOG10.md#061427-the-unique-units-stats-and-the-unique-techs-tech-values-and-tech-effects-set-in-uuut)
- **The window fixture ticks a row in a list that takes ticks,** as Choose castle effects... has, and picks it in
  any other list.
  [2026-11-24 06:14:27](DEVLOG/DEVLOG10.md#061427-the-unique-units-stats-and-the-unique-techs-tech-values-and-tech-effects-set-in-uuut)
- **The small game grows only for a test that asks for it:** with `small.elite` set, the soldier of each civ costs
  food, gold and population, has an attack bonus before its attack, and an elite unit with hit points and a reload
  time of its own; the game of every other test stays as it was.
  [2026-11-24 06:14:27](DEVLOG/DEVLOG10.md#061427-the-unique-units-stats-and-the-unique-techs-tech-values-and-tech-effects-set-in-uuut)
- **A survivor that only the window meets is killed by a test that asks what the window asks,** without the window:
  `civ_design_has_tech_effect` as the ticks of Choose castle effects... ask it, and `number_name` as a field's name.
  [2026-11-24 06:14:27](DEVLOG/DEVLOG10.md#061427-the-unique-units-stats-and-the-unique-techs-tech-values-and-tech-effects-set-in-uuut)
- **A bug reported gets a row in `BUGS.md` and a core test.** The team bonus that could not be edited is Bug 1 of
  2026-11-24, covered by Test 10; the tests written for the rest of that change are redundancy tests.
  [2026-11-24 06:15:02](DEVLOG/DEVLOG10.md#061502-the-team-bonuss-numbers-set-in-fields-as-a-civ-bonuss)
- **No test measures a speed.** No requirement names one, and a test of a time fails by chance; the speed of the
  window is measured with a probe that is not kept in the tests, and its numbers are written in the log.
  [2026-11-24](DEVLOG/DEVLOG10.md#rows-measured-once-a-width-and-less-redrawn)
- **The mutator with the window runs with one worker, on the lines of the change** (`--max-workers 1 --lines`), about
  20 seconds a mutant: with two workers its baseline fails, since the test of the popup of the look choices does not
  hold while another test program shows a window. Whether such a run may be made without asking is an open question.
  [2026-11-24](DEVLOG/DEVLOG10.md#the-changes-of-the-day-gated-and-measured)
- **A survivor on a line that no change touched is counted, not read,** in the section of the run; reading those is
  a task of its own.
  [2026-11-24](DEVLOG/DEVLOG10.md#the-changes-of-the-day-gated-and-measured)
- **What a list erases is tested with a picture in memory:** the list is sent `WM_ERASEBKGND` with a black picture
  and a pixel of it is read, since a window far off the screen, where the tests keep it, draws nothing that can be
  read. A blink itself is measured on the screen of the built program, before and after, outside the tests.
  [2026-11-24 11:26:10](DEVLOG/DEVLOG10.md#112610-a-list-never-paints-its-rows-white-the-team-bonuss-panel-lower-an-error-box-traced-to-a-mutant)

## Documents

- **Sections written before the rename keep the name civbuilder,** and so do their titles and the links to them,
  as the log is not rewritten; so do the notes that Git ignores.
  [2026-11-20 03:59:19](DEVLOG/DEVLOG7.md#035919-the-project-named-civeditor)
- **`BUGS.md` is in `documentation/`,** Rule 33's table of three columns and nothing else, not even a title. A
  bug's "Discovered" is the time it was recorded when the time it was found is not known. The bugs found before
  it are not added while question 4 of 2026-11-17 10:57:53 is open.
  [2026-11-19 21:28:52](DEVLOG/DEVLOG7.md#212852-a-turn-of-the-wheel-that-cannot-scroll-the-step-does-nothing)
- **`TESTS.md` lists the regression and the redundancy tests apart,** after the other tests, under headings
  "Regression test N" and "Redundancy test N" that its checks of the other tests do not count.
  [2026-11-18 07:24:48](DEVLOG/DEVLOG7.md#072448-civs-of-the-game-edited-and-saved-into-one-named-mod)
  [2026-11-22 02:10:47](DEVLOG/DEVLOG8.md#021047-ticking-a-node-ticks-its-needs-in-the-first-regression-test)
- **The test numbers of `BUGS.md` follow the tests.** When the tests are renumbered, the number in a bug's status is
  corrected; a bug is still added only when a human reports it (Rule 33).
  [2026-11-22 02:10:47](DEVLOG/DEVLOG8.md#021047-ticking-a-node-ticks-its-needs-in-the-first-regression-test)
- **repo_template's documents, without its setup.** `.claude/skills/project-setup` is not copied: the project is set
  up. [2026-11-09 00:44:18](DEVLOG/DEVLOG1.md#004418-civbuilder-begun-in-c-with-its-own-build-and-rule-tests)
- **The documents point at no file that Git ignores.** The instructions, the answers to the first questions and
  the sitrep are kept where Git does not track them, and the log records what they asked. Replaced: a folder of
  their own in the main folder, which `AGENTS.md` and `MAINTAINER.md` pointed at; before it, the sitrep in the
  main folder, as Rule 14 allowed.
  [2026-11-10 04:21:27](DEVLOG/DEVLOG3.md#042127-the-instructions-in-a-folder-of-their-own-pictures-as-documents-and-a-night-stopped-by-the-usage-limit)
  [2026-11-09 10:05:44](DEVLOG/DEVLOG2.md#100544-the-questions-of-part-1-answered)
  [2026-11-16 21:03:39](DEVLOG/DEVLOG5.md#210339-the-answers-recorded-no-cap-on-tests-p10s-bounds-and-check-count-given-up-aa-only-coverage-measured)
  [2026-11-20 04:06:25](DEVLOG/DEVLOG7.md#040625-the-documents-point-at-no-file-that-git-ignores)
- **The checklist of accessibility is `.github/ACCESSIBILITY.md`,** repo_template's file of that name, since
  `documentation/` holds the 7 files that Rule 14 allows and Rule 15 allows one file of a name.
  [2026-11-09 10:08:11](DEVLOG/DEVLOG2.md#100811-a-checklist-of-wcag-22-for-requirement-4)
- **The tree's items are nodes.** The universal, regional and unique items of the order are universal, regional and
  unique nodes, as `TERMINOLOGY.md` names each unit, building and tech of the tree a node.
  [2026-11-09 10:07:36](DEVLOG/DEVLOG2.md#100736-a-design-begun-from-a-base-civ-generic-at-first)
- **An entry of `TESTS.md` carries its test's name,** as a run of the tests prints it; the entries are made again
  from such a run.
  [2026-11-23 03:53:22](DEVLOG/DEVLOG9.md#035322-the-numbers-of-chosen-bonuses-typed-or-slid-in-place-of-stronger-and-weaker)
- **A unique tech's ticked bonuses are its tech effects,** apart from an effect of the dat file; a stat is any number
  of the unique unit, its costs too, and a tech value is a cost or the research time of a unique tech.
  [2026-11-24 06:14:27](DEVLOG/DEVLOG10.md#061427-the-unique-units-stats-and-the-unique-techs-tech-values-and-tech-effects-set-in-uuut)
