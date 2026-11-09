This project strives to be compliant with [WCAG 2.2](https://www.w3.org/TR/WCAG22/) at level AAA.

# Accessibility checklist

When every box below is ticked, civbuilder is WCAG 2.2 AAA compliant (Requirements 3 and 4). Level AAA asks for every
success criterion of levels A, AA and AAA, so all 86 of WCAG 2.2 are listed; 4.1.1 Parsing is obsolete in WCAG 2.2.

civbuilder is a program for Windows, not a web page, so each criterion is read as
[WCAG2ICT](https://www.w3.org/TR/wcag2ict/) reads it for software: a page is the window and its dialogs, a link or a
heading is a control or a label of the window.

A box is ticked only when the criterion has been checked, in one of two ways, which its line names:

- **Test:** a test of `build.exe` checks it on every build; the test is named by its purpose in `TESTS.md`.
- **Code:** civbuilder has nothing the criterion applies to, as the code in `src/` shows; such a criterion is met.

An unticked box says how civbuilder meets the criterion, or would meet it, and what is left to check or to make.
"By hand" means with the keyboard, a screen reader such as Narrator, and the settings of Windows named.

## 1. Perceivable

- [ ] **1.1.1 Non-text Content (A).** The only pictures are the check of a ticked row and the frame of a box, drawn
  with lines; a ticked row is also selected, which Windows reports. To check by hand with a screen reader.
- [x] **1.2.1 Audio-only and Video-only (Prerecorded) (A).** Code: civbuilder has no audio or video.
- [x] **1.2.2 Captions (Prerecorded) (A).** Code: no video.
- [x] **1.2.3 Audio Description or Media Alternative (Prerecorded) (A).** Code: no video.
- [x] **1.2.4 Captions (Live) (AA).** Code: no live audio or video.
- [x] **1.2.5 Audio Description (Prerecorded) (AA).** Code: no video.
- [x] **1.2.6 Sign Language (Prerecorded) (AAA).** Code: no audio or video.
- [x] **1.2.7 Extended Audio Description (Prerecorded) (AAA).** Code: no video.
- [x] **1.2.8 Media Alternative (Prerecorded) (AAA).** Code: no video.
- [x] **1.2.9 Audio-only (Live) (AAA).** Code: no live audio.
- [ ] **1.3.1 Info and Relationships (A).** Each input has its own label, placed before it in the order of the
  controls, and the steps are a list. To check by hand that a screen reader reads each label with its control.
- [ ] **1.3.2 Meaningful Sequence (A).** The controls are made in the order they are read, label before control. To
  check by hand with a screen reader.
- [ ] **1.3.3 Sensory Characteristics (A).** No text points at a shape, a place or a sound. To check by reading every
  text of `src/view/` and `src/win/`.
- [x] **1.3.4 Orientation (AA).** Code: the window is never locked to one orientation of the screen.
- [ ] **1.3.5 Identify Input Purpose (AA).** No field asks for facts about the user. To confirm by reading the steps.
- [ ] **1.3.6 Identify Purpose (AAA).** The controls are the standard controls of Windows, whose roles Windows
  reports. To check by hand with a screen reader.
- [ ] **1.4.1 Use of Color (A).** A ticked row has a check, not only a color; a picked row of a plain list is shown
  by color and by its selection. To check by hand, and in the high contrast of Windows.
- [x] **1.4.2 Audio Control (A).** Code: civbuilder plays no audio; a message box gives only the short sound that
  Windows is set to give.
- [x] **1.4.3 Contrast (Minimum) (AA).** Test: every text contrasts at least 7:1 with its background, more than the
  4.5:1 asked here. In high contrast, the colors of Windows are used.
- [ ] **1.4.4 Resize Text (AA).** The text follows the scale of the display that Windows is set to, up to 200%, and
  the window can be made larger. Not yet: the size of text set in Windows' accessibility settings is not followed.
- [x] **1.4.5 Images of Text (AA).** Code: every text is drawn as text; civbuilder has no pictures of text.
- [x] **1.4.6 Contrast (Enhanced) (AAA).** Test: every text contrasts at least 7:1 with its background.
- [x] **1.4.7 Low or No Background Audio (AAA).** Code: no audio.
- [ ] **1.4.8 Visual Presentation (AAA).** The colors of high contrast are followed; the text is not justified. Not
  yet checked: lines of at most 80 characters, line spacing of 1.5, and text at 200% without scrolling sideways.
- [x] **1.4.9 Images of Text (No Exception) (AAA).** Code: no pictures of text.
- [ ] **1.4.10 Reflow (AA).** The window is at least 960 by 560 at 100%, so at 400% it cannot fit 320 pixels. To
  decide how WCAG2ICT's reading applies to a window that the user sizes, and to check by hand at 400%.
- [ ] **1.4.11 Non-text Contrast (AA).** The boxes, the check and the frames are drawn in the colors of the text.
  To be added to the test of contrast, at 3:1.
- [ ] **1.4.12 Text Spacing (AA).** WCAG2ICT applies it only where the user can set the spacing of text; Windows
  gives no such setting for these controls. To confirm.
- [x] **1.4.13 Content on Hover or Focus (AA).** Code: civbuilder shows nothing on hover or focus, no tooltip.

## 2. Operable

- [ ] **2.1.1 Keyboard (A).** Every control is reached with Tab, and every command is in the menu (Alt) and has a
  shortcut. To check by hand, step by step.
- [ ] **2.1.2 No Keyboard Trap (A).** Tab and Shift+Tab leave every control; Escape closes a dialog. To check by hand.
- [ ] **2.1.3 Keyboard (No Exception) (AAA).** As 2.1.1; nothing needs the mouse. To check by hand.
- [x] **2.1.4 Character Key Shortcuts (A).** Code: every shortcut uses Ctrl (`FIXED_KEYS` in `app_window.c`); the
  letters a list reads while it has the focus are the list's own.
- [x] **2.2.1 Timing Adjustable (A).** Code: nothing has a time limit; civbuilder has no timer.
- [x] **2.2.2 Pause, Stop, Hide (A).** Code: nothing moves, blinks or updates by itself, but the text cursor, which
  blinks as Windows is set.
- [x] **2.2.3 No Timing (AAA).** Code: no timer.
- [x] **2.2.4 Interruptions (AAA).** Code: a message box opens only in answer to a command of the user.
- [x] **2.2.5 Re-authenticating (AAA).** Code: civbuilder asks for no account or password.
- [x] **2.2.6 Timeouts (AAA).** Code: nothing is lost by waiting; civbuilder has no timeout.
- [x] **2.3.1 Three Flashes or Below Threshold (A).** Code: nothing flashes.
- [x] **2.3.2 Three Flashes (AAA).** Code: nothing flashes.
- [x] **2.3.3 Animation from Interactions (AAA).** Code: civbuilder has no animation of its own.
- [ ] **2.4.1 Bypass Blocks (A).** Ctrl+1 to Ctrl+9 go to a step, Ctrl+Page Down and Ctrl+Page Up to the next and
  previous one, past the list of steps. To check by hand.
- [x] **2.4.2 Page Titled (A).** Test: the window's title names the civ and the step, such as "Testonia - Step 2
  of 9: Tech tree - civbuilder", and "A new civ" before the civ has a name.
- [ ] **2.4.3 Focus Order (A).** The controls are made in the order they are read, so Tab follows it. To check by hand.
- [x] **2.4.4 Link Purpose (In Context) (A).** Code: civbuilder has no links.
- [ ] **2.4.5 Multiple Ways (AA).** A step is reached from the list of steps, from Next and Back, from the menu Go
  and by shortcut. To check by hand.
- [ ] **2.4.6 Headings and Labels (AA).** Each step has a title and each control a label, which the test "every input
  has a visible label" finds. To judge by hand that each one says what it is for.
- [ ] **2.4.7 Focus Visible (AA).** The control with the focus is drawn with a focus frame, in owner-drawn lists too.
  To check by hand.
- [ ] **2.4.8 Location (AAA).** The list of steps shows the current step picked. To check by hand.
- [x] **2.4.9 Link Purpose (Link Only) (AAA).** Code: no links.
- [ ] **2.4.10 Section Headings (AAA).** Each step is a section with its title. To check by hand.
- [x] **2.4.11 Focus Not Obscured (Minimum) (AA).** Test: no two controls of a step overlap, even in the smallest
  window.
- [x] **2.4.12 Focus Not Obscured (Enhanced) (AAA).** Test: no two controls of a step overlap, even in the smallest
  window.
- [ ] **2.4.13 Focus Appearance (AAA).** Not yet: the focus frame is Windows' dotted line of one pixel; it needs a
  solid frame at least two pixels wide, with a contrast of 3:1.
- [x] **2.5.1 Pointer Gestures (A).** Code: every action is a click; nothing needs a path or two fingers.
- [ ] **2.5.2 Pointer Cancellation (A).** Buttons act when the mouse button is let go; a row of a list is picked when
  it is pressed, and Ctrl+Z takes that back. To check by hand.
- [ ] **2.5.3 Label in Name (A).** Each control's name for a screen reader should start with its visible label. To
  check by hand with a screen reader.
- [x] **2.5.4 Motion Actuation (A).** Code: nothing reacts to the motion of the device.
- [x] **2.5.5 Target Size (Enhanced) (AAA).** Test: every control one acts on is at least 44 by 44.
- [x] **2.5.6 Concurrent Input Mechanisms (AAA).** Code: civbuilder takes the mouse, the keyboard, touch and the pen
  as Windows gives them, and turns none of them off.
- [x] **2.5.7 Dragging Movements (AA).** Code: nothing needs dragging; a scroll bar can also be moved by its arrows
  and by the keyboard.
- [x] **2.5.8 Target Size (Minimum) (AA).** Test: every control one acts on is at least 44 by 44, more than the 24
  by 24 asked here.

## 3. Understandable

- [ ] **3.1.1 Language of Page (A).** Every text is English. Not yet: the program does not say so to Windows; the
  usual way, a resource of the program, is not C (Rule 29).
- [x] **3.1.2 Language of Parts (AA).** Code: all of civbuilder's own texts are English; the names of the game are
  proper names.
- [ ] **3.1.3 Unusual Words (AAA).** Not yet: words of the game, such as Folwark or Krepost, have no explanation.
- [ ] **3.1.4 Abbreviations (AAA).** Not yet: the catalog shortens a few names, such as "H. Mounted Crossbowman".
- [ ] **3.1.5 Reading Level (AAA).** The texts are kept short and plain. To check against the level of lower
  secondary education.
- [ ] **3.1.6 Pronunciation (AAA).** To check whether any word's meaning depends on how it is said.
- [ ] **3.2.1 On Focus (A).** Moving the focus changes nothing. To check by hand.
- [ ] **3.2.2 On Input (A).** Picking a base civ changes much of the civ at once, and the status line says so;
  Ctrl+Z takes it back. To judge by hand whether the status line warns enough.
- [ ] **3.2.3 Consistent Navigation (AA).** The list of steps, Next and Back keep their places on every step. To be
  added to the tests of the layout.
- [ ] **3.2.4 Consistent Identification (AA).** The same command has the same name in the menu, on its button and in
  `README.md`. To check by reading.
- [ ] **3.2.5 Change on Request (AAA).** A dialog opens and a step changes only on a command of the user. To check
  by hand.
- [x] **3.2.6 Consistent Help (A).** Code: civbuilder has no help in its window, so nothing must keep a place.
- [ ] **3.3.1 Error Identification (A).** A file that cannot be read or written is named in a message box, and the
  review lists what the civ still needs. To check by hand.
- [x] **3.3.2 Labels or Instructions (A).** Test: every input has a visible label.
- [ ] **3.3.3 Error Suggestion (AA).** The review says what each step still needs. To check by hand.
- [x] **3.3.4 Error Prevention (Legal, Financial, Data) (AA).** Test: undo brings back the civ before a change, and
  redo makes an undone change again; Build asks before it writes, and unsaved changes are asked about.
- [ ] **3.3.5 Help (AAA).** Not yet: no control has help of its own beyond its label.
- [x] **3.3.6 Error Prevention (All) (AAA).** Test: undo brings back the civ before a change, and redo makes an undone
  change again (Requirement 5); Build asks before it writes.
- [x] **3.3.7 Redundant Entry (A).** Code: the design keeps everything entered; nothing is asked twice.
- [x] **3.3.8 Accessible Authentication (Minimum) (AA).** Code: civbuilder asks for no login.
- [x] **3.3.9 Accessible Authentication (Enhanced) (AAA).** Code: no login.

## 4. Robust

- [ ] **4.1.2 Name, Role, Value (A).** The controls are the standard controls of Windows, which report their names,
  roles and values; the owner-drawn lists keep their texts (`LBS_HASSTRINGS`). To check by hand with a screen reader.
- [ ] **4.1.3 Status Messages (AA).** Not yet: the status line changes its text, but a screen reader is not told; it
  needs an event of Windows that announces the change.
