This project strives to be compliant with [WCAG 2.2](https://www.w3.org/TR/WCAG22/) at level AA.

# Accessibility checklist

When every box below is ticked, civeditor is WCAG 2.2 AA compliant (Requirements 3 and 4). Level AA asks for every
success criterion of levels A and AA, so those 55 of WCAG 2.2 are the checklist: 31 of level A and 24 of level AA;
4.1.1 Parsing is obsolete in WCAG 2.2. No criterion of level AAA is on the list.

civeditor is a program for Windows, not a web page, so each criterion is read as
[WCAG2ICT](https://www.w3.org/TR/wcag2ict/) reads it for software: a page is the window and its dialogs, a link or a
heading is a control or a label of the window.

A box is ticked only when the criterion has been checked, in one of three ways, which its line names:

- **Test:** a test of `build.exe` checks it on every build; the test is named by its purpose in `TESTS.md`.
- **Code:** civeditor has nothing the criterion applies to, as the code in `src/` shows; such a criterion is met.
- **Read:** every text the window can show (144 texts of `src/view/`, `src/present/` and `src/win/`) was read on
  the day named, against what the criterion asks of words. A text added later must be read in the same way.

A ticked box that ends in "Human review needed." is one that no test settles, or settles in part: it rests on
"Code" or "Read", or on a test with something left that was not looked at, heard or tried. A human looks at it
before the box is relied on; 31 of the 55 ticked boxes are such.

An unticked box says how civeditor meets the criterion, or would meet it, and what is left to check or to make.
"By hand" means with the keyboard, a screen reader such as Narrator, and the settings of Windows named.

## 1. Perceivable

- [x] **1.1.1 Non-text Content (A).** Test: a ticked row is told to a screen reader as chosen. The only pictures are
  the check of a ticked row and the frame of its box, drawn with lines; what the check says, Windows tells as the
  row's state.
- [x] **1.2.1 Audio-only and Video-only (Prerecorded) (A).** Code: civeditor has no audio or video.
  Human review needed.
- [x] **1.2.2 Captions (Prerecorded) (A).** Code: no video.
  Human review needed.
- [x] **1.2.3 Audio Description or Media Alternative (Prerecorded) (A).** Code: no video.
  Human review needed.
- [x] **1.2.4 Captions (Live) (AA).** Code: no live audio or video.
  Human review needed.
- [x] **1.2.5 Audio Description (Prerecorded) (AA).** Code: no video.
  Human review needed.
- [x] **1.3.1 Info and Relationships (A).** Test: every control tells a screen reader a name that begins with its
  label, so each label is tied to its control for assistive technology; the steps are a list, and a ticked row is a
  chosen row of its list.
- [x] **1.3.2 Meaningful Sequence (A).** Test: the Tab key follows the order in which a step is read. Code: each
  label is made just before its control (`create_control` in `controls.c`), so the order of the windows, which a
  screen reader walks, is the order of reading.
- [x] **1.3.3 Sensory Characteristics (A).** Read, 2026-11-10: no text points at a shape, a color, a size, a place
  or a sound; a control is always named by its label.
  Human review needed.
- [x] **1.3.4 Orientation (AA).** Code: the window is never locked to one orientation of the screen.
  Human review needed.
- [x] **1.3.5 Identify Input Purpose (AA).** Read, 2026-11-10: no field asks for a fact about the user; every field
  names or describes a part of the civ.
  Human review needed.
- [x] **1.4.1 Use of Color (A).** Test: every text contrasts at least 4.5:1 with its background. A ticked row has a
  check, not only a color; the chosen row of a plain list is darker than the rows around it by 4.5:1 or more, the
  pair of the focus color on a field in that test, which is a difference of lightness and not of hue alone. Not
  looked at in the high contrast of Windows.
- [x] **1.4.2 Audio Control (A).** Code: civeditor plays no audio; a message box gives only the short sound that
  Windows is set to give.
  Human review needed.
- [x] **1.4.3 Contrast (Minimum) (AA).** Test: every text contrasts at least 4.5:1 with its background, as asked
  here. In high contrast, the colors of Windows are used.
- [x] **1.4.4 Resize Text (AA).** Test: the text and the layout grow with the text size of Windows, up to 225%; a
  window shorter than the wide layout reflows; the control in focus is scrolled into a short window; a long row of
  a narrow list is higher than a short one. The text and the layout follow the scale of the display and the text
  size of Windows' accessibility settings. Where the wide layout no longer fits the screen, the window shows the
  step in one column and scrolls, so nothing is out of reach. Only 100% was looked at, in a window made small:
  set the text size of Windows to 200% and walk through the four steps. Human review needed.
- [x] **1.4.5 Images of Text (AA).** Code: every text is drawn as text; civeditor has no pictures of text.
  Human review needed.
- [x] **1.4.10 Reflow (AA).** Test: no control reaches past a window 320 wide; no two controls overlap in a window
  320 wide; the window may be as small as 320 by 256; a narrow window places its controls in one column; a long row
  of a narrow list is higher than a short one. A window narrower than 960 or shorter than 640 shows a step in one
  column as wide as the window and scrolls up and down only: with the scroll bar, the wheel, Scroll up and Scroll
  down of the menu Go (Ctrl+Up, Ctrl+Down), and by itself to the control in focus. The rows of the lists wrap
  instead of being cut. Not looked at: a row of a list is at most 255 pixels high, a limit of Windows, so at the
  largest scales a row of many lines can be cut; and the text beside a list is 132 high in the one column, which
  a long summary such as 24 chosen bonuses passes. To check by hand at 400% on a screen 1280 wide. Human review
  needed.
- [x] **1.4.11 Non-text Contrast (AA).** Test: every text contrasts at least 4.5:1 with its background. The test
  checks every pair of colors the window draws with: the frame of a box, the check and the focus frame are drawn
  in those same pairs (`controls_draw_item` in `controls.c`), so each has more than the 3:1 asked here. The frames
  of the standard controls are Windows' own; in high contrast the colors of Windows are used.
- [x] **1.4.12 Text Spacing (AA).** Code: the criterion is about content made in a markup language that lets the
  spacing of text be set; the window is made of the controls of Windows, not of markup.
  Human review needed.
- [x] **1.4.13 Content on Hover or Focus (AA).** Code: civeditor shows nothing on hover or focus, no tooltip.
  Human review needed.

## 2. Operable

- [x] **2.1.1 Keyboard (A).** Test: the Tab key reaches every control of every step, in the real window. Code: every
  command is in the menu (Alt) and has a shortcut (`make_menu` and `make_accelerators` in `app_window.c`), and
  the controls are the standard ones of Windows, which the keyboard works: arrows pick, Space ticks a row.
- [x] **2.1.2 No Keyboard Trap (A).** Test: the Tab key leaves every control. The test found the fields of several
  lines, such as Description, holding the Tab key; the window now moves the focus on when such a field asks it to
  (`move_focus` in `app_window.c`). The dialogs are Windows' own.
- [x] **2.1.4 Character Key Shortcuts (A).** Code: every shortcut uses Ctrl (`FIXED_KEYS` in `app_window.c`); the
  letters a list reads while it has the focus are the list's own.
  Human review needed.
- [x] **2.2.1 Timing Adjustable (A).** Code: nothing has a time limit; civeditor has no timer.
  Human review needed.
- [x] **2.2.2 Pause, Stop, Hide (A).** Code: nothing moves, blinks or updates by itself, but the text cursor, which
  blinks as Windows is set.
  Human review needed.
- [x] **2.3.1 Three Flashes or Below Threshold (A).** Code: nothing flashes.
  Human review needed.
- [x] **2.4.1 Bypass Blocks (A).** Test: the list of steps shows the current step as the chosen one, after the
  command of each step. Code: Ctrl+1 to Ctrl+9 send those commands and Ctrl+Page Down and Ctrl+Page Up the next and
  the one before (`make_accelerators` in `app_window.c`), and each puts the focus on the step's first control, past
  the list of steps.
- [x] **2.4.2 Page Titled (A).** Test: the window's title names the civ and the step, such as "Testonia - Step 2
  of 4: Bonuses - civeditor", and "A new civ" before the civ has a name.
- [x] **2.4.3 Focus Order (A).** Test: the Tab key follows the order in which a step is read: the list of steps,
  each column from the top, then Back and Next.
- [x] **2.4.4 Link Purpose (In Context) (A).** Code: civeditor has no links.
  Human review needed.
- [x] **2.4.5 Multiple Ways (AA).** Test: the list of steps shows the current step as the chosen one; the list of
  steps is on every step, with Back and Next wherever they lead. Code: the menu Go and the shortcuts send the same
  commands (`make_go_menu` and `make_accelerators` in `app_window.c`).
- [x] **2.4.6 Headings and Labels (AA).** Test: every input has a visible label. Read, 2026-11-10: each of the nine
  titles and 36 labels says what its step or control is for; two that leaned on the label before them, "Its
  description" and "Its base unit", were made "Description of the unique unit" and "The selected unit".
- [x] **2.4.7 Focus Visible (AA).** Test: the control in focus has a solid frame at least two pixels thick. The
  window draws the frame itself around whichever control has the focus (`assist_draw_focus` in `assist.c`); a
  row in focus has its own frame inside a list. Not looked at with the eyes.
  Human review needed.
- [x] **2.4.11 Focus Not Obscured (Minimum) (AA).** Test: no two controls of a step overlap, even in the smallest
  window.
- [x] **2.5.1 Pointer Gestures (A).** Code: every action is a click; nothing needs a path or two fingers.
  Human review needed.
- [x] **2.5.2 Pointer Cancellation (A).** Test: undo brings back the civ before a change. Code: the buttons are
  Windows' own and act when the mouse button is let go; a row of a list is picked when it is pressed, and undo
  takes that back.
- [x] **2.5.3 Label in Name (A).** Test: every control tells a screen reader a name that begins with its label.
- [x] **2.5.4 Motion Actuation (A).** Code: nothing reacts to the motion of the device.
  Human review needed.
- [x] **2.5.7 Dragging Movements (AA).** Code: nothing needs dragging; a scroll bar can also be moved by its arrows
  and by the keyboard.
  Human review needed.
- [x] **2.5.8 Target Size (Minimum) (AA).** Test: every control one acts on is at least 44 by 44, more than the 24
  by 24 asked here.

## 3. Understandable

- [x] **3.1.1 Language of Page (A).** Test: Windows is told that the window's language is English. Every text is
  English, and the program says so in C (`SetProcessPreferredUILanguages` in `app_open`), where the usual way, a
  resource of the program, is not C (Rule 29). Whether a screen reader takes its voice from it was not tried.
  Human review needed.
- [x] **3.1.2 Language of Parts (AA).** Code: all of civeditor's own texts are English; the names of the game are
  proper names.
  Human review needed.
- [x] **3.2.1 On Focus (A).** Test: moving the focus through every control changes nothing.
- [x] **3.2.2 On Input (A).** Read, 2026-11-10: the help line of the first step says before the choice is made that
  a base civ sets the tree, bonuses and looks; the status line says so after, and undo takes it back. No other
  input changes more than itself.
  Human review needed.
- [x] **3.2.3 Consistent Navigation (AA).** Test: the list of steps is on every step, with Back and Next wherever
  they lead. Their places cannot differ between steps: `layout_box` in `layout.c` places a control without
  knowing the step.
- [x] **3.2.4 Consistent Identification (AA).** Read, 2026-11-10: a command has one name in the menu, on its button
  and in `README.md`: Save civ file..., Open civ file..., Build mod..., Next and Back. The menu said "Previous
  step" where the button says "Back"; it says "Back" now.
  Human review needed.
- [x] **3.2.6 Consistent Help (A).** Test: the help key says what the control in focus is for, on every step. The
  help is in the same places on every step: the F1 key, the menu Help, the status line and the line under the title.
- [x] **3.3.1 Error Identification (A).** Test: the summary names what is still needed. Read, 2026-11-10: each thing
  that can go wrong is said in words, in the status line or a message box: a file that is not a civ file, a file
  that cannot be written, a change that cannot be made, a mod that cannot be built and why.
- [x] **3.3.2 Labels or Instructions (A).** Test: every input has a visible label.
- [x] **3.3.3 Error Suggestion (AA).** Test: the summary names what is still needed: the review says what to add,
  a name, a unique unit, a castle tech, an imperial tech or a name for the hero.
- [x] **3.3.4 Error Prevention (Legal, Financial, Data) (AA).** Test: undo brings back the civ before a change, and
  redo makes an undone change again; Build asks before it writes, and unsaved changes are asked about.
- [x] **3.3.7 Redundant Entry (A).** Code: the design keeps everything entered; nothing is asked twice.
  Human review needed.
- [x] **3.3.8 Accessible Authentication (Minimum) (AA).** Code: civeditor asks for no login.
  Human review needed.

## 4. Robust

- [x] **4.1.2 Name, Role, Value (A).** Test: every control tells a screen reader a name that begins with its label,
  asked of Windows' accessibility interface as a screen reader asks it. Code: the controls are the standard
  controls of Windows, which report their roles, values and states themselves; the owner-drawn lists keep their
  texts (`LBS_HASSTRINGS`). Not tried with a screen reader.
  Human review needed.
- [x] **4.1.3 Status Messages (AA).** Test: the status line is a live region that a screen reader is told of. The
  status line is marked as a polite live region (`mark_live` in `controls.c`), and each change of its text raises
  the event of Windows for a changed live region (`dialogs_status` in `dialogs.c`), so it is read without the
  focus moving. Not heard with a screen reader.
  Human review needed.

