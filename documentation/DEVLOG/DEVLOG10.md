# Development log, part 10

- [2026-11-24](#2026-11-24)

Part 10 of the development log. It follows [part 9](DEVLOG9.md). The decisions made in the log are listed in
[DECISIONS.md](../DECISIONS.md).

## 2026-11-24

### 06:14:27: the unique unit's stats and the unique techs' tech values and tech effects set in UU&UT

#### What changed

- **Why.** In UU&UT the unique unit and the two unique techs could only be picked and named: what they cost, what the
  techs do and the unit's hit points, attack, armor, speed and the rest were always the game's.
- **Answers.** Four questions were posed before the work and answered. The unit and the techs are edited in place, in
  panels like Chosen bonuses; what a unique tech does is a set of civ bonuses, its tech effects; the elite unit takes
  the same change as the unit; and a test build that opens the window may run when a test needs it, one at a time,
  while a run of the mutator with the window is still agreed first.
- **The design.** `UnitPick` holds an override (`Override`: a value, and whether it is set) of each stat of each rank,
  the unit and its elite unit; `TechPick` holds one of each tech value, and up to 6 tech effects
  (`CIV_TECH_EFFECT_CAPACITY`), each a bonus with its multiplier. `civ_design_set_unit_stat`,
  `civ_design_set_tech_value`, `civ_design_toggle_tech_effect` and `civ_design_set_tech_effect` set them, within
  `civ_stat_range` and `civ_tech_range`, and picking another unit or tech clears them. `UnitStat` moved from
  `catalog.h` to `civ_design.h`. The commands `COMMAND_SET_UNIT_STAT`, `COMMAND_SET_TECH_VALUE`,
  `COMMAND_TOGGLE_TECH_EFFECT` and `COMMAND_SET_TECH_EFFECT` make each change one that undo takes back; a tech's
  command carries its tier in its field (`TIER_STRIDE`). `civ_compare.c` compares the overrides and tech effects too,
  so a stat set makes the civ a changed civ.
- **The civ file.** `civ_file_write.c` writes the stats under `overrides` of `unique_unit`, as `hp_base` and
  `hp_elite`, a tech's costs under its `cost` and its research time as `time`, each `null` when not set, and a tech
  with tech effects with `mode` `custom` and its `effects`. `civ_file_read.c` reads them, and a cost of four zeros as
  not set.
- **The number panels.** Chosen bonuses is now one of four number panels (`NumberPanel` in `number_edit.h`); The
  selected unit, What the castle tech does and What the imperial tech does, under the lists of UU&UT, are the others.
  Their first rows are a sheet (`number_unique.c`, new): the name, the game's description of a tech, the costs and
  the stats or the research time, each stat and tech value in a field of a number, without a card. A tech's tech
  effects follow, each with the fields, card and slider of Chosen bonuses, and the game's description is left out.
  `chosen_window.c`, `chosen_edit.c` and `chosen_draw.c` take the panel as an argument; `chosen_window.c` was written
  anew. `catalog_find_unit` in `catalog_lookup.c` takes the place of four copies of one search.
- **Choose castle effects... and Choose imperial effects....** Two buttons of UU&UT, between each tech's name and its
  panel, open the dialog of `text_dialog.c` in a new mode that ticks rows (`ticks` of `AskQuestion`), with the civ
  bonuses as its rows. OK makes the ticked ones the tech effects, as one change (`dialogs_choose_effects` in
  `dialogs.c`, `choose_effects` in `sync.c`, `history_replace`); Cancel changes nothing. A row of a dialog holds 320
  characters in place of 64 (`ASK_ROW_WIDTH`), since 66 of the 394 bonuses are longer than 63.
- **The build.** `unique_build_stats` in `unique_stats.c`, new, writes each stat set into the civ's unique unit in
  the dat file, both the value the game counts and the value it shows, and into the elite unit the same change; an
  attack goes to the larger of the unit's attacks of classes 4 and 3. The costs set fill the three slots of costs of
  both units (`dat_edit_patch_cost`, which was `put_cost`). What cannot be built is counted in `unit_stats_skipped` of
  `RecipeReport`, and the summary of the build says "A changed stat or cost of the unique unit is not built."
  `recipe_add_unique_tech` in `unique_tech.c` builds a tech's costs and research time set, and a tech with tech
  effects with one new effect, "Unique tech effects": for each tech effect, the changes of the game techs that its
  bonus names, then its lines in `bonus_effects.txt` (`bonus_build_lines`, in place of `bonus_build_team`), each
  scaled by its multiplier.
- **The mod's texts.** `mod_tech_text` in `mod_text.c` describes a tech with tech effects by its bonuses' texts,
  scaled by their multipliers and joined by "; ", as "Infantry +18 armor" for "Infantry +9 armor" counted twice.
- **Removed.** The texts of the unique unit and techs in `details.c`, which nothing called.
- **Tests.** 778, up from 693: 85 redundancy tests and none of another kind (Rule 31), 24 in `stat_tests.c`, 26 in
  `stat_build_tests.c` and 30 in `unit_number_tests.c`, all new in `tests/redundancy/unique/`, and 5 in
  `tests/redundancy/chosen/unit_panel_tests.c`, new. 23 of them were written for the survivors of the mutator, below.
  The redundancy tests from 165 on took new numbers. The small game of the tests has a new mode, `small.elite`, in
  which the soldier of each civ costs food, gold and population, has an attack bonus before its attack, and has an
  elite unit with hit points and a reload time of its own; only the tests of `stat_build_tests.c` set it.
- **Checked.** Every one of the 778 tests passed, those that open the window among them. The gate with the analyzer was
  left to the end of the work of the night that followed, and is in [the changes of the day gated and
  measured](#the-changes-of-the-day-gated-and-measured). Each new test was seen to fail: before the code it tests was
  written, or with that code broken on purpose. The 23 for survivors failed with the survivor put back. Three were
  written after their code and were broken afterwards: "a build that left out a stat of the unique unit is not whole"
  failed with `is_whole` in `summary.c` not asking `unit_stats_skipped`, "a value typed past the range of a stat is
  refused" with `number_unique_command` taking a value past the range, and "the numbers of the selected unit have no
  slider" with `number_slides` sliding the unit's numbers. Each breakage was undone. After the changes named under
  Obstacles, the crapper scored every function under 10 and the dryer found no two functions 0.8 alike.
- **Documents.** `TESTS.md`: the 85 new tests, and the numbers and lines of the redundancy tests after them, made
  again from a run of every test. `MAINTAINER.md`: the section "The unique unit and techs", the rows of `src/model/`,
  `src/mod/`, `src/numbers/`, `src/unique/`, `src/shell/`, `src/win/` and `src/chosen/`, of `tests/redundancy/` and of
  `tests/dat/`, with `small.elite`, the window fixture's ticks, and the diagram's `src/unique` using `src/bonus`.
  `TERMINOLOGY.md`: stat, rank, tech value, override, number panel, tech effect, sheet, Choose castle effects... and
  Choose imperial effects... added; field of a number, number in focus, range and card no longer of Chosen bonuses
  alone. `DECISIONS.md`. This part, `DEVLOG9.md`, was started: the day 2026-11-23 moved into it whole from
  `DEVLOG8.md`, which had 998 lines, with its links to sections of part 8 and those of `DECISIONS.md` mended.

#### The mutator's survivors

- **The runs.** A run without the window on 2026-11-23, of every folder the change touched but those of the window, left
  122 survivors on lines of this change, 93 of them on its own code, and 15 sites not covered. A second run of those
  folders on 2026-11-24, with the changes of the sections below, left in the functions of this change only what is
  listed under Left.
- **Checks taken out,** as no caller can fail them: of the tier, rank and stat in `civ_design_toggle_tech_effect`,
  `civ_design_has_tech_effect`, `civ_design_set_tech_effect`, `civ_stat_fits`, `civ_design_set_unit_stat`,
  `civ_design_set_tech_value`, `civ_stat_range` and `civ_tech_range`; that a unit's origin and id agree, in
  `civ_design_pick_unit`; of the tier in `number_panel_tier`; and of the row and the number in `number_unique_row` and
  `number_at`, with the returns under them in `number_unique_name`, `number_unique_range` and
  `number_unique_command`, since every way into a sheet reads the number's text first, which checks both. The second
  bounds of the loops of `read_effects`, `entry_at` and `attack_at` went with them: a tech refuses an effect past its
  room, and the dat file's counts are checked where its unit is read.
- **One check of a range.** `range_holds` in `civ_design.c` is now the check of both `civ_stat_fits`, which the build
  asks of the elite unit's value, and `set_override`, which a command meets; the model keeps it, since the tests of a
  command out of range hold it, though every caller fits a value to its range first.
- **Tests for the rest,** 23, each seen to fail with its survivor put back. In the larger small game: the elite unit
  built from its own hit points and reload time, an attack built past an attack bonus, the least and the most hit
  points, the population and other costs kept, a cost of 1 alone, a cost of 0 taken out, a tech's cost of 1, and an
  effect unticked. Without it: no stat left out for a unit missing from the catalog, the mod's text of a tech built
  twice, a unit's cost saved and changing the civ, the same unit picked again, a tech's cost of 0 saved, a tech
  without effects saved as `vanilla`, an effect found ticked, the least and the most hit points typed, the range of a
  tech's cost, the second row of a tech with effects, and the name of an effect's field. The last two of these and the
  one found ticked stand for the window, which alone calls `civ_design_has_tech_effect` and gives a field its name, and
  whose tests passed with both survivors.
- **Left,** each with the state that would tell it: the first values of the report in `recipe_apply`, which a build sets
  again before anything reads them, so only a build that skipped a step would show them; the bound of the ranks in
  `same_stats`, past which lies memory that is the same in the two designs compared, so only a third rank would tell;
  the checks of an id in `civ_design_pick_unit` and `civ_design_pick_tech`, which only an id past `ID_LIMIT`, larger
  than any the catalog holds, would fail; and `civ_design_set_tech_value` refusing a value while no tech is picked,
  which the window cannot ask for, as it shows no field then, so only a civ file that sets a value of a tech it does not
  pick would tell. Two more were left then and are gone since: the name row of a tech's sheet in `fill_tech` has a test,
  "the first row of a selected tech holds its name", and the text of `details_of` starts empty by its initializer. On
  lines older than this change, in functions it renamed or gave a parameter, the second run showed again: the first line
  and the second bound of the loop of `bonus_build_lines`; the places of the attack graphic, the hero mode and the
  garrison graphic in `skip_combatant` and `skip_creatable`; and the train place and the costs of a hero in
  `train_at_castle`. A test that reads a hero back from a built data file would tell the last two.

#### Obstacles met, and how they were solved

- **Full folders and files.** `src/model/`, `src/dat/`, `src/forge/`, `src/win/`, `src/view/` and `src/present/` hold
  7 files each (Rule 14), so the new code went to `src/unique/` and `src/numbers/`, and the dialog of ticks became a
  mode of `text_dialog.c`. `recipe_game.c` reached 400 lines (Rule 13): two of its readers moved into
  `recipe_tests.c` behind `recipe_game_rebuilt`, and at 402 lines the civ of tech effects was folded into the civ of
  stats. The window's tests went into `unit_panel_tests.c`, as `chosen_tests.c` was near 400.
- **The rules of the code.** Test 603 (P10, one dereference a line) failed at `unique_stats.c` and `recipe.c`, and was
  passed with pointers of their own; Test 58 (Rule 12) at a line of 121 characters, which was split. Test 37 (a name
  for a screen reader that starts with the label) failed for the new panels until `show_label` named every panel.
- **The dryer** found four pairs, 0.8 and more alike, each joined into one function: `skip_combatant` and
  `skip_creatable` into `skip_list`, `chosen_panel` and `chosen_panel_list` into `chosen_controls`, `civ_stat_least`
  and `civ_stat_most` into `civ_stat_range`, and the `tier_of` of `number_unique.c` and `binding.c` into one sum of
  `check_index`.
- **The call graph of the tests,** which reads every function for the test of recursion, holds 3200 functions and
  32000 calls, up from 2400 and 24000: the code passed 2400 functions, and Redundancy test 645 failed.
- **The coverage.** gcovr stopped at counts of `game_text.c` that add up over the runs, and left an old
  `lcov.info`; it now runs with `--gcov-ignore-parse-errors=suspicious_hits.warn`, since no agent may delete the
  `.gcda` files (Rule 30). The crapper then scored `copy_tech_effect` 12.0, as no test ran it: the civ of stats now
  has `MIXED_BONUS`, a bonus with a game tech and a line, as its castle tech effect, and a test of the game tech's
  changes was added.
- **Expectations mended.** A reload time of 2 is shown as "2", not "2.00"; and "Villagers carry 3 food" holds no
  number to set, since a number without a sign is left as it is, so the test's bonus became "Villagers carry +3 food".

#### Not done

- The mutator did not run on this change's functions in `src/chosen/`, `src/win/` and `src/shell/`, since their
  tests open the window (Question 13); what survives there is not known. `src/view/`, whose tests need no window,
  was run on 2026-11-24: the change touched only its tables, which hold no mutants.
- The survivors under "The mutator's survivors" that are left stand, each named there.
- The program was not tried by hand, and no mod it built was played in the game; only the tests ran, those that open
  the window among them.
- Train time and line of sight cannot be set (Question 1), nor can the elite unit's own stats be seen (Question 3).

#### Questions

1. **Train time and line of sight.** Neither can be set: the catalog has no train time. A civ file of the original
   holds the train time of the unit and its elite unit in `overrides`, as `train_base` and `train_elite`; civeditor
   does not read them, so saving such a file again loses them. Proposal: add the train time and the line of sight as
   stats, read from the dat file, and read and write `train_base` and `train_elite`.
2. **A unique tech that costs nothing.** A cost of four zeros in a civ file is read as not set, the game's cost, as
   the original writes it for a cost not changed. So a tech given 0 of each resource is free in the session and in
   the mod, but costs the game's again once its civ file is opened. Proposal: keep it; a free unique tech is rare,
   and the original's files open as they should.
3. **The elite unit's own stats.** A civ file of the original may set a stat of the elite unit apart, as `hp_elite`;
   civeditor keeps it, saves it and builds it, but no panel shows it, so it can be neither seen nor cleared.
   Proposal: show the elite unit's own stats in a row of The selected unit when a civ file set any.
4. **Esc in the field of a stat.** In Chosen bonuses Esc puts back the number shown and hides the card; in the field
   of a stat or a tech value, which has no card, Esc does nothing, and the value typed is kept when the field is left.
   Proposal: Esc puts back the number shown there too.
5. **A custom tech of the original without a game tech.** A unique tech of `mode` `custom` whose `vanilla_km_idx` is
   `null` opens as no tech picked, since civeditor builds a unique tech from a game tech, so it is not built.
   Proposal: keep it, and say in the status line which tech of the file was not opened.
6. **What a bonus does that a tech cannot.** A bonus's unit patches, its train lines, cost lines and look lines, are
   not built for a tech effect, since they change the dat file from the start and not when a tech is researched; such
   a tech effect builds its changes alone, and nothing says so. Proposal: count it as left out in the report of the
   build (Requirement 9).
7. **The ages of a bonus's lines.** A line of `bonus_effects.txt` that comes with an age is built, as a tech effect,
   when the tech is researched, as every other change of the tech. Proposal: keep it.
8. **A game tech that the dat file lacks.** A game tech that the bonus of a tech effect names, but that the dat file
   lacks, or whose effect it lacks, is passed over without a word (`copy_tech_effect` in `unique_tech.c`). Proposal:
   count it in the report of the build (Requirement 9).
9. **Ticks shown as picked rows.** The list of Choose castle effects... shows a ticked bonus as a picked row, in the
   color of a selection, not with a check box; a screen reader says that it is selected. Proposal: keep it; check
   boxes would need a list that civeditor draws itself.
10. **A tech effect whose bonus has no text.** It adds an empty part to the mod's text of its tech, as
    "Infantry +18 armor; ". Proposal: leave such a part out.
11. **The help of UU&UT.** The step's help says "Select the unique unit and the castle and imperial techs, and name
    them. Tick Hero for a hero." It does not say that their numbers and the techs' effects can be set. Proposal: "Select
    the unique unit and the castle and imperial techs, name them and set their numbers and effects. Tick Hero for a
    hero."
12. **The counts of the coverage.** The `.gcda` files of `build/obj_coverage` add up the counts of every coverage run,
    so gcovr warns of counts that look wrong. Proposal: a human deletes `build/obj_coverage/*.gcda` before a coverage
    run when the counts matter; making the build program delete them would be code written to delete files, so it is
    asked first.
13. **The mutator in the folders of the window.** `src/chosen/`, `src/win/` and `src/shell/` changed, but this
    change's functions there were not mutated: their tests open the window, and such a run takes the computer for
    hours. Proposal: run it with one worker, about 20 seconds a mutant, when the computer can be spared, and kill
    what survives.

### 06:15:02: the team bonus's numbers set in fields, as a civ bonus's

#### What changed

- **Why.** The team bonus was reported as not editable: the step Bonuses gave the numbers of a ticked civ bonus
  fields, and those of the picked team bonus none, though the design, the civ file and the build already carried a
  multiplier for it. It is Bug 1 of 2026-11-24 in `BUGS.md`.
- **The panel.** The text under the list of team bonuses, `CONTROL_TEAM_INFO`, is now a number panel,
  `NUMBER_PANEL_TEAM` in `number_edit.h`, the fifth: its one row is the picked team bonus as it counts, with a field
  for each number to set and the card with the slider under the field in focus, as in Chosen bonuses. Without a team
  bonus it has no row. `chosen_window.c` lists it among its panels, with the list of team bonuses as the control that
  takes the focus when its number goes away.
- **The command.** A value typed or slid there is a `COMMAND_SET_TEAM_MULTIPLIER` (`number_edit.c`, `command.c`),
  which `civ_design_set_multiplier` applies to the team bonus; it goes through `sync_apply`, so it can be undone.
- **Shown as it counts.** The summary of Finish (`summary.c`) and the description in the mod's strings
  (`mod_text.c`) scale the team bonus's text by its multiplier, as they do a civ bonus's. The help of the panel says
  how a number is set (`control_help.c`).
- **Tests.** Test 10 is new, a core test of the bug: with the Britons' team bonus, 20 typed into its field makes the
  row read "Archery Ranges work 20% faster". The core tests after it took new numbers. Five redundancy tests in
  `tests/redundancy/present/number_edit_tests.c`: the row as it counts, the command of a value typed, no row without
  a team bonus, the mod's description and the summary.
- **Checked.** Test 10 failed before the panel was made, since the window had no field to type into, and passed
  after. The first two of the redundancy tests failed before their code; the other three passed when written and
  were left to the mutator, in
  [the changes of the day gated and measured](#the-changes-of-the-day-gated-and-measured).
- **Answers.** Of the questions of 03:53:22, in [the numbers of Chosen bonuses typed or
  slid](DEVLOG9.md#035322-the-numbers-of-chosen-bonuses-typed-or-slid-in-place-of-stronger-and-weaker), and of 14:02:39, in [the
  numbers typed in their own
  fields](DEVLOG9.md#140239-the-numbers-of-chosen-bonuses-typed-in-their-own-fields-the-slider-just-below): the team bonus has its
  numbers to set, in a panel under the list of team bonuses (3 and 4).
- **Replaced.** That the team bonus has no number to set, of the same two sections, and that the team bonus's text is
  shown as it is, of [2026-11-21
  21:19:41](DEVLOG8.md#211941-unticking-a-node-unticks-its-descendants-and-stronger-and-weaker-edit-a-bonus).
- **Documents.** `BUGS.md`: the bug, covered by Test 10. `TESTS.md`, `MAINTAINER.md` (the team bonus's panel in the
  section on civs and bonuses), `TERMINOLOGY.md` (number panel) and `DECISIONS.md`.

#### Not done

- The team bonus's panel was not tried by hand.

### the choice of asking to save before exiting kept by Options...

#### What changed

- **Why.** It was asked that the options say whether civeditor asks to save the civ before it exits.
- **The tick box.** Options... in the menu File shows, under the mods folder, the tick box "Ask to save the civ before
  exiting". `AskQuestion` in `shell.h` has two new members, `check_label` and `check`; `text_dialog.c` adds an
  `AUTOCHECKBOX` only for a question that has them, moves the buttons down by its room, and gives the tick back on
  OK. A question with a tick box may be answered with an empty text, so that Options... takes the tick while no mods
  folder is known.
- **Where it is kept.** In the options file, `civeditor_options.txt` in the user's application data (`%APPDATA%`),
  as one character, `1` or `0` (`options_ask_to_save` and `options_set_ask_to_save` in `ask.c`). Without the file, or
  without `%APPDATA%`, the program asks, as before. A test scripts another folder as that home (`AskScript.home`), so
  no test touches the user's own file.
- **Exiting.** `handle_ending` in `app_window.c` asks `dialogs_may_discard` only when the option says to. Open
  civ..., which also discards unsaved changes, still asks: the option is about exiting.
- **Options answered.** With the folder left as it is, OK keeps the tick and the status line says "The options were
  kept."; Cancel leaves both, with "The options were left as they were." Another folder is checked as before.
  `dialogs_options` returns nothing now, as nothing read what it returned.
- **Tests.** Nine redundancy tests of the window: closing asks nothing of a changed civ when the option says not to;
  Options keeps the choice not to ask; Options with the folder left says the options were kept; a changed civ is
  asked to be saved when no option is kept; a click ticks the tick box of a question; a click unticks one given as
  ticked; and three written for the mutator's findings: Options cancelled leaves the choice as it was, the tick
  box of a question lies above its buttons, and a question with a tick box takes an empty text.
- **Checked.** The first six failed before their code. With `options_ask_to_save` answering false when no file is there,
  "a changed civ is asked to be saved when no option is kept" failed. Of the last three, two failed with their code
  broken: Options saving the tick after Cancel, and the buttons moved up by the room of the box, not down. Each breakage
  was undone. The third failed while the code it tests was out, as the section below tells.
- **Documents.** `TESTS.md`, `MAINTAINER.md` (Options..., with the options file), `TERMINOLOGY.md` (Options...,
  options file) and `DECISIONS.md`.

#### Not done

- The dialog was not looked at by eye: where the tick box sits and how it reads at a large text size.
- The mods folder is still kept only until civeditor closes; only the tick box is written to the options file.

#### Questions

1. **Where the option is kept.** The options file is `%APPDATA%\civeditor_options.txt`, beside no other file of
   civeditor. Should it be there, in a folder of its own, or beside `civeditor.exe`? Proposal: as it is, since the
   program's own folder may not be writable.
2. **What the option covers.** Unticked, only exiting stops asking; Open civ... still asks before it discards
   unsaved changes. Should the option cover that too? Proposal: no, as it was worded for exiting.

### rows measured once a width, and less redrawn

#### What changed

- **Why.** The window was reported as a bit laggy. It was measured with a probe that sent the window its messages
  off screen and timed them: nearly all the time went to measuring the height of list rows, a `DrawTextW` with
  `DT_CALCRECT` for each row, 0.085 to 0.17 ms each, done for every row of every list at each refill and each
  placing.
- **Heights kept.** `assist_wrap_rows` in `assist.c` keeps the height of each item of a list for as long as the list
  keeps its width (`WrappedRows` in `win_parts.h`, one for each control), and measures only an item not yet
  measured. `assist_placed` wraps only the lists of the step shown that are not wrapped at their width;
  `assist_forget_rows` forgets all when the fonts are made anew (`make_fonts` in `controls.c`). The list is not
  redrawn while its rows are set.
- **The filter.** `binding.c` works out which civs the filter's text names once for a list (`name_civs`, into
  `NamedCivs`), not once for each row.
- **Less redrawn.** `tick_rows` in `sync.c` sets the top row and redraws a list only when a tick in it changed, and
  `chosen_sync` in `chosen_window.c` fills only the number panels of the step shown.
- **Measured,** in milliseconds a message, before and after, off screen, each the mean of repeated sends: opening the
  window 210 and 92 to 151; a key in the name 7.4 and 1.7 to 2.1; a step shown 77 and 21 to 25; a key in the filter of
  the bonuses 57 and 3.2 to 3.3; a bonus ticked 5.8 and 4.9 to 5.6; a team bonus picked 6.3 and 4.2 to 5.8; a step of
  resizing 75 and 66 to 71; a node ticked 2.4 and 0.6 to 0.8; a key in the filter of the tech tree 18.7 and 5.9 to 6.8;
  an undo or a redo 3.8 and 0.9 to 1.4. Before is one run, after the least and the most of three runs on the code as it
  is left. The probe is not kept in the tests.
- **Tests.** None of a time: no requirement names a speed, and a test of a time would fail by chance. Three
  redundancy tests of the window pin what the change must not break, written when the mutator showed that nothing
  did: the rows of a list shown are measured anew when it becomes wider; a letter typed into the name leaves the
  tech tree nothing to redraw; the longest row of the narrow list of team bonuses is higher than the shortest, the
  first test of the rows of a list without check boxes.
- **Checked.** The tests of before passed with the change, those that draw the lists among them. Each of the three
  failed with its code broken: the kept heights not dropped at a new width, `is_wrapped` turned about, a shown list
  not wrapped anew when placed, `tick_rows` redrawing always, and a list without check boxes not wrapped. Each
  breakage was undone.
- **Documents.** `TESTS.md`, `MAINTAINER.md` (the narrow window: heights kept) and `DECISIONS.md`.

#### Obstacles met, and how they were solved

- **Placing only the step's controls** was tried and taken out: a hidden list kept a width of 0, and the row height
  then failed its own check of a width above 0. The controls are all placed as before; only the wrapping is limited
  to the step shown.

#### Not done

- **Resizing** still measures every row of the step's lists at each new width and moves every control: about
  66 to 71 ms a step of the drag. Wrapping once the drag ends, or `WS_CLIPCHILDREN` on the window, might help, and
  was not tried, since neither could be judged without looking at the window.
- **A stall on screen.** With the window on screen, `LB_SETSEL` and `LB_SETCURSEL` took about 30 ms each after a sync
  in some runs, against 0.04 ms in a plain list; the cause was not found, and it could not be told whether a click
  by the user pays it. It may come from the display being off while the measurement ran.
- The window was not tried by hand, so whether it now feels quick enough is not known.

#### Questions

1. **Lag as a bug.** The lag was reported in passing and has no row in `BUGS.md`, as no single fault describes it.
   Should it have one? Proposal: no.
2. **Resizing.** Is the drag of the window's edge among what felt slow? If so, wrapping only when the drag ends is
   proposed, with the rows cut short in between.

### the catalog's numbers and nodes read from the game

#### What changed

- **Why.** It was asked that civeditor depend on no catalog that can go stale: its data should come from the game
  installed, not from files of the project. This section does the part of it that could be done and checked in one
  night; what is left is listed under Not done, with a question.
- **What was found** in the game installed, against the files of `data/`:
  - `units.txt` and `techs.txt` agree with the game's data file but for the speed of the Bolas Rider, 1.50 in the
    file and 1.45 in the game, and for what the files leave unknown: the stats of the Immortal, Strategos, Hippeus,
    Companion Cavalry, Rhomphaia Warrior and Pattiyodha Longbowman, and the costs of Bearded Axe, Pavise, Carrack,
    Madrasah, Orthodoxy and Zealotry.
  - Which civs have each node of `tree.txt` agrees with the 62 files of `CivTechTrees` without one difference.
  - `tree.txt` itself holds 205 of the game's 567 nodes, a chosen part that no rule of the node's kind gives, and
    differs from the game's files in 33 buildings, 10 parents, 23 names and 1 age, so it cannot be made from them
    as it is.
- **The numbers.** `game_numbers_read` in the new `src/gamedata/game_numbers.c` reads from the game's data file, into
  the catalog, the stats of every unit of role `V` (found by its make-available tech, as a build finds it) and the
  costs and research time of every tech that the catalog copies from a tech of the game. `unique_stats_read` in
  `unique_stats.c` reads a unit's stats with the places the build writes them to; `read_costs` there now serves both.
- **The range as shown.** The data file holds two ranges for a unit, the one it shoots by and the one the game
  shows: the Fire Archer has 9 and 5. The range is read as shown, since that is what the user knows; every other
  stat is read as held, as the Iron Pagoda's reload time, held 2.15 and shown 1.8, is 2.15 in `units.txt` too.
- **The nodes.** `tech_tree_read_game` in `tech_tree_file.c` reads the civ list and each civ's tech tree file, and
  `tech_tree_mark` in `tech_tree.c` marks in the catalog the civs that have each node: a node that the file lacks, or
  gives as `NotAvailable`, is taken from that civ. The walk of a tree file's two lists, `visit_lists` and
  `visit_nodes`, now serves the marking and the writing both.
- **When.** `forge_read_game` in `forge.c` does both. `app_run` posts `APP_GAME_WANTED` to the window once it is
  shown, and `dialogs_read_game` then finds the game and reads it, which took about 0.8 s here, 0.43 s of it the
  data file; the window is up meanwhile. A civ to edit with no unsaved change and no file is then made its base civ
  anew, so that its tech tree is the game's; a civ already changed is left as it is.
- **The files stay as the fallback.** A unit, a tech or a civ that the game lacks keeps what `data/` says, and
  everything does when no game is found, so civeditor still opens without the game.
- **Tests.** 18 redundancy tests in the new `tests/redundancy/dat/game_number_tests.c`, on the small game of the tests:
  a unit's cost, a resource it does not cost, its attack, a unit the game lacks, a custom unit, the catalog's first
  unit, a tech's cost, a resource a tech does not cost, its research time, a tech whose game tech is lacking, a tech of
  no game tech, the read from a game's folder, a folder without a game, the range as shown, and a node the civ's file
  has, lacks and gives as not available, and a civ without a file. Three of the window in `tests/shell/dialog_tests.c`:
  the window shows the numbers read, an unchanged civ takes its tech tree from the game, a changed civ keeps its own.
- **Checked.** The tests of a number read failed before their code. With the guard of unsaved changes taken out of
  `dialogs_read_game`, "a changed civ keeps its tech tree when the game's files are read" failed; the breakage was
  undone. Those that say what is kept passed when written and were left to the mutator, in the section below. Against
  the game installed, the read gave back `units.txt` and `techs.txt` but for the differences above.
- **Documents.** `MAINTAINER.md` (the row of `src/gamedata/`, the diagram, what the game replaces in "The catalog
  files"), `TERMINOLOGY.md` (catalog), `TESTS.md` and `DECISIONS.md`.

#### Obstacles met, and how they were solved

- **Reading when the window is made** delayed its first paint and broke tests that pump messages; the read moved to
  a message posted by `app_run`, which the tests' `app_open` does not post. A test that wants the read sends
  `APP_GAME_WANTED` itself.
- **Every stat read as shown** made the attack read wrong in the small game; only the range is read as shown.
- **`recipe_game.c` at 400 lines** (Rule 13) had no room for a tech with a cost, so the test of a tech's cost adds
  that tech to the small game itself (`costly_tech`).

#### Not done

- **Still from the files of `data/` only,** and so still able to go stale:
  - `bonuses.txt`, `team_bonuses.txt` and `bonus_effects.txt`: the 394 bonuses and their changes are the
    original's own list, with ids that civ files use; the game has no such list. Their texts are the original's
    wording.
  - `tree.txt` but for its civs: which nodes are offered, their names, ages, buildings, parents and lanes.
  - `appearance.txt`: the civs, their names and base lines, the architectures and other looks, and the string ids of
    the campaigns' texts.
  - `units.txt` and `techs.txt` but for their numbers: which units and techs are offered, their names, their
    descriptions and the techs of the game they come from; the custom units and the heroes whole.
- The files of `data/` were not changed: the Bolas Rider's 1.50 stands in `units.txt`, and is 1.45 in the window
  once the game is read.
- A game update that adds a civ still needs its lines in `data/`, as `MAINTAINER.md` says.
- The read was not tried by hand in the window, nor with a game folder other than the one installed.

#### Questions

1. **How far the files should go.** The numbers and the civs of each node now come from the game; the lists
   themselves do not. Which of these is wanted next? Proposed order: (a) names and descriptions of units, techs and
   nodes from the game's strings file; (b) the units, techs and civs offered, from the civ list and the data file,
   so that a new civ of the game appears by itself; (c) the tree's nodes with their ages, buildings and parents
   from the tech tree files, which changes what the step Tech tree shows, 567 nodes against 205. The bonuses cannot
   come from the game: they are the original's list. Should they stay a file of the project?
2. **The files as fallback.** Should civeditor still open without the game, on the files of `data/`, as now? Or
   should the numbers be taken out of `units.txt` and `techs.txt`, so that nothing stale can be shown, at the price
   of a window without numbers until a game is found?
3. **The read at every start.** It takes about 0.8 s after the window is shown, and a civ not yet changed is made
   anew after it. Should it run in the background instead, or be kept from one start to the next and read again
   only when the game's files are newer?
4. **Range as shown.** Is the range the user sets the one the game shows, as read now, with every other stat as the
   data file holds it?
5. **The Bolas Rider's speed** in `units.txt`: correct it to 1.45, or leave the file alone now that the game is read?

### the changes of the day gated and measured

#### What changed

- **Why.** The five sections above were written together, at the end of the work; this one holds what was checked
  for all of them at once, and what the mutator found in the night.
- **Tests.** 818, up from 778: one core test, Test 10 of the bug of the team bonus, and 39 redundancy tests: 5 of the
  team bonus's panel, 9 of the option and its tick box, 3 of the rows of the lists, 21 of the game's numbers and nodes,
  and one of a tech's sheet, left from the unique unit's change. There are 69 core tests, 30 regression tests and 719
  redundancy tests.
- **Checked.** The gate, `build.exe redundancy`, passed on the code as it is left, at 06:14: the build with the analyzer
  gave no warning and no finding, and each of the 818 tests passed, those that open the window and those of the rules
  among them. The log and `DECISIONS.md` were written after it, and the tests were then run once more.
- **Requirements 10 and 11.** After the last change every `.c` file was touched and the coverage build run with
  every test; the crapper scored every function under 10, the highest `skip_graphics` in `dat_skip.c` at 9.2, then
  `sync_details` and `civ_has` at 9.0, and the dryer found no two functions 0.8 alike.
- **Documents.** `TESTS.md` was made anew from a run of every test, with the entries of the new tests, by the tool
  that the section on the unique unit names. `MAINTAINER.md`: the row of `tests/shell/` names `change_tests.c`.
  `DECISIONS.md`: the decisions of the six sections, and three numbers of tests that it cited, corrected to the
  numbers the tests have now. `DELETION_REQUESTS.md`: the workers of a run of the mutator that was stopped.

#### The mutator's survivors

- **Without the window, the folders of the night.** `src/gamedata/`, `src/techtree/`, `src/unique/`, `src/forge/`,
  `src/numbers/`, `src/model/`, `src/present/`, `src/mod/` and `src/persist/`, an hour: 1967 mutants killed, 88
  survived, 75 not covered. In the code of the night two survived. One read the catalog's units from the second on
  in `game_numbers.c`; the test "the first unique unit of the catalog is read from the game's data file" was written
  and failed with it. The other was the second of two loops alike in `tech_tree.c`, which now share `visit_lists`.
  Two tests more were written there: a resource that a unique tech does not cost is 0, and the first row of a
  selected tech holds its name, for a survivor left from the unique unit's change.
- **Not covered in the code of the night:** a tree file whose node has no id (`tech_tree.c`), a civ list whose entry
  has no name, and a tree file that cannot be opened (`tech_tree_file.c`). Each needs a file of the game that is
  broken in that one way; none was made.
- **With the window, one worker.** `ask.c`, `dialogs.c`, `app_window.c`, `sync.c`, `assist.c`, `controls.c`,
  `chosen_window.c` and `text_dialog.c`, only the lines of the functions the night changed, 53 minutes at about 20
  seconds a mutant: 76 killed, 77 survived, and one not covered among those lines, Cancel in Options.... With four
  workers and with two the run failed before its first mutant: "Appearance opens the popup of the look choices"
  fails when two test programs show their windows at once (Question 2).
- **Taken out for it.** What `dialogs_options` returned, which nothing read; and the first character of a `DetailText`
  set by hand in `details_of`, `details_title` and `details_window_title`, now set by the initializer.
- **Taken out and put back.** That a question with a tick box may be answered with an empty text survived in
  `takes_no_text` of `text_dialog.c`, and was taken out as code nothing needed. It was then seen that Options...
  needs it: while no mods folder is known its text is empty, and OK would have left the tick as it was. The test "a
  question with a tick box takes an empty text" was written, failed, and passed with the code put back; it failed
  again with each of the two mutants of that code.
- **Tests written for it,** six, each seen to fail with its survivor put back: Options cancelled leaves the choice of
  asking to save as it was; the tick box of a question lies above its buttons; a question with a tick box takes an empty
  text; the rows of a list shown are measured anew when it becomes wider; a letter typed into the name leaves the tech
  tree nothing to redraw; the longest row of the narrow list of team bonuses is higher than the shortest.
- **Run again.** The lines of `dialogs_options`, of the tick box in `text_dialog.c`, of `tick_rows` and of the wrapping
  in `assist.c` were run again with one worker, 27 minutes, the old survivors of those files with them: 56 killed, 53
  survived. Cancel in Options... is covered and killed, and so is each mutant that a test was written for: the room of
  the tick box in `drop_of` and the top of OK, the width compared in `assist_wrap_rows` and in `is_wrapped`, a list of
  the step not wrapped anew in `assist_placed`, a list without check boxes not wrapped, and `tick_rows` redrawing
  always. The code put back in `takes_no_text` came after that run; its two mutants were made by hand.
- **Left with the window,** each with what would tell it: results of window messages that nothing reads, in
  `app_window.c` (10) and in `tick_rows` and `assist_wrap_rows`; second bounds of loops; the first values of the
  question in `dialogs_options`, which the dialog sets before it reads them; `sync_design(app, false)` made `true` in
  `dialogs_read_game`, which only a look at what is redrawn after the read would tell; `assist_forget_rows`, which only
  a change of the text size of Windows while the window is open would tell, and no test may change that setting of the
  user; the arithmetic of the dialog's template in `make_template`, which only the place of each control would tell,
  tested now for the tick box and OK alone; a check of a font in `make_fonts`; whether a list has check boxes, where the
  width of its rows is asked for, which only a width a few pixels off would tell; a hidden list wrapped too in
  `assist_placed`, which only the time taken would tell; and the answer of a question cancelled with a text in it, as no
  test cancels the dialog shown.
- **Without the window, again.** `src/bonus/`, `src/dat/`, `src/hero/`, `src/data/`, `src/json/`, `src/core/`,
  `src/gamedata/`, `src/techtree/`, `src/present/`, `src/numbers/` and `src/view/`, 55 minutes: 1868 killed, 583
  survived, 130 not covered. None survived in `game_numbers.c`, `number_unique.c`, `number_edit.c` and
  `tech_tree_file.c`. In `src/present/`, `src/techtree/` and `src/numbers/`, which the night changed, 24 survived,
  and 11 in the functions of the unique unit's change in `src/bonus/`, `src/dat/` and `src/hero/`, all of kinds left
  before: checks that no caller fails (`details_title`, `tech_tree_mark`), bounds of loops made `<=` that read one
  entry more, which is empty (`visit_lists`, `tech_tree_mark`, `mark_needs`, `mark_needers`, `set_marked`,
  `bonus_build_lines`), the two bounds of `bounded`, the field of a command that has none in `hero_box_command`,
  the wording for one civ changed in `details_changed`, a name that is empty in `add_choice` and in
  `details_window_title`, and, on lines older than the change in functions it renamed or gave a parameter, the
  places of the attack graphic, the hero mode and the garrison graphic in `skip_combatant` and `skip_creatable` and
  the train place and costs of a hero in `train_at_castle`, which no test reads back from a built data file.
- **Counted, not read:** the other 548 survivors and the 130 mutants not covered of that run. They lie on lines that
  neither change touched, most of them in `layout.c` (99), `catalog.c` (77), `json_reader.c` (39), `dat_edit.c`
  (37), `dat_skip.c` (36), `tsv.c` (34) and `dat_map.c` (33) (Question 3).

#### Not done

- The mutator did not run with the window on the functions of the unique unit's change in `src/chosen/`,
  `src/win/` and `src/shell/`, nor on `chosen_edit.c`; only the functions of the night were run there.
- The survivors named under Left stand, and the 548 counted were not read.
- Nothing was tried by hand: not the team bonus's panel, not Options..., not the speed of the window, not the read
  of the game in the window. Only the tests and the probe ran.
- Resizing is as slow as it was, and the stall seen on screen was not explained; both are in the section on the
  rows above.
- The lists of the catalog, the bonuses and the structure of the tech tree still come from the files of `data/`;
  the section on the game above says which.
- The gate was run once, at the end, not after each of the five changes.

#### Questions

1. **The mutator with the window.** It had been agreed that a run of the mutator with the window is asked for
   first. For this night it was said that tests could be run as much as wanted, and the computer was not in use, so
   the run was made, with one worker, 53 minutes and then 27 minutes. Was that reading right, and may such
   a run be made again when the computer is said to be free?
2. **The test of the popup of the look choices** fails when two test programs run at once, so the mutator with the
   window cannot use more than one worker, and two builds with the window cannot run side by side. Proposal: find
   what the two share, the focus or the foreground window most likely, and make the test hold without it.
3. **The survivors on lines no change touched,** 548 in the folders run without the window, and those of the window
   never counted in full. Should they be read and killed folder by folder, as a task of its own? Proposal: yes,
   starting with `layout.c` and `catalog.c`.
4. **Open from before:** the questions of the five sections above, and those of 2026-11-23 that no section has
   answered.

### 11:26:10: a list never paints its rows white, the team bonus's panel lower, an error box traced to a mutant

#### What changed

- **Why.** Three things were reported. The selected rows of a list, which are dark blue, blinked white while the list
  was scrolled. The panel of the selected team bonus took almost half of its column, though its text is one or two lines
  (`temp/current.png`). And a box of Windows, "tests.exe - Application Error", had been left on the screen
  (`temp/error.png`) and was to be looked into.
- **The cause of the blink.** Windows erases a list before it draws its rows, with the brush that the window answers to
  `WM_CTLCOLORLISTBOX`, which was the brush of the field colour. `move_list` in `list_scroll.c` asks for the whole list
  to be erased and drawn at every row that the wheel or an arrow moves it, so every row shown was white from the erasing
  until its own drawing, and a dark row showed it.
- **Seen first outside the project.** A small program beside the project, not part of it, made a list box of the same
  styles with one selected row and read the screen while the first row was drawn after a scroll: as `move_list` does it,
  no selected row was on the screen at that moment; without the erasing, or with a brush that paints nothing, one was.
  The same program showed that Windows paints nothing under the last row of such a list by itself, that a list scrolled
  by Windows itself moves the picture of its rows and does not blink, and that a list made with `WS_EX_COMPOSITED` drew
  no row at all.
- **Seen in the program.** The built program was started on the screen, 24 bonuses ticked, and 60 notches of the wheel
  sent to the list of bonuses while one pixel of the ticked rows was read from the screen, about 15 times a notch. Eight
  runs each: before the change the pixel had another colour in 6 of 480 notches, after it in 0 of 480.
- **A list erased with a brush that paints nothing.** `controls_color` in `controls.c` passes its answer through the new
  `list_scroll_brush` in `list_scroll.c`. For a list box it paints the room under the last row with the brush it was
  given and answers the hollow brush of Windows; for every other control it answers the brush it was given. The rows
  draw their whole rectangles as before, so no pixel of a list is left unpainted and none is painted twice.
- **A strip under a full list.** In its first form `list_scroll_brush` painted from the bottom of the last row to the
  bottom of the list also when the last row lies far below the list. `FillRect` turns such a rectangle the right way up,
  and a picture of the window made with `PrintWindow` showed white strips under the lists; on the screen Windows clips
  them. The room is now painted only when the last row ends inside the list.
- **The panel of the selected team bonus.** It no longer stretches: its line of `SPECS` in `layout.c` gives it
  `TEAM_PANEL_HEIGHT`, 124, and the list of team bonuses above it takes the rest of the column. 124 is the room of two
  lines of text, 72 at 96 dots an inch, the card under them, 48, the border, and 2 to spare. A text of more lines
  scrolls in the panel, as before. The narrow layout of `reflow.c` gave the panel a fixed height already and is as it
  was.
- **The error box.** Nothing was changed for it. The application log of Windows holds the crash: on 2026-11-24 at
  01:55:04, `tests.exe` of `target\mutation-workers\run-aae01843\worker-0\build\`, exception 0xc0000005, fault offset
  0xd46d; the address in the picture ends in D46D and reads address 0. It is the test program of a mutant, built by the
  run of the mutator that started at 01:52:32, in which a mutant read through a null pointer. The log holds 108 crashes
  of a `tests.exe` from 2026-11-21 to 2026-11-24, every one of them under `target\mutation-workers\`, none of the
  project's own `build\tests.exe`. The box is shown because a debugger is registered for crashes on the computer (the
  `AeDebug` key names `vsjitdebugger.exe`, without `Auto`) and the test program does not turn the box off; it waits
  until it is answered. No `tests.exe` was still running when this was looked into.
- **Tests.** 822, up from 818: 70 core tests, 30 regression tests and 722 redundancy tests. Core Test 11, "a list that
  is erased keeps the picture of its rows", for the bug of the blink, which `BUGS.md` now lists; the core tests after it
  are one higher. Redundancy test 89, "the panel of the selected team bonus is no higher in a higher window"; Redundancy
  test 658, "the room under the last row of a list is painted in the colour of fields"; Redundancy test 659, "a list
  full of rows paints nothing under itself". The three of the lists send the list `WM_ERASEBKGND` with a black picture
  in memory and read a pixel of it (`scroll_erased_pixel` in `tests/shell/scroll_tests.c`).
- **Checked.** Each test was seen to fail first: Test 11 and Redundancy test 89 before the change, Redundancy test 658
  with the brush that paints nothing alone, Redundancy test 659 with the room painted without the condition. Four
  mutants of `list_scroll_brush` were made by hand, one at a time, and every test run with the window on each: the list
  taken for no list, the row after the last asked for, and the condition turned both ways; each failed one or two of the
  new tests, and the file was put back. After every `.c` file was touched and the coverage build run with every test,
  the crapper scored every function under 10, the highest still `skip_graphics` at 9.2, `list_scroll_brush` 3.0 with
  every line covered, and the dryer found no two functions 0.8 alike. The mutator without the window found no place to
  change on the two changed lines of `layout.c`; the panel's test failed with the stretching, as seen before the change.
  A picture of the built program's window showed the panel at its height, the lists whole, and a list filtered to three
  rows clean under them. The gate, `build.exe redundancy`, passed at 11:25 with all 822 tests.
- **Documents.** `BUGS.md`: the row of the blink, discovered 10:52:05, when the report was read. `TESTS.md`: made anew
  from a run of every test, with the four entries. `MAINTAINER.md`: a point on the erasing in the part on how lists
  scroll, and the height of the panel in the part on the team bonus. `DECISIONS.md`: the decisions below, the numbers of
  three tests that it cites, moved with the numbering, and the links to the day that moved. The log: part 9 would have
  passed 1000 lines, so 2026-11-24 moved whole into part 10, `DEVLOG10.md`, and part 9 says that it goes on there.

#### Not done

- The mutator was not run with the window on `list_scroll.c` and `controls.c`: such a run is asked for first, and the
  question of 2026-11-24 about it is open. Four mutants were made by hand instead. The condition made `<=` was not
  tried: it paints an empty rectangle.
- The blink was judged by the pixel read from the screen, not by eye. Only the wheel was measured in the program;
  dragging the knob, the keys and a click on the bar were seen not to blink in the small program only.
- A list with no row at all, as after a filter that finds nothing, has no test of its own.
- Nothing was done about the error box, and it was not tried whether the box stays when the mutator ends the process of
  a mutant that crashed: trying it puts such a box on the screen.
- A decision cites "Redundancy test 491" for a test that compares what is read back; the number was moved to 492 with
  the numbering, and it was not checked that it named the right test before.
- `temp/current.png` and `temp/error.png` are left where they were put.

#### Questions

1. **The error box of a mutant that crashes.** Every mutant that crashes can leave such a box on the screen, and its
   worker waits until the mutator's timeout. Proposal: the test program turns the box off for itself at its start, with
   `SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX)`, so that a crash ends it at once with a failing exit
   code; a mutant that crashes is then killed at once, and a real crash of a test fails the build as it does now. Should
   that be done?
2. **The height of the panel of the selected team bonus,** 124: two lines of text and the card with the slider under
   them. With 88 it would hold the two lines only, and the card would be reached by scrolling the panel. Is 124 right?
3. **Open from before:** the questions of the six sections of this day above, the first of them whether the mutator may
   be run with the window without asking when the computer is said to be free.
