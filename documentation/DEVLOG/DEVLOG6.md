# Development log, part 6

- [2026-11-17](#2026-11-17)

Part 6 of the development log. It follows [part 5](DEVLOG5.md) and goes on in [part 7](DEVLOG7.md). The decisions
made in the log are listed in [DECISIONS.md](../DECISIONS.md).

## 2026-11-17

### 00:25:45: the window's scroll bar a control of its own, classic like the lists'

#### What changed

- **Why.** Bug 2 of 2026-11-16 asks for scroll bars of normal appearance. The window's own scroll bar, shown when the
  window is too small for the step, was still the thin bar of Windows 11, as the section of
  [23:56:00](DEVLOG5.md#235600-lists-scroll-a-row-a-notch-and-a-row-an-arrow-without-sliding-under-classic-scroll-bars)
  left it: with the visual styles turned off for the window, its title bar too looks like that of Windows 2000.
- **The scroll bar control.** The window no longer has a scroll bar in its frame (`WS_VSCROLL`). `assist_make_bar` makes
  a scroll bar control (class `SCROLLBAR`, `WINDOW_BAR_ID`) when the window is made, kept in `app->bar`.
  `assist_scroll_range` sets its range, puts it at the right edge of the window and shows it only when the step does not
  fit; `bar_target` reads where its knob is dragged. A scroll bar control draws the classic bar whatever the visual
  styles, so the window keeps them, and its title bar.
- **The room beside it.** `assist_view` gives the inside of the window less the scroll bar when the bar is shown, which
  `room_of` in `controls.c` lays out; Windows did that by itself for the bar of the frame. The checks of `room_of` and
  `assist_scroll_range`, which no caller can fail, were taken out with them. `assist_scroll_range` starts its
  `SCROLLINFO` empty (`ZeroMemory`) instead of setting its least value to 0.
- **Tests.** 377, up from 372. Test 286 checks that the top right corner of the window, at 480 by 360 on the Tech tree,
  shows the up arrow of a scroll bar (at least 4 colours where the thin bar has 2); Test 287 that Next ends as far from
  the scroll bar as Back starts from the left; Test 288 that the knob reaches the end of the bar when the step is
  scrolled to its end; Test 289 that a window exactly as high as the step Hero shows no scroll bar; Test 290 that the
  bar's up arrow moves the step up a line, 44 units. Tests 286 to 372 became 291 to 377. Test 285 reads its list through
  the same helper, `colours_in_corner`. The tests of the scroll bars and those of the lists' input are two groups of
  `shell_suite.c`, one subject each. The two tests of the one column that read how far the window is scrolled read the
  scroll bar control.
- **Checked.** Test 286 failed before the change, the window having no such control, and passes after it. The mutator,
  with one worker, on `assist_scroll_range`, `assist_view`, `assist_make_bar`, `bar_target`'s reading of the knob and
  `room_of`: of 8 mutants, 7 lived when only Test 286 was there. Tests 287 to 289 were written for them, and each mutant
  of `assist.c` was then made by hand and failed one of them: the bar moved past the right edge (Tests 286 and 287), the
  room not narrowed (287), the range one or two too long (288), the bar shown when the step just fits (289). The least
  value's `0`, which no test could tell from `1`, is gone with the `ZeroMemory`. The one in `room_of`, `<` made `<=`
  where a width under 320 is made 320, gives 320 either way: it cannot be killed. The mutator run again on the same
  lines: the 5 mutants of `assist.c` killed, the one of `room_of` alive. Coverage does not see the table `MOVES` of
  `bar_target`; its 4 mutants, made by hand, were killed but the up arrow's, which moved the step down a line, and Test
  290 was written for it; it fails on that mutant. A picture of the program in a window of 480 by 360, on the step
  Unique unit, shows the window's scroll bar and the list's with arrows and knob, under the title bar of Windows 11.
  `build.exe`, with the analyzer: 377 tests, every one passed, no warning.
- **Documents.** `TESTS.md`: Tests 285 to 290, and the numbers and lines of those after them. `MAINTAINER.md`: the
  window's scroll bar, under The narrow window. `TERMINOLOGY.md`: knob; classic scroll bar now names the window's.
  `DELETION_REQUESTS.md`: `build/heights_probe.c`, which printed each step's height in one column to choose the step of
  Test 289. `DECISIONS.md`: the new decisions, and Test 293, which was 288, in the decision on time.
- **Replaced.** That the window's own scroll bar is left as it was, of
  [23:56:00](DEVLOG5.md#235600-lists-scroll-a-row-a-notch-and-a-row-an-arrow-without-sliding-under-classic-scroll-bars).

#### Not done

- The window's scroll bar was not tried by hand with the mouse; the tests drive the window by messages. Its menu of the
  right button, Top and Bottom among them, is answered by nothing: `bar_target` leaves the window where it is on
  `SB_TOP` and `SB_BOTTOM`, as it did for the bar of the frame.
- Test 289 needs a screen high enough for the step Hero, 810 units and the frame: at 96 dots an inch, 900 pixels.
- The drop-down lists of the choices still slide and move three rows a notch.

### 02:39:14: every function of `src/` under CRAP 10, the checks no caller can fail taken out

#### What changed

- **Why.** Requirement 10 asks for a CRAP score under 10 for all code. The last measure, of 2026-11-16 23:04, put 174
  functions of `src/` at 10 or more. Most were scored down by `CHECK`s that no caller can fail: each adds to a
  function's complexity, and its branch that returns is never taken, which halves the coverage the crapper reads.
  Answer 2 of `instructions_3.md` gives up P10 where it threatens ub-mode, and checks no caller can fail are taken
  out, as decided in the section of 2026-11-16 21:03:39.
- **Checks of pointers.** A script in the scratchpad, not part of the project, took out every `CHECK(p != NULL)` of
  a pointer from the conditions of `if` and from the `&&` of a `return`: 257 guards went, and 389 conditions lost one
  or more terms. Only five functions can return a null pointer (`recipe_design_unit`, `menu_keys`, `app_open`,
  `controls_color`, `make_font`), and their callers compare the result with `NULL` themselves, without `CHECK`; those
  comparisons stay. The handles of Win32 among the pointers (windows, menus, brushes) are refused by Win32 itself
  without harm, and `color_control` answers a null brush with 0, as it did. `check.h` is no longer included by
  `recipe.c`, `civ_file_write.c`, `summary.c` and `tech_tree_file.c`.
- **Checks the code makes true itself.** Taken out by hand, file by file:
  - the cursor and the sink of the game's file (`dat_field.c`): the place within the file, a width within 4 bytes
    and the loops' second bounds, the sink's length within its room; every read of a cursor that has failed gives 0
    and every skip fails, so the heads that returned when the cursor had failed (`dat_skip.c`, `skip_tech_tree`) give
    the same answer without them;
  - counts handed to `dat_cursor_skip_items`, which refuses a negative count itself, and the count of civs against
    `DAT_CIV_CAPACITY`, which `map_civs` checks where it reads it (`list_both`, `dat_map_has_civ` and the loop of
    `dat_map.c` that walks the civs);
  - numbers that callers take from a loop or a table (`mod_tip`, `write_language`, `read_vanilla`, `language_file`,
    `step_columns`, `catalog_string_id`, `on_text_change`, `add_record`, `add_choice_record`, `read_looks`), room that
    is a `sizeof` (`exe_folder`, `item_text`, `dat_cursor_text`), the inflater's own state (`build_dynamic_codes`,
    `inflate_fixed`, `inflate_stored`, `repeat_lengths`, `take_length`), the catalog's counts checked twice against
    their capacities, the place of `train_in_castle` and the bytes of `deflate_pack`;
  - `identifier` and `code` of `on_command` and `sync_on_notify`, which `LOWORD` and `HIWORD` give and which are never
    negative.
- **Two comparisons of constants are `_Static_assert`s,** which C11 checks while compiling instead of at each call:
  `CBN_SELCHANGE == LBN_SELCHANGE` in `sync.c` and `UNIT_STAT_SPEED < UNIT_STAT_COUNT` in `catalog.c`.
- **A file cut short is data, not a fault.** `dat_read_unit` returns `cursor->fine` instead of `CHECK(cursor->fine)`.
- **The catalog reads a row apart from adding it.** `add_tech`, `add_unit_patch` and `add_bonus_change` each did both;
  `read_tech`, `read_unit_patch` and `read_bonus_change` now read the fields of a row and check them, and the three
  `add_` functions only check the room and count the row in.
- **Kept.** The checks of the game's file and of the catalog's lines where they enter (counts, ids, values), which a
  damaged or changed file can fail.
- **Tests.** 377, unchanged. No test checked that a null pointer was refused.
- **Checked.** `build.exe`, with the analyzer, after the pointers and after each file by hand: 377 tests, every one
  passed, no warning, no finding. The crapper on `src/`: of 174 functions at CRAP 10 or more, 25 were left after the
  checks of pointers, and none after the checks by hand and the catalog's split; the highest are `text_widen`,
  `sort_edits` and `huffman_build`, at 9.95. The dryer, at 0.8, on `src/` and the files of `tools/build/`: no two
  functions alike. The mutator on the lines changed is the next section.
- **Documents.** `DECISIONS.md`. `MAINTAINER.md`: under Rules for changing the code, that a condition on constants
  is a `_Static_assert`.

#### Not done

- `tools/build/`: 14 functions are at CRAP 10 or more, most of them the steps that run gcc, which no test runs.
- The checks were taken out where they guard what the callers already make true; a caller that one day hands a null
  pointer or a value out of range is no longer caught by a `CHECK`.

### 02:40:05: a number past the largest `long` refused, and the checks the mutator showed always true taken out

#### What changed

- **Why.** Requirement 12: the mutator on the lines that the section of 02:39:14 changed, in 49 files of `src/`. Of
  1328 mutants, 600 were killed and 415 survived; 313 were on lines that no test runs. Most survivors were checks that
  no caller can fail which the first pass had left: a condition that is always true lets `>` become `>=`, or a `!` be
  deleted, without any test seeing it.
- **A long number wrapped round.** `text_to_number` took at most ten digits and a value up to a billion, but a `long`
  is 32 bits on Windows, so "2147483648" wrapped round to a negative number, which was taken as read. A survivor of
  the check of the length showed it. `take_digit` now refuses a digit that would pass `LONG_MAX`, and the limits of
  ten digits and a billion are gone.
- **Second pass.** Taken out, as in the section of 02:39:14: the ranges of enums that only constants of the code are
  given (fields, kinds, tiers, the type of a change, the source of a list); counts and indexes already checked where
  they are set, and the second bounds of loops over them; checks of the size of a buffer that is a constant;
  `dat_map_has_civ`, as every caller has the civ's slot from the map or checks it where it enters (`start_run`); in
  `ask.c`, `menu.c`, `unique_tech.c` and `app_window.c`, conditions on constants and the window's step. `check.h` lost
  `CHECK_AMONG_LIMIT`, and `check_report` writes its message in one expression.
- **A third `_Static_assert`:** `RECIPE_ENABLE_UNIT != RECIPE_UPGRADE_UNIT`, in `recipe_tree.c`.
- **Kept.** The checks of the game's file, of the catalog's lines, of the civ file and of what Windows gives, where
  they enter. Their survivors stand where no test gives a damaged file: the slot of the civ list, the bound of a skip
  in `dat_field.c`, the room of the sink, the capacities of `dat_edit.c`, `json_patch.c` and `json_reader.c` and the
  depth of the latter, the lengths of `tsv.c`, `PLAIN_LIMIT` of `deflate.c`, the room of `inflate.c`, the ids of a
  civ file in `civ_design.c`, and `count <= 0` in `history.c`. The `0` that `command_of` gives a new command's text,
  made `1`, is the same command, an equivalent mutant.
- **Tests.** 379, up from 377. Test 127, "a number past the largest long is no number", replaces "a number beyond a
  billion is no number"; Test 128 is new: the largest number a `long` holds is read. Test 40 is new: a value the civ
  keeps stays the game's in the civ list. Test 6, of a design written as JSON and read back, also compares the texts
  read back, not only the two writings, which the same writer made and so could not tell a dropped escape.
- **Checked.** `build.exe quick`: 379 tests, every one passed. Test 127 failed against the old `take_digit`; Test 40
  failed with `value < 0L` made `value < -1L` in `civ_list.c`; Test 6 failed with an escape of `put_escape` broken.
  Each break was undone.
- **Obstacles.** Raising a suite's size by hand was missed twice while the tests were added: once the last core test
  dropped out of the run without a failure, once the run stopped at 176. The section of TIME_THREE makes a test fail
  then.
- **Documents.** `TESTS.md`: Tests 6, 40, 127 and 128, and the numbers after them. `MAINTAINER.md`: in Rules for
  changing the code, that a condition on constants alone is a `_Static_assert`. `DECISIONS.md`.

#### Not done

- The survivors of the last files of the run, in `src/win/`, and the mutator on the lines of this pass, are in the
  section of TIME_FOUR.
- Numbers from a billion to `LONG_MAX` are now read where they were refused.

### 02:41:02: the build plan apart from the steps that run gcc, and a test that every test defined is run

#### What changed

- **Why.** After the section of 02:39:14, 14 functions of `tools/build/` were still at CRAP 10 or more: the steps that
  make folders, compile, link and run the tests decided what to do and did it in the same function, and no test can run
  gcc, so none of their branches was covered.
- **The build plan.** `tools/build/build_plan.c` and `build_plan.h`, new, hold every decision of the build program: the
  mode from the command line (`build_mode_of`), the words passed on to the tests (`build_test_arguments`), the verdict,
  the folders of each mode, the command of gcc for a source, which programs are linked, their response files and links,
  and the command that runs the tests. They are tables and small functions of them, which read no file. `BuildMode`
  moved there from `build_steps.h`.
- **The steps.** `build_steps.c` now only makes the folders, compiles, links and runs, with what the plan gives; each of
  its functions takes one decision at most. Making the folders, compiling and linking count their failures instead of
  stopping at the first, so a build that fails shows the errors of every source at once. `build.c` reads the command
  line once and prints the verdict.
- **The checks of `source_list.c`.** The same pass as in the section of 02:39:14 took out the checks of its pointers: 5
  guards and 11 terms. The tests are linked with `tools/build/build_plan.c` too (`PROGRAM_PREFIXES`).
- **Every test defined is run.** The sizes of the suites in `suites.h` and the starts of the groups are kept by hand
  beside the `case`s. A size one short left the last test of a suite out of the run, and one too long stopped the rest
  of its category; both happened while the tests of the second pass were written, and no test failed. The runner now
  tells how many tests ran before the one running (`test_run_before`), and the last test of all compares that, and
  itself, with the `case`s of the suites, which `defined_tests` in `log_rules.c` counts as Test 362 did.
- **Room of the call graph.** The test of no recursion reads every function of the code; with the new ones they passed
  its 1600, and it failed on the check of its room. `GRAPH_FUNCTION_CAPACITY` is now 2400, `GRAPH_EDGE_CAPACITY` 24000.
- **Tests.** 391, up from 379. Tests 380 to 390 check the build plan: the three modes, the words passed on, the verdict,
  the analyzer in the command of gcc, the programs and the link of a coverage build, the folder of its report and the
  command of the tests. Test 391 checks that every test defined is run. The maintainability suite has two new groups for
  them, of eight and four.
- **Checked.** `build.exe quick`: 391 tests, every one passed. With the plan broken in ten ways, each of Tests 380 to
  390 failed; with the size of the core suite one short, Test 390, then the last, failed, and one too long stopped the
  run at 286 tests, the last failing. Each break was undone. The crapper on the four files of `tools/build/`: every
  function under 10, the highest `scan_folder` and `source_list_collect` at 8.3. The dryer at 0.8 on `src/` and those
  files: no two functions alike.
- **Documents.** `TESTS.md`: Tests 380 to 391; the lines of Tests 371 to 379, which named lines above the tests already
  before this change; and the line in `maintainability_suite.c` of Tests 350 to 363, one lower. `MAINTAINER.md`: the
  table of files, on `build_plan.c` and `build_steps.c`, and, in the steps of adding a test, what a wrong size does.
  `TERMINOLOGY.md`: build plan. `DECISIONS.md`.

#### Not done

- `build_steps.c` and `build.c` are not run by any test: they run gcc and the tests, which a test cannot do from within
  the tests. Their functions are each of complexity 2 or less, so CRAP stays under 10 without coverage.
- In a run with `nowindow`, Test 391 passes without counting, since the window's tests are left out of it.

### 03:54:37: the last checks no caller can fail taken out, and ten tests of what no test saw

#### What changed

- **Why.** The last files of the run in the section of 02:40:05, `assist.c`, `controls.c`, `dialogs.c` and `sync.c`, had
  59 survivors, most on checks that no caller can fail. Then the mutator ran on every line changed since 02:39:14, in 47
  files (Requirement 12). The survivors that are not on a check show what no test sees.
- **Checks taken out,** as in the section of 02:40:05: `app->dpi > 0` six times, since `assist_dpi` checks what Windows
  gives where it gives it; the window's step; the number of a control, which `sync_on_notify` checks where Windows gives
  it; the count of a list's rows, which `list_model_fill` keeps within its room; the colour role; sizes made from
  constants; the conditions on `CONTROL_COUNT`, `BUTTON_COUNT` and `TEXT_PERCENT_PLAIN`; the pointers and room of
  `dialogs_widen` and `dialogs_narrow`, whose callers pass buffers of a constant size; in `hero_hidden` the pointer to a
  member of the run; in `skip_tree_nodes` the kind of a tree, which only constants give; and in `text_percent` the room
  for a `DWORD`, which is the size of a `DWORD`.
- **Loops bounded by what they walk.** Seven loops had a second bound that no text or data reaches: `text_widen` (each
  letter takes a place in the wide text, whose room ends the loop, so its last `return` is now plainly `true`),
  `flush_bytes` (the waiting bits, at most 64; `FLUSH_LIMIT` is gone), the loop of `deflate.c` over the plain text (each
  step moves on by a letter or a match), `forge_prefix` (the alias ends within its buffer, which the design checks where
  it enters), `trim_end` and `put_on_one_line` of `mod_text.c` (the text, which ends within its buffer), and
  `huffman_build` (the count of symbols, checked at its start; its CRAP score falls from 9.95).
- **Tests of what no test saw.** A unique tech keeps one change of an attribute of the civ's unit, but one change of
  each armor class; `unique_same_target` tells them apart, and no test held it to either. The details beside the list of
  Castle Age techs were never read by a test. The tree's changes were sorted and kept once each, but the recipe's tests
  have one disable, so no test saw their order, nor that a disable names its tech in `d`. U+10000, the first letter that
  needs two wide letters, was never widened by a test, so `<` made `<=` at the plane survived. A failed check's message
  could begin with the first letter of the message before it: Test 1 looked for its text within the message, and now
  makes two checks fail and compares the whole message. No tip was made of a text with a line break, so the loop that
  puts a tip on one line could be skipped. No test read the text of a row of the unique techs, so a list of techs could
  have shown units' names. No test gave a unit a speed or a reload time, kept in hundredths, so a division by 100 made a
  product survived.
- **`norules` keeps the tests of the build program.** The mutator could not judge `build_plan.c` and `source_list.c`:
  their tests are in the maintainability category, which ran only with the tests of the rules, and those fail in a
  worker, whose copy of the tree has no files that Git lists. `norules` now leaves out only the tests of the rules and
  the documents; the runner takes the category's tests of the build program from `maintainability_tools_run`, and the
  last test passes uncounted without the rules, as without the window.
- **`source_list.c` measured at last.** With that, the mutator judged `build_plan.c` and `source_list.c` for the first
  time. Of the 11 survivors of `source_list.c`, six were on checks and second bounds that no caller can fail: the
  program given to `source_in_program`, the count of `source_list_newest`, an empty path in `source_file_modified`, and
  the second bounds of three loops; they were taken out. A list of the project's sources could have begun with a stray
  entry: Test 382 now compares it with the lists of the three folders, and Test 381, new, checks that the list of a
  folder holds only its files.
- **The suites without checks of their index.** With one more test, `tests/suites/model_suite.c` passed 400 lines of
  code (Rule 13). Each group of tests in a suite, and each suite, began with a check of the range of the index, which no
  caller can fail: the runner asks for 1 up to 400, and the `default` of each `switch` answers an index past its tests.
  The 53 checks were taken out of the six files of suites, with `SUITE_TEST_LIMIT` of `suites.h`.
- **Tests.** 401, up from 391. Tests 34 and 35 are new: a unique tech's changes of one attribute by two amounts are one
  target, and its changes of two armor classes are two targets. Tests 78 and 79 are new: the tree's changes are sorted,
  unlocks first, then by tech, and two disabled techs are two changes of the tree. Test 124 is new: a line break in a
  tip is written as a space. Test 137 is new: the first letter beyond the basic plane is widened to two. Test 219 is
  new: a row of the castle techs shows its tech's name. Test 232 is new: the details of a unique tech name it and say
  what it does. Test 233 is new: the details of a unit give its speed to a tenth. Test 381 is new: a folder lists only
  its own sources. Test 1 makes two checks fail, and Test 382 counts the sources of the project. The tests after each
  new one are renumbered; the last is Test 401.
- **Checked.** `build.exe`, with the analyzer: 401 tests, every one passed, no warning, no finding. The crapper on
  `src/` and the four files of `tools/build/`: 882 functions, none at 10 or more, the highest `bottom_box` at 9.6. The
  dryer at 0.8: no two functions alike. Each new test was seen to fail on the survivor it was written for, and the break
  undone: Test 232 with `found == CATALOG_MISSING` made `!=` in `add_tech`; Test 34 with either `!=` of `armor_class`
  made `==`, and Test 35 with the first; Tests 78 and 79 with the `==` of `change_key` made `!=`, and Test 78 with
  `place > 0` made `place > 1` in `recipe_sort_changes`; Test 137 with `code < UTF16_PLANE` made `<=` in `put_wide`;
  Test 124 with `!=` made `==` in `put_on_one_line`; Test 219 with `==` made `!=` in `picked_row`; Test 233 with `/`
  made `*` in `add_hundredths`; Test 1 with `last_failure[0]` made `last_failure[1]` in `check_report`. Test 382 with
  `list->count = 0` made `1` in `source_list_collect_all`, and Test 381 with the same in `source_list_collect`. The
  mutator on every line changed since 02:39:14, in 47 files (run 4): 695 mutants, 268 killed, 146 survived, and 281 on
  lines that no test runs, 41 of them in `build_steps.c`, which runs gcc; the baselines of `build_plan.c` and
  `source_list.c` failed in the workers. Then, after the changes of this section, on the lines they changed and those of
  the survivors the new tests were written for (run 5): 208 mutants, 101 killed, 22 survived, 85 on lines that no test
  runs; the 12 of `build_plan.c` were all killed. Last, on the lines changed in `source_list.c` (run 6): 44 mutants, 20
  killed, 7 survived, 17 on lines that no test runs; Test 381 was written for one of the 7.
- **Replaced.** `norules` leaving out every test of maintainability, those of the build program too, in [the analyzed
  build clean, and ub-mode's mutator able to run on tests without the
  rules](DEVLOG5.md#025216-the-analyzed-build-clean-and-ub-modes-mutator-able-to-run-on-tests-without-the-rules) of
  2026-11-16.
- **Correction.** The section of 02:40:05 names "the section of TIME_THREE" and "the section of TIME_FOUR", whose
  times were not known when it was written: they are the section of 02:41:02 and this one.
- **Documents.** `TESTS.md`: Tests 1, 34, 35, 78, 79, 124, 137, 219, 232, 233, 381 and 382, the numbers after them, the
  lines of the tests of `read_tests.c`, `present_tests.c` and `tool_tests.c`, which moved, and what Test 401 expects
  without the rules. `MAINTAINER.md`: what `norules` leaves out. `DECISIONS.md`: the decision on the tests the mutator
  runs, and Test 304 in the decision on tests that measure time. `DEVLOG6.md`, new: the day 2026-11-17 moved into it
  whole from part 5, which this section would have taken past 1000 lines (Rules 18 and 22); its two links to part 5
  mended, and two of its lines wrapped within 120 characters, their words unchanged. A copy of part 5 that was written
  into `documentation/DEVLOG/` by mistake while part 5 was closed was moved to the scratchpad. `DELETION_REQUESTS.md`:
  the workers of the runs of the mutator, and what this night's work left in its scratchpad.

#### Not done

- **Survivors that stand.** On checks of outside data (question 1): the game's file, the catalog, a civ file, the
  library list of Steam, and what Windows gives. On capacities that no design reaches. Equivalent mutants, such as a
  sort or a clearing that starts one place later where that place is always set, and in `focus_step` the list of steps,
  which no step owns. In 28 checks of whether a text is empty, `text[0]` made `text[1]`, which only a text of one letter
  tells apart.
- **Gaps that no test covers yet.** A unique tech that costs food alone (`any_cost` in `unique_tech.c`); the hot key a
  unique tech keeps; the look of a second unit in `look_copy.c`, which needs a game file of two units; the name of a
  hero's enabling tech, which no test reads; a file with the name of the mod's folder (`mod_places.c`); in
  `recipe_copy.c` the places of research of the replaced civ's own techs and a disable that names a copied tech; in
  `recipe_tree.c` a tech of resources with a place of research; and in the window, the knob of its scroll bar dragged,
  the rows of a plain list wrapped, the fonts freed when the scale changes, and the guard `syncing` against the window's
  own writes.
- Question 1 of 22:12:27, in [the done rule's mutants killed, and the checks of `wizard.c` no caller can fail taken
  out](DEVLOG5.md#221227-the-done-rules-mutants-killed-and-the-checks-of-wizardc-no-caller-can-fail-taken-out), is still
  open: the last test now fails when a suite's size is wrong, which the comparison proposed there would also catch.

#### Questions

1. **Survivors at checks of outside data.** The checks of the game's file, the catalog, a civ file and what Windows
   gives keep their survivors, since no test gives the program such data damaged. Rule 24 allows a test only for a
   requirement or a bug found before. Should tests give damaged files, under Requirement 9 (Build says what it could not
   build), to kill them? Proposal: yes for the game's file and the catalog, where a damaged file should make Build say
   what it left out; the capacities that no design reaches stay as survivors, listed in the log.

### 04:37:52: a bonus's multiplier applied to the bonuses built from changes and to the team bonus

#### What changed

- **Why.** A civ file gives each bonus a `multiplier`, how many times it counts; civbuilder wrote it, but read it as 1
  and built every bonus once (in [bonuses built from the original's lists of
  changes](DEVLOG1.md#024535-bonuses-built-from-the-originals-lists-of-changes) of 2026-11-09 it was left undone). It is
  one of the original's features still missing.
- **How the original scales.** No copy of the original is on the disk; its source was read on GitHub. `civ_appender.py`
  scales each change of a bonus so that one application does what `multiplier` applications would
  (`_scale_ec_for_multiplier`, `_scale_packed_cmds`): an amount added to an attribute or a resource is multiplied by it;
  a factor that multiplies an attribute or a resource, or a tech's cost or time in mode 2, is raised to it; a spawn
  makes as many times the units; armor and attack, whose value of at most 255 shares one number with its armor class,
  are split into several changes when they grow past 255, or past 255% for a factor, a percent halfway between two
  rounded to the even one, as Python rounds; a change that sets a value, the spawn limit (resource 234) and the effect
  function (resource 33) are kept.
- **`bonus_scale.c`.** `bonus_scale` in `src/bonus/bonus_scale.c`, new, does the same: it takes one change and a
  multiplier and gives up to 10 changes, as a pure function. `bonus_add_scaled` adds them to an effect. A multiplier of
  1 gives the change unchanged, so the effects that the recipe copies whole, through the same function, are as before.
- **Where it applies.** `bonus_build_civ` and `bonus_build_team` take the multiplier and add each change scaled, and the
  team bonus copied from the game's effect is copied scaled (`recipe_copy_changes`). A civ bonus made of the game's
  techs is copied or unlocked as before and counts once: the original multiplies the copied effect, which the recipe
  does not yet. The report counts such bonuses (`multipliers_skipped`), and the summary of the build says "A bonus made
  of the game's techs counts only once." (Requirement 9).
- **Read where it enters.** `read_bonuses` of `civ_file_read.c` reads `multiplier`, and `civ_design_set_multiplier`
  takes it if it is 1 to 10 (`CIV_MULTIPLIER_LIMIT`); any other leaves the bonus at 1. It returns nothing: no caller had
  a use for whether it took the value. The original reads it with `int()` and no bound; 10 bounds the room of
  `ScaledChanges` and the loops that split armor and attack.
- **The change types in one place.** The types of change that the scaling tells apart are named in `dat_map.h`, beside
  `DatChange`, as `DAT_CHANGE_...`; `RECIPE_TECH_COST` and `RECIPE_TECH_TIME` of `recipe_parts.h` became
  `DAT_CHANGE_TECH_COST` and `DAT_CHANGE_TECH_TIME`.
- **One level of pointers.** Test 390 (P10 rule 9) failed twice on the first version: `run->report->` in `plan_bonus`
  and `input->design->` in `add_team_bonus`. Each function now names the report or the design first.
- **Tests.** 428, up from 401. Test 7: a bonus's multiplier is read back from a civ file. Test 25: a bonus taken twice
  is built with its changes doubled. Test 69: a copied tech's effect keeps the amounts of its changes, since the copy
  now goes through the scaling. Test 73: the report counts a multiplier of a bonus made of the game's techs. Tests 75
  and 90: a team bonus taken twice doubles the changes of its effect, and one made of changes doubles them. Tests 106 to
  123, in `tests/bonus/scale_tests.c`, new: one for each way a change is scaled, kept, split or rounded. Test 260: the
  build report says when a bonus counts only once. Tests 290 and 291: a multiplier past ten, and one below one, is
  refused. The recipe's tests build two more civs, with multipliers. The tests after each new one are renumbered; the
  last is Test 428.
- **Checked.** `build.exe`, with the analyzer: 428 tests, every one passed, no warning, no finding. The crapper on
  `src/` and the four files of `tools/build/`: 896 functions, none at 10 or more, the highest still `bottom_box` at 9.6;
  `bonus_scale` at 7.0. The dryer at 0.8: no two functions alike. Each new test was seen to fail on a break of the code
  it covers, in three runs of breaks that touch different tests, and in each run exactly the tests expected failed: 7
  tests with, among others, the added amount multiplied by 2 always, a multiplied attribute not raised, the spawn's
  count kept and half rounded up; 10 with, among others, the factor of a resource not raised, the sign of armor lost,
  the bounds of the design's multiplier moved by one and the build report's condition turned; 6 with the multiplier of
  each place where it is applied made 1, the report's count left out and the file's key misspelt. Each break was undone,
  the files compared with their copies. The mutator on every line this change touched, in seven files (run 7): 150
  mutants, 91 killed, 29 survived, 30 on lines that no test runs. Four survivors showed what no test saw: a copy of a
  tech whose amounts were all made 0, since the copy now goes through the scaling, a percent rounded past a half, one
  below a half on an odd number, and armor grown to exactly 255 split into a second change of 0; Tests 69 and 121 to 123
  were written for them, each seen to fail on its survivor and the break undone. One showed dead code: no caller read
  whether `civ_design_set_multiplier` took the value, so it returns nothing now. On the lines these changed (run 8): 104
  mutants, 71 killed, 15 survived, 18 on lines that no test runs; the four survivors were killed.
- **Documents.** `TESTS.md`: Tests 7, 25, 69, 73, 75, 90, 106 to 123, 260, 290 and 291, and the numbers after them.
  `MAINTAINER.md`: `bonus_scale.c` in the table of files, how a multiplier is applied, `tests/bonus/`, and the six civs
  of `recipe_game.c`. `TERMINOLOGY.md`: multiplier. `DECISIONS.md`: the decisions of this section, and Test 331, the old
  Test 304, in the decision on tests that measure time. `DELETION_REQUESTS.md`: the copies kept in the scratchpad while
  the tests were seen to fail.

#### Not done

- **Survivors that stand.** Equivalent mutants: `*` made `/` by a sign of 1 or -1; the sign of an armor of 0; the second
  bound of the loop that splits a factor, and its `>` made `>=` at a factor of exactly 2.55, which only a multiplier of
  1, where the loop does not run, reaches; `<` made `<=` at an exact half on an odd number, which no factor reaches; a
  multiplier of 1 refused, or a missing one read as 0 and refused, where the bonus already counts once. On lines the
  mutator ran again, older than this change: the report's starting values, of which three the recipe always sets and
  three, the team bonuses built and skipped and the unit's techs built, no test reads as 0; a civ bonus or a team bonus
  with nothing to build counted as built when `> 0` is made `>= 0`, since no test has a bonus in the catalog with
  neither techs nor changes; the remapping of a copied effect, as listed in the section of 03:54:37.
- The multiplier of a civ bonus made of the game's techs, about 200 of the 394, is not applied: the copied effect would
  be scaled as the original's `_multiply_effect` does, skipping the spawns of the helper units of triggers, and a
  generic tech the bonus unlocks would need the scaled rest in a tech of its own. Nor is it applied to a bonus's unit
  patches, nor to the bonuses the original builds by code of their own with the multiplier (`_create_bonus_handler`).
- The window cannot set a multiplier, and the review does not show one: only a civ file gives it.
- The mod was not built with a multiplier and tried in the game.

#### Questions

1. **The bound of a multiplier.** The original takes any whole number; civbuilder takes 1 to 10, and any other leaves
   the bonus counting once. What is the most a bonus can be taken in the original's window? Proposal: 10, unless the
   original allows more.
2. **A resource set, taken twice.** The original multiplies every change of a resource but the spawn limit and the
   effect function, whatever its mode, so a bonus that sets a resource to 1 sets it to 2 when taken twice. civbuilder
   does the same. Proposal: keep the original's way, since a civ file made by the original must build alike.

### 04:45:01: copied effects tested for the techs they name, and why the copied techs' multiplier waits

#### What changed

- **Why.** Two survivors of run 7 of the section of 04:37:52 stood on the remapping of copied effects: no test saw that
  a copied effect naming a tech the recipe also copies names the copy, nor that the team bonus, which allies take too,
  keeps naming the game's own tech. Both stood among the survivors listed there.
- **The small game of the recipe's tests.** The effect of bonus tech 6 and team effect 9 each have a change more: tech
  8's wood cost lowered by 10. Tech 8 is copied as tech 13 for the civ.
- **Tests.** 430, up from 428. Test 70: a copied effect names the copy of a copied tech. Test 76: the team bonus names
  the game's own techs. The tests after each new one are renumbered; the last is Test 430.
- **Checked.** `build.exe quick`: 430 tests, every one passed. With the remapping of a copied effect turned off, Test 70
  failed, and with the team bonus's copy remapped, Test 76; both breaks were undone, the files compared with their
  copies. The mutator on the two lines, with the earlier survivors of their files (run 9): 44 mutants, 22 killed, 8
  survived, 14 on lines that no test runs; both survivors were killed, and the 8 left are the older ones of `recipe.c`
  that the section of 04:37:52 lists.
- **Why the multiplier of a bonus made of a civ's techs waits.** The original multiplies the copied effect, but keeps
  every spawn of a helper unit of a trigger: a unit whose building names a tech of one civ (`_is_trigger_helper`).
  civbuilder reads no unit's building, so it cannot tell such a spawn from one of real units, and scaling both could
  make a trigger fire twice. Such a bonus counts once, and Build says so, as in the section of 04:37:52.
- **Documents.** `TESTS.md`: Tests 70 and 76, and the numbers after them. `DECISIONS.md`: Test 333, the old Test 331, in
  the decision on tests that measure time.

#### Not done

- The multiplier of a civ bonus made of the game's techs: it needs the tech that a unit's building names, read from the
  dat file, to keep the spawns of helper units as the original does; and for a generic tech the bonus unlocks, the
  scaled rest in a tech of the civ's own.

### 04:50:03: the build's report held to what it skipped and built

#### What changed

- **Why.** Among the older survivors that the section of 04:37:52 lists, five showed counts of the build's report that
  no test held (Requirement 9: Build says what it could not build). A civ bonus or a team bonus that the catalog lists
  with nothing to build would have been counted as built had `> 0` been `>= 0`, since no test had one; and the starting
  zeros of the team bonuses built and skipped and of the unique unit's techs built could have been ones.
- **The small game of the recipe's tests.** Its catalog lists bonus 53 and team bonus 62 with nothing to build, and a
  seventh civ takes both.
- **Tests.** 435, up from 430. Test 74: a bonus with nothing to build is counted as skipped. Test 94: a team bonus with
  nothing to build is counted as skipped. Test 95: a skipped team bonus is not counted as built. Test 96: a built team
  bonus is not counted as skipped. Test 106: a unique unit not built counts no techs. The tests after each new one are
  renumbered; the last is Test 435.
- **Checked.** `build.exe quick`: 435 tests, every one passed. With `> 0` made `>= 0` for the civ bonus and the team
  bonus, Tests 74, 94 and 95 failed; with the report's team bonuses built and skipped and unit techs built starting at
  1, Tests 95, 96 and 106 failed; both breaks were undone, the file compared with its copy. The mutator on the lines of
  those survivors in `recipe.c` (run 10): 32 mutants, 21 killed, 3 survived, 8 on lines that no test runs; the five
  survivors were killed. The three that stand are equivalent: the report's unit, tree and hero flags start false, and
  the recipe always sets each of them before the build ends.
- **Documents.** `TESTS.md`: Tests 74, 94, 95, 96 and 106, and the numbers after them. `MAINTAINER.md`: the seven civs
  of `recipe_game.c`. `DECISIONS.md`: Test 338, the old Test 333, in the decision on tests that measure time.

### 05:17:56: bonuses 81, 103, 247, 283 and 352 built from the game's requirement techs

#### What changed

- **Correction to the section of 04:37:52.** Its **Checked.** point names a run of `build.exe` with the analyzer at 428
  tests; that run was made at 04:51, after the section of 04:50:03, at 435 tests, with the same code in `src/`. It
  passed: no warning, no finding.
- **Why.** Of the 22 civ bonuses that Build skipped, five make a tech of the game come an age early, or need no
  building: 81 (no buildings needed to advance or to unlock buildings), 103 (the Cavalier in the Castle Age), 247
  (Parthian Tactics in the Castle Age), 283 (Chemistry and the Hand Cannoneer in the Castle Age) and 352 (Siege
  Engineers in the Castle Age).
- **How the game does it.** The dat file of the installed game was read by a program in the scratchpad that uses the
  project's readers. Each of those techs of the game lists, among its requirements, a tech of the one civ that has the
  bonus: the Cavalier (209) needs two of the Imperial Age, the Knight and tech 768, a tech of civ 36 that needs the
  Castle Age and the Knight and is researched by itself; Parthian Tactics (436) needs the Imperial Age or tech 57, of
  civ 8; Chemistry (47) the Imperial Age or tech 800, of civ 39; the Hand Cannoneer (85) two of three, among them tech
  801, of civ 39; Siege Engineers (377) the Imperial Age or tech 978, of civ 52; the shadow nodes of the three ages
  (659, 660 and 661), each one of its requirements, among them tech 638, of civ 28, which needs nothing.
- **How the original does it.** Its `_create_bonus_handler` copies 800, 801, 638 and 978 for the civ and puts the copy
  in a free slot of the requirements of 47, 659 to 661 and 377. For 103 and 247 it copies the Cavalier, the Paladin and
  Parthian Tactics themselves, with the Castle Age in place of the Imperial Age, and disables the game's own in the
  civ's tree; for the Hand Cannoneer it copies tech 85.
- **How civbuilder does it.** As the game does for its own civs, for all five: a new kind of line in
  `bonus_effects.txt`, `require`, names a bonus, a tech of the game and a requirement tech of another civ. The recipe
  copies the requirement tech for the civ (`recipe_plan_requirement` in `recipe_copy.c`, through the plan that copies
  every tech) and, once the copies have their numbers, writes the copy's number in the first free slot of the
  requirements of the game's tech (`add_requirement`). No tech that the tech tree shows is copied, so nothing has to be
  disabled, and a tech that the design's tree disables stays disabled. Two lines that require the same tech share its
  copy, as bonus 81's three lines share the copy of 638. A game tech with no free slot leaves its bonus skipped; Build
  says so (Requirement 9). A bonus of requirements counts once, whatever its multiplier: a tech cannot come two ages
  early.
- **The data.** Eight `require` lines: 81 three, 283 two, 103, 247 and 352 one each. The catalog reads them
  (`add_requirement` in `catalog.c`) and holds up to 32 (`CATALOG_REQUIREMENT_CAPACITY`).
- **Tried on the game's file.** The program in the scratchpad built a mod from the installed game with the five bonuses,
  for the slot of the Britons, into the scratchpad: 5 built, 0 skipped. In the mod's dat file, techs 659, 660 and 661
  each list tech 1510, a copy of 638 for civ 1; 209 lists 1511, a copy of 768; 436 lists 1512, 47 lists 1513, 85 lists
  1514 and 377 lists 1515, each a copy for civ 1 of the tech the game gave there, with its requirements.
- **One function split.** `recipe_commit_plan` passed CRAP 10 with the loop that writes the requirements (11.5); the
  loop is `add_requirements` now, and the function is at 9.2.
- **Tests.** 443, up from 435. Test 76: a bonus requirement is a tech of the civ. Test 77: two techs given one
  requirement share its copy. Test 78: a bonus of requirements is counted as built. Test 79: a bonus whose tech has no
  room is skipped. Test 80: a second build keeps no requirement of the first. Test 81: a tech that no line names keeps
  its requirements. Test 233: the catalog holds every requirement line of its file. Test 234: a requirement line names
  its bonus, its tech and the tech required. The small game of the recipe's tests has two more bonuses, one with two
  lines and one whose tech has all six slots taken, and an eighth civ that takes both. The tests after each new one are
  renumbered; the last is Test 443. The cases of the team bonus are a group of their own in `build_suite.c`, as the
  group of the bonuses passed 60 lines (Rule 11).
- **Checked.** `build.exe quick`: 443 tests, every one passed. Each new test was seen to fail on a break of the code it
  covers, in five runs, and in each run only the tests expected failed: with the plan's requirements not emptied before
  a build and the requirement read from the wrong field, Tests 80 and 234; with a slot that names tech 0 taken for free,
  Tests 78 and 79; with the template written in place of its copy, Test 76; with the first line left out of the writing,
  Test 77. With every copy of the plan made anew, seven other tests failed, and Test 77 did not, since a requirement
  names the first copy of its tech. Each break was undone, the files compared with their copies. The crapper on `src/`
  and the four files of `tools/build/`: 903 functions, none at 10 or more, the highest still `bottom_box` at 9.6. The
  dryer at 0.8: no two functions alike. The mutator on every line this change touched, in the three files (run 11): 94
  mutants, 50 killed, 12 survived, 32 on lines that no test runs. Two survivors showed what no test saw: the plan's
  requirements emptied to 1 instead of 0, so that an empty line, or one left by an earlier build, is written again; and
  one line more than the plan holds written. Test 80 was made to look at the first line's slot, where it looked at the
  second's, and Test 81 was written; each was seen to fail on its survivor, and the break undone. On those two lines of
  `recipe_copy.c` (run 12): 40 mutants, 30 killed, 2 survived, 8 on lines that no test runs; both survivors were killed,
  and the two left are bounds at the room of the plan. The survivors that stand: the checks of a requirement line where
  the catalog reads it, at 0 and below (outside data, as question 6 of the sitrep asks); the room for 16 requirements in
  the plan and the bound of the loops at the catalog's 32, which no design reaches; and the loop over the catalog's
  lines in `plan_bonus_requirements` run past the last line, where every line is zero and names bonus 0, which is made
  of techs and so never comes there.
- **Documents.** `TESTS.md`: Tests 76 to 81, 233 and 234, and the numbers after them. `MAINTAINER.md`: the `require`
  lines in the table of catalog files, where they come from and how many the catalog holds, how a mod builds them, and
  the eight civs of `recipe_game.c`. `TERMINOLOGY.md`: requirement tech. `DECISIONS.md`: the decisions of this section,
  and Test 346, the old Test 338, in the decision on tests that measure time. `DELETION_REQUESTS.md`: the copies, the
  original's file and the two programs kept in the scratchpad.

#### Not done

- 17 civ bonuses are still skipped: 221, 222, 239, 286, 308, 309, 310, 332, 356, 400, 451, 454, 455, 456, 457, 459 and
  460. Most of them make a unit or change one.
- The tech tree screen still shows the five techs in their old ages: `civTechTrees` is not written.
- A bonus of two lines whose second finds no free slot keeps its first.
- The mod was not tried in the game.

### 05:27:22: bonuses 221 and 286 built from the game's techs

#### What changed

- **Why.** Two more of the bonuses that Build skipped turned out to be techs of the game's own civs.
- **221, the upgrades of the Militia and the Spearman an age early.** The Long Swordsman (207), the Two-Handed Swordsman
  (217), the Champion (264), the Pikeman (197) and the Halberdier (429) each list, among their requirements, a
  requirement tech of civ 44: 952 to 956, each needing the age before and the upgrade before. Five `require` lines give
  them to the civ, as in the section of 05:17:56. The original copies every Barracks upgrade it finds with the age moved
  and disables the game's own; the Man-at-Arms, which the bonus leaves out, has no requirement tech in the game either.
- **286, the Houfnice.** Its line in `bonuses.txt` names tech 787, the Houfnice of civ 39, which the recipe copies for
  the civ as it copies every tech of a civ that a bonus names. The original adds the Imperial Age to the copy's
  requirements in case Chemistry comes in the Castle Age; the game's 787 already needs the Imperial Age and the Bombard
  Cannon (188), so nothing is added.
- **Tests.** 443, as before. Test 233 now counts 13 requirement lines.
- **Checked.** `build.exe quick`: 443 tests, every one passed. With one of the lines of 221 taken out of
  `bonus_effects.txt`, Test 233 failed, and only it; the line was put back and the file compared with its copy. The
  program in the scratchpad built a mod from the installed game with the seven bonuses 81, 103, 221, 247, 283, 286 and
  352: 7 built, 0 skipped. In its dat file, techs 207, 217, 264, 197 and 429 list 1516 to 1520, copies for civ 1 of 952
  to 956 with their requirements, and tech 1521 is the Houfnice for civ 1, with a copy of its effect, needing techs 103
  and 188. No code changed, so the crapper, the dryer and the mutator were not run again.
- **Documents.** `TESTS.md`: Test 233. `MAINTAINER.md`: bonus 221 among those of the `require` lines.

#### Not done

- 15 civ bonuses are still skipped: 222, 239, 308, 309, 310, 332, 356, 400, 451, 454, 455, 456, 457, 459 and 460.
- The mod was not tried in the game.

### 05:41:10: bonus 356 built from the game's pasture techs and a line that disables the Farms

#### What changed

- **Why.** Bonus 356, Pastures in place of Farms and their upgrades at the Mill, was skipped: its line in `bonuses.txt`
  named no techs, and it had no lines in `bonus_effects.txt`.
- **Data.** In `bonuses.txt`, bonus 356 names techs 1008, 1012, 1013 and 1014 of civ 53. Tech 1008 enables the pasture
  (unit 1889) by itself at the start; the three others are the pasture's upgrades at the Mill, each needing the one
  before. In `bonus_effects.txt`, four `civ` lines of entry 0 disable the Farms (216), Horse Collar (14), Heavy Plow
  (13) and Crop Rotation (12). The game disables techs in the same way, from the effect of a tech of one civ, such as
  tech 895 of civ 43. The original copies only the three upgrades, and puts the pasture and the disabled techs in the
  civ's tech tree.
- **Recipe.** In `plan_bonus` of `recipe.c`, a bonus that names techs builds its lines too, at a multiplier of 1, since
  a bonus of techs counts once. Before, such a bonus built its techs only; no bonus had both. `build_bonus_lines` gives
  how many lines and requirements it built, or `BONUS_BUILD_FAILED`, with a guard of its own for each failure.
- **Tests.** 445, up from 443. Test 100 checks that bonus 56 of the small game, which names tech 10 and has one line,
  gets the change of its line in tech 11; Test 101 that, taken twice, it adds 9 and not 18. The bare civ with
  multipliers takes bonus 56 twice. In `recipe_game.c`, `scenario_tech` reads a tech of any build of the small game,
  `recipe_game_required_tech` reads through it, and `recipe_game_scenario_changes` and `recipe_game_scenario_amount` are
  new. Tests 100 to 443 became 102 to 445.
- **Checked.** `build.exe quick`: 445 tests, every one passed. With the lines of a bonus of techs left out, Tests 100
  and 101 failed; with them built at the bonus's multiplier, Test 101 failed, and only it; each break was undone and the
  file compared with its copy. The program in the scratchpad built a mod from the installed game with the eight bonuses
  81, 103, 221, 247, 283, 286, 352 and 356: 8 built, 0 skipped. In its dat file, tech 1523 is 'C-Bonus, Pastures' for
  civ 1 and enables unit 1889; techs 1526, 1525 and 1524 are the three upgrades for civ 1, needing the Feudal Age, then
  the Castle Age and 1526, then the Imperial Age and 1525; tech 1510, which needs no tech, disables 216, 14, 13 and 12.
  Coverage and the crapper: 903 functions, the highest 9.6 (`bottom_box`), `plan_bonus` 5.2 and `build_bonus_lines` 3.1.
  The dryer found nothing. The mutator on the changed lines of `recipe.c`, run 13: 25 mutants, 14 killed, 4 survived, 7
  on lines that no test runs. Three of the survivors are the loop bound of `plan_bonus_requirements` past the catalog's
  end, which survived before (section 05:17:56). The fourth, the `||` of the one check of both failures in
  `build_bonus_lines` made `&&`, showed that no test has a bonus whose lines build while its requirement finds no room;
  the check became two guards. Run 15, after a new measure of coverage: 18 mutants, 8 killed, the 3 of the loop bound
  survived, 7 on lines that no test runs, each a `return false` after a failed `CHECK` elsewhere in the file.
- **Documents.** `TESTS.md`: Tests 100 and 101, and the numbers of Tests 102 to 445. `MAINTAINER.md`: a bonus that names
  techs builds its lines too, as bonus 356 does. `DECISIONS.md`: the decision of the section of 05:27:22, which it
  lacked, one decision of this section, and the number of Test 348 in a decision on scrolling.

#### Not done

- Bonus 345 gives a free Villager for each economic upgrade, but not for the three pasture upgrades when the civ takes
  bonus 356 too. The original adds three techs for them, each needing the copy of an upgrade.
- No test has a bonus whose lines build while its requirement finds no room. The two guards of `build_bonus_lines` skip
  it, but no test holds that.
- 14 civ bonuses are still skipped: 222, 239, 308, 309, 310, 332, 400, 451, 454, 455, 456, 457, 459 and 460.
- The mod was not tried in the game.

#### Questions

1. **The game's way or the original's.** Bonuses 81, 103, 221, 247, 283, 352 and 356 are built as the game builds them
   for its own civs: a requirement tech, or the game's own techs of one civ, copied. The original copies the techs of
   the game and disables the game's own in the civ's tree, or puts them in the tree. A mod built from the installed game
   had each in its dat file, but none was tried in the game. Proposal: keep the game's way, and try one such mod in the
   game.
2. **Bonus 345 with 356.** Should the free Villager of 345 come with the pasture upgrades too, as in the original?
   Proposal: yes, by three techs that each need the copy of a pasture upgrade, as the original does, after the bonuses
   still skipped.

### 05:47:59: compliance with RULES.md and REQUIREMENTS.md, after the bonuses built from the game's techs

#### What changed

- **Why.** Many files changed on 2026-11-17, so the project was checked against every rule and requirement.
- **Mended.** Nothing: no breach was found that could be mended. No file changed for the check but this section and
  `DECISIONS.md`.
- **The build.** `build.exe`, with the analyzer: no warning, no finding, and 445 tests, every one passed. In `TESTS.md`,
  the tests hold Rules 3 to 6, 9 to 15, 18 to 22, 24, 26 (in part) and 29, and Requirements 1, 2, 3 (in part) and 5 to
  9.
- **Rules 0, 2 and 30, by git.** The last commit is of 2026-11-16, and none was made since. `REQUIREMENTS.md` and
  `RULES.md` are as the last commit has them. `git status` lists no file deleted.
- **Rule 17.** Part 6 of the log has no word for a person: no "I", "we", "you", "our", no name of an agent, and no
  "chief developer".
- **Rules 7, 8 and 23.** `MAINTAINER.md` begins with its table of contents, then "Build and test after a change". Each
  of the 239 links of `DECISIONS.md` and `MAINTAINER.md` to a heading of the documents leads to one, as a program in the
  scratchpad checked.
- **Rule 29.** The files in git: 112 of `.c`, 83 of `.h`, 34 of `.md`, 10 of `.txt`, 4 of `.png`, 2 of `.json`, and
  `LICENSE`, `.gitignore` and `.gitattributes`. The files not yet in git are C and Markdown.
- **Requirements 3 and 4.** `.github/ACCESSIBILITY.md` ticks all of its 55 items; 32 of them end in "Human review
  needed".
- **Requirements 10 and 11.** The crapper: 903 functions of `src/` and `tools/build/`, the highest 9.6 (`bottom_box`).
  The dryer: no two functions 80% alike.
- **Requirement 12.** The mutator ran on every line of code changed on 2026-11-17, last in runs 13 and 15 of the section
  of 05:41:10.

#### How the rules were read

- **Rule 9 and the game's words.** "tech", "unit", "Castle" and "Villager" are words of the game, not of the project.
  `TERMINOLOGY.md` has used them without a line of their own since its line on the civ. Rule 9 is read as asking a line
  for each of the project's terms, each line using only the project's terms defined above it and the game's words. This
  reading changes no line.

#### Not done

- Requirements 3 and 4 rest on the 32 items of `ACCESSIBILITY.md` that wait for a human: the keyboard, a screen reader
  and the settings of Windows.
- Requirements 10, 11 and 12 were not measured on `tests/`, as the tools pass by it (question 1 of the sitrep).
- Rule 4: no workflow runs the tests on a push; they are run by hand before one, as decided.
- Rules 26, 27 and 28 could not be checked in full by reading in this run; the code changed on 2026-11-17 was read, and
  the tests of P10 passed.
- The mutator does not mutate the data files, so the change of `bonus_effects.txt` and `bonuses.txt` for bonuses 221 and
  286 was not mutated; Test 233 was seen failing when one of its lines was taken out.

#### Questions

1. **The game's words in the glossary.** Should `TERMINOLOGY.md` define the game's words, such as tech and unit, as it
   defines the project's? Proposal: no; the Glossary of the menu Help explains them to the user, and the reading above
   keeps every line as it is.

### 06:03:49: bonus 222 built, the cow trained at the Mill by a train line

#### What changed

- **Why.** Bonus 222, Cows from Mills, was skipped. The game trains the cow (unit 705) at the Town Center (109), button
  3, for every civ, and no civ of the game has it at the Mill. The original moves the cow's first train location to the
  Mill (68), button 3, and enables the cow in the civ's tech tree. A `unit` line could not do it: a unit patch stays
  within the first 157 bytes of a unit, and the train locations come after.
- **The train line.** A new kind of line in `bonus_effects.txt`, `train`, has the columns of a `unit` line, but its
  place counts from the unit's first train location. The catalog reads it as a unit patch with `in_train_location` set
  (`UnitPatch` in `catalog.h`), so it needs no list of its own. `patch_train_location` in `bonus_build.c` reads the unit
  and patches its first train location, within its 9 bytes; a unit with no train location leaves the bonus skipped, as
  the original does.
- **Data.** Two `train` lines of 222 set the cow's building to 68 and its button to 3, and a `civ` line of entry 0
  enables unit 705 from the start.
- **Tests.** 449, up from 445. Test 43 checks that a train line sets the building where a unit of the civ is trained,
  Test 44 its button, and Test 45 that a unit with no train location leaves the bonus failed, on the small dat. Test 240
  checks that the catalog reads the first `train` line of `bonus_effects.txt` as a unit patch in the train location.
  Tests 43 to 236 became 46 to 239, and Tests 237 to 445 became 241 to 449.
- **Checked.** `build.exe quick`: 449 tests, every one passed. The four were seen failing before the code was written:
  43 and 44 since the patch landed at the unit's start, 45 since nothing refused a unit with no train location, and 240
  since the file had no `train` line. With the place counted from the unit's start again, Tests 43, 44 and 45 failed;
  with a unit of no train location patched, Test 45 failed, and only it; with a `train` line read as a `unit` line, Test
  240 failed, and only it; each break was undone and the files compared with their copies. The program in the scratchpad
  built a mod from the installed game with the nine bonuses 81, 103, 221, 222, 247, 283, 286, 352 and 356: 9 built, 0
  skipped. In its dat file, civ 1 trains the cow at building 68, button 3, and tech 1511, which needs no tech, enables
  unit 705. Coverage and the crapper: 904 functions, the highest 9.6 (`bottom_box`), `patch_train_location` 3.1 and
  `add_bonus_change` 7.8. The dryer found nothing. The mutator on the changed lines of `bonus_build.c` and `catalog.c`,
  run 16: 34 mutants, 11 killed, 2 survived, 21 on lines that no test runs. The two survivors are the bound of a patch's
  place in `patch_fits`, which only a line of the catalog past the bound would reach, as question 1 of the section of
  03:54:37 asks; the lines that no test runs are each a `return false` after a failed `CHECK` of the catalog's file, a
  patch that does not fit, or a unit the civ lacks.
- **Documents.** `TESTS.md`: Tests 43, 44, 45 and 240, and the numbers of the tests after them. `MAINTAINER.md`: the
  `train` line in the table of the catalog's files, in the paragraph on `bonus_effects.txt` and in "How a mod is built";
  the paragraphs rewrapped. `TERMINOLOGY.md`: train line. `DECISIONS.md`: one decision, and the number of Test 352 in a
  decision on scrolling.

#### Not done

- The original enables the cow in the civ's tech tree; here a tech of the civ enables it at the start, as the other
  bonuses of lines do. No game was played to see the cow at the Mill.
- 13 civ bonuses are still skipped: 239, 308, 309, 310, 332, 400, 451, 454, 455, 456, 457, 459 and 460. Each needs a
  unit or a tech made anew, or a part of a unit past its train locations, such as its costs, tasks or graphics.

### 06:14:20: bonus 460 built, the Castle's costs set by cost lines

#### What changed

- **Why.** Bonus 460, Wooden Castles, was skipped. Its card gives the Castle 300 stone and 500 wood, 175 seconds to
  build and 3000 HP, where the game's has 650 stone, 200 seconds and 4800 HP. The Castle's costs come after the first
  157 bytes of a unit, which a `unit` line cannot reach.
- **The unit's parts.** A unit patch now names the part of the unit its place counts from: its start, its costs or its
  first train location (`UnitPart` in `catalog.h`, in place of the `in_train_location` of the section of 06:03:49). A
  new kind of line, `cost`, has the columns of a `unit` line, with the place counted from the unit's three costs, within
  their 18 bytes. `patch_unit_part` in `bonus_build.c`, which replaces `patch_train_location`, finds the part of the
  unit; a unit without it leaves the bonus skipped. `part_of` in `catalog.c` gives the part of each kind of line.
- **Data.** Bonus 460 has a `unit` line for the hit points (3000), a `train` line for the time its builder takes (175),
  and six `cost` lines for the type, amount and flag of its first two costs: stone 300 and wood 500, as the original
  replaces them.
- **Tests.** 451, up from 449. Test 46 checks that a cost line sets a cost of a unit of the civ, on the small dat; Test
  242 that the catalog reads the second `cost` line of `bonus_effects.txt` as a unit patch in the costs. Tests 46 to 240
  became 47 to 241, and Tests 241 to 449 became 243 to 451.
- **Checked.** `build.exe quick`: 451 tests, every one passed. Test 242 failed until the lines of 460 were in the file;
  with the costs never taken as the part, Test 46 failed, and only it; with a `cost` line read as a `train` line, Test
  242 failed, and only it; each break was undone and the files compared with their copies. With the bound of the costs
  widened to 157 bytes, no test failed: no line of the catalog is past a bound. The program in the scratchpad built a
  mod from the installed game with the ten bonuses 81, 103, 221, 222, 247, 283, 286, 352, 356 and 460: 10 built, 0
  skipped. In its dat file, the Castle of civ 1 costs 300 stone and 500 wood, takes 175 seconds and has 3000 HP; the
  Castle of civ 2 keeps the game's 650 stone, 200 seconds and 4800 HP. Coverage and the crapper: 906 functions, the
  highest 9.6 (`bottom_box`), `add_bonus_change` 7.0, `patch_unit_part` 3.3. The dryer found nothing. The mutator on the
  changed lines of `bonus_build.c` and `catalog.c`, run 17: 35 mutants, 11 killed, 2 survived, 22 on lines that no test
  runs. The two survivors are the bound of a patch's place in `patch_fits`, as in run 16; the lines that no test runs
  are each a `return false` after a failed `CHECK` of the catalog's file or a failed read of a unit, a patch that does
  not fit, or a unit the civ lacks.
- **Documents.** `TESTS.md`: Tests 46 and 242, and the numbers of the tests after them. `MAINTAINER.md`: the `cost` line
  in the table of the catalog's files, in the paragraph on `bonus_effects.txt` and in "How a mod is built".
  `TERMINOLOGY.md`: cost line. `DECISIONS.md`: one decision, and the number of Test 354 in a decision on scrolling.
- **Replaced.** The flag `in_train_location` and `patch_train_location` of the section of
  [06:03:49](#060349-bonus-222-built-the-cow-trained-at-the-mill-by-a-train-line), by `UnitPart` and `patch_unit_part`.

#### Not done

- The original also gives the Castle the look of the campaign's Wooden Fort (unit 2600): its graphics, rubble, snow and
  garrison graphics. Those lie in parts of the unit that no line reaches, and the damage graphics are a list.
- No test has a unit without costs: the small dat makes no unit of a type below 70.
- 12 civ bonuses are still skipped: 239, 308, 309, 310, 332, 400, 451, 454, 455, 456, 457 and 459.
- No game was played to build such a Castle.

### 06:18:15: bonuses 451 and 454 built from the game's techs of the Chronicles' unique units

#### What changed

- **Why.** Bonuses 451 (the Phalangite) and 454 (the Sannahya) were skipped: their lines in `bonuses.txt` named no
  techs. The game has the techs: 1290 enables the Phalangite for civ 54 and 1291 is its elite upgrade; 1327 and 1328 are
  the Sannahya's, for civ 56. Each enabling tech needs only the Castle Age and comes by itself; each elite upgrade needs
  it and the Imperial Age, at the Barracks (12) or the Stable (101). The units are in the data of every civ, disabled,
  as the original says.
- **Data.** In `bonuses.txt`, 451 names 1290 and 1291, and 454 names 1327 and 1328, which the recipe copies for the civ.
  Two `unit` lines of 454 give the Sannahya (2390) 250 HP and the Elite Sannahya (2391) 300, the Battle Elephant's,
  where the game gives 300 and 400, as the original does; they are built with the techs, since a bonus that names techs
  builds its lines too.
- **Tests.** 451, as before. No code changed.
- **Checked.** `build.exe quick`: 451 tests, every one passed. The program in the scratchpad built a mod from the
  installed game with the twelve bonuses 81, 103, 221, 222, 247, 283, 286, 352, 356, 451, 454 and 460: 12 built, 0
  skipped. In its dat file, techs 1528 to 1531 are the four copied for civ 1, the enabling ones needing the Castle Age,
  the elite ones their copy and the Imperial Age; the Sannahya of civ 1 has 250 HP and its elite 300, while the Sannahya
  of civ 2 keeps 300. No code changed, so the crapper, the dryer and the mutator were not run again.
- **Documents.** `MAINTAINER.md`: the `unit` lines of 454 and the techs of 451 and 454 in the paragraph on
  `bonus_effects.txt`. `DECISIONS.md`: one decision.

#### Not done

- The original writes names of its own for the Chronicles' units in its UI mod, and moves a unit whose button another
  unit of the building has; civbuilder does neither, so the units take the game's names and buttons.
- 455 (the Hoplite) and 456 (the Scythian Horse Archer) need their techs copied with other requirements, as the game's
  need techs of the Chronicles only; no line does that yet.
- 10 civ bonuses are still skipped: 239, 308, 309, 310, 332, 400, 455, 456, 457 and 459.
- No game was played with these units.

### 06:23:23: bonuses 455 and 456 built, the Chronicles' requirements met by a requirement tech of the civ

#### What changed

- **Why.** Bonuses 455 (the Hoplite) and 456 (the Scythian Horse Archer) were skipped, as the section of 06:18:15 said:
  the game's techs that enable them need techs of the Chronicles only.
- **How the game's techs read.** Tech 1136, which enables the Hoplite, needs 2 of the Castle Age (102) and 1138, a
  shadow tech of the Chronicles; the Elite Hoplite (1137) needs 2 of the Imperial Age (103), 1136 and 1267, so the first
  two suffice. Tech 1336, which enables the Scythian Horse Archer and disables the Cavalry Archer (39), needs 2 of the
  duplicate Castle Age (113), which every civ gets, and 1335, which no game researches; the elite (1334) needs 2 of the
  duplicate Imperial Age (115) and 1336. These techs belong to no civ, and each has a free slot.
- **Data.** Two `require` lines, of 455 on 1136 and of 456 on 1336, put the civ's copy of 638 in the free slot. Tech 638
  belongs to one civ and is researched by itself at the start, so its copy meets one of the two requirements, and the
  Castle Age the other. Bonus 81 copies the same tech, and the two share one copy. A `civ` line of 456 disables the
  Heavy Cavalry Archer (218), whose upgrade would bring the Cavalry Archer line back as the Heavy Cavalry Archer, as the
  original takes the line out of the civ's tree. The original instead copies the four techs for the civ with the
  standard ages as their requirements.
- **Tests.** 451, as before. Test 239 now counts 15 requirement lines.
- **Checked.** `build.exe quick`: 451 tests, every one passed. Before Test 239 was changed, it failed on the two new
  lines, and only it. The program in the scratchpad built a mod from the installed game with the fourteen bonuses 81,
  103, 221, 222, 247, 283, 286, 352, 356, 451, 454, 455, 456 and 460: 14 built, 0 skipped. In its dat file, techs 1136
  and 1336 list tech 1513 in their third slot, the copy of 638 for civ 1, as tech 659 of bonus 81 does; 1137 and 1334
  are as the game has them; tech 1512, which needs no tech, disables 218. No code changed, so the crapper, the dryer and
  the mutator were not run again.
- **Documents.** `TESTS.md`: Test 239. `MAINTAINER.md`: the `require` lines of 455 and 456 and the `civ` line of 456, in
  the paragraph on `bonus_effects.txt`. `DECISIONS.md`: one decision.

#### Not done

- 8 civ bonuses are still skipped: 239, 308, 309, 310, 332, 400, 457 and 459. Each needs a unit or a tech made anew, a
  train location added to a unit, or a part of a unit that no line reaches, such as its tasks.
- No game was played to train these units.

### 06:27:31: bonus 457 built, the Trebuchet's attacks multiplied by civ lines

#### What changed

- **Why.** Bonus 457, the Palintonon, was skipped. Its card gives the Trebuchet 15% more attack, and the original
  dresses the civ's Trebuchet (42) and the packed one (331) in the look of the Athenians' Trebuchet, the Palintonon, and
  names them so.
- **How the game writes it.** The game's Chinese Rocketry multiplies an attack by a change of type 5 with c 9, its d
  packed as the class of the attack times 256 plus the percent: 2941 is class 11 at 125%. The original writes one such
  change for each attack of the unpacked Trebuchet, as the packed one does not fire. In the game's data file of
  2026-11-17, the Trebuchet has two attacks: 250 on class 11 (buildings) and 200 on class 3 (pierce).
- **Data.** Two `civ` lines of 457, at the start, multiply the attacks of unit 42: d 2931 (class 11 at 115%) and 883
  (class 3 at 115%). `bonus_scale.c` already scales such a packed multiply by the bonus's multiplier.
- **Tests.** 451, as before. No code changed, and no test counts the `civ` lines.
- **Checked.** `build.exe quick`: 451 tests, every one passed. The program in the scratchpad built a mod from the
  installed game with the fifteen bonuses of the sections of today and 457: 15 built, 0 skipped. In its dat file, tech
  1513, of civ 1 and needing no tech, has the two changes: type 5, a 42, c 9, d 2931 and 883. No code changed, so the
  crapper, the dryer and the mutator were not run again.
- **Documents.** `MAINTAINER.md`: the `civ` lines of 457 in the paragraph on `bonus_effects.txt`. `DECISIONS.md`: one
  decision.

#### Not done

- The look of the Palintonon: its graphics lie in parts of the unit that no line reaches, as the Wooden Fort's look of
  460 does. The civ's Trebuchet keeps its look and its name.
- 7 civ bonuses are still skipped: 239, 308, 309, 310, 332, 400 and 459.
- No game was played to fire such a Trebuchet.

### 06:49:31: the Palintonon's look of bonus 457 given by look lines

#### What changed

- **Why.** The section of 06:27:31 left out the Palintonon's look: the original dresses the civ's Trebuchet (42) and
  packed Trebuchet (331) in the Athenians' units, as `_apply_unit_skin` does. In the game's data file of 2026-11-17, the
  Athenians (civ 47) have Trebuchets of their own look: graphics 15245, attack graphic 15243 and icon 673 for unit 42,
  where the British have 1180, 1174 and 28.
- **The look line.** A new kind of line in `bonus_effects.txt`, `look`: bonus id, unit, and the civ whose same unit's
  look it takes. The catalog keeps them as `UnitLook` in `unit_looks` (`catalog.h`). `add_id_line` in `catalog.c` reads
  a `require` line and a `look` line alike, by `read_ids`, in place of `add_requirement`.
- **The copy.** `look_copy_unit` in `look_copy.c` gives a unit the graphics, the icon and the walking and attack
  graphics of the same unit of the civ named, by `copy_unit_look`, which the monk's copy now uses as well.
  `look_dress_units` in `look_sources.c` copies the units of the look lines of each bonus that the design has, after the
  other looks (`build_looks` in `recipe.c`). A unit or civ that the game lacks is left as it is.
- **Data.** Two `look` lines of 457 name units 42 and 331 and civ 47.
- **A bug found and mended.** `catalog_load` did not set `unit_look_count` to 0, so the window, which loads the catalog
  again, read the look lines again each time, until they passed `CATALOG_UNIT_LOOK_CAPACITY` and the catalog failed to
  load: Tests 336 and 341 to 343 failed. The count is now set to 0 with the others.
- **Tests.** 455, up from 451. Test 125 checks that a look line of a chosen bonus gives its unit the look of the civ it
  names, and Test 126 that it does not when the bonus is not chosen, both on the small dat; Test 245 that the catalog
  reads the first look line of `bonus_effects.txt`, and Test 246 that it holds the 2 look lines however often it loads.
  Tests 125 to 242 became 127 to 244, and Tests 243 to 451 became 247 to 455.
- **Checked.** `build.exe quick`: 455 tests, every one passed. Tests 125 and 245 failed while `look_dress_units` did
  nothing and the file had no look line; Test 246 failed, and only it, until the count was set to 0; with the check of
  the bonus taken out of `look_dress_units`, Test 126 failed, and only it, and the file was put back from its copy. The
  program in the scratchpad built a mod from the installed game with the fifteen bonuses of the sections of today: 15
  built, 0 skipped. In its dat file, the Trebuchet of civ 1 has graphics 15245, attack graphic 15243 and icon 673, and
  the packed one 15248, walking graphic 15247 and icon 674, the Athenians'; those of civ 2 keep the game's 1180 and
  2281. Coverage and the crapper: 910 functions, the highest 9.6 (`bottom_box`), `add_id_line` 6.3, `copy_unit_look`
  6.0, `look_dress_units` 5.4. The dryer first paired `add_requirement` with the new reader of look lines (1.00); with
  the two folded into `add_id_line`, it found nothing. The mutator on the changed lines of `catalog.c`, `look_copy.c`,
  `look_sources.c` and `recipe.c`, run 18: every mutant of the new lines of `look_copy.c` and `recipe.c` was killed. In
  `look_sources.c` the two bounds of the loop survived: the count's, past which an empty line names bonus 0, which no
  design has, and the capacity's. In `catalog.c` the bound of the loop of `read_ids` survived, whose `<=` writes past
  the three ids; the loop was replaced by three reads. Run 19, on the changed lines of `catalog.c`: 8 survived, each a
  guard of `read_ids` or `add_id_line` against a line with a missing bonus, an id below 0, or a full catalog, which no
  line of the file is. The survivors on lines this change did not touch, line 71 of `recipe.c` and lines 68, 111, 155
  and 255 of `look_copy.c`, were not looked into. `build.exe` with the analyzer: 455 tests, every one passed, no
  warning.
- **Documents.** `TESTS.md`: Tests 125, 126, 245 and 246, and the numbers of the tests after them. `MAINTAINER.md`: the
  `look` line in the table of the catalog's files, the folder `src/look/`, the paragraph on `bonus_effects.txt` and the
  section on the looks. `TERMINOLOGY.md`: look line. `DECISIONS.md`: one decision, and the number of the test of
  sliding. The sitrep: the numbers of the tests it names.
- **Replaced.** `add_requirement` in `catalog.c`, by `add_id_line`.

#### Not done

- The original names the Palintonon in its UI mod; the civ's Trebuchet keeps the game's name.
- A bonus of look lines only would be counted as skipped, though its looks are copied: a look line is not counted among
  the lines built. No bonus is made of look lines only.
- The Wooden Fort's look of 460 needs a look taken from another unit (2600) and its rubble; a look line takes the same
  unit only.
- No game was played to see such a Trebuchet.

### 06:55:32: the Hoplite's tech of bonus 455 freed of its cost

#### What changed

- **Why.** This corrects the section of
  [06:23:23](#062323-bonuses-455-and-456-built-the-chronicles-requirements-met-by-a-requirement-tech-of-the-civ), which
  counted bonus 455 as built. The game's tech 1136, which enables the Hoplite, costs 1 food and is researched nowhere
  (its place is -1), and such a tech never fires, as the original's notes say: no one can pay for it. Its `require` line
  met its requirements but left the cost, so the Hoplite would not have come. The game frees it so for the Chronicles:
  its tech 1261 sets the food cost of 1136 and of 1138 to 0, by a change of type 101 (a 1136, b 0 for food, c 0 to set
  it, d 0). Techs 1336, 1290 and 1327, of bonuses 456, 451 and 454, cost nothing.
- **Data.** A `civ` line of 455, at the start, makes the change of 1261 for the civ: type 101, a 1136, b 0, c 0, d 0.
- **Tests.** 455, as before. No code changed, and no test counts the `civ` lines.
- **Checked.** `build.exe quick`: 455 tests, every one passed. The program in the scratchpad built a mod from the
  installed game with the fifteen bonuses of the sections of today: 15 built, 0 skipped. In its dat file, tech 1512, of
  civ 1 and needing no tech, has the change of type 101, a 1136, b 0, c 0, d 0. The costs of the techs were read by
  another program in the scratchpad.
- **Documents.** `MAINTAINER.md`: the `civ` line of 455 in the paragraph on `bonus_effects.txt`. `DECISIONS.md`: one
  decision.

#### Not done

- No game was played to train the Hoplite, so it is not shown that 1136 fires with its cost set to 0.
- The names of the Chronicles units: their ids, such as 405131 for the Phalangite, are in the game's
  `key-value-paphos-strings-utf8.txt`, not in `key-value-strings-utf8.txt`. Whether a game outside the Chronicles shows
  them was not tried.

#### Questions

1. **The names of the Chronicles units.** The original writes names of its own for the Phalangite, the Sannahya, the
   Hoplite, the Scythian Horse Archer and the Palintonon in its UI mod; civbuilder leaves the game's ids, whose texts
   are in the Chronicles' strings. Proposal: build one such civ and look in the game; if the names are missing, write
   them as the original does.

### 06:57:33: the techs of the bonuses read for the trap of 1136, and the state at the end of the run

#### What changed

- **Why.** The trap of 1136, a tech researched nowhere that costs something, could lie in any tech a bonus copies or
  gives a requirement to.
- **The techs read.** The 375 techs that `bonuses.txt` names or a `require` line gives a requirement to were read from
  the game's data file, by a program in the scratchpad, for a place of -1 and a cost above 0. Six were found: 1136, now
  freed of its cost; 699 to 702, the Lithuanians' relic bonuses, whose cost is the relics held (resource 7), as the game
  fires them; and 791, the Poles' Winged Hussar of a start after the Imperial Age, which costs 1 food as the game's
  Poles have it, and which bonus 282 copies with 789. None of the five was changed.
- **The survivors of `look_copy.c` read.** Of the survivors on lines the section of 06:49:31 did not touch: line 111
  takes the smaller of two counts of tasks, and `<=` there gives the same count, so that mutant cannot be killed; line
  68 is the guard of a part that only one of two units has, line 155 the guards of a unit's number, and line 255 the
  bound of the loop over the units, which the guard of line 155 catches again. Each is a check of P10, as the log read
  such survivors at 04:39:06 of 2026-11-16.
- **Tests.** 455, as before. Nothing changed in the project but this log.
- **Checked.** `build.exe` with the analyzer, at 06:49: 455 tests, every one passed, no warning; `build.exe quick` after
  the last change of the data: 455 tests, every one passed. The crapper: 910 functions, the highest 9.6. The dryer: no
  pair alike.

#### Not done

- 7 of the 394 civ bonuses are still skipped: 239 and 400 (City Walls: new techs, and a train location added to a unit),
  308, 309 and 310 (Imperial Scorpions, Royal Battle Elephants, Royal Lancers: new units), 332 (Feudal Knights: a new
  unit) and 459 (work rates near Castles and towers: tasks of a unit, which no line reaches).
- The Wooden Fort's look of 460: the original copies some fields of another unit (2600) and its building part; a look
  line copies the same unit only.
- No mod built by civbuilder with today's bonuses was tried in the game.
- Questions 1 to 11 of the sitrep, and Question 1 of 06:55:32, are open.

### 07:05:37: a look line gives its unit the timing of its attack too

#### What changed

- **Why.** The original copies two more fields of the Palintonon: the frame at which its attack lets go of the stone
  (`frame_delay`) and the place it lets go from (`graphic_displacement`). In the game's data file of 2026-11-17 the
  Athenians' Trebuchet lets go at frame 7 from a height of 4.25, the British one at frame 24 from 9.5; with the
  Palintonon's attack graphic and the Trebuchet's timing, the stone would leave out of step with the picture.
- **The copy.** `copy_attack_timing` in `look_copy.c` copies those two fields, the 14 bytes that end 9 bytes before the
  attack graphic, for a look line only (`look_copy_unit`); the monk's copy is as before. `timing_of` gives their place,
  or none for a unit without an attack.
- **Tests.** 456, up from 455. Test 127 checks, on the small dat, that a look line gives its unit the frame delay of the
  unit it copies. Tests 127 to 455 became 128 to 456.
- **Checked.** `build.exe quick`: 456 tests, every one passed. Test 127 failed before the copy was written, and again,
  and only it, with the copy skipped; the file was put back from its copy. The program in the scratchpad built a mod
  from the installed game with the fifteen bonuses of today: 15 built, 0 skipped; in its dat file, the Trebuchet of civ
  1 lets go at frame 7 from 4.25, and that of civ 2 at frame 24 from 9.5. Coverage and the crapper: 912 functions, the
  highest 9.6, `look_copy_unit` 6.0. The dryer found nothing. The mutator on the changed lines of `look_copy.c`, run 20:
  12 mutants, 6 killed, 1 survived, 5 on lines that no test runs; the survivor is the guard of `timing_of` against a
  place before byte 23, which no unit with an attack has.
- **Documents.** `TESTS.md`: Test 127, and the numbers of the tests after it. `MAINTAINER.md`: the section on the looks.
  `DECISIONS.md`: one decision, and the number of the test of sliding. The sitrep: the numbers of the tests it names.

#### Not done

- The original also copies a building's construction graphic for a skin; no look line is of a building.

The day 2026-11-17 goes on in [part 7](DEVLOG7.md#2026-11-17).
