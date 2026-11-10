# Deletion requests

`instructions.md` forbids any deletion. What should be deleted is listed here instead, for a human to delete. Nothing
in this list is needed by the project. Each entry says what it is, where it is and why it may go.

## Outside the project

| What | Where | Why it may go |
| --- | --- | --- |
| An empty file made by mistake | `C:\Users\Hjörleif\AppData\Local\Temp\sinkfix.txt` | Written by a mistyped command; never used. |
| A list of links written by mistake | `C:\Users\Hjörleif\AppData\Local\Temp\dummy_never_used` | Written outside the scratchpad by a check of `DECISIONS.md`'s links; never read. |
| Clones of repo_template, ub-mode and aoe2-empire-forge | the session's scratchpad folder, `ext/` | Read for the remake; the project does not use them. |
| Text extracted from the three books | the session's scratchpad folder, `books/` | Read while working; the books are in `materials/`. |
| A probe of the compiler | the session's scratchpad folder, `probe/` | Showed which flags gcc 13.2.0 accepts. |
| Notes on the dat file and the mods, clones of genieutils | the session's scratchpad folder, `research/` | Read to plan the building of mods; what they found is in the log. |
| The dat file inflated, 93 272 287 bytes | the session's scratchpad folder, `research/datraw/` | A copy of the game's data, made to read its layout. |
| A screenshot of the window | the session's scratchpad folder, `shot1.png` | Showed that the first step draws. |
| Two Perl scripts of search and replace, and their outputs | the session's scratchpad folder, `fix_controls.pl`, `fix_layout.pl`, `controls.new`, `layout.new` | Used once while the code was written. |
| An empty file made by a mistyped command in `src/pack/`, moved out of the project | the session's scratchpad folder, `stray/huffman_fix.tmp` | Never used; empty. |
| A small text and its gzip file, made for a test's bytes | the session's scratchpad folder, `vectors/` | The bytes are in `tests/pack/pack_tests.c`. |
| Parts of `TESTS.md` and a build's output | the session's scratchpad folder, `tests_*.md`, `TESTS.assembled.md`, `analyzed_build.txt` | Used to put `TESTS.md` together and to read the build. |
| A list of the project's files, and an empty file | `C:\Users\Hjörleif\AppData\Local\Temp\x`, `C:\Users\Hjörleif\AppData\Local\Temp\null_unused_never` | Left outside the scratchpad by two commands; never used. `x` was written over at 09:44 by a third, with the names of the game's civs. |
| Check programs, with what they built | the session's scratchpad folder, `datcheck/`, `unitcheck/`, `catalogcheck/`, `inflatecheck/`, `placescheck/`, `forgecheck/`, `lookcheck/`, `herocheck/`, `civlistcheck/`, `bonuscheck/`, `unitpatchcheck/`, `rulescheck/`, `herocheck2/` | Read the catalog and the installed game outside the project; what they found is in the log. `forgecheck/mods/`, `forgecheck/mods_names/`, `forgecheck/mods_unit_names/`, `lookcheck/mods_looks/`, `lookcheck/mods_monk/`, `lookcheck/mods_monk_nordic/`, `herocheck/mods_hero/`, `herocheck/mods_hero_low/`, `civlistcheck/mods_list/`, `civlistcheck/mods_bolas/`, `civlistcheck/mods_bolas_unnamed/`, `civlistcheck/mods_longbow_unnamed/`, `bonuscheck/mods_bonuses/`, `bonuscheck/mods_bonus_133/`, `bonuscheck/mods_bonus_131/`, `bonuscheck/mods_bonus_all/`, `bonuscheck/mods_bonus_third/`, `bonuscheck/mods_bonus_195/`, `bonuscheck/mods_bonus_elite/`, `bonuscheck/mods_bonus_277/` and `unitpatchcheck/mods_unit_patches/` each hold a test mod's two folders, 14 MB each. |
| The original's pool of string ids, and `appearance.txt` made from it | the session's scratchpad folder, `stringids/` | Made the string ids of `data/appearance.txt`, which the project holds. |
| The converter of the original's lists of changes, with its output | the session's scratchpad folder, `convert/` | Made `data/bonus_effects.txt`, which the project holds; the lists in it were used to count the bonuses. |
| A check of the compressed dat file by gzip | the session's scratchpad folder, `gzipcheck/`, with `mod_dat.gz` of 13 MB | Showed that gzip reads the compressed file; the result is in the log. |
| Object files of a check build | the session's scratchpad folder, `dat_edit.o`, `dat_field.o`, `dat_map.o`, `dat_skip.o`, `dat_tests.o` | Made by a build outside the project. |
| A copy of the project, and its path | the session's scratchpad folder, `repo_copy_020018/`, `repo_copy_path.txt` | A copy of the working tree at 02:00, kept while the data file's code was written; the project holds it all. |
| Drafts of code and of the catalog | the session's scratchpad folder, `ms_head.c`, `text_head.c`, `inflate.cut.c`, `mod_strings.keep.c`, `looks.new`, `unit_techs.txt`, `units.with_techs.txt`, `existing_names.txt`, `lang_test.c`, `test5_entry.md`, `looks_work/`, `hero_work/`, `list_units.txt`, `list_upgrades.txt`, `catalog_upgrades.txt`, `test34_entry.md`, `skipped_bonuses.txt`, `test36_entry.md` | Used while the code and `data/` were written; what was kept is in the project. |
| More parts of `TESTS.md` and builds' output | the session's scratchpad folder, `TESTS.bumped.md`, `TESTS.new2.md`, `TESTS.renumbered.md`, `TESTS.assembled2.md`, `toc_pack.md`, `entries_pack.md`, `suite_tail.txt`, `analyzed_build_copy.txt`, `analyzed_build2.txt`, `analyzed_build3.txt`, `analyzed_build4.txt`, `analyzed_build5.txt`, `analyzed_build6.txt`, `analyzed_build7.txt`, `analyzed_build8.txt`, `analyzed_build9.txt`, `analyzed_build10.txt`, `analyzed_build11.txt`, `analyzed_build12.txt`, `analyzed_build13.txt`, `analyzed_build14.txt`, `analyzed_build15.txt`, `analyzed_build16.txt`, `analyzed_build17.txt`, `analyzed_build18.txt`, `analyzed_build19.txt` | Used to put `TESTS.md` together and to read the builds. |
| An empty file made by mistake | `C:\Users\Hjörleif\AppData\Local\scratch_edit.txt` | Made outside the scratchpad at 09:55 by a command that was stopped; 0 bytes, never used. |
| Two copies of the program linked for a check | the session's scratchpad folder, `appcheck/` | `civbuilder1.exe` and `civbuilder2.exe`, linked there while `build\civbuilder.exe` was open; the build makes the program anew. |
| The script that linked them and ran the tests | the session's scratchpad folder, `quick_check.sh` | Used while `build\civbuilder.exe` was open; `build.exe` does the same. |
| Check programs and tables of the base civs | the session's scratchpad folder, `basecivs/` | Read the game's data and the catalog to make the `base` lines of `data/appearance.txt`, which the project holds. |
| Tables of the game's tech trees | the session's scratchpad folder, `trees/` | Read the game's `CivTechTrees` files to make the last two columns of `data/tree.txt`, which the project holds. |
| Mutants of `base_civ.c`, `tech_tree.c` and `json_patch.c`, the test programs built from them, and the scripts that made them | the session's scratchpad folder, `mut/` | Showed that the new tests fail when the code is broken; the results are in the log. |
| A check program that built two mods, and the mods | the session's scratchpad folder, `treecheck/` | Showed the tech tree file the data mod holds; the results are in the log. |
| The parts of `TESTS.md` and the scripts that put it together anew | the session's scratchpad folder, `testsdoc/` | `TESTS.md` holds the result. |
| Copies of four files kept before they were changed | the session's scratchpad folder, `civ_list_before_patch.c`, `civ_list_h.txt`, `functional_suite_before_split.c`, `mod_tests_before_parsed.c` | The changed files are in the project. |
| The output of the build with the analyzer | the session's scratchpad folder, `analyzed_build20.txt` | Its result is in the log. |
| A check program that drove the window, and its pictures | the session's scratchpad folder, `shots/`, with `poke.c`, `poke2.c`, `tick_part.c`, `poke.exe`, the PowerShell scripts `shoot.ps1`, `shoot2.ps1`, `drive.ps1`, `bug1.ps1`, `names.ps1` and `title.ps1`, and the pictures | Showed the base civ, Bugs 1 and 2 and the window's title in the running program; the results are in the log. |
| Drafts and copies kept while tests were seen to fail first | the session's scratchpad folder, `details_stub.c`, `details_new.c`, `mod_tests_before_keeps.c`, `mod_tests_keeps.c`, `mod_tests_before_keep.c` | The changed files are in the project. |
| More mutants, in the folder of mutants listed above | the session's scratchpad folder, `mut/`: those of `civ_design.c`, `details.c`, `tech_tree.c` and `recipe_tree.c` made after 10:18, with `run_mutants4.sh` and its lists | Showed that the new tests fail when the code is broken; the results are in the log. |
| The output of eighteen runs of the tests | the session's scratchpad folder, `quick_keep1.txt` to `quick_keep6.txt`, `quick_wire1.txt`, `quick_wire2.txt`, `quick_wire3.txt`, `quick_wire4.txt`, `quick_wire5.txt` to `quick_wire12.txt` | Their results are in the log. |
| Parts of `TESTS.md` from the night, not listed before | the session's scratchpad folder, `tests_file.md`, `tests_head.md`, `tests_maint.md`, `tests_maint_renumbered.md`, `tests_new.md`, `tests_new_entries.md`, `tests_new_toc.md`, `tests_toc.md` | Made at 01:17 and 02:13; `TESTS.md` holds the result. |
| An empty file left by a stopped command | the session's scratchpad folder, `stray/huffman_fix.tmp` | 0 bytes, made at 01:31, never used. |
| More of the check program that built mods, in its folder listed above | the session's scratchpad folder, `treecheck/`: `main_tree2.c`, `treecheck2.exe`, `mods_keep1/`, `mods_keep2/`, `mods_same0/`, and the tables `game_nodes.txt`, `mod1_nodes.txt`, `all_nodes.txt`, `all_typed.txt`, `all_typed2.txt` | Showed the tech tree file of two more mods and counted the game's statuses; the results are in the log. |
| The output of the last builds with the analyzer | the session's scratchpad folder, `analyzed_build21.txt`, `analyzed_build22.txt`, `analyzed_build23.txt`, `analyzed_build24.txt` | Their results are in the log. |
| Copies of files kept before the tests of the dat file's tree, and a draft of them | the session's scratchpad folder, `before_wire/` | The changed files are in the project. |
| Seven copies of the working tree, and what the night's streams left | the scratchpad folder of the session of 2026-11-09 22:12, `C:\Users\Hjörleif\AppData\Local\Temp\claude\f12a97a1153d82dc\8654ebe4-e6c7-439c-aef8-37fc76b8ce31\scratchpad`: `w/` (`base0`, `forgeA`, `forgeB`, `wincode`, `wintest`, `tools1`, `tools2`, 57 MB in all), `x/` (check programs, copies of ub-mode's dryer and what three readings gathered), `notes/` (empty) and `action_log.md` | Made for work side by side that the usage limit stopped; nothing in them was taken over, and the project holds what was kept. |

The session's scratchpad folder is
`C:\Users\Hjörleif\AppData\Local\Temp\claude\f12a97a1153d82dc\b2aa4ee3-2ce0-43a1-9a78-fdedf7304369\scratchpad`.

## Inside the project

| What | Where | Why it may go |
| --- | --- | --- |
| Build output | `build/` and `build.exe` | Made again by the build; ignored by Git. Delete only to free space. |
