# SnipShape Screen Capture and Annotation Tool

AutoHotkey 2.1 alternate to Windows Snipping Tool with additional features and lower system impact.
Creates Gui of Snip that can be manipulated onscreen with shapes and annotation tools added to it.

## Credits

Credits:
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

## Snipping Overview

### One step - hold down activation keys

Current key assignments for screen selection (can be modified in the SnipShape.ahk2 file)

| HotKey | Action |
| ------ | ------ |
| Ctrl-Win | Then drag selection to create a snip |
| Alt-Win  | Then drag a selection to copy to the clipboard directly |

#### Two Step - Don't have to hold down activation keys, similar to built-in snipper

- Activation will change cursor (and optionally dim/freeze screen)
- Selection is done with mouse button-drag-release

| HotKey | Action |
| ------ | ------ |
| AppsKey | Initiates a snip and changes the cursor to indicate. |
| Left Win-s | Initiates a snip and changes the cursor to indicate. |
| Left Win-z | Initiates a snip to copy to the clipboard (like built in capability). |
| Left Win-o | Initiates a OCR of the user selection |

### Other snipping actions

| HotKey | Action |
| ------ | ------ |
| Shift-PrtScn | To capture current window to the clipboard |
| Alt-PrtScn   | To create a snip of the current window |
| Ctrl-PrtScn  | To create a snip of the current monitor screen |
| Shift-Win-e  | To create a snip of the current clipboard (if it has a bitmap) |
| Shift-Win-c  | To copy the selection to the clipboard and create a snip of it (if it has a  bitmap). |

### While dragging the mouse to create a snip

- Holding shift key down forces a square capture
- Holding space bar down will allow shifting of the entire box as you draw it
- Holding right mouse button down will allow shifting of the entire box as you draw it

### Hotkeys on Snip Surface

- Hold Alt key down when drawing shape to get a 2x a thicker border or pen/text size on most shapes
- When drawing or resizing, hold space bar to shift start location with mouse (except free hand)
- When drawing or resizing, press up/down arrow keys to increase/decrease line thickness for shapes with edges/lines
- When drawing free-hand shapes, Shift key will keep ling straigt until shift key is let go, this
  can be used to draw connected line segments, just stop at corner and quickly let go shift and press again
  before continuing.
- When drawing/resizing rectangles/ellipse, Shift key will keep the width/height the same (i.e. square)
- While in draw mode, single clicking on the snip surface (or Esc key) will change back to select mode
- While in draw mode, holding Ctrl key and moving the mouse will move the snip. Release Ctrl to resume draw mode.

## Buttonbar - F1 to toggle while snip active

A toolbar will optionally appear above each active snip to provide a list of actions grouped into the following. You can toggle the visibility of the toolbar with the F1 key (while snip is active), or the leftmost X button. Left mouse button on each button will select the shape to draw next. Right mouse button will allow you to set options for each of the shapes via a menu. The buttonbar is usefull in case you cannot remember the various shape shortcut keys.

- Arrow Shapes
- Line/Rectangle/Ellipse Shapes/Bracket
- Free pen shapes
- Highlighter shapes
- Redact/Obfuscate shapes
- Crop tool/OCR tool
- Text shapes (text/tag/fill/outline/arrow text)
- Color menu

## Drawing on snips

A number of drawing shapes are provided to cover most of the actions a user would need.
Press one of the keys below to change into a drawing mode, the border color will change, also on Button Bar After drawing hitting the space bar or deactivating the window will cancel drawing mode

| HotKey | Action |
| ------ | ------ |
|Arrows||
|V |Creates an arrow from end to arrow-head|
|&#92;|Creates an tapered outline arrow from end to arrow-head|
|J |Creates a freehand arc arrow (with slow bends)|
|A|Creates an outlined arrow from end to arrow-head|
|/|Creates a double outline arrow|
|Shapes||
|L|Creates a line, shift to keep on an axis|
|R|Creates an outlined rectangle, shift to keep square|
|D|Creates an outlined rounded rectangle, shift to keep square|
|E|Creates an outlined ellipse/circle, shift to keep a circle|
|]|Creates an bracket/brace|
|Free Hand||
|P|Free-hand Pen in current color, shift to stay on an axis|
|M|Free-hand Wide (Marker) Pen in current color, shift to stay on an axis|
|Highlighters||
|H|Free-Hand Highlighter starting in yellow, shift to stay on an axis|
|Y|Yellow highlighter block|
|Redaction-Obfuscation||
|W|Free-Hand White Out Pen in background color, shift to stay on an axis|
|X|Block/Redact rectangle with color under the cursor|
|I|Pixelate Rectangle, good for text obscurring|
|B|Blur Rectangle, good for photo obscurring|
|G|Gaussian Blur Rectangle, good for text obscurring|
|Tools||
|C|Cropping rectangle to make the current snip smaller by cropping it to selection|
|O|OCR the selected area|
|Text||
|N|Increasing Numbered tag on click, resets when no text shapes exist, Alt key for 2x size|
|T|Add Text to cursor location|
|U|Add Outline Text to cursor location|
|F|Add Text with adaptive translucent background to stand out from busy backgrounds|
|<|Text inside a left point arrow|
|>|Text inside a right point arrow|
|Colors||
|1-9|To set the drawing color of the next/active shape (also while drawing & resizing). Will also change text/shape color if shape has the focus rectangle.|

### Shape Notes

- When you hover over a shape a focus rectangle/line will appear over it allowing you to move or resize it.
- When the focus rectangle is active over a shape you may use the right mouse button to bring up a shape dependent menu or use shortcut keys to perform operations on the shape as described below

## Hotkeys on Shapes

- Ctrl-C to copy the snip and shapes to the clipboard (minimizing snip)
- Ctrl-S to save the snip and shapes to the a file
- Ctrl-X to cut the snip and shapes to the clipboard (closing snip)
- Ctrl-V on a snip will paste a new picture shape from the clipboard as either a bitmap (if available),
  or text (if available)
- Ctrl-D to clone the current snip if on snip background, if focus is on a shape, it will clone the shape
- RMB Over Snip blank area should show a menu of various snip actions
- Ctrl-O on Snip to OCR the text to lines of text
- Ctrl-T on Snip to OCR the text to html table

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
- RMB on Snip area, select Clear all Shapes in the menu selection
Resizing Shapes
- Enabling focus rectangle will help locate edges and resize points
- Grab corner to resize in that direction, shift key maintains aspect ratio or square/linear
- While resizing shapes you can use up/down to change the line thickness (when it applies)

### Clicking on Shapes

- Alt-click to remove the object
- Ctrl-click to clone a copy of the object (except for free-hand shapes), also Ctrl-D on shape
- RMB on text will edit the text
- RMB over inserted picture will reset to original size
- RMB on other shapes will send them to the bottom of the stack / z-order
- OCR - Uses Descolas OCR engine to read text from snip
  - Read lines of text, with line breaks as shown
  - Read words of text, that only have line breaks at paragraph breaks, good of pasting
  - Read table text attempting to create an HTML table that represents each cell (with limitations)
  - Press O (or button-bar icon) to select a region of a snip to put the OCR on the clipboard

### Other

- Shift-Del will remove all the shapes on a Snip

### Tray Menu options

- Initiate snip or clipboard selection
- OCR selection to clipboard (Words, Lines or Table)
- Freeze Screen Before Selection (toggle)
- Dim Screen When Selecting - similar to built in snipping tool (toggle)
- Show snip guides when selecting (toggle)
- Snips Always On Top (toggle)
- Confirm Snip Close if it has Shapes (toggle)
- Auto Update Clipboard on snip (toggle)
- Auto Save Clips to File (toggle)
- Include Border With Clipboard Copy (toggle)
- Include Border With File Save (toggle)
- Open Saved Snips to retrieve a prior saved snip
- Edit Settings File to edit additional settings
