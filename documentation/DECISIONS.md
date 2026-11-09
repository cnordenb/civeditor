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

- **The rules of `RULES.md` hold, Rules 0 to 36.** Rules 0 to 25 are repo_template's, as amended outside the log,
  Rules 26 to 30 hold what `instructions.md` asks, and Rules 31 to 36 are derived from ub-mode; Rules 10 to 13 apply
  to code only (Rule 25). Replaced: Rules 0 to 23, 25 and 26 of repo_template.
  [2026-11-09 00:44:18](DEVLOG/DEVLOG1.md#004418-civbuilder-begun-in-c-with-its-own-build-and-rule-tests)
  [2026-11-09 10:06:18](DEVLOG/DEVLOG2.md#100618-rules-26-to-36-settings-that-deny-commits-and-deletions-and-version-100)
- **ub-mode's rules as far as they fit C.** Its steps that commit, push or make worktrees are left out (Rules 0 and
  30). Its dryer, crapper and mutator may be written in C; until they are, Rule 34 is kept by breaking the code by
  hand.
  [2026-11-09 10:06:18](DEVLOG/DEVLOG2.md#100618-rules-26-to-36-settings-that-deny-commits-and-deletions-and-version-100)
- **The settings deny what the rules forbid.** `.claude/settings.json` denies commits, pushes, merges, rebases,
  cherry-picks and pull requests, the commands that delete, and every edit of `REQUIREMENTS.md`.
  [2026-11-09 10:06:18](DEVLOG/DEVLOG2.md#100618-rules-26-to-36-settings-that-deny-commits-and-deletions-and-version-100)
- **No workflow runs the tests.** `.github/` holds none; the tests are run by hand before a push (Rule 4).
  [2026-11-09 10:05:44](DEVLOG/DEVLOG2.md#100544-the-questions-of-part-1-answered)
- **A day may go on in the next part.** When a part holds fewer than two days, its day goes on in the next part,
  and each notes it, as 2026-11-09 goes on from `DEVLOG1.md` in `DEVLOG2.md` (Rule 22).
  [2026-11-09 10:05:44](DEVLOG/DEVLOG2.md#100544-the-questions-of-part-1-answered)
- **An order is read as a requirement to come.** The tests of the base civ cover the order of `instructions_2.md`,
  which `REQUIREMENTS.md` does not hold yet (Rule 24).
  [2026-11-09 10:07:36](DEVLOG/DEVLOG2.md#100736-a-design-begun-from-a-base-civ-generic-at-first)
- **P10 is read rule by rule.** No `goto` or recursion, `for` loops bounded by a named constant, no allocation, 60
  lines a function, two checks a function on average, one level of dereferencing, no function pointers, all warnings.
  [2026-11-09 00:44:18](DEVLOG/DEVLOG1.md#004418-civbuilder-begun-in-c-with-its-own-build-and-rule-tests)
- **One function pointer.** The window procedure, in `src/win/app_window.c` only, since Win32 needs it.
  [2026-11-09 00:44:18](DEVLOG/DEVLOG1.md#004418-civbuilder-begun-in-c-with-its-own-build-and-rule-tests)
- **The checker is not counted.** `src/core/check.c` has no checks of its own, as they would call it again.
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

## Requirements, and how they are read

- **`REQUIREMENTS.md` holds the requirements,** nine, made from the proposals of part 1 with changes; the original
  settles what they leave open. Replaced: the original was the requirement until `REQUIREMENTS.md` was written.
  [2026-11-09 00:44:18](DEVLOG/DEVLOG1.md#004418-civbuilder-begun-in-c-with-its-own-build-and-rule-tests)
  [2026-11-09 10:05:44](DEVLOG/DEVLOG2.md#100544-the-questions-of-part-1-answered)
- **The default name is `civbuilder_civ`.** Requirement 1 is read as the name offered for a civ that has none,
  `civbuilder_civ.civbuilder.json`; the format inside stays `empireforge_v2`.
  [2026-11-09 10:06:18](DEVLOG/DEVLOG2.md#100618-rules-26-to-36-settings-that-deny-commits-and-deletions-and-version-100)
- **A design starts from a base civ,** picked first in the step "Identity": its tech tree, bonuses, team bonus,
  unique unit, unique techs and looks, all of which can be changed after; picking one can be undone.
  [2026-11-09 10:07:36](DEVLOG/DEVLOG2.md#100736-a-design-begun-from-a-base-civ-generic-at-first)
- **Generic at first.** A new design has every universal node of the tech tree and nothing else.
  [2026-11-09 10:07:36](DEVLOG/DEVLOG2.md#100736-a-design-begun-from-a-base-civ-generic-at-first)
- **A base civ of the game is also the replaced civ,** until another is picked on the review step; with Generic,
  the replaced civ is the Britons, as in a new design.
  [2026-11-09 10:07:36](DEVLOG/DEVLOG2.md#100736-a-design-begun-from-a-base-civ-generic-at-first)
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
- **WCAG 2.2 AAA, read for a window** (Requirements 3 and 4). The 86 success criteria are listed in
  `.github/ACCESSIBILITY.md`, read as WCAG2ICT reads them for software; a box is ticked only when a test checks the
  criterion or when civbuilder has nothing it applies to, which counts as met. Contrast of 7:1, targets of 44 by
  44, a visible label for every input, and the colors of Windows in high contrast.
  [2026-11-09 01:20:32](DEVLOG/DEVLOG1.md#012032-the-nine-steps-in-a-window-with-undo-a-review-and-accessibility-tests)
  [2026-11-09 10:08:11](DEVLOG/DEVLOG2.md#100811-a-checklist-of-wcag-22-for-requirement-4)
- **The window's title names the civ and the step** (Requirement 3, WCAG 2.2 SC 2.4.2), as "Testonia - Step 2 of
  9: Tech tree - civbuilder", with "A new civ" before the civ has a name.
  [2026-11-09 10:27:22](DEVLOG/DEVLOG2.md#102722-a-windows-title-that-names-the-civ-and-the-step-and-the-base-civ-tried-in-the-window)
- **A mod takes the slot of one civ** (Requirement 8). The replaced civ is picked on the review step, the Britons at
  first, and is kept in the civ file as `replace_civ`.
  [2026-11-09 02:15:04](DEVLOG/DEVLOG1.md#021504-a-mod-built-from-the-civ-from-the-catalog-and-the-games-own-data-file)
  [2026-11-09 10:05:44](DEVLOG/DEVLOG2.md#100544-the-questions-of-part-1-answered)
- **Two folders, written after asking** (Requirements 2 and 7). Build writes `<prefix>-data` and `<prefix>-ui` into
  the mods folder it finds, where the original gives a zip to unpack by hand.
  [2026-11-09 02:15:04](DEVLOG/DEVLOG1.md#021504-a-mod-built-from-the-civ-from-the-catalog-and-the-games-own-data-file)
  [2026-11-09 10:05:44](DEVLOG/DEVLOG2.md#100544-the-questions-of-part-1-answered)
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

## Code and files

- **C11 and Win32 only.** Built by gcc with `-std=c11 -pedantic-errors` and every warning as an error.
  [2026-11-09 00:44:18](DEVLOG/DEVLOG1.md#004418-civbuilder-begun-in-c-with-its-own-build-and-rule-tests)
- **One step builds and tests.** `build.exe`, made from `tools/build/`, compiles what changed, links and runs the
  tests. [2026-11-09 00:44:18](DEVLOG/DEVLOG1.md#004418-civbuilder-begun-in-c-with-its-own-build-and-rule-tests)
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
- **`info.json` as the original writes it,** with `builder_version` "1.0.0", civbuilder's version, until it is said
  otherwise. Replaced: "2.7.0", the original's version.
  [2026-11-09 02:15:04](DEVLOG/DEVLOG1.md#021504-a-mod-built-from-the-civ-from-the-catalog-and-the-games-own-data-file)
  [2026-11-09 10:06:18](DEVLOG/DEVLOG2.md#100618-rules-26-to-36-settings-that-deny-commits-and-deletions-and-version-100)
- **"Change" for an entry of an effect,** where the original says command, since a command is a change to a design.
  [2026-11-09 02:15:04](DEVLOG/DEVLOG1.md#021504-a-mod-built-from-the-civ-from-the-catalog-and-the-games-own-data-file)
  [2026-11-09 10:05:44](DEVLOG/DEVLOG2.md#100544-the-questions-of-part-1-answered)
- **The civ's description has a description id of its own,** 990100 plus its slot, which the game's strings do not
  use. The UI mod writes the description under it and under the game's id, and the civ list points at it.
  [2026-11-09 10:06:47](DEVLOG/DEVLOG2.md#100647-the-civs-description-under-a-description-id-of-its-own)
- **A base civ is a line of the catalog.** A `base` line of `appearance.txt` gives a civ's bonuses, team bonus,
  unique unit, unique techs and looks by the catalog's ids; Generic is base 0, and every other base has its slot.
  [2026-11-09 10:07:36](DEVLOG/DEVLOG2.md#100736-a-design-begun-from-a-base-civ-generic-at-first)
- **What is universal is the game's word.** The region of a node, and the civs that have it, are read once from the
  game's own `CivTechTrees` files into the last two columns of `tree.txt`; the civs are bits of one 64-bit number.
  [2026-11-09 10:07:36](DEVLOG/DEVLOG2.md#100736-a-design-begun-from-a-base-civ-generic-at-first)
- **The data mod holds the civ's tech tree file,** the game's `CivTechTrees\<name>.json` of the replaced civ with each
  node of `tree.txt` set to the design: `NotAvailable` when the civ does not keep it, `ResearchedCompleted` when the
  design has it and the replaced civ had not; a node the design has keeps any other status, such as
  `ResearchRequired`. Nodes `tree.txt` does not hold stay as they were. Replaced: `NotAvailable` for every node the
  design lacks.
  [2026-11-09 10:18:41](DEVLOG/DEVLOG2.md#101841-a-tech-tree-screen-that-shows-the-designs-tree-and-all-71-tests-listed)
  [2026-11-09 10:40:46](DEVLOG/DEVLOG2.md#104046-one-rule-for-what-a-civ-keeps-in-the-dat-file-and-the-tech-tree-file-alike)
- **What a civ keeps is decided in one place,** `civ_design_tree_keeps` in `civ_design.c`: the whole tree when the
  design's is empty, and otherwise each node it has and the Villager, both Town Centers, the Mill and the two camps.
  The dat file and the tech tree file both follow it.
  [2026-11-09 10:40:46](DEVLOG/DEVLOG2.md#104046-one-rule-for-what-a-civ-keeps-in-the-dat-file-and-the-tech-tree-file-alike)
- **One way to change a JSON text.** `json_patch.c` replaces values one by one and copies every other byte; the
  civ list and the tech tree file are both changed with it.
  [2026-11-09 10:18:41](DEVLOG/DEVLOG2.md#101841-a-tech-tree-screen-that-shows-the-designs-tree-and-all-71-tests-listed)

## Tests

- **The tests number themselves.** `tests/runner.c` runs the functional, accessibility and maintainability tests in
  that order and numbers them as they run.
  [2026-11-09 00:44:18](DEVLOG/DEVLOG1.md#004418-civbuilder-begun-in-c-with-its-own-build-and-rule-tests)
- **Mutation by hand.** ub-mode's mutator does not read C, so a new test is shown to fail by breaking the code
  (Rules 33 and 34).
  [2026-11-09 00:44:18](DEVLOG/DEVLOG1.md#004418-civbuilder-begun-in-c-with-its-own-build-and-rule-tests)
  [2026-11-09 10:06:18](DEVLOG/DEVLOG2.md#100618-rules-26-to-36-settings-that-deny-commits-and-deletions-and-version-100)
- **No test of drawing or scrolling.** Bugs 1 and 2 are in `src/win/`, which the tests do not run; an honest test
  of them would need a window, so none is written (Rule 33).
  [2026-11-09 10:06:34](DEVLOG/DEVLOG2.md#100634-a-list-that-keeps-its-place-a-check-in-a-ticked-box-and-no-next-on-the-last-step)
- **The test of the log reads every part,** while `DEVLOG<n>.md` exists, and lets a part begin with the day the
  part before it ended with only when that part holds fewer than two days.
  [2026-11-09 10:06:18](DEVLOG/DEVLOG2.md#100618-rules-26-to-36-settings-that-deny-commits-and-deletions-and-version-100)
- **The layout is tested without a window.** The accessibility tests measure `layout.c` and `palette.c` at the
  smallest window, 960 by 560, so they run in the build without Win32.
  [2026-11-09 01:20:32](DEVLOG/DEVLOG1.md#012032-the-nine-steps-in-a-window-with-undo-a-review-and-accessibility-tests)
- **No test needs the game.** The tests make small data files in memory; the installed game was read only by check
  programs outside the project.
  [2026-11-09 02:15:04](DEVLOG/DEVLOG1.md#021504-a-mod-built-from-the-civ-from-the-catalog-and-the-games-own-data-file)
- **A new group before 60 lines.** When a suite's `switch` would pass 60 lines (Rule 11), its later cases move to a
  new group, and every test keeps its number.
  [2026-11-09 05:46:17](DEVLOG/DEVLOG1.md#054617-bonuses-330-339-and-458-built-from-unit-patches)
  [2026-11-09 10:18:41](DEVLOG/DEVLOG2.md#101841-a-tech-tree-screen-that-shows-the-designs-tree-and-all-71-tests-listed)
- **TESTS.md cites the requirements by their numbers.** The proposals of the first questions became Requirements 1
  to 9, and each test's purpose names the one it serves; a test of a bug found before cites Rule 24.
  [2026-11-09 10:18:41](DEVLOG/DEVLOG2.md#101841-a-tech-tree-screen-that-shows-the-designs-tree-and-all-71-tests-listed)

## Documents

- **repo_template's documents, without its setup.** `.claude/skills/project-setup` is not copied: the project is set
  up. [2026-11-09 00:44:18](DEVLOG/DEVLOG1.md#004418-civbuilder-begun-in-c-with-its-own-build-and-rule-tests)
- **`sitrep.md` stays in the main folder,** as Rule 14 now allows.
  [2026-11-09 10:05:44](DEVLOG/DEVLOG2.md#100544-the-questions-of-part-1-answered)
- **The checklist of accessibility is `.github/ACCESSIBILITY.md`,** repo_template's file of that name, since
  `documentation/` holds the 7 files that Rule 14 allows and Rule 15 allows one file of a name.
  [2026-11-09 10:08:11](DEVLOG/DEVLOG2.md#100811-a-checklist-of-wcag-22-for-requirement-4)
- **The tree's items are nodes.** The universal, regional and unique items of the order are universal, regional and
  unique nodes, as `TERMINOLOGY.md` names each unit, building and tech of the tree a node.
  [2026-11-09 10:07:36](DEVLOG/DEVLOG2.md#100736-a-design-begun-from-a-base-civ-generic-at-first)
