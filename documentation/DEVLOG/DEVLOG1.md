# Development log, part 1

Part 1 of the development log. It goes on in [part 2](DEVLOG2.md). The decisions made in the log are listed in
[DECISIONS.md](../DECISIONS.md).

- [2026-11-09](#2026-11-09)

## 2026-11-09

### 00:44:18: civbuilder begun in C, with its own build and rule tests

#### What changed

- **Why.** `instructions.md` asks for a remake of aoe2-empire-forge, a Flask and genieutils-py web application that
  designs a civilization for Age of Empires II: Definitive Edition and builds it into a mod, in C alone, with the
  Win32 API for the window. P10, Head First Design Patterns and Clean Code are to be followed, ub-mode is the guide
  for the way of working, and the documents follow repo_template. The work began at 00:03.
- **The original studied.** Its nine steps (identity, tech tree, civ bonuses, team bonus, unique unit, castle and
  imperial unique techs, hero unit, review), its file format `empireforge_v2` (which still reads `civbuilder_v1`),
  its catalog of bonuses in `bonus_catalog_raw.json` and the effect commands it writes into `empires2_x2_p1.dat`
  were read from a clone outside the project. The installed game was found under
  `C:\Program Files (x86)\Steam\steamapps\common\AoE2DE`, with a 11 963 833 byte `empires2_x2_p1.dat` in raw deflate.
- **Build.** `tools/build/build.c` builds and tests in one step. Once, `gcc -o build.exe tools/build/*.c
  src/core/*.c` makes the build program; then `build.exe` builds with the static analyzer of gcc (`-fanalyzer`), and
  `build.exe quick` without it. Each `.c` file is compiled again only when it or a header is newer than its object.
  Three programs are linked: `build/civbuilder.exe`, `build/tests.exe` and `build/build_check.exe`, the last proving
  that the build program itself compiles under the same warnings. The warnings are those of `-std=c11
  -pedantic-errors -Wall -Wextra -Werror` and 30 more, listed in `tools/build/build_steps.c`; there are none.
- **Core.** `src/core/check.c` holds `CHECK`, the assertion of P10 rule 5: a failed check is counted, its file, line
  and condition are kept, and the caller recovers by returning. `src/core/text.c` holds bounded string work and
  `src/core/file_io.c` whole-file reading and writing through wide-character paths.
- **Model.** `src/model/civ_design.c` holds `CivDesign`, the civilization being designed, in fixed arrays: texts,
  choices of look, bonuses, the team bonus, the tech tree, the unique unit, two unique techs and the hero unit.
- **JSON.** `src/json/` writes JSON with indentation and reads it with a tokenizer that keeps the open containers on
  a fixed stack instead of calling itself.
- **Civ files.** `src/persist/` writes a design as `empireforge_v2` and reads both `empireforge_v2` and
  `civbuilder_v1`. The original's own example civ, copied as `tests/fixtures/example.civbuilder.json`, loads.
- **Catalog.** `src/data/tsv.c` reads tab-separated text and `src/data/catalog.c` the catalog of looks, bonuses, team
  bonuses, units, techs and tech tree nodes from `data/*.txt`.
- **Tests.** 25: 5 functional, none for accessibility yet, 20 for maintainability. `tests/runner.c` numbers them as
  they run and prints, for a failure, what it found and the last failed check.
- **Checked.** The build gave no warning, with and without the analyzer. 21 tests passed; Tests 22 to 25 failed
  until the documents existed. Test 8 found two lines over 120 characters and Test 14 too few checks; both were
  mended. Test 6 was shown to fail on a comment put into `src/core/text.c`; the comment was taken out again.
- **Documents.** From repo_template: `README.md`, `AGENTS.md`, `CLAUDE.md`, `LICENSE`, `.gitattributes`, the files
  of `.github/` and `.claude/skills/`, and every file of `documentation/`. `DELETION_REQUESTS.md` is new.

#### How the rules were read

- **P10 rule 1.** No `goto`, `setjmp`, `longjmp` and no recursion; Tests 10 and 11 look for them, the second through
  a graph of every call in `src/` and `tools/`.
- **P10 rule 2.** Every loop is a `for` whose condition holds a named constant as its bound; `while` and `do` are
  not used. Test 12 checks it.
- **P10 rule 3.** No memory is allocated at all: every buffer is a fixed array, most of them `static`. Test 13.
- **P10 rule 4.** At most 60 lines of code a function, as Rule 11 also asks. Test 7.
- **P10 rule 5.** Two checks a function on average; `check.c` itself is not counted, since a check inside the
  checker would call itself. Test 14.
- **P10 rule 6.** Data is declared in the smallest scope; buffers that are too large for the stack are `static` in
  the one file that uses them.
- **P10 rule 7.** Every return value is used or cast to `(void)` where it is ignored on purpose; every parameter is
  checked where it is used.
- **P10 rule 8.** The preprocessor only includes files, guards headers and defines `CHECK`, which needs `__FILE__`
  and `__LINE__`. Test 15.
- **P10 rule 9.** One level of dereferencing, no `**`, no chains of `->`, and no function pointers. Tests 16, 17.
- **P10 rule 10.** All warnings, as errors, every day, and the gcc analyzer, which is the only analyzer at hand.
- **Head First Design Patterns.** Its principles are followed; its patterns are built with an `enum` and a `switch`
  in place of objects and function pointers, which P10 rule 9 forbids. Only one `switch` decides each choice
  (Clean Code G23).
- **Clean Code.** One step builds, one step tests (E1, E2). Functions take at most three arguments (F1) and return
  results instead of writing into arguments where they can (F2). No magic numbers (G25).
- **ub-mode.** Tests first where a behavior is new; each test seen failing for the reason it names; the shape of the
  data decided before the code; illegal states made hard to express.
- **repo_template.** Its `main` branch holds Rules 25 and 26 and says that Rules 11 to 13 apply to code; these were
  taken. It has no Rule 24.

#### Decisions where rules met each other

- **The window procedure.** Win32 calls a window through a function pointer, which P10 rule 9 forbids. The one
  `lpfnWndProc` the program needs is allowed in `src/win/app_window.c` and nowhere else; Test 17 checks that.
- **The checks of the checker.** P10 rule 5 asks for checks in every function, P10 rule 1 forbids recursion;
  `check.c` guards its input with plain conditions.
- **CI.** repo_template runs `.github/scripts/check-rules.sh` from `.github/workflows/checks.yml`. Shell and YAML
  are other languages, which `instructions.md` forbids, so neither file was copied; every check of
  `check-rules.sh` became a C test (Tests 18 to 25). The case for the two files is made under Questions.
- **Folders.** Rule 14 allows 7 files a folder, so the code is split by layer: `src/core`, `src/model`, `src/json`,
  `src/persist`, `src/data`, `src/win`, and the tests by kind.

#### Obstacles met, and how they were solved

- **A name that is not ASCII.** The user's folder holds "ö". Every path in the program is therefore a wide string,
  opened with `_wfopen`.
- **No deletions.** `instructions.md` forbids deleting anything. A file made by mistake, the empty `/tmp/sinkfix.txt`,
  was left, and is listed in `DELETION_REQUESTS.md` with the clones and build output that are no longer needed.
- **ub-mode's tools.** crapper, dryer and mutator are Python tools for other languages; none reads C. Their rules
  were carried by C tests where possible, and mutation testing was done by hand: a test is broken on purpose and
  seen to fail.
- **One analyzer.** No compiler but gcc 13.2.0 (MinGW-w64) is installed, so there is no second static analyzer.
- **An ignore that hid code.** `build/` in `.gitignore` also hid `tools/build/`; it became `/build/`.

#### Not done

- The window, the steps of the wizard, the building of the mod and the catalog data are not made yet.
- No accessibility test exists yet.

#### Questions

1. **Requirements.** `REQUIREMENTS.md` is the template's and empty, and only the chief developer may write it
   (Rule 2). Until it is written, the original's behavior is taken as the requirement. Proposal: 1. a civ is designed
   in nine steps; 2. it is saved and opened as `empireforge_v2` and `civbuilder_v1` reads; 3. a mod is built from it
   into the game's local mods folder; 4. every step works by keyboard alone; 5. text and background contrast at least
   7:1 (WCAG AAA).
2. **License.** repo_template is MIT, but the original is GPL-3.0, and the catalog's descriptions come from it. The
   project was given GPL-3.0. Should it stay GPL-3.0, or should the data be replaced so the code can be MIT?
3. **Rules from `instructions.md`.** P10, Head First Design Patterns, Clean Code, C and Win32 only, and no deletion
   are not in `RULES.md`, which may only change when the chief developer asks. Proposal: add them as Rules 27 to 31.
4. **CI.** Without `checks.yml`, nothing runs the tests on a push (Rule 4). Proposal: allow one YAML file,
   `.github/workflows/checks.yml`, that runs `gcc -o build.exe tools/build/*.c src/core/*.c` and `build.exe` on a
   Windows runner with MinGW; no shell script is needed, since the checks are C tests now.
5. **`.claude/settings.json`.** repo_template's `README.md` names it, but the template does not hold it. Should it
   be written, denying `git commit`, `git push` and edits of `REQUIREMENTS.md`?
6. **Rule 24.** repo_template has Rules 23, 25 and 26 but no Rule 24. Is one missing, or is the gap meant?

### 01:20:32: the nine steps in a window, with undo, a review and accessibility tests

#### What changed

- **Why.** The section of 00:44:18 left the window, the steps and the accessibility tests undone.
- **Steps.** `src/view/wizard.c` holds the nine steps of the original as states: each step's title, its line of
  help, the step after it and before it, and whether it is done. The review is the last step, and Next keeps it there.
- **Layout.** `src/view/layout.c` places every control of every step from one table, `SPECS`: its step, kind,
  column, height and label. A label stands above its control. Everything fits a window of 960 by 560 at 96 dots per
  inch; at more dots per inch, all sizes grow with it. The identity step has three columns, as one did not fit at
  1080 lines and 150 %.
- **Colors.** `src/view/palette.c`: near-black ink on paper, a picked row white on dark blue, and a dark blue frame of
  3 pixels, grown with the dots per inch, around the row of a list that has the focus. Every pair of text and
  background contrasts at least 7:1. When Windows is set to high contrast, its own colors are used instead.
- **Changes as commands.** `src/model/command.c` makes every change to the design a command (the Command pattern),
  and `src/model/history.c` keeps a copy of the design before each, 32 deep (the Memento pattern), for undo and redo.
  Letters typed one after another into the same field join one command, so one undo takes back a whole word.
- **What the controls show.** `src/present/binding.c` says, for each control, which text, look or list of the catalog
  it shows, and which command a row of it gives. `summary.c` writes the review and `details.c` the text beside each
  list: the chosen bonuses, a unit's numbers, a tech's cost.
- **The window.** `src/win/`: one window procedure, the one function pointer (P10 rule 9). At the left the list of
  steps; lists and checklists are drawn by civbuilder, with rows 44 pixels high. A menu (File, Edit, Go) and keys:
  Ctrl+Page Down and Ctrl+Page Up, Ctrl+1 to Ctrl+9, Ctrl+S, Ctrl+O, Ctrl+Z, Ctrl+Y and Ctrl+B. Tab reaches every
  control. Civ files are opened and saved with the dialogs of Windows, and unsaved changes are asked about first.
- **The review asks what the original asks.** The original does not go on without a name and an architecture
  (step 1), a unique unit (step 5), a name for each unique tech (steps 6 and 7) and a name for a chosen hero (step 8).
  `civ_design_is_complete` and the review's "Still needed" now ask the same: a name, a unique unit, a castle tech, an
  imperial tech and, when a hero is chosen, its name. Before, they asked for a civ bonus, which the original does
  not. An architecture is chosen from the start, so it is never missing.
- **Bug: a review without labels.** `add_line` in `src/present/summary.c` checked its label but never wrote it, so
  the review read "Vikings" where it should read "Name: Vikings". It was found while the test of the review was
  written, and mended; Test 12 keeps it from coming back.
- **Tests.** 36, up from 25. New functional tests: undo (6), a typed name undone at once (7), redo (8), a 25th bonus
  refused (9), Next through the steps (10), the review's "Still needed" (11) and its labels (12, a regression test).
  New accessibility tests: contrast of 7:1 (13), a label for every input (14), targets of 44 by 44 (15), no overlap
  in the smallest window (16). The maintainability tests are now Tests 17 to 36.
- **Checked.** All 36 tests passed in the analyzed build, without a warning or a finding. Broken on purpose, then
  mended: grey text of 150, 150, 150 made the contrast test fail; an undo that kept the design made Tests 6 and 7
  fail; `add_line` without its label made Test 12 fail. `build\civbuilder.exe` was started: the identity step showed
  with its labels and three columns, and the window closed without a fault.
- **Documents.** `TESTS.md`: Tests 6 to 16, and the new numbers of Tests 17 to 36. `MAINTAINER.md`: the folders
  `src/view/`, `src/present/` and `src/win/`, a diagram, how a change reaches the window, and how to add a control.
  `README.md`: the keys. `TERMINOLOGY.md`: control, checklist, command, undo, review. `DECISIONS.md`.
  `DELETION_REQUESTS.md`: the files left in the scratchpad folder.

#### Obstacles met, and how they were solved

- **Too many arguments.** Clean Code (F1) asks for three at most; making a control needed six. They became one
  `ChildSpec`, and a label is made by its caller.
- **Wide text.** `IDC_ARROW` and others are narrow strings unless `UNICODE` is defined, which the wide functions
  refused. The build defines `UNICODE` and `_UNICODE` now.
- **A brush made at every draw.** Drawing a picked row made a brush each time and never gave it back. One brush is
  kept in `AppState` and replaced only when the colors change.
- **The message loop.** P10 rule 2 asks for a bound on every loop; the loop of messages runs while the window is
  open. Its bound is `MESSAGE_LIMIT`, 2 147 483 647 messages, never reached in practice.
- **No manifest.** The themed controls of Windows need a manifest, which is XML, a language other than C. The window
  uses the classic controls instead.
- **Freeing what Windows made.** `DeleteObject` gives back a brush or a font and `DestroyWindow` a window. Both were
  read as outside the ban on deletion in `instructions.md`, which is read as about files and folders.
- **An editing tool.** Two replacements of many lines at once, in `controls.c` and `layout.c`, were made by two Perl
  scripts of search and replace in the scratchpad folder, as `sed` could not make them. The project holds no Perl;
  later edits were made with the editor.

#### Not done

- The catalog, `data/*.txt`, is not made yet, so the lists of the steps are empty when the program runs, and there is
  no test of the catalog.
- No mod is built: Build says so and asks for the civ file to be saved instead.
- By hand, only the identity step was tried; the others were checked by the tests of the layout alone.
- The selection in a text field is drawn in the system's colors, whose contrast no test checks.
- No screen reader was tried.

#### Questions

1. **Requirements, continued.** Two proposals more to those of 00:44:18: 6. every change can be undone and redone,
   as WCAG 2.2 SC 3.3.6 asks for what a user enters; 7. a civ has at most 24 civ bonuses, as P10 rule 3 allows no
   list that grows; the original has no limit. Tests 6 to 8 rest on proposal 6, Test 9 on proposal 7.
2. **Freeing what Windows made.** Is `DeleteObject` on a brush, and `DestroyWindow` on a window, allowed under line 16
   of `instructions.md`? Without them every brush stays until the program ends. Proposal: allowed, as they delete no
   file.
3. **Themed controls.** A manifest would give the controls the look of Windows 11 and sharp text on each monitor. It
   is XML, or a resource script, neither of them C. Proposal: keep the classic controls.

### 02:15:04: a mod built from the civ, from the catalog and the game's own data file

#### What changed

- **Why.** The section of 01:20:32 left the catalog and the building of a mod undone: Build only said so.
- **The catalog.** `data/` holds six tab-separated files, taken from the original's catalogs: `appearance.txt` (92
  looks: 62 civs, 13 architectures, 4 scouts, 13 monks), `bonuses.txt` (394), `team_bonuses.txt` (80), `units.txt`
  (146: 65 of the game's own unique units, 51 custom ones, 30 heroes), `techs.txt` (120 unique techs) and `tree.txt`
  (205 nodes). A field with nothing in it is `-`. Each of the game's unique units in `units.txt` also carries its
  make-available and elite techs, from the original's `_KM_UU_TECHS`. `src/data/catalog.c` reads all six; every
  record loads.
- **Unpacking.** `src/pack/inflate.c` and `huffman.c` inflate raw deflate (RFC 1951), as the game packs
  `empires2_x2_p1.dat`: its 11 963 833 bytes become 93 272 287 in 0.41 s, the same bytes gzip gives.
  `deflate_store.c` packs in stored blocks of 65 535 bytes, which is valid deflate but does not compress.
- **The game's data file.** `src/dat/dat_map.c` walks the unpacked file and finds where each effect, civ and tech
  starts; it reads only `VER 8.9` and refuses anything that does not end exactly at the file's end. `dat_edit.c`
  writes the file anew with patches of up to 8 bytes over it and new effects and techs after the old ones, so every
  old id keeps its number. A check program outside the project read the installed file: its 51 checks of offsets,
  counts and round trips passed.
- **Places.** `src/mod/mod_places.c` finds the game through Steam's `libraryfolders.vdf`, and the mods folder as
  `mods\local` of the profile under `%USERPROFILE%\Games\Age of Empires 2 DE` changed last.
- **Strings.** `src/mod/mod_text.c` writes the civ's description in the original's form (head, bonuses, unique unit,
  unique techs, team bonus) and four tips; `mod_strings.c` writes the UI mod's strings file: the name under the
  replaced civ's `name_string_id` from the game's `civilizations.json`, "Click to play as" under that id plus 80000,
  the description under it plus 109 879, the game's descriptions of the other civs, and the tips.
- **The recipe.** `src/forge/recipe.c`, `recipe_copy.c` and `recipe_tree.c` change the game's data for the civ, as
  the original's `apply_civ` does in its first steps:
  - every tech of the replaced civ is given civ 99, which no civ is, and loses its research buildings;
  - the civ points at two new effects, "<name> Team Bonus" and "<name> Tech Tree";
  - each civ bonus whose catalog entry names techs (path A of the original) gets copies of those techs for the civ,
    with copies of their effects and of prerequisites that belong to a civ; a tech for all civs that only a civ's
    tech tree unlocks gets that unlock in the new tech tree;
  - the team bonus effect of the catalog is copied into the new team bonus;
  - the make-available and elite techs of one of the game's unique units are copied for the civ;
  - each unique tech is the catalog's source tech copied for the civ, researched in the Castle (button 7 or 8) after
    the Castle or Imperial Age, at the cost and time of the civ file, else of the catalog;
  - a chosen starting scout is written into the civ's resource 263.
- **The tech tree.** When the tech tree step has anything ticked, every tech that some civ's tech tree disables, that
  the catalog's tree shows, or that makes or upgrades into an unticked unit or building of the catalog is disabled,
  unless it is ticked or makes a ticked unit or building. The ages, techs of other civs and techs that only give
  resources by themselves are never disabled; Villager, Town Center and the resource camps always stay. With nothing
  ticked, nothing is disabled, as in the original.
- **The forge.** `src/forge/forge.c` reads and unpacks the game's file, maps it, applies the recipe, writes and packs
  the result, then writes `<prefix>-data` (`info.json` and the dat file) and `<prefix>-ui` (`info.json` and the
  strings in 15 languages) into the mods folder. The prefix is the name in lower case with every other character
  `_`, as the original makes it.
- **The window.** Build mod... asks before it writes into the mods folder it found, shows the wait cursor while it
  works (2.5 s in an optimized test program), and then says what was built and what this version does not build yet.
- **The civ it replaces.** The review step has a list, "Civ it replaces in the game", of the game's 62 civs; the
  Britons are chosen at first. The civ file keeps it as `replace_civ`, and the review shows it.
- **Bug: civ descriptions out of order.** The game's strings hold the descriptions 120200 to 120211 out of order, and
  the strings were written in the order found, stopping at 120199. They are now written in the order of their ids,
  from 120150 to 120277. Test 20 keeps it from coming back.
- **Tests.** 46, up from 36. New functional tests: three of unpacking (13 to 15), three of the data file written back
  unchanged, an effect and a tech written as the game holds them (16 to 18), the name under the replaced civ's id
  (19), the descriptions in order (20, a regression test), the folder name (21) and no mod from an unfinished civ
  (22). The accessibility tests are now 23 to 26, the maintainability tests 27 to 46.
- **Checked.** All 46 tests passed in the analyzed build, without a warning or a finding. With the descriptions
  written in reverse order on purpose, Test 20 failed; that was undone. A civ was built from the installed game into
  a folder outside the project twice, in the Britons' slot without a tree and in the Japanese slot with the example
  civ's tree: 4 bonuses, the team bonus, the Longbowman's two techs and both unique techs were built; the old techs
  of the slot had civ 99; the tech tree held 165 disables, under the game's limit of about 189; the mod's dat file
  unpacked and mapped again; the English strings held the name, the 62 descriptions in order and the tips.
- **Documents.** `TESTS.md`: Tests 13 to 22, and the new numbers of Tests 23 to 46. `MAINTAINER.md`: the folders
  `data/`, `src/pack/`, `src/dat/`, `src/mod/` and `src/forge/`, and building a mod. `TERMINOLOGY.md`: slot, change,
  patch, mods folder, prefix, replaced civ, recipe. `README.md`: Build mod. `DECISIONS.md`. `DELETION_REQUESTS.md`:
  the new files left outside the project.

#### Obstacles met, and how they were solved

- **No compressor.** Writing a deflate with Huffman codes was left for later; the mod's dat file is stored, so it is
  93 281 303 bytes where the game's is 11 963 833.
- **Lost backslashes.** A shell here-document dropped the backslashes of Windows paths in a test program; such
  programs were then written with the editor.
- **A stray file in the project.** An empty `src/pack/huffman_fix.tmp`, made by a mistaken here-document, broke the
  rule of only C and documents. It was moved out of the project into the scratchpad folder, not deleted, and is
  listed in `DELETION_REQUESTS.md`.
- **awk.** Two checks that only read files, outside the project, were made with `awk`; no file was written by it.
- **Files outside the scratchpad.** Two temporary files were left under `%TEMP%` itself; they are listed in
  `DELETION_REQUESTS.md`.

#### Not done

- 208 of the 394 bonuses have no techs in the catalog: the original makes them in code (its `ec_list` and handlers).
  They are skipped, and Build says how many.
- 21 of the 80 team bonuses have no effect in the game's file (the original's `team_ec_list`); they are skipped too.
- Custom unique units and heroes are not built; only the game's own unique units are.
- Of the looks, only the starting scout is built: not the architecture, language, wonder, castle or monk.
- A unique unit or unique tech given a new name keeps the game's name in the game; the new names appear only in the
  description and the tips.
- `civilizations.json`, the `CivTechTrees` files, `futuravailableunits.json`, flags and icons are not written.
- The replaced civ keeps its own units and resources; the original first copies the Britons' record over it.
- Units that no tech makes, such as those an age makes, stay when unticked; the original turns them off in the unit.
- Unique tech effects aimed at the source civ's unique unit are not aimed at the new one.
- The mod was not tried in the game, and Build was not tried by hand in the window, as it writes into the real mods
  folder.

#### Questions

1. **A compressor.** The mod's dat file is 93 MB where the game's is 12 MB. Proposal: write deflate with Huffman
   codes in C, about 300 lines, so the mod takes as much room as the game's file.
2. **Where the mod goes.** Build writes the two folders into the mods folder it finds, after asking; the original
   gives a zip to unpack by hand. Proposal: keep the folders, as a zip needs a zip writer and one more step by hand.
3. **`builder_version`.** Both `info.json` files say "2.7.0", the original's version, as the original writes them.
   Proposal: keep it until the game is known not to read it.
4. **Change or command.** `TERMINOLOGY.md` calls a change to a design a command, so an entry of an effect is called a
   change (`DatChange`), where the original says command. Proposal: keep "change".
5. **Requirements, continued.** Proposal 3, "a mod is built", made concrete: 3a. the mod is two folders, a data mod
   and a UI mod, in the game's local mods folder; 3b. it replaces one civ of the game, chosen on the review step;
   3c. Build says what it could not build. Tests 16 to 22 rest on proposal 3.

### 02:32:41: the mod's dat file compressed, 13.5 MB in place of 93 MB

#### What changed

- **Why.** The section of 02:15:04 left the mod's dat file in stored blocks: 93 281 303 bytes, where the game's own is
  11 963 833 (its question 1).
- **Compressing.** `src/deflate/deflate.c` packs the dat file as one block of deflate's fixed Huffman codes (RFC
  1951). It finds repeats of 3 to 258 bytes up to 32 767 bytes back, through a hash of 3 bytes and chains of at most 64
  earlier places; a repeat of only 3 bytes from further back than 4096 bytes is written as 3 literals, as zlib does.
  `bit_writer.c` writes the codes' bits. All its tables are static and of fixed size.
- **The tables of deflate's codes shared.** The bases and extra bits of lengths and distances are now
  `DEFLATE_LENGTH_BASE` and the like, defined in `inflate.c` and declared in `inflate.h`, so packing and unpacking use
  the same tables.
- **The forge.** `forge.c` packs with `deflate_pack`, and with `deflate_store` only if the packed file would not fit
  in its 100 MiB buffer.
- **Tests.** 48, up from 46. New: Test 16, packed data unpacks to the same bytes, and Test 17, repeated data packs to a
  tenth of its size. Tests 16 to 46 are now 18 to 48.
- **Checked.** All 48 tests passed in the analyzed build, without a warning or a finding. With the extra bits of each
  distance written one too high, Test 16 failed; with no repeat ever taken, Test 17 failed; both were undone. The
  example civ was built from the installed game into a folder outside the project: the mod's dat file was 13 487 942
  bytes, unpacked again to 93 274 183 bytes and mapped as before, and the whole build took 2.9 s. Put into a gzip
  file by a check program outside the project, it passed `gzip -t`, so another inflater reads it too, with the right
  CRC-32 and length.
- **Documents.** `TESTS.md`: Tests 16 and 17, the numbers of Tests 18 to 48, and the lines of Tests 13 to 15, one
  lower for the new include. `MAINTAINER.md`: `src/deflate/` in the table of files and the diagram, and step 4 of how
  a mod is built. `DECISIONS.md`. `DELETION_REQUESTS.md`: the gzip check.
- **Replaced.** The decision "Stored, not compressed", of
  [02:15:04](#021504-a-mod-built-from-the-civ-from-the-catalog-and-the-games-own-data-file).

#### Not done

- Dynamic Huffman codes are not written. They would save about another 1.5 MB, for about 150 more lines of code.
- The compressed file was not tried in the game; civbuilder's inflate and gzip read it.
- Question 1 of 02:15:04 is still open. Its proposal is carried out here; if the answer is no, `deflate_store` alone
  can be used again by changing one line of `forge.c`.

### 02:45:35: bonuses built from the original's lists of changes

#### What changed

- **Why.** The section of 02:15:04 skipped 208 of the 394 civ bonuses and 21 of the 80 team bonuses: their lines in
  the catalog name no techs, as the original makes them from lists of changes of its own, `ec_list` and
  `team_ec_list` in its `bonus_catalog_raw.json`.
- **The catalog.** `data/bonus_effects.txt` holds those lists: 1 500 lines of one change each, with its kind, bonus
  id, entry and the techs the entry needs; 1 194 changes in 192 entries of 158 civ bonuses, and 306 changes of 24 team
  bonuses. A converter in C outside the project made it from the original's file, and its counts match the
  original's. `catalog.c` reads each line into a `BonusChange`; `tsv_decimal` in `tsv.c` reads the last value as
  written, such as 0.6666666865348816, which the hundredths of `tsv_hundredths` could not hold.
- **Building.** `src/bonus/bonus_build.c` does what the original's `_apply_ec_list_entry` does: each entry becomes a
  new effect, "C-Bonus EC-list", with its changes, and a new tech of the civ that fires it. The tech costs nothing, is
  researched nowhere and needs the entry's techs, so it fires by itself at the start of a game or once they are
  researched. `recipe.c` builds a civ bonus from its techs if the catalog names any, else from its changes, as the
  original does. A team bonus whose effect in the game is missing or empty gets its changes in the civ's team bonus
  effect.
- **What is built now.** 336 of the 394 civ bonuses, 150 more than before, and all 80 team bonuses. The other 58 civ
  bonuses are made by code in the original, one by one; among them are those that unlock a second unique unit (405
  to 417).
- **Tests.** 49, up from 48. New: Test 21, a bonus without techs is built from its changes. Tests 21 to 48 are now 22
  to 49.
- **Checked.** All 49 tests passed in the analyzed build, without a warning or a finding. With the bonus's tech given
  to the next civ, Test 21 failed; that was undone. A civ with the bonuses 0, 55, 105 and 110 and the team bonus 88
  was built from the installed game into a folder outside the project: all four bonuses were built, 55 as two techs
  that need the Castle and the Imperial Age and 105 and 110 as techs that need nothing; the team bonus effect held
  the 13 changes of team bonus 88; the mod's dat file unpacked and mapped again, with 11 new effects and 9 new techs.
  The catalog read all 1 500 lines.
- **Documents.** `TESTS.md`: Test 21, the numbers of Tests 22 to 49, and the lines of Tests 18 to 20, one lower for
  the new include. `MAINTAINER.md`: `src/bonus/` in the table of files and the diagram, `bonus_effects.txt` among the
  catalog files, and how a bonus is built or skipped. `DECISIONS.md`. `DELETION_REQUESTS.md`: the converter and its
  output.
- **Replaced.** The decision "What cannot be built is skipped and told", of
  [02:15:04](#021504-a-mod-built-from-the-civ-from-the-catalog-and-the-games-own-data-file), in what it names: every
  civ bonus without techs, and every team bonus without an effect.

#### Not done

- 58 civ bonuses are still skipped, and Build says how many.
- Bonus 105 gets its lower cost only: "available one age earlier" needs other prerequisites, which no change can
  give; the original adds them in code.
- The `multiplier` of a bonus in the civ file is not applied, in either way of building: each bonus is built once.
  The original multiplies the bonus's changes.
- The new bonuses were not tried in the game.

### 02:49:33: 351 civ bonuses built, with the unit-unlocking bonuses and bonus 404

#### What changed

- **Why.** Of the 58 civ bonuses still skipped at 02:45:35, 14 are built in the original by copying fixed techs for
  the civ, and one by a fixed list of changes; both can be data.
- **Bonuses that unlock a unit.** In `data/bonuses.txt`, the bonuses 405 to 417 and 428 now name the techs that the
  original's `_UNLOCK_UNIT_BONUSES` copies for the civ, such as 1377 and 1378 for the Bolas Rider, and 885 and 895
  for the Legionary, 895 being the Romans' tech that turns off the Two-Handed Swordsman's upgrade. The recipe copies
  them with their prerequisites, as for any bonus that names techs.
- **Bonus 404.** "Mining Camp technologies are free" is 20 lines at the end of the civ lines of
  `data/bonus_effects.txt`: the four costs and the time of the techs 55, 182, 278 and 279 set to 0, the changes of
  the original's `_free_tech_cmds`.
- **What is built now.** 351 of the 394 civ bonuses, and all 80 team bonuses.
- **Checked.** All 49 tests passed. A civ with the bonuses 405, 416, 417 and 428 was built from the installed game
  into a folder outside the project: the Bolas Rider's two techs, the Legionary, the Savar, their two switch-off techs
  and the Flemish Militia's tech were copied for the civ, the elite tech pointing at the copy of its make-available
  tech. With bonus 404, one tech that needs nothing was built; the catalog read 1 520 lines.
- **Documents.** `MAINTAINER.md`: where the lines of `bonus_effects.txt` come from. `DECISIONS.md`.

#### Not done

- 43 civ bonuses are still skipped. The original builds them with code that reads the game's data, such as the techs
  researched at the Blacksmith for bonus 125, or that changes units, buildings or prerequisites.
- No test was added, as only data changed. The copying of a bonus's techs has no test of its own; it was checked
  against the installed game only.

### 03:06:22: unique techs aimed at the civ's own unique unit

#### What changed

- **Why.** A unique tech is copied from a tech of another civ, and its effect was copied unchanged, so it still
  changed that civ's unique unit: Bearded Axe gave its range to the Throwing Axeman, which the new civ cannot train.
  The section of 02:15:04 left this undone.
- **Aiming.** `src/unique/unique_aim.c` aims each change of the copied effect, as the original's
  `_build_ut_effect_cmds` does. A change to the stats of the other civ's unique unit (types 0, 4 and 5) goes to the
  civ's unique unit, and one to the other elite unit to the civ's elite unit; making the other unit available
  (type 2) makes the civ's available; a unit that appears (type 7) is the civ's elite unit; the other unit's upgrade
  (type 3), any other change of whether it can be made, and making a unit available to the team (type 12) are left
  out. Every other change is copied as it is.
- **Finding the units.** `unique_unit_of_civ` takes the other civ's unique unit from the `V` row of `units.txt` whose
  make-available tech belongs to that civ, and reads the unit and its elite unit from the effects of the row's two
  techs. The civ's own come from the design's unique unit in the same way.
- **Order and doubles.** `recipe_copy_unique_effect` in `recipe_copy.c` writes the changes kept as they are first and
  the aimed ones after them. An aimed change is left out when one written before it changes the same attribute of the
  same unit, and for armor and attack of the same armor class, as in the original. `add_unique_tech` in `recipe.c`
  uses it in place of `recipe_copy_effect`.
- **Tests.** 50, up from 49. New: Test 26, a unique tech is aimed at the civ's own unique unit. Tests 26 to 49 are now
  27 to 50.
- **Checked.** All 50 tests passed in the analyzed build, without a warning or a finding. With `unique_aim` made to
  keep a change to the other unit's stats as it is, Test 26 failed; that was undone. A civ with the Longbowman was
  built from the installed game into a folder outside the project with three castle techs in turn, and the mod's dat
  file was unpacked and mapped again. Bearded Axe's 6 changes were made to the Longbowman (8) and the Elite
  Longbowman (530). The units First Crusade makes appear at the Town Center were Elite Longbowmen. Zealotry kept its
  8 other changes and gave the Mameluke's +20 hit points to units 8 and 530. Cuman Mercenaries, the imperial tech,
  lost its change of type 12 and kept its 2 others.
- **Documents.** `TESTS.md`: Test 26, the numbers of Tests 27 to 50, and the lines of Tests 22 to 25, one lower for
  the new include. `MAINTAINER.md`: `src/unique/` in the table of files and the diagram, and how a unique tech is
  aimed. `TERMINOLOGY.md`: elite unit, aim. `DECISIONS.md`. `DELETION_REQUESTS.md`: the build's output.

#### Not done

- Companion techs. Many unique techs have a second half that fires by itself once the unique tech and an ordinary
  tech are researched, such as Svan Towers with Chemistry and Kshatriyas with Supplies. These name the game's unique
  tech, not its copy, so they never fire for the civ; the original wires them in `_wire_ut_companions`.
- The Donjon does not get what a unique tech gives the Keep; the original copies it over (`_donjon_like_keep`).
- Elephant techs such as Howdah reach only the elephants the game's effect names; the original adds the others.
- Anarchy, Marauders and Ambuscade make the civ's unique unit trainable at the Barracks or Stable, but the unit gets
  no button of its own there. The original adds one to the unit (`_ensure_anarchy_train_slot`); its notes say the
  unit otherwise takes the first button.
- The free elite units of Cuman Mercenaries (type 12) are left out: the original gives them a copy of the civ's elite
  unit in a unit slot of its own (`_setup_mercenary_uu_unit`), and civbuilder does not change units.
- With a custom unique unit the changes to the other civ's unit are left out, as custom units are not built, and Build
  does not say so.
- The mod was read back, but the game was not started with it.

### 03:19:07: unique techs shown in the game by the names the civ gives them

#### What changed

- **Why.** A unique tech kept the name of the tech it is copied from: a castle tech named "Testing Axes" was still
  Bearded Axe on its button, in its tooltip and in the tech tree. The new name appeared only in the description and
  the tips. The section of 02:15:04 left this undone.
- **String ids.** Each `civ` line of `data/appearance.txt` now ends with the two string ids that the original gives
  the castle and the imperial unique tech of a civ in that slot: 44008 and 44009 for the slot of the Britons, up to
  44433 and 44434 for slot 62. They are entries 335 to 460 of the original's pool of the game's own string ids, as
  the original found that the game takes a UI mod's text only for an id it already has. `catalog.c` reads them, and
  `catalog_tech_string_id` in `catalog_lookup.c` finds those of a slot.
- **The techs.** `name_unique_tech` in `recipe.c` points the copied unique tech at them, as the original does: its
  name at the id, the label of its button at the id plus 1 000, its tooltip plus 21 000 and its name in the tech
  tree plus 150 000.
- **The strings.** For each unique tech, `mod_strings.c` writes its name under the id and the id plus 150 000,
  "Research" with the name and the description in brackets plus 1 000, and the tooltip plus 21 000: "Research", the
  name in bold, the cost the game fills in, and the description on a line of its own. The description is the
  catalog's, without its closing full stop in the label.
- **Room in `recipe.c`.** `unique_swap_of` moved to `recipe_copy.c` as `recipe_unique_swap`, so that `recipe.c`
  stays under 400 lines.
- **A constant's name.** After the analyzed build of 03:06:22, `TEAM_TRAIN_UNIT` in `unique_aim.c` was renamed
  `TEAM_ENABLE_UNIT`, as type 12 makes a unit available to the team. The analyzed build of this section covers it.
- **README.** Turn the UI mod off before playing a campaign, as a few of its lines give way to the names.
- **Tests.** 51, up from 50. New: Test 27, a unique tech's name is written under the string id of its slot. Tests 27
  to 50 are now 28 to 51.
- **Checked.** All 51 tests passed in the analyzed build, without a warning or a finding. With the name written one
  id too high, Test 27 failed; that was undone. A civ in the slot of the Britons, with Bearded Axe named "Testing
  Axes" and Elite Mercenaries named "Testing Crowns", was built from the installed game into a new folder outside the
  project. In the mod's dat file,
  unpacked and mapped again, the castle tech pointed at 44008, 45008, 65008 and 194008 and the imperial tech at
  44009, 45009, 65009 and 194009. The strings of all 15 languages held their four lines once each, such as
  `45008 "Research Testing Axes (Unique Unit +2 range)"`.
- **Documents.** `TESTS.md`: Test 27, and the numbers of Tests 28 to 51. `MAINTAINER.md`: the fields of
  `appearance.txt`, where its string ids come from, and the strings of the unique techs. `TERMINOLOGY.md`: string id.
  `README.md`: campaigns. `DECISIONS.md`. `DELETION_REQUESTS.md`: the check's folders.

#### Not done

- The unique unit keeps the game's name in the game; a new name is still only in the description and the tips.
  Naming it needs a change to the civ's copy of the unit in the dat file, which civbuilder does not make yet.
- A unique tech's description is always the catalog's: the design has no field for one of its own, where the
  original lets one be typed.
- The game was not started with the mod; its files were read back only.

#### Questions

1. **Campaign lines.** The string ids of the unique techs belong to texts of the game's campaigns, so with the UI mod
   on, a few of those lines show the civ's texts. The original uses the same ids, and found that new ids are not
   read. Proposal: keep them, with the warning now in `README.md`.

### 03:31:22: the unique unit shown in the game by the name the civ gives it

#### What changed

- **Why.** A unique unit of the game kept its own name in the game, as the section of 03:19:07 left it.
- **String ids.** Each `civ` line of `data/appearance.txt` now has two more string ids, those the original gives the
  unique unit and the elite unit of a civ in that slot: 69802 and 69803 for the slot of the Britons, up to 70124 and
  70125 for slot 62, entries 0 to 125 of the original's pool. The catalog keeps the four ids of a slot in
  `string_ids`, of the kinds of `CivString`, and `catalog_string_id` replaces `catalog_tech_string_id`.
- **Finding a unit.** `dat_find_unit` in `dat_map.c` finds where a unit starts in a civ: `dat_skip_civ_head`, split
  from `dat_skip_civ`, walks to the civ's units, and `dat_skip_unit`, no longer static, skips each one before it. In
  the game's file, the name, the text of the button and the tooltip of every unit read were at 3, 7 and 95 bytes from
  its start, such as 5107, 6107 and 105107 for the Longbowman; these offsets are in `dat_map.h`.
- **The units.** When the civ gives a unique unit of the game a name or a description (`mod_unit_has_own_texts`),
  `recipe_name_unique_unit` in `recipe_copy.c` patches the slot's copies of the unit and of its elite unit to the
  slot's ids: the name at the id, the button at the id plus 1 000 and the tooltip plus 100 000, as the original's
  `_own_renamed_uu_strings` does. The copied elite tech takes the elite unit's ids, and none for the tech tree.
  The other civs keep the game's names.
- **The strings.** `mod_strings.c` writes the name, "Create" with the name plus 1 000, and the tooltip plus 21 000 and
  100 000: "Create", the name in bold, the cost the game fills in, and the description on a line of its own. The
  elite unit's name is "Elite" and the name. The tooltip of a unique tech is made by the same `make_tooltip`.
- **README.** The warning on campaigns names the unique unit as well.
- **Tests.** 52, up from 51. New: Test 28, a unique unit's name is written under the string id of its slot. Tests 28
  to 51 are now 29 to 52.
- **Checked.** All 52 tests passed in the analyzed build, without a warning or a finding. With the unit's name looked
  up under the elite unit's kind, Test 28 failed; that was undone. A civ in the slot of the Britons with the Longbowman named "Testbowman" and described "Shoots far." was
  built from the installed game into a new folder outside the project. In the mod's dat file, unpacked and mapped
  again, the Britons' slot had the Longbowman at 69802, 70802 and 169802 and the Elite Longbowman at 69803, while the
  Franks kept 5107 for theirs; the copied Elite Longbow tech pointed at 69803, 70803 and 169803. The English strings
  held `69802 "Testbowman"`, `70802 "Create Testbowman"` and the tooltip with "Shoots far.".
- **Documents.** `TESTS.md`: Test 28, and the numbers of Tests 29 to 52. `MAINTAINER.md`: `src/dat/`, the fields of
  `appearance.txt`, and how the unit is named. `README.md`. `DECISIONS.md`. `DELETION_REQUESTS.md`: the checks.

#### Not done

- A custom unique unit is still not built, so it has no name in the game.
- The tooltips give the name, the cost and the description, not the unit's stats as the original's do.
- The game was not started with the mod; its files were read back only.

### 03:45:50: a civ file's language, wonder and castle counted as the original counts them

#### What changed

- **Why.** The original keeps a civ's language, wonder and castle in a civ file as the place of a civ in its own list,
  which counts from 0 for the Britons: its `_km_civ_name` and `_voice_idx` add 1 to reach the slot. civbuilder read
  the number as a slot, so the example civ's language 4, the Japanese, was shown as the Teutons, and a castle saved as
  the Britons' would open in the original as the Franks'. Of the catalog, only these three looks name a civ this way.
- **Reading and writing.** `slot_of_civ_index` in `civ_file_read.c` adds 1 to `language`, `wonder` and `castle` (or
  `wonder_model` and `castle_model`), and `civ_index_of_slot` in `civ_file_write.c` takes it off again; -1, no
  choice, stays -1. The design keeps slots, as the catalog's lists do.
- **Default language.** A new design speaks as the Britons, slot 1 (`CIV_DEFAULT_LANGUAGE`), as the original's
  language 0 does; before, it was 0, which no item of the list has.
- **Tests.** 53, up from 52. New: Test 5, the original's example civ keeps its language. Tests 5 to 52 are now 6 to
  53.
- **Checked.** All 53 tests passed in the analyzed build, without a warning or a finding. Without the 1 added on
  reading, Tests 5 and 6 failed; that was undone.
- **Documents.** `TESTS.md`: Test 5, the lines of Tests 2 to 4 and 6, and the numbers of Tests 6 to 53.
  `MAINTAINER.md`: how a civ file counts these civs. `DECISIONS.md`.

#### Not done

- In the section of 03:31:22, one line of the **Checked.** point is 170 characters long, more than the 120 the log
  keeps to; it was left as it is, since an earlier section is never rewritten.

### 04:04:36: the civ given the looks of its architecture, castle and wonder

#### What changed

- **Why.** Of the looks, only the starting scout was built, as the section of
  [02:15:04](#021504-a-mod-built-from-the-civ-from-the-catalog-and-the-games-own-data-file) left it: a civ kept the
  buildings, castle, wonder and monks of the civ whose slot it took.
- **Copies.** `dat_edit_copy` in `dat_edit.c` adds a copy, `DatCopy`: bytes of the game's file written over others of
  the same size when the file is written, before the patches. An edit holds up to 2 048 of them.
- **The parts of a unit.** `dat_read_unit` in `dat_skip.c`, which `dat_skip_unit` now calls, notes where the parts of
  a unit start as it walks over it: its damage graphics, walking graphic, tasks, attack graphic, garrison graphic and
  building part. `dat_unit_read` in `dat_map.c` adds the unit's class and the unit it leaves when it dies, 11 and 45
  bytes from its start; `dat_list_units` lists where each unit of a civ starts; `dat_civ_read` gives where the civ's
  set of building icons is. To keep `dat_skip.c` under 400 lines of code, the skipping of the tech tree moved from it
  to `dat_map.c`, the only file that uses it.
- **The looks.** The new folder `src/look/` holds `look_build` in `look_copy.c`, which does what the original's
  `_copy_architecture`, `_copy_rubble` and its copies of the castle and the wonder do. Each unit of the slot takes the
  graphics of the same unit of the architecture's civ, by the unit's class there: a building, tower, wall or gate its
  standing, dying and undead graphics, damage graphics, building part, garrison graphic and the graphics of its rubble;
  a king, monk, monk with a relic, trade cart or warship its standing, dying, undead, walking and attack graphics and
  the graphics of its tasks that the original copies. The dead monk, unit 134, and the set of building icons follow.
  The castle (82) and the wonder (276) and their rubble come from the civ chosen for them, else from the
  architecture's civ. A copy whose bytes are the same already is not made, and damage graphics of another count are
  left as they are.
- **The catalog.** Each `architecture` line of `data/appearance.txt` names the slot of its civ, as the original's
  `_ARCH_REP_CIVS` does: 3, 1, 5, 8, 15, 7, 20, 22, 25, 28, 33, 21 and 11 for architectures 1 to 13.
  `catalog_architecture_civ` in `catalog_lookup.c` finds it.
- **The recipe.** `build_looks` in `recipe.c`, the last of the recipe's steps, gives `look_build` the slot, the
  architecture's civ and the civs chosen for the castle and the wonder.
- **The castle and the wonder.** The original copies the whole unit 82 or 276 of the chosen civ; civbuilder copies its
  looks, so that the file keeps its size. In the game's file, the castles of the Britons and of the Japanese differed
  in nothing else: their standing and dying graphics, three damage graphics, garrison graphic and snow graphic, all of
  which are copied; nor did the castles of the Britons and the Aztecs, or the wonders of the Britons and the Goths.
- **Tests.** 54, up from 53. New: Test 30, a building takes the look of its architecture's civ. Tests 30 to 53 are now
  31 to 54. The small dat file of the dat tests can now hold civs of one building each.
- **Checked.** All 54 tests passed in the analyzed build, without a warning or a finding. With the graphics copied
  from the slot's own unit instead of the architecture's civ's, Test 30 failed; that was undone. Outside the project,
  with the installed game: copying the
  looks of each of the 13 architectures' civs onto the slot of the Britons made from 0 to 497 copies, kept the file at
  93 272 287 bytes, left no building of the four classes with graphics other than its source's, and gave the slot the
  icon set of the architecture's own number, 1 for the Goths to 13 for the Vikings. Copying from the Britons onto each
  of slots 2 to 62 found no unit whose damage graphics or tasks differ in count, so nothing was left out. A mod built
  for a civ in the slot of the Aztecs, with the Central European architecture and the castle of the Japanese, into a
  new folder outside the project, had in its dat file the Japanese castle (graphic 172, before 6747) and its rubble
  (164), the wonder, Monastery, Monk, Monk with a relic, dead Monk, tower and wall of the Goths, and icon set 1; the
  slot of the Franks was unchanged, and no check failed.
- **Documents.** `TESTS.md`: Test 30, the lines of Tests 18 to 21, and the numbers of Tests 31 to 54.
  `MAINTAINER.md`: `src/look/` and `src/dat/` in the table, the diagram, the field of `architecture` lines, step 3 of
  building a mod, and how the looks are made. `TERMINOLOGY.md`: graphic, copy. `DECISIONS.md`.
  `DELETION_REQUESTS.md`: the checks.

#### Not done

- The monk skin chosen in the civ file: the original's `_copy_monk_skin` copies the Monk, its corpse and the Monk with
  a relic, with the Monk's icon, from the chosen civ or a scenario priest. Here the monks follow the architecture.
- The Monastery's own look (`_copy_monastery_skin`) and the language, whose voices the original takes from the game's
  sound banks.
- The original starts the civ from a copy of the Britons' record; civbuilder changes the replaced civ's own record, so
  what differs between the two besides the looks stays as the replaced civ has it.
- The game was not started with the mod; its file was read back only.

### 04:12:16: the civ's monks given the look of the monk skin chosen for it

#### What changed

- **Why.** The monk skin chosen in the civ file was not built, as the section of
  [04:04:36](#040436-the-civ-given-the-looks-of-its-architecture-castle-and-wonder) left it.
- **The catalog.** Each `monk` line of `data/appearance.txt` now names the slot of the civ whose monks it copies and
  three of its units, the monk, the dead monk and the monk with a relic: 125, 134 and 286 of its own slot for the
  eleven skins of civs, and of civ 0 the original's `SCENARIO_MONK_SKINS`, 1023, 1024 and 1025 for the Priest and
  1940, 1024 and 1941 for the Pagan Priest. `catalog_find_look` in `catalog_lookup.c` finds a look's line by its kind
  and id; it replaces `catalog_architecture_civ` of 04:04:36.
- **What the design names.** `look_sources_of` in the new `src/look/look_sources.c` reads the civs and units of the
  design's architecture, castle, wonder and monk skin; `build_looks` in `recipe.c` now only calls it and `look_build`.
- **The monks.** After the castle and the wonder, `look_build` gives the civ's units 125, 134 and 286 the standing,
  dying, undead, walking and attack graphics, the icon and the graphics of the tasks of the skin's three units, as
  the original's `_copy_monk_skin` does; a unit whose class differs from its source's is left as it is. Without a
  monk skin, the units are the architecture's civ's, as in the original.
- **Tests.** 55, up from 54. New: Test 31, a monk takes the look of its monk skin. Tests 31 to 54 are now 32 to 55.
  The small dat file of the dat tests can now put its one unit at another place, such as 125.
- **Checked.** All 55 tests passed in the analyzed build, without a warning or a finding. With the units copied only
  when their classes differ, Test 31 failed; that was undone. Two mods for a civ in the slot of the Aztecs were built
  from the installed game into new folders outside the project. With the Priest's skin, the slot's monk, dead monk and
  monk with a relic had graphics 8421, 8422 and 8426, those of units 1023 to 1025, and the monk the Priest's icon, 294;
  with the Nordic skin they had 19519, 19520 and 19523, the Vikings', with icon 910. No check failed.
- **Documents.** `TESTS.md`: Test 31, the lines of Tests 18 to 21 and 30, and the numbers of Tests 32 to 55.
  `MAINTAINER.md`: `src/look/`, the fields of `monk` lines and how the monks are made. `TERMINOLOGY.md`: monk skin.
  `DECISIONS.md`. `DELETION_REQUESTS.md`: the checks.

#### Not done

- The original also gives the monk the villager lines of its gender when the language's monk has the other
  (`_female_monk_civs`); with no language built, nothing of the voice is done.
- The Monastery's own look (`_copy_monastery_skin`) is still not built: the steps offer no choice of it.

### 04:35:41: the civ's hero trained at the Castle from the Imperial Age

#### What changed

- **Why.** The hero chosen in the steps or in the civ file was saved and loaded, but the mod was built without it.
- **The hero.** The new `src/hero/hero_build.c` makes it as the original's `_place_hero` and `_apply_hero_unit` do.
  `hero_find` finds the hero's unit in the slot of the replaced civ. The civ's tech tree makes the unit unavailable,
  and a new tech of the civ, `__hero_imp_enable_` and the slot, free and researched by itself once the Imperial Age
  is, makes it available with an effect of three changes: the unit made available, its attribute 126 set to 1 and 4
  added to its attribute 127, so that one hero lives at a time and a dead one can be trained again. The unit's first
  train location becomes the Castle, button 4, hotkey 16730 and 60 seconds; it costs 500 food and 500 gold; its hero
  mode becomes 1; its hit points become at least 300; both its copy ids become its own id; and it is not disabled.
- **The name.** A named hero's name, label and tooltip point at the slot's hero string id, that id plus 1 000 and
  plus 100 000, and the strings file holds the name, "Create" and the name, and the tooltip plus 21 000 and 100 000,
  as for a unique unit. Each `civ` line of `data/appearance.txt` now ends in that id: the entry of the original's
  `CAMPAIGN_STRING_POOL` at `HERO_POOL_OFFSET`, 465, plus the slot, from 44500 for the Britons to 44702 for the Danes.
- **A civ without the unit.** Its hero is not built, and the build's message says "The hero is not built: the civ
  lacks its unit." (`hero_skipped` in `RecipeReport`).
- **The dat file.** `dat_read_unit` now also records where a unit's copy ids, costs, train locations and hero mode
  are, and `dat_map.h` holds the offsets of its hit points (22), its disabled flag (57) and the fields of a train
  location. `dat_edit_patch_u8` patches one byte. `DatTarget` in `dat_edit.h` replaces `LookTarget`, so the looks and
  the hero name the slot in the same way; `recipe_apply` makes it once.
- **Tests.** 57, up from 55. New: Test 32, a hero trains at the castle, and Test 33, a hero's name is written under
  the string id of its slot. Tests 32 to 55 are now 34 to 57. The small dat file of the dat tests can now hold a unit
  that is trained, with one train location.
- **Checked.** All 57 tests passed in the analyzed build, without a warning or a finding. With the castle made 83,
  Test 32 failed, and with the unique unit's string id taken for the hero, Test 33 failed; both were undone. Two mods
  were built from the installed game into new folders outside the project. With Liu Bei (1966) named "Testhero" in
  the slot of the Aztecs, the unit had name id 44519, a train location of 60 seconds at building 82, button 4 and
  hotkey 16730, costs of 500 food and 500 gold, hero mode 1 and its 425 hit points; the tech tree held the change that
  makes it unavailable, tech 1518, `__hero_imp_enable_15`, needed tech 103 and fired effect 1510 with the three
  changes, and the strings held the four lines. With Aethelflaed (169), of 140 hit points and trained nowhere, in the
  slot of the Britons, the unit got 300 hit points and the same train location. No check failed.
- **Documents.** `TESTS.md`: Tests 32 and 33, the lines of Tests 19 to 22, 30 and 31, and the numbers of Tests 34 to
  57. `MAINTAINER.md`: `src/hero/`, the diagram, the fields of `civ` lines and how the hero is made; a line of 137
  characters in its paragraph on the looks was wrapped. `TERMINOLOGY.md`: train location, and the hero as short for
  hero unit. `README.md`: the hero's name is among the names that take the place of lines of the game's campaigns.
  `DECISIONS.md`. `DELETION_REQUESTS.md`: the checks.

#### Not done

- The original's `_resolve_button_collisions` is not built: a unit of the Castle that also takes button 4, such as
  the Kipchak that Cuman Mercenaries makes available, is not moved to the second page, and the two share the button.
- The hero's `description`, `flags` (more regeneration) and `overrides` (hit points, attack, armour, speed, train
  time) in the original's civ file are not read, and the steps offer none of them. Its tooltip is the plain one,
  "Create" and the name with its cost; the original renames the game's own tooltip of the unit, with its stats.
- The strings of a named hero are written even when the civ lacks its unit; they then name a hero that is not there.
- The game was not started with the mod; its file was read back only.

### 05:05:16: the civ list in the data mod naming the civ's unique techs and unique unit

#### What changed

- **Why.** The game names a civ's unique techs, its unique unit and that unit's line from its civ list,
  `civilizations.json`, and the mod held none, so the civ kept those of the replaced civ, as the original found: the
  Britons stayed on the Longbowman. The original writes the list into its data mod with the slot's entry changed
  (`patch_civilizations_list`).
- **The civ list.** The new `src/civlist/civ_list.c` writes the game's list anew with the slot's entry changed:
  `unique_tech_id_1` and `unique_tech_id_2` become the copied Imperial and Castle techs; with a unique unit of the
  game, `unique_unit_id`, `elite_unique_unit_id` and `unique_unit_upgrade_id` become the unit, its elite unit and
  the copied elite tech, and `unique_unit_line`, `unique_unit_image_paths` and `unique_unit_string_ids` are copied
  from the game's entry that has the unit. Only those values are rewritten; the rest of the file is copied as it is.
  `forge.c` writes it as `<prefix>-data\resources\_common\dat\civilizations.json`. `civ_list_entry` finds a
  slot's entry, for this and for the replaced civ's name id, which `forge.c` found by itself before.
- **A unit without an entry.** Three unique units of the catalog are no civ's `unique_unit_id` in the game's list:
  the Bolas Rider, the Ibirapema Warrior and the Temple Guard. For them, `read_own_texts` in `forge.c` takes the
  icon, the name id and the help id less 79 000 from the unit in the dat file, and the replaced civ's line is kept,
  as the original keeps it. Of the game's 62 entries, the unit's own icon matches in 61, its name id in 58 and its
  help id less 79 000 in 61, so an entry's own values are taken wherever there is one.
- **A named unit.** A unique unit that the civ names or describes gets the slot's unit string id and that id plus
  21 000, the tooltip's, as the game's entries have them: 5107 and 26107 for the Longbowman. The original writes the
  description id it is given, or the name id plus its `DLL_HELP_OFFSET`.
- **The ids.** `RecipeReport` now holds the ids of the two copied unique techs, `unique_tech_ids`, and
  `recipe_unique_unit_ids` in `recipe_copy.c` gives the unique unit, its elite unit and the copied elite tech. The
  list is made before the dat file is packed: the packing reuses the buffer of the game's dat file, which these read.
- **Tests.** 59, up from 57. New: Test 34, the civ list names the civ's own unique unit, and Test 35, a unique unit
  missing from the civ list takes its own icon and texts. Tests 34 to 57 are now 36 to 59. The lines of Tests 23 to
  29 and 33, in `mod_tests.c`, moved down by one.
- **Checked.** All 59 tests passed in the analyzed build, without a warning or a finding. Test 34 failed with the line
  read from a key that is not there, and with the elite unit's id written as the unit's; Test 35 failed with the
  icon's number padded to two digits, and with a unit that no entry has given no texts; each was undone. Four mods
  were built from the installed game into new folders outside the project, in the slot of the Aztecs, and their civ
  lists compared with the game's; only the Aztecs' entry differed. It had the unique techs 1517 and 1516 for 24 and
  460. With the Longbowman, it had the units 8 and 530 and the tech 1515 for 725, 726 and 432, and the Britons' line
  -276 and icon `041_50730.png`. With the Bolas Rider, it had the units 2569 and 2571, the icon `547_50730.png` and
  the Aztecs' line -267. A described unit had the string ids 69832 and 90832; the undescribed Longbowman had the
  Britons' 5107 and 26107, and the undescribed Bolas Rider 5514, "Bolas Rider", and 26514, "Create Bolas Rider" and
  its description.
- **Documents.** `TESTS.md`: Tests 34 and 35, the lines of Tests 23 to 29 and 33, and the numbers of Tests 36 to 59.
  `MAINTAINER.md`: `src/civlist/`, the diagram, the data mod's files and how its civ list is made.
  `TERMINOLOGY.md`: civ list. `DECISIONS.md`. `DELETION_REQUESTS.md`: the checks.

#### Not done

- The game was not started with the mod; its civ list was read back and compared only.
- A unit that no entry of the game has keeps the replaced civ's `unique_unit_line`, as in the original.
- The original's other lists for the game's screens are not built: the tech tree's `civTechTrees`, and the future
  units and unit lines (`patch_future_units`, `patch_unitlines`).

### 05:20:00: fourteen more civ bonuses built from the changes the original's code makes

#### What changed

- **Why.** 43 civ bonuses of the catalog named neither techs nor changes, so a design with one of them was built
  without it. The original builds them in its code (`_create_bonus_handler` in `civ_appender.py`), much of it from
  the game's data: the techs of a building and their costs.
- **The bonuses.** For 14 of them, the changes that code makes were worked out from the installed game's data file
  with check programs outside the project, and written into `data/bonus_effects.txt`, 442 lines; no code changed.
  - 125 and 234: the 15 Blacksmith techs and the 5 Market techs cost no gold (type 101, gold set to 0).
  - 137, 138 and 290: the food of the Blacksmith and Siege Workshop techs, and every cost of the Stable and Barracks
    techs, set to half the game's.
  - 123: every cost of the game's own Dock and University techs less a third, rounded.
  - 131: 100 of resource 92 taken once tech 639 is researched; the 25 changes of the game's tech 289 to the houses;
    and 2 000 of population room and the House made unavailable, which the original adds to the tech tree.
  - 133: the cost of 18 foot archers and 3 skirmishers times 0.9, 0.889 and 0.875 in the Feudal, Castle and Imperial
    Ages.
  - 209: the changes of tech 189, which make the stone walls and gates available, from the start.
  - 258, 278 and 279: for each of the game's own Town Center, University and Monastery techs, 8, 20 and 11 of them,
    villagers carry 1 more, five classes of buildings get 3% more hit points, and a Monk comes out of the Monastery
    and the Fortified Monastery.
  - 325: Husbandry's changes of speed made changes of reload time, by 1/1.1.
  - 401: Forging, Iron Casting and Blast Furnace add 1, 1 and 2 against buildings (class 21) to six classes of units.
  29 civ bonuses are still skipped.
- **Tests.** 59, as before. The data needed no new test: Test 22, a bonus without techs is built from its changes,
  covers how lines become effects and techs.
- **Checked.** All 59 tests passed in the analyzed build, without a warning or a finding. Five mods were built from
  the installed game into new folders outside the project, in the slot of the Britons, and their data files read
  back. With the eleven bonuses 125, 131, 133, 137, 138, 234, 258, 278, 290, 325 and 401, the build reported 11 built
  and 0 skipped, and the file held 42 new techs of the civ, each needing the techs its entry names and firing an
  effect with its changes; with 123, 209 and 279, 3 built and 0 skipped, and 13 new techs.
- **Documents.** `MAINTAINER.md`: which lines of `bonus_effects.txt` come from the original's code and go out of
  date with a game update, and the five string ids of a `civ` line, where it said four. `DECISIONS.md`.
  `DELETION_REQUESTS.md`: the checks.
- **Replaced.** "Bonuses whose code reads the game's data stay unbuilt", of [351 civ bonuses built, with the
  unit-unlocking bonuses and bonus 404](#024933-351-civ-bonuses-built-with-the-unit-unlocking-bonuses-and-bonus-404).

#### Not done

- The game was not started with the mods; their techs and effects were read back only.
- The lines hold the game's techs and costs of 2026-11-09; a game update that adds a tech to one of these buildings
  or changes a cost is not followed.
- 29 civ bonuses are still skipped. Most of them make or copy units or techs, or change the tech tree; 277 could be
  worked out in the same way from the attacks of the gunpowder units, and 339 needs a storage slot opened on the
  Krepost and the Harbor, which a line of changes cannot do.
- `bonus_effects.txt` has 1 962 lines, and the catalog holds 2 048 (`CATALOG_BONUS_CHANGE_CAPACITY`).

### 05:22:54: bonus 195 built, and room for 4 096 lines of changes

#### What changed

- **Bonus 195.** "Blacksmith upgrades are free one age later" is built from 50 new lines of `bonus_effects.txt`, as
  the original's code makes it: once the Castle Age is reached, the costs and the time of the five Blacksmith techs
  of the Feudal Age become 0, and once the Imperial Age is, those of the five of the Castle Age. 28 civ bonuses are
  still skipped.
- **Room.** `CATALOG_BONUS_CHANGE_CAPACITY` in `catalog.h` is 4 096, up from 2 048: `bonus_effects.txt` has 2 012
  lines, and each place takes 100 bytes.
- **Tests.** 59, as before.
- **Checked.** All 59 tests passed in the analyzed build, without a warning or a finding. A mod was built from the
  installed game into a new folder outside the project with bonus 195 in the slot of the Britons: 1 built, 0
  skipped, and two new techs of the civ, needing techs 102 and 103, made Forging, Scale Mail Armor, Scale Barding
  Armor, Fletching and Padded Archer Armor, then Iron Casting, Chain Mail Armor, Chain Barding Armor, Bodkin Arrow
  and Leather Archer Armor free of cost and time.
- **Documents.** `MAINTAINER.md`: bonus 195 among those from the original's code, and the catalog's room for
  changes. `DECISIONS.md`. `DELETION_REQUESTS.md`: the check.

#### Not done

- The game was not started with the mod.

### 05:26:39: bonuses 155, 261 and 277 built

#### What changed

- **The bonuses.** Three more civ bonuses are built from new lines of `bonus_effects.txt`, as the original's code
  makes them; 25 are still skipped.
  - 155 and 261: Elite Battle Elephant (tech 631) and Elite Steppe Lancer (tech 715) cost nothing and take no time.
    The original also frees the Royal Battle Elephant and the Royal Lancer, but only when bonus 309 or 310 has made
    them, and those two are not built here.
  - 277: for each of the game's 20 University techs, 1 more attack of its class, pierce or melee, for 18 gunpowder
    units, 360 lines. The class is the first pierce or melee attack the unit has in the game's data file; the
    attacks were read from civ 0, as the original reads them, by a check program outside the project.
- **Tests.** 59, as before.
- **Checked.** All 59 tests passed in the analyzed build, without a warning or a finding. Two mods were built from
  the installed game into new folders outside the project, in the slot of the Britons. With 155 and 261: 2 built, 0
  skipped, and two new techs of the civ that set the four costs and the time of techs 631 and 715 to 0. With 277: 1
  built, 0 skipped, and 20 new techs, each needing one University tech and adding 769 (pierce, +1) or 1025 (melee,
  +1) to attribute 9 of the 18 units. The attacks of the 18 units were first read wrongly, the count of the list
  taken for an attack, so that four units, 420, 691, 771 and 773, got the wrong class; that was found by reading the
  lists again and mended before the lines were written.
- **Documents.** `MAINTAINER.md`: the three bonuses, and that the lines of 277 hold the units' attacks.
  `DECISIONS.md`. `DELETION_REQUESTS.md`: the checks.

#### Not done

- The game was not started with the mods.
- With bonus 155 or 261, the Royal Battle Elephant and the Royal Lancer are not freed, since bonuses 309 and 310,
  which make them, are not built.

### 05:46:17: bonuses 330, 339 and 458 built from unit patches

#### What changed

- **Why.** Three of the skipped bonuses need more than changes: the original's code sets fields of the civ's own
  units in the dat file. 330 makes the farms 2×2 by setting their sizes from 1.5 to 1.0. 339 opens the third
  storage of the Harbor (1189) and the Krepost (1251), which the game leaves empty, and adds 55 food to that storage
  of 19 buildings. 458 sets the hill mode to 0, any elevation, on every building the Thracians build on any
  elevation.
- **Unit patches.** `bonus_effects.txt` has a third kind of line, `unit`: bonus id, unit, the field's place in the
  unit, its type (`u8`, `i16` or `f32`) and its value. They share the file of changes because `data/` already holds
  7 files, the most Rule 14 allows. `catalog.c` reads them into `unit_patches` of the `Catalog`,
  up to 512 (`CATALOG_UNIT_PATCH_CAPACITY`). After a bonus's changes, `bonus_build_civ` makes its unit patches with
  `patch_unit` in `bonus_build.c`: it finds the unit of the civ with `dat_find_unit` and patches the field; a unit the
  civ lacks is passed over. The place must lie within the first 157 bytes of the unit, which every unit has
  (`DAT_UNIT_FIXED_SIZE` in `dat_map.h`). `BonusBuild` now holds a `DatTarget` in place of the edit and the slot.
  `bonus_build_civ` counts the unit patches with the techs it makes, so a bonus of unit patches alone counts as built.
- **The lines.** 192 new lines, worked out from the game's data file of 2026-11-09 by a check program outside the
  project:
  - 330: 36 unit patches setting to 1.0 the collision, clearance and outline sizes, x and y, of farms 50, 357, 1187
    and 1188, and the clearance and outline sizes of farms 1193, 1194 and 1195, whose collision sizes are 0;
  - 339: 19 changes adding 55 to attribute 27, the third storage, of the Dock in its ages, the Harbor, the Barracks,
    the Stable, the Archery Range and the Siege Workshop in their ages, the Castle and the Krepost, the original's
    `_FOOD_BUILDINGS`; and 6 unit patches setting the third storage of 1189 and 1251 to food, 0, flag 8;
  - 458: 131 unit patches setting the hill mode, at +74, to 0 on each building that the Thracians (slot 55) have at
    0 and every other civ has above 0. Town Centers and wonders keep theirs, as the Thracians' do.
- **The original's conditions.** The original sets a farm's size only where it is 1.5, opens a storage only where it
  is empty, and reads the Thracians' hill modes from the player's dat file. The lines hold what those conditions find
  in the game's data file of 2026-11-09, where each holds in all 63 civs.
- **Tests.** 60, up from 59: Test 36 checks that a unit patch of a bonus sets the field of the unit of its own civ.
  Tests 36 to 59 became 37 to 60. With the new case, `run_mod_test` in `functional_suite.c` would have passed 60
  lines, so its cases 9 to 17 moved to a new group, `run_civ_test`, keeping their numbers.
- **Checked.** All 60 tests passed in the analyzed build, without a warning or a finding. A copy of `bonus_build.c`
  outside the project, made to patch civ 0 in place of the slot, was linked into the tests: Test 36 failed and no
  other. A mod was built from the installed game into a new folder outside the project with bonuses 330, 339 and
  458 in the slot of the Britons: 3 built, 0 skipped. In its dat file the Britons' farm 50 has all six sizes at 1.0
  and farm 1193 its clearance and outline sizes; the Harbor and the Krepost store food, 0, flag 8, in their third
  storage; buildings 10, 1251 and 2745 have hill mode 0, and the Town Center (109) keeps 2; a new tech of the civ,
  free and by itself, fires the 19 changes of 339. The same units of civ 2 kept sizes of 1.5, empty storages and
  hill mode 3.
- **Documents.** `MAINTAINER.md`: the `unit` lines in the table of data files and after the recipe, the bonuses from
  the original's code, the catalog's room and the row of `src/bonus/`. `TERMINOLOGY.md`: unit patch. `TESTS.md`:
  Test 36, and the numbers of Tests 37 to 60. `DECISIONS.md`. `DELETION_REQUESTS.md`: the checks.

#### Not done

- The game was not started with the mod: that the farms are 2×2, that the buildings give food and that they stand on
  any elevation was read from the dat file only.
- 22 bonuses are still skipped: 81, 103, 221, 222, 239, 247, 283, 286, 308, 309, 310, 332, 352, 356, 400, 451, 454,
  455, 456, 457, 459 and 460.

### 05:52:33: compliance with RULES.md and REQUIREMENTS.md

#### What changed: documents

- **`DELETION_REQUESTS.md`.** A file that a check of this section wrote outside the session's scratchpad by mistake,
  `dummy_never_used` in the user's temporary folder, and the check's folder `rulescheck/`.

#### Tests

- 60, as before; all passed in the analyzed build, without a warning or a finding. Tests 41 to 60 check Rules 5, 6,
  9 to 15 and 18 to 22 and the rules of P10; Tests 37 to 40 the accessibility asked of the window.

#### How the rules were read

- **Checked by reading.** Rule 0: the repository has no commit. Rule 2: `REQUIREMENTS.md` holds only the template's
  line 0. Rules 7 and 8: `MAINTAINER.md` has its table of contents, and its first chapter is still "Build and test
  after a change". Rule 9: "unit patch" uses only terms above it. Rule 17: the log names only files, such as
  `AGENTS.md` and `CLAUDE.md`, and "the user". Rule 18: this part has under 1000 lines. Rule 23: each of the 18
  sections `DECISIONS.md` links to has a heading in this part. Rule 25: Test 36 makes one check, for a bonus of the
  original, Req. proposal 3.
- **Rules 1 and 25 without requirements.** `REQUIREMENTS.md` has none yet, so the tests cover the requirement
  proposals of the section at 00:44:18, as before.

#### Not done

- **Rule 4.** The tests run with `build.exe`, but nothing runs them on a push: `.github/` has no workflow, as asked
  in question 4 of 00:44:18.
- **Line 550 of this part** is 170 characters long, more than the log's 120; it belongs to an earlier section, which
  is not rewritten.
- **Rule 24** is still missing from `RULES.md`, as asked in question 6 of 00:44:18.

### 05:56:32: no strings for a hero that is not built

#### What changed

- **Why.** A named hero's strings were written to the UI mod even when the civ lacked the hero's unit, as the section
  at 04:35:41 left undone; they named a hero that was not there, over lines of the game's campaigns.
- **The strings.** `ModCiv` in `mod_text.h` has `hero_skipped`, which `write_ui_mod` in `forge.c` takes from the
  recipe's report; `put_hero_name` in `mod_strings.c` writes the hero's lines only when it is false.
- **Tests.** 61, up from 60: Test 34 checks that a skipped hero's name is not written; Tests 34 to 60 became 35 to
  61. Tests 33 and 34 share `write_hero_strings` in `mod_tests.c`.
- **Checked.** All 61 tests passed in the analyzed build, without a warning or a finding. A copy of `mod_strings.c`
  outside the project, without the new condition, was linked into the tests: Test 34 failed, besides the test of
  `TESTS.md`, which was not yet updated.
- **Documents.** `MAINTAINER.md`: the hero's strings. `TESTS.md`: Test 34, the numbers of Tests 35 to 61, and the
  lines of the tests in `mod_tests.c`. `DECISIONS.md`. `DELETION_REQUESTS.md`: the check.

### 05:57:53: the day of 2026-11-09 at the end of its room in the log

#### What changed

- **The log.** This part has 990 lines, nearly all of them the day 2026-11-09. Rule 18 allows 1000 lines a part,
  and Rule 22 never splits a day, so no further change was made today: its section would not fit.

#### Questions

1. **A day over 1000 lines.** Rules 18 and 22 together allow a day at most 1000 lines of the log. Proposal: a day
   that alone passes 1000 lines goes on in the next part under the same date, marked "continued", and Rule 22 says so.

---

*The day 2026-11-09 goes on in [part 2](DEVLOG2.md#2026-11-09), as Rule 22 allows when a part holds fewer
than two days.*
