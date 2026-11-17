# Tests

This project strives to have sensible testing. The goal is not to have 100% test coverage but to have tests which _sufficiently_ ensure the functional completeness, functional correctness, maintainability, and accessibility of civbuilder. For more guidelines on software quality and accessibility, see [ISO/IEC 25010:2011](https://cdn.standards.iteh.ai/samples/35733/2ca18b477b7845a5b8cae39d6de0c098/ISO-IEC-25010-2011.pdf), [ISO/IEC 25010:2023](https://cdn.standards.iteh.ai/samples/78176/13ff8ea97048443f99318920757df124/ISO-IEC-25010-2023.pdf) and [WCAG 2.2](https://www.w3.org/TR/WCAG22/).

This document lists each test, its purpose, and in which test file it resides. This document is an effort to mitigate redundant tests and ensure test quality. It is intended as an aid in the trouble-shooting of potential false positives.

Test descriptions must remain brief and parameters (eg. Purpose) may not exceed 120 characters. Tests are ordered in order of their execution.

The test report names each test by its number here, such as "Test 15 failed": a test's number is its place in the order
the tests run. So when a test is added, removed or moved, renumber this document to match.

Every test serves a requirement, or catches again a bug found before, and its purpose names the requirement, rule
or bug (Rules 5 and 6). Rule 4, which asks only that there are tests, is never a test's purpose.

The tests run with the build: `build.exe` (or `build.exe quick`) in the project's folder. `build\tests.exe` runs them
again without building. A file and lines given as `suite.c`, line N, is the line that names the test; the lines after
it are the code that makes the check.

## Table of contents per category

Functional completeness & correctness tests:

- [Test 1](#test-1): a failed check names its file, line and condition
- [Test 2](#test-2): the original's example civ keeps its name
- [Test 3](#test-3): the original's example civ keeps its bonuses
- [Test 4](#test-4): the original's example civ keeps its unique unit
- [Test 5](#test-5): the original's example civ keeps its language
- [Test 6](#test-6): a saved civ loads back unchanged
- [Test 7](#test-7): a bonus's multiplier is read back from a civ file
- [Test 8](#test-8): undo brings back the civ before a change
- [Test 9](#test-9): typing a name is undone at once
- [Test 10](#test-10): redo makes an undone change again
- [Test 11](#test-11): a 25th bonus is refused
- [Test 12](#test-12): next walks every step in order
- [Test 13](#test-13): the summary names what is still needed
- [Test 14](#test-14): the summary labels the civ's name
- [Test 15](#test-15): the last step shows no Next button
- [Test 16](#test-16): a dynamic Huffman stream inflates
- [Test 17](#test-17): a fixed Huffman stream inflates
- [Test 18](#test-18): a stored stream inflates back
- [Test 19](#test-19): packed data unpacks to the same bytes
- [Test 20](#test-20): repeated data packs to a tenth of its size
- [Test 21](#test-21): an unchanged dat file is written back byte for byte
- [Test 22](#test-22): an added effect is written as the game holds it
- [Test 23](#test-23): an added tech is written as the game holds it
- [Test 24](#test-24): a bonus without techs is built from its changes
- [Test 25](#test-25): a bonus taken twice is built with its changes doubled
- [Test 26](#test-26): the name is written under the replaced civ's name id
- [Test 27](#test-27): the civ descriptions are written in id order
- [Test 28](#test-28): the mod's folder name is made from the civ's name
- [Test 29](#test-29): a mod is not built from an unfinished civ
- [Test 30](#test-30): a unique tech is aimed at the civ's own unique unit
- [Test 31](#test-31): a unique tech's name is written under the string id of its slot
- [Test 32](#test-32): a unique unit's name is written under the string id of its slot
- [Test 33](#test-33): a building takes the look of its architecture's civ
- [Test 34](#test-34): a monk takes the look of its monk skin
- [Test 35](#test-35): a hero trains at the castle
- [Test 36](#test-36): a unique tech's changes of one attribute by two amounts are one target
- [Test 37](#test-37): a unique tech's changes of two armor classes are two targets
- [Test 38](#test-38): a hero's name is written under the string id of its slot
- [Test 39](#test-39): a skipped hero's name is not written
- [Test 40](#test-40): the civ list names the civ's own unique unit
- [Test 41](#test-41): a unique unit missing from the civ list takes its own icon and texts
- [Test 42](#test-42): a unit patch changes its civ's unit
- [Test 43](#test-43): a train line moves where its civ's unit is trained
- [Test 44](#test-44): a train line sets the button of its civ's unit
- [Test 45](#test-45): a unit with nowhere to train leaves its bonus skipped
- [Test 46](#test-46): a cost line sets a cost of its civ's unit
- [Test 47](#test-47): a cost line reaches the last field of the costs
- [Test 48](#test-48): the civ list points at the civ's own description
- [Test 49](#test-49): a value the civ keeps stays the game's in the civ list
- [Test 50](#test-50): the Generic base civ has exactly the universal nodes
- [Test 51](#test-51): choosing Generic takes away a regional node
- [Test 52](#test-52): a base civ has the nodes its civ has in the game
- [Test 53](#test-53): a base civ gives the civ exactly its bonuses
- [Test 54](#test-54): a base civ gives the civ its team bonus
- [Test 55](#test-55): a base civ gives the civ its unique unit
- [Test 56](#test-56): a building the civ lacks is not available in its tech tree
- [Test 57](#test-57): a building the civ has is available in its tech tree
- [Test 58](#test-58): a civ with an empty tree keeps the game's tech tree
- [Test 59](#test-59): a civ keeps its villagers in its tech tree
- [Test 60](#test-60): a building the civ has keeps the research it needs in its tech tree
- [Test 61](#test-61): only the tech of a unit the civ lacks is disabled
- [Test 62](#test-62): only a tech the civ lacks is disabled
- [Test 63](#test-63): a civ keeps its villagers in the dat file
- [Test 64](#test-64): a building the replaced civ lacked takes the status most civs give it
- [Test 65](#test-65): the replaced civ loses its own techs
- [Test 66](#test-66): the unique unit is made by a tech of the civ
- [Test 67](#test-67): a unique tech is researched at the castle in its age
- [Test 68](#test-68): a built unique tech changes the civ's own unit
- [Test 69](#test-69): a chosen cost takes the place of the tech's own
- [Test 70](#test-70): the built tech tree disables the tech of a unit the civ lacks
- [Test 71](#test-71): the report counts the unique techs built
- [Test 72](#test-72): the built unique unit is named by the string id of its slot
- [Test 73](#test-73): a civ bonus made of a civ's tech is copied for the civ
- [Test 74](#test-74): a copied tech's effect keeps the amounts of its changes
- [Test 75](#test-75): a copied effect names the copy of a copied tech
- [Test 76](#test-76): a civ bonus made of a generic tech unlocks it
- [Test 77](#test-77): the report counts the civ bonuses built
- [Test 78](#test-78): the report counts a civ bonus the catalog lacks as skipped
- [Test 79](#test-79): a bonus with nothing to build is counted as skipped
- [Test 80](#test-80): the report counts a multiplier of a bonus made of the game's techs
- [Test 81](#test-81): a bonus requirement is a tech of the civ
- [Test 82](#test-82): two techs given one requirement share its copy
- [Test 83](#test-83): a bonus of requirements is counted as built
- [Test 84](#test-84): a bonus whose tech has no room is skipped
- [Test 85](#test-85): a second build keeps no requirement of the first
- [Test 86](#test-86): a tech that no line names keeps its requirements
- [Test 87](#test-87): the team bonus takes the changes of its effect
- [Test 88](#test-88): the team bonus names the game's own techs
- [Test 89](#test-89): a team bonus taken twice doubles the changes of its effect
- [Test 90](#test-90): the elite unit's tech takes the string id of its slot
- [Test 91](#test-91): a unique tech makes no change twice for the civ's unit
- [Test 92](#test-92): a unique tech that enabled the old unit enables the civ's own
- [Test 93](#test-93): a unique tech no longer upgrades the old unit
- [Test 94](#test-94): a unit that a unique tech spawns becomes the civ's elite unit
- [Test 95](#test-95): the tech tree unlocks before it disables
- [Test 96](#test-96): a tech of resources that is researched nowhere stays
- [Test 97](#test-97): the tree's changes are sorted, unlocks first, then by tech
- [Test 98](#test-98): two disabled techs are two changes of the tree
- [Test 99](#test-99): the report says that the unique unit was skipped
- [Test 100](#test-100): the report counts the unique techs the catalog lacks
- [Test 101](#test-101): a hero is hidden in the tech tree
- [Test 102](#test-102): an empty tree disables no tech
- [Test 103](#test-103): a team bonus without an effect is built from its changes
- [Test 104](#test-104): a team bonus of changes taken twice doubles them
- [Test 105](#test-105): a bonus of techs builds its lines too
- [Test 106](#test-106): the lines of a bonus of techs count once
- [Test 107](#test-107): a team bonus with nothing to build is counted as skipped
- [Test 108](#test-108): a skipped team bonus is not counted as built
- [Test 109](#test-109): a built team bonus is not counted as skipped
- [Test 110](#test-110): a named hero takes the string id of its slot
- [Test 111](#test-111): a copied tech requires the copy of a civ's tech it required
- [Test 112](#test-112): a copied tech still requires a tech of no civ
- [Test 113](#test-113): a chosen scout is written as the civ's scout
- [Test 114](#test-114): a hero that the replaced civ lacks is skipped and reported
- [Test 115](#test-115): a dat file with sounds graphics tasks and a tech tree is written back
- [Test 116](#test-116): the tech that makes the unique unit belongs to the civ
- [Test 117](#test-117): a unique tech asks for the age of its tier
- [Test 118](#test-118): the report counts the techs that the tree disables
- [Test 119](#test-119): a unique unit not built counts no techs
- [Test 120](#test-120): a warship takes the look of its architecture's civ
- [Test 121](#test-121): a trade cart's load takes the look of its architecture's civ
- [Test 122](#test-122): a monk's healing takes the look of its monk skin
- [Test 123](#test-123): a castle takes the look of the civ chosen for the castle
- [Test 124](#test-124): a soldier keeps the look of the replaced civ
- [Test 125](#test-125): the look of an architecture is the civ that the catalog names
- [Test 126](#test-126): a look line of a chosen bonus gives its unit the look of its civ
- [Test 127](#test-127): a look line of a bonus not chosen leaves its unit its look
- [Test 128](#test-128): a look line gives its unit the attack timing of its civ
- [Test 129](#test-129): a look line gives its unit the attack graphic of its civ
- [Test 130](#test-130): a look line gives its unit the walking graphic of its civ
- [Test 131](#test-131): a look line gives its unit the icon of its civ
- [Test 132](#test-132): an added amount taken three times is tripled
- [Test 133](#test-133): a multiplied attribute taken twice is squared
- [Test 134](#test-134): a multiplied resource taken twice is squared
- [Test 135](#test-135): a tech cost multiplied in mode two taken twice is squared
- [Test 136](#test-136): a tech time multiplied in mode two taken twice is squared
- [Test 137](#test-137): a tech cost added in mode one is kept
- [Test 138](#test-138): a spawn taken twice makes twice the units
- [Test 139](#test-139): a resource taken twice is doubled
- [Test 140](#test-140): the spawn limit is kept
- [Test 141](#test-141): the effect function is kept
- [Test 142](#test-142): a set attribute is kept
- [Test 143](#test-143): an armor added past 255 is split in two
- [Test 144](#test-144): an armor taken away keeps its sign
- [Test 145](#test-145): an attack multiplied past 255 percent is split in two
- [Test 146](#test-146): a multiplied attack halfway is rounded to even
- [Test 147](#test-147): a multiplied attack past a half is rounded up
- [Test 148](#test-148): a multiplied attack below a half is rounded down
- [Test 149](#test-149): an armor added to 255 is one change
- [Test 150](#test-150): a built mod has a data folder named after the civ
- [Test 151](#test-151): a built mod has a ui folder named after the civ
- [Test 152](#test-152): the data mod's info names the civ
- [Test 153](#test-153): the ui mod's info says that it is the ui mod
- [Test 154](#test-154): the data mod holds the dat file with the civ's techs
- [Test 155](#test-155): the data mod's list of civs names the civ's unique unit
- [Test 156](#test-156): the data mod holds the tech tree file of the replaced civ
- [Test 157](#test-157): the ui mod names the civ in english
- [Test 158](#test-158): the ui mod is written for the last language of the game
- [Test 159](#test-159): a game folder without its list of civs is reported
- [Test 160](#test-160): a data file that is not the game's is reported
- [Test 161](#test-161): a game without the civ's tech tree file is reported
- [Test 162](#test-162): a civ with parts the catalog lacks is built and reported
- [Test 163](#test-163): a line break in a text is written as an escape
- [Test 164](#test-164): a carriage return before a line break is left out
- [Test 165](#test-165): a quote in a text is escaped
- [Test 166](#test-166): a quote that is escaped already is left as it is
- [Test 167](#test-167): a unique tech's label ends its description without a full stop
- [Test 168](#test-168): a unique tech without a description has a plain label
- [Test 169](#test-169): a unit's tooltip carries its description
- [Test 170](#test-170): the first tip names the first civ bonus
- [Test 171](#test-171): the first tip names the team bonus of a civ without civ bonuses
- [Test 172](#test-172): the tip of a unique tech says where it is researched
- [Test 173](#test-173): the second tip names the unique unit
- [Test 174](#test-174): a line break in a tip is written as a space
- [Test 175](#test-175): the game is found in the folder of steam
- [Test 176](#test-176): the game is found in a library that steam lists
- [Test 177](#test-177): the game is not found where no library holds it
- [Test 178](#test-178): the local mods folder of a profile is found
- [Test 179](#test-179): a home without a profile of the game has no mods folder
- [Test 180](#test-180): a number with a minus sign is negative
- [Test 181](#test-181): a number with a letter in it is no number
- [Test 182](#test-182): a number past the largest long is no number
- [Test 183](#test-183): the largest number a long holds is read
- [Test 184](#test-184): a letter of two bytes is widened to one wide letter
- [Test 185](#test-185): a letter of three bytes is widened to one wide letter
- [Test 186](#test-186): a letter beyond the basic plane is widened to two
- [Test 187](#test-187): the first letter beyond the basic plane is widened to two
- [Test 188](#test-188): a byte that starts no letter is not widened
- [Test 189](#test-189): a text that starts inside a letter is not widened
- [Test 190](#test-190): a letter cut short is not widened
- [Test 191](#test-191): a text longer than its place is not widened
- [Test 192](#test-192): a field that is a dash reads as empty
- [Test 193](#test-193): a field the line lacks reads as no text
- [Test 194](#test-194): hundredths keep two digits after the point
- [Test 195](#test-195): hundredths of one digit after the point end in zero
- [Test 196](#test-196): a second point makes no hundredths
- [Test 197](#test-197): a decimal with a minus sign is negative
- [Test 198](#test-198): a decimal with a letter in it is no decimal
- [Test 199](#test-199): a minus sign alone is no decimal
- [Test 200](#test-200): an escaped letter of four hex digits is read
- [Test 201](#test-201): an escape with capital hex digits is read
- [Test 202](#test-202): an escaped pair of surrogates is one letter
- [Test 203](#test-203): an escape that is not hex is unreadable
- [Test 204](#test-204): a high surrogate alone is unreadable
- [Test 205](#test-205): a high surrogate before a plain letter is unreadable
- [Test 206](#test-206): an escaped new line is read
- [Test 207](#test-207): an escaped tab is read
- [Test 208](#test-208): an escaped carriage return is read
- [Test 209](#test-209): an escaped quote is read
- [Test 210](#test-210): a string longer than its place is not copied
- [Test 211](#test-211): a text that is a list is no civ file
- [Test 212](#test-212): an object left open is unreadable
- [Test 213](#test-213): edits made out of order are written in the file's order
- [Test 214](#test-214): an escape with the hex digits a and f is read
- [Test 215](#test-215): an escape with the capital hex digit f is read
- [Test 216](#test-216): the first letter of two bytes is read
- [Test 217](#test-217): the first letter of three bytes is read
- [Test 218](#test-218): the first letter beyond the basic plane is read
- [Test 219](#test-219): the last letter of unicode is read
- [Test 220](#test-220): a stream of dynamic codes with repeated lengths inflates
- [Test 221](#test-221): a text is not equal to a longer one that starts with it
- [Test 222](#test-222): a text is not equal to its own beginning
- [Test 223](#test-223): a text that fills its place is copied
- [Test 224](#test-224): a text one letter too long for its place is not copied
- [Test 225](#test-225): a text that does not fit is not appended
- [Test 226](#test-226): a negative number is appended with its minus sign
- [Test 227](#test-227): a capital z is found by a small z
- [Test 228](#test-228): a text does not contain a letter it lacks
- [Test 229](#test-229): two members are written with a comma between
- [Test 230](#test-230): the first member of an object inside another has no comma
- [Test 231](#test-231): an object left open is not a finished text
- [Test 232](#test-232): a text too long for the writer is reported
- [Test 233](#test-233): a line of a file is split at its tabs
- [Test 234](#test-234): a number field of a file is read
- [Test 235](#test-235): a text field of a file is read
- [Test 236](#test-236): a list of numbers is read in its order
- [Test 237](#test-237): a list of slots is read as bits
- [Test 238](#test-238): the second line of a file follows the first
- [Test 239](#test-239): a file ends after its last line
- [Test 240](#test-240): a file that is missing is not opened
- [Test 241](#test-241): the catalog holds every civ bonus of its file
- [Test 242](#test-242): the catalog holds every team bonus of its file
- [Test 243](#test-243): the catalog holds every unit of its file
- [Test 244](#test-244): the catalog holds every tech of its file
- [Test 245](#test-245): the catalog holds every node of its file
- [Test 246](#test-246): the catalog holds every requirement line of its file
- [Test 247](#test-247): a requirement line names its bonus, its tech and the tech required
- [Test 248](#test-248): a train line is a unit patch placed in the train location
- [Test 249](#test-249): a cost line is a unit patch placed in the costs
- [Test 250](#test-250): a look line names its bonus, its unit and the civ of the look
- [Test 251](#test-251): the catalog holds every look line of its file
- [Test 252](#test-252): the first unit of the catalog has its name
- [Test 253](#test-253): a units speed is kept in hundredths
- [Test 254](#test-254): the first unit of the catalog is a unit of the game
- [Test 255](#test-255): a catalog folder that is missing is not loaded
- [Test 256](#test-256): a command to pick a unit picks it
- [Test 257](#test-257): a command to pick a tech picks it
- [Test 258](#test-258): a command to pick a hero picks it
- [Test 259](#test-259): a command to tick a civ bonus ticks it
- [Test 260](#test-260): a command to tick a team bonus ticks it
- [Test 261](#test-261): a command to tick a node ticks it
- [Test 262](#test-262): a command to set a choice sets it
- [Test 263](#test-263): a command of no kind changes nothing
- [Test 264](#test-264): a civ bonus ticked twice is no longer picked
- [Test 265](#test-265): a team bonus ticked twice is no longer picked
- [Test 266](#test-266): a row of the tech tree gives the command to tick its node
- [Test 267](#test-267): a row of the civ bonuses gives the command to tick its bonus
- [Test 268](#test-268): a row of the team bonuses gives the command to tick its bonus
- [Test 269](#test-269): a row of a look gives the command to set that choice
- [Test 270](#test-270): a row of the units gives the command to pick its unit
- [Test 271](#test-271): a row of the heroes gives the command to pick its hero
- [Test 272](#test-272): the row no hero gives the command to pick no hero
- [Test 273](#test-273): a row of the castle techs gives the command to pick its tech
- [Test 274](#test-274): a row of the imperial techs gives the command to pick its tech
- [Test 275](#test-275): a row of the castle techs shows its tech's name
- [Test 276](#test-276): the summary names the unique unit and the unit it is made from
- [Test 277](#test-277): the summary lists a civ bonus by its description
- [Test 278](#test-278): the summary names a civ bonus the catalog lacks by its number
- [Test 279](#test-279): the summary names a unique tech by its catalog name
- [Test 280](#test-280): the summary says when the catalog lacks the unique unit
- [Test 281](#test-281): the summary says when the catalog lacks a unique tech
- [Test 282](#test-282): the build report counts the civ bonuses built and skipped
- [Test 283](#test-283): the build report says when the hero was not built
- [Test 284](#test-284): the build report says when a bonus counts only once
- [Test 285](#test-285): the build report of a whole civ names the two folders
- [Test 286](#test-286): no step of a new design is done
- [Test 287](#test-287): the identity step is done once the civ has a name
- [Test 288](#test-288): the tech tree step is done once a node is chosen
- [Test 289](#test-289): the bonuses step is done once a bonus is ticked
- [Test 290](#test-290): the team bonus step is done once one is picked
- [Test 291](#test-291): a tree that swaps a node of the base civ is changed
- [Test 292](#test-292): bonuses that swap one of the base civ are changed
- [Test 293](#test-293): the unique unit step is done once a unit is picked
- [Test 294](#test-294): the castle tech step is done once a tech is picked
- [Test 295](#test-295): the imperial tech step is done once a tech is picked
- [Test 296](#test-296): the hero step is done once a hero is picked
- [Test 297](#test-297): a pick finishes only its own step
- [Test 298](#test-298): back from the second step goes to the first
- [Test 299](#test-299): a team bonus other than the base civ's is a change
- [Test 300](#test-300): the base civ's own team bonus is no change
- [Test 301](#test-301): a new design has nothing to undo
- [Test 302](#test-302): a new design has nothing to redo
- [Test 303](#test-303): an undo that fails leaves nothing to redo
- [Test 304](#test-304): a change that is made is reported as made
- [Test 305](#test-305): a change after an undo leaves nothing to redo
- [Test 306](#test-306): undo goes back as many changes as the history holds
- [Test 307](#test-307): undo past the depth brings back the change before
- [Test 308](#test-308): a design put in place of another can be undone
- [Test 309](#test-309): a design put in place of another leaves nothing to redo
- [Test 310](#test-310): a change after typing is undone by itself
- [Test 311](#test-311): typing after an undo is a change of its own
- [Test 312](#test-312): a ticked bonus counts once
- [Test 313](#test-313): a picked team bonus counts once
- [Test 314](#test-314): a multiplier past ten is refused
- [Test 315](#test-315): a multiplier below one is refused
- [Test 316](#test-316): a bonus ticked twice is not chosen
- [Test 317](#test-317): a node chosen twice is not chosen
- [Test 318](#test-318): taking a node out keeps the other nodes
- [Test 319](#test-319): a tree takes no more nodes than it holds
- [Test 320](#test-320): a unit of the game is saved with no custom units index
- [Test 321](#test-321): a custom unit is saved with no id of the game
- [Test 322](#test-322): the language of the britons is saved as index 0
- [Test 323](#test-323): a new civ is saved as civbuilder civ by default
- [Test 324](#test-324): a named civ is saved under its name
- [Test 325](#test-325): a save that is cancelled says that the civ was not saved
- [Test 326](#test-326): a saved civ is opened again
- [Test 327](#test-327): a file that is no civ file is refused with a message
- [Test 328](#test-328): the next button goes to the next step
- [Test 329](#test-329): a typed name is shown in the title of the window
- [Test 330](#test-330): redo says when there is nothing to redo
- [Test 331](#test-331): no step is shown as done when the window opens
- [Test 332](#test-332): closing is stopped when the question of saving is cancelled
- [Test 333](#test-333): closing without saving closes the window
- [Test 334](#test-334): the exit command closes a window without changes
- [Test 335](#test-335): build is refused when the game is not found
- [Test 336](#test-336): build writes nothing when the question is not confirmed
- [Test 337](#test-337): build of an unfinished civ says that it is not finished
- [Test 338](#test-338): build from the window reports the mod it built
- [Test 339](#test-339): the help line is written in the muted colour
- [Test 340](#test-340): a field is written on the colour of fields
- [Test 341](#test-341): a row in focus is drawn with a frame in the focus colour
- [Test 342](#test-342): picking the picked row again makes no new change
- [Test 343](#test-343): a list's scroll bar shows its arrows while the mouse is away
- [Test 344](#test-344): the window's scroll bar shows its arrows while the mouse is away
- [Test 345](#test-345): a step ends a margin from the window's scroll bar
- [Test 346](#test-346): the knob of the window's scroll bar reaches its end with the step
- [Test 347](#test-347): a step that just fits the window shows no scroll bar
- [Test 348](#test-348): the up arrow of the window's scroll bar moves the step up a line
- [Test 349](#test-349): each wheel notch moves a list one row its way
- [Test 350](#test-350): a press on either arrow moves a list one row its way
- [Test 351](#test-351): a shown list scrolls without animation
- [Test 352](#test-352): a held arrow keeps scrolling a list
- [Test 353](#test-353): a press between a list's arrows moves it a page
- [Test 354](#test-354): a key in a list moves its caret one row

Accessibility tests:

- [Test 355](#test-355): every text contrasts at least 4.5:1 with its background
- [Test 356](#test-356): every input has a visible label
- [Test 357](#test-357): every control one acts on is at least 44 by 44
- [Test 358](#test-358): no two controls of a step overlap, even in the smallest window
- [Test 359](#test-359): the window's title names the civ and the step
- [Test 360](#test-360): the list of steps is on every step, with Back and Next wherever they lead
- [Test 361](#test-361): a window narrower than the wide layout reflows
- [Test 362](#test-362): a window shorter than the wide layout reflows
- [Test 363](#test-363): a window as large as the wide layout does not reflow
- [Test 364](#test-364): no control reaches past a window 320 wide
- [Test 365](#test-365): no two controls overlap in a window 320 wide
- [Test 366](#test-366): every control one acts on is at least 44 by 44 in a window 320 wide
- [Test 367](#test-367): a step is as high as its last control and a margin
- [Test 368](#test-368): a control below the window is scrolled up into it
- [Test 369](#test-369): a control above the window is scrolled down into it
- [Test 370](#test-370): a control in the window leaves it where it is
- [Test 371](#test-371): the window scrolls no further than its content
- [Test 372](#test-372): the window scrolls no higher than its top
- [Test 373](#test-373): a window that holds its content does not scroll
- [Test 374](#test-374): the first control of a step comes under the help
- [Test 375](#test-375): Back and Next come under the status line
- [Test 376](#test-376): a control starts a margin from the left
- [Test 377](#test-377): Next ends a margin from the right
- [Test 378](#test-378): the Tab key reaches every control of every step
- [Test 379](#test-379): the list of steps shows the current step as the chosen one
- [Test 380](#test-380): the Tab key follows the order in which a step is read
- [Test 381](#test-381): a ticked row leaves the list of bonuses where it was scrolled
- [Test 382](#test-382): every control tells a screen reader a name that begins with its label
- [Test 383](#test-383): a ticked row is told to a screen reader as chosen
- [Test 384](#test-384): moving the focus through every control changes nothing
- [Test 385](#test-385): the Tab key leaves every control
- [Test 386](#test-386): Windows is told that the window's language is English
- [Test 387](#test-387): the help key says what the control in focus is for
- [Test 388](#test-388): the status line is a live region that a screen reader is told of
- [Test 389](#test-389): the text and the layout grow with the text size of Windows, up to 225%
- [Test 390](#test-390): the control in focus has a solid frame at least two pixels thick
- [Test 391](#test-391): a ticked row is drawn with a check, not a filled box
- [Test 392](#test-392): after an undo, the base civ shown is the design's again
- [Test 393](#test-393): a narrow window places its controls in one column
- [Test 394](#test-394): the control in focus is scrolled into a short window
- [Test 395](#test-395): the window may be as small as 320 by 256
- [Test 396](#test-396): Scroll down moves a short window down a page
- [Test 397](#test-397): Scroll up takes back a Scroll down
- [Test 398](#test-398): the wheel scrolls a short window
- [Test 399](#test-399): a long row of a narrow list is higher than a short one
- [Test 400](#test-400): a short row of a list is as high as a target

Maintainability tests:

- [Test 401](#test-401): no comments in the code
- [Test 402](#test-402): no function over 60 lines
- [Test 403](#test-403): no line of code over 120 characters
- [Test 404](#test-404): no file over 400 lines of code
- [Test 405](#test-405): no goto, setjmp or longjmp
- [Test 406](#test-406): no recursion, direct or indirect
- [Test 407](#test-407): no dynamic memory allocation
- [Test 408](#test-408): preprocessor only includes and guards
- [Test 409](#test-409): one level of pointer dereferencing
- [Test 410](#test-410): no function pointers but the window procedure
- [Test 411](#test-411): no folder over 7 files, the main folder 8
- [Test 412](#test-412): no two files share a name
- [Test 413](#test-413): only C code and documents in the project
- [Test 414](#test-414): every placeholder of the template is filled in
- [Test 415](#test-415): the development log keeps its form
- [Test 416](#test-416): TESTS.md numbers its tests in order
- [Test 417](#test-417): TESTS.md documents every test
- [Test 418](#test-418): every line of TERMINOLOGY.md is short
- [Test 419](#test-419): the rule against goto names the line of a goto
- [Test 420](#test-420): the rule against allocation names the line of a malloc
- [Test 421](#test-421): the rule of the preprocessor names a macro with arguments
- [Test 422](#test-422): the rule of one dereference names two arrows in a row
- [Test 423](#test-423): the rule against function pointers names one
- [Test 424](#test-424): the rule against comments names the line of a comment
- [Test 425](#test-425): the rule against recursion names a function that calls itself
- [Test 426](#test-426): the sources of a folder include those of its folders
- [Test 427](#test-427): a folder lists only its own sources
- [Test 428](#test-428): the sources of the project are those of src, tests and tools
- [Test 429](#test-429): the newest source is the one changed last
- [Test 430](#test-430): the object of a source is named after it in the folder given
- [Test 431](#test-431): a source older than its object is not built again
- [Test 432](#test-432): a source with a newer header is built again
- [Test 433](#test-433): a source changed when its object was made is built again
- [Test 434](#test-434): the objects of the build tool are listed without libraries
- [Test 435](#test-435): a coverage build is asked for by coverage
- [Test 436](#test-436): the tests are run with the arguments given to the build
- [Test 437](#test-437): a coverage build links the tests alone
- [Test 438](#test-438): a coverage build links the tests with coverage
- [Test 439](#test-439): a coverage build makes the folder of its report

## Test 1

### File residence:
`tests/functional/core_tests.c`, lines 6-13

### Purpose:
Verify that a failed check tells where it failed and what, so a fault can be found (Rule 26, P10 rule 5).

### Assertion expectation:
After `CHECK(width > 5)` and `CHECK(width > 4)` fail, the last failure is the file and `:11: check failed: width > 4`.

## Test 2

### File residence:
`tests/functional/civ_file_tests.c`, lines 16-19

### Purpose:
Verify that a civ file of the original opens, with its name (Req. 1).

### Assertion expectation:
`tests/fixtures/example.civbuilder.json` loads, and the design's name is "Vanguards".

## Test 3

### File residence:
`tests/functional/civ_file_tests.c`, lines 21-25

### Purpose:
Verify that a civ file of the original opens with its bonuses (Req. 1).

### Assertion expectation:
The example civ has exactly two bonuses, 9 and 29.

## Test 4

### File residence:
`tests/functional/civ_file_tests.c`, lines 27-31

### Purpose:
Verify that a civ file of the original opens with its unique unit (Req. 1).

### Assertion expectation:
The example civ's unique unit is the custom unit with index 5.

## Test 5

### File residence:
`tests/functional/civ_file_tests.c`, lines 33-36

### Purpose:
Prevent again a civ file's language from being read as the civ after it: the original counts from 0 (Req. 1).

### Assertion expectation:
The example civ's language 4, the Japanese in the original's count, is read as slot 5, the Japanese's.

## Test 6

### File residence:
`tests/functional/civ_file_tests.c`, lines 96-101

### Purpose:
Verify that saving and opening a civ loses nothing (Req. 1).

### Assertion expectation:
A design written as JSON and read back keeps its texts, quotes and line breaks included, and is written as before.

## Test 7

### File residence:
`tests/functional/civ_file_tests.c`, lines 103-109

### Purpose:
Verify that a civ file keeps how many times each civ bonus counts, as the original's files do (Req. 8).

### Assertion expectation:
A design whose bonus 17 counts 10 times is written, then read back with a `multiplier` of 10.

## Test 8

### File residence:
`tests/functional/model_tests.c`, lines 23-30

### Purpose:
Verify that undo takes back the last change, so a slip costs nothing (Req. 5).

### Assertion expectation:
After bonus 7 is chosen and undone, undo reports success and the civ has no bonus.

## Test 9

### File residence:
`tests/functional/model_tests.c`, lines 32-39

### Purpose:
Verify that a name typed letter by letter is undone as one change, not a letter at a time (Req. 5).

### Assertion expectation:
After "V", "Vi" and "Vik" are typed as the name, one undo leaves the name empty.

## Test 10

### File residence:
`tests/functional/model_tests.c`, lines 41-49

### Purpose:
Verify that redo makes again a change that was undone (Req. 5).

### Assertion expectation:
After unit 1263 is picked, undone and redone, redo reports success and the unique unit is 1263 again.

## Test 11

### File residence:
`tests/functional/model_tests.c`, lines 51-61

### Purpose:
Prevent a bonus past the fixed 24 places from being written beyond them (Req. 6).

### Assertion expectation:
After 24 bonuses are chosen, a 25th is refused and the civ still has 24.

## Test 12

### File residence:
`tests/functional/model_tests.c`, lines 63-75

### Purpose:
Verify that Next leads through the nine steps in their order and stays on the review (Req. 3).

### Assertion expectation:
From the identity step, each Next gives the step after it, and Next on the review gives the review.

## Test 13

### File residence:
`tests/functional/model_tests.c`, lines 87-90

### Purpose:
Verify that the review names what the original's steps would not let pass yet (Req. 3).

### Assertion expectation:
For a civ with only a name, the summary reads "Still needed before a mod can be built: a unique unit; a castle tech;".

## Test 14

### File residence:
`tests/functional/model_tests.c`, lines 92-95

### Purpose:
Prevent again the review losing its labels, a bug found on 2026-11-09 (Rule 24, a regression test).

### Assertion expectation:
For a civ named "Vikings", the summary holds the line "Name: Vikings".

## Test 15

### File residence:
`tests/functional/model_tests.c`, lines 97-100

### Purpose:
Prevent again a Next button on the review, the last step, a bug found on 2026-11-09 (Rule 24, a regression test).

### Assertion expectation:
`layout_shows(CONTROL_NEXT, STEP_REVIEW)` is false, so the review step has no Next button.

## Test 16

### File residence:
`tests/pack/pack_tests.c`, lines 44-49

### Purpose:
Verify that a stream with its own Huffman codes inflates, as the game's dat file is packed (Req. 8).

### Assertion expectation:
The 91 bytes gzip made of a 119-character text inflate to exactly that text.

## Test 17

### File residence:
`tests/pack/pack_tests.c`, lines 51-56

### Purpose:
Verify that a stream with the fixed Huffman codes of RFC 1951 inflates (Req. 8).

### Assertion expectation:
The 7 bytes `cb 48 cd c9 c9 07 00` inflate to "hello".

## Test 18

### File residence:
`tests/pack/pack_tests.c`, lines 58-76

### Purpose:
Verify that a dat file packed for a mod in stored blocks reads back unchanged (Req. 8).

### Assertion expectation:
140 000 bytes, packed by `deflate_store` in three blocks and inflated again, are the same 140 000 bytes.

## Test 19

### File residence:
`tests/pack/pack_tests.c`, lines 119-134

### Purpose:
Verify that a dat file compressed for a mod reads back unchanged (Req. 8).

### Assertion expectation:
140 000 bytes of copied runs and random bytes, packed by `deflate_pack` and inflated again, are the same 140 000 bytes.

## Test 20

### File residence:
`tests/pack/pack_tests.c`, lines 136-146

### Purpose:
Verify that a mod's dat file is compressed, as the original compresses it, not only stored (Req. 8).

### Assertion expectation:
140 000 bytes that repeat every 251 bytes are packed by `deflate_pack` into fewer than 14 000 bytes.

## Test 21

### File residence:
`tests/dat/dat_tests.c`, lines 66-74

### Purpose:
Verify that a mod's dat file keeps the game's data wherever nothing is edited (Req. 8).

### Assertion expectation:
A minimal valid dat file of 82 678 bytes, mapped and written with an empty edit, is the same 82 678 bytes.

## Test 22

### File residence:
`tests/dat/dat_tests.c`, lines 76-91

### Purpose:
Verify that an effect added for the civ is written as the game holds an effect (Req. 8).

### Assertion expectation:
Loom's effect with its 3 changes, added to an empty dat file, is written as the 43 bytes the game's own file holds.

## Test 23

### File residence:
`tests/dat/dat_tests.c`, lines 93-102

### Purpose:
Verify that a tech added for the civ is written as the game holds a tech (Req. 8).

### Assertion expectation:
The tech Loom, added to an empty dat file, is written as the 75 bytes the game's own file holds.

## Test 24

### File residence:
`tests/dat/dat_tests.c`, lines 131-149

### Purpose:
Verify that a civ bonus that the catalog makes from changes, not techs, is built for the civ (Req. 8).

### Assertion expectation:
Two changes of bonus 55 make one tech of civ 1 that needs tech 102 and fires a new effect with the 2 changes.

## Test 25

### File residence:
`tests/dat/dat_tests.c`, lines 151-167

### Purpose:
Verify that a civ bonus made of changes and taken twice counts twice in the game (Req. 8).

### Assertion expectation:
Bonus 55 taken twice is built with its +1 pierce armor made +2: the change's `d` is 770.

## Test 26

### File residence:
`tests/mod/mod_tests.c`, lines 36-44

### Purpose:
Verify that the civ's name replaces the name of the civ it replaces in the UI mod's strings (Req. 8).

### Assertion expectation:
For "Testonia" in the slot of the Britons, the strings start with the line `10271 "Testonia"`.

## Test 27

### File residence:
`tests/mod/mod_tests.c`, lines 46-58

### Purpose:
Prevent again civ descriptions written out of order, a bug found on 2026-11-09 (Rule 24, a regression test).

### Assertion expectation:
With the game's 120201 line before its 120200 line, the strings hold 120200 before 120201.

## Test 28

### File residence:
`tests/mod/mod_tests.c`, lines 60-68

### Purpose:
Verify that the mod's folders are named from the civ's name as the original names them (Req. 7).

### Assertion expectation:
"Æsir Kings 2!" gives the folder name "_sir_kings_2_": one "_" for each character that is not a-z, 0-9, _ or -.

## Test 29

### File residence:
`tests/mod/mod_tests.c`, lines 70-82

### Purpose:
Verify that no mod is built from a civ the review still finds unfinished (Req. 9).

### Assertion expectation:
A civ with a name but no unique unit or techs is refused with a problem that says it is "not finished".

## Test 30

### File residence:
`tests/mod/mod_tests.c`, lines 96-103

### Purpose:
Verify that a unique tech taken from another civ works on the civ's own unique unit (Req. 8).

### Assertion expectation:
A change of +1 range for Throwing Axeman 281, the other civ's unique unit, is aimed at the civ's Longbowman 8.

## Test 31

### File residence:
`tests/mod/mod_tests.c`, lines 154-163

### Purpose:
Verify that the name given to a unique tech is the name the game shows for it (Req. 8).

### Assertion expectation:
A castle tech named "Testing Axes" in the slot of the Britons gives the line `44008 "Testing Axes"` in the strings.

## Test 32

### File residence:
`tests/mod/mod_tests.c`, lines 169-175

### Purpose:
Verify that the name given to a unique unit of the game is the name the game shows for it (Req. 8).

### Assertion expectation:
The Longbowman named "Testbowman" in the slot of the Britons gives the line `69802 "Testbowman"` in the strings.

## Test 33

### File residence:
`tests/dat/dat_tests.c`, lines 171-183

### Purpose:
Verify that the civ's buildings look as those of the civ its architecture names (Req. 8).

### Assertion expectation:
A building of graphic 101 in slot 1 is given graphic 100, that of the same building of civ 0, the architecture's civ.

## Test 34

### File residence:
`tests/dat/dat_tests.c`, lines 187-201

### Purpose:
Verify that the civ's monks look as those of the monk skin chosen for it (Req. 8).

### Assertion expectation:
A monk, unit 125, of graphic 101 in slot 1 is given graphic 100, that of the monk of civ 0, whose skin is chosen.

## Test 35

### File residence:
`tests/dat/dat_tests.c`, lines 213-233

### Purpose:
Verify that the hero chosen for the civ is trained at the Castle, as in the original (Req. 8).

### Assertion expectation:
Unit 5 of slot 1, trained nowhere in the small dat file, is given building 82, the Castle, as its train location.

## Test 36

### File residence:
`tests/mod/mod_tests.c`, lines 124-127

### Purpose:
Prevent a unique tech from making two changes of one attribute of the civ's unit that differ only in amount (Req. 8).

### Assertion expectation:
`unique_same_target` says that +10 and +300 hit points for the Longbowman 8 change one target.

## Test 37

### File residence:
`tests/mod/mod_tests.c`, lines 129-132

### Purpose:
Verify that a unique tech keeps its changes of the armor of two classes, as each changes its own (Req. 8).

### Assertion expectation:
`unique_same_target` says that +5 pierce armor (class 3) and +5 melee armor (class 4) are two targets.

## Test 38

### File residence:
`tests/mod/mod_tests.c`, lines 198-201

### Purpose:
Verify that the name given to the hero is the name the game shows for it (Req. 8).

### Assertion expectation:
Liu Bei, unit 1966, named "Testhero" in the slot of the Britons gives the line `44500 "Testhero"` in the strings.

## Test 39

### File residence:
`tests/mod/mod_tests.c`, lines 203-206

### Purpose:
Prevent the strings of a hero that was not built from being written (bug of 2026-11-09 05:56:32).

### Assertion expectation:
With the hero skipped, the UI mod's strings have no line `44500 "Testhero"` for slot 1's hero string id.

## Test 40

### File residence:
`tests/mod/mod_tests.c`, lines 238-241

### Purpose:
Verify that the civ list names the civ's own unique unit, as the original's does (Req. 8).

### Assertion expectation:
Slot 1 given the Longbowman, unit 8, takes the id 8 and the line -276 of the entry that has it; all else is kept.

## Test 41

### File residence:
`tests/mod/mod_tests.c`, lines 256-259

### Purpose:
Verify that a unique unit no civ of the game has shows its own icon and texts in the civ list (Req. 8).

### Assertion expectation:
Unit 9, absent from the list, makes slot 1's icon `007_50730.png` and its string ids 5200 and 26200; all else is kept.

## Test 42

### File residence:
`tests/dat/dat_tests.c`, lines 240-254

### Purpose:
Verify that a bonus's unit patch sets the field of the unit of its own civ (Req. 8).

### Assertion expectation:
A patch of bonus 330 sets collision size x, at +29, of unit 0 of civ 1 to 1.0; the bonus counts as built.

## Test 43

### File residence:
`tests/dat/dat_tests.c`, lines 302-308

### Purpose:
Verify that a train line sets the building where a unit of the civ is trained, as bonus 222 needs (Req. 8).

### Assertion expectation:
With unit 5 of slot 1 creatable, the two lines of bonus 222 build 2, and its first train location names building 68.

## Test 44

### File residence:
`tests/dat/dat_tests.c`, lines 310-316

### Purpose:
Verify that a train line sets the button that trains a unit of the civ, as bonus 222 needs (Req. 8).

### Assertion expectation:
With unit 5 of slot 1 creatable, the two lines of bonus 222 build 2, and its first train location has button 3.

## Test 45

### File residence:
`tests/dat/dat_tests.c`, lines 318-330

### Purpose:
Verify that a train line for a unit with no train location fails, so Build names its bonus (Req. 9).

### Assertion expectation:
With unit 5 of slot 1 a building, which has no train location, the lines of bonus 222 give BONUS_BUILD_FAILED.

## Test 46

### File residence:
`tests/dat/dat_tests.c`, lines 325-330

### Purpose:
Verify that a cost line sets a field of the costs of a unit of the civ, as bonus 460 needs (Req. 8).

### Assertion expectation:
With unit 5 of slot 1 creatable, the one cost line of bonus 460 builds 1, and its first cost has an amount of 300.

## Test 47

### File residence:
`tests/dat/dat_tests.c`, lines 332-337

### Purpose:
Verify that a cost line may set the last field of a unit's three costs, the third cost's flag (Req. 8).

### Assertion expectation:
A cost line at place 16, of 2 bytes, is built, and the flag of the third cost of the civ's unit becomes 1.

## Test 48

### File residence:
`tests/mod/mod_tests.c`, lines 270-273

### Purpose:
Prevent again a civ selection that shows the old civ's bonuses, a bug found on 2026-11-09 (Rule 24, a regression test).

### Assertion expectation:
An entry whose `tech_tree_description_string_id` is 120150 is written with 990101, the own id of slot 1.

## Test 49

### File residence:
`tests/mod/mod_tests.c`, lines 279-282

### Purpose:
Prevent the list of civs from changing a value that the civ keeps from the replaced civ (Req. 8).

### Assertion expectation:
`civ_list_write` with every value `CIV_LIST_KEEP` writes the list unchanged: the description id stays 120150.

## Test 50

### File residence:
`tests/base/base_tests.c`, lines 40-44

### Purpose:
Prevent again a new civ that starts with an empty tech tree, a bug found on 2026-11-09 (Rule 24, a regression test).

### Assertion expectation:
Generic gives the one universal node of the catalog, the Barracks, and not a universal row past the catalog's count.

## Test 51

### File residence:
`tests/base/base_tests.c`, lines 46-50

### Purpose:
Verify that choosing Generic after another base civ takes that civ's regional nodes away (Req. 8; order of 11-09).

### Assertion expectation:
A design given base civ 1 and then Generic no longer has the regional unit that civ 1 has.

## Test 52

### File residence:
`tests/base/base_tests.c`, lines 52-55

### Purpose:
Verify that a base civ gives the design the nodes its civ has in the game (Req. 8; base civ order of 2026-11-09).

### Assertion expectation:
Base civ 1 gives the design the regional unit whose list of civs holds civ 1.

## Test 53

### File residence:
`tests/base/base_tests.c`, lines 57-60

### Purpose:
Verify that a base civ gives the design its own civ bonuses and no others (Req. 8; base civ order of 2026-11-09).

### Assertion expectation:
Base civ 1, with bonus 3 alone in its list, gives the design exactly one bonus: bonus 3.

## Test 54

### File residence:
`tests/base/base_tests.c`, lines 62-65

### Purpose:
Verify that a base civ gives the design its team bonus (Req. 8; base civ order of 2026-11-09).

### Assertion expectation:
Base civ 1, whose team bonus is 0, gives the design team bonus 0.

## Test 55

### File residence:
`tests/base/base_tests.c`, lines 67-71

### Purpose:
Verify that a base civ gives the design its unique unit, as the game has it (Req. 8; base civ order of 2026-11-09).

### Assertion expectation:
Base civ 1, whose unique unit is 0, gives the design unit 0 of the game, of vanilla origin.

## Test 56

### File residence:
`tests/mod/mod_tests.c`, lines 313-316

### Purpose:
Prevent again a tech tree screen that shows units the civ lacks, a bug found on 2026-11-09 (Rule 24, a regression test).

### Assertion expectation:
A Stable that the game's tree has as ResearchedCompleted is written NotAvailable for a design without a Stable.

## Test 57

### File residence:
`tests/mod/mod_tests.c`, lines 318-321

### Purpose:
Prevent again a tech tree screen that hides what the civ has, a bug found on 2026-11-09 (Rule 24, a regression test).

### Assertion expectation:
A Stable that the game's tree has as NotAvailable is written ResearchedCompleted for a design with a Stable.

## Test 58

### File residence:
`tests/mod/mod_tests.c`, lines 323-326

### Purpose:
Verify that a civ with an empty tree keeps the game's tech tree screen, as its dat keeps every tech (Req. 8).

### Assertion expectation:
With nothing ticked, a Stable that the game's tree has as ResearchedCompleted is written unchanged.

## Test 59

### File residence:
`tests/mod/mod_tests.c`, lines 328-331

### Purpose:
Prevent a tech tree screen without the Villager, Town Centers, Mill and camps, which the dat never disables (Req. 8).

### Assertion expectation:
A Villager that the game's tree has as ResearchedCompleted is written unchanged for a design without it.

## Test 60

### File residence:
`tests/mod/mod_tests.c`, lines 333-336

### Purpose:
Verify that a building the civ has still needs the research the game asks for it, as the Keep does (Req. 8).

### Assertion expectation:
A Keep that the game's tree has as ResearchRequired is written unchanged for a design with a Keep.

## Test 61

### File residence:
`tests/dat/dat_tests.c`, lines 389-392

### Purpose:
Verify that the dat file disables the tech of a unit the civ lacks, and not that of a unit it has (Req. 8).

### Assertion expectation:
Of two techs that enable units 4 and 7, for a design with unit 4 only, tech 1 alone gets a change of type 102.

## Test 62

### File residence:
`tests/dat/dat_tests.c`, lines 394-397

### Purpose:
Verify that the dat file disables a tech of the tech tree the civ lacks, and not one it has (Req. 8).

### Assertion expectation:
Of techs 0 and 1, both in the catalog, for a design with tech 0 only, tech 1 alone gets a change of type 102.

## Test 63

### File residence:
`tests/dat/dat_tests.c`, lines 399-405

### Purpose:
Prevent a dat file that disables the Villager, which every civ keeps in the dat file and the tech tree file (Req. 8).

### Assertion expectation:
Of the techs that enable the Villager and unit 7, for a design with neither, only the second gets a change of type 102.

## Test 64

### File residence:
`tests/functional/functional_suite.c`, lines 270-272; `tests/mod/mod_tests.c`, lines 338-349

### Purpose:
Enforce that the tech tree screen shows a node as the game does for most civs (Requirement 8).

### Assertion expectation:
A Keep the replaced civ lacked, which most civs must research, is written `ResearchRequired` when the design has it.

## Test 65

### File residence:
`tests/dat/recipe_tests.c`, lines 7-10

### Purpose:
Verify that the techs of the replaced civ no longer belong to it in the built dat file (Req. 8).

### Assertion expectation:
The old unique unit's tech of civ 1 reads back with civ 99, the civ no player has.

## Test 66

### File residence:
`tests/dat/recipe_tests.c`, lines 12-15

### Purpose:
Verify that the tech that makes the chosen unique unit is copied for the replaced civ (Req. 8).

### Assertion expectation:
The effect of tech 14 of the built dat file enables unit 6, once.

## Test 67

### File residence:
`tests/dat/recipe_tests.c`, lines 17-20

### Purpose:
Verify that a built unique tech is researched at the Castle and asks for its age (Req. 8).

### Assertion expectation:
Tech 15, the castle tech, has research location 82, the Castle.

## Test 68

### File residence:
`tests/dat/recipe_tests.c`, lines 22-25

### Purpose:
Verify that a unique tech's change of its old civ's unique unit is aimed at the civ's own (Req. 8).

### Assertion expectation:
The effect of tech 16, the imperial tech, adds to an attribute of unit 6, once, where the game's adds to unit 5.

## Test 69

### File residence:
`tests/dat/recipe_tests.c`, lines 27-30

### Purpose:
Verify that the cost chosen for a unique tech is written in place of the catalog's (Req. 8).

### Assertion expectation:
The first cost of tech 15 has the amount 300.

## Test 70

### File residence:
`tests/dat/recipe_tests.c`, lines 32-35

### Purpose:
Verify that the civ's tech tree effect disables the tech of a node the design lacks (Req. 8).

### Assertion expectation:
The effect that civ 1's tech tree id names disables tech 5, once.

## Test 71

### File residence:
`tests/dat/recipe_tests.c`, lines 37-40

### Purpose:
Verify that the build's report counts the unique techs it built (Req. 9).

### Assertion expectation:
`unique_techs_built` is 2 for a design with both unique techs in the catalog.

## Test 72

### File residence:
`tests/dat/recipe_tests.c`, lines 42-45

### Purpose:
Verify that the civ's unique unit takes the name string id of the replaced civ's slot (Req. 8).

### Assertion expectation:
Unit 6 of civ 1 has name string id 111003 in the built dat file.

## Test 73

### File residence:
`tests/dat/recipe_tests.c`, lines 47-50

### Purpose:
Verify that a civ bonus whose tech belongs to another civ is copied for the replaced civ (Req. 8).

### Assertion expectation:
Tech 12, the copy of the bonus's tech 6, has civ 1.

## Test 74

### File residence:
`tests/dat/recipe_tests.c`, lines 52-55

### Purpose:
Verify that a tech copied for the civ does what the game's tech does, its changes counted once (Req. 8).

### Assertion expectation:
The copy of bonus tech 6, whose effect adds 5 to infantry, adds 5: the change's `d` is 5.

## Test 75

### File residence:
`tests/dat/recipe_tests.c`, lines 57-60

### Purpose:
Verify that a copied effect that changes a tech the recipe also copies changes the civ's copy of it (Req. 8).

### Assertion expectation:
The copy of bonus tech 6 lowers the cost of tech 13, the copy of tech 8, which the game's tech 6 lowers.

## Test 76

### File residence:
`tests/dat/recipe_tests.c`, lines 62-65

### Purpose:
Verify that a civ bonus whose tech belongs to no civ is unlocked in the civ's tech tree (Req. 8).

### Assertion expectation:
The civ's tech tree effect unlocks tech 10, once.

## Test 77

### File residence:
`tests/dat/recipe_tests.c`, lines 67-70

### Purpose:
Verify that the build's report counts the civ bonuses built in each of the three ways (Req. 9).

### Assertion expectation:
`bonuses_built` is 3: one of a civ's tech, one of a generic tech, one of the catalog's changes.

## Test 78

### File residence:
`tests/dat/recipe_tests.c`, lines 72-75

### Purpose:
Verify that the build says which civ bonus it could not build (Req. 9).

### Assertion expectation:
`bonuses_skipped` is 1 for a design with bonus 99, which the catalog lacks.

## Test 79

### File residence:
`tests/dat/recipe_tests.c`, lines 77-80

### Purpose:
Verify that Build says it skipped a civ bonus the catalog has but gives neither techs nor changes (Req. 9).

### Assertion expectation:
A civ with bonus 53, which the catalog lists with no techs and no changes, has 1 civ bonus skipped.

## Test 80

### File residence:
`tests/dat/recipe_tests.c`, lines 114-117

### Purpose:
Verify that Build counts each bonus whose multiplier it could not apply (Req. 9).

### Assertion expectation:
With bonus 50, made of a game's tech, taken twice, the report's `multipliers_skipped` is 1.

## Test 81

### File residence:
`tests/dat/recipe_tests.c`, lines 82-85

### Purpose:
Verify that a requirement tech of another civ is copied for the civ and added to the game's tech (Req. 8).

### Assertion expectation:
The tech named in the third slot of bonus tech 6's requirements, free in the game, belongs to civ 1.

## Test 82

### File residence:
`tests/dat/recipe_tests.c`, lines 87-91

### Purpose:
Verify that two techs given one requirement tech share one copy of it, as the original does for bonus 81 (Req. 8).

### Assertion expectation:
The first slot of tech 10's requirements and the third of tech 6's name the same tech.

## Test 83

### File residence:
`tests/dat/recipe_tests.c`, lines 93-96

### Purpose:
Verify that Build counts a bonus made of requirement lines as built (Req. 9).

### Assertion expectation:
The civ with bonuses 54 and 55 has 1 civ bonus built.

## Test 84

### File residence:
`tests/dat/recipe_tests.c`, lines 98-101

### Purpose:
Verify that Build says it skipped a bonus whose game tech has no free slot for a requirement (Req. 9).

### Assertion expectation:
The civ with bonus 55, whose tech 7 has all six slots of its requirements taken, has 1 civ bonus skipped.

## Test 85

### File residence:
`tests/dat/recipe_tests.c`, lines 103-112

### Purpose:
Prevent a build from adding the requirement techs of the build before it (Req. 8).

### Assertion expectation:
After a build with bonus 54, a build of the bare civ leaves the first slot of tech 10's requirements at -1.

## Test 86

### File residence:
`tests/dat/recipe_tests.c`, lines 109-112

### Purpose:
Prevent the build from giving a requirement to a tech of the game that no requirement line names (Req. 8).

### Assertion expectation:
With bonuses 54 and 55, the first slot of tech 0's requirements stays at -1.

## Test 87

### File residence:
`tests/dat/recipe_tests.c`, lines 119-132

### Purpose:
Verify that the civ's team bonus effect holds the changes of the chosen team bonus (Req. 8).

### Assertion expectation:
The last change of the effect that civ 1's team bonus id names is of attribute 5.

## Test 88

### File residence:
`tests/dat/recipe_tests.c`, lines 124-127

### Purpose:
Verify that the team bonus, which allies take too, changes the game's own techs, not the civ's copies (Req. 8).

### Assertion expectation:
The team bonus copied from effect 9 lowers the cost of tech 8, as the game's effect does, not of its copy 13.

## Test 89

### File residence:
`tests/dat/recipe_tests.c`, lines 129-132

### Purpose:
Verify that a team bonus copied from the game's effect and taken twice counts twice (Req. 8).

### Assertion expectation:
Team bonus 60 taken twice writes its effect's +1 as +2.

## Test 90

### File residence:
`tests/dat/recipe_tests.c`, lines 134-137

### Purpose:
Verify that the tech of the elite unique unit is named by the slot's string id (Req. 8).

### Assertion expectation:
Tech 13, the copy of the elite tech, has name string id 111004.

## Test 91

### File residence:
`tests/dat/recipe_tests.c`, lines 139-142

### Purpose:
Prevent a unique tech from changing the civ's unit twice when its old and new unit are both named (Req. 8).

### Assertion expectation:
The castle tech's effect has 4 changes: of 7 in the game, 2 are dropped and 1 is already made.

## Test 92

### File residence:
`tests/dat/recipe_tests.c`, lines 144-147

### Purpose:
Verify that a unique tech that enabled its old civ's unique unit enables the civ's own (Req. 8).

### Assertion expectation:
The castle tech's effect enables unit 6, once.

## Test 93

### File residence:
`tests/dat/recipe_tests.c`, lines 149-152

### Purpose:
Verify that a unique tech's upgrade of its old civ's unique unit is left out (Req. 8).

### Assertion expectation:
The castle tech's effect has no upgrade of unit 5.

## Test 94

### File residence:
`tests/dat/recipe_tests.c`, lines 154-157

### Purpose:
Verify that a unit spawned by a unique tech is the civ's elite unique unit (Req. 8).

### Assertion expectation:
The castle tech's effect spawns unit 11, once.

## Test 95

### File residence:
`tests/dat/recipe_tests.c`, lines 159-162

### Purpose:
Verify that the civ's tech tree effect lists its unlocks before its disables, as the game's do (Req. 8).

### Assertion expectation:
The last change of the tree effect is a disable.

## Test 96

### File residence:
`tests/dat/recipe_tests.c`, lines 164-167

### Purpose:
Prevent the tech tree from disabling a tech that only sets resources and has no research place (Req. 8).

### Assertion expectation:
The tree effect does not disable tech 7.

## Test 97

### File residence:
`tests/dat/recipe_tests.c`, lines 295-304

### Purpose:
Verify that the tech tree effect lists its unlocks first, then each kind of change by tech, as the game's do (Req. 8).

### Assertion expectation:
A disable of tech 30, an unlock of tech 40 and a disable of tech 20, sorted, come as the unlock, then 20, then 30.

## Test 98

### File residence:
`tests/dat/recipe_tests.c`, lines 306-312

### Purpose:
Prevent the tech tree from keeping one of two techs it disables, since a disable names its tech in `d` (Req. 8).

### Assertion expectation:
After a disable of tech 20 and one of tech 30 are added, the tree has 2 changes.

## Test 99

### File residence:
`tests/dat/recipe_tests.c`, lines 169-172

### Purpose:
Verify that the build says when it could not build the unique unit (Req. 9).

### Assertion expectation:
`unit_skipped` is true for a design whose unique unit the catalog lacks.

## Test 100

### File residence:
`tests/dat/recipe_tests.c`, lines 174-177

### Purpose:
Verify that the build says which unique techs it could not build (Req. 9).

### Assertion expectation:
`unique_techs_skipped` is 2 for a design whose two unique techs the catalog lacks.

## Test 101

### File residence:
`tests/dat/recipe_tests.c`, lines 179-182

### Purpose:
Verify that the civ's tech tree effect hides the hero until its own tech enables it (Req. 8).

### Assertion expectation:
The tree effect has one change that enables unit 6 with mode 0, off.

## Test 102

### File residence:
`tests/dat/recipe_tests.c`, lines 184-187

### Purpose:
Verify that a design without a tech tree leaves the game's techs as they are (Req. 8).

### Assertion expectation:
`tree_disabled` is 0 for the bare civ, which has no tree.

## Test 103

### File residence:
`tests/dat/recipe_tests.c`, lines 189-192

### Purpose:
Verify that a team bonus the game has no effect for is built from the catalog's changes (Req. 9).

### Assertion expectation:
`team_bonuses_built` is 1 for team bonus 61, which has one change and no effect.

## Test 104

### File residence:
`tests/dat/recipe_tests.c`, lines 209-212

### Purpose:
Verify that a team bonus made of changes and taken twice counts twice (Req. 8).

### Assertion expectation:
Team bonus 61 taken twice writes its change of +3 as +6.

## Test 105

### File residence:
`tests/dat/recipe_tests.c`, lines 214-217

### Purpose:
Verify that a civ bonus that names techs also gets its lines of changes, as bonus 356 needs (Req. 8).

### Assertion expectation:
The bare civ with bonus 56, which names tech 10 and has one line, has 1 change adding to infantry in tech 11.

## Test 106

### File residence:
`tests/dat/recipe_tests.c`, lines 219-224

### Purpose:
Verify that the lines of a bonus that names techs count once, as its techs do, whatever its multiplier (Req. 9).

### Assertion expectation:
Bonus 56, taken twice, adds 9 to infantry in tech 11, as its line gives, not 18.

## Test 107

### File residence:
`tests/dat/recipe_tests.c`, lines 194-197

### Purpose:
Verify that Build says it skipped a team bonus with neither an effect nor changes (Req. 9).

### Assertion expectation:
A civ with team bonus 62, which the catalog lists with no effect and no changes, has 1 team bonus skipped.

## Test 108

### File residence:
`tests/dat/recipe_tests.c`, lines 199-202

### Purpose:
Prevent Build from saying it built a team bonus it skipped (Req. 9).

### Assertion expectation:
A civ with team bonus 62, which has nothing to build, has 0 team bonuses built.

## Test 109

### File residence:
`tests/dat/recipe_tests.c`, lines 204-207

### Purpose:
Prevent Build from saying it skipped a team bonus it built (Req. 9).

### Assertion expectation:
The full civ, whose team bonus 60 is copied from the game's effect, has 0 team bonuses skipped.

## Test 110

### File residence:
`tests/dat/recipe_tests.c`, lines 226-229

### Purpose:
Verify that a hero with a name takes the hero's string id of the replaced civ's slot (Req. 8).

### Assertion expectation:
Unit 6 of civ 1 has name string id 111005 in the built dat file.

## Test 111

### File residence:
`tests/dat/recipe_tests.c`, lines 231-234

### Purpose:
Verify that a tech copied for the civ requires the civ's copy of a tech it required (Req. 8).

### Assertion expectation:
Tech 12, the copy of tech 6, requires tech 13, the copy of tech 8, which belonged to another civ.

## Test 112

### File residence:
`tests/dat/recipe_tests.c`, lines 236-239

### Purpose:
Verify that a tech copied for the civ keeps a requirement that every civ shares (Req. 8).

### Assertion expectation:
The second required tech of tech 12 is tech 5, as in the game.

## Test 113

### File residence:
`tests/dat/recipe_tests.c`, lines 241-244

### Purpose:
Verify that the scout chosen for the civ is written into the civ's resources (Req. 8).

### Assertion expectation:
With scout 448 chosen, resource 263 of civ 1 is 448 in the built dat file.

## Test 114

### File residence:
`tests/dat/recipe_tests.c`, lines 246-249

### Purpose:
Verify that the build says so when the hero is a unit the replaced civ does not have (Req. 9).

### Assertion expectation:
With unit 4 as the hero, which civ 1 of the small game lacks, the civ is built and `hero_skipped` is true.

## Test 115

### File residence:
`tests/dat/recipe_tests.c`, lines 251-260

### Purpose:
Verify that the dat file is read past its sounds, graphics, unit tasks and tech tree nodes (Req. 8).

### Assertion expectation:
A small dat with one sound, two graphic slots, two unit headers and four tree nodes is written back byte for byte.

## Test 116

### File residence:
`tests/dat/recipe_tests.c`, lines 262-265

### Purpose:
Verify that the tech that makes the unique unit is the replaced civ's own (Req. 8).

### Assertion expectation:
Tech 14 of the built dat file, the copy of the unit's tech, has civ 1.

## Test 117

### File residence:
`tests/dat/recipe_tests.c`, lines 267-270

### Purpose:
Verify that a built unique tech can be researched only in the age of its tier (Req. 8).

### Assertion expectation:
The first required tech of tech 15, the castle tech, is 102, the Castle Age.

## Test 118

### File residence:
`tests/dat/recipe_tests.c`, lines 272-275

### Purpose:
Verify that the build's report counts the techs its tech tree disables (Req. 9).

### Assertion expectation:
`tree_disabled` is 1 for the full civ, whose tree lacks one unit of the catalog.

## Test 119

### File residence:
`tests/dat/recipe_tests.c`, lines 277-280

### Purpose:
Prevent Build from counting the techs of a unique unit it did not build (Req. 9).

### Assertion expectation:
The bare civ, whose unique unit the catalog lacks, has 0 techs of its unit built.

## Test 120

### File residence:
`tests/look/look_tests.c`, lines 56-59

### Purpose:
Verify that a ship of the civ takes the look of the civ of its architecture (Req. 8).

### Assertion expectation:
A unit of class 22 in civ 1 has the standing graphic 100 of civ 0 after the build.

## Test 121

### File residence:
`tests/look/look_tests.c`, lines 61-64

### Purpose:
Verify that a trade cart's loaded look is taken with its architecture (Req. 8).

### Assertion expectation:
The carrying graphic of the first task of a unit of class 19 in civ 1 is 100, that of civ 0.

## Test 122

### File residence:
`tests/look/look_tests.c`, lines 66-73

### Purpose:
Verify that a monk's working look is taken with its monk skin (Req. 8).

### Assertion expectation:
The proceeding graphic of the first task of unit 125 in civ 1 is 100, that of the monk skin's civ 0.

## Test 123

### File residence:
`tests/look/look_tests.c`, lines 75-81

### Purpose:
Verify that the Castle takes the look chosen for it and not that of the architecture (Req. 8).

### Assertion expectation:
With architecture civ 0 and castle civ 2, unit 82 of civ 1 has the standing graphic 102.

## Test 124

### File residence:
`tests/look/look_tests.c`, lines 83-86

### Purpose:
Prevent a unit that is neither a building nor a ship, monk, cart or king from changing its look (Req. 8).

### Assertion expectation:
A unit of class 6 in civ 1 keeps its standing graphic 101 after the build.

## Test 125

### File residence:
`tests/look/look_tests.c`, lines 88-94

### Purpose:
Verify that the architecture chosen in the design is looked up in the catalog for its civ (Req. 8).

### Assertion expectation:
For architecture 2, which the catalog gives the look of civ 0, `look_sources_of` names civ 0.

## Test 126

### File residence:
`tests/look/look_tests.c`, lines 111-114

### Purpose:
Verify that a look line of a bonus in the design dresses its unit in the look of the civ it names (Req. 8, 9).

### Assertion expectation:
The infantry unit of the civ built, a look line naming civ 0 and its bonus chosen, has the graphics of civ 0.

## Test 127

### File residence:
`tests/look/look_tests.c`, lines 116-119

### Purpose:
Prevent a look line from dressing a unit when the design lacks its bonus (Req. 8, 9).

### Assertion expectation:
The infantry unit of the civ built, a look line naming civ 0 but its bonus not chosen, keeps the graphics of civ 1.

## Test 128

### File residence:
`tests/look/look_tests.c`, lines 168-171

### Purpose:
Verify that a look line gives its unit the frame and the place its attack is let go from, with the look (Req. 8, 9).

### Assertion expectation:
The unit of the civ built, its look copied from civ 0 whose frame delay is 7, has a frame delay of 7.

## Test 129

### File residence:
`tests/look/look_tests.c`, lines 173-176

### Purpose:
Verify that a look line gives its unit the attack graphic of the unit it copies (Req. 8, 9).

### Assertion expectation:
The unit of the civ built, its look copied from civ 0 whose attack graphic is 7, has an attack graphic of 7.

## Test 130

### File residence:
`tests/look/look_tests.c`, lines 178-181

### Purpose:
Verify that a look line gives its unit the walking graphic of the unit it copies (Req. 8, 9).

### Assertion expectation:
The unit of the civ built, its look copied from civ 0 whose walking graphic is 7, has a walking graphic of 7.

## Test 131

### File residence:
`tests/look/look_tests.c`, lines 183-186

### Purpose:
Verify that a look line gives its unit the icon of the unit it copies (Req. 8, 9).

### Assertion expectation:
The unit of the civ built, its look copied from civ 0 whose icon is 7, has an icon of 7.

## Test 132

### File residence:
`tests/bonus/scale_tests.c`, lines 52-55

### Purpose:
Verify that an amount a bonus adds grows with the times the bonus counts, as the original scales it (Req. 8).

### Assertion expectation:
+15 hit points taken 3 times is one change of +45.

## Test 133

### File residence:
`tests/bonus/scale_tests.c`, lines 57-60

### Purpose:
Verify that the factor a bonus multiplies an attribute by is raised to the times it counts (Req. 8).

### Assertion expectation:
Speed times 1.5, taken twice, is speed times 2.25.

## Test 134

### File residence:
`tests/bonus/scale_tests.c`, lines 62-65

### Purpose:
Verify that the factor a bonus multiplies a resource by is raised to the times it counts (Req. 8).

### Assertion expectation:
Food times 1.5, taken twice, is food times 2.25.

## Test 135

### File residence:
`tests/bonus/scale_tests.c`, lines 67-70

### Purpose:
Verify that a tech's cost multiplied in mode 2 is raised to the times the bonus counts (Req. 8).

### Assertion expectation:
Loom's food cost times 0.5 in mode 2, taken twice, is times 0.25.

## Test 136

### File residence:
`tests/bonus/scale_tests.c`, lines 72-75

### Purpose:
Verify that a tech's time multiplied in mode 2 is raised to the times the bonus counts (Req. 8).

### Assertion expectation:
Loom's time times 0.5 in mode 2, taken twice, is times 0.25.

## Test 137

### File residence:
`tests/bonus/scale_tests.c`, lines 77-80

### Purpose:
Verify that a tech's cost changed in a mode other than 2 is kept, as the original keeps it (Req. 8).

### Assertion expectation:
Loom's food cost -10 in mode 1, taken twice, stays -10.

## Test 138

### File residence:
`tests/bonus/scale_tests.c`, lines 82-87

### Purpose:
Verify that a bonus that spawns units spawns more as it counts more times (Req. 8).

### Assertion expectation:
3 militia spawned at the barracks, taken twice, are 6: the change's `c` is 6.

## Test 139

### File residence:
`tests/bonus/scale_tests.c`, lines 89-92

### Purpose:
Verify that a resource a bonus gives grows with the times the bonus counts (Req. 8).

### Assertion expectation:
+100 food taken twice is +200.

## Test 140

### File residence:
`tests/bonus/scale_tests.c`, lines 94-97

### Purpose:
Verify that the spawn limit, resource 234, is kept, as the original keeps it (Req. 8).

### Assertion expectation:
Resource 234 +5, taken twice, stays +5.

## Test 141

### File residence:
`tests/bonus/scale_tests.c`, lines 99-102

### Purpose:
Verify that resource 33, which names a function of the game, not an amount, is kept (Req. 8).

### Assertion expectation:
Resource 33 set to 7, taken twice, stays 7.

## Test 142

### File residence:
`tests/bonus/scale_tests.c`, lines 104-107

### Purpose:
Verify that a change that sets an attribute is kept, since setting it twice sets it once (Req. 8).

### Assertion expectation:
Hit points set to 100, taken twice, stay 100.

## Test 143

### File residence:
`tests/bonus/scale_tests.c`, lines 109-113

### Purpose:
Verify that armor added past 255, the most that one change holds, is split into two changes (Req. 8).

### Assertion expectation:
Pierce armor +200 taken twice is +255 and +145: the second change's `d` is 913.

## Test 144

### File residence:
`tests/bonus/scale_tests.c`, lines 115-119

### Purpose:
Verify that armor taken away stays taken away as the bonus counts more times (Req. 8).

### Assertion expectation:
Pierce armor -2 taken twice is -4: the change's `d` is -772.

## Test 145

### File residence:
`tests/bonus/scale_tests.c`, lines 121-125

### Purpose:
Verify that an attack multiplied past 255%, the most that one change holds, is split into two (Req. 8).

### Assertion expectation:
Attack times 150% taken 3 times is 255% and 132%: the second change's `d` is 900.

## Test 146

### File residence:
`tests/bonus/scale_tests.c`, lines 127-131

### Purpose:
Verify that a percent halfway between two is rounded to the even one, as the original rounds it (Req. 8).

### Assertion expectation:
Attack times 50% taken 3 times is 12.5%, written as 12%: the change's `d` is 780.

## Test 147

### File residence:
`tests/bonus/scale_tests.c`, lines 133-137

### Purpose:
Verify that a percent past a half is rounded up, as the original rounds it (Req. 8).

### Assertion expectation:
Attack times 109% taken twice is 118.81%, written as 119%: the change's `d` is 887.

## Test 148

### File residence:
`tests/bonus/scale_tests.c`, lines 139-143

### Purpose:
Verify that a percent below a half is rounded down, odd or even, as the original rounds it (Req. 8).

### Assertion expectation:
Attack times 111% taken twice is 123.21%, written as 123%: the change's `d` is 891.

## Test 149

### File residence:
`tests/bonus/scale_tests.c`, lines 145-150

### Purpose:
Verify that armor that grows to exactly 255, the most one change holds, stays one change (Req. 8).

### Assertion expectation:
Pierce armor +85 taken 3 times is one change of +255.

## Test 150

### File residence:
`tests/mod/forge_tests.c`, lines 134-137

### Purpose:
Verify that a built mod has the folder `<name>-data` in the mods folder (Req. 2, 7).

### Assertion expectation:
For the civ "Testonia" and a small game in `build/test_game`, the result's data mod ends in `testonia-data`.

## Test 151

### File residence:
`tests/mod/forge_tests.c`, lines 139-142

### Purpose:
Verify that a built mod has the folder `<name>-ui` in the mods folder (Req. 2, 7).

### Assertion expectation:
For the civ "Testonia", the result's UI mod ends in `testonia-ui`.

## Test 152

### File residence:
`tests/mod/forge_tests.c`, lines 144-147

### Purpose:
Verify that the data mod's `info.json` carries the civ's name as its title (Req. 7).

### Assertion expectation:
`build/test_mods/testonia-data/info.json` holds "Testonia".

## Test 153

### File residence:
`tests/mod/forge_tests.c`, lines 149-152

### Purpose:
Verify that the UI mod's `info.json` is told apart from the data mod's by its title (Req. 7).

### Assertion expectation:
`build/test_mods/testonia-ui/info.json` holds "Testonia (UI)".

## Test 154

### File residence:
`tests/mod/forge_tests.c`, lines 154-165

### Purpose:
Verify that the data mod holds the game's dat file with the civ built in, packed as the game reads it (Req. 8).

### Assertion expectation:
The mod's `empires2_x2_p1.dat` unpacks and has 17 techs, the game's 11 and the civ's 6.

## Test 155

### File residence:
`tests/mod/forge_tests.c`, lines 167-170

### Purpose:
Verify that the data mod's `civilizations.json` names the civ's unique unit for the replaced civ (Req. 8).

### Assertion expectation:
The mod's list of civs holds `"unique_unit_id":6,` where the game's holds 5.

## Test 156

### File residence:
`tests/mod/forge_tests.c`, lines 172-175

### Purpose:
Verify that the data mod holds the tech tree file under the replaced civ's name (Req. 8).

### Assertion expectation:
`CivTechTrees/BRITONS.json` of the data mod holds `civ_techs_units`.

## Test 157

### File residence:
`tests/mod/forge_tests.c`, lines 177-180

### Purpose:
Verify that the UI mod writes the civ's name under the replaced civ's name id (Req. 8).

### Assertion expectation:
The UI mod's strings of `en` hold `10271 "Testonia"`.

## Test 158

### File residence:
`tests/mod/forge_tests.c`, lines 182-185

### Purpose:
Verify that the UI mod is written for every language of the game, to the last (Req. 7).

### Assertion expectation:
The UI mod's strings of `zh`, the fifteenth language, hold `10271 "Testonia"`.

## Test 159

### File residence:
`tests/mod/forge_tests.c`, lines 187-190

### Purpose:
Verify that Build says so when the game's list of civs cannot be read (Req. 9).

### Assertion expectation:
For the game folder `build/no_game` the build fails with a problem that names the "list of civs".

## Test 160

### File residence:
`tests/mod/forge_tests.c`, lines 192-195

### Purpose:
Verify that Build says so when the game's data file cannot be unpacked (Req. 9).

### Assertion expectation:
With 19 bytes of text as the dat file, the build fails with "read or unpacked" in its problem.

## Test 161

### File residence:
`tests/mod/forge_tests.c`, lines 197-200

### Purpose:
Verify that Build says so when the replaced civ's tech tree file is missing (Req. 9).

### Assertion expectation:
With a list of civs that names the tree `NOBODY`, the build fails with "tech tree" in its problem.

## Test 162

### File residence:
`tests/mod/forge_tests.c`, lines 202-205

### Purpose:
Verify that a civ is built without what the catalog lacks, and that the report says so (Req. 9).

### Assertion expectation:
The bare civ is built, and the result's report has `unit_skipped`.

## Test 163

### File residence:
`tests/mod/strings_tests.c`, lines 35-38

### Purpose:
Verify that a text of several lines is written to the game's strings on one line (Req. 8).

### Assertion expectation:
The tagline "North", a line break, "South" is written `"North\nSouth civilization`.

## Test 164

### File residence:
`tests/mod/strings_tests.c`, lines 40-43

### Purpose:
Prevent a line break typed in Windows from being written as two line breaks (Req. 8).

### Assertion expectation:
The tagline "North", a carriage return and a line break, "South" is written with one `\n`.

## Test 165

### File residence:
`tests/mod/strings_tests.c`, lines 45-48

### Purpose:
Prevent a quote in a text from ending its line of the game's strings early (Req. 8).

### Assertion expectation:
The tagline `The "best"` is written `"The \"best\" civilization`.

## Test 166

### File residence:
`tests/mod/strings_tests.c`, lines 50-53

### Purpose:
Prevent a quote that the user escaped from being escaped twice (Req. 8).

### Assertion expectation:
The tagline `The \"best` is written `"The \"best civilization`.

## Test 167

### File residence:
`tests/mod/strings_tests.c`, lines 55-58

### Purpose:
Verify that a unique tech's label holds its description in brackets, without its full stop (Req. 8).

### Assertion expectation:
The tech "Bulwark", "Walls are stronger.", is written `"Research Bulwark (Walls are stronger)"`.

## Test 168

### File residence:
`tests/mod/strings_tests.c`, lines 60-63

### Purpose:
Verify that a unique tech without a description gets a label of its name alone (Req. 8).

### Assertion expectation:
The imperial tech named "Zeal" is written `"Research Zeal"`.

## Test 169

### File residence:
`tests/mod/strings_tests.c`, lines 65-69

### Purpose:
Verify that the unique unit's tooltip holds the description the user gave it (Req. 8).

### Assertion expectation:
The unit "Tester", "It tests.", is written `"Create <b>Tester<b> (<cost>)\nIt tests."`.

## Test 170

### File residence:
`tests/mod/strings_tests.c`, lines 71-75

### Purpose:
Verify that the first loading tip of the civ is its first civ bonus (Req. 8).

### Assertion expectation:
`IDS_CIVTIPS_1_0` is "Civilization bonus: Infantry is stronger.", with a full stop added.

## Test 171

### File residence:
`tests/mod/strings_tests.c`, lines 77-80

### Purpose:
Verify that a civ without civ bonuses gets its team bonus as its first tip (Req. 8).

### Assertion expectation:
For the bare civ, `IDS_CIVTIPS_1_0` is "Team bonus: Allies are faster."

## Test 172

### File residence:
`tests/mod/strings_tests.c`, lines 82-86

### Purpose:
Verify that the third tip names the castle tech, its place and what it does (Req. 8).

### Assertion expectation:
`IDS_CIVTIPS_1_2` is "Research Bulwark in your Castle: Walls are stronger."

## Test 173

### File residence:
`tests/mod/strings_tests.c`, lines 88-91

### Purpose:
Verify that the second tip names the civ's unique unit (Req. 8).

### Assertion expectation:
`IDS_CIVTIPS_1_1` is "Your unique unit is the Tester."

## Test 174

### File residence:
`tests/mod/strings_tests.c`, lines 93-97

### Purpose:
Verify that a tip, which the game shows on one line, takes a line break of the civ's texts as a space (Req. 8).

### Assertion expectation:
With the unit named "Test", a line break and "er", `IDS_CIVTIPS_1_1` is "Your unique unit is the Test er."

## Test 175

### File residence:
`tests/places/places_tests.c`, lines 51-55

### Purpose:
Verify that the game is found when it is installed in Steam's own folder (Req. 2).

### Assertion expectation:
With a dat file under `build/test_steam`, the game folder found ends in `test_steam\steamapps\common\AoE2DE`.

## Test 176

### File residence:
`tests/places/places_tests.c`, lines 57-60

### Purpose:
Verify that the game is found in another library that Steam's list names (Req. 2).

### Assertion expectation:
With a list that names `build\\test_library`, the game folder found is in `build\test_library`.

## Test 177

### File residence:
`tests/places/places_tests.c`, lines 62-66

### Purpose:
Prevent a folder without the game's data file from being taken for the game (Req. 2).

### Assertion expectation:
With a list that names only `build\\nowhere`, no game folder is found.

## Test 178

### File residence:
`tests/places/places_tests.c`, lines 68-73

### Purpose:
Verify that the local mods folder of the user's profile of the game is found (Req. 2).

### Assertion expectation:
Under the home `build/test_home` with profile 7656, the folder found ends in `7656\mods\local`.

## Test 179

### File residence:
`tests/places/places_tests.c`, lines 75-78

### Purpose:
Prevent a mods folder from being named where the game has no profile (Req. 2).

### Assertion expectation:
Under the home `build/no_home`, which does not exist, no mods folder is found.

## Test 180

### File residence:
`tests/core/read_tests.c`, lines 99-104

### Purpose:
Verify that a number of the catalog or a civ file is read with its sign (Req. 1).

### Assertion expectation:
`text_to_number` reads "-12" as valid and -12.

## Test 181

### File residence:
`tests/core/read_tests.c`, lines 106-109

### Purpose:
Prevent a text with a letter among its digits from being read as a number (Req. 1).

### Assertion expectation:
`text_to_number` says that "1x" is not valid.

## Test 182

### File residence:
`tests/core/read_tests.c`, lines 111-114

### Purpose:
Prevent a number too large to hold from being read as another number (Req. 1).

### Assertion expectation:
`text_to_number` says that "2147483648", one more than the largest `long`, is not valid.

## Test 183

### File residence:
`tests/core/read_tests.c`, lines 116-121

### Purpose:
Verify that the largest number a `long` holds is still read, now that a larger one is refused (Req. 1).

### Assertion expectation:
`text_to_number` reads "2147483647" as valid and equal to `LONG_MAX`.

## Test 184

### File residence:
`tests/core/read_tests.c`, lines 123-126

### Purpose:
Verify that a path or text with an accented letter reaches Windows whole (Req. 1, 2).

### Assertion expectation:
`text_widen` makes the two bytes C3 A9 the one wide letter 00E9.

## Test 185

### File residence:
`tests/core/read_tests.c`, lines 128-131

### Purpose:
Verify that a letter of three bytes in UTF-8 reaches Windows whole (Req. 1, 2).

### Assertion expectation:
`text_widen` makes the three bytes E2 82 AC the one wide letter 20AC.

## Test 186

### File residence:
`tests/core/read_tests.c`, lines 133-136

### Purpose:
Verify that a letter of four bytes in UTF-8 becomes a pair of wide letters (Req. 1, 2).

### Assertion expectation:
`text_widen` makes F0 9F 98 80 the two wide letters D83D DE00.

## Test 187

### File residence:
`tests/core/read_tests.c`, lines 138-141

### Purpose:
Verify that U+10000, the first letter that needs a pair of wide letters, is widened to a pair (Req. 1, 2).

### Assertion expectation:
`text_widen` makes F0 90 80 80 the two wide letters D800 DC00.

## Test 188

### File residence:
`tests/core/read_tests.c`, lines 143-146

### Purpose:
Prevent a text that is not UTF-8 from becoming a wrong path (Req. 1, 2).

### Assertion expectation:
`text_widen` fails on the byte FF.

## Test 189

### File residence:
`tests/core/read_tests.c`, lines 148-151

### Purpose:
Prevent a text that begins with the middle of a letter from being widened (Req. 1, 2).

### Assertion expectation:
`text_widen` fails on the byte 80.

## Test 190

### File residence:
`tests/core/read_tests.c`, lines 153-156

### Purpose:
Prevent a letter whose second byte is missing from being widened (Req. 1, 2).

### Assertion expectation:
`text_widen` fails on C3 followed by "A".

## Test 191

### File residence:
`tests/core/read_tests.c`, lines 158-161

### Purpose:
Prevent a widened text from being cut where its place ends (Req. 1, 2).

### Assertion expectation:
`text_widen` fails on "abc" for a place of 2 wide letters.

## Test 192

### File residence:
`tests/core/read_tests.c`, lines 163-169

### Purpose:
Verify that a field of the catalog that holds only a dash is read as an empty text (Req. 8).

### Assertion expectation:
`tsv_text` succeeds on "-" and leaves an empty text.

## Test 193

### File residence:
`tests/core/read_tests.c`, lines 171-177

### Purpose:
Prevent a field beyond the end of a catalog line from being read (Req. 8).

### Assertion expectation:
`tsv_text` fails for field 3 of a line of one field and leaves an empty text.

## Test 194

### File residence:
`tests/core/read_tests.c`, lines 191-194

### Purpose:
Verify that a decimal of the catalog is read in hundredths, without rounding (Req. 8).

### Assertion expectation:
`tsv_hundredths` reads "1.257" as 125.

## Test 195

### File residence:
`tests/core/read_tests.c`, lines 196-199

### Purpose:
Verify that a decimal with one digit after the point is read in hundredths (Req. 8).

### Assertion expectation:
`tsv_hundredths` reads "1.5" as 150.

## Test 196

### File residence:
`tests/core/read_tests.c`, lines 201-204

### Purpose:
Prevent a field with two points from being read as a number (Req. 8).

### Assertion expectation:
`tsv_hundredths` gives `TSV_MISSING` for "1.2.3".

## Test 197

### File residence:
`tests/core/read_tests.c`, lines 206-209

### Purpose:
Verify that a change of the catalog with a negative amount is read with its sign (Req. 9).

### Assertion expectation:
`tsv_decimal` reads "-1.5" as -1.5.

## Test 198

### File residence:
`tests/core/read_tests.c`, lines 211-214

### Purpose:
Prevent a field with a letter among its digits from being read as a decimal (Req. 8).

### Assertion expectation:
`tsv_decimal` fails on "1x".

## Test 199

### File residence:
`tests/core/read_tests.c`, lines 216-219

### Purpose:
Prevent a field with a sign and no digit from being read as zero (Req. 8).

### Assertion expectation:
`tsv_decimal` fails on "-".

## Test 200

### File residence:
`tests/core/read_tests.c`, lines 221-224

### Purpose:
Verify that a civ file's text with an escaped letter is opened as that letter (Req. 1).

### Assertion expectation:
The string `é` of a civ file is copied as the bytes C3 A9.

## Test 201

### File residence:
`tests/core/read_tests.c`, lines 226-229

### Purpose:
Verify that an escaped letter written with capital hex digits is opened (Req. 1).

### Assertion expectation:
The string `€` is copied as the bytes E2 82 AC.

## Test 202

### File residence:
`tests/core/read_tests.c`, lines 231-234

### Purpose:
Verify that an escaped pair of surrogates is opened as one letter (Req. 1).

### Assertion expectation:
The string `😀` is copied as the bytes F0 9F 98 80.

## Test 203

### File residence:
`tests/core/read_tests.c`, lines 236-239

### Purpose:
Prevent a broken escape of a civ file from being opened as some letter (Req. 1).

### Assertion expectation:
The string `\u00zz` is not copied.

## Test 204

### File residence:
`tests/core/read_tests.c`, lines 241-244

### Purpose:
Prevent half a pair of surrogates from being opened as a letter (Req. 1).

### Assertion expectation:
The string `\ud83d` followed by plain letters is not copied.

## Test 205

### File residence:
`tests/core/read_tests.c`, lines 246-249

### Purpose:
Prevent a high surrogate followed by another escape from being opened (Req. 1).

### Assertion expectation:
The string `\ud83dA` is not copied.

## Test 206

### File residence:
`tests/core/read_tests.c`, lines 251-254

### Purpose:
Verify that a description of several lines is opened with its line breaks (Req. 1).

### Assertion expectation:
The string `x\ny` is copied as "x", a new line and "y".

## Test 207

### File residence:
`tests/core/read_tests.c`, lines 256-259

### Purpose:
Verify that an escaped tab of a civ file is opened as a tab (Req. 1).

### Assertion expectation:
The string `x\ty` is copied as "x", a tab and "y".

## Test 208

### File residence:
`tests/core/read_tests.c`, lines 261-264

### Purpose:
Verify that an escaped carriage return of a civ file is opened as one (Req. 1).

### Assertion expectation:
The string `x\ry` is copied as "x", a carriage return and "y".

## Test 209

### File residence:
`tests/core/read_tests.c`, lines 266-269

### Purpose:
Verify that a quote inside a text of a civ file is opened as a quote (Req. 1).

### Assertion expectation:
The string `x\"y` is copied as "x", a quote and "y".

## Test 210

### File residence:
`tests/core/read_tests.c`, lines 271-274

### Purpose:
Prevent a text of a civ file from being cut where its place ends (Req. 1).

### Assertion expectation:
The string "abc" is not copied into a place of 2 bytes.

## Test 211

### File residence:
`tests/core/read_tests.c`, lines 276-279

### Purpose:
Prevent a file whose outermost value is not an object from being opened as a civ file (Req. 1).

### Assertion expectation:
`json_parse` fails on `[1]`, after reading its 2 tokens.

## Test 212

### File residence:
`tests/core/read_tests.c`, lines 281-284

### Purpose:
Prevent a civ file that ends before its object closes from being opened (Req. 1).

### Assertion expectation:
`json_parse` fails on `{"a":1`, after reading its 3 tokens.

## Test 213

### File residence:
`tests/core/read_tests.c`, lines 286-295

### Purpose:
Verify that the game's files are patched rightly whatever the order of the edits (Req. 8).

### Assertion expectation:
`{"a":1,"b":2}` with "b" set to 20 and then "a" to 10 is written `{"a":10,"b":20}`.

## Test 214

### File residence:
`tests/core/read_tests.c`, lines 297-300

### Purpose:
Verify that an escape is read with each small hex digit, the first and the last among them (Req. 1).

### Assertion expectation:
`"\u00af"` reads as the two bytes C2 AF.

## Test 215

### File residence:
`tests/core/read_tests.c`, lines 302-305

### Purpose:
Verify that an escape is read with the last capital hex digit (Req. 1).

### Assertion expectation:
`"\u00AF"` reads as the two bytes C2 AF.

## Test 216

### File residence:
`tests/core/read_tests.c`, lines 307-310

### Purpose:
Verify that a letter is written in two bytes from U+0080 on (Req. 1).

### Assertion expectation:
`"\u0080"` reads as the bytes C2 80.

## Test 217

### File residence:
`tests/core/read_tests.c`, lines 312-315

### Purpose:
Verify that a letter is written in three bytes from U+0800 on (Req. 1).

### Assertion expectation:
`"\u0800"` reads as the bytes E0 A0 80.

## Test 218

### File residence:
`tests/core/read_tests.c`, lines 317-320

### Purpose:
Verify that the first pair of surrogates is read as one letter of four bytes (Req. 1).

### Assertion expectation:
`"\ud800\udc00"` reads as the bytes F0 90 80 80.

## Test 219

### File residence:
`tests/core/read_tests.c`, lines 322-325

### Purpose:
Verify that the last pair of surrogates is read as one letter (Req. 1).

### Assertion expectation:
`"\udbff\udfff"` reads as the bytes F4 8F BF BF.

## Test 220

### File residence:
`tests/pack/dynamic_tests.c`, lines 47-58

### Purpose:
Verify that a dat file packed by another program, with repeated code lengths, is unpacked (Req. 8).

### Assertion expectation:
236 bytes packed by the deflater of Windows unpack to the 532 letters they were made from.

## Test 221

### File residence:
`tests/core/text_tests.c`, lines 10-13

### Purpose:
Prevent a name from being taken for a longer name that starts with it (Req. 1).

### Assertion expectation:
`text_equal` of "civ" and "civics" is false.

## Test 222

### File residence:
`tests/core/text_tests.c`, lines 15-18

### Purpose:
Prevent a name from being taken for the start of itself (Req. 1).

### Assertion expectation:
`text_equal` of "civics" and "civ" is false.

## Test 223

### File residence:
`tests/core/text_tests.c`, lines 20-23

### Purpose:
Verify that a text as long as its place allows is kept whole (Req. 1).

### Assertion expectation:
"abc" is copied into a place of 4 bytes and reads back as "abc".

## Test 224

### File residence:
`tests/core/text_tests.c`, lines 25-28

### Purpose:
Prevent a text from being written past the end of its place (Rule 26).

### Assertion expectation:
`text_copy` of "abcd" into a place of 4 bytes gives false.

## Test 225

### File residence:
`tests/core/text_tests.c`, lines 30-34

### Purpose:
Prevent an appended text from being written past the end of its place (Rule 26).

### Assertion expectation:
`text_append` of "cd" to "ab" in a place of 4 bytes gives false.

## Test 226

### File residence:
`tests/core/text_tests.c`, lines 36-41

### Purpose:
Verify that a negative number is written as one, such as a missing id of -1 (Req. 1).

### Assertion expectation:
`text_append_number` of -12 gives "-12".

## Test 227

### File residence:
`tests/core/text_tests.c`, lines 43-46

### Purpose:
Verify that the filters find a row whatever the case of its letters, Z among them (Req. 1).

### Assertion expectation:
`text_contains_folded` of "Zulu" and "z" is true.

## Test 228

### File residence:
`tests/core/text_tests.c`, lines 48-51

### Purpose:
Prevent a filter from keeping rows that do not hold what was typed (Req. 1).

### Assertion expectation:
`text_contains_folded` of "civ" and "x" is false.

## Test 229

### File residence:
`tests/core/writer_tests.c`, lines 18-24

### Purpose:
Verify that a civ file is JSON that the original can read: members are split by commas (Req. 1).

### Assertion expectation:
An object of `a` 1 and `b` 2 is written as `{`, `  "a": 1,`, `  "b": 2` and `}` on four lines.

## Test 230

### File residence:
`tests/core/writer_tests.c`, lines 26-34

### Purpose:
Prevent a comma before the first member of an object inside another (Req. 1).

### Assertion expectation:
An object `in` after `a` is written as `"in": {`, its member `b` on a line of its own, and `}`.

## Test 231

### File residence:
`tests/core/writer_tests.c`, lines 36-40

### Purpose:
Prevent an unfinished text from being saved as a civ file (Req. 1).

### Assertion expectation:
`json_writer_succeeded` is false while an object is not ended.

## Test 232

### File residence:
`tests/core/writer_tests.c`, lines 42-57

### Purpose:
Prevent a civ file from being cut short without a word (Req. 1).

### Assertion expectation:
After 133 texts of 1000 letters, more than the writer's 131072 bytes, `json_writer_succeeded` is false.

## Test 233

### File residence:
`tests/data/lines_tests.c`, lines 32-35

### Purpose:
Verify that a line of a catalog file is split into its fields (Req. 1).

### Assertion expectation:
The line `7`, `word`, `-`, `3,5,8`, `1,63`, split by tabs, has 5 fields.

## Test 234

### File residence:
`tests/data/lines_tests.c`, lines 37-40

### Purpose:
Verify that an id of a catalog file is read as a number (Req. 1).

### Assertion expectation:
The first field of that line reads as 7.

## Test 235

### File residence:
`tests/data/lines_tests.c`, lines 42-47

### Purpose:
Verify that a name of a catalog file is read as its text (Req. 1).

### Assertion expectation:
The second field of that line reads as "word".

## Test 236

### File residence:
`tests/data/lines_tests.c`, lines 49-57

### Purpose:
Verify that a list of techs or effects of a catalog file keeps its order (Req. 1).

### Assertion expectation:
The field `3,5,8` reads as 3 numbers, the first 3 and the last 8.

## Test 237

### File residence:
`tests/data/lines_tests.c`, lines 59-62

### Purpose:
Verify that the civs that have a node are read as a set, slot 63 among them (Req. 1).

### Assertion expectation:
The field `1,63` reads as bits 1 and 63.

## Test 238

### File residence:
`tests/data/lines_tests.c`, lines 64-69

### Purpose:
Verify that a file with the line ends of Windows is read line by line (Req. 1).

### Assertion expectation:
After a line ended by CR LF, the next line's first field reads as "second".

## Test 239

### File residence:
`tests/data/lines_tests.c`, lines 71-74

### Purpose:
Prevent a line from being read past the end of a file (Rule 26).

### Assertion expectation:
In a file of two lines, the third `tsv_next` gives false.

## Test 240

### File residence:
`tests/data/lines_tests.c`, lines 76-79

### Purpose:
Verify that a missing catalog file is told, not read as empty (Req. 1).

### Assertion expectation:
`tsv_open` of `build\no_such_lines.txt` gives false.

## Test 241

### File residence:
`tests/data/catalog_tests.c`, lines 22-25

### Purpose:
Verify that every line of `bonuses.txt` becomes a bonus of the catalog (Req. 1).

### Assertion expectation:
After `catalog_load` of `data`, the catalog holds 394 bonuses.

## Test 242

### File residence:
`tests/data/catalog_tests.c`, lines 27-30

### Purpose:
Verify that every line of `team_bonuses.txt` becomes a team bonus (Req. 1).

### Assertion expectation:
The catalog holds 80 team bonuses.

## Test 243

### File residence:
`tests/data/catalog_tests.c`, lines 32-35

### Purpose:
Verify that every line of `units.txt` becomes a unit (Req. 1).

### Assertion expectation:
The catalog holds 146 units.

## Test 244

### File residence:
`tests/data/catalog_tests.c`, lines 37-40

### Purpose:
Verify that every line of `techs.txt` becomes a unique tech (Req. 1).

### Assertion expectation:
The catalog holds 120 techs.

## Test 245

### File residence:
`tests/data/catalog_tests.c`, lines 42-45

### Purpose:
Verify that every line of `tree.txt` becomes a node (Req. 1).

### Assertion expectation:
The catalog holds 205 nodes.

## Test 246

### File residence:
`tests/data/catalog_tests.c`, lines 47-50

### Purpose:
Verify that the catalog reads every requirement line of `bonus_effects.txt` (Req. 8).

### Assertion expectation:
The catalog holds 15 requirement lines.

## Test 247

### File residence:
`tests/data/catalog_tests.c`, lines 52-57

### Purpose:
Verify that a requirement line is read whole: its bonus, the game's tech and the tech it requires (Req. 8).

### Assertion expectation:
The fourth requirement line is bonus 103, tech 209, the Cavalier, and tech 768.

## Test 248

### File residence:
`tests/data/catalog_tests.c`, lines 59-65

### Purpose:
Verify that the catalog reads a train line of its file as a unit patch in the train location (Req. 8).

### Assertion expectation:
Unit patch 173 is placed in the train location: bonus 222, unit 705, place 2, value 68.

## Test 249

### File residence:
`tests/data/catalog_tests.c`, lines 67-73

### Purpose:
Verify that the catalog reads a cost line of its file as a unit patch in the costs (Req. 8).

### Assertion expectation:
Unit patch 178 is placed in the costs: bonus 460, unit 82, place 2, value 300.

## Test 250

### File residence:
`tests/data/catalog_tests.c`, lines 75-80

### Purpose:
Verify that the catalog reads a look line of its file: its bonus, its unit and the civ of the look (Req. 8).

### Assertion expectation:
The first look line is of bonus 457, unit 42, civ 47.

## Test 251

### File residence:
`tests/data/catalog_tests.c`, lines 82-85

### Purpose:
Verify that the catalog reads every look line of `bonus_effects.txt`, once however often it loads (Req. 8).

### Assertion expectation:
The catalog holds 2 look lines.

## Test 252

### File residence:
`tests/data/catalog_tests.c`, lines 87-90

### Purpose:
Verify that a unit's name is read from the third field of its line (Req. 1).

### Assertion expectation:
The first unit of the catalog is named "Longbowman".

## Test 253

### File residence:
`tests/data/catalog_tests.c`, lines 92-95

### Purpose:
Verify that a unit's speed, a decimal in the file, is kept exactly (Req. 1).

### Assertion expectation:
The speed of the first unit, 0.96 in `units.txt`, is 96.

## Test 254

### File residence:
`tests/data/catalog_tests.c`, lines 97-100

### Purpose:
Verify that the letter V of a line of `units.txt` makes a unit of the game, not a hero (Req. 1).

### Assertion expectation:
The role of the first unit of the catalog is `UNIT_ROLE_VANILLA`.

## Test 255

### File residence:
`tests/data/catalog_tests.c`, lines 102-105

### Purpose:
Verify that a missing `data` folder is told, so that the window can say so (Req. 1).

### Assertion expectation:
`catalog_load` of `build\no_data` gives false.

## Test 256

### File residence:
`tests/model/present_tests.c`, lines 43-46

### Purpose:
Verify that the command a pick in the list of units gives changes the design (Req. 5).

### Assertion expectation:
After the command to pick unit 8, the design's unique unit is 8.

## Test 257

### File residence:
`tests/model/present_tests.c`, lines 48-51

### Purpose:
Verify that the command to pick a unique tech changes the design (Req. 5).

### Assertion expectation:
After the command to pick imperial tech 12, the design's imperial tech is 12.

## Test 258

### File residence:
`tests/model/present_tests.c`, lines 53-56

### Purpose:
Verify that the command to pick a hero changes the design (Req. 5).

### Assertion expectation:
After the command to pick hero 6, the design's hero is unit 6.

## Test 259

### File residence:
`tests/model/present_tests.c`, lines 58-61

### Purpose:
Verify that the command to tick a civ bonus changes the design (Req. 5, 6).

### Assertion expectation:
After the command to tick civ bonus 5, the design has it.

## Test 260

### File residence:
`tests/model/present_tests.c`, lines 63-66

### Purpose:
Verify that the command to tick a team bonus changes the design (Req. 5).

### Assertion expectation:
After the command to tick team bonus 5, the design has it.

## Test 261

### File residence:
`tests/model/present_tests.c`, lines 68-71

### Purpose:
Verify that the command to tick a node of the tech tree changes the design (Req. 5).

### Assertion expectation:
After the command to tick building 12, the design's tree has it.

## Test 262

### File residence:
`tests/model/present_tests.c`, lines 73-76

### Purpose:
Verify that the command to set a look changes the design (Req. 5).

### Assertion expectation:
After the command to set the wonder to 12, the design's wonder is 12.

## Test 263

### File residence:
`tests/model/present_tests.c`, lines 78-84

### Purpose:
Prevent a command without a kind from changing the design (Req. 5).

### Assertion expectation:
`command_apply` refuses `command_nothing()`, and the design still has no hero.

## Test 264

### File residence:
`tests/model/present_tests.c`, lines 86-92

### Purpose:
Verify that ticking a civ bonus again takes it out and keeps the others (Req. 6).

### Assertion expectation:
With bonuses 5 and 9 ticked and 5 ticked again, the design lacks 5 and has 9.

## Test 265

### File residence:
`tests/model/present_tests.c`, lines 94-99

### Purpose:
Verify that ticking the team bonus again takes it out (Req. 8).

### Assertion expectation:
With team bonus 5 ticked twice, the design has no team bonus 5.

## Test 266

### File residence:
`tests/model/present_tests.c`, lines 101-104

### Purpose:
Verify that a row of the tech tree list stands for its node of the catalog (Req. 8).

### Assertion expectation:
The first row of the tree list gives the command to tick unit 9.

## Test 267

### File residence:
`tests/model/present_tests.c`, lines 106-109

### Purpose:
Verify that a row of the civ bonus list stands for its bonus of the catalog (Req. 6).

### Assertion expectation:
The second row of the bonus list gives the command to tick civ bonus 51.

## Test 268

### File residence:
`tests/model/present_tests.c`, lines 111-114

### Purpose:
Verify that a row of the team bonus list stands for its bonus of the catalog (Req. 8).

### Assertion expectation:
The second row of the team bonus list gives the command to tick team bonus 61.

## Test 269

### File residence:
`tests/model/present_tests.c`, lines 116-120

### Purpose:
Verify that a row of a choice of looks stands for its entry of the catalog (Req. 8).

### Assertion expectation:
The second entry, shown for Architecture, gives the command to set the architecture to 2.

## Test 270

### File residence:
`tests/model/present_tests.c`, lines 122-125

### Purpose:
Verify that a row of the list of units stands for its unit of the catalog (Req. 8).

### Assertion expectation:
The second row of the unit list gives the command to pick the game's unit 8.

## Test 271

### File residence:
`tests/model/present_tests.c`, lines 127-130

### Purpose:
Verify that a row of the list of heroes stands for its hero of the catalog (Req. 8).

### Assertion expectation:
The row of the catalog's third unit, a hero, gives the command to pick hero 6.

## Test 272

### File residence:
`tests/model/present_tests.c`, lines 132-135

### Purpose:
Verify that the row "No hero" takes the hero out of the design (Req. 8).

### Assertion expectation:
The row without an item gives the command to pick hero -1, no choice.

## Test 273

### File residence:
`tests/model/present_tests.c`, lines 137-140

### Purpose:
Verify that a row of the castle techs stands for its tech, of the castle tier (Req. 8).

### Assertion expectation:
The first row gives the command to pick tech 0 for the castle tier.

## Test 274

### File residence:
`tests/model/present_tests.c`, lines 142-145

### Purpose:
Verify that a row of the imperial techs stands for its tech, of the imperial tier (Req. 8).

### Assertion expectation:
The second row gives the command to pick tech 0 for the imperial tier.

## Test 275

### File residence:
`tests/model/present_tests.c`, lines 147-152

### Purpose:
Verify that a row of the castle techs shows the name of its tech, not of a unit (Req. 8).

### Assertion expectation:
The first row of the castle techs reads "Bulwark", the name of the catalog's first tech.

## Test 276

### File residence:
`tests/model/present_tests.c`, lines 154-157

### Purpose:
Verify that the review names the unique unit by its own name and its base unit (Req. 3, WCAG 3.3.4).

### Assertion expectation:
The summary holds "Unique unit: Tester, from Own".

## Test 277

### File residence:
`tests/model/present_tests.c`, lines 159-162

### Purpose:
Verify that the review lists each civ bonus by its description (Req. 3, WCAG 3.3.4).

### Assertion expectation:
The summary holds "- Infantry is stronger".

## Test 278

### File residence:
`tests/model/present_tests.c`, lines 164-167

### Purpose:
Verify that the review names a civ bonus that the catalog lacks, by its number (Req. 9).

### Assertion expectation:
The summary holds "- bonus #99".

## Test 279

### File residence:
`tests/model/present_tests.c`, lines 169-172

### Purpose:
Verify that the review names a unique tech without a name of its own by the catalog's (Req. 3, WCAG 3.3.4).

### Assertion expectation:
The summary holds "Castle tech: Bulwark".

## Test 280

### File residence:
`tests/model/present_tests.c`, lines 174-177

### Purpose:
Verify that the review says so when the picked unit is not in the catalog (Req. 9).

### Assertion expectation:
For the bare civ the summary holds "Unique unit: an unknown unit".

## Test 281

### File residence:
`tests/model/present_tests.c`, lines 179-182

### Purpose:
Verify that the review says so when a picked tech is not in the catalog (Req. 9).

### Assertion expectation:
For the bare civ the summary holds "Castle tech: an unknown tech".

## Test 282

### File residence:
`tests/model/present_tests.c`, lines 197-200

### Purpose:
Verify that Build says how many civ bonuses it built and that others were left out (Req. 9).

### Assertion expectation:
A report of 3 built and 1 skipped reads "Civ bonuses built: 3 of 4; the others are not built yet."

## Test 283

### File residence:
`tests/model/present_tests.c`, lines 202-205

### Purpose:
Verify that Build says so when it could not build the hero (Req. 9).

### Assertion expectation:
A report with `hero_skipped` holds "The hero is not built: the civ lacks its unit."

## Test 284

### File residence:
`tests/model/present_tests.c`, lines 207-210

### Purpose:
Verify that Build says when a bonus counts fewer times than its multiplier asks (Req. 9).

### Assertion expectation:
A report with one multiplier not applied says "A bonus made of the game's techs counts only once."

## Test 285

### File residence:
`tests/model/present_tests.c`, lines 212-216

### Purpose:
Verify that Build names the two folders of the mod and nothing as left out (Req. 7, 9).

### Assertion expectation:
A report without anything skipped names `testonia-data` and `testonia-ui` and "2 of 2.", then the advice.

## Test 286

### File residence:
`tests/model/wizard_tests.c`, lines 34-38

### Purpose:
Verify that the list of steps marks no step as done before anything is chosen (Req. 1).

### Assertion expectation:
Of the nine steps of a design just reset, `wizard_step_is_done` is true for none.

## Test 287

### File residence:
`tests/model/wizard_tests.c`, lines 40-43

### Purpose:
Verify that the Identity step is marked done when the civ has a name (Req. 1).

### Assertion expectation:
After the name "Testonia" is set, `wizard_step_is_done` is true for the Identity step.

## Test 288

### File residence:
`tests/model/wizard_tests.c`, lines 45-48

### Purpose:
Verify that the Tech tree step is marked done when a node is chosen (Req. 1).

### Assertion expectation:
After building 12 is chosen, `wizard_step_is_done` is true for the Tech tree step.

## Test 289

### File residence:
`tests/model/wizard_tests.c`, lines 50-53

### Purpose:
Verify that the Civ bonuses step is marked done when a bonus is ticked (Req. 1).

### Assertion expectation:
After bonus 4 is ticked, `wizard_step_is_done` is true for the Civ bonuses step.

## Test 290

### File residence:
`tests/model/wizard_tests.c`, lines 55-58

### Purpose:
Verify that the Team bonus step is marked done when one is picked (Req. 1).

### Assertion expectation:
After team bonus 8 is picked, `wizard_step_is_done` is true for the Team bonus step.

## Test 291

### File residence:
`tests/model/wizard_tests.c`, lines 92-97

### Purpose:
Prevent a tree as large as the base civ's but not the same from being shown as not done (Bug 1 of 2026-11-16).

### Assertion expectation:
With unit 1 in the design and unit 2 in the base, `wizard_step_is_done` is true for the Tech tree step.

## Test 292

### File residence:
`tests/model/wizard_tests.c`, lines 99-104

### Purpose:
Prevent as many bonuses as the base civ's but not the same from being shown as not done (Bug 1 of 2026-11-16).

### Assertion expectation:
With bonus 4 in the design and bonus 5 in the base, `wizard_step_is_done` is true for the Civ bonuses step.

## Test 293

### File residence:
`tests/model/wizard_tests.c`, lines 60-63

### Purpose:
Verify that the Unique unit step is marked done when a unit is picked (Req. 1).

### Assertion expectation:
After unit 1 of the game is picked, `wizard_step_is_done` is true for the Unique unit step.

## Test 294

### File residence:
`tests/model/wizard_tests.c`, lines 65-68

### Purpose:
Verify that the Castle tech step is marked done when a tech is picked (Req. 1).

### Assertion expectation:
After castle tech 3 is picked, `wizard_step_is_done` is true for the Castle tech step.

## Test 295

### File residence:
`tests/model/wizard_tests.c`, lines 70-74

### Purpose:
Verify that the Imperial tech step is marked done when a tech is picked (Req. 1).

### Assertion expectation:
After imperial tech 3 is picked, `wizard_step_is_done` is true for the Imperial tech step.

## Test 296

### File residence:
`tests/model/wizard_tests.c`, lines 76-79

### Purpose:
Verify that the Hero unit step is marked done when a hero is picked (Req. 1).

### Assertion expectation:
After hero 1911 is picked, `wizard_step_is_done` is true for the Hero unit step.

## Test 297

### File residence:
`tests/model/wizard_tests.c`, lines 81-85

### Purpose:
Prevent a choice in one step from marking another step as done (Req. 1).

### Assertion expectation:
After a unique unit is picked on a new design, exactly one of the nine steps is done.

## Test 298

### File residence:
`tests/model/wizard_tests.c`, lines 87-90

### Purpose:
Verify that Back leads from the second step to the first (Req. 1).

### Assertion expectation:
`wizard_move` of the Tech tree step by -1 gives the Identity step.

## Test 299

### File residence:
`tests/model/wizard_tests.c`, lines 106-111

### Purpose:
Verify that a team bonus other than the base civ's marks the Team bonus step done (Bug 1 of 2026-11-16).

### Assertion expectation:
With team bonus 8 in the design and 9 in the base, `wizard_step_is_done` is true for the Team bonus step.

## Test 300

### File residence:
`tests/model/wizard_tests.c`, lines 113-118

### Purpose:
Prevent the base civ's own team bonus from marking the Team bonus step done (Bug 1 of 2026-11-16).

### Assertion expectation:
With team bonus 8 in both the design and the base, `wizard_step_is_done` is false for the Team bonus step.

## Test 301

### File residence:
`tests/model/history_tests.c`, lines 50-54

### Purpose:
Verify that undo does nothing before the first change (Req. 1).

### Assertion expectation:
After `history_start`, `history_step` back gives false.

## Test 302

### File residence:
`tests/model/history_tests.c`, lines 56-60

### Purpose:
Verify that redo does nothing before the first undo (Req. 1).

### Assertion expectation:
After `history_start`, `history_step` forward gives false.

## Test 303

### File residence:
`tests/model/history_tests.c`, lines 62-67

### Purpose:
Prevent an undo with nothing to undo from giving redo something to do (Req. 1).

### Assertion expectation:
After an undo on a new design, `history_step` forward gives false.

## Test 304

### File residence:
`tests/model/history_tests.c`, lines 69-73

### Purpose:
Verify that the window is told when a change was made, so that it marks the civ unsaved (Req. 1).

### Assertion expectation:
`history_do` of ticking bonus 7 on a new design gives true.

## Test 305

### File residence:
`tests/model/history_tests.c`, lines 75-81

### Purpose:
Verify that a new change ends what could be made again (Req. 1).

### Assertion expectation:
After a change, an undo and another change, `history_step` forward gives false.

## Test 306

### File residence:
`tests/model/history_tests.c`, lines 83-87

### Purpose:
Verify that undo goes back 32 changes and no further (Req. 1).

### Assertion expectation:
After 33 changes, undo succeeds 32 times, `HISTORY_DEPTH`, and then fails.

## Test 307

### File residence:
`tests/model/history_tests.c`, lines 101-106

### Purpose:
Verify that the newest changes are the ones kept once the history is full (Req. 1).

### Assertion expectation:
After changes numbered 1 to 33, two undos leave the design at the change numbered 31.

## Test 308

### File residence:
`tests/model/history_tests.c`, lines 108-113

### Purpose:
Verify that opening a civ file or picking a base civ can be taken back (Req. 1).

### Assertion expectation:
After `history_replace`, an undo brings back the design with its one bonus.

## Test 309

### File residence:
`tests/model/history_tests.c`, lines 115-122

### Purpose:
Verify that a design put in place of another ends what could be made again (Req. 1).

### Assertion expectation:
After a change, an undo and `history_replace`, `history_step` forward gives false.

## Test 310

### File residence:
`tests/model/history_tests.c`, lines 124-130

### Purpose:
Prevent a tick from joining the typing before it into one undo (Req. 1).

### Assertion expectation:
After typing "V" and ticking a bonus, one undo leaves the name "V".

## Test 311

### File residence:
`tests/model/history_tests.c`, lines 132-140

### Purpose:
Prevent typing after an undo from joining the typing that was undone (Req. 1).

### Assertion expectation:
After typing "V" and "Vi", an undo and typing "X", an undo succeeds and leaves the name empty.

## Test 312

### File residence:
`tests/design/design_tests.c`, lines 26-29

### Purpose:
Verify that a bonus is saved with the multiplier 1, as the original's civ files have it (Req. 1).

### Assertion expectation:
After bonus 7 is ticked, the multiplier of the design's first bonus is 1.

## Test 313

### File residence:
`tests/design/design_tests.c`, lines 31-34

### Purpose:
Verify that a team bonus is saved with the multiplier 1 (Req. 1).

### Assertion expectation:
After team bonus 3 is picked, the multiplier of the design's team bonus is 1.

## Test 314

### File residence:
`tests/design/design_tests.c`, lines 36-41

### Purpose:
Prevent a civ file from taking a bonus more than 10 times, the most that Build makes (Req. 8).

### Assertion expectation:
A multiplier of 11 is refused, and the bonus counts once.

## Test 315

### File residence:
`tests/design/design_tests.c`, lines 43-48

### Purpose:
Prevent a civ file from taking a bonus less than once (Req. 8).

### Assertion expectation:
A multiplier of 0 is refused, and the bonus counts once.

## Test 316

### File residence:
`tests/design/design_tests.c`, lines 50-54

### Purpose:
Verify that unticking a bonus takes it out of the design (Req. 1).

### Assertion expectation:
After bonus 7 is ticked twice, the design has 0 bonuses.

## Test 317

### File residence:
`tests/design/design_tests.c`, lines 56-61

### Purpose:
Verify that unticking a node takes it out of the tech tree (Req. 1).

### Assertion expectation:
After unit 5 is chosen twice, `civ_design_tree_has` is false for it.

## Test 318

### File residence:
`tests/design/design_tests.c`, lines 63-69

### Purpose:
Prevent a node that is taken out from taking another with it (Req. 1).

### Assertion expectation:
After units 5 and 9 are chosen and 5 is taken out, `civ_design_tree_has` is true for 9.

## Test 319

### File residence:
`tests/design/design_tests.c`, lines 71-75

### Purpose:
Prevent a node from being written past the end of the tree's list (Rule 26).

### Assertion expectation:
After 400 units are chosen, `CIV_TREE_CAPACITY`, choosing unit 401 gives false.

## Test 320

### File residence:
`tests/design/saved_tests.c`, lines 21-26

### Purpose:
Verify that a civ file names a unit of the game as the original's files do (Req. 1).

### Assertion expectation:
With unit 1263 of the game picked, the saved text holds `"km_idx": null`.

## Test 321

### File residence:
`tests/design/saved_tests.c`, lines 28-33

### Purpose:
Verify that a civ file names a custom unit as the original's files do (Req. 1).

### Assertion expectation:
With custom unit 5 picked, the saved text holds `"vanilla_id": null`.

## Test 322

### File residence:
`tests/design/saved_tests.c`, lines 35-40

### Purpose:
Verify that a civ file counts the civ of its language from 0, as the original does (Req. 1).

### Assertion expectation:
With the language of slot 1, the Britons, the saved text holds `"language": 0`.

## Test 323

### File residence:
`tests/shell/dialog_tests.c`, lines 205-208

### Purpose:
Verify that a civ without a name is saved as `civbuilder_civ` by default (Req. 1).

### Assertion expectation:
With Save answered in `build/`, `build/civbuilder_civ.civbuilder.json` is written anew, over a stale file.

## Test 324

### File residence:
`tests/shell/dialog_tests.c`, lines 210-213

### Purpose:
Verify that a civ the user named is saved under that name (Req. 1).

### Assertion expectation:
With the name "Testonia" typed, Save writes `build/Testonia.civbuilder.json` anew, over a stale file.

## Test 325

### File residence:
`tests/shell/dialog_tests.c`, lines 215-218

### Purpose:
Verify that a cancelled Save is said in the status line (Req. 3, WCAG 3.3.1).

### Assertion expectation:
With the Save dialog cancelled, the status line holds "The civ was not saved."

## Test 326

### File residence:
`tests/shell/dialog_tests.c`, lines 220-223

### Purpose:
Verify that a civ saved by the window is opened by it again (Req. 1).

### Assertion expectation:
"Testonia" is saved, the name is changed to "Other", the file is opened: the title names "Testonia" again.

## Test 327

### File residence:
`tests/shell/dialog_tests.c`, lines 225-228

### Purpose:
Verify that a file that is no civ file is refused in words (Req. 1, WCAG 3.3.1).

### Assertion expectation:
Opening `README.md` gives a message that holds "not a civ file".

## Test 328

### File residence:
`tests/shell/dialog_tests.c`, lines 230-233

### Purpose:
Verify that the button Next goes to the next step (Req. 3, WCAG 2.1.1).

### Assertion expectation:
After a click of Next on step 1, the list of steps has its second row chosen.

## Test 329

### File residence:
`tests/shell/dialog_tests.c`, lines 235-238

### Purpose:
Verify that a name typed into the window reaches the design and the title (Req. 3, WCAG 2.4.2).

### Assertion expectation:
With "Testonia" typed into Name, the window's title holds "Testonia - Step 1".

## Test 330

### File residence:
`tests/shell/dialog_tests.c`, lines 240-243

### Purpose:
Verify that Redo answers in the status line when nothing was undone (Req. 5).

### Assertion expectation:
In a new window the Redo command leaves "There is nothing to redo." in the status line.

## Test 331

### File residence:
`tests/shell/dialog_tests.c`, lines 245-260

### Purpose:
Prevent a step from being shown as done when the window opens, before the user did anything (Bug 1 of 2026-11-16).

### Assertion expectation:
In a new window, made but not shown, no row of the list of steps holds "(done)", though Generic fills the tree.

## Test 332

### File residence:
`tests/shell/dialog_tests.c`, lines 262-265

### Purpose:
Prevent unsaved changes from being lost when closing is cancelled (Req. 3, WCAG 3.3.4).

### Assertion expectation:
With a name typed and the question answered Cancel, the window is still there after a close.

## Test 333

### File residence:
`tests/shell/dialog_tests.c`, lines 267-270

### Purpose:
Verify that the window closes when the user chooses not to save (Req. 3, WCAG 3.3.4).

### Assertion expectation:
With a name typed and the question answered No, the window is gone after a close.

## Test 334

### File residence:
`tests/shell/dialog_tests.c`, lines 272-275

### Purpose:
Verify that Exit closes a window without changes, without a question (Rule 24: asked wrongly before).

### Assertion expectation:
In a new window the Exit command closes the window; no question about unsaved changes is asked.

## Test 335

### File residence:
`tests/shell/dialog_tests.c`, lines 277-280

### Purpose:
Verify that Build says so when the game is not found on the computer (Req. 9).

### Assertion expectation:
With Steam's folder named as `build/no_steam`, Build gives a message that holds "cannot be built".

## Test 336

### File residence:
`tests/shell/dialog_tests.c`, lines 282-285

### Purpose:
Verify that Build asks before it writes and writes nothing when refused (Req. 3, WCAG 3.3.4).

### Assertion expectation:
With the question answered Cancel, the status line holds "No mod was built."

## Test 337

### File residence:
`tests/shell/dialog_tests.c`, lines 287-290

### Purpose:
Verify that Build of a civ that is not finished says what is wrong (Req. 9).

### Assertion expectation:
For a new civ, Build gives a message that holds "not finished".

## Test 338

### File residence:
`tests/shell/dialog_tests.c`, lines 292-301

### Purpose:
Verify that Build from the window writes the mod into the mods folder and reports it (Req. 2, 7, 9).

### Assertion expectation:
With a name, a unit and two techs picked and the small game, the message holds "built as testonia-data".

## Test 339

### File residence:
`tests/shell/dialog_tests.c`, lines 303-306

### Purpose:
Verify that the help line under the title is drawn in the muted colour of the palette (Req. 3, WCAG 1.4.3).

### Assertion expectation:
Asked for its colours, the window gives the help line the text colour of `COLOR_ROLE_MUTED`.

## Test 340

### File residence:
`tests/shell/dialog_tests.c`, lines 308-311

### Purpose:
Verify that a field one types in is drawn on the field colour of the palette (Req. 3, WCAG 1.4.3).

### Assertion expectation:
Asked for its colours, the window gives the field Name the background of `COLOR_ROLE_FIELD`.

## Test 341

### File residence:
`tests/shell/dialog_tests.c`, lines 313-333

### Purpose:
Verify that the row in focus inside a list has a frame in the focus colour (Req. 3, WCAG 2.4.7).

### Assertion expectation:
A row drawn with the focus has the colour of `COLOR_ROLE_FOCUS` at its left edge.

## Test 342

### File residence:
`tests/shell/dialog_tests.c`, lines 335-344

### Purpose:
Prevent a pick of the row that is picked already from making a change to undo (Req. 5).

### Assertion expectation:
With the first unit picked twice, the second Undo says "There is nothing to undo."

## Test 343

### File residence:
`tests/shell/scroll_tests.c`, lines 186-191

### Purpose:
Verify that a list's scroll bar shows its arrows and knob without the mouse over it (Bug 2 of 2026-11-16).

### Assertion expectation:
The up arrow of the shown Tech tree list, drawn by `WM_PRINT`, holds at least 4 colours; the thin bar holds 2.

## Test 344

### File residence:
`tests/shell/scroll_tests.c`, lines 193-200

### Purpose:
Verify that the window's scroll bar shows its arrows and knob without the mouse over it (Bug 2 of 2026-11-16).

### Assertion expectation:
In a window of 480 by 360 on the Tech tree, the top right corner of its inside holds at least 4 colours.

## Test 345

### File residence:
`tests/shell/scroll_tests.c`, lines 202-212

### Purpose:
Prevent the window's scroll bar from taking the step's margin on the right (Bug 2 of 2026-11-16).

### Assertion expectation:
In a window of 480 by 360, Next ends as far from the scroll bar as Back starts from the left, to a pixel.

## Test 346

### File residence:
`tests/shell/scroll_tests.c`, lines 225-241

### Purpose:
Verify that the knob of the window's scroll bar reaches the bar's end at the end of a step (Req. 3, WCAG 1.4.10).

### Assertion expectation:
After 16 pages down on the Tech tree in a window of 480 by 360, position and page of the bar add up to its range.

## Test 347

### File residence:
`tests/shell/scroll_tests.c`, lines 243-253

### Purpose:
Prevent the window's scroll bar from showing when the step fits the window exactly (Req. 3, WCAG 1.4.10).

### Assertion expectation:
A window 480 wide whose inside is exactly as high as the step Hero in one column has its scroll bar hidden.

## Test 348

### File residence:
`tests/shell/scroll_tests.c`, lines 255-268

### Purpose:
Verify that the up arrow of the window's scroll bar moves the step up a line (Req. 3, WCAG 1.4.10).

### Assertion expectation:
On the Tech tree at 480 by 360, a page down and then the up arrow leave the bar 44 units above the page.

## Test 349

### File residence:
`tests/shell/scroll_tests.c`, lines 270-279

### Purpose:
Verify that each notch of the mouse wheel moves a list one row, up or down (Bug 4 of 2026-11-16).

### Assertion expectation:
From row 2 of the Tech tree list, a notch up and two down, run through `app_run_messages`, leave row 3 on top.

## Test 350

### File residence:
`tests/shell/scroll_tests.c`, lines 281-290

### Purpose:
Verify that a press on either arrow of a list's scroll bar moves it one row, up or down (Bug 4 of 2026-11-16).

### Assertion expectation:
From row 2, a click on the up arrow, one on the down arrow and a double click on it, each released, leave row 3.

## Test 351

### File residence:
`tests/shell/scroll_tests.c`, lines 302-318

### Purpose:
Prevent a list on the screen from sliding when its wheel or arrows scroll it (Bug 3 of 2026-11-16).

### Assertion expectation:
Two notches, a click and a double click on the up arrow each take less than 150 ms; sliding takes 190 to 530 ms.

## Test 352

### File residence:
`tests/shell/scroll_tests.c`, lines 320-328

### Purpose:
Verify that holding a list's arrow keeps moving it a row at a time, as a normal scroll bar does (Bug 4 of 2026-11-16).

### Assertion expectation:
A press on the down arrow, still held, and one tick of `LIST_REPEAT_TIMER` leave row 2 on top.

## Test 353

### File residence:
`tests/shell/scroll_tests.c`, lines 330-339

### Purpose:
Prevent the arrows of a list's scroll bar from taking a press on the bar between them (Bug 4 of 2026-11-16).

### Assertion expectation:
With the list at its end, a press just under the up arrow moves its top row up by more than one row.

## Test 354

### File residence:
`tests/shell/scroll_tests.c`, lines 341-349

### Purpose:
Verify that the loop of messages hands a key pressed in a list to the list once (Req. 3, WCAG 2.1.1).

### Assertion expectation:
With the caret on row 0 of the Tech tree list, the key Down, run through `app_run_messages`, puts it on row 1.

## Test 355

### File residence:
`tests/accessibility/access_tests.c`, lines 14-27

### Purpose:
Verify that all text can be read by those who see less contrast (Req. 3, WCAG 2.2 SC 1.4.3).

### Assertion expectation:
Each of the 7 pairs of text and background colors in `palette.c` contrasts at least 4.50:1 by the WCAG formula.

## Test 356

### File residence:
`tests/accessibility/access_tests.c`, lines 29-49

### Purpose:
Verify that every input says what it is for, seen and to a screen reader (Req. 3, WCAG 2.2 SC 2.5.3, 3.3.2).

### Assertion expectation:
Every text field, list, checklist, choice and report in `layout.c` has a label of its own.

## Test 357

### File residence:
`tests/accessibility/access_tests.c`, lines 51-65

### Purpose:
Verify that everything one clicks is large enough to hit (Req. 3, WCAG 2.2 SC 2.5.8).

### Assertion expectation:
In the smallest window, 960 by 560, every control one can act on is at least 44 by 44, and so is a list row.

## Test 358

### File residence:
`tests/accessibility/access_tests.c`, lines 67-114

### Purpose:
Verify that no control or label hides another, so focus is never covered (Req. 3, WCAG 2.2 SC 2.4.11).

### Assertion expectation:
On each of the 9 steps at 960 by 560, no two boxes of controls and labels overlap, and all lie inside the window.

## Test 359

### File residence:
`tests/accessibility/access_tests.c`, lines 116-125

### Purpose:
Verify that the window's title says which civ is open and on which step (WCAG 2.2 SC 2.4.2; Req. 3).

### Assertion expectation:
For a civ named Testonia on the tech tree step, the title is "Testonia - Step 2 of 9: Tech tree - civbuilder".

## Test 360

### File residence:
`tests/accessibility/accessibility_suite.c`, lines 29-31; `tests/accessibility/access_tests.c`, lines 127-141

### Purpose:
Enforce that the ways from step to step keep their places on every step (Requirement 3, WCAG 2.2 SC 3.2.3).

### Assertion expectation:
On each of the nine steps the list of steps is shown, Back wherever a step lies before, Next wherever one lies after.

## Test 361

### File residence:
`tests/reflow/reflow_tests.c`, lines 103-106

### Purpose:
Verify that a window too narrow for the columns gets one column (Req. 3, WCAG 1.4.10).

### Assertion expectation:
`reflow_is_needed` is true for a window of 959 by 560.

## Test 362

### File residence:
`tests/reflow/reflow_tests.c`, lines 108-111

### Purpose:
Verify that a window too short for the columns gets one column and scrolls (Req. 3, WCAG 1.4.4).

### Assertion expectation:
`reflow_is_needed` is true for a window of 960 by 559.

## Test 363

### File residence:
`tests/reflow/reflow_tests.c`, lines 113-116

### Purpose:
Prevent the one column from taking the place of the columns in a window that holds them (Req. 1).

### Assertion expectation:
`reflow_is_needed` is false for a window of 960 by 560.

## Test 364

### File residence:
`tests/reflow/reflow_tests.c`, lines 118-121

### Purpose:
Verify that no step needs scrolling sideways at a width of 320 (Req. 3, WCAG 1.4.10).

### Assertion expectation:
In every step, every control and label starts at 0 or more and ends at 320 or less, across.

## Test 365

### File residence:
`tests/reflow/reflow_tests.c`, lines 123-132

### Purpose:
Prevent a control from hiding another in the one column (Req. 3, WCAG 1.4.10).

### Assertion expectation:
In every step, no two boxes of controls and labels overlap at a width of 320.

## Test 366

### File residence:
`tests/reflow/reflow_tests.c`, lines 134-137

### Purpose:
Verify that every control one acts on stays large enough in the one column (Req. 3, WCAG 2.5.8).

### Assertion expectation:
In every step, every control one acts on is at least 44 wide and 44 high at a width of 320.

## Test 367

### File residence:
`tests/reflow/reflow_tests.c`, lines 139-144

### Purpose:
Verify that the window scrolls to the end of a step and no further (Req. 3, WCAG 1.4.10).

### Assertion expectation:
`reflow_height` of the bonuses step is the lower edge of Next plus a margin of 16.

## Test 368

### File residence:
`tests/reflow/reflow_tests.c`, lines 146-149

### Purpose:
Verify that the control in focus is never left below the window (Req. 3, WCAG 2.4.11).

### Assertion expectation:
A box at 600, 44 high, in a view of 256 scrolled to 0, gives a scroll of 600 + 44 + 16 - 256.

## Test 369

### File residence:
`tests/reflow/reflow_tests.c`, lines 151-154

### Purpose:
Verify that the control in focus is never left above the window, nor its label (Req. 3, WCAG 2.4.11).

### Assertion expectation:
A box at 100 in a view scrolled to 300 gives a scroll of 100 - 26 - 16: its label and a margin show.

## Test 370

### File residence:
`tests/reflow/reflow_tests.c`, lines 156-159

### Purpose:
Prevent the window from jumping when the focus moves to a control in view (Req. 3, WCAG 3.2.1).

### Assertion expectation:
A box at 400, 44 high, in a view of 256 scrolled to 300, gives a scroll of 300.

## Test 371

### File residence:
`tests/reflow/reflow_tests.c`, lines 161-164

### Purpose:
Prevent scrolling past the end of a step (Req. 3).

### Assertion expectation:
`reflow_scroll_within` of 900, for a content of 700 in a view of 256, gives 444.

## Test 372

### File residence:
`tests/reflow/reflow_tests.c`, lines 166-169

### Purpose:
Prevent scrolling above the start of a step (Req. 3).

### Assertion expectation:
`reflow_scroll_within` of -5 gives 0.

## Test 373

### File residence:
`tests/reflow/reflow_tests.c`, lines 171-174

### Purpose:
Prevent scrolling a step that fits the window (Req. 3).

### Assertion expectation:
`reflow_scroll_within` of 50, for a content of 200 in a view of 256, gives 0.

## Test 374

### File residence:
`tests/reflow/reflow_tests.c`, lines 307-310

### Purpose:
Verify that the one column has no gap between the help and a step's controls (Req. 3, WCAG 1.4.10).

### Assertion expectation:
At a width of 320, the Base civ list of the Identity step is at 370 from the top.

## Test 375

### File residence:
`tests/reflow/reflow_tests.c`, lines 312-317

### Purpose:
Verify that Back and Next end the one column, as they end the Tab order (Req. 3, WCAG 2.4.3).

### Assertion expectation:
At a width of 320, Back is 8 below the lower edge of the status line.

## Test 376

### File residence:
`tests/reflow/reflow_tests.c`, lines 319-322

### Purpose:
Verify that a control of the one column keeps its margin on the left (Req. 3, WCAG 1.4.10).

### Assertion expectation:
At a width of 320, the Name field starts at 16.

## Test 377

### File residence:
`tests/reflow/reflow_tests.c`, lines 324-329

### Purpose:
Verify that Back and Next together are as wide as the other controls (Req. 3, WCAG 1.4.10).

### Assertion expectation:
At a width of 320, Next ends at 304.

## Test 378

### File residence:
`tests/accessibility/accessibility_suite.c`, lines 43-45; `tests/accessibility/window_tests.c`, lines 36-207

### Purpose:
Enforce that every control can be reached with the keyboard alone (Requirement 3, WCAG 2.2 SC 2.1.1).

### Assertion expectation:
In the real window, made but not shown, the Tab order of each step holds every control of it that one acts on.

## Test 379

### File residence:
`tests/accessibility/accessibility_suite.c`, lines 46-48; `tests/accessibility/window_tests.c`, lines 232-244

### Purpose:
Enforce that the window tells which step the user is on, as it shows it (Requirement 3, WCAG 2.2 SC 1.3.1).

### Assertion expectation:
In the real window, after going to each step by its command, the list of steps has that step as its chosen row.

## Test 380

### File residence:
`tests/accessibility/accessibility_suite.c`, lines 49-51; `tests/accessibility/window_tests.c`, lines 246-299

### Purpose:
Enforce that the keyboard goes through a step as it is read (Requirement 3, WCAG 2.2 SC 2.4.3 and 1.3.2).

### Assertion expectation:
On each step, Tab goes from the list of steps down each column from left to right, then to Back and Next.

## Test 381

### File residence:
`tests/accessibility/accessibility_suite.c`, lines 52-54; `tests/accessibility/window_tests.c`, lines 301-324

### Purpose:
Catch again Bug 1 of the trial: a click on a row scrolled the list of bonuses to its top (Rule 24).

### Assertion expectation:
In the real window, with the list scrolled to row 30, ticking row 34 leaves row 30 at the top of the list.

## Test 382

### File residence:
`tests/accessibility/accessibility_suite.c`, lines 55-57; `tests/accessibility/window_tests.c`, lines 53-215

### Purpose:
Enforce that each control has a name for assistive technology (Requirement 3, WCAG 2.2 SC 4.1.2, 1.3.1, 2.5.3).

### Assertion expectation:
In the real window, the name that Windows' accessibility interface gives each control begins with its label.

## Test 383

### File residence:
`tests/accessibility/accessibility_suite.c`, lines 58-60; `tests/accessibility/window_tests.c`, lines 326-356

### Purpose:
Enforce that the check of a ticked row has a text alternative (Requirement 3, WCAG 2.2 SC 1.1.1).

### Assertion expectation:
In the real window, Windows' accessibility interface gives a ticked row of the bonuses as chosen, another row not.

## Test 384

### File residence:
`tests/accessibility/accessibility_suite.c`, lines 61-63; `tests/accessibility/window_tests.c`, lines 358-379

### Purpose:
Enforce that a control does nothing by getting the focus alone (Requirement 3, WCAG 2.2 SC 3.2.1).

### Assertion expectation:
After the focus was put on every control of every step, undo still says "There is nothing to undo."

## Test 385

### File residence:
`tests/accessibility/accessibility_suite.c`, lines 64-65; `tests/accessibility/window_tests.c`, lines 87-220

### Purpose:
Enforce that the keyboard is never trapped in a control (Requirement 3, WCAG 2.2 SC 2.1.2).

### Assertion expectation:
With the focus on each control of each step in turn, a Tab key given to the dialog manager moves the focus on.

## Test 386

### File residence:
`tests/accessibility/accessibility_suite.c`, lines 77-79; `tests/accessibility/window_tests.c`, lines 381-393

### Purpose:
Enforce that the language of the window can be found by a program (Requirement 3, WCAG 2.2 SC 3.1.1).

### Assertion expectation:
Once the window is made, the one language the process prefers for its texts, as Windows tells it, is `en-US`.

## Test 387

### File residence:
`tests/accessibility/accessibility_suite.c`, lines 80-82; `tests/accessibility/window_tests.c`, lines 113-225

### Purpose:
Enforce that every control has instructions of its own (Requirement 3, WCAG 2.2 SC 3.3.2).

### Assertion expectation:
In the real window, with the focus on each control in turn, the help command puts its help in the status line.

## Test 388

### File residence:
`tests/accessibility/accessibility_suite.c`, lines 83-85; `tests/accessibility/window_tests.c`, lines 395-440

### Purpose:
Enforce that a status message reaches assistive technology (Requirement 3, WCAG 2.2 SC 4.1.3).

### Assertion expectation:
In the real window, UI Automation gives the status line the live setting "polite".

## Test 389

### File residence:
`tests/accessibility/accessibility_suite.c`, lines 86-88; `tests/accessibility/access_tests.c`, lines 143-146

### Purpose:
Enforce that the text follows the text size set in Windows (Requirement 3, WCAG 2.2 SC 1.4.4).

### Assertion expectation:
At 96 dots an inch and a text size of 400%, the layout is scaled as at 225%, the most Windows offers: 216.

## Test 390

### File residence:
`tests/accessibility/accessibility_suite.c`, lines 89-91; `tests/accessibility/window_tests.c`, lines 127-230

### Purpose:
Enforce that the focus can be seen (Requirement 3, WCAG 2.2 SC 2.4.7).

### Assertion expectation:
With the focus on each control in turn, the window paints 2 or more pixels of another color just left of it.

## Test 391

### File residence:
`tests/accessibility/accessibility_suite.c`, lines 92-94; `tests/accessibility/draw_tests.c`, lines 11-94

### Purpose:
Catch again Bug 2 of the trial: a ticked box showed a filled rectangle, not a check (Rule 24).

### Assertion expectation:
In the picture the window draws of a ticked row, the inside of the box holds marks, on less than two fifths of it.

## Test 392

### File residence:
`tests/accessibility/accessibility_suite.c`, lines 95-97; `tests/accessibility/choice_tests.c`, lines 11-26

### Purpose:
Enforce that a list or choice shows what the design holds, after an undo too (Requirement 5).

### Assertion expectation:
In the real window, after another base civ is picked and the pick undone, the choice shows Generic again.

## Test 393

### File residence:
`tests/reflow/reflow_tests.c`, lines 218-226

### Purpose:
Verify that the real window reflows when it is made narrow (Req. 3, WCAG 1.4.10).

### Assertion expectation:
In a window 420 wide, the Architecture list starts as far left as the Name field.

## Test 394

### File residence:
`tests/reflow/reflow_tests.c`, lines 228-239

### Purpose:
Verify that the real window brings the control in focus into view (Req. 3, WCAG 2.4.11).

### Assertion expectation:
In a window 420 by 360, the Scout list lies between the window's top and bottom once it has the focus.

## Test 395

### File residence:
`tests/reflow/reflow_tests.c`, lines 241-257

### Purpose:
Verify that the window can be made as small as WCAG 1.4.10 asks to be usable (Req. 3).

### Assertion expectation:
The least size the window reports is the frame around 320 by 256, at the scale in use.

## Test 396

### File residence:
`tests/reflow/reflow_tests.c`, lines 259-262

### Purpose:
Verify that the keyboard scrolls a step: Go, Scroll down, Ctrl+Down (Req. 3, WCAG 2.1.1).

### Assertion expectation:
In a window 420 by 360, the scroll bar's place is above 0 after Scroll down.

## Test 397

### File residence:
`tests/reflow/reflow_tests.c`, lines 264-268

### Purpose:
Verify that the keyboard scrolls a step back: Go, Scroll up, Ctrl+Up (Req. 3, WCAG 2.1.1).

### Assertion expectation:
Scroll down twice and Scroll up once leave the scroll bar where one Scroll down leaves it.

## Test 398

### File residence:
`tests/reflow/reflow_tests.c`, lines 270-279

### Purpose:
Verify that the mouse wheel scrolls a step (Req. 3).

### Assertion expectation:
In a window 420 by 360, the scroll bar's place is above 0 after one notch of the wheel downwards.

## Test 399

### File residence:
`tests/reflow/reflow_tests.c`, lines 293-296

### Purpose:
Verify that a row too long for its list is shown on more lines, not cut (Req. 3, WCAG 1.4.10).

### Assertion expectation:
In a window 420 wide, the first row of the bonuses is higher than the third, "Shepherds work 25% faster".

## Test 400

### File residence:
`tests/reflow/reflow_tests.c`, lines 298-305

### Purpose:
Prevent a row of one line from becoming smaller than a target (Req. 3, WCAG 2.5.8).

### Assertion expectation:
The third row of the bonuses is 44 high at the scale in use.

## Test 401

### File residence:
`tests/maintainability/maintainability_suite.c`, lines 19-21; `tests/maintainability/code_rules.c`, lines 52-67

### Purpose:
Enforce that the code holds no comments (Rule 10).

### Assertion expectation:
No `.c` or `.h` file holds `//` or `/*` outside a string; a failure names the file and line.

## Test 402

### File residence:
`tests/maintainability/maintainability_suite.c`, lines 22-24; `tests/maintainability/code_rules.c`, lines 22-35

### Purpose:
Enforce that no function is longer than 60 lines of code (Rule 11, P10 rule 4).

### Assertion expectation:
Every function, from its opening to its closing brace, has at most 60 lines that are not blank.

## Test 403

### File residence:
`tests/maintainability/maintainability_suite.c`, lines 25-27; `tests/maintainability/code_rules.c`, lines 37-50

### Purpose:
Enforce that no line of code is longer than 120 characters (Rule 12).

### Assertion expectation:
Every line of every `.c` and `.h` file has at most 120 characters; a failure names the file and line.

## Test 404

### File residence:
`tests/maintainability/maintainability_suite.c`, lines 28-30; `tests/maintainability/code_rules.c`, lines 52-67

### Purpose:
Enforce that no file of code is longer than 400 lines of code (Rule 13).

### Assertion expectation:
Every `.c` and `.h` file has at most 400 lines that are not blank.

## Test 405

### File residence:
`tests/maintainability/maintainability_suite.c`, line 45; `tests/maintainability/code_patterns.c`, lines 78-81

### Purpose:
Enforce simple control flow without jumps (Rule 26, P10 rule 1).

### Assertion expectation:
The words `goto`, `setjmp` and `longjmp` appear in no code.

## Test 406

### File residence:
`tests/maintainability/maintainability_suite.c`, line 47; `tests/maintainability/code_rules.c`, lines 155-171

### Purpose:
Enforce that no function calls itself, directly or through others (Rule 26, P10 rule 1).

### Assertion expectation:
The graph of calls between the functions of all `.c` files has no cycle; a failure names a function in one.

## Test 407

### File residence:
`tests/maintainability/maintainability_suite.c`, line 49; `tests/maintainability/code_patterns.c`, lines 83-86

### Purpose:
Enforce that no memory is allocated while the program runs (Rule 26, P10 rule 3).

### Assertion expectation:
None of `malloc`, `calloc`, `realloc`, `free`, `alloca`, `strdup` or the Win32 allocators appears in the code.

## Test 408

### File residence:
`tests/maintainability/maintainability_suite.c`, line 51; `tests/maintainability/code_patterns.c`, lines 88-144

### Purpose:
Enforce that the preprocessor only includes files and guards headers (Rule 26, P10 rule 8).

### Assertion expectation:
Only `#include`, `#ifndef`, `#endif` and `#define` of a `_H` guard or of `CHECK(` appear.

## Test 409

### File residence:
`tests/maintainability/maintainability_suite.c`, line 53; `tests/maintainability/code_patterns.c`, lines 146-188

### Purpose:
Enforce at most one level of dereferencing (Rule 26, P10 rule 9).

### Assertion expectation:
No `**`, no `*const *` and no chain of two `->` in the code.

## Test 410

### File residence:
`tests/maintainability/maintainability_suite.c`, lines 54-55; `tests/maintainability/code_patterns.c`, lines 190-219

### Purpose:
Enforce that no function pointer is used but the one window procedure Win32 needs (Rule 26, P10 rule 9).

### Assertion expectation:
No `(*name)(`, no callback API; `lpfnWndProc` only in `src/win/app_window.c`.

## Test 411

### File residence:
`tests/maintainability/maintainability_suite.c`, line 69; `tests/rules/repo_rules.c`, lines 30-80

### Purpose:
Enforce that no folder holds more than 7 files, the main folder 8 (Rule 14).

### Assertion expectation:
Every folder of the files Git sees holds at most 7 files, the main one 8; `documentation/DEVLOG/` is not counted.

## Test 412

### File residence:
`tests/maintainability/maintainability_suite.c`, line 71; `tests/rules/repo_rules.c`, lines 82-161

### Purpose:
Enforce that no two files share a name (Rule 15).

### Assertion expectation:
No two file names are equal when case is ignored, `SKILL.md` aside.

## Test 413

### File residence:
`tests/maintainability/maintainability_suite.c`, line 73; `tests/rules/repo_rules.c`, lines 119-166

### Purpose:
Enforce that the project holds only C and documents (Rule 29).

### Assertion expectation:
Every file ends in `.c`, `.h`, `.md`, `.txt`, `.json` or `.png`, or is `LICENSE`, `.gitignore` or `.gitattributes`.

## Test 414

### File residence:
`tests/maintainability/maintainability_suite.c`, line 75; `tests/rules/repo_rules.c`, lines 168-231

### Purpose:
Enforce that the documents of repo_template are filled in (Rule 3).

### Assertion expectation:
No `.md` file holds two opening braces before a capital, or the template's start or end mark.

## Test 415

### File residence:
`tests/maintainability/maintainability_suite.c`, line 77; `tests/rules/log_rules.c`, lines 31-217

### Purpose:
Enforce the form of the development log (Rules 18 to 21).

### Assertion expectation:
`DEVLOG1.md` has its title, at most 1000 lines, dated days listed in order at its top, and times as hh:mm:ss.

## Test 416

### File residence:
`tests/maintainability/maintainability_suite.c`, line 79; `tests/rules/log_rules.c`, lines 219-293

### Purpose:
Enforce that TESTS.md numbers its tests 1, 2, 3 in order, with short fields (Rules 5, 6).

### Assertion expectation:
Headings and contents both count up from 1, agree, and no field line passes 120 characters.

## Test 417

### File residence:
`tests/maintainability/maintainability_suite.c`, line 81; `tests/rules/log_rules.c`, lines 295-332

### Purpose:
Enforce that TESTS.md documents every test and no more (Rule 6).

### Assertion expectation:
The `## Test` headings are as many as the `case` lines of the three suites.

## Test 418

### File residence:
`tests/maintainability/maintainability_suite.c`, line 83; `tests/rules/log_rules.c`, lines 334-348

### Purpose:
Enforce that every term is one short line (Rule 9).

### Assertion expectation:
No line of `TERMINOLOGY.md` passes 120 characters.

## Test 419

### File residence:
`tests/rules/gate_tests.c`, lines 29-32

### Purpose:
Prevent the test of simple control flow from passing a `goto` unseen (Rule 26).

### Assertion expectation:
In `tests/fixtures/broken_rules.txt`, `pattern_forbidden_flow_line` gives 5, the line of `goto out;`.

## Test 420

### File residence:
`tests/rules/gate_tests.c`, lines 34-37

### Purpose:
Prevent the test of dynamic memory from passing an allocation unseen (Rule 26).

### Assertion expectation:
In `tests/fixtures/broken_rules.txt`, `pattern_allocation_line` gives 12, the line of `malloc(4)`.

## Test 421

### File residence:
`tests/rules/gate_tests.c`, lines 39-42

### Purpose:
Prevent the test of the preprocessor from passing a macro unseen (Rule 26).

### Assertion expectation:
In `tests/fixtures/broken_rules.txt`, `pattern_complex_directive_line` gives 23, the line of `#define TWICE(x)`.

## Test 422

### File residence:
`tests/rules/gate_tests.c`, lines 44-47

### Purpose:
Prevent the test of pointer depth from passing two dereferences in a row unseen (Rule 26).

### Assertion expectation:
In `tests/fixtures/broken_rules.txt`, `pattern_deep_pointer_line` gives 27, the line of `outer->inner->value`.

## Test 423

### File residence:
`tests/rules/gate_tests.c`, lines 49-52

### Purpose:
Prevent the test of function pointers from passing one unseen (Rule 26).

### Assertion expectation:
In `tests/fixtures/broken_rules.txt`, `pattern_function_pointer_line` gives 30, the line of `(*callback)(int)`.

## Test 424

### File residence:
`tests/rules/gate_tests.c`, lines 54-57

### Purpose:
Prevent the test of comments from passing a comment unseen (Rule 10).

### Assertion expectation:
After `source_scan` of `tests/fixtures/broken_rules.txt`, `comment_line` is 34, the line of its one comment.

## Test 425

### File residence:
`tests/rules/gate_tests.c`, lines 59-66

### Purpose:
Prevent the test of recursion from passing a function that calls itself unseen (Rule 26).

### Assertion expectation:
The call graph of `tests/fixtures/broken_rules.txt` has a cycle, and the function named is `again`.


## Test 426

### File residence:
`tests/rules/tool_tests.c`, lines 82-85

### Purpose:
Verify that the build reads the sources of a folder and of the folders in it (Rule 3).

### Assertion expectation:
`source_list_collect` of `tests` and `.txt` lists `tests/fixtures/broken_rules.txt`.

## Test 427

### File residence:
`tests/rules/tool_tests.c`, lines 89-102

### Purpose:
Verify that the build lists a folder's sources and nothing before them or beside them (Rule 3).

### Assertion expectation:
Every file that `source_list_collect` lists of `tests` and `.txt` lies in `tests/`, and there is at least one.

## Test 428

### File residence:
`tests/rules/tool_tests.c`, lines 111-116

### Purpose:
Verify that the build reads the sources of `src`, `tests` and `tools`, each once (Rule 3).

### Assertion expectation:
`source_list_collect_all` of `.h` lists as many files as `source_list_collect` of `src`, `tests` and `tools` together.

## Test 429

### File residence:
`tests/rules/tool_tests.c`, lines 118-124

### Purpose:
Verify that a header changed after an object makes the build compile again (Rule 3).

### Assertion expectation:
Of two files changed at 9 and at 5, `source_list_newest` gives 9.

## Test 430

### File residence:
`tests/rules/tool_tests.c`, lines 126-132

### Purpose:
Verify that each program finds the object of a source where the build put it (Rule 3).

### Assertion expectation:
`source_built_path` of `build/obj_quick`, `src/core/text.c` and `.o` gives `build/obj_quick/text.o`.

## Test 431

### File residence:
`tests/rules/tool_tests.c`, lines 134-137

### Purpose:
Verify that a build compiles only what changed (Rule 3).

### Assertion expectation:
A source changed at 0, with headers changed at 0, is fresh against an object that exists.

## Test 432

### File residence:
`tests/rules/tool_tests.c`, lines 139-142

### Purpose:
Prevent a stale object after a header changed (Rule 3).

### Assertion expectation:
A source is not fresh when a header is changed one tick after its object.

## Test 433

### File residence:
`tests/rules/tool_tests.c`, lines 144-147

### Purpose:
Prevent a stale object when a source is saved in the tick its object was made (Rule 3).

### Assertion expectation:
A source changed at the very time of its object is not fresh.

## Test 434

### File residence:
`tests/rules/tool_tests.c`, lines 149-152

### Purpose:
Verify that the build tool is linked from its own sources and `src/core/` only (Rule 3).

### Assertion expectation:
For the build tool, `src/core/text.c` and `tools/build/build.c` give `obj/text.o` and `obj/build.o`, and no library.

## Test 435

### File residence:
`tests/rules/tool_tests.c`, lines 154-157

### Purpose:
Verify that `build.exe coverage` builds the tests to measure what they run (Req. 10).

### Assertion expectation:
`build_mode_of` of "build.exe coverage" is `BUILD_MODE_COVERAGE`.

## Test 436

### File residence:
`tests/rules/tool_tests.c`, lines 159-162

### Purpose:
Verify that `nowindow` and `norules` given to the build are passed on to the tests (Req. 12).

### Assertion expectation:
`build_test_arguments` of "build.exe quick nowindow norules" is " nowindow norules".

## Test 437

### File residence:
`tests/rules/tool_tests.c`, lines 164-169

### Purpose:
Verify that a coverage build links the tests and no other program (Req. 10).

### Assertion expectation:
`build_programs_of(BUILD_MODE_COVERAGE)` starts at `SOURCE_PROGRAM_TESTS`, ends before `SOURCE_PROGRAM_BUILD_TOOL`.

## Test 438

### File residence:
`tests/rules/tool_tests.c`, lines 171-175

### Purpose:
Verify that a coverage build links the tests with gcc's coverage (Req. 10).

### Assertion expectation:
The link command of the tests in a coverage build holds "gcc -g --coverage -o build/tests.exe".

## Test 439

### File residence:
`tests/rules/tool_tests.c`, lines 177-181

### Purpose:
Verify that a coverage build makes the folder that its report is written to (Req. 10).

### Assertion expectation:
The last folder of `BUILD_MODE_COVERAGE` is "target/coverage/c".

