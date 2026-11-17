# Development log, part 5

- [2026-11-16](#2026-11-16)

Part 5 of the development log. It follows [part 4](DEVLOG4.md) and goes on in [part 6](DEVLOG6.md). The decisions
made in the log are listed in [DECISIONS.md](../DECISIONS.md).

## 2026-11-16

### 00:01:19: mutants made by hand on copies of the tree, and the run stopped by the usage limit

#### What changed

- **Why.** The mutator refuses to run while Test 253 fails on the cap of 100 tests, so Rule 34 had no measure.
- **A sweep outside the project.** Nothing in the project changed. In the session's scratchpad, `sweep/sites.exe`
  lists the sites that `mutant_next` finds in every function of `src/` but `src/win/`: 3937. Four copies of the
  working tree, each built and each failing only the cap, take a quarter of the sites each: a site is changed,
  its file compiled, the test program linked and run without the window, and the names of the failing tests
  compared with those of the unchanged copy. A mutant that changes nothing in that list has survived (`S`); the
  other letters are `K` killed, `N` not compiled, `T` ended after 60 seconds.
- **Where the results are.** `sweep/result_1.csv` to `result_4.csv`: fate, file, line, function, place, length
  and what was put there. The sweep was running when this section was written; 50 sites were done.
- **Documents.** `DELETION_REQUESTS.md`: what the run left in the scratchpad and in `/tmp`.

#### Not done

- The results of the sweep were not read, and no survivor was killed. The first mutants, in `mod_write_info` of
  `mod_ui.c`, survived.
- The window does not reflow: 1.4.4 Resize Text and 1.4.10 Reflow of `.github/ACCESSIBILITY.md` are unticked, so
  Requirement 3 is not met. Only `layout.c` was read for it.
- `MAINTAINER.md`, `TERMINOLOGY.md`, `sitrep.md` and `README.md` do not yet tell of this run's new files and
  folders (`tests/suites/`, `tests/core/`, `tests/look/`, `tests/places/`, `tests/model/`, `tests/shell/`,
  `tests/rules/gate_tests.c` and `tool_tests.c`, `src/shell/`, `mod_ui.c`, `unique_tech.c`, `dry_report.c`), nor
  of how a test is added to a suite. `AGENTS.md`, the introduction of `TESTS.md`, `MAINTAINER.md` and two
  skills still say that there are at most 100 tests.
- `build.exe` with the analyzer was not run after this run's changes.
- 27 functions of the tools are at CRAP 10 or above, as the section of 23:56:59 on 2026-11-15 says.
- The features of the original that Build skips are as they were: 22 civ bonuses, the Royal tiers, the bonus
  multiplier, custom unique units, language and voices, and a hero's description, flags and overrides.

#### Questions

1. **The cap of 100 tests.** `TEST_LIMIT` in `tests/rules/log_rules.c` is 100 and there are 285 tests, so Test
   253 fails, the full build cannot pass and the mutator cannot run. Proposal: raise it by hand, or take it
   out, since no rule now names a number of tests.

### 00:53:27: the sweep by hand ended: 2001 mutants killed, 1923 survived

#### What changed

- **The sweep ended.** Of 3937 mutants of `src/` but `src/win/`: 2001 killed, 1923 survived, 9 ended after 60
  seconds, 4 not compiled. Nothing in the project changed.
- **What the number does not say.** Unlike the mutator, the sweep did not leave out the lines that no test runs,
  and it ran the tests without the window, so it counts as survivors the mutants of a return behind a `CHECK`,
  of the second bound of a loop (Rule 26), and of `src/view/` and `src/shell/`, which the tests of the window
  run. The share that the mutator itself would report as survivors is not known.
- **Where most survived.** `catalog.c` 173, `recipe_tree.c` 101, `tsv.c` 96, `layout.c` 79, `recipe_copy.c` 79,
  `inflate.c` 74, `look_copy.c` 74, `json_reader.c` 70, `mod_strings.c` 68, `recipe.c` 67, `dat_skip.c` 63.
  `sweep/survivors_by_swap.txt` in the session's scratchpad counts them by what was put in.

#### Not done

- No survivor was read or killed. Rule 34 and Requirement 12 are not met, and not measured by the project's
  own tool.

### 02:13:51: Rules 31, 32 and 34 measured by the original tools only, and no pair alike in src

#### What changed

- **Why.** It was decided that only the original crapper, dryer and mutator are used from now on: the three
  projects that lie beside this one, in `..\crapper`, `..\dryer` and `..\mutator`. The folders `tools/crap/`,
  `tools/dry/` and `tools/mutate/` were taken out of the working tree.
- **What the original tools read.** They read `src/` only: folders named `tests` and `build` are passed by,
  so neither the tests nor `tools/build/` is measured. They write `.metrics/` and `target/` in the project's
  folder; both were added to `.gitignore`.
- **The build tool builds three programs.** `source_list.c` and `build_steps.c` name the application, the
  tests and the build tool; the crapper, the dryer and the mutator are gone from `SourceProgram`,
  `PROGRAM_PREFIXES`, `RESPONSES` and `LINKS`.
- **The original dryer at 0.8.** `..\dryer\dryer --threshold 0.8` gave 50 pairs in `src/`, nearly all of them
  functions of one line. It gives none now:
  - eight one-line names for `check_is_among` (`is_age`, `is_space`, `is_bracket`, `is_building_class`,
    `is_mobile_class`, `ends_quoted`, `ends_a_sentence`, `is_stat_change`) are gone; their callers ask
    `check_is_among` with the table;
  - `catalog_find` is the one lookup by table, kind and id; `catalog_find_unit`, `_tech` and `_tree` are gone,
    and `CatalogTable` is in `catalog.h`;
  - `text_starts_with` holds the loop that `matches` held, and `text_equal` is true when each text starts with
    the other;
  - `binding_of` is public and gives a `Binding`; `binding_text_field` and `binding_source` are gone;
  - `pick_kind` says whether a list's source is a unit or a tech, in place of `is_unit_source` and
    `is_tech_source`; `is_filter` of `sync.c` is written where it was asked;
  - `sync_design` takes whether the lists are filled again, in place of `sync_all` and `sync_model`;
  - `wizard_page` gives a step's title and help from one table, in place of `wizard_title` and `wizard_help`;
  - `layout_is_focusable` and `layout_has_label` ask `layout_spec`, which checks the control itself;
  - `put_letters` writes a text escaped or plain, for `put_string` and what `put_text` wrote; `put_escape`
    writes a backslash and a letter;
  - `catalog_category_name` checks its category instead of asking `check_index`.
- **Tests.** 272, down from 285. The 13 tests of the removed tools are gone: the eight of the old Tests 256 to
  263 (the CRAP score, complexity, likeness, mutants, coverage), 272 to 274 (the dryer and the crapper on a
  fixture) and 278 and 279 (what of a tool is built into the tests). The gate tests are 256 to 263, the
  sources of the build 264 to 266, its plan 267 to 272. The last now asks about the build tool, not the dryer.
- **Checked.** `build.exe quick`, with `build.exe` built anew: 272 tests run, every one passed but the cap (Test
  253). `..\dryer\dryer --threshold 0.8`: no duplicate candidates found. The table of contents of `TESTS.md`
  agrees with the 272 names the tests print.
- **Measured.** `..\crapper\crapper --no-coverage`: 746 functions in `src/`; 29 have a complexity of 10 or more
  (the highest is 12), 34 of 9, 73 of 8, 102 of 7.
- **Documents.** `TESTS.md`. `AGENTS.md`: how Rules 31, 32 and 34 are measured. `DECISIONS.md`.
  `DELETION_REQUESTS.md`: two fixtures that nothing reads any more.
- **Replaced.** The crapper, the dryer and the mutator in C, of
  [2026-11-10 04:22:28](DEVLOG3.md#042228-the-crapper-in-c-and-369-functions-at-crap-10-or-above),
  [04:27:23](DEVLOG3.md#042723-the-dryer-in-c-and-62-pairs-of-functions-alike) and
  [04:37:03](DEVLOG3.md#043703-the-mutator-in-c-tried-on-one-folder). With them goes the reading that a `CHECK`
  is no branch: the original crapper counts the `if` and each `||` of a guard, and the original mutator
  mutates it.

#### Obstacles met, and how they were solved

- **No CRAP score yet.** The original crapper reads coverage from `target/coverage/c/lcov.info`, which gcovr
  makes from a build with `--coverage`. gcovr is not installed, and installing it was not permitted to this
  run. Until it is there, only the complexity is known.
- **No mutant yet.** The original mutator runs the tests first and stops if they fail; Test 253 fails on the
  cap of 100 tests. It also needs a test command for a project without CMake or a makefile.

#### Not done

- The CRAP scores of `src/` by the original crapper, and the mutants by the original mutator.
- 29 functions have a complexity of 10 or more, so a score of 10 or more whatever their coverage.
- The numbers of the earlier sections of this part, and the sweep by hand of 00:53:27, were made with tools
  that are no longer used; they are not the measure of Rules 31, 32 and 34.
- `MAINTAINER.md` and `TERMINOLOGY.md` still tell of the tools in C.

#### Questions

1. **gcovr.** Should gcovr be installed (`pip install gcovr`), so that the original crapper and mutator have
   coverage? Proposal: yes, in the environment of `..\crapper`.
2. **The mutator's test command.** The original mutator needs `--test-command` here. Proposal:
   `build.exe quick`, once the cap no longer fails it; each worker then builds the whole project once.
3. **Tests and the build tool outside the measure.** The original tools pass by `tests/` and `tools/build/`.
   Proposal: accept it, as ub-mode measures the code that is shipped.

### 02:23:39: no function of src with a complexity of 10 or more, by the original crapper

#### What changed

- **Why.** The original crapper counts every `if`, loop, `case`, `default`, `?:`, `&&` and `||`, those of a
  `CHECK` among them, so 29 functions of `src/` had a complexity of 10 to 12 and would score 10 or more
  whatever the tests run.
- **The 29 functions divided.** Each gave a part of its decisions to a function of its own: `add_record`
  (`add_choice_record`), `add_appearance` (`read_looks`), `add_unit` (`role_of`, `read_stats`),
  `dat_map_read` (`map_head`, `map_tables`, `map_tech_tables`), `dat_list_units` (`place_units`),
  `patch_unit` (`patch_fits`), `set_costs` (`any_cost`, `clear_costs`), `json_patch_replace` (`has_room_for`,
  `span_of`), `json_patch_write` (`put_edit`, `put_rest`), `prefix_letter` (`is_folder_letter`),
  `forge_build` (`build_mods`), `make_codes` (`fill_symbols`, one loop for the lengths and the distances),
  `tsv_number_list` (asks `has_field`), `tsv_decimal` (`take_decimal`), `range_fits` (`is_clear_of`,
  `counts_are_clear`), `output_copies` (`copy_lands`), `frame_box` (`bottom_box`), `details_of`
  (`add_picked_tech`), `inflate_block` (`inflate_fixed`), `huffman_build_fixed` (three small tables in place
  of four choices), `count_lengths` (`lengths_fit`), `deflate_store` (`store_fits`), `json_copy_string`
  (`copy_letters`), `decode_escape` (`control_of`), `hero_find`, `note_unit_change` (`changed_unit`,
  `is_in_tree`), `emit_changes` (`is_disabled`), `place_of` (`place_in`) and `name_units`.
- **One check of a civ's place.** `dat_map_has_civ` in `dat_map.c` holds the three checks that `patch_unit`,
  `hero_find`, `name_units` and `list_both` each made of a civ's number.
- **Functions that are mostly checks.** A branch of a `CHECK` that fails is never run, so a function with
  five or more of them cannot be covered enough. Twelve such functions gave their checks to a part that is a
  chain of at most four: `edit_fits` (`counts_match`), `dat_edit_add_tech` (`tech_fits`), `output_patch`
  (`patch_lands`), `dat_edit_start` (`map_is_in_order`), `dat_edit_add_change` (`count_change`), `copy_bytes`
  (`copy_is_inside`), `civ_design_set_choice` (`is_choice_value`), `map_techs` (`read_tech_count`) and
  `sink_put` (`sink_has_room`) among them.
- **Tests.** 272, unchanged.
- **Checked.** `build.exe quick` after each of the four steps: 272 tests run, every one passed but the cap (Test
  253); "at least two checks per function" passes.
- **Measured.** `..\crapper\crapper --no-coverage`: 789 functions in `src/`, none with a complexity of 10 or
  more; 29 of 9, 84 of 8, 117 of 7. `..\dryer\dryer --threshold 0.8`: no duplicate candidates found.
  `..\mutator\mutator --scan src/core` lists the sites of `src/core/` without running a test.
- **Documents.** `DECISIONS.md`.

#### Not done

- No CRAP score: coverage needs gcovr, as the section of 02:13:51 says. A function of complexity 9 needs
  about 77% of its branches run to score under 10, one of 8 about 69%, one of 7 about 61%.
- No mutant: the cap of 100 tests fails the tests that the original mutator runs first.

### 02:26:03: the guide, the glossary and the sitrep tell of ub-mode's tools and of the new folders

#### What changed

- **`MAINTAINER.md`.** The table of files and folders has `src/shell/`, `mod_ui.c`, `unique_tech.c` and what
  `tools/build/` holds now, and no longer the three folders of tools in C. The chapter on tests has a table of
  the test folders and the steps to add a test to a suite. The rules for changing the code say how Rules 31,
  32 and 34 are measured, and that Test 253 fails while there are more than 100 tests. `sync_design` and
  `LIBRARIES` take the place of names that are gone. The diagram has `src/shell`.
- **`TERMINOLOGY.md`.** Changed: complexity, crapper, shape, dryer, mutator. Added: suite, group, fixture, gate
  test, script. The three programs are called ub-mode's, since "original" is the term for aoe2-empire-forge.
- **`AGENTS.md`.** The same name for the three programs.
- **`sitrep.md`.** A new sitrep at its top: what is new, what is not done, six questions, how to test.
- **Checked.** `build.exe quick`: 272 tests run, every one passed but the cap (Test 253).
- **Documents.** Those above.

#### Not done

- `AGENTS.md`, the introduction of `TESTS.md` and the skill `tests-doc` still say that there are at most 100
  tests; they follow the test that still enforces it.
- `README.md` was not read again.

### 02:45:16: a window too small for the columns shows a step in one column and scrolls

#### What changed

- **Why.** 1.4.4 Resize Text and 1.4.10 Reflow were the two unticked boxes of the checklist: at 200% the least
  window was 1920 by 1120 pixels, and a window could not be narrower than 960 units, so Requirement 3 was not met.
- **A step in one column.** `src/reflow/reflow.c`, a new folder without Win32, places a step for a window that
  is narrower than 960 or shorter than 560: the list of steps, the title, the help, the step's controls in the
  order of the Tab key, the status line, and Back and Next side by side, each as wide as the window.
  `reflow_height` gives the height of a step so placed, and three functions say how far the window may scroll and
  how far it must to show a box.
- **The window scrolls.** `controls_place` chooses the wide layout or the one column, and moves the controls up by
  `app->scroll`. `assist_scroll_range` sets the scroll bar; `assist_scroll` answers the bar, the wheel and two
  new commands of the menu Go, Scroll up and Scroll down, with Ctrl+Up and Ctrl+Down; `assist_follow_focus`
  scrolls the control in focus into view. A change of step starts at the top. The window may be as small as 320
  by 256 units.
- **Rows that wrap.** The lists have rows of their own heights (`LBS_OWNERDRAWVARIABLE`). `assist_wrap_rows`
  measures each row for the list's width when a list is filled and when the window changes size, and
  `assist_row_format` draws a row's text on as many lines as it needs, centred when it needs one. Before, a
  row too long for its list ended in three dots, in the wide layout as well.
- **The whole window is drawn again when its controls are placed** (`assist_placed`), since parts of the focus
  frame stayed behind when the controls moved.
- **Tests.** 293, up from 272. Tests 220 to 232: when a window reflows (3), the one column at a width of 320
  (4), and scrolling (6). Tests 249 to 256 drive the real window made narrow and short: one column, the focus
  in view, the least size, Scroll down, Scroll up, the wheel, a long row and a short row. The tests of the
  window that were 220 to 235 are 233 to 248, and those of maintainability start at 257.
- **Checked.** `build.exe quick`: 293 tests run, every one passed but the cap (Test 274 now). Fourteen faults
  were made on purpose and undone, and each failed a test: the scroll not taken off a control's place (250), the
  one column never chosen (250, 252, 254), Scroll up and Scroll down exchanged (252, 253), the least width left
  at 960 (249, 251), the wheel turned round (254), the focus not followed (250), a page up made a page down
  (253), Next not moved beside Back (224), a window of 960 called too narrow (222), a margin taken off a step's
  height (226), the two comparisons of `reflow_scroll_to_show` each turned (227 to 229), a row never higher than
  one line (255) and a row always higher (256).
- **Looked at.** `build\civbuilder.exe` was started and its window made 440 and 360 pixels wide and 1240 wide, on
  the Identity and Civ bonuses steps, at 100%: one column with a scroll bar when narrow, three columns when
  wide, rows on two and three lines, Back and Next side by side. The bits of the focus frame left behind were
  seen there first.
- **Measured.** `..\crapper\crapper --no-coverage`: 815 functions, the highest complexity 9.
  `..\dryer\dryer --threshold 0.8`: no duplicate candidates found.
- **Documents.** `.github/ACCESSIBILITY.md`: 1.4.4 and 1.4.10 ticked, each to be reviewed by a human; all 55
  boxes are ticked, 31 of them so marked. `TESTS.md`: the 21 tests and the new numbers. `MAINTAINER.md`: a
  section "The narrow window", `src/reflow/` and `tests/reflow/`. `TERMINOLOGY.md`: wide layout, reflow.
  `sitrep.md`. `DECISIONS.md`.

#### Not done

- The window was not tried at a text size of 200% nor at a scale of 400%, with a screen reader, or with the
  keyboard by hand.
- A row of a list can be at most 255 pixels high, a limit of Windows; at the largest scales a row of many lines
  is cut.
- The text beside a list is 132 units high in the one column and does not scroll; a long summary, such as that
  of 24 chosen bonuses, is cut there. In the wide layout it is cut too once it passes the column's height.
- Ctrl+Up and Ctrl+Down now scroll the window wherever the focus is.

### 02:52:16: the analyzed build clean, and ub-mode's mutator able to run on tests without the rules

#### What changed

- **Why.** The analyzed build had not been run since the day before, and ub-mode's mutator could not run: it runs
  the tests first, in the project and then in each worker's copy of it, and stops if they fail.
- **The analyzed build is clean.** `build.exe` found one fault: `objects_are` of `tests/rules/tool_tests.c` left
  a file open when a check failed. It is now `write_objects` and `read_objects`, each closing what it opens, and
  a file that did not open is tested plainly, as the analyzer cannot see through a `CHECK`. The same was done in
  `write_response_file` of `build_steps.c`.
- **Tests without the rules.** `build\tests.exe norules` leaves out the maintainability tests, as `nowindow`
  leaves out those of the real window; `read_arguments` of `runner.c` reads both words in any order.
  `build.exe` passes them on (`test_arguments` in `build.c`), as in `build.exe quick norules`.
- **Why the mutator needs that.** A worker of the mutator is a copy of the tree under `target/`, which Git
  ignores, so `git ls-files` lists nothing there and every test that walks the project's files fails; the
  tests of the documents say nothing about a mutant of `src/` either. Without them the tests pass, in the
  project and in a worker, whatever the cap of 100 tests says of `TESTS.md`.
- **The mutator's command.** From the project's folder:
  `..\mutator\mutator --no-coverage --test-command ".\build.exe quick nowindow norules" src/<folder or file>`.
  `--no-coverage` runs the sites of lines that no test runs as well, since there is no coverage report;
  `.\` is needed, as the mutator's shell does not look for a program in the folder it is in. For the folders of
  the window the command is `.\build.exe quick norules`, so that the tests of the real window judge them.
- **Tests.** 293, unchanged. No test was written for the two words: a test cannot ask the program that runs it
  to leave tests out (Rule 33).
- **Checked.** `build.exe`: no warning and no finding of the analyzer; 293 tests run, every one passed but the
  cap (Test 274). `build.exe quick norules`: 256 tests run, 0 failed, "Build and tests passed.".
  `build.exe quick nowindow norules`: 209 tests run, 0 failed. The mutator on `src/view/wizard.c`, with 4
  workers and without the window: 66 mutants in one minute, 12 killed and 54 survived.
- **Documents.** `MAINTAINER.md`: the two words, and the mutator's command. `AGENTS.md`: the same command.
  `sitrep.md`. `DECISIONS.md`.

#### Not done

- The survivors: `pick_is_made`, `tree_is_chosen` and `wizard_step_is_done` of `wizard.c` are run only by the
  tests of the real window, which that run left out.
- The mutator on the rest of `src/`.

### 04:39:06: ub-mode's mutator run and read: three survivors in four are made by P10's checks and bounds

#### What changed

- **Why.** ub-mode's mutator was run for the first time, and its survivors were read.
- **What it measured first.** With the tests of the real window, 6 workers: `src/view/`, 384 mutants, 181
  killed and 203 survived, in 58 minutes; `src/reflow/`, 164 mutants, 88 killed and 76 survived, in 24 minutes.
  The tests of the window are slow when several run at once, so the other folders are run without them.
- **What the survivors are.** The survivors of `src/core/`, `src/model/` and `src/base/` (293) were sorted by
  the line they are on: 114 on a line with a `CHECK`, 50 on the `return` right under such a guard, 42 on a `for`
  line with its fixed bound, 13 on a declaration with a starting value, 74 elsewhere. Three in four are made
  by what P10 asks for: a guard only fails on arguments that no caller passes, and the second bound of a loop
  is never the one that ends it, so no test that calls the code as its user does can tell the mutant.
- **Real gaps closed.** The rest showed tests that were missing:
  - `wizard_step_is_done` was run only through the real window. Eleven tests in `tests/model/wizard_tests.c`:
    no step of a new design is done, each of eight steps is done once its choice is made, a pick finishes only
    its own step, and Back from the second step goes to the first. `tree_is_chosen` is gone;
    `wizard_step_is_done` asks `civ_design_tree_is_empty`, which decided the same.
  - the one column was never asked where exactly a control is. Four tests: the first control of a step comes
    under the help, Back and Next come under the status line, a control starts a margin from the left, and
    Next ends a margin from the right.
  - the undo history had three tests. Eleven more in `tests/model/history_tests.c`: nothing to undo or redo on
    a new design, a failed undo leaves nothing to redo, a change made is reported, a change after an undo and
    a design put in place of another leave nothing to redo, undo goes back 32 changes and keeps the newest,
    a replaced design can be undone, and typing joins neither a tick nor typing that was undone.
- **Tests.** 319, up from 293: Tests 191 to 201 (the steps), 202 to 212 (the history) and 255 to 258 (places in
  the one column). The tests after them moved up; the cap is Test 300 now.
- **Checked.** `build.exe quick`: 319 tests run, every one passed but the cap. Mutants made by hand, each
  undone: nine in `reflow.c` and `wizard.c`, one for the step back, and eleven in `history.c`; every one failed
  a test, the last after the test of the depth was given numbered changes in place of a tick put on and off.
  The mutator again, without the window: `wizard.c` 42 killed and 13 survived (12 and 54 before), `reflow.c`
  94 and 70 (88 and 76 before). `..\crapper\crapper --no-coverage`: 814 functions, the highest complexity 9.
  `..\dryer\dryer --threshold 0.8`: no duplicate candidates found.
- **Documents.** `TESTS.md`: the 26 tests and the new numbers. `DECISIONS.md`.

#### How the rules were read

- **Rule 34 and Rule 26.** Rule 34 asks that every mutant of code that the tests run is killed; Rule 26 asks
  for P10 unless it contradicts ub-mode. P10's two checks a function (1682 in `src/` and `tools/build/`) and
  its fixed bound on every loop each leave mutants that cannot be killed. Nothing was taken out: the tests
  "at least two checks per function" and "every loop has a fixed upper bound" still hold P10, and which of
  the two gives way is a decision for the chief developer (question 1).

#### Not done

- The survivors of `src/view/` other than `wizard.c`, of `src/reflow/` (70, nearly all in guards) and of the
  other folders were not killed.
- The mutator's run over the other folders of `src/` was still going when this section was written; it runs
  on a copy of the tree in the session's scratchpad, so its snapshots are not in `.metrics/`.

#### Questions

1. **P10 or the mutator.** Should the code keep a `CHECK` and a second bound that no test can make fail, and
   Rule 34 count only the mutants outside them; or should they go, with the two rule tests that ask for them?
   Proposal: keep P10 and read Rule 34 as "every mutant outside a guard, the return under it and a loop's fixed
   bound", since a check that cannot fail in a test is what P10 means by an assertion.
2. **How long the mutator may take.** With the tests of the real window a folder takes up to an hour. Is a
   run without them enough for the folders that are not the window's? Proposal: yes.

### 04:47:25: fourteen tests for survivors of the text helpers and the design

#### What changed

- **Why.** Among the survivors of `src/core/text.c` and `src/model/civ_design.c` that are on no guard, bound or
  starting value, eleven showed behaviour that no test asked about.
- **Text.** `tests/core/text_tests.c`, eight tests: a text is not equal to a longer one that starts with it,
  nor to its own beginning; a text that fills its place is copied and one a letter longer is not; a text that
  does not fit is not appended; a negative number is appended with its minus sign; a capital Z is found by a
  small z; a text does not contain a letter it lacks.
- **The design.** `tests/design/design_tests.c`, a new folder since `tests/model/` holds six files: a ticked
  bonus and a picked team bonus count once; a bonus ticked twice and a node chosen twice are not chosen; taking
  a node out keeps the other nodes; a tree takes no more nodes than it holds.
- **Tests.** 333, up from 319: Tests 157 to 164 and 221 to 226. The tests after them moved up; the cap is
  Test 314.
- **Checked.** `build.exe quick`: 333 tests run, every one passed but the cap. Eleven mutants made by hand in
  `text.c` and `civ_design.c`, each undone, each failed a test; the one of `remove_pick` failed the test of the
  history's depth. The crapper: 814 functions, the highest complexity 9. The dryer: no duplicate candidates.
- **Documents.** `TESTS.md`.

#### Not done

- The survivors of `text.c` at the edges of UTF-8 and UTF-16, and those of `check.c` and `file_io.c`.

### 05:00:10: thirteen tests for survivors of the civ file and of JSON, and a saved civ that sets everything

#### What changed

- **Why.** The survivors of `src/persist/` and `src/json/` that are on no guard, bound or starting value showed
  what the tests never looked at.
- **The civ of the round trip sets everything.** `design_a_civ` of `civ_file_tests.c` left most choices and texts
  as a new civ has them, so a reader that lost them was not seen. It now sets all eight texts, all eight
  choices, a node of each kind, both unique techs with a cost and a time, and the test "a saved civ loads back
  unchanged" catches a choice or text that is not read.
- **The saved text.** `tests/design/saved_tests.c`: a unit of the game is saved with no custom unit's index, a
  custom unit with no id of the game, and the language of the Britons as index 0.
- **The writer.** `tests/core/writer_tests.c`: two members are written with a comma between; the first member of
  an object inside another has no comma; an object left open is not a finished text; a text too long for the
  writer is reported. Before, no test would have seen a civ file without its commas, since the project's own
  reader reads one.
- **The edges of escapes.** Six tests at the end of `tests/core/read_tests.c`: the hex digits a, f and F, the
  first letter of two bytes, of three bytes and beyond the basic plane, and the last letter of Unicode.
- **Tests.** 346, up from 333: Tests 156 to 161, 171 to 174 and 233 to 235. The cap is Test 327.
- **Checked.** `build.exe quick`: 346 tests run, every one passed but the cap. 22 mutants made by hand in
  `civ_file_write.c`, `civ_file_read.c`, `json_writer.c` and `json_string.c`, each undone: 21 failed a test.
  The one that did not gives a tech without a time the time 1 in place of 0, which only a file without that
  key would show.
- **Documents.** `TESTS.md`.

#### Not done

- The survivors of `json_reader.c`, which are nearly all at the ends of lists that a well-formed text never
  reaches.

### 05:11:38: the catalog files read by tests of their own

#### What changed

- **Why.** Without the tests of the window and of the documents, `split_fields`, `tsv_open`, `tsv_number` and
  `tsv_number_list` killed no mutant, and `catalog.c` few: the catalog was read only by tests that open the
  window, and a line of a file only by tests that read documents without looking at fields.
- **Lines of a file.** `tests/data/lines_tests.c`, eight tests on a file of two lines written under `build/`: its
  fields, a number, a text, a list of numbers, a list of slots as bits, the second line after a line end of
  Windows, the end of the file, and a file that is missing.
- **The catalog.** `tests/data/catalog_tests.c`, nine tests on `data/`: the 394 bonuses, 80 team bonuses, 146
  units, 120 techs and 205 nodes of the files, the first unit's name, speed in hundredths and role, and a
  folder that is missing.
- **Tests.** 363, up from 346: Tests 175 to 191. The cap is Test 344.
- **Checked.** `build.exe quick`: 363 tests run, every one passed but the cap. Seven mutants made by hand in
  `tsv.c` and `catalog.c`, each undone: five failed a test, and the test of the role was written for the
  sixth. The seventh makes the last field of a line longer than the line, which reads the same up to the
  line's end.
- **Documents.** `TESTS.md`.

### 05:41:51: all of src measured by the mutator: 8955 mutants, 4754 killed

#### What changed

- **The mutator on every folder of `src/`.** Nothing in the project changed. The run of 20 folders was made
  on a copy of the tree as it was at 04:15, in the session's scratchpad, without the tests of the real window,
  with 10 workers, in 86 minutes; `src/view/` and `src/reflow/` were run in the project, as the sections above
  say.

  | Folder | Killed | Survived | | Folder | Killed | Survived |
  | --- | --- | --- | --- | --- | --- | --- |
  | `core` | 237 | 116 | | `forge` | 593 | 385 |
  | `model` | 249 | 156 | | `bonus` | 95 | 47 |
  | `base` | 35 | 21 | | `unique` | 196 | 105 |
  | `json` | 418 | 234 | | `look` | 210 | 87 |
  | `persist` | 109 | 102 | | `hero` | 84 | 44 |
  | `data` | 193 | 386 | | `civlist` | 82 | 30 |
  | `pack` | 266 | 131 | | `techtree` | 75 | 13 |
  | `deflate` | 126 | 104 | | `present` | 236 | 213 |
  | `dat` | 684 | 274 | | `shell` | 13 | 101 |
  | `mod` | 486 | 251 | | `win` | 92 | 1128 |
  | `view` | 181 | 203 | | `reflow` | 94 | 70 |

  In all 8955 mutants, 4754 killed and 4201 survived, 53%. Without `win` and `shell`, whose code only the tests
  of the real window run, 4649 of 7621 were killed, 61%.
- **What the numbers do not say.** The run had no coverage report, so it also made the mutants of lines that no
  test runs. The folders other than `view` and `reflow` were measured before the 55 tests of 04:39:06 to
  05:11:38 that concern them were written, so `core`, `model`, `persist`, `json` and `data` stand better now.
  Of the survivors sorted so far (1524, of nine folders), 676 are on a line with a `CHECK`, 191 on the `return`
  under one, 197 on a loop's fixed bound, 39 on a starting value and 421 elsewhere.
- **Checked.** `build.exe`, with the analyzer: no warning and no finding; 363 tests run, every one passed but
  the cap (Test 344). The crapper: 814 functions, the highest complexity 9. The dryer: no duplicate candidates.
- **Documents.** `sitrep.md`: a new first sitrep in place of that of 02:25, with the table's totals, six
  questions and how to test. `DELETION_REQUESTS.md`: the copy of the tree and what the mutator printed.

#### Not done

- 4201 mutants survive (Rule 34, Requirement 12), fewer by those the 55 later tests kill. The survivors of
  `src/win/`, `src/shell/` and `src/present/` were not judged with the tests of the real window.
- The CRAP scores (Rule 32, Requirement 10): no coverage report without gcovr. No function has a complexity of
  10 or more.
- The window at 200% and 400%, a screen reader, the dialogs by hand, the mod in the game.
- The features of the original that Build skips: 22 civ bonuses, the Royal tiers, the bonus multiplier, custom
  unique units, language and voices, and a hero's description, flags and overrides.
- `AGENTS.md`, the introduction of `TESTS.md` and the skill `tests-doc` still say at most 100 tests.

#### Questions

1. **The open questions are the six of `sitrep.md`:** gcovr, P10 or the mutator, the cap of 100 tests,
   Requirement 4 and AAA, pairs alike by form only, and `tests/` and `tools/build/` outside the measure.

### 06:14:25: four folders measured again after their tests, and a mutator that stops while clearing a worker

#### What changed

- **Measured again, in the project, without the window.** `core`: 247 killed and 106 survived (237 and 116
  before the tests of this morning); `model`: 286 and 119 (249 and 156); `persist`: 127 and 84 (109 and 102);
  `json`: 450 and 202 (418 and 234). Nothing in the project changed.
- **The mutator stopped five times** with "being used by another process" while it cleared a worker away:
  once while `build.exe` ran in the project, twice with 12 workers, and three times on `src/data/`, the last
  with 6 workers after `catalog.c` alone: 129 killed and 133 survived there. The cause on `src/data/` was not
  found; the run of 04:15 on a copy, before the tests of the catalog were written, finished that folder.
- **Documents.** `MAINTAINER.md`: not to run `build.exe` while the mutator runs, and what a stopped run
  leaves. `DELETION_REQUESTS.md`: the five folders of workers left in `target/mutation-workers/`, which hold
  junctions into the project and must not be deleted with a tool that follows them.

#### Not done

- `src/data/` was not measured again as a whole.

### 21:03:39: the answers recorded: no cap on tests, P10's bounds and check count given up, AA only, coverage measured

#### What changed

- **Answers.** Of the six questions of 05:41:51, in
  [all of src measured by the mutator](#054151-all-of-src-measured-by-the-mutator-8955-mutants-4754-killed), in
  `instructions_3.md`: gcovr is installed (1); P10 gives way wherever it threatens ub-mode, and the mutator makes
  no exception for it (2); the cap of 100 tests is given up for the criteria of Rules 5 and 6 (3); the criteria of
  level AAA leave the checklist (4); a function written another way because the dryer paired it by form only is
  accepted (5); question 6 was unclear, and is asked again below. Rules 31 to 36 are no longer in `RULES.md`;
  Requirements 10, 11 and 12 ask for what Rules 32, 31 and 34 asked, and ub-mode's skill for the rest.
- **No cap on tests.** `TEST_LIMIT` is out of `tests/rules/log_rules.c`; the test of the numbering of `TESTS.md`
  checks the numbering and the fields only.
- **P10's fixed bounds and two checks a function given up** (P10 rules 2 and 5). The tests "every loop has a fixed
  upper bound" and "at least two checks per function" and the gate test "the rule of bounded loops names a loop
  without a fixed bound" are taken out, with what only they used: `pattern_unbounded_loop_line` and its helpers in
  `code_patterns.c`, `code_has_two_checks_per_function` in `code_rules.c`, and the count of checks of each function
  in `tests/scan/`. A check that no caller can fail, and a loop's second bound that never ends it, make mutants no
  test can kill; from now on data is checked where it enters, and a loop is bounded by what it walks.
- **The coverage build.** `build.exe coverage` builds the tests with `--coverage -O0` into `build/obj_coverage`,
  links `build\tests.exe` only, makes `target\coverage\c\` and runs the tests. gcovr then writes
  `target\coverage\c\lcov.info`, which the crapper and the mutator read with `--use-existing-coverage`. The modes
  of `build_steps.c` are a table of folders and flags in place of two functions, and its checks of arguments that
  no caller can get wrong are gone.
- **AA only.** The section "Beyond the requirement: level AAA" of `.github/ACCESSIBILITY.md` is taken out, 31
  criteria. Tests that named an AAA criterion name the AA criterion they serve: the contrast test checks 4.5:1
  (1.4.3), not 7:1 (`PALETTE_AA_CONTRAST`, 450, in `palette.h`), the size of targets 2.5.8, controls that never
  overlap 2.4.11, the step chosen in the list 1.3.1, the help of each control 3.3.2. The two tests of the glossary
  name no criterion of level AA; they name Requirement 10, since without them its code is run by no test (question
  2).
- **A folder of 8 files.** `instructions_3.md` made `sitrep&instructions/` hold 8 files, one over Rule 14, and the
  test of folders failed. The four pictures of 2026-11-09 are moved into `sitrep&instructions/pictures/`, and the
  links of `instructions_2.md` to them mended. Nothing was deleted.
- **A program in use.** `build\civbuilder.exe` was open in a window, so the build could not write it. It was
  renamed `build\civbuilder_in_use.exe`, which Windows allows of a running program, and the window was left open.
- **Measured.** With coverage, the crapper scores 814 functions of `src/`: 180 at CRAP 10 or above, the worst
  `keep_game_texts` in `forge.c`, 20.0, then `deflate_pack`, 19.1; most have a complexity of 7 to 9 and half
  their branches run. The dryer finds no pair in `src/`.
- **Tests.** 360, down from 363: Tests 333, 335 and 349 are taken out; Tests 334 to 363 are now 333 to 360.
- **Checked.** `build.exe quick`: 360 tests, every one passed. Before the tests were taken out, the two P10 tests
  failed on `build_steps.c`, which shows they read it. `build.exe coverage` ran every test and wrote 110 `.gcda`
  files; gcovr made a report of 112 files.
- **Documents.** `TESTS.md`: the introduction, the entries of the three tests taken out, the numbers and lines of
  those after them, and the purposes that named AAA or Rule 33. `AGENTS.md`, `MAINTAINER.md` (a step 7 of the first
  chapter, to run the mutator; the rules the code keeps; how to measure Requirements 10 to 12; the cap; the folder
  of the instructions) and the skill `tests-doc`: no cap, the criteria, Requirements 10 to 12 in place of Rules 31,
  32 and 34. `TERMINOLOGY.md`: "coverage build" added; "check", "CRAP score", "crapper", "dryer", "mutant" and
  "mutator" name the requirements. A new part of the log, `DEVLOG5.md`, with the day 2026-11-16 moved into it
  whole, since part 4 would pass 1000 lines; the links to its sections in `DECISIONS.md` are mended.
  `DELETION_REQUESTS.md`: `build\civbuilder_in_use.exe`. `DECISIONS.md`.

#### Not done

- The checks that no caller can fail and the loops' second bounds are still in `src/`; they are taken out next,
  folder by folder.
- 87 tests' purposes name no requirement, rule or bug in the form `TESTS.md` uses, and no test checks that each
  does (Rule 6).
- 180 functions of `src/` at CRAP 10 or above.
- The four faults of the trial in `instructions_3.md`: steps shown as done at the start, the scroll bar, smooth
  scrolling and the step of the wheel.

#### Questions

1. **`tests/` and `tools/build/` outside the measure** (question 6 of the sitrep, asked again). The crapper, the
   dryer and the mutator pass by every folder named `tests` or `build`, and the crapper and the dryer every file
   named as a test. So the 95 files of `tests/` and `tools/build/` are not measured when the three are run on the
   project, though Requirements 10 and 11 speak of all code. Named one by one, they are read: 126 of their functions
   are at CRAP 10 or above (the worst `source_span_is` in `tests/scan/source_scan.c`, 90), and 282 pairs are 80%
   alike, nearly all of them tests. A test needs no test of its own, and a mutant of a test kills nothing, which is
   why the programs leave tests out. Proposal: `tools/build/` is measured by naming its files and kept under both
   bars; `tests/` stays outside Requirements 10 and 11. Until answered, `src/` and then `tools/build/` are brought
   under them, and `tests/` is not.
2. **Tests of what no requirement asks.** The glossary of the menu Help was made for two criteria of level AAA
   (3.1.3 and 3.1.4), and Tests 262 and 315 check it. Without them its code would have no test, and a function of a
   complexity of 3 or more without one fails Requirement 10. Proposal: such a test names Requirement 10. Otherwise
   the two tests are taken out, or the glossary.
3. **The pictures moved.** To keep `sitrep&instructions/` within 7 files, the four pictures went into
   `sitrep&instructions/pictures/`, and their links in `instructions_2.md` were mended. Proposal: keep it so, and put
   later pictures there.

### 21:18:51: no step shown as done before the user changed it

#### What changed

- **Why.** The trial in `instructions_3.md` found steps marked "(done)" when the program started, and took them for
  a past state loaded at the start (Bug 1 of 2026-11-16). Nothing is loaded at the start: the design starts from the
  base civ Generic, whose universal nodes fill the tech tree, and a step was "done" as soon as it held anything.
- **Done means changed from the base civ.** `wizard_step_is_done` in `wizard.c` takes the base civ's design as well
  as the design: the tech tree, the civ bonuses and the team bonus are done when they differ from the base civ's;
  the unique unit and the two unique techs when the pick differs or a name or description is typed; Identity when the
  civ has a name, Hero unit when a hero is picked, Review when the civ is complete, as before. `sync_steps` in
  `sync.c` makes the base civ's design with `base_civ_apply` each time it writes the list of steps. So a new design,
  and one just set from a base civ, shows no step done. Replaced: a step done as soon as it holds anything, and the
  steps a base civ fills marked done.
- **Tests.** 361, up from 360: Test 268, "no step is shown as done when the window opens", in
  `tests/shell/dialog_tests.c`, reads the rows of the list of steps of the real window. The tests of
  `wizard_tests.c` judge a step against a blank base.
- **Checked.** Test 268 failed before the change, on "Tech tree (done)", and passes after it. With the old rule put
  back for the tech tree alone, it failed again; the rule was restored. `build.exe quick`: 361 tests, every one
  passed. A picture of the window on the step Tech tree shows "2. Tech tree" without "(done)".
- **Documents.** `TESTS.md`: Test 268, and the numbers and lines of those after it. `MAINTAINER.md`: when a step is
  marked done, under Base civs. `DECISIONS.md`.

### 22:12:27: the done rule's mutants killed, and the checks of `wizard.c` no caller can fail taken out

#### What changed

- **Why.** The mutator was run on `src/view/wizard.c` after the fix of 21:18:51 (Requirement 12). Of 71 mutants, 67
  were killed; 2 survived and 2 were not covered. The survivors were in `is_step` and in the check of `by` in
  `wizard_move`: no caller passes a step outside the nine or a move other than 1 or -1, so no test could tell the
  mutants from the code. The uncovered lines were the `return false` of `same_tree` and `same_bonuses` for a tree, or
  bonuses, as large as the base civ's but not the same.
- **Checks taken out.** `is_step`, the check of `by` and the empty tenth page of `PAGES` were taken out, as the answer
  to question 2 of 2026-11-16 05:45 asks of checks no caller can fail: `wizard_page` returns the step's page, and
  `wizard_move` stops at the first and the last step.
- **A second run** of the mutator, after that change and two new tests, left 8 of 58 mutants alive. They had been
  reported killed in the first run, but no test checked them: a tree that differs from the base civ's only in its
  units, a team bonus other than the base civ's, and a text one letter long (`[0]` changed to `[1]`).
- **The done rule.** `same_team_bonus` asks `civ_design_has_team_bonus` instead of comparing the first team bonus of
  both designs by hand. `is_typed` (`text_length(text, 1) > 0`) says whether a text holds anything, for the civ's
  name, the unique unit's name and description and the unique techs' names, where four comparisons with `'\0'` were.
  `wizard.c` includes `text.h` instead of `check.h`.
- **Tests.** 365, up from 361. Test 231 checks that a tree that swaps a unit of the base civ's for another counts as
  changed; Test 232 the same of the civ bonuses; Test 239 that a team bonus other than the base civ's counts as
  changed, and Test 240 that the base civ's own does not. The tests after them were renumbered. `MODEL_SUITE_SIZE` in
  `tests/suites/suites.h` went from 65 to 69.
- **Obstacle met.** With two cases added but `MODEL_SUITE_SIZE` left as it was, `build.exe quick` reported "361 tests
  run, 0 failed" and passed: the last two cases of the model suite were not run, and no test noticed, since the test
  of `TESTS.md` counts the `case` lines, not the tests run. See question 1.
- **Checked.** `build.exe quick`: 365 tests, every one passed. The mutator, run a third time on `wizard.c`: 51
  mutants, all killed (100%).
- **Documents.** `TESTS.md`: Tests 231, 232, 239 and 240, and the numbers and lines of those after them.

#### Not done

- The number of tests a suite runs is still written twice, in its `case` lines and in a size constant.

#### Questions

1. **A suite's size is written twice.** `suites.h` gives each suite of the functional category its size, and the
   suite's `case` lines give it again; when they differ, tests are left out without a failure. Proposal: let
   `build.exe` compare the "N tests run" of the runner with the number of tests in `TESTS.md` and fail when they
   differ, which catches any test left out, whatever the cause.

### 23:56:00: lists scroll a row a notch and a row an arrow, without sliding, under classic scroll bars

#### What changed

- **Why.** The trial in `instructions_3.md` asked for scroll bars of normal appearance, with knob and arrows shown
  without the mouse over them (Bug 2 of 2026-11-16), for no smooth scrolling (Bug 3), and for one row per scroll
  (Bug 4). On Windows 11 (build 26300), a list drew a thin bar that grew only under the mouse, slid each scroll for
  190 to 530 ms, and moved three rows a notch. Measured on the Tech tree list: a notch 389 ms and 3 rows, a click on
  an arrow 530 ms; a page 11 ms; any move with the list's redrawing turned off 6 ms or less. Keys and clicks on the
  lists of civbuilder, whose rows differ in height, took 0 to 16 ms; only a plain list slides on keys (186 ms a row).
- **The loop of messages.** `assist_run_message` in `assist.c` is now the body of the loop in `app_run_messages`. It
  takes a list's wheel and arrows before Windows does. A notch of the wheel over a list moves it one row
  (`turn_wheel`); a part of a notch, from a fine wheel or a touch pad, is kept in `wheel_rest` until it makes a whole
  one. A press on an arrow of a list's scroll bar moves it one row (`press_arrow`, `arrow_rows`), and the list holds
  the mouse until the button is let go, as the scroll bar of Windows does; while it holds it, `LIST_REPEAT_TIMER`
  moves it a row after 400 ms and then every 50 ms (`repeat_arrow`). A press between the arrows is left to Windows,
  which moves the list a page. `move_list` moves a list with its redrawing turned off, so that it does not slide.
- **A list is known by its class.** `is_list` asks the window's class, `LISTBOX`, so that the list of steps, the
  lists and the checklists are all handled alike, and no number of a control is looked up.
- **Not by a second window procedure.** Taking a list's messages in a window procedure of its own (subclassing)
  would need a second function pointer, which the test of P10 rule 9 forbids; every wheel turn and press that
  scrolls a list passes through the loop, so the loop takes it there.
- **Classic scroll bars.** `create_child` in `controls.c` calls `assist_plain_bar` for every control whose style in
  `STYLES` has a vertical scroll bar: the lists, the checklists, the paragraphs and the report, and the list of each
  drop-down choice. It turns the visual styles off for that control alone (`SetWindowTheme`, from `uxtheme`, now
  among the libraries in `tools/build/source_list.c`), and Windows then draws its scroll bar with arrows and knob at
  all times. The style is read from `STYLES`, not from the window: Windows takes the bit off a list that has nothing
  to scroll yet. The window's own scroll bar was left as it was: with the visual styles off for the window, its title
  bar looks like that of Windows 2000.
- **Checks taken out.** The checks of `view_height`, `assist_placed` and `create_child`, which no caller can fail,
  were taken out, as the answer to question 2 of
  [05:41:51](#054151-all-of-src-measured-by-the-mutator-8955-mutants-4754-killed) asks; `assist.c` is 370 lines of
  code and `controls.c` 399, under Rule 13's 400.
- **Tests.** 372, up from 365, in the new `tests/shell/scroll_tests.c`: Test 285 checks that the up arrow of the
  Tech tree list's scroll bar is drawn (at least 4 colours where the thin bar has 2), Test 286 that each notch moves
  the list a row up or down, Test 287 that a click or a double click on either arrow does, Test 288 that a notch and
  a click on each arrow each take less than 150 ms on a list shown on the screen, Test 289 that a held arrow keeps
  moving the list, Test 290 that a press between the arrows still moves a page, and Test 291 that a key reaches a
  list once through the loop. Tests 285 to 365 became 292 to 372. Test 288 shows the window at the top left of the
  screen for a moment, since Windows slides only what is seen.
- **Checked.** Tests 285, 286 and 288 failed before the change and pass after it. The mutator was run on the new
  lines of `assist.c` three times. The first run, on a first version, left most mutants of the new code alive: the
  tests then turned the wheel one way, pressed one arrow, never sent a key through the loop, and the code quieted
  keys and clicks that never slide. That code was taken out, Tests 287, 290 and 291 were added, and Test 286 turns
  the wheel both ways. The second run, with six workers, killed 41 and left 3 alive. One of them, the up arrow not
  taken, fails Test 288 when the tests run alone: the list then slides for 328 ms. With six workers their windows
  cover each other, and Windows slides only what is seen. `press_arrow` now returns whether the press was on an
  arrow, which the same decision moves the list on, so that the press left to Windows as well fails Test 290. The
  third run, with one worker: of the 44 mutants of the new code, 43 were killed. The one alive leaves a double click
  on an arrow (`WM_NCLBUTTONDBLCLK`) to Windows, which then moves the list a row as well, without sliding; the two
  differ only while the second press is held, in Windows' own loop, which no posted message can hold. The mutants
  of `create_child`'s call of `CreateWindowExW` were its zeros, which no test could tell from ones; they are now
  `WS_EX_LEFT` and `CW_USEDEFAULT`, which for a control means a place and a size of 0, as before. `build.exe`, with
  the analyzer: 372 tests, every one passed, no warning.

- **Documents.** `TESTS.md`: Tests 285 to 291, and the numbers and lines of those after them. `MAINTAINER.md`: the
  rows of `src/win/` and `tests/shell/`, and how a list scrolls, under The narrow window. `TERMINOLOGY.md`: notch,
  smooth scrolling, classic scroll bar. `DECISIONS.md`.

#### Not done

- The window's own scroll bar, shown when the window is too small for the step, is still the thin bar of Windows 11.
  A scroll bar control (class `SCROLLBAR`) draws the classic bar even with the visual styles on, so it could take
  the place of the window's own.
- The drop-down lists of the choices still slide and move three rows a notch: their wheel goes to the choice, not
  to the list. A list dragged past its edge with the button held is scrolled by Windows, which may slide it.
- The program was not tried with a real wheel or a real held button; the tests post the messages Windows sends.
- The mutant of the double click lives. The older code of `assist.c` (the focus, the window's scroll bar, the
  rows' heights), `app_window.c` and the rest of `sync.c` were not mutated; `assist_placed` keeps two mutants of
  its loop, which starts at a control that is no list.


#### Questions

1. **A press between the arrows.** Bug 4 asks for one row "per scroll instance". It was read as a notch of the wheel
   and a press on an arrow; a press on the bar between the arrows still moves a page and the knob still follows the
   mouse, as in every program of Windows. Should a press between the arrows move one row too? Proposal: no.
