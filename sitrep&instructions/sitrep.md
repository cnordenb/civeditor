# Sitrep, 2026-11-09 10:51

Nothing is committed and nothing is deleted; the work is in the working tree for review. The answers in
`instructions_2.md` to the sitrep of 08:06, which is kept below, are recorded in the log and in `DECISIONS.md`.
`build.exe` runs the analyzer, which finds nothing, and 78 tests (53 functional, 5 accessibility, 20
maintainability): 76 pass. Tests 71 and 73 fail only on the main folder's files (question 1).

## What changed since 08:06

- **Bugs 1 to 3.** A list keeps its place when a row is ticked; a ticked row shows a check; the last step has no
  Next button.
- **Bugs 4 and 5.** The civ's description is written under a description id of its own, 990 100 plus its slot.
- **Bug 6.** The data mod holds the civ's tech tree file, `resources\_common\dat\CivTechTrees\<NAME>.json`: the
  game's file of the replaced civ, with each node set to the design. An empty tree keeps the game's file, and every
  civ keeps the Villager, both Town Centers, the Mill and the two camps, as the dat file does.
- **The base civ.** "Base civ" is the first choice on the step "Identity". Picking a civ sets its tech tree,
  bonuses, team bonus, unique unit, unique techs and looks, each still editable; undo takes it back. A new civ
  starts from Generic: every universal node, no regional or unique node, no bonus.
- **Accessibility.** The window's title names the civ and the step (WCAG 2.4.2). `.github/ACCESSIBILITY.md` is the
  checklist of Requirement 4: 46 of its 86 criteria are ticked.
- **Rules.** Rules 26 to 36 are in `RULES.md`; `.claude/settings.json` denies commits, pushes and deletions; the
  version is 1.0.0.

## Questions to answer now

Each has a proposal; "yes" accepts it. Each is also in the log, under the Questions of its section.

1. **Files in the main folder.** `instructions_2.md` and `image.png` to `image-3.png` make Tests 71 and 73 fail: 13
   files where Rule 14 allows 8, and pictures, which are neither C nor documents. Proposal: move them into
   `materials/`, which Git ignores.
2. **The default name.** "Saved and opened as `civbuilder_civ`" (Requirement 1) is read as the file name offered,
   `civbuilder_civ.civbuilder.json`; the format inside stays `empireforge_v2`, so the original still opens the file.
   Proposal: keep this reading.
3. **The priority of the UI mod.** A text mod of higher priority can still cover the civ's names, which keep the
   game's string ids. Proposal: keep the advice in `README.md` to give the UI mod the highest priority.
4. **The base civ as a requirement.** The order is in `instructions_2.md`, not in `REQUIREMENTS.md`. Proposal: a
   Requirement 10, "A civ starts as its base civ, picked first; at first Generic, with every universal node and
   nothing else."
5. **Six civs with gaps.** The Achaemenids, Athenians, Spartans, Macedonians, Thracians and Puru have no castle or
   imperial tech and no monk skin in the catalog, and some no civ or team bonus. Proposal: add them from the game's
   dat file.
6. **"Evaluated as true" (Requirement 4).** A criterion with nothing to apply to, such as captions without video,
   is ticked as met. Proposal: keep this.
7. **Reflow (1.4.10).** A step needs a window of 960 by 560, where reflow asks for 320 pixels wide at 400%.
   Proposal: lay a step out in one column below 960 pixels, with a test.
8. **The status of a node the replaced civ lacked.** Such a node is written `ResearchedCompleted`. For a Generic
   civ that replaces the Britons, four of them are `ResearchRequired` for nearly every civ that has them: the
   Bombard Tower, Carrack, Hand Cannoneer and Bombard Cannon. Proposal: a column of `tree.txt` with each node's usual
   status, read once from the game's files.
9. **Abbreviations (3.1.4).** The catalog's texts shorten words: "HP" 42 times, "vs" 41, "LOS" 17 and "TC" once,
   besides "Skirms", "stone def" and the like; two nodes are "H. Mounted Crossbowman" and "E. Varangian Guard". The
   game itself says "HP" 236 times, but "line of sight" 74 times and "LOS" twice. Proposal: spell them out in
   `data/` ("hit points", "line of sight", "against"), the two names as the game names them, with a test.

## How to test what is new

### In the application

1. Run `build.exe`, then `build\civbuilder.exe`. The title reads "A new civ - Step 1 of 9: Identity - civbuilder".
2. On "Identity", "Base civ" shows "Generic". On "Tech tree", 54 units, 29 buildings and 83 techs are ticked.
3. Pick a base civ, such as the Franks: the looks, bonuses and tree change, the steps it fills are marked done,
   and the status line says so. Ctrl+Z brings Generic back.
4. On "Civ bonuses", scroll down and tick a row: the list stays where it was, and the row shows a check.
5. The last step has no Next button.

### In the game

All of "In the game" in the sitrep of 08:06 below still holds, and none of it has been tried. In addition:

1. **The tech tree screen (Bug 6).** Open the civ's tech tree: it should show the design's tree, not the replaced
   civ's. Look at the Villager, the Town Center, the Mill and the camps, which are always there; at the Keep, which
   still needs its research; and at the Hand Cannoneer and the Bombard Cannon, which may look researched already
   (question 8).
2. **The description (Bugs 4 and 5).** The civ selection screen and the tech tree's help show the new civ's
   description, not the replaced civ's. With another text mod on, give the UI mod the highest priority (question 3).
3. **A base civ.** A mod built from the Franks, with nothing changed, should play as the Franks in the replaced slot.

### Known gaps

- Not built: custom unique units, language and voices, the hero's description, flags and overrides, the Royal
  tiers of bonuses 155 and 261, and the 22 civ bonuses listed below.
- No screen reader has been tried.

---

# Sitrep, 2026-11-09 08:06 (answered in `instructions_2.md`)

Nothing is committed and nothing is deleted. The work is in the working tree for review. All 61 tests pass in the
analyzed build (37 functional, 4 accessibility, 20 maintainability). The game has never been started with a mod
from civbuilder, so testing it in the game is the most valuable check now.

## 1. Questions to answer

Each question has a proposal. Answering "yes" means the proposal is accepted.

### Requirements (REQUIREMENTS.md holds only the template, and only you may write it)

1. Accept these as requirements? Until then, the tests cover them as proposals.
   - The civ is designed in nine steps.
   - It is saved and opened as `empireforge_v2`.
   - The mod goes into the local mods folder.
   - Every step works by keyboard alone.
   - Contrast is at least 7:1.
   - Undo and redo (Ctrl+Z, Ctrl+Y).
   - A civ has at most 24 civ bonuses.
   - The mod is two folders, `<name>-data` and `<name>-ui`.
   - The mod replaces one civ of the game.
   - Build says what it could not build.

### Rules and repository

2. **License:** keep GPL-3.0, since the catalog in `data/` comes from the GPL original? Or replace the data so the
   code can be MIT? Proposal: keep GPL-3.0.
3. **Rules from instructions.md:** add P10, Head First Design Patterns, Clean Code, "C and Win32 only" and "no
   deletion" as Rules 27–31? Proposal: yes.
4. **CI:** may one `.github/workflows/checks.yml` run `build.exe` on a Windows runner on every push? Without it,
   Rule 4 holds only partly. Proposal: yes.
5. **Settings:** write `.claude/settings.json` to deny commit, push and edits of REQUIREMENTS.md? Proposal: yes.
6. **Rule 24:** is the gap between Rules 23 and 25 intended, or is a rule missing?
7. **Line 16 of instructions.md:** are `DeleteObject` and `DestroyWindow` allowed? They free GDI objects and
   windows, not files. Proposal: allowed.
8. **Log length:** Rules 18 and 22 together cap a day at 1000 lines. DEVLOG1.md is at 990 lines, nearly all of
   them 2026-11-09. Proposal: a day may continue in the next part under a heading marked "continued", and Rule 22
   says so.
9. **This file:** with it, the root has 9 files, one over Rule 14's limit. Remove it once read, or say where it
   belongs.

### Design

10. **Themed controls:** these need a manifest, which is XML, not C. Proposal: keep the classic controls.
11. **Compressor:** the dat file is packed by deflate written in C, with fixed Huffman codes, to 13.5 MB. Proposal:
    keep it; dynamic codes would only make the file smaller.
12. **Where the mod goes:** two folders in `mods\local`, rather than a zip. Proposal: keep the folders.
13. **Version:** the civ file keeps `builder_version` "2.7.0", the original's. Proposal: keep.
14. **Wording:** "change" names one entry of an effect, where the original says "command". Proposal: keep
    "change".
15. **Campaign lines:** the names of the unique techs, the unique unit and the hero go under string ids that a few
    lines of the game's campaigns also use. Proposal: keep this, with the warning in the README to turn off the UI
    mod before playing a campaign.

## 2. How to test

### Without the game

1. In the project's folder, once: `gcc -o build.exe tools/build/*.c src/core/*.c`.
2. Run `build.exe`. It runs the static analyzer, then the 61 tests, and must end with every test passed. For a
   faster run without the analyzer, use `build.exe quick`.
3. To see what each test checks, read `documentation/TESTS.md`.

### The application

1. Run `build\civbuilder.exe`.
2. Open `tests\fixtures\example.civbuilder.json` (Ctrl+O). This is the original's example civ, "Vanguards"; it
   should load with its name, its 2 bonuses, its unique unit and its language. Or make a civ from scratch.
3. Walk the nine steps by mouse and by keyboard (Tab; arrows; Space; Ctrl+Page Down / Ctrl+Page Up; Ctrl+1 to
   Ctrl+9). Try undo and redo (Ctrl+Z, Ctrl+Y), and the menu (Alt).
4. Save it (Ctrl+S), reopen it and compare. The last step lists what the civ still needs.
5. On the last step, pick the civ of the game to replace, then press Build mod... (Ctrl+B). Read the report; it
   lists every bonus that could not be built.

### In the game (the check that has never been made)

Before you start: the game must be installed through Steam. civbuilder only reads the game's data file and never
changes it. It writes `<name>-data` and `<name>-ui` into the game's `mods\local` folder, usually
`%USERPROFILE%\Games\Age of Empires 2 DE\<number>\mods\local`.

1. **Loading:** in the mod manager, turn on `<name>-ui`. In a skirmish lobby, pick `<name>-data` as the data mod
   and the replaced civ for a player. The first risk is that the game rejects or crashes on the dat file, which
   civbuilder wrote and compressed itself. If it does, note the error and the step where it happened. That is the
   most important report.
2. **Civ selection:** the replaced civ's slot shows the new name, description and unique unit (from the
   `civilizations.json` the mod writes).
3. **Bonuses:** check a few civ bonuses with a visible effect, such as costs, starting resources or speeds. Also
   check the team bonus on an allied player. These are worth picking because they were built in the last stretch:
   - 330: farms of 2×2 tiles;
   - 339: food stored at military buildings, the Harbor and the Krepost;
   - 458: buildings can be placed on any elevation (the Town Center keeps its own rule).
4. **Unique unit and techs:** the Castle trains the chosen unique unit under its new name. The Castle Age and
   Imperial Age unique techs have their new names and change that unit.
5. **Looks:** the buildings show the chosen architecture. The Castle and the Wonder show their chosen looks, and
   Monks show the chosen monk skin.
6. **Hero:** if one was chosen, the hero trains at the Castle from the Imperial Age, under its new name.
7. **Afterwards:** turn off the UI mod before playing a campaign (question 15).

### Known gaps you will see

- 22 of the 394 civ bonuses are not built: 81, 103, 221, 222, 239, 247, 283, 286, 308, 309, 310, 332, 352, 356,
  400, 451, 454, 455, 456, 457, 459 and 460. The build's report names the ones it skipped. All 80 team bonuses are
  built.
- These are not built at all:
  - custom unique units (only the game's own units can be picked);
  - language and voices;
  - the tech tree screen, which still shows the replaced civ's tree (`civTechTrees` is not written);
  - the hero's description, flags and overrides;
  - the Royal tiers of bonuses 155 and 261.
- A bonus's multiplier in a civ file is not applied.
- For a unique unit with no entry of its own, the civ list keeps the replaced civ's `unique_unit_line`.

### Afterwards

Test mods are folders in `mods\local`. Delete them by hand when done. The scratch folders and files listed in
`documentation/DELETION_REQUESTS.md` also wait for a human to delete them.
