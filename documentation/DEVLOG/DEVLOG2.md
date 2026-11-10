# Development log, part 2

- [2026-11-09](#2026-11-09)

Part 2 of the development log. It follows [part 1](DEVLOG1.md), and the day 2026-11-09 goes on here from
[its start in part 1](DEVLOG1.md#2026-11-09), as Rule 22 allows when a part holds fewer than two days. It goes on in
[part 3](DEVLOG3.md). The decisions made in the log are listed in [DECISIONS.md](../DECISIONS.md).

## 2026-11-09

### 10:05:44: the questions of part 1 answered

#### What changed

- **Why.** The questions of part 1, gathered in `sitrep.md`, were answered in `instructions_2.md`, which also gives the
  results of a first trial of civbuilder and of one of its mods in the game, and a new order. The sections after this
  one carry them out.
- **Answers.** By the sections of part 1 that asked them, with their numbers there:
  - [00:44:18](DEVLOG1.md#004418-civbuilder-begun-in-c-with-its-own-build-and-rule-tests): the requirements are
    written in `REQUIREMENTS.md`, from the proposals, with changes (1); GPL-3.0 stays (2); the rules of
    `instructions.md` become rules of `RULES.md`, with rules derived from ub-mode (3); no workflow runs the tests on a
    push, which stays a step done by hand (4); `.claude/settings.json` is written, before any other change (5); Rule
    24 is in `RULES.md` now (6).
  - [01:20:32](DEVLOG1.md#012032-the-nine-steps-in-a-window-with-undo-a-review-and-accessibility-tests): the
    requirements, as above (1); `DeleteObject` and `DestroyWindow` are allowed, since line 16 of `instructions.md` is
    about files and folders on the disk, not objects in the code (2); the classic controls stay (3).
  - [02:15:04](DEVLOG1.md#021504-a-mod-built-from-the-civ-from-the-catalog-and-the-games-own-data-file): the
    compressor written at 02:32:41 stays, with its fixed Huffman codes (1, and question 11 of `sitrep.md`); the two
    folders stay (2); `builder_version` is "1.0.0", not the original's "2.7.0", since this is civbuilder 1.0.0 until
    it is said otherwise (3); "change" stays (4); the requirements, as above (5).
  - [03:19:07](DEVLOG1.md#031907-unique-techs-shown-in-the-game-by-the-names-the-civ-gives-them): the campaign lines
    stay, with the warning in `README.md` (1).
  - [05:57:53](DEVLOG1.md#055753-the-day-of-2026-11-09-at-the-end-of-its-room-in-the-log): Rule 22 was amended: when
    a part holds fewer than two days, a day may take up several parts, its continuation noted at the end of one and
    the start of the next (1). So this part goes on with the day 2026-11-09.
  - Asked in `sitrep.md` only: `sitrep.md` stays in the main folder, and Rule 14 was amended to allow it (9).
- **The requirements.** `REQUIREMENTS.md` has nine: the civ saved and opened as `civbuilder_civ` by default (1), the
  local mods folder (2), WCAG 2.2 AAA (3), with a checklist in `ACCESSIBILITY.md` (4), undo and redo (5), at most 24
  civ bonuses (6), the mod in two folders (7) that replaces one civ (8), and Build saying what it could not build (9).
  The nine steps, the keyboard alone and the contrast of 7:1 are no longer requirements of their own; the last two
  are part of Requirement 3.
- **The trial.** Three bugs were found in the window and three in the game, with one observation:
  - Bug 1: on the step "Civ bonuses", a click on a row scrolled the list up by itself.
  - Bug 2: a ticked box showed a filled rectangle, not a check.
  - Bug 3: the step "Review" had a Next button, with no step after it.
  - Bug 4: the civ selection screen showed the replaced civ's bonuses, not the design's.
  - Bug 5: so did the descriptions of the tech tree screen.
  - Bug 6: the tech tree screen showed many units for a design that had only the Town Center and Villagers.
  - Observation 7: in the game the Barracks was missing, as in the design, though the tech tree screen showed it.
- **The order.** Every part of a civ starts as its base civ's, picked first in the step "Identity" and changeable
  after; the base civ at first is Generic: every universal node, and no regional or unique node.
- **Documents.** `DEVLOG1.md`: a line under its title and a note at its end, which name this part (Rule 22).
  `TERMINOLOGY.md`: universal node, regional node, unique node, base civ and Generic. `DECISIONS.md`.

#### Not done

- With no workflow (question 4 of 00:44:18), Rule 4 holds only when the tests are run by hand before a push.

#### Questions

1. **Files in the main folder.** `instructions_2.md` and four pictures, `image.png` and `image-1.png` to
   `image-3.png`, came into the main folder with the answers. With them it holds 13 files, where Rule 14 allows 8,
   and pictures, which are neither code nor documents; so the tests "no folder over 7 files, the main folder 8" and
   "only C code and documents in the project" fail. Rule 14 lets them stay, but the tests cannot tell why a file is
   there. Proposal: move `instructions_2.md` and the pictures into `materials/`, which Git ignores; the main folder
   then holds 8 files and both tests pass. They were left where they are, as they are not the project's to move.

### 10:06:18: Rules 26 to 36, settings that deny commits and deletions, and version 1.0.0

#### What changed

- **Why.** Answers 3, 5 and 13 of [the questions of part 1 answered](#100544-the-questions-of-part-1-answered),
  Requirement 1, and Rule 22 as amended.
- **Settings.** `.claude/settings.json`, written before any other change, denies in Bash and in PowerShell
  `git commit`, `git push`, `git merge`, `git rebase`, `git cherry-pick`, `gh pr create`, `gh pr merge`, `git rm`
  and `git clean`; the commands that delete, `rm`, `rmdir` and `unlink`, and in PowerShell `Remove-Item`, `del`,
  `erase` and `ri`; and every edit of `REQUIREMENTS.md`.
- **Rules.** Rules 26 to 30 hold what `instructions.md` asks: P10 (26), Head First Design Patterns (27), Clean Code
  (28), C11 with the Win32 API alone (29), and no deletion on the disk, a needed one being listed in
  `DELETION_REQUESTS.md`, while `DeleteObject` and `DestroyWindow` may free objects of the code (30). Rules 31 to 36
  are derived from ub-mode: one decision in one place, two functions 80% alike made one, as its dryer measures (31);
  a CRAP score below 10 for every function, as its crapper computes it (32); a change of behaviour begun with a test
  that fails for the reason expected (33); every mutant of the code that the tests run killed, as its mutator makes
  them (34); the shape of the data before the logic, data checked where it enters (35); and the cause of a fault
  fixed, a structure being changed only while tests pin its behaviour (36).
- **How ub-mode was read.** Its steps that commit, push or make worktrees are left out, as they break Rules 0 and
  30. Its tools are written in Python and read no C, so Rules 31, 32 and 34 name what the tools measure, not the
  tools.
- **Version 1.0.0.** `info.json` of both mods gives `builder_version` "1.0.0", from `forge.c`, in place of "2.7.0".
- **The default name.** For a civ that has no name yet, the Save dialog offers `civbuilder_civ.civbuilder.json`,
  from `suggest_name` in `dialogs.c` (Requirement 1); a named civ is offered its name, as before. The format inside
  the file is still `empireforge_v2`.
- **The log in parts.** `devlog_keeps_its_form` in `tests/rules/log_rules.c` reads every part in turn, `DEVLOG1.md`,
  `DEVLOG2.md` and on while one exists: each part's first line, its days, its list of days and their order. A part
  may begin with the day that the part before it ended with only when that part holds fewer than two days (Rule 22).
- **Tests.** The test of the log reads every part; no test was added here.
- **Checked.** PowerShell's `ConvertFrom-Json` read the settings without an error. In the quick build of 09:53, with
  part 1 alone, the test of the log passed; with this part it runs in the next build, recorded in a later section.
- **Documents.** `RULES.md`: Rules 26 to 36, as answered. `README.md`: the name offered to a civ without one.
  `DECISIONS.md`.

#### Not done

- ub-mode's dryer, crapper and mutator are not written in C, so Rules 31, 32 and 34 are not measured; Rule 34 is
  kept as before, by breaking the code by hand to see each new test fail.
- The Open dialog offers no name: it lists the civ files of the folder, `civbuilder_civ.civbuilder.json` among them.
- The test of the log does not check the links between neighbouring parts.

#### Questions

1. **The default name.** Requirement 1 says the civ "is saved and opened as `civbuilder_civ` by default unless user
   names it". It was read as the name offered for the file, `civbuilder_civ.civbuilder.json`, while the format
   written inside stays `empireforge_v2`, which the original opens. It could also mean that the format itself
   becomes `civbuilder_civ`. Proposal: keep this reading, so that the original still opens civbuilder's files.

### 10:06:34: a list that keeps its place, a check in a ticked box, and no Next on the last step

#### What changed

- **Why.** Bugs 1 to 3 of the trial, in
  [the questions of part 1 answered](#100544-the-questions-of-part-1-answered).
- **The list keeps its place (Bug 1).** `tick_rows` in `sync.c` ticks the rows of a checklist after each command: it
  keeps the list's top row, draws nothing while it works, and changes only the rows whose tick differs from the
  design. `select_rows` hands every checklist to it. Before, every row was set again after each command, and the list
  moved.
- **A check (Bug 2).** `draw_check` in `controls.c` draws a check of two strokes in the ink of a chosen row, as wide
  as the line of the box and scaled to the screen, in place of the filled box.
- **No Next on the last step (Bug 3).** `layout_shows` in `layout.c` hides Next on the step that has no step after
  it, and Back on the step that has none before it.
- **Tests.** One test was added: the last step shows no Next button. Bugs 1 and 2 have none: they are in the drawing
  and scrolling of `src/win/`, which the tests do not run, and an honest test of them would need a window (Rule 33).
- **Checked.** In the quick build of 09:53 the new test passed, with every test but the three of the main folder and
  of `TESTS.md`.
- **Documents.** `DECISIONS.md`.

#### Not done

- Bugs 1 and 2 were not tried again in the window, and no test covers them.
- The new test was not yet seen to fail with `layout_shows` as it was before.

### 10:06:47: the civ's description under a description id of its own

#### What changed

- **Why.** Bugs 4 and 5 of the trial, in
  [the questions of part 1 answered](#100544-the-questions-of-part-1-answered). The civ selection screen and the
  tech tree screen show the string that the civ list names as the civ's `tech_tree_description_string_id`, 120150
  for the Britons. The UI mod wrote the design's description under that id, but another mod that the game loads
  with a higher priority writes its own text for the same id, and the game showed that one.
- **A description id of its own.** `mod_description_sid` in `mod_text.c` gives each slot its own description id,
  990100 plus the slot (`MOD_OWN_DESCRIPTION_BASE` in `mod_text.h`); the game's strings use no id from 990100 to
  990199. `mod_strings_write` in `mod_strings.c` writes the description under it, as well as under the game's id.
  `make_civ_list` in `forge.c` puts it in `description_sid` of `CivListEntry`, and `civ_list_write` in `civ_list.c`
  sets the slot's `tech_tree_description_string_id` to it.
- **Tests.** One test was added: the civ list points at the civ's own description.
- **Checked.** In the quick build of 09:53 the new test passed, with every test but the three of the main folder and
  of `TESTS.md`.
- **Documents.** `TERMINOLOGY.md`: description id. `README.md`: the UI mod given the highest priority among the mods
  that change texts. `DECISIONS.md`.

#### Not done

- The game was not started with a mod built since, so it is not known yet that it reads a string id it does not use
  itself; the original found that the game reads no new ids for unique techs (03:19:07 in part 1).
- Another mod could still write ids from 990100 to 990199.
- The new test was not yet seen to fail with `civ_list_write` as it was before.

#### Questions

1. **The priority of the UI mod.** The civ's name and the names of its unique unit, unique techs and hero keep the
   game's string ids, so a mod of texts with a higher priority can still cover them. civbuilder cannot set a mod's
   priority; the game's mod manager does. Proposal: keep the advice in `README.md` to give the UI mod the highest
   priority, and move the other texts to ids of their own only if the game is seen to read the description's new id.

### 10:07:36: a design begun from a base civ, Generic at first

#### What changed

- **Why.** The order of `instructions_2.md`, in
  [the questions of part 1 answered](#100544-the-questions-of-part-1-answered): every part of a civ starts as its
  base civ's. Before, the step "Tech tree" began with nothing ticked, not even the universal nodes, so a design
  lacked what every civ has unless each node was ticked by hand, as the Barracks in Observation 7 of the trial.
- **The base civs in the catalog.** `data/appearance.txt` has 63 `base` lines: Generic, with the id 0, and the 62
  civs of the game, each by its slot. A line gives the civ's bonuses, team bonus, unique unit, castle and imperial
  techs, architecture, monk skin and scout, by the catalog's ids, with `-` or -1 where the catalog has none. They
  were worked out once from the game's dat file, its `civilizations.json` and the catalog, by check programs outside
  the project.
- **The regions of the tree.** `data/tree.txt` has two columns more: the node's region, `universal`, `regional` or
  `unique`, and the slots of the civs of the game that have it. They come from the game's tech tree files,
  `resources/_common/dat/CivTechTrees/<civ>.json`, one for each civ: a node whose type there is neither regional nor
  unique is universal, and a civ has a node unless its file marks it `NotAvailable`. Of the 205 nodes of the
  catalog, 166 are universal, 37 regional and 2 unique, the Camel Scout and the Folwark.
- **Reading them.** `tsv_bits` in `tsv.c` reads a list of numbers from 0 to 63 into the bits of one 64-bit number.
  `add_base` in `catalog.c` reads a `base` line into a `BaseCiv` of the `Catalog`, and `add_tree_node` sets
  `universal` and `civs` of each `TreeEntry`; `catalog_find_base` in `catalog_lookup.c` finds a base civ by its id.
- **The design.** `CHOICE_BASE` in `civ_design.h` holds the design's base civ, Generic after `civ_design_reset`. The
  civ file keeps it as `base_civ` (`civ_file_write.c`, `civ_file_read.c`); a file without it opens with Generic.
- **Taking a base civ.** `base_civ_apply` in `src/base/base_civ.c`, a new folder, starts the design afresh, keeping
  its texts and its hero, and ticks the nodes the base civ has: every universal node for Generic, else the nodes
  that its civ has in the game. For a civ of the game it also ticks the civ's bonuses and team bonus, picks its
  unique unit and unique techs, and takes its architecture, monk skin and scout, its own language, wonder and castle,
  and its slot as the replaced civ.
- **The step "Identity".** Its first control is the list "Base civ" (`CONTROL_BASE` in `layout.h`, bound in
  `binding.c`). Picking a civ in it runs `apply_base` in `sync.c`, which hands the new design to `history_replace`,
  so Ctrl+Z takes it back (Requirement 5), and the status line says what changed. At start, once the catalog is
  read, `app_window.c` gives the design Generic.
- **Tests.** Six tests were added, in a new folder `tests/base/` (`base_tests.c`, `base_tests.h`): Generic has every
  universal node; choosing Generic takes away a regional node; a base civ has the nodes its civ has in the game; and
  a base civ gives the civ its bonuses, its team bonus and its unique unit. Their numbers are given in a later
  section.
- **Documents.** `TERMINOLOGY.md`: universal node, regional node, unique node, base civ and Generic. `README.md`: the
  base civ, picked first. `DELETION_REQUESTS.md`: the check programs, their tables and two stray files.
  `DECISIONS.md`.

#### Not done

- civexplorer.net, named in the order as the source of what is universal, was not compared node by node; the game's
  own files were read instead.
- Six civs have gaps in the catalog: the Achaemenids, Athenians and Spartans (slots 46 to 48) and the Macedonians,
  Thracians and Puru (54 to 56) have no castle or imperial tech and no monk skin there, so their base civ leaves those
  unpicked. The Athenians and Spartans have no civ bonus in the catalog, and the Achaemenids and Athenians no team
  bonus.
- Bug 6 is not mended here: the tech tree screen still shows the replaced civ's tree, since the data mod writes no
  tech tree file.
- The base civ was not tried in the window or in the game.
- **Slips.** Two files were written outside the scratchpad by mistaken commands:
  `C:\Users\Hjörleif\AppData\Local\Temp\x`, a deletion request already, was written over with the names of the
  game's civs, and `C:\Users\Hjörleif\AppData\Local\scratch_edit.txt` was made, empty, by a command that was stopped.
  Both are in `DELETION_REQUESTS.md`.

#### Questions

1. **The base civ in the requirements.** The order for a base civ is in `instructions_2.md`, not in
   `REQUIREMENTS.md`, while Rule 24 allows a test only for a requirement or for a bug found before. The six tests were
   written for the order, read as a requirement to come. Proposal: a Requirement 10, "A civ starts as its base civ,
   picked first; at first Generic, with every universal node and nothing else."
2. **The civs with gaps.** Should the unique techs and monk skins of the six civs above be added to the catalog, from
   the game's dat file as for the other civs, or should those civs stay out of the list of base civs until they are?
   Proposal: add them to the catalog.

### 10:08:11: a checklist of WCAG 2.2 for Requirement 4

#### What changed

- **Why.** Requirement 4: `ACCESSIBILITY.md` holds a checklist, and civbuilder is WCAG 2.2 AAA compliant
  (Requirement 3) when every item of it holds.
- **Where.** The checklist is `.github/ACCESSIBILITY.md`, the file of that name that came from repo_template, since
  Rule 15 allows one file of a name and `documentation/` holds the 7 files that Rule 14 allows.
- **The checklist.** It lists the 86 success criteria of WCAG 2.2 at levels A, AA and AAA, all of which level AAA
  asks for; 4.1.1 Parsing is obsolete. Each is read as WCAG2ICT reads it for software, the window and its dialogs
  standing for a page. A box is ticked only when a test checks the criterion, or when civbuilder has nothing it
  applies to, as the code shows: 45 are ticked, 41 not. Each unticked line says how civbuilder meets the criterion
  or would meet it, and what is left.
- **What is left.** Most of the 41 need a check by hand, with the keyboard and a screen reader. Eight are known not
  to be met yet: the size of text set in Windows (1.4.4), the window's title (2.4.2), a focus frame two pixels wide
  (2.4.13), the language told to Windows (3.1.1), words of the game and shortened names (3.1.3, 3.1.4), help for
  each control (3.3.5), and the status line told to a screen reader (4.1.3).
- **README.md.** Its steps start with the base civ, name the name offered to a civ without one, and advise giving
  the UI mod the highest priority.
- **Tests.** None was added; the checklist names the tests of accessibility by their purpose.
- **Documents.** `.github/ACCESSIBILITY.md`. `README.md`. `DECISIONS.md`.

#### Not done

- No criterion was checked by hand, and no screen reader was tried.
- The eight criteria not met are not mended.

#### Questions

1. **What "evaluated as true" takes.** A criterion that has nothing to apply to, such as captions without video, was
   ticked, as WCAG counts it met. Proposal: keep this, each tick with its evidence on its line.
2. **Reflow (1.4.10).** It asks that content fit 320 pixels wide at 400% without scrolling both ways, where the
   controls of a step need a window of 960 by 560. Proposal: lay a step out in one column when the window is
   narrower than 960, and add that to the tests of the layout.

### 10:18:41: a tech tree screen that shows the design's tree, and all 71 tests listed

#### What changed

- **Why.** Bug 6 of the trial: the tech tree screen of a design with only the Town Center and villagers showed many
  units, those of the replaced civ. The game draws that screen from `resources/_common/dat/CivTechTrees/<name>.json`,
  which the data mod did not hold, so the game's own file of the replaced civ was shown. Observation 7, no Barracks
  in the game, matched the design; with Generic at the start, a new design has the Barracks.
- **The tech tree file.** The data mod holds `resources\_common\dat\CivTechTrees\<name>.json`, `<name>` being the
  replaced civ's `tech_tree_name` in the game's `civilizations.json`, such as `BRITONS`. It is the game's file with
  each node that `tree.txt` holds set to the design: `NotAvailable` when the design lacks it, `ResearchedCompleted`
  when the design has it and the replaced civ had not. The other nodes, such as the unique ones, stay as they were.
  Nodes are matched by `Use Type` and `Node ID` together. `tech_tree_write` in `src/techtree/tech_tree.c`, a new
  folder, sets the nodes; `tech_tree_file.c` finds the name, reads the game's file and writes the mod's; and
  `write_tech_tree` in `forge.c` gives them the paths. When it fails, the build says "The civ's tech tree could not
  be written into the data mod." (Requirement 9).
- **One way to change a JSON text.** The list of changes that `civ_list.c` kept, with its sorting and writing out,
  moved to `src/json/json_patch.c`, so that the civ list and the tech tree file are changed the same way (Rule 31):
  each change replaces one value, a string with its quotes, and every other byte is copied. A patch holds up to
  1 024 changes, where the civ list's list held 10. `CivListOutput` is now a `JsonPatchOutput`; the civ list is
  written as before, and its tests passed unchanged.
- **The base civ's tests in the project's terms.** The three tests of `tests/base/` that said "item" say "node", as
  `TERMINOLOGY.md` does. Two were made exact: Generic gives exactly the universal nodes, from a catalog that holds a
  universal row past its count, which must not be read; and a base civ gives exactly its own bonuses.
- **Tests.** 71, up from 63. Tests 40 to 45 are the base civ's (`tests/base/base_tests.c`), and Tests 46 and 47 the
  tech tree's (`tests/mod/mod_tests.c`): a building the civ lacks is not available in its tech tree, and one it has
  is. They run in a new group, `run_base_test` in `functional_suite.c`, which keeps `run_civ_test` within 60 lines.
  The accessibility tests are now Tests 48 to 51, and the maintainability tests 52 to 71.
- **Checked.** `build.exe`, with the analyzer: every file compiled without a warning or a finding; 71 tests ran and
  69 passed. Tests 64 and 66 failed only on the main folder's `instructions_2.md`, `image.png`, `image-1.png`,
  `image-2.png` and `image-3.png`, as asked in
  [the questions of part 1 answered](#100544-the-questions-of-part-1-answered) (1). Each new test was shown to fail
  by mutants of the code it guards, made one at a time by a check program outside the project and undone: 10
  mutants of `base_civ.c` (`==` and `!=`, `<` and `<=`, `>=` and `>`, 0 and 1), all killed once Tests 40 and 43
  were made exact, since two `<` made `<=` had lived before; 7 of `tech_tree.c` and 4 of `json_patch.c`, all
  killed. Two mods were built into the scratchpad, from the catalog and the installed game, by a check program:
  with the Britons as base civ, the mod's `BRITONS.json` was the game's, byte for byte; with Generic, the universal
  nodes the Britons lack, such as the Bombard Tower, Hand Cannoneer, Cavalry Archer, Hussar, Paladin, Bloodlines,
  Siege Ram and Siege Onager, were `ResearchedCompleted`, and the Britons' regional nodes, the Mounted Crossbowman,
  Heavy Mounted Crossbowman and Cranequins, `NotAvailable`.
- **Documents.** `TESTS.md`: made anew from its entries, with Tests 14 and 39, added earlier, and Tests 40 to 47;
  the numbers after them; the lines of every functional test and of the four tests in `log_rules.c`; each
  "Req. proposal N" as the requirement it now is (Requirements 1, 3, 5, 6, 7, 8 and 9); and "Rule 25, a regression
  test" as Rule 24. `MAINTAINER.md`: the table of files (`src/base/`, `src/techtree/`, `json_patch.c`, `sitrep.md`,
  `.claude/settings.json`, `.github/ACCESSIBILITY.md`), the diagram, the `base` lines and the new columns of
  `tree.txt`, a chapter "Base civs", the tech tree file and the description id in "How a mod is built", and the
  folders of the tests. `DELETION_REQUESTS.md`: the check programs and outputs of this section. `DECISIONS.md`.

#### Not done

- The mod was not tried in the game: whether the tech tree screen now shows the design is for the next trial.
- A node that the replaced civ lacked gets `ResearchedCompleted`, the status of most nodes a civ has in the game's
  files. Nodes that the game marks `ResearchRequired` for the civs that have them may then show as researched.
- The base civ and Bugs 1 and 2 were not tried in the window.
- Mutants that only a broken bound or guard would show, such as `index < NODE_LIMIT` made `index <= NODE_LIMIT`,
  were not made; nor were mutants of the files this section did not change.
- Rules 32 and 34 still have no tool in C: no CRAP score was measured.

#### Questions

1. **The status of a node the replaced civ lacked.** The game's files mark a node a civ has `ResearchedCompleted`,
   or for a few nodes `ResearchRequired`. Proposal: keep `ResearchedCompleted`, and if the trial shows a node drawn
   wrongly, write in `tree.txt` the status the node has for the civs that have it.

### 10:27:22: a window's title that names the civ and the step, and the base civ tried in the window

#### What changed

- **Why.** The checklist of Requirement 4 found 2.4.2 Page Titled not met: the window's title was "civbuilder" on
  every step, whatever civ was open.
- **The title.** `details_window_title` in `src/present/details.c` makes the title from the civ's name, or "A new
  civ" before it has one, the step's title and "civbuilder", as "Testonia - Step 2 of 9: Tech tree - civbuilder".
  `sync_details` in `sync.c` sets it on the window, so it follows every change of name and of step.
- **Tests.** 72, up from 71. Test 52, "the window's title names the civ and the step", is an accessibility test
  (Requirement 3); the maintainability tests are now Tests 53 to 72.
- **Checked.** `build.exe quick`: 72 tests run, 70 passed; Tests 65 and 67 failed on the main folder's files, as
  before. Test 52 failed first, against a `details_window_title` that returned nothing, and passed once the function
  was written; the two `!=` of the function made `==` were each caught by Test 52. In the running program, the
  title read "A new civ - Step 1 of 9: Identity - civbuilder" at the start and "A new civ - Step 2 of 9: Tech tree -
  civbuilder" on the step "Tech tree".
- **The base civ in the window.** The program was run and driven by a check program outside the project, which sent
  it the window messages of the menu "Go", of picking in a list and of undo, and saved pictures of the window,
  without taking the keyboard. At the start, the step "Identity" shows "Base civ: Generic" first, and the step
  "Tech tree" shows 54 units, 29 buildings and 83 techs ticked, the universal nodes. Picking the Franks set the
  castle, language and wonder to the Franks, the monk skin to European and the scout to the Scout Cavalry, marked
  steps 2 to 7 done, ticked the Franks' three civ bonuses, and showed "The civ now has the tech tree, bonuses and
  looks of its base civ."; undo brought Generic back.
- **Bugs 1 and 2 in the window.** With the list of civ bonuses scrolled to row 30, ticking row 34 left it at row 30,
  where before the list went back to its top (Bug 1). Ticked rows show a check, not a filled box (Bug 2).
- **Documents.** `TESTS.md`: Test 52, the numbers of Tests 53 to 72, and the lines of the four other tests of
  `access_tests.c`. `.github/ACCESSIBILITY.md`: 2.4.2 ticked, by Test 52; 46 boxes are ticked, 40 not.
  `MAINTAINER.md`: `details.c` makes the window's title. `DELETION_REQUESTS.md`: the check program and its pictures.
  `DECISIONS.md`.

#### Not done

- No screen reader was tried. The check program read the controls through UI Automation without the proxies that
  Narrator uses, so it could not show whether each list and field is named by its label (4.1.2).
- The mod was not tried in the game.

### 10:40:46: one rule for what a civ keeps, in the dat file and the tech tree file alike

#### What changed

- **Why.** The tech tree file of
  [10:18:41](#101841-a-tech-tree-screen-that-shows-the-designs-tree-and-all-71-tests-listed)
  made `NotAvailable` every node of `tree.txt` that the design lacked, while the dat file keeps more: with nothing
  ticked, the civ keeps the game's whole tree ("An empty tech tree disables nothing"), and every civ keeps the
  Villager, both Town Centers, the Mill and the two camps, which `recipe_tree.c` never disables. The tech tree screen
  would then have shown an empty tree with nothing available, and every other tree without a Villager.
- **One place for the rule.** `civ_design_tree_is_empty` and `civ_design_tree_keeps` in `src/model/civ_design.c`
  decide what a civ keeps: the whole tree when it is empty; otherwise each node the design has, and for units and
  buildings the six ids of `ALWAYS_KEPT`, the list `recipe_tree.c` held before as `ALWAYS_ON`. Techs are left out of
  it, since id 68 is both the Mill and the tech Iron Casting. `recipe_tree.c` and `tech_tree.c` both ask these
  functions (Rule 31); the dat file is made as before.
- **A node the civ has keeps its research.** A node the design has is written as the game's file has it, unless that
  is `NotAvailable`; a Keep, which the game marks `ResearchRequired`, still needs its research. This was so before,
  and a test now holds it.
- **Tests.** 75, up from 72. Tests 48 to 50 are new, in `tests/mod/mod_tests.c`: a civ with an empty tree keeps the
  game's tech tree; a civ keeps its villagers in its tech tree; a building the civ has keeps the research it needs in
  its tech tree. The accessibility tests are now Tests 51 to 55, and the maintainability tests 56 to 75.
- **Checked.** `build.exe quick`: 75 tests ran and 73 passed; Tests 68 and 70 failed on the main folder's files, as
  before. Tests 48 and 49 failed first, against the tech tree writer of 10:18:41, and passed once the rule was in one
  place. Mutants, made one at a time by a check program outside the project and undone: 5 of the two new functions
  (`==` and `!=`, `&&` and `||`), all killed, by Tests 46 to 49; 3 of `tech_tree.c` (two `!` taken away, and `&&`
  made `||`). The `&&` made `||`, which turned a Keep's `ResearchRequired` into `ResearchedCompleted`, lived; Test
  50 was written for it and kills it. The recipe's tests passed unchanged.
- **Documents.** `TESTS.md`: Tests 48 to 50, the numbers of Tests 51 to 75, and the lines of Tests 46 and 47.
  `MAINTAINER.md`: in "How a mod is built", that a node the design has keeps its status, and where what a civ keeps
  is decided. `TERMINOLOGY.md`: "tech tree file" added, and "UI mod" no longer said to hold the tech tree, which the
  data mod holds. `DELETION_REQUESTS.md`: the check programs, copies and outputs of this section and of the window's
  title. `DECISIONS.md`, with the decision of the window's title, which the section of
  [10:27:22](#102722-a-windows-title-that-names-the-civ-and-the-step-and-the-base-civ-tried-in-the-window)
  had left out.
- **Replaced.** The tech tree file's `NotAvailable` for every node the design lacks, of 10:18:41: it is now for
  every node the civ does not keep.

#### Not done

- The mod was not tried in the game.
- Mutants that only a broken bound or guard would show were not made, as before.
- Rules 32 and 34 still have no tool in C: no CRAP score was measured.

### 10:47:03: the tech tree file of two built mods, and the game's statuses counted

#### What changed

- **Checked.** Two mods were built into the scratchpad from the catalog and the installed game, by the check program
  of 10:18:41 made anew from the objects of the build, the Britons being the replaced civ. With an empty tree, the
  mod's `BRITONS.json` is the game's, byte for byte. With Generic and the Villager and the Mill unticked, both stay
  `ResearchedCompleted`, as do both Town Centers and the two camps; the Keep, Guard Tower and Fortified Wall stay
  `ResearchRequired`; and 24 of the 172 nodes differ from the game's file: 21 universal nodes that the Britons lack
  are `ResearchedCompleted`, and the Britons' Mounted Crossbowman, Heavy Mounted Crossbowman and Cranequins
  `NotAvailable`.
- **The game's statuses counted.** The statuses of the game's 62 `CivTechTrees` files were counted by `Use Type`
  and `Node ID`, leaving out `NotAvailable`. Of the 21 nodes above, four are `ResearchRequired` for nearly every civ
  that has them: the building Bombard Tower for 18 of 18, the Carrack for 47 of 48, the Hand Cannoneer for 23 of 24
  and the Bombard Cannon for 23 of 25. The other 17, such as the Cavalry Archer, Paladin and Bloodlines, are
  `ResearchedCompleted` for every civ that has them, or for all but one.
- **Documents.** `DELETION_REQUESTS.md`: the new check program, its mods and tables, and the analyzed build's output.

#### Not done

- The tech tree file still writes `ResearchedCompleted` for the four nodes above, as the open question of 10:18:41
  proposed until a trial shows otherwise.

#### Questions

1. **The status of a node the replaced civ lacked, with the game's numbers.** This adds to question 1 of
   [10:18:41](#101841-a-tech-tree-screen-that-shows-the-designs-tree-and-all-71-tests-listed): a Generic design
   that replaces the Britons gets the Bombard Tower, Carrack, Hand Cannoneer and Bombard Cannon as
   `ResearchedCompleted`, which almost no civ of the game has. Proposal: a column of `tree.txt` with the status that
   most civs that have the node give it, read once from the game's files, written for a node that the replaced civ
   lacked.

### 10:51:59: sitrep.md brought up to date for the next trial

#### What changed

- **Why.** `sitrep.md` held the questions of 08:06, which `instructions_2.md` answered, and steps of testing that no
  longer matched the program, such as a tech tree screen that the mod did not write.
- **sitrep.md.** A new first part, of the present: what changed since 08:06, the nine questions still open, with
  their proposals, and how to test what is new, in the program and in the game. The part of 08:06 is kept below it,
  since `instructions_2.md` answers it by its numbers.
- **The game's abbreviations counted.** The game's English strings say "HP" 236 times, "hit points" once, "LOS"
  twice and "line of sight" 74 times.
- **Documents.** `sitrep.md`.

#### Not done

- 3.1.4 Abbreviations stays unticked: the catalog's texts shorten words, as question 1 says.

#### Questions

1. **Abbreviations (WCAG 2.2 SC 3.1.4).** The texts of `data/` shorten words: "HP" 42 times, "vs" 41, "LOS" or
   "LoS" 17 and "TC" once, besides "Skirms", "stone def" and the like; two nodes of `tree.txt` are "H. Mounted
   Crossbowman" and "E. Varangian Guard", which the game names in full. Proposal: spell them out in `data/` ("hit
   points", "line of sight", "against", "Town Center"), the two names as the game names them, with a test that no
   text of the catalog holds them.

### 10:55:20: the dat file's tree, which no test runs

#### What changed

- **Why.** This corrects the **Checked.** point of
  [10:40:46](#104046-one-rule-for-what-a-civ-keeps-in-the-dat-file-and-the-tech-tree-file-alike), which said that
  the recipe's tests passed unchanged. They did, but none of them runs `recipe_wire_tree`, the part of the recipe
  that disables in the dat file the techs of what the civ lacks.
- **Checked.** 5 more mutants of `civ_design_tree_is_empty` and `civ_design_tree_keeps` (two `==` made `!=`, an
  `&&` and an `||` swapped, a `!` taken away) were all killed, by Tests 46 to 50: 10 of the two functions in all.
  2 mutants of `recipe_tree.c` lived: `||` made `&&` in `unit_chosen`, and a `!` put before
  `civ_design_tree_is_empty` in `recipe_wire_tree`. The dat file of a mod built at 10:54 for a Generic design, with
  the code of this morning's last change, is byte for byte the one built at 10:10 for the same design with the code
  before the rule moved: 13 487 812 bytes. So the move did not change the dat file of that design; no test holds it.
- **Documents.** `DELETION_REQUESTS.md`: the new mutants and the new mod.

#### Not done

- No test runs the dat file's tree (Rule 34). One could be built on the small dat file of `tests/dat/dat_tests.c`:
  a tech that enables a unit the design lacks, which `recipe_wire_tree` should disable, and one that enables the
  Villager, which it should not.

### 10:57:00: keys sent to a window that was not civbuilder's

#### What changed

- **A slip.** At 10:20 and 10:21, before the check program of
  [10:27:22](#102722-a-windows-title-that-names-the-civ-and-the-step-and-the-base-civ-tried-in-the-window) sent
  window messages, two scripts sent keys with `SendKeys`, which types into whatever window is active: Ctrl+1,
  Ctrl+2, Ctrl+3 and Down. civbuilder's window was not always the active one, so some of the keys may have gone to
  another program. They type no text, and no file of the project changed in those two minutes; what they did in
  another program is not known. The scripts are `shoot.ps1` and `shoot2.ps1` in `shots/`, listed in
  `DELETION_REQUESTS.md`; keys were not sent again.

### 10:59:58: a test of the dat file's tree

#### What changed

- **Why.** The section of [10:55:20](#105520-the-dat-files-tree-which-no-test-runs) found that no test runs
  `recipe_wire_tree`, so two of its mutants lived.
- **Tests.** 76, up from 75. Test 51, "only the tech of a unit the civ lacks is disabled", in
  `tests/dat/dat_tests.c`: the small dat file of the dat tests gets two techs, each with an effect that enables one
  unit, and for a design with only the first unit `recipe_wire_tree` disables the second tech alone. It fails if
  the function does nothing. The accessibility tests are now Tests 52 to 56, and the maintainability tests 57 to 76.
- **Checked.** `build.exe quick`: 76 tests ran and 74 passed; Tests 69 and 71 failed on the main folder's files, as
  before. The two mutants of `recipe_tree.c` that had lived, `||` made `&&` in `unit_chosen` and a `!` put
  before `civ_design_tree_is_empty`, both made Test 51 fail.
- **Documents.** `TESTS.md`: Test 51, the numbers of Tests 52 to 76, and the lines of the eight other tests of
  `dat_tests.c`, one lower for its new `#include`. `sitrep.md`: the count of tests and the numbers of the two that
  fail. `DELETION_REQUESTS.md`: the copies and outputs of this section.

#### Not done

- Test 51 does not cover the Villager kept by every civ; the tech tree file's Test 49 does, and `civ_design_tree_keeps`
  decides both.

### 11:01:06: the build with the analyzer after the morning's changes

#### What changed

- **Checked.** `build.exe`, with the analyzer, at 10:42 and again at 11:00 after Test 51: every file compiled
  without a warning or a finding of the analyzer. 76 tests ran and 74 passed; Tests 69 and 71 failed only on the main
  folder's files, as asked in [the questions of part 1 answered](#100544-the-questions-of-part-1-answered) (1). The
  program built at 10:53 started, and its title read "A new civ - Step 1 of 9: Identity - civbuilder", then "A new
  civ - Step 2 of 9: Tech tree - civbuilder" on the step "Tech tree".

### 11:02:08: what Test 51 reaches of the dat file's tree

#### What changed

- **Checked.** 10 more mutants of `recipe_tree.c`, made one at a time outside the project and undone, against the
  76 tests. Test 51 killed 5: `!= 0` made `== 0` for an enabled unit, `||` made `&&` in finding a unit of the
  catalog, `&&` made `||` for a kept tech that a civ unlocks, and each `!` taken away before `kept` and
  `shielded` in `emit_changes`. 5 lived:
  - `&&` made `||` between `RECIPE_ENABLE_UNIT` and `b != 0` in `note_unit_change`: no change of another type;
  - `tech.civ >= 0` made `> 0` in `scan_tech`: no tech of a civ;
  - `== RESOURCE_CHANGE` made `!=`, and the first `&&` of the last line of `scan_tech` made `||`: no tech that
    only gives resources and is researched nowhere;
  - a `!` put before `civ_design_tree_has` in `mark_catalog_techs`: no tech of the catalog.
- **Documents.** `DELETION_REQUESTS.md`: the mutants are in `mut/`, listed there.

#### Not done

- The 5 mutants above live (Rule 34). Each needs a test on the small dat file, as Test 51 is: a tech of a civ that is
  never disabled, a tech of resources only that is never disabled, and a tech of the catalog that the design has and
  keeps.

### 11:05:16: a test of the techs the dat file disables

#### What changed

- **Why.** Of the 5 mutants that lived in [11:02:08](#110208-what-test-51-reaches-of-the-dat-files-tree), the one
  in `mark_catalog_techs` needs only a tech of the catalog, which the small dat file of Test 51 can hold.
- **Tests.** 77, up from 76. Test 52, "only a tech the civ lacks is disabled", in `tests/dat/dat_tests.c`: techs 0
  and 1 are both in the catalog, the design has tech 0, and `recipe_wire_tree` disables tech 1 alone. Test 51 and
  Test 52 share `design_has_first_of` and `only_second_tech_is_disabled`, so that they are not two copies (Rule
  31). The accessibility tests are now Tests 53 to 57, and the maintainability tests 58 to 77.
- **Checked.** `build.exe`, with the analyzer: no warning and no finding; 77 tests ran and 75 passed; Tests 70 and
  72 failed on the main folder's files, as before. The `!` put before `civ_design_tree_has` in
  `mark_catalog_techs` made Test 52 fail; the two mutants that Test 51 killed before still made it fail.
- **Documents.** `TESTS.md`: Test 52, the lines of Test 51, and the numbers of Tests 53 to 77. `sitrep.md`: the
  count of tests and the numbers of the two that fail. `DELETION_REQUESTS.md`: the copies and outputs.

#### Not done

- 4 mutants of `recipe_tree.c` still live, those of a change of another type, a tech of a civ, and a tech of
  resources only, as [11:02:08](#110208-what-test-51-reaches-of-the-dat-files-tree) lists.

### 11:06:58: Rules 33 and 34 in the guide's rules for changing the code

#### What changed

- **MAINTAINER.md.** "Rules for changing the code" names Rule 33, a test that fails before the change, and Rule 34,
  with how to check it by hand: swap one operator of the code, run `build.exe quick`, see the test fail, and undo
  the swap. The mutants of this morning were made that way, by a script outside the project.
- **Documents.** `MAINTAINER.md`.

#### Not done

- Rules 32 and 34 still have no tool in C, though the answer to question 3 of part 1 allows one to be built.

### 11:10:18: a test that the dat file keeps the Villager

#### What changed

- **Why.** Every civ keeps the Villager, both Town Centers, the Mill and the two camps ("An empty tech tree disables
  nothing"). Test 49 holds it for the tech tree file, but nothing held it for the dat file: `unit_chosen` in
  `recipe_tree.c` could ask `civ_design_tree_has` again, as before 10:40:46, and no test would fail, while the civ
  could then train no villager.
- **Tests.** 78, up from 77. Test 53, "a civ keeps its villagers in the dat file", in `tests/dat/dat_tests.c`: for a
  design without the Villager, of two techs that enable the Villager and another unit, `recipe_wire_tree` disables
  the second alone. The accessibility tests are now Tests 54 to 58, and the maintainability tests 59 to 78.
- **Checked.** `build.exe`, with the analyzer: no warning and no finding; 78 tests ran and 76 passed; Tests 71 and
  73 failed on the main folder's files, as before. With `unit_chosen` asking `civ_design_tree_has` instead of
  `civ_design_tree_keeps`, made outside the project and undone, Test 53 failed; no other test failed but
  those that failed before, the two of the main folder and that of `TESTS.md`, which did not list Test 53 yet.
- **Documents.** `TESTS.md`: Test 53 and the numbers of Tests 54 to 78. `sitrep.md`: the count of tests and the
  numbers of the two that fail. `DELETION_REQUESTS.md`: the copies and outputs.
