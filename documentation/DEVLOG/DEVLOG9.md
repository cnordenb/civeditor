# Development log, part 9

- [2026-11-23](#2026-11-23)

Part 9 of the development log. It follows [part 8](DEVLOG8.md) and goes on in [part 10](DEVLOG10.md). The decisions
made in the log are listed in [DECISIONS.md](../DECISIONS.md).

## 2026-11-23

### 03:53:22: the numbers of Chosen bonuses typed or slid, in place of Stronger and Weaker

#### What changed

- **Why.** Stronger and Weaker did not say what they changed, nor by how much. A bonus's number is now set where it is
  read: a click on a number of Chosen bonuses, and the value wanted typed, or slid between its least and its most.
- **Answers.** Of the questions of 02:10:47, in [ticking a node ticks its needs, in the first regression
  test](DEVLOG8.md#021047-ticking-a-node-ticks-its-needs-in-the-first-regression-test): a value between two counts can be set,
  typed for the number itself or slid, and Stronger and Weaker are gone (1). Questions 2 to 4 stay open; the second
  is asked again below, without Stronger and Weaker. So question 6 of
  [23:08:30](DEVLOG8.md#230830-the-core-tests-within-rule-31-and-a-list-joined-by-commas-scaled-as-one) is answered for its
  question 1.
- **The multiplier.** `Multiplier` in `civ_design.h` holds a bonus's count in millionths, from 1 to 10, and may lie
  between, as 1.5; `civ_multiplier_of` makes one from a number. The civ file writes it as a decimal, as
  `"multiplier": 1.5` (`json_write_decimal`, by `text_append_decimal`), and reads it with `json_decimal`, by
  `text_to_decimal`, which moved from `tsv.c` to `text.c` so that `tsv_decimal` reads with it too. The build scales a
  change by a count that is not whole as by a whole one: a factor is raised to it (`bonus_raised`), an amount
  multiplied, and armor, attack and the units of a spawn are rounded to the nearest whole.
- **Numbers to set.** `bonus_text_show` in `bonus_text.c` writes a bonus's text as it counts, to one decimal, and
  records each number to set: an amount (`BONUS_NUMBER_AMOUNT`), a percent with a sign (`BONUS_NUMBER_PERCENT`) or a
  power, as "2x" (`BONUS_NUMBER_POWER`). A number that no count changes, as a cut of 100% or a power of 1, is none.
  `bonus_number_value` gives a number's value at a count, `bonus_number_times` the count that gives a value. A bonus
  with a number kept, or none, ends in "(counts 2 times)" as before, in Chosen bonuses also "(counts 1 time)", and then
  its count is its number to set; the game's texts show a bonus that counts once as it is (`bonus_text_scaled`).
- **Chosen bonuses.** The list, of the new kind `WIDGET_LINKS`, draws each ticked civ bonus as it counts, wrapped to
  its width, its numbers to set underlined (`chosen_window.c` in `src/chosen/`); its label reads "Chosen bonuses: 3 of
  24". A click on a number, or Enter on its row, moves the focus to the value field; the left and right arrows move
  the number in focus, which is framed, along its row. `src/numbers/` holds what this needs without Win32:
  `number_layout.c` cuts a row into words, spaces and numbers, wraps it and finds the number a click falls on, and
  `number_edit.c` keeps the number in focus on a number that is there and makes its value, its range and the commands.
- **The value field and the slider.** "Value of the number" (`CONTROL_BONUS_VALUE`) shows the number in focus. A value
  typed sets, with Enter or when the field is left, the multiplier that shows it (`number_typed_command`), if its
  tenths lie within those of the values at the multipliers 1 and 10; otherwise the status line reads, as for
  "-50% wood", "Type a value from -50% to -99.9%.". The slider (`CONTROL_BONUS_SLIDER`, a trackbar of comctl32, which
  the build now links with `-lcomctl32`) runs from 100 to 1000, the multiplier in hundredths, and its label names the
  range, as "Slide from -50% to -99.9%". Dragged, it shows in the field the value it would set; let go, it sets it
  (`number_slid_command`). It is shown only while the field or the slider has the focus (`chosen_follow_focus`, after
  each message and as the focus moves). Each change is a `COMMAND_SET_MULTIPLIER` through `sync_apply`, once `apply`
  in `sync.c`, so that undo and redo take it back and make it again.
- **Taken out.** Stronger and Weaker (`CONTROL_BONUS_STRONGER`, `CONTROL_BONUS_WEAKER`), `bonus_strength_command` of
  `binding.c`, and the text of Chosen bonuses that `details.c` made (`add_bonuses`), which no control shows now that
  the list draws itself.
- **A frame that could not be seen.** The frames of the number in focus and of the row in focus were drawn in
  `COLOR_ROLE_FOCUS`, the blue (24, 52, 96) of a chosen row, and the number in focus is always on the chosen row. On
  a chosen row both are now drawn in the chosen ink, as the other lists draw their focus: `controls_row_color` in
  `controls.c` holds that rule for every list, `controls_frame_rect`, once `frame_rect`, draws the frame, and
  `FOCUS_LINE` moved to `win_parts.h`. The link font is made with the other fonts (`chosen_restyle`) and kept in
  `AppState` as `link_font`.
- **One function each** (Req. 11). `check_is_index` in `check.c`, `text_is_sign` in `text.c` and `assist_scaled` in
  `assist.c` took the place of two copies each: `has_field` of `tsv.c` and a check of `number_edit.c`; `is_sign` of
  `bonus_text.c` and of `number_layout.c`; and `scaled` of `controls.c` and of `chosen_window.c`, with five lengths
  that `assist.c` scaled by hand.
- **Split** (Req. 10). `take_key` of `chosen_window.c` into it and `take_list_key`; `typed_value` of `number_edit.c`
  into it and `skipped`; `sync_on_notify` of `sync.c` into it and `on_notify`; and `text_append_decimal` into it and
  `put_fraction`.
- **Checks and bounds taken out** (Req. 12), as decided on 2026-11-16 for a check that no caller can fail and for a
  loop's second bound: the check of `number_span` in `number_layout.c`, whose two callers give it only a number of
  its row; the bounds `NUMBER_PIECE_CAPACITY` of the loops of `number_layout_place` and `number_layout_hit`, and
  `BONUS_NUMBER_CAPACITY` of the loop of `number_at`, which the counts they walk never pass; and the bound
  `NUMBER_TEXT_CAPACITY` of the loop of `skipped` in `number_edit.c`, which the end of the text typed comes to first.
- **The Tab test.** Test 33, that the Tab key reaches every control of every step, now puts the focus on each control
  it passes, as Tab does, since the slider is shown only once the value field has the focus.
- **The numbers of 23:08:30.** Its questions 1, 2 and 4 name redundancy tests by numbers one below those that its
  **Redundancy tests.** point gives them. They are now: the checks of P10, 501 to 505 there, 568 to 572; the gate
  tests, 506 to 512, 573 to 579; and the tests of the coverage build and of the arguments of the build, 487 to 500,
  554 to 567.
- **Names in `TESTS.md`.** The entries of Test 47 and of six tests of Choose hero... carried names that the tests no
  longer have, "the civ to edit" and "Choose hero..."; they now carry the tests' names.
- **Tests.** 677: 22 core tests, 46 accessibility tests, 30 regression tests and 579 redundancy tests. Test 11 now
  counts Chosen bonuses and the slider among the inputs that need a label. Regression test 19 now slides "Town
  Centers cost -50% wood" to 200 and finds "-75% wood". New: Redundancy tests 93 to 107, counts that are not whole,
  saved and built, and the decimals and multipliers that hold them; 108 to 118, the numbers to set and the values
  typed for them; 124 to 159, the number in focus, its value, its range, the slider, and a row cut, wrapped and
  clicked, also after a longer row; and 184 to 192, Chosen bonuses, the value field and the slider in the window. Of
  these, 100 to 107, 115 to 118 and 136 to 159 were written for mutants that had survived (Req. 12). Taken out: the
  redundancy tests of Stronger, of Weaker, of Stronger past the list and at 10, and of the text of Chosen bonuses,
  five. Redundancy test 119 was 93, 120 to 123 were 96 to 99, 160 to 183 were 100 to 123, 193 to 457 were 124 to
  388, and 458 to 579 were 392 to 513.
- **Checked.** The seven tests of counts that are not whole failed before the civ file and the build took such a
  count. The test that the number in focus is framed in the ink of its chosen row, Redundancy test 192, failed while
  the frame was drawn in the blue of the row, and passed once it was drawn in the chosen ink. With six faults put into
  `chosen_window.c` at once (a typed value not applied, another text for the range, the right arrow kept in place,
  Enter and a click leaving the focus on the list, the slider showing the value it had, and the slider never shown),
  ten tests failed: Redundancy tests 184 to 191, Regression test 19 and Test 33; the file was put back and they
  passed. Test 33 failed at "Slide" until it put the focus on each control it passes. Test 11 failed with the label
  of the slider taken out of `layout.c`, and again with that of Chosen bonuses; the labels were put back. Each test
  written for a mutant failed on that mutant: run again, the mutator found it killed. With the window and the static
  analyzer, `build.exe redundancy` gave no warning and no finding, and of its 677 tests none failed.
- **Measured.** Req. 10: the highest CRAP is 9.2, of `skip_graphics` in `dat_skip.c`. Req. 11: the dryer finds no two
  functions 80% alike or more. Req. 12: the mutator ran without the window over every file this change touched but those
  of `src/win/` and `src/chosen/`, in two runs, the first stopped after `src/bonus/`: 1538 mutants killed, 638 survived
  and 95 not covered. 65 of those that survived or were not covered were in code this change wrote. Seven went with the
  checks and bounds above, and Redundancy tests 100 to 107, 115 to 118 and 136 to 159 were written for 50, which the
  mutator, run again over their files, found killed. The other eight no test can kill. Two are in `fit_in` of
  `number_edit.c` (line 45, `<` made `<=` and 0 made 1), which only a count of 0 would tell apart, and
  `number_focus_fit` gives it none; one in `tenths_of` (line 117, `+` made `-`), which then gives one tenth less for
  every value, on both sides of each comparison; one in `number_layout_place` of `number_layout.c` (line 98, 0 made 1),
  which only a word that starts one pixel into its line would tell apart, and no font draws a piece one pixel wide;
  three in `scale_packed_multiply` of `bonus_scale.c` (line 84, 1 made 0, `<` made `<=` and `>` made `>=`): a packed
  value is at most 255, so the factor is at most 2.55 once `made` reaches the count, and neither ends the loop alone,
  but the count is kept, since the factor is divided in floating point; and one in `json_write_decimal` of
  `json_writer.c` (line 133, `false` made `true`), since the digits of a number read the same escaped or not. The other
  survivors are on lines this change left as they were, among them those of `recipe.c` and `recipe_copy.c`, whose
  functions only had `int multiplier` changed for `double times`. `src/present/`, `src/reflow/` and `src/view/` ran
  without the window.
  `src/chosen/chosen_window.c` ran with the window and one worker, since with one for each core a test of the window
  failed before the first mutant: in 85 minutes, 59 mutants killed, 74 survived and 4 not covered.
- **Documents.** `TESTS.md`: made again from the run of the 677 tests, by their names: the new entries, Regression
  test 19, Test 11, whose lines were given as 29 to 37 and are 39 to 49, the five taken out, the numbers and lines of
  the others, and the names of seven. `MAINTAINER.md`: the table of files (`src/core/`, `src/bonus/`, `src/numbers/`
  and `src/chosen/`, new, and `tests/redundancy/`), the diagram, Chosen bonuses with the value field and the slider
  in place of Stronger and Weaker and the one layout its rows share, the mutator run with one worker over the folders
  of the window, `src/chosen/` among them, the text of a bonus, and the multiplier.
  `TERMINOLOGY.md`: multiplier; number to set, Chosen bonuses, number in focus, value field, range and slider, new;
  Stronger and Weaker, taken out. `DECISIONS.md`: the decisions of this section and the two it replaces.
  `DELETION_REQUESTS.md`, new: the folders that two runs of the mutator stopped by hand left.
- **Replaced.** A bonus edited by Stronger and Weaker, a value between two counts not to be set, of
  [21:19:41](DEVLOG8.md#211941-unticking-a-node-unticks-its-descendants-and-stronger-and-weaker-edit-a-bonus); and a multiplier
  a whole number from 1 to 10, of [2026-11-17
  04:37:52](DEVLOG6.md#043752-a-bonuss-multiplier-applied-to-the-bonuses-built-from-changes-and-to-the-team-bonus).

#### Not done

- The mutator was not run over `src/win/`, and `src/present/`, `src/reflow/` and `src/view/` ran without the window,
  so a survivor there may be one that a test of the window kills. With the window, each folder takes
  `..\mutator\mutator.cmd --use-existing-coverage --max-workers 1 --timeout-factor 40 --test-command ".\build.exe
  quick norules redundancy" src/<folder>`, which leaves the computer unusable while it runs.
- The 74 survivors and 4 mutants not covered of `chosen_window.c` were neither read nor killed. Among them are the
  second bounds of the loops at its lines 63, 88, 142 and 207, which the decision of 2026-11-16 takes out.
- The survivors on lines this change left as they were were counted, not read.
- The program was not tried by hand; only the tests ran.
- Two runs of the mutator stopped by hand left the folders of their workers, `target/mutation-workers/run-cc196952/`
  and `target/mutation-workers/run-5ca15ed4/`; `DELETION_REQUESTS.md` lists them for a human to delete.

#### Questions

1. **Where the value is typed.** The value field and the slider are under Chosen bonuses, and a click on a number
   moves the focus there, rather than opening a field over the number itself. One field with a label is reached by
   Tab, read by a screen reader and seen at every size of text, where a field over the number would cover the text
   around it and move as the row wraps. Should the value be typed over the number? Proposal: no, the field stays
   under the list.
2. **A count below 1.** A value is taken between the values at the multipliers 1 and 10, so "-50% wood" can be made
   -75%, but not -25%: that would be the bonus counted 0.415 times. Should a count below 1 be possible? Proposal: no;
   a bonus weaker than its own text is another bonus.
3. **The team bonus.** The team bonus has no field to set its count, and always counts once, though the design keeps
   a count for it and the build scales its changes by it. Should it have one? Proposal: the ticked team bonus shown
   with its numbers to set, as a row of Chosen bonuses, under the list of team bonuses.
4. **Numbers without a sign.** "Shepherds work 25% faster" keeps its 25%, as question 3 of 02:10:47 asks, so only its
   count can be set. That question stays open; its answer would make such numbers numbers to set as well.
5. **The questions still open.** Questions 1 to 5 of
   [23:08:30](DEVLOG8.md#230830-the-core-tests-within-rule-31-and-a-list-joined-by-commas-scaled-as-one), with the numbers that
   **The numbers of 23:08:30.** gives, and questions 3 and 4 of 02:10:47 wait for their answers; question 2 of
   02:10:47, Stronger and Weaker for the team bonus, is question 3 here.

### 14:02:39: the numbers of Chosen bonuses typed in their own fields, the slider just below

#### What changed

- **Why.** The numbers of a chosen bonus are to be edited where they stand in its text, each in a field of its own,
  as "Shepherds work [25]% faster", with the slider just below the field being edited. One value field and one
  slider under Chosen bonuses for all the numbers were not wanted, nor a count added to a text, as "Shepherds work
  25% faster (counts [1] times)".
- **Answers.** Of the questions of 03:53:22, in [the numbers of Chosen bonuses typed or slid, in place of Stronger
  and Weaker](#035322-the-numbers-of-chosen-bonuses-typed-or-slid-in-place-of-stronger-and-weaker): the value is typed
  over the number itself, in its text (1), and a percent without a sign is a number to set (4). So question 3 of
  02:10:47, in [ticking a node ticks its needs, in the first regression
  test](DEVLOG8.md#021047-ticking-a-node-ticks-its-needs-in-the-first-regression-test), is answered for such a percent: it is
  scaled by the word after it, as that question proposed, but no text keeps "(counts 2 times)". Questions 2 and 3 of
  03:53:22 stay open and are asked again below.
- **Chosen bonuses.** It is a panel of its own class, `CHOSEN_CLASS_NAME`, registered in `app_window.c`, whose window
  procedure is `chosen_procedure` in `chosen_window.c`. Each ticked civ bonus is a label in it (an owner-drawn
  `STATIC`), its text as it counts, and each number to set is a field (`EDIT`) in the label, in the place of the
  number, holding its sign and digits: "Town Centers cost [-50]% wood". A value typed sets, with Enter or when the
  field is left, the multiplier that shows it (`chosen_commit` in the new `chosen_edit.c`, by
  `number_typed_command`); a value past the range makes the status line name the range, as before, and the field
  shows the number again. Tab goes through the fields row by row, from the left, and the label around a field draws
  its focus frame (`assist_draw_ring` in `assist.c`), since a field is not a control of the main window.
- **The card.** While a field has the focus, the card is shown just below it (`chosen_place_card`), centered under
  the field and kept inside the panel (`number_card_place` in `number_layout.c`): the slider, from 1 to 10 in
  hundredths, with the least value of the number left of it and the most right of it. Tab goes from the field to
  the slider. Dragged, the slider shows in the field the value it would set; let go, it sets it (`chosen_slide`). Esc
  in the field or the slider hides the card and leaves the focus on the field (`dismissed`), and the card is hidden
  when the focus leaves the field and its slider (`chosen_follow_focus`).
- **A field that a change takes away.** It was found that Clear, or an undo, left the focus on the hidden field of
  the bonus it took away, and that leaving that field made the status line read "There is no number to set.".
  Windows gives the focus of a hidden window to its parent, the panel, which no key moves on from. Now the focus goes
  to the list of civ bonuses when it is on the panel, the field or the slider and the field in focus is gone
  (`give_up_field` in `chosen_edit.c`), and `chosen_commit` sets nothing from a field that shows no number.
- **The windows of the panel.** They are made when first needed and kept, hidden while not needed, until the window
  closes: a label for each row, a field for each number and one card (`ChosenWindows` in `AppState.chosen`). None is
  destroyed, so a change never destroys the field whose message it is made in. `chosen_sync` gives them their texts
  after every change and `chosen_arrange` lays them out: `number_layout.c` cuts each row into words, spaces, numbers
  and units and wraps it, each number's piece as wide as its field, at least 24 by 24 (`chosen_lay_out` in the new
  `chosen_draw.c`, which also draws the labels and the card). The fields come first in the Z order, in reading order,
  then the card, after the field in focus, and the labels last. The panel scrolls itself, with room under its last
  row for the card.
- **Names for a screen reader.** The panel, the fields and the slider are not controls of the main window, so they
  are named through Windows' annotations (`assist_annotate` in `assist.c`): the panel by its label, as "Chosen
  bonuses: 2 of 24", each field by the text of its bonus, and the slider by its range, as "Slide from -50% to
  -99.9%", with the value it would set as its value. Each label keeps the text of its bonus.
- **No count in a text.** "(counts 2 times)" and "(counts 1 time)" are gone (`put_count` of `bonus_text.c`, taken
  out). A percent without a sign before " faster", " more" or " longer" is a gain, and before " less" or " cheaper" a
  cut (`change_sign`): "Cavalry 20% faster" counted twice is "Cavalry 44% faster", and "Siege Workshop units 15%
  cheaper" counted 1.5 times is "21.6% cheaper". It is a number to set, shown and typed without a sign. Any other
  number without a sign is kept as it is, and a bonus with no number to set has no field; see question 1.
- **One rounding.** A value typed is compared with its range in tenths rounded as the text of a bonus rounds them, by
  `bonus_tenths` in `bonus_text.c`, which took the place of `tenths_of` in `number_edit.c` and of the rounding in
  `put_number`.
- **One way for every count.** `bonus_text_scaled`, which gives the summary and the game's texts, no longer copies
  the text of a bonus counted once, but scales it as Chosen bonuses does at every count. Each of the 394 texts of
  `bonuses.txt` reads the same so, and a text of two decimals would read the same in all three places.
- **Taken out.** The value field (`CONTROL_BONUS_VALUE`) and the slider (`CONTROL_BONUS_SLIDER`, `WIDGET_SLIDER`) of
  `layout.c`, with their entries in `binding.c`; the underlined numbers, the link font (`link_font`,
  `chosen_restyle`) and the arrows that moved the number in focus along a row; `number_layout_hit`, which found the
  number a click fell on; `number_focus_fit` and `fit_in`; and the cases of WM_DRAWITEM and WM_HSCROLL in
  `app_window.c` and of `chosen_on_notify` in `sync.c`. `WIDGET_LINKS` is now `WIDGET_PANEL`, and `number_value_text`
  is `number_text`, which gives a number with its unit or without (`NUMBER_WITH_UNIT`, `NUMBER_DIGITS`).
  `number_card_place` takes only the width of the panel, which always starts at 0.
- **Shared.** `assist_bar_target` in `assist.c` moves a scroll bar for the main window and for the panel alike,
  `assist_draw_ring` draws the focus frame of both, and `open_service` opens Windows' accessibility service for
  `assist_annotate` and `assist_mark_live`.
- **Names used once.** The test of recursion joins the calls of all files by name, so `scroll_to` of
  `chosen_window.c` and that of `assist.c` read as one function that called itself. The one of `chosen_window.c` is
  now `scroll_panel_to`; `row_of` and `field_of` are `row_window` and `field_window`, and `control_of` of `assist.c`
  is `holding_control`.
- **Split for Req. 10.** `chosen_paint` into it and `chosen_print`, which also answers `WM_PRINTCLIENT`, so that a
  test can print the card; and `dismissed` into it and `is_editing`, which `give_up_field` uses as well.
- **The numbers of 23:08:30.** The redundancy tests that its questions 1, 2 and 4 name, which **The numbers of
  23:08:30.** of 03:53:22 gave anew, are now, after the numbers below: the checks of P10, 579 to 583; the gate tests,
  584 to 590; and the tests of the coverage build and of the arguments of the build, 565 to 578.
- **Tests.** 688, up from 677: 22 core tests, 46 accessibility tests and 30 regression tests, as before, and 590
  redundancy tests, up from 579. New, 38 redundancy tests: 95, 113, 119 and 344, a percent without a sign scaled or
  kept; 112, a bonus without numbers read as it is; 128, 132, 133, 135, 136, 139, 141, 149, 159 and 161 to 163, the
  numbers of a row, their fields and their pieces; 142 to 144, 148, 155 and 164, the card placed under its field;
  160, the range of an amount; and 189 and 191 to 203, Chosen bonuses in the window: its labels, its fields, the card
  and its slider, Esc, Tab, the frame and a field that a change takes away. Changed: Test 11 no longer counts a
  slider among the inputs of `layout.c`; Test 33 takes a control as reached when the focus is on a window inside it,
  as on a field of Chosen bonuses, and Test 35 takes the fields inside one control in the order they are read; Test
  45 checks the frame by `access_frame_shows`, now in `draw_tests.c`; Regression test 19 moves the slider of the
  field "-50"; Redundancy tests 90 to 92 read a text without a count, and 365, 367, 469 and 477, of the first tip and
  of the summary, the bonus "Infantry +1 attack" of their fixture in place of "Infantry is stronger", which no count
  scales; Redundancy test 119 reads "Loot 25% of the resource cost of each destroyed building" in place of "Fallen
  units return 15% of their cost", for a mutant of `change_sign`. Taken out, 27: those of "(counts 1 time)" and
  "(counts 2 times)", of the value field and the slider under the list, of the number in focus moved by an arrow, by
  Enter or by a click, of the click on a number, and of the frame in the ink of the row. Renumbered: Redundancy tests
  120 to 127 were 119 to 126, 129 to 131 were 128 to 130, 134 was 133, 137 and 138 were 136 and 137, 140 was 139, 145
  to 147 were 144 to 146, 150 to 154 were 149 to 153, 157 and 158 were 158 and 159, 165 to 188 were 160 to 183, 190
  was 185, 204 to 343 were 193 to 332, and 345 to 590 were 334 to 579.
- **Checked.** Of the tests written before their code, each failed first for the reason expected: the eight of
  texts without a count and of percents without a sign (Redundancy tests 90 to 92, 95, 112, 113, 119 and 344), before
  `bonus_text.c` scaled such a percent and wrote no count; the two of the ends of the card (200 and 201), since the
  card printed blank before the panel answered `WM_PRINTCLIENT`; and the two of a field that a change takes away (202
  and 203), before `give_up_field` and the check in `chosen_commit`. 202 failed again while `give_up_field` moved the
  focus only from the field and the slider, since Windows had given it to the panel as it hid the field. Redundancy
  tests 159 to 164, and 119 with its new text, were written for mutants that had survived; run again, the mutator
  found those mutants killed. The tests of Chosen bonuses in the window, 189 and 191 to 199, and the new tests of the
  fields, the pieces and the card among 128 to 155 were written with the code of the fields and not run against the
  code before it, which had no field for them to find; the mutator's run over `src/chosen/` is what shows the
  faults they catch. Before `bonus_text_scaled` lost its own way for a count of 1, a program written for the check,
  and not kept, read each of the 394 texts of `bonuses.txt` through `bonus_text_show` at a count of 1 and found
  every one the same as it is. After the last change of the code, `build.exe coverage redundancy`, with the window,
  ran the 688 tests, and none failed.
- **Measured.** Req. 10: the highest CRAP is 9.2, of `skip_graphics` in `dat_skip.c`; in `src/chosen/`, 8.6, of
  `chosen_procedure`. Req. 11: the dryer finds no two functions 80% alike or more. Req. 12: without the window, over
  `src/numbers/` and `bonus_text.c`, the mutator killed 179 mutants of 183, and none was left uncovered. Of the ten
  that had survived in `src/numbers/` at its run earlier in this change, and the one not covered, each is now killed
  or went with its code, but the two of `bounded` in `number_layout.c` (line 126, `>` made `>=`, and line 128, `<`
  made `<=`), which give the same value at the bound either way. Two survived in `bonus_text.c`. In `change_sign`
  (line 166, `&&` made `||`), no test had a percent before another word with no letter as many places after it as a
  change word is long; Redundancy test 119 now reads "Loot 25% of the resource cost of each destroyed building",
  with a space seven places on. In `bonus_text_scaled` (line 223, `<=` made `<`), a count of 1 copied the text, which
  every text of `bonuses.txt` reads the same scaled; the copy was taken out rather than a test written for a text no
  bonus has. Run again over `bonus_text.c`, the mutator killed all 80 of its mutants. The run over
  `src/chosen/` is in **Not done**.
- **Documents.** `MAINTAINER.md`: the table of files (`src/numbers/`, `src/chosen/` and `tests/redundancy/`),
  Chosen bonuses with its labels, fields and card, the focus given to the list when a change takes a field away,
  `chosen_print`, and the text of a bonus without a count. `TERMINOLOGY.md`: number to set, Chosen bonuses, field of
  a number, number in focus, card and slider; value field, taken out. `.github/ACCESSIBILITY.md`: 1.4.13, 2.1.1,
  2.4.3, 2.4.7, 2.5.7, 2.5.8, 3.3.1, 3.3.3 and 4.1.2, for the fields and the card. `TESTS.md`: made again from the
  run of the 688 tests, by their names: the 38 new entries and the 27 taken out, the numbers and lines of the others,
  Test 45, whose check of the frame moved to `draw_tests.c`, and the texts of Test 11, Regression test 19 and
  Redundancy tests 90 to 92, 365, 367, 469 and 477. `DECISIONS.md`: the decisions of this section and those it
  replaces.
- **Replaced.** Of [03:53:22](#035322-the-numbers-of-chosen-bonuses-typed-or-slid-in-place-of-stronger-and-weaker):
  the value field and the slider under Chosen bonuses, the numbers to set underlined and found by a click, a number
  to set that took in its "%" or "x", "(counts 1 time)" in Chosen bonuses, and the text of a bonus counted once
  copied as it is for the summary and the game's texts; and "(counts 2 times)" after a text with a number kept, of
  [21:19:41](DEVLOG8.md#211941-unticking-a-node-unticks-its-descendants-and-stronger-and-weaker-edit-a-bonus).

#### Not done

- The program was not tried by hand: only the tests ran, with the window made but not shown.
- The percents that the word after them scales were not checked against what the game makes of each text, as
  question 3 of 02:10:47 proposed.
- The tests written with the code of the fields, named in **Checked.**, were not seen failing before that code.
- The mutator did not run with the window over `src/win/`, whose functions that this change wrote or changed hold
  114 covered sites: 26 in `app_window.c`, 69 in `assist.c`, 14 in `controls.c` and 5 in `sync.c`. A run with the
  window, which leaves the computer unusable while it lasts, was asked for `src/chosen/` alone. `src/present/`,
  `src/reflow/` and `src/view/` had only tables changed, so a run would take none of their functions.
- The mutator's run over `src/chosen/`, with the window and one worker, was started at 14:02 and had not ended
  when this section was written; nor had `build.exe redundancy`, with the window and the analyzer, run after the
  last change, since the mutator's run holds the project.
- The workers of the two runs of the mutator stopped by hand, listed in `DELETION_REQUESTS.md`, are still in
  `target\mutation-workers\`.

#### Questions

1. **Numbers without a sign that are kept.** 19 of the 394 civ bonuses have a number without a sign that is kept as
   it is and no number to set, such as "Receive 500 gold upon reaching Imperial Age", "Ships regenerate 15 HP per
   minute" and "Gain 20 gold for each technology researched"; 146 more have no number at all, as "Monks double
   healing range". Chosen bonuses gives them no field, so they count once, though the build would scale their
   changes by a count. Such a number is not always the change: "Houses support 10 population" names the room of a
   house, not the five it adds, and "Castles cost 300 stone" a cost that a count should lower. Should they be numbers
   to set? Proposal: a column of `bonuses.txt` marks the numbers of a text that a count scales, and those become
   fields; the others stay as they are.
2. **Numbers kept beside numbers to set.** In "Start with +3 villagers, -50 wood, -200 food; Town Centers support 15
   population and +7 LOS", counted twice, the four numbers with a sign are scaled and "15 population" is kept, while
   the build scales every change of the bonus. Should the text say what the build makes of it? Proposal: the column
   of question 1 marks the 15 as well.
3. **A count read for a bonus with no number to set.** A civ file can give such a bonus a count, saved before this
   change, and the build scales its changes by it, though Chosen bonuses shows no count and no way to change it.
   Proposal: a civ file read gives a bonus with no number to set the count 1.
4. **The team bonus.** It still has no field, as question 3 of 03:53:22 asks. Proposal: the ticked team bonus shown
   as a label with its fields under the list of team bonuses, as a row of Chosen bonuses.
5. **A count below 1.** As question 2 of 03:53:22 asks: "-50% wood" can be made -75%, but not -25%. Proposal: no
   count below 1.
6. **Fields of 24 by 24.** The fields of numbers are at least 24 by 24, as WCAG 2.2 SC 2.5.8 asks for Requirement 3,
   where every control of `layout.c` is at least 44 by 44, as Tests 12 and 21 check: a field 44 high would part the
   lines of a bonus's text, and the criterion leaves a target in a line of text out. Should they be 44 by 44?
   Proposal: no.
7. **The questions still open.** Questions 1 to 5 of
   [23:08:30](DEVLOG8.md#230830-the-core-tests-within-rule-31-and-a-list-joined-by-commas-scaled-as-one), with the numbers that
   **The numbers of 23:08:30.** above gives, and question 4 of 02:10:47, whether ticking in the tech tree and the
   count of a bonus become requirements of their own, wait for their answers.

### 16:03:28: the fields of Chosen bonuses wide enough for their widest value, and their drawing tested

#### What changed

- **Why.** The mutator's run over `src/chosen/`, started at 14:02, measured `chosen_draw.c` alone: of its 86 mutants,
  56 were killed, 26 survived and 4 were not covered. For `chosen_edit.c` and `chosen_window.c` the baseline, the
  tests run in the worker's copy of the project before the first mutant, failed, for a reason the report does not
  name, so none of their mutants ran. The run was stopped at 14:41, when the computer ran short of memory.
- **The width of a field.** Redundancy test 205 found a field too narrow for its widest value: `field_width` counted
  1 pixel a side for the border of the `EDIT`, but not the pixel that an `EDIT` keeps free inside its border, so
  "+5.5", typed for the "+1" of the foot archers, needed 33 pixels and had 31. A field now counts 2 pixels a side
  (`CHOSEN_FIELD_INSET` in `chosen_parts.h`), in its width and in its height (`chosen_lines`); with the font of 11
  points its height is still the least, 24 pixels.
- **Taken out.** In `piece_width`, the check of `text_copy_span`, whose `return 0` no test reached: a
  `_Static_assert` states that a piece of a shown text fits in its copy, and the result is cast to `(void)`. In
  `chosen_lay_out`, the second bound of the loop, `NUMBER_PIECE_CAPACITY`, on which two mutants survived: the loop is
  bounded by the row's `count`, which `number_layout_cut` keeps within that capacity.
- **Tests.** 693, up from 688. Redundancy tests 204 to 208 are new: the field of a number stands just after the text
  before it; it holds a number wider than at either end of its range; the frame of the number in focus is drawn
  whole within its label; the frame of a number on a later line of its label is drawn around it; and the field is
  just as tall as its text needs, and at least 24 pixels. Redundancy test 199 finds the frame by the same helper as
  206 and 207. Redundancy tests 209 to 595 were 204 to 590.
- **Checked.** Redundancy test 205 failed on the code before `CHOSEN_FIELD_INSET` and passed after it. It was first
  written with "Shepherds work 25% faster", where it could not fail: a percent compounds, so the values of that 25 at
  1 and 10, 25 and 831.3, are as wide as any typed between them, while "+5.5" is wider than "+1" and "+10". Each
  survivor that a new test kills was written into `chosen_draw.c` by hand and the tests run with the window, one build
  at a time: those of lines 20, 49, 53 (three), 61 (two), 76 (`0` made `1`), 78 and 94 of the run's report made
  Redundancy tests 204 to 207 fail, and the two of line 18 made 208 fail, alone. Each time the file was put back and
  compared with its copy. So was line 98 with `==` made `!=`, which Redundancy test 199 still fails on. With the code
  as it is, the 693 tests of `build.exe coverage redundancy` passed, and `build.exe redundancy`, with the window
  and the analyzer, passed them all with no warning and no finding.
- **Measured.** Requirement 10: the highest CRAP score is 9.2, of `skip_graphics`; in `chosen_draw.c`, 8.3, of
  `chosen_draw_row`. Requirement 11: the dryer finds no two functions 80% alike. Requirement 12, for `chosen_draw.c`:
  of the 86 mutants, 68 are killed (56 by the run, 12 by the new tests, by hand), 3 went with the code taken out, 12
  survive and 3 are not covered.
- **Survivors left.** In `chosen_lines`, 5 in the larger of the text's height with its inset and the least height:
  `>=` is the same in a larger-of, and the other four differ only for a font taller than that of 11 points. In
  `chosen_text_width`, the start of `size` (2): `cy` is never read, and `cx` only when `GetTextExtentPoint32W` fails.
  In `field_width`, 4: `widest` started at 1, when every number is wider; the width of a value whose `number_text`
  fails, which a field's number does not; and `>=` in its two larger-of. In `chosen_lay_out`, `<=` on the row's
  `count`, which measures one piece more that nothing reads, and would pass the array only in a row of 192 pieces.
  Not covered: the `return 0` of `chosen_text_width` for a text that is not UTF-8 and the `return false` of
  `chosen_lay_out` when `number_row` gives no row, both on data checked where it enters; and `chosen_paint`, since
  the window of the tests is never shown, so it gets no `WM_PAINT`: the tests print it with `WM_PRINTCLIENT`.
- **Documents.** `TESTS.md`: Redundancy tests 204 to 208, the lines of 189 to 203, and the numbers of 209 to 595.
  `MAINTAINER.md`: the size of a field. `DECISIONS.md`: the decisions of this section.

#### Not done

- The mutator did not run again, so the mutants of `chosen_edit.c` and `chosen_window.c` were not run, and those of
  `chosen_draw.c` that the new tests kill were checked by hand only. After `build.exe coverage redundancy`, the run
  is `..\mutator\mutator.cmd --use-existing-coverage --mutate-all --max-workers 1 --timeout-factor 40 --test-command
  ".\build.exe quick norules redundancy" src/chosen`, with `--mutate-all` since the report of `chosen_window.c` in
  `.metrics\mutate\` is older than its code: about 310 mutants of some 25 seconds each, over two hours with the window.
- The mutator did not run with the window over `src/win/`, as at 14:02:39.
- The program was not tried by hand: only the tests ran, with the window made but not shown.
- The words of a label stand 1 pixel above the number in its field; see question 1.

#### Questions

1. **The words 1 pixel above the number.** `draw_piece` draws the words of a label 1 pixel below the top of their
   place (`CHOSEN_FIELD_EDGE`), but an `EDIT` draws its number 2 pixels below the top of the field
   (`CHOSEN_FIELD_INSET`), so in "Shepherds work [25]% faster" the 25 stands 1 pixel lower than the words. Mending it
   needs a failing test first, and none is allowed: no requirement says how a label looks, `BUGS.md` names no such
   bug, and no survivor of the mutator calls for one, so it would be a core test past the limit of Rule 31. It could
   plausibly come back with any change of the font or of the border or inset of a field, since the words and the
   number are drawn by different windows. Proposal: if it is a bug, it is listed in `BUGS.md` and a regression test
   asked for, which compares the top of the words printed in the label with the top of the number printed in the
   field; then `draw_piece` draws the words `CHOSEN_FIELD_INSET` below the top.
2. **The survivors left.** The 12 survivors and the 3 mutants not covered, in **Survivors left.**, stand as the
   decisions of 2026-11-18 07:24:48 say, but for `chosen_paint`: every user reaches it, so those decisions want a test
   of it. Such a test would show the window on the screen, as Regression test 27 does, and read the screen, where
   another window can cover the panel. Proposal: leave `chosen_paint` without a test, and the others as they are.
