# SnipShape Screen Capture and Annotation Tool

AutoHotkey 2.1 alternate to Windows Snipping Tool with additional features and
lower system impact. Creates a overlay Gui of Snipped area that can be
manipulated onscreen with shapes and annotation tools added to it. Based on
Autohotkey Snipper tool that places the snip GUI directly over the snipped area vs
the more complex methods of using external windows or retrieving from the
clipboard. Small buttonbar is available to assist with annotation and other tools.

## Credits

**Credits:**
Fanatic Guru / SKAN / iseahound plus
The work of dozens of people inspired this script.
Many of them sited/listed in the source code:

| Script | Link |
| ------ | ------ |
|Screen Clipping|<https://www.autohotkey.com/boards/viewtopic.php?f=6&t=12088>|
|Gdip|<https://www.autohotkey.com/boards/viewtopic.php?t=72011>|
|Snipper|<https://www.autohotkey.com/boards/viewtopic.php?f=83&t=115622>|
|UWP OCR Library|<https://github.com/Descolada/OCR>|

## Requirements

Autohotkey 2.1 Alpha 14 or later

## Installation

Clone the repository to a local folder. If you have ahk2 associated with
Autohotkey 2.1 alpha then just click on the SnipShape.ahk2 file in the base
folder. You may rename the SnipShape.ahk2 extension to something else if you are
using a different file association.

## Snipping Overview

### One step - hold down activation keys

Current key assignments for screen selection (can be modified in the SnipShape.ahk2 file)

| **HotKey** | **Action** |
| ------ | ------ |
| Ctrl-Win | Then drag selection to create a snip |
| Alt-Win  | Then drag a selection to copy to the clipboard directly |

#### Two Step - Don't have to hold down activation keys, similar to built-in snipper

- Activation will change cursor (and optionally dim/freeze screen)
- Selection is done with mouse button-drag-release

| **HotKey** | **Action** |
| ------ | ------ |
| Left Win-s | Initiates a snip and changes the cursor to a cross. |
| AppsKey | Initiates a snip and changes the cursor to a cross. I only use this to bind a macro to my mouse. |
| Left Win-z | Initiates a snip to copy to the clipboard (like built in capability). |
| Left Win-o | Initiates a OCR of the user selection |

### Other snipping actions

| **HotKey** | **Action** |
| ------ | ------ |
| Shift-PrtScn | To capture current window to the clipboard |
| Alt-PrtScn   | To create a snip of the current window |
| Ctrl-PrtScn  | To create a snip of the current monitor screen |
| Shift-Win-e  | To create a snip of the current clipboard (if it has a bitmap) |
| Shift-Win-c  | To copy the selection to the clipboard and create a snip of it (if it has a  bitmap). |

### While dragging the mouse to create a snip

- Holding shift key down forces a square capture
- Holding space bar or right mouse button down will allow shifting of the entire box as you draw it

### Hotkeys on Snip Surface

- Ctrl-C to copy the snip and shapes to the clipboard (minimizing snip)
- Ctrl-X to cut the snip and shapes to the clipboard (closing snip)
- Ctrl-S to save the snip and shapes to the a file in the Screenshot folder
- Ctrl-V on a snip will try to paste a new picture shape from the clipboard as
  either a bitmap (if available), or text (if btimap not available)
- Ctrl-D to clone the current snip if on snip background, if focus is on a
  shape, it will duplicate the shape
- Shift-Del will remove all the shapes on a Snip
- RMB Over Snip blank area should show a menu of various snip actions
- Ctrl-O on Snip to OCR the text to lines of text
- Ctrl-T on Snip to OCR the text to html table

## Drawing on snips

A number of drawing shapes are provided to cover most of the actions a user
would need. Press one of the keys below to change into a drawing mode, the
border color will change, also on Button Bar After drawing hitting the space bar
or deactivating the window will cancel drawing mode. These options are also
available from the Buttonbar described below.

| **HotKey** | **Action** |
| ------ | ------ |
|**Arrows**|**Arrowheads are draw at the finish point**|
|V |Creates an arrow from end to arrow-head|
|&#92;|Creates an tapered outline arrow from end to arrow-head|
|J |Creates a freehand outlined arc arrow (with slow bends)|
|A|Creates a single outlined arrow|
|/|Creates a double outline arrow|
|**Shapes**||
|L|Creates a line, shift to keep on an axis|
|R|Creates an outlined rectangle, shift to keep square|
|D|Creates an outlined rounded rectangle, shift to keep square|
|E|Creates an outlined ellipse/circle, shift to keep a circle|
|]|Creates an bracket/brace|
|**Free Hand**||
|P|Free-hand Pen in current color, shift to stay on an axis|
|M|Free-hand Wide (Marker) Pen in current color, shift to stay on an axis|
|**Highlighters**||
|H|Free-Hand Highlighter starting in yellow, shift to stay on an axis|
|Y|Yellow highlighter block|
|**Redaction-Obfuscation**||
|W|Free-Hand White Out Pen in background color, shift to stay on an axis|
|X|Block/Redact rectangle with color under the cursor|
|I|Pixelate Rectangle, good for text obscurring|
|B|Blur Rectangle, good for photo obscurring|
|G|Gaussian Blur Rectangle, good for text obscurring|
|**Tools**||
|C|Cropping rectangle to make the current snip smaller by cropping it to selection|
|O|OCR the selected area|
|**Text**|Text is entered via a dialog box allowing you set text options|
|N|Increasing Numbered tag on click, resets when no text shapes exist, Alt key for 2x size|
|T|Add Text to cursor location|
|U|Add Outline Text to cursor location|
|F|Add Text with adaptive translucent background to stand out from busy backgrounds|
|<|Text inside a left point arrow|
|>|Text inside a right point arrow|
|**Colors**||
|1-9|To set the drawing color of the next/active shape (also while drawing & resizing). Will also change text/shape color if shape has the focus rectangle.|

## Buttonbar - F1 to toggle while snip active

A toolbar will optionally appear above each active snip to provide a list of
actions grouped into the following. You can toggle the visibility of the toolbar
with the F1 key (while snip is active), or the leftmost X button. Left mouse
button on each button will select the shape to draw next. Right mouse button
will allow you to set options for each of the shapes via a menu. The buttonbar
is usefull in case you cannot remember the various shape shortcut keys.

- Arrow Shapes
- Line/Rectangle/Ellipse Shapes/Bracket
- Free pen shapes
- Highlighter shapes
- Redact/Obfuscate shapes
- Crop tool/OCR tool
- Text shapes (text/tag/fill/outline/arrow text)
- Color menu

### Drawing/Resizing Shapes

- Hold Alt key down when drawing shape to get a 2x a thicker border, pen/text size, blur amount on most shapes.
- When drawing or resizing, hold space bar to shift start location with mouse (except free hand).
- When drawing or resizing, press up/down arrow keys to increase/decrease line thickness for shapes with edges/lines/freehand path.
- When drawing free-hand shapes, Shift key will keep ling straigt until shift key is let go, this
  can be used to draw connected line segments, just stop at corner and quickly let go shift and press again
  before continuing.
- When drawing/resizing rectangles/ellipse, Shift key will keep the width/height the same (i.e. square).
- While in draw mode, single clicking on the snip surface (or Esc key) will change back to select mode.
- While in draw mode, holding Ctrl key and moving the mouse will move the snip. Release Ctrl to resume draw mode.
- While drawing line or arrow shapes, the shape will snap at 45° angles.

## Shape Notes

- When you hover over a shape a focus rectangle/line will appear over it
  allowing you to move or resize it (cursor will change to a resize). If you see
  a focus circle (lines) or rectangle (other shapes), you can click those points
  to resize the shape. All shapes/text can be resized.
- When the cursor changes to a hand you can click then move the shape around the
  snip.
- When the focus rectangle is active over a shape and you click on the shape,
  you may use the right mouse button to bring up a shape dependent menu or use
  shortcut keys to perform operations on the shape as described below

### Hotkeys on Snips

- Ctrl-Click or Ctrl-D on a shape to duplicate it

### Text / Graphics

- Hit T/U key over snip and dialog box to add text to current mouse location
- Hit ^P key over snip to select a graphic from a folder to add to current mouse location
- RMB on snip to select a graphic using the menu to current mouse location (other options as well)
Moving Shapes
- Grab one of the sides or object itself to move it around

### Deleting Shapes

- If focus rectangle is active and a shape is highlighted then Del key will remove it
- Alt-Click on a shape will remove it
- Shift-Del will remove all shapes on a ship
- RMB on Snip area, select Clear all Shapes in the menu selection Resizing Shapes
- Enabling focus rectangle will help locate edges and resize points
- Grab corner to resize in that direction, shift key maintains aspect ratio or square/linear
- While resizing shapes you can use up/down to change the line thickness (when it applies)

### Clicking on Shapes

- Alt-click to remove the object
- Ctrl-click to clone a copy of the object (except for free-hand shapes), also Ctrl-D on shape
- Click on a shape and the up/down arrow key will adjust border thickness, blur amount
- RMB on text will edit the text
- RMB over inserted picture will reset to original size
- RMB on other shapes will send them to the bottom of the stack / z-order
- OCR - Uses Descolas OCR engine to read text from snip
  - Read lines of text, with line breaks as shown
  - Read words of text, that only have line breaks at paragraph breaks, good of pasting
  - Read table text attempting to create an HTML table that represents each cell (with limitations)
  - Press O (or button-bar icon) to select a region of a snip to put the OCR on the clipboard

## Tray Menu options

- Initiate snip or clipboard selection
- OCR selection to clipboard (Words, Lines or Table)
- Freeze Screen Before Selection (toggle)
- Dim Screen When Selecting - similar to built in snipping tool (toggle)
- Show snip guides when selecting (toggle)
- Snips Always On Top (toggle)
- Confirm Snip Close if it has Shapes (toggle)
- Auto Update Clipboard on snip (toggle)
- Auto Save Clips to File on snip (toggle)
- Include Border With Clipboard Copy (toggle)
- Include Border With File Save (toggle)
- Open Saved Snips dialog to retrieve a prior saved snip or screenshot
- Edit Settings File to edit additional settings

## Configuration Files

- By default a coniguration file called SnipShape.ini is created in your local AppData folder.
- Most of the options are set via the Tray Menu and Button bar, however some options may be modified by editing the file.
  - [User Shape Color], custom colors 1 through 20
  - [User Snips], location of the cursor files, saved snip files and icon clips that are available from the snip menu.
    - **SnipPath** - default is user\Screenshots folder
    - **IconPath** - default is Stickies subfolder of installation, these are small images that can be pasted on a snip from the snip menu.
    - **CursorPath** - default is the Cursors subfolder of the installation, these are used as icons & cursors throughout the program
    - **Border Width** is used to draw borders on snips
    - **Vertical Sync on Select** - only needed on slow computers and over-stressed graphics cards, set to 1 to prevent choppy rectangles when drawing.
  - [User Button Bar], size of the button bar font/buttons
    - **Font Size** - increase or decrease to change the default size of the button bar icons (restart required)
  - The Tray menu allows you to edit these directly in Notepad.

## Limiations

- Saves snips with shapes are not later editable
- Undo has not been implemented
