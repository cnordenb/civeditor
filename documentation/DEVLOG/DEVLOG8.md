# Development log, part 8

- [2026-11-21](#2026-11-21)
- [2026-11-22](#2026-11-22)

Part 8 of the development log. It follows [part 7](DEVLOG7.md) and goes on in [part 9](DEVLOG9.md). The decisions
made in the log are listed in [DECISIONS.md](../DECISIONS.md).

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

### 21:19:41: unticking a node unticks its descendants, and Stronger and Weaker edit a bonus

#### What changed

- **Why.** Two changes were asked for: unticking a unit of the tech tree should untick the nodes that need it, as the
  Crossbowman and the Arbalester need the Archer; and a bonus should be editable, as "Town Centers cost -50%" made
  "-75%".
- **Needs.** `catalog_tree_needs` in `catalog_lookup.c` gives the two nodes that a node of the catalog needs, as a
  `TreeNeeds`: its parent, the node of the parent's id in the same building or else the building of that id, and for
  a unit or a tech the building it is made in. For a building, the building column of `tree.txt` is the column the
  screen shows it in, such as the Wonder under the House, so a building needs only its parent; the Watch Tower's
  parent, the Outpost, and the Farm's, the Mill, are found as buildings of that id.
- **Unticking.** `tree_command` in the new `src/present/tree_command.c` makes the command of a row of the tech tree.
  Unticking a ticked node unticks its descendants too, every node that needs it directly or through others; ticking
  a node ticks only it. `untick_with_needers` goes over the catalog until a pass marks no node more, so a node listed
  before the node it needs is unticked as well. A node of `ALWAYS_KEPT`, which every civ keeps, is unticked alone and
  is never unticked by another (`civ_design_always_keeps` in `civ_design.c`). `list_item_command` and
  `tick_command` in `binding.c` take the design for it, and the help text of the tech tree list says so.
- **One command for the whole tree.** `COMMAND_TOGGLE_TREE` became `COMMAND_SET_TREE`, which carries the whole tree
  (`command_set_tree` in `command.c`), so one undo brings back every node that was unticked together.
- **Stronger and Weaker.** Two buttons under Chosen bonuses, `CONTROL_BONUS_STRONGER` and `CONTROL_BONUS_WEAKER`,
  raise or lower by one the multiplier of the ticked civ bonus in focus in Bonuses, from 1 to 10
  (`bonus_strength_command` in `binding.c`, `COMMAND_SET_MULTIPLIER`, `civ_design_set_multiplier`). A bonus not
  ticked, or a multiplier that would leave 1 to 10, is refused, and the status line says so. Each button has a help
  text in `control_help.c`.
- **The text of a bonus.** `bonus_text_scaled` in the new `src/bonus/bonus_text.c` writes a bonus's description with
  its numbers as often as the bonus counts. An amount with a sign is multiplied, "+1" counted twice being "+2"; a
  percent with a sign is a factor raised to the multiplier, "-50%" being "-75%" and "+5%" "+10.3%"; each number of a
  list such as "+20/30%" alike; and "2x" is raised, to "4x". A sign counts only at the start of a word, and only a
  slash joins numbers into a list. A number without a sign is left as it is, and a text with such a number, or with
  none, ends in "(counts 2 times)". Chosen bonuses (`details.c`), the summary of Finish (`summary_add_bonus` in
  `summary.c`) and the civ's texts in the game (`bonus_text` in `mod_text.c`) show it; the team bonus is shown as it
  is. `raised` of `bonus_scale.c` became `bonus_raised`, which both use.
- **The build.** A civ bonus that names techs of a civ now counts as often as its multiplier: `recipe_plan_tech` in
  `recipe_copy.c` keeps a multiplier for each tech it plans, and the changes of each copy are scaled by it
  (`recipe_copy_effect`); the techs planned for requirements and for the unique unit count once. The bonus's lines
  in `bonus_effects.txt` count as often as the bonus too. A tech that every civ of the game shares is only unlocked,
  not copied, so it counts once: such a bonus is counted in `multipliers_skipped`, and the report now says "A tech
  that every civ of the game shares counts only once in a bonus."
- **Made smaller for the mutator.** The loops over a bonus's text are bounded by the text itself and lost their fixed
  limits of 2048 steps, 9 digits, 8 parts of a list and 4 letters, as the decision on P10's bounds has it; letters
  after a number of a list are no longer read, since no text of `bonuses.txt` has them; a number right after a
  letter, as in "Siege Towers x2 HP", is left and counted like any number without a sign; "2x" is raised whatever
  follows it; the size of a percent's change is taken with `fabs` in place of a product with its sign, and an amount
  is bounded with `fmin`. `tree_command` and `catalog_tree_needs` no longer check the node, which `on_pick` in
  `sync.c` checks where the window gives it; the passes of `untick_with_needers` lost their bound, since each pass
  marks a node or is the last, and its marks are cleared with `memset`. The loops over the tree in `tree_command.c`
  and `node_in_building` stop at `tree_count` alone, which `catalog.c` keeps within `CATALOG_TREE_CAPACITY`, as in
  `base_civ.c`. `civ_design_always_keeps` finds a node in `ALWAYS_KEPT` with `check_is_among`.
- **Tests.** 486, up from 467, and 120 redundancy tests, up from 112.
  - New: Test 81, a bonus of the game's techs taken twice doubles the changes of its copy; Tests 151 to 156, the text
    of a bonus; Test 179, the first tip; Tests 278 to 283, unticking; Tests 284 to 287, Stronger, Weaker, Chosen
    bonuses and the summary; Test 401, Stronger in the window. Redundancy test 89, a list of signed percents, which
    `list_end` needed for its CRAP score; Redundancy tests 90 to 96, for mutants that survived: a sign inside a word,
    a number after a space, a tree listed out of order, Stronger past the end of the list and at 10, a bonus not
    ticked, and the report of a civ whose multipliers are carried out in full.
  - Changed: Test 80 counts a multiplier of a bonus that names a generic tech, no longer of any bonus of the game's
    techs; Test 107 (106) finds the lines of a bonus of techs counted as often as the bonus, no longer once;
    Tests 269 (261) and 274 (266) make the command of the whole tree; Test 302 (284) reads the new words of the report.
  - Renumbered: the tests after each new one, up to Test 486, and Redundancy tests 89 to 112 to 97 to 120. The plan
    and patch groups of `redundancy_suite.c` became one, `run_plan_patch_test`, so that the file stays under 400 lines.
  - Written and taken out again: a test that undo brings back a cascade, since it passed before the change.
- **Checked.** Each new test failed before the change, or with the code it checks broken on purpose, and passed after
  it; each breakage was undone and the file compared with its copy. Redundancy tests 90 to 96 were shown by the
  mutator, which kills with them the mutants they were written for. `build.exe nowindow redundancy`, with the static
  analyzer and without the window: no warning, no finding, 501 tests run, and only the test of the first line of
  `TERMINOLOGY.md` failed, as before.
- **Measured.** Requirement 10, without the window: no function this change added or changed is at CRAP 10 or more;
  the highest is `put_count` at 7.3. Thirty functions that only the tests of the window reach read 10 or more, 0 to
  17% covered, since the window did not run: 20 of `sync.c`, 5 of `details.c`, 4 of `binding.c` and
  `layout_text_dpi`. Requirement 11: the dryer finds no two functions alike. Requirement 12, without the window: the
  mutator ran over `src/present`, `src/bonus`, `src/data`, `src/model`, `src/forge`, `src/mod` and `src/view`, 2505
  mutants, of which 1611 were killed, 714 survived and 180 were not covered, most survivors in code this change did
  not touch; then three times more over the files changed for the survivors. Of the functions this change added,
  every mutant is killed but four: three make a loop over the tree read one node past its end, a node of zeros, and
  one of `list_end` changes a text only where a percent sign follows a slash, as "+20/%", which no bonus has. Of the
  lines it changed in other functions, every mutant is killed but seven: the multiplier 1 of the techs planned for
  the unique unit and for requirements made 0 (four), which copies them alike, since in the test game they only
  enable or upgrade units; and a field or value of `command_of`, `command_nothing` and `command_set_tree` made 1
  (three), which no command of those kinds reads. The other survivors of the functions it changed are in lines from
  before, four of them in `plan_unit_techs`, where the test game plans the elite tech also as a requirement of a
  bonus's tech.
- **Documents.** `TESTS.md`: the new and changed tests and the numbers after them. `MAINTAINER.md`: the table of files
  (`tree_command.c`, `bonus_text.c`, `catalog_lookup.c`), the diagram (`src/present` and `src/mod` use
  `src/bonus`), Stronger and Weaker and the text of a bonus, unticking with the descendants, the multiplier of a bonus
  of techs, and the tables of test files. `TERMINOLOGY.md`: need, descendant, Stronger, Weaker. `DECISIONS.md`: the
  decisions of this section, the two it replaces, and the numbers of Tests 349 and 352, now 367 and 370.
- **Replaced.** A civ bonus made of the game's techs counting once, of
  [a bonus's multiplier applied to the bonuses built from changes and to the team bonus](DEVLOG6.md#043752-a-bonuss-multiplier-applied-to-the-bonuses-built-from-changes-and-to-the-team-bonus);
  the lines of a bonus of techs counting once, of
  [bonus 356 built from the game's pasture techs and a line that disables the Farms](DEVLOG6.md#054110-bonus-356-built-from-the-games-pasture-techs-and-a-line-that-disables-the-farms).

#### Not done

- The window was not opened, since a run with it takes the computer for minutes and was not asked for: Test 401,
  Stronger in the window, and the other tests of the window did not run, and nothing was tried by hand.
- So the functions that only the tests of the window reach, such as `click_command` in `sync.c` with its 7 branches,
  were not truly measured by the crapper, and the mutator ran without the window and not on `src/win`. The mutants
  that survived, named under Measured, were left.
- No value between can be set: "-60%" is not "-50%" counted any whole number of times. The team bonus has no
  Stronger or Weaker, and its text is shown as it is. Ticking a node does not tick the nodes it needs.
- No requirement names either change: the tests cite Requirement 8 for the tech tree and Requirement 5 for the
  bonuses, which ask for the lists they change.
- `BUGS.md` says "Covered by test 349" and "Covered by test 359"; those tests are Tests 370 and 377 now. The lines were
  left, since `BUGS.md` may only be edited to document a bug that was reported.
- The count of core tests breaks Rules 31 and 34 by 19 more.
- The failing test of the first line of `TERMINOLOGY.md`, Test 465, is left as it was.

#### Questions

1. **Ticking a node.** Should ticking a node also tick the nodes it needs, as unticking now unticks those that need
   it? Proposal: yes, as its own change, so that a ticked node can always be made.
2. **Whole counts.** A bonus is edited by how many times it counts, so "-50%" can become "-75%" or "-87.5%" but not
   "-60%". Is that enough? Proposal: keep it; a value between would need a field and a new kind of line in
   `bonus_effects.txt`.
3. **The team bonus.** Should it get Stronger and Weaker too, and its text be scaled? Proposal: yes, as its own change.
4. **Numbers without a sign.** "Shepherds work 25% faster" is left as it is, with "(counts 2 times)" after it. Should
   such a percent be scaled as one with a sign? Proposal: yes, once the texts of `bonuses.txt` are checked one by one.
5. **Requirements.** Should `REQUIREMENTS.md` name the two changes, so that their tests cite them? Proposal: one line
   each, under Requirements 8 and 5.
6. **The window.** May the tests and the coverage of the window be run for this change? They take the computer for
   some minutes. Proposal: yes, once, before the commit.
7. **`BUGS.md`.** May its test numbers be corrected whenever the tests are renumbered, as asked at 15:13:46 (2)?
   Proposal: yes.
8. **Rules 31 and 34.** Nineteen more tests are in the core category. Should they move to the category Regression,
   or the limit be lifted? Proposal: lift the limit for tests of requirements, as asked before.

## 2026-11-22

### 02:10:47: ticking a node ticks its needs, in the first regression test

#### What changed

- **Answers.** Of the questions of 21:19:41, in [unticking a node unticks its descendants, and Stronger and Weaker
  edit a bonus](#211941-unticking-a-node-unticks-its-descendants-and-stronger-and-weaker-edit-a-bonus): ticking a
  node ticks the nodes it needs (1); the tests and the coverage of the window run once for this change (6); the test
  numbers of `BUGS.md` are corrected whenever the tests are renumbered (7), which also answers question 2 of
  15:13:46, in [a dropdown opens and scrolls at once, as the lists
  do](#151346-a-dropdown-opens-and-scrolls-at-once-as-the-lists-do).
  Questions 2 to 5 were asked to be explained, and are asked again below with what is needed to decide them.
  Question 8 was answered with a question: for which requirement more than one test is wanted, and why; the answer
  is question 5 below.
- **Ticking.** Ticking a node of the tech tree ticks every node it needs, directly or through others, as one change
  that one undo takes back: ticking the Arbalester ticks the Crossbowman, the Archer, the Archery Range and the
  Barracks. A node that every civ keeps is ticked too when another needs it, since that changes nothing in the build
  and shows what the civ has. `tree_command` in `tree_command.c` marks the node clicked, then, until a pass marks no
  node more, the needs of every marked node when ticking (`mark_needs`), or every node that needs a marked one when
  unticking (`mark_needers`), and last ticks or unticks every marked node (`set_marked`). When a node does not fit
  the design's tree, nothing changes.
- **Regression tests.** The fourth category of Rule 34 has its folder, `tests/regression/`, with its suite,
  `regression_suite.c`. `tests/runner.c` runs it on every run, after the maintainability tests, and numbers its
  tests on their own, as "Regression test 1", as it does the redundancy tests; the labels of the categories are one
  list, `LABELS`.
- **`BUGS.md`.** The wheel turn outside the lists is "Covered by test 370", and the dropdowns "Covered by test 377",
  the numbers those tests have had since 21:19:41.
- **Tests.** 486 tests, 1 regression test and 122 redundancy tests. Regression test 1 checks that, for the Britons,
  with the Archer unticked and so the Arbalester, ticking the Arbalester ticks the Archer again. Redundancy tests 97
  and 98 are new, in `edit_tests.c`, for mutants that survived: ticking a node of a tree listed out of order ticks
  all three of its needs, and ticking a node whose needs do not fit the design's tree changes nothing. Redundancy
  tests 97 to 120 are now 99 to 122.
- **The runner.** `tests/runner.c` ran at most 400 tests of a category, `CATEGORY_TEST_LIMIT`, and stopped there
  without a word. With the window the functional category has held 401 tests since 21:19:41, so Test 401, Stronger
  clicked in the window, never ran. The limit is 1000, and a category that reaches it fails the run, with a line that
  names `CATEGORY_TEST_LIMIT`.
- **Checked.** Regression test 1 failed before the change, the Archer left unticked, and passed after it. Redundancy
  tests 97 and 98 killed the mutants they were written for in the second run of the mutator. With the limit made
  300 for one run, the run named it and failed; it was made 1000 again. `build.exe redundancy`, with the static
  analyzer and the window, before the runner was mended: no warning, no finding, 608 tests run, Test 401 not among
  them, and only the test of the lines of `TERMINOLOGY.md` failed. After it was mended, `build.exe coverage
  redundancy`, with the window: 609 tests run, Test 401 passed, and only that test failed, Test 465; and
  `build.exe nowindow redundancy`, with the static analyzer: no warning, no finding, 504 tests run, and only that
  test failed.
- **Measured.** Requirement 10, with the window, measured for the first time since 21:19:41: no function is at CRAP
  10 or more. `click_command` in `sync.c` read 10.9 while Test 401 did not run, and reads 8.1 with it; the highest
  of `tree_command.c` is `mark_needers` at 6.0. Requirement 11: the dryer finds no two functions alike.
  Requirement 12, without the window: the mutator ran twice over `tree_command.c`, 30 mutants. The first run killed
  23, and 7 survived; with Redundancy tests 97 and 98 the second killed 27, and the 3 that survived make a loop over
  the tree read one node past its end, a node of zeros, which changes nothing.
- **Documents.** `TESTS.md`: Regression test 1, and its category in the table of contents; Redundancy tests 97 and
  98, the numbers of those after them, and the lines of Redundancy tests 92 to 96. `MAINTAINER.md`: ticking in the
  tech tree, the regression tests under Tests and their folder in its table, the redundancy tests of
  `edit_tests.c`, and what to do when a category reaches the runner's limit. `TERMINOLOGY.md`: need, now ticked with
  the node that needs it, and regression test. `DECISIONS.md`: the decisions of this section and those it replaces.
  `BUGS.md`: the two test numbers.
- **Corrected.** The section of 21:19:41 says under Not done that the tests of the bonuses cite Requirement 5; they
  cite Requirement 8, as those of the tech tree do, and Test 80 Requirement 9. Its question 2 says that a value
  between two counts would need a new kind of line in `bonus_effects.txt`; it would not, see question 1 below.
- **Replaced.** Ticking a node ticks only it, of 21:19:41; and the runner's 400 tests of a category, reached without
  a failure, of 2026-11-15 22:14:14 in [a mod built end to end by tests, 166 functions at CRAP 10 or
  above](DEVLOG4.md#221414-a-mod-built-end-to-end-by-tests-166-functions-at-crap-10-or-above).

#### Not done

- One text is scaled wrongly. "Military units cost -10, 15, 20, 25% food in Dark, Feudal, Castle, Imperial", counted
  twice, reads "-20, 15, 20, 25% food" and ends in "(counts 2 times)": its -10 is taken for an amount, since only a
  slash joins numbers into a list, and the percent sign comes after the last number. The game gives -19%. It was
  left for question 3, as a test of it would be one more test in the core category.
- The mutator did not run on `src/win`, since a run with the window opens it once for each core. The window was not
  tried by hand; only its tests drove it.
- `TERMINOLOGY.md`'s first line has 152 characters and its line of the changed civ 121, so the test of its lines
  fails as before; line 304 of `MAINTAINER.md` has 130.
- The core category holds 440 tests, where Rules 31 and 34 allow 49: 12 requirements, 35 rules and 2 bugs.

#### Questions

1. **A value between two counts.** A bonus counts a whole number of times, and each time its change is made again:
   "Town Centers cost -50% wood" leaves half the cost, counted twice a quarter (-75%), three times an eighth
   (-87.5%). So no count gives -60%, and none below once gives -25%. The build scales each change of a bonus by the
   count, a factor raised to it and an amount multiplied by it, so a count in tenths, or one worked out from a value
   typed, would build as well; a whole thing, as "+3 villagers", would be rounded. Should a value between be
   possible? Proposal: yes, typed in a field beside Stronger and Weaker as the first number of the text, the count
   worked out from it and the other numbers scaled by it.
2. **The team bonus.** Stronger and Weaker work on the civ bonus in focus in Chosen bonuses, and the team bonus is
   ticked in a list of its own, so it always counts once. The design keeps a count for it, and the build already
   scales its changes by that count, so what is missing is a way to set it and its text scaled: "Relics generate
   +33% gold" counted twice would read "+76.9% gold". Should it have them? Proposal: Stronger and Weaker under the
   list of team bonuses too, for the ticked one, and its text scaled as a civ bonus's.
3. **Numbers without a sign.** 59 of the 394 texts of `bonuses.txt` have a percent without a sign outside a list, 41
   of them "faster", as "Shepherds work 25% faster"; it is left, and the text ends in "(counts 2 times)". Each word
   would need its own reckoning: "25% faster" counted twice is 1.25 times 1.25, 56.3% faster, and "15% cheaper" 0.85
   times 0.85, 27.8% cheaper, but "Fallen units return 15% of their cost" is 30%, and "market trade cost only 5%"
   stays. Should they be scaled? Proposal: yes, by the word after the number, "faster", "more", "longer", "less" and
   "cheaper", each text checked against what the game makes of it; the others keep "(counts 2 times)". The list of
   Not done, joined by commas, would be mended with them.
4. **Requirements.** Rule 1 has `REQUIREMENTS.md` say what civeditor does, and Rule 6 has every test cite a
   requirement, a rule or a bug. No requirement names ticking and unticking in the tech tree or the count of a bonus,
   so their tests cite Requirement 8, that the mod replaces one civ of the game, as the nearest. Should the two be
   requirements of their own? Proposal: two lines, such as "13. Unticking a node of the tech tree unticks every node
   that needs it, and ticking one ticks every node it needs." and "14. A civ bonus can be made to count more or less
   than once.", written when the two have been tried.
5. **A second test of a requirement.** Asked: which requirement more than one test is wanted for, and why.
   Requirement 8: the 19 tests that 21:19:41 added to the core category all cite it, and Test 80, changed then,
   cites Requirement 9. Everything the civ is, its tree, bonuses, unique unit, techs and texts, falls under
   Requirement 8, and a test makes one check (Rule 24), so one test of it checks one of them. By Rule 34 a further
   test of a covered requirement is a regression test, written at the chief developer's direction. Proposal: three
   regression tests of Requirement 8 from 21:19:41: unticking the Archer unticks the Arbalester (Test 278), Stronger
   makes "Town Centers cost -50% wood" read "-75%" (Test 401), and a bonus taken twice counts twice in the game
   (Test 81). The other 16 would leave the core category: to the redundancy tests where a mutant needs them
   (Rule 32), and out where none does. The core category held 421 tests before 21:19:41, so bringing it to 49 needs
   a choice, for each requirement and rule, of the test that stays.

### 23:08:30: the core tests within Rule 31, and a list joined by commas scaled as one

#### What changed

- **Why.** The core category held 440 tests, where Rules 31 and 34 allow 49: 12 requirements, 35 rules and 2 bugs.
  Under Not done of 02:10:47, besides, one text was scaled wrongly, two lines of `TERMINOLOGY.md` and one of
  `MAINTAINER.md` were too long, and the mutator had not run on `src/win`.
- **Answers.** Of the questions of 02:10:47, in [ticking a node ticks its needs, in the first regression
  test](#021047-ticking-a-node-ticks-its-needs-in-the-first-regression-test): Tests 278, 401 and 81 become regression
  tests, and the other 16 tests that 21:19:41 added to the core category leave it (5). Questions 1 to 4 stay open, to
  be answered later, and are listed again below. A test that leaves the core category is a regression test when the
  log names a bug that breaks what it covers, and a redundancy test otherwise; a test past the limit with no such bug
  is added only when its reason, with how plausibly what it tests could fail, has been accepted. This answers in part
  question 4 of 2026-11-17 10:57:53, in [no test names Rule 4 alone: the 21 that did are
  removed](DEVLOG7.md#105753-no-test-names-rule-4-alone-the-21-that-did-are-removed): the core category keeps one test
  of each requirement, rule and bug, and the others leave it. Its part on `BUGS.md` stays open, as only a human adds a
  bug there (Rule 33). The line of clanker that question 3 of 10:57:53 proposed was written, since the long line was
  to be mended. The mutator is not run on `src/win` for now; its command is under Not done.
- **The core tests.** 22, of the 49 allowed. Of each requirement that a test can check, the test kept is the one
  that checks most nearly what its words say: Test 1 that the local mods folder is found (Req. 2), Test 2 undo
  (Req. 5), Test 3 the 24 places of the civ bonuses (Req. 6), Test 4 the names of the mod's two folders (Req. 7),
  Test 5 the replaced civ's techs (Req. 8), Test 6 the bonus that the build could not build (Req. 9) and Test 7 the
  name `civeditor_civ` (Req. 1). Tests 8 and 9 cover the two bugs of `BUGS.md`. Tests 10 to 55 are the accessibility
  tests, 46 of the 55 that Rule 32 allows, for Requirements 3 and 4; Requirements 10 to 12 are measured by the tools
  beside the project only. Tests 56 to 68 cover Rules 10 to 13, 26, 14, 15, 29 and 3, the log's form (Rules 18 to
  21), `TESTS.md` (Rules 5 and 6) and `TERMINOLOGY.md` (Rule 9). Of the checks of P10 (Rule 26), only that of goto
  stays a core test.
- **Regression tests.** 30, up from 1. Regression tests 2, 3 and 19 are Tests 278, 81 and 401, of Requirement 8
  (question 5). Regression tests 4 to 17 are Tests 5, 14, 15, 27, 39, 48, 50, 56, 57, 259, 309, 310, 317 and 318,
  and 20 to 30 are Tests 352, 355, 364 to 366, 371 to 375 and 378, those that open the window; each cites the bug of
  the log that it catches again. Regression test 13, once Test 259, loads the catalog twice, as the window does: the
  bug of 2026-11-17 06:49:31 read the look lines again at each load. Regression test 18 is new; see below.
- **Redundancy tests.** 513, up from 122. Redundancy tests 1 to 91 are as before, 92 is new (see below), and 93 to
  123 were 92 to 122. 124 to 444 are the other tests that were functional tests and open no window, and 445 to 487
  those that open it; 488 to 501 are the tests of the build program, 502 to 506 the checks of P10 against recursion,
  allocation, macros, pointers to pointers and pointers to functions, and 507 to 513 the gate tests.
- **One category a file** (Rule 34). `tests/requirements/` is new: `requirement_tests.c` and its suite,
  `requirement_suite.c`. `tests/regression/` holds the regression tests of the design and the civ file
  (`design_regression.c`), of the mod (`mod_regression.c`) and of the window (`window_regression.c`), beside
  `regression_suite.c`. `tests/redundancy/rules/` is new: the checks of P10 but goto (`p10_patterns.c`, from
  `code_patterns.c`), the check against recursion (`recursion_rule.c`, from `code_rules.c`), and `rule_redundancy.c`,
  which runs them with the tests of the build program and the gate tests. A helper that a test of another category
  needs stays in its file and is named in a header of its folder, its name starting with its file's subject, as
  `mod_tests_named_civ`, `places_write_under` and `scroll_shown_list`.
- **The runner.** `tests/runner.c` runs the core tests of the requirements and bugs, the accessibility tests and the
  core tests of the rules, numbered together as "Test 12"; then the regression tests; then, with `redundancy`,
  `redundancy_suite.c`, `functional_suite.c` with its suites, `shell_suite.c` and `rule_redundancy.c`, numbered
  together as "Redundancy test 3". With `norules`, `rule_redundancy.c` runs only the tests of the build program, so
  that the mutator still judges `tools/build/`. The test of the entries of `TESTS.md` counts the cases of
  `requirement_suite.c`, `accessibility_suite.c` and `maintainability_suite.c`.
- **A list joined by commas.** `list_end` in `bonus_text.c` takes numbers joined by a comma and a space for one list,
  as it took those joined by a slash, by `joint_width`. "Military units cost -10, 15, 20, 25% food in Dark, Feudal,
  Castle, Imperial" counted twice reads "-19, 27.8, 36, 43.8% food", each number a cut counted twice, as the game
  makes it. Only bonus 55 of `bonuses.txt` joins numbers by commas. Regression test 18 checks it, citing the bug of
  02:10:47. `list_end` reads the first number of a list in its loop, as it reads the others, and a comma joins only
  when a space follows it, which Redundancy test 92 checks: "+5,10 food" counted twice reads "+10,10 food (counts 2
  times)".
- **Long lines.** The first line of `TERMINOLOGY.md`, clanker, has 108 characters, and that of the changed civ 119;
  line 304 of `MAINTAINER.md` was wrapped.
- **Tests.** 611: 22 core tests, 46 accessibility tests, 30 regression tests and 513 redundancy tests. A run without
  `redundancy` runs 98 of them.
- **Checked.** Regression test 18 failed before the change, the text reading "-20, 15, 20, 25% food" and ending in
  "(counts 2 times)", and passed after it; texts with a slash, with "Elite-" and with a number without a sign read
  as before. With `mark_needers` in `tree_command.c` made to mark nothing, Regression tests 1 and 2 failed; with
  `catalog_load` no longer setting the count of look lines to 0, Regression test 13 failed. Both breaks were undone,
  and the files compared equal to the copies kept. The first run after the tests moved failed only the test of the
  entries of `TESTS.md`, that of the lines of `TERMINOLOGY.md` and Regression test 18, which the documents and the
  change then mended. Redundancy test 92 passed, and failed alone with the `&&` of `joint_width` made `||`; that
  break was undone. The 394 texts of `bonuses.txt` and 20 made for the check, each counted twice and three times,
  read the same before and after each change of `list_end`. `build.exe quick nowindow redundancy`: 506 tests run,
  none failed. `build.exe nowindow redundancy`, with the static analyzer: no warning, no finding, 506 tests run, none
  failed.
- **Measured.** Requirement 10, without the window, by `build.exe coverage redundancy nowindow norules`, 481 tests:
  no function is at CRAP 10 or more, the highest being `take_decimal_letter` of `tsv.c` at 9.5; in `bonus_text.c`
  the highest is `put_count` at 7.3, and `list_end` and `joint_width` are at 4.0. Requirement 11: the dryer finds no
  two functions alike. Requirement 12, on `bonus_text.c`: the first run made 69 mutants and killed 66. The three
  that survived changed no text of the game: the `&&` of `joint_width` made `||`, which only a comma that no space
  follows tells apart; and in `list_end`, a check that a joint was found, which the loop's own condition makes again,
  and the first number's end read backwards, which the loop then read again to the same end. The check was taken
  out, and the second run made 65 mutants, of which the other two survived; with the first number read in the loop
  and Redundancy test 92, the third run made 63 and killed them all.
- **Documents.** `TESTS.md`: made again from the tests, 611 entries under the four categories, with a paragraph on
  them; each regression test cites the bug it catches again, and the residences of four tests were mended;
  Redundancy test 91 no longer says that only a slash joins numbers. `MAINTAINER.md`: Tests, with the categories and
  their order, the table of the tests' folders, and "To add a test", which now starts by choosing the category; the
  label of a failed test in the first chapter; that a run which leaves tests out numbers the others otherwise than
  `TESTS.md`; the list joined by commas; and line 304. `TERMINOLOGY.md`: clanker, test and changed civ; bug, core
  test and category, new; and redundancy test and regression test, as the categories now are. `DECISIONS.md`: the
  decisions of this section and those it replaces, and four test numbers. `BUGS.md`: the two bugs are covered by
  Tests 8 and 9.
  `.claude/skills/tests-doc/SKILL.md`: the four categories, and the limits of Rules 31 and 32 where it said there was
  none.
- **Replaced.** No cap on the number of tests, of [2026-11-16
  21:03:39](DEVLOG5.md#210339-the-answers-recorded-no-cap-on-tests-p10s-bounds-and-check-count-given-up-aa-only-coverage-measured);
  a further test of what a test covers already a regression test only when ordered, of [2026-11-19
  21:28:52](DEVLOG7.md#212852-a-turn-of-the-wheel-that-cannot-scroll-the-step-does-nothing); the redundancy tests
  kept in `tests/redundancy/`, of [2026-11-17
  10:57:53](DEVLOG7.md#105753-no-test-names-rule-4-alone-the-21-that-did-are-removed); the runner numbering the
  functional, accessibility and maintainability tests in that order, of [2026-11-09
  00:44:18](DEVLOG1.md#004418-civbuilder-begun-in-c-with-its-own-build-and-rule-tests); new tests named in
  `tests/suites/`, of [2026-11-15
  22:14:14](DEVLOG4.md#221414-a-mod-built-end-to-end-by-tests-166-functions-at-crap-10-or-above); and only a slash
  joining numbers into a list, of
  [21:19:41](#211941-unticking-a-node-unticks-its-descendants-and-stronger-and-weaker-edit-a-bonus).

#### Not done

- No window was opened since the tests moved: Tests 7 to 9, Regression tests 19 to 30, and the accessibility and
  redundancy tests that open the window were compiled but did not run where they are now. The run that shows them is
  `.\build.exe quick redundancy`, a few minutes with the computer given over to it.
- The mutator did not run on `src/win`, and the window was not tried by hand. The run takes 3 to 5 hours with the
  computer given over to it: a coverage run with the window, as `AGENTS.md` gives it but without `nowindow`, then
  `..\mutator\mutator.cmd --use-existing-coverage --timeout-factor 40 --test-command ".\build.exe quick norules
  redundancy" src/win`.
- The tests that left the core category were not each shown to kill a mutant that no other test kills, which is what
  Rule 32 keeps the redundancy tests for; see question 3.
- The checks of P10 but goto, and the gate tests, run only with `redundancy`, so `build.exe` alone no longer checks
  for recursion, allocation, macros, pointers to pointers or pointers to functions; see questions 1 and 2.

#### Questions

1. **The checks of P10 but goto.** Rule 26 asks the code to keep P10, and Test 60 checks one of its rules, no goto.
   Redundancy tests 501 to 505 check five more: no recursion, no allocation, a preprocessor of includes and guards
   only, one level of dereference, and no pointer to a function but the window procedure. As redundancy tests they
   run only with `redundancy`, and no mutant needs them, as they read the code rather than run it. How plausibly
   they would fail: each is a common way of writing C that gcc accepts without a warning, such as `malloc` for a
   buffer, `qsort` with its function of comparison, a macro with arguments or a helper that calls itself, and code
   written by someone new to the project is likely to hold one. Should they be regression tests, run on every build?
   Proposal: yes.
2. **The gate tests.** Redundancy tests 506 to 512 show that the checks of comments, goto, allocation, macros,
   dereference, pointers to functions and recursion each name the line of `tests/fixtures/broken_rules.txt` that
   breaks them. Without them, a scanner of `tests/scan/` that found nothing would let its rule pass, and no mutant
   needs them, as the mutator passes by `tests/`. How plausibly they would fail: only when `tests/scan/` changes,
   which is seldom. Should they be kept, and as what? Proposal: those of comments and goto become regression tests,
   since they alone show that Tests 56 and 60 can fail; the other five go with the checks of question 1.
3. **Redundancy tests that no mutant needs.** Rule 32 keeps the redundancy tests for the mutants that survive the
   other tests. The 390 tests that left the core category were moved whole, without a run that shows which of them
   kills a mutant that no core, accessibility or regression test kills. Proposal: the mutator run over each folder of
   `src/` with the tests of the first three categories only, and each redundancy test that kills none of the mutants
   that survive them taken out; many hours of runs, those of `src/win` with the window.
4. **Requirements 10 to 12.** No core test covers them, as only the crapper, the dryer and the mutator beside the
   project measure them. Redundancy tests 487 to 500 test the coverage build and the arguments that the measures
   give the build. Should one of them be the core test of Requirement 10, and one that of Requirement 12? Proposal:
   no, since none of them measures the requirement; the **Measured.** point of each section records the measures.
5. **Regression tests and Rule 31.** Rule 31 limits every test but those of Rule 32 to the requirements, rules and
   bugs together, 49, and the core and regression tests are now 52: 22 and 30. Rule 34 adds the regression tests for
   what is ordered past the core tests, so they are read as not counted. Is that the reading? Proposal: yes; if they
   are counted, three regression tests become redundancy tests.
6. **The questions of 02:10:47.** Four questions of [ticking a node ticks its needs, in the first regression
   test](#021047-ticking-a-node-ticks-its-needs-in-the-first-regression-test) wait for their answers: whether a value
   between two counts can be set (1), whether the team bonus gets Stronger and Weaker (2), whether a percent without
   a sign is scaled by the word after it (3), and whether ticking in the tech tree and the count of a bonus become
   requirements of their own (4). Their facts and proposals are in that section.
