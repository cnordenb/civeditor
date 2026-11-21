# Development log, part 8

- [2026-11-21](#2026-11-21)

Part 8 of the development log. It follows [part 7](DEVLOG7.md). The decisions made in the log are listed in
[DECISIONS.md](../DECISIONS.md).

## 2026-11-21

### 02:20:51: README.md says how the binary is compiled

#### What changed

- **Why.** The Compilation chapter of `README.md` said to run `build.exe`, which a clone of the project does not
  have: Git ignores it, and it is made from `tools/build/`.
- **`README.md`.** Compilation has four steps in place of three: the third makes the build program with
  `gcc -o build.exe tools/build/*.c src/core/*.c`, the first time only, and the fourth runs `.\build.exe` and says
  that the binary is left at `build\civeditor.exe`. The commands are those of the first chapter of `MAINTAINER.md`.
- **Tests.** As before; only a document changed.
- **Checked.** The command of the third step was run in the project's folder and made `build.exe` without a warning.
  `build.exe quick nowindow` then left `build\civeditor.exe` and ran 384 tests, of which only the test of the
  first line of `TERMINOLOGY.md` failed (Test 363 without the window's tests), as before.

#### Not done

- The whole `build.exe` of the fourth step, with the static analyzer and the window's tests, was not run: a build
  without `nowindow` opens the window for a few minutes.
- The failing test of `TERMINOLOGY.md` is left as it was.

### 04:26:03: changes kept at once, the look choices in a popup, a filter on every list, a build without questions

#### What changed

- **Why.** Fourteen changes to the window were asked for in one list. They are given here by what they changed.
- **The Appearance popup.** The six look choices (architecture, language, wonder, castle, monk skin and scout) are no
  longer in the first step. The button Appearance opens the Appearance popup, a small window owned by the main
  window (`src/popup/popup.c`), which holds them in one column above a button Close. They are still controls of the
  layout, placed `PLACE_POPUP` in `SPECS` of `layout.c`; `controls.c` makes them children of the popup, and the popup
  passes their messages to the main window, so each choice changes the design at once and is undone as before. The
  popup's class is registered in `app_window.c`, since the rule test allows a window procedure to be named only there.
- **Clear.** A button Clear under the civ bonuses unticks them all as one change (`COMMAND_CLEAR_BONUSES`,
  `civ_design_clear_bonuses`).
- **Ranked civs.** `data/appearance.txt` has a new kind of line, `unranked`, for the six civs of Chronicles: the
  Achaemenids, Athenians, Spartans, Macedonians, Thracians and Puru. Civ to edit leaves them out
  (`catalog_is_ranked`, used by `item_belongs` in `binding.c`). Their `base` lines stay, so their bonuses, units and
  techs are still offered and a filter still knows their names.
- **No Save civ.** The button Save civ is gone. After every change `sync_details` hands the design to `roster_keep`,
  which keeps it as the draft of its slot and marks the slot as a changed civ when the design differs from its base
  civ, and no longer when it is as the game has it again. The roster holds no second copy of a saved design any
  more. The label of civs changed counts the changed civs, Build mod builds their drafts, and the last step is done
  once the civ differs from its base civ and lacks nothing.
- **The asterisk.** A changed civ's row in Civ to edit ends in an asterisk, as "Britons*" (`details_civ_row`); the
  list is filled anew when a mark flips.
- **Build mod.** The question "Build the mod into this folder?" is gone: Build mod asks for the mod's name and
  writes. A build that left nothing out (`summary_build_is_whole`) shows no message; the status line names the mod
  and Finish shows the built mark, a green check mark with "Successfully built mod" (`CONTROL_BUILT`, a new kind of
  control, `WIDGET_MARK`, in the new color `COLOR_ROLE_SUCCESS`), until the next change. A build that left
  something out says what in a message, as before, and a build that failed says why.
- **Options.** Options... in the menu File shows the mods folder that was found and takes another, if it exists
  (`dialogs_options`). The folder is kept in `app->mods_folder` until civeditor closes.
- **Filters.** The team bonuses, the unique units, the Castle Age techs and the Imperial Age techs have a filter
  each, as the tech tree and the civ bonuses had; `filter` of a line of `BINDINGS` names each list's filter. A filter
  also takes a civ's name: a row stays when the filter's text starts the name of a civ that has the row's item, so
  "britons" leaves the Britons' civ bonuses, Longbowman, Yeomen and Warwolf (`civ_matches` in `binding.c`).
- **Finish.** The last step is named Finish in place of Review (`STEP_FINISH`).
- **Choose hero.** Its list scrolls a row at a time without sliding, as the other lists do: the code that does so
  moved from `assist.c` to `src/shell/list_scroll.c`, and the dialog of `text_dialog.c` now runs its own loop of
  messages that hands each message to it. The dialog has a filter above its list.
- **The smallest wide window.** With a filter above each of its lists UU&UT no longer fits 560 in height, so the wide
  layout now needs 960 by 640 (`LAYOUT_MIN_HEIGHT`); a shorter window shows the step in one column, as before.
- **Tests.** 465, up from 444, and 112 redundancy tests, up from 73. New: Tests 323 to 325 (a whole build, a build
  with a civ bonus skipped, the title Finish) and Tests 363 to 380 (Build without the question, Options twice, the
  popup three times, a civ outside ranked play, the asterisk twice, the count of Finish, the filters seven times and
  Clear). Changed: Tests 339 to 341, which no longer save the civ first, Test 341 now checking the message of a
  build that left something out. Redundancy: the tests of Save civ became tests of `roster_keep` and of Finish
  (Redundancy tests 14 to 21, 93, 94 and 96), four tests of slot 0 were added (25 to 28) and 35 tests of the
  filters, ranked civs, `tsv_list_holds`, a whole build and Clear (54 to 88), in the new
  `tests/redundancy/present/filter_tests.c`, since `tests/redundancy/` holds 7 files.
- **Checked.** `build.exe redundancy`, with the static analyzer and the window: no warning, no finding, 577 tests
  run, and only the test of the first line of `TERMINOLOGY.md` failed, as before. Fifteen of the new tests of the
  window were seen to fail with the code broken on purpose (the civ's name in a filter, the asterisk given and
  taken back, the ranked civs, the popup shown, Clear, Options' refusal, the filter of Choose hero, the team
  bonuses' filter, a question put back before the build); the breakage was undone each time.
- **Measured.** Requirement 10: no function at CRAP 10 or more; `is_whole`, `civ_has`, `layout_shows` and
  `run_dialog` were brought under it by tests or by splitting, and `tsv_list_holds` by a test. Requirement 11: the
  dryer finds no two functions alike. Requirement 12, without the window: of the functions this change added or
  changed, every mutant is killed but nine: two of the roster, one of `tsv_list_holds` and one of `civ_matches` that
  only read one place past the end of an array, one of `binding_filtered_list` that skips control 0, which is no
  list, and four of `item_matches` in the category of the bonuses, which only tests of the window reach.
- **Documents.** `TESTS.md`: the new entries, the numbers after them, and the lines of every test in the files that
  changed. `TERMINOLOGY.md`: changed civ, ranked civ, look choice, Appearance popup, filter, Clear, Options...,
  built mark added; Finish in place of review; Save civ removed; civs changed, roster and job changed.
  `MAINTAINER.md`: the table of files (`src/popup/`, `list_scroll.c`), the diagram, the sizes of the columns, the
  list scrolling, `appearance.txt`, and how a change is kept, the popup, the filters and Build mod work.
  `README.md`: the last step and the steps of Usage, which no longer save. `DECISIONS.md`.
- **Answers.** Of the questions of 20:19:34, in
  [five steps, and no field for the civ's description](DEVLOG7.md#201934-five-steps-and-no-field-for-the-civs-description):
  the wide layout needs more height (2), 640 in place of the 680 proposed; see question 2 below.
- **Replaced.** Save civ and the review's two buttons, and the mod written after asking, of
  [civs of the game edited and saved into one named mod](DEVLOG7.md#072448-civs-of-the-game-edited-and-saved-into-one-named-mod);
  the smallest wide window of 960 by 560.

#### Not done

- Nothing was tried by hand, and no mod was built with the real game. The game of the tests builds no civ whole, so
  no test shows the built mark in the window or a whole build without a message; Tests 323 and 324 check only the
  decision between the two.
- The scrolling of the list of Choose hero has no test of its own: it now runs the code the other lists run.
- The Appearance popup has one size, 320 by 544 units: it does not turn into one column or scroll in a small window
  or at a large text size, draws no focus frame of its own, and the accessibility tests, which go step by step, do
  not look at its controls. In `.github/ACCESSIBILITY.md` only the height 560 became 640; its boxes were not
  checked again.
- The mods folder given in Options... is forgotten when civeditor closes.
- Clear with no civ bonus ticked says "That change cannot be made: ... or nothing is ticked." in the status line.
- The built mark is on Finish only; after Build mod from another step the status line alone names the mod.
- A civ file that replaces a civ that is not ranked opens, but Civ to edit then shows no row picked.
- Requirement 12: the mutator ran without the window only, as agreed. So measured, 25 of 36 mutants of
  `src/roster` were killed before the new tests, 215 of 265 of `src/model`, 55 of 205 of `src/present`, 279 of 452
  of `src/data`, 147 of 198 of `src/core` and 195 of 345 of `src/view`; most survivors are in code this change did
  not touch and that tests of the window reach. They were left. `src/win`, `src/shell`, `src/popup` and
  `src/reflow` were not measured.
- The count of core tests breaks Rules 31 and 34 further: 21 tests were added to it.
- Redundancy tests 92 and 95 still say "the review" in their names.
- The failing test of the first line of `TERMINOLOGY.md` is left as it was.

#### Questions

1. **Which civs are not ranked.** The six civs of Chronicles were taken: slots 46, 47, 48, 54, 55 and 56. Is that
   the list meant? And should a civ file that replaces one of them still open? Proposal: keep both as they are.
2. **960 by 640.** The wide layout needs 640 in height in place of 560, so that UU&UT holds its three filters.
   Proposal: keep 640, which also gives the lists of UU&UT more rows.
3. **The mods folder of Options.** Should it be remembered between runs, and where? Proposal: yes, in a small text
   file beside the civ files in the user's documents; nothing is written there today without a question.
4. **A build that left something out.** Bonuses that are "not built yet" make a build less than whole, so such a
   build still shows the message. Is that what "if something did fail" means, or only a build that wrote no mod?
   Proposal: keep the message whenever something is left out, since the user would otherwise not learn of it.
5. **The Appearance popup and accessibility.** Should the accessibility tests treat the popup as a step of its own,
   and should it scroll in a small window? Proposal: yes to both, as its own change.
6. **A civ's name in a filter.** The name must start with the filter's text ("brit" finds the Britons, "ritons"
   does not), while the rows' own text is searched anywhere. The filter of Choose hero searches the heroes' names
   only. Proposal: keep both.
7. **Rules 31 and 34.** Each of the fourteen changes got tests in the core category, as the hero did. Should they
   move to the category Regression, or the limit be lifted? Proposal: lift the limit for tests of requirements.

### 15:13:46: a dropdown opens and scrolls at once, as the lists do

#### What changed

- **Why.** The dropdowns of the first step, Civ to edit and the look choices of the Appearance popup, slid open and
  scrolled as Windows does, while the lists move at once, a row at a time. It was reported as a bug.
- **Opening.** Windows slides a dropdown's rows open only when they are not shown yet. `sync_on_notify` in `sync.c`
  now hands a dropdown that is about to open (`CBN_DROPDOWN`) to the new `list_scroll_open` in
  `src/shell/list_scroll.c`, which shows its rows first; Windows then places them and does not slide. It holds for
  every dropdown of the window, the bonus categories among them.
- **The wheel.** `list_scroll_takes` now also takes a turn of the wheel over an open dropdown, or over its rows
  (`wheeled_list`), and moves the rows one row a notch with `move_list`, as it moves a list. A closed dropdown is
  left to Windows.
- **Tests.** 467, up from 465: Tests 359 and 360, in the new `tests/shell/dropdown_tests.c`, check that Civ to edit
  opens in under 150 ms and that a notch of the wheel moves its open rows one row. Tests 359 to 465 became 361 to
  467. A third test, of the time a wheel turn over an open dropdown takes, was written and taken out again: it
  passed before the change too.
- **Checked.** Both tests failed before the change and pass with it. With the window on the screen, the opened rows
  were measured once: right under Civ to edit, as wide as it, shown and drawn. `build.exe redundancy`, with the
  static analyzer and the window: no warning, no finding, 579 tests run, and only the test of the first line of
  `TERMINOLOGY.md` failed, as before.
- **Measured.** Requirement 10: no function at CRAP 10 or more; `sync_on_notify` is at 9.2. Requirement 11: the
  dryer finds no two functions alike.
- **Documents.** `BUGS.md`: the bug, as reported. `TESTS.md`: Tests 359 and 360 and the numbers after them.
  `TERMINOLOGY.md`: dropdown. `MAINTAINER.md`: the list scrolling and the table of test files. `DECISIONS.md`.
  This part of the log, part 8, was started: part 7 would have passed 1000 lines, so the day 2026-11-21 moved here
  whole, and its links in `DECISIONS.md` and to part 7 were mended.

#### Not done

- Nothing was tried by hand: whether the dropdown now looks instant to the eye is not known, only its time.
- The arrows of an open dropdown's scroll bar, and a drag of its knob, are left to Windows. An open dropdown holds
  the mouse, so its presses do not come as a list's do; it was not looked into further.
- Keys in an open dropdown are left to Windows, as keys in a list are.
- Requirement 12: the mutator was not run. What changed is in `src/shell` and `src/win`, which only tests of the
  window reach, and the mutator runs without the window only, as agreed before.
- `BUGS.md` says of its first bug "Covered by test 349"; that test is Test 352 now. The line was left, since
  `BUGS.md` may only be edited to document a bug that was reported.
- The count of core tests breaks Rules 31 and 34 by two more.

#### Questions

1. **The scroll bar of an open dropdown.** Should its arrows also move the rows one row at once, as a list's do?
   Proposal: only if it is seen to slide when tried by hand.
2. **`BUGS.md`, "Covered by test 349".** May the number be corrected to 352 whenever tests are renumbered?
   Proposal: yes.
