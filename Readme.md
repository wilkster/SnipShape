# SnipShape Screen Capture and Annotation Tool

AutoHotkey 2.1 alternate to Windows Snipping Tool with additional features and
lower system impact. Creates a overlay Gui of Snipped area that can be
manipulated onscreen with shapes and annotation tools added to it. Based on
Autohotkey Snipper tool that places the snip GUI directly over the snipped area vs
the more complex methods of using external windows or retrieving from the
clipboard. Small buttonbar is available to assist with annotation and other tools.

- [SnipShape Screen Capture and Annotation Tool](#snipshape-screen-capture-and-annotation-tool)
  - [Credits](#credits)
  - [Requirements](#requirements)
  - [Installation](#installation)
  - [Snipping Overview](#snipping-overview)
    - [Snip Annotation Examples](#snip-annotation-examples)
    - [One step - hold down activation keys](#one-step---hold-down-activation-keys)
    - [Two Step - Don't have to hold down activation keys, similar to built-in snipper](#two-step---dont-have-to-hold-down-activation-keys-similar-to-built-in-snipper)
    - [While dragging the mouse to create a snip](#while-dragging-the-mouse-to-create-a-snip)
    - [Other snipping actions that don't use the mouse](#other-snipping-actions-that-dont-use-the-mouse)
    - [Hotkeys on Snip Surface](#hotkeys-on-snip-surface)
  - [Buttonbar - F1 to toggle while snip active](#buttonbar---f1-to-toggle-while-snip-active)
  - [Drawing on snips](#drawing-on-snips)
    - [Hotkeys to create shapes](#hotkeys-to-create-shapes)
    - [Buttonbar Menus to create shapes](#buttonbar-menus-to-create-shapes)
    - [Drawing/Resizing Shapes](#drawingresizing-shapes)
  - [Shape Notes](#shape-notes)
    - [Hotkeys on Shapes](#hotkeys-on-shapes)
    - [Text / Graphics](#text--graphics)
    - [Deleting Shapes](#deleting-shapes)
    - [Clicking on Shapes](#clicking-on-shapes)
  - [Tray Menu options](#tray-menu-options)
  - [Configuration Files](#configuration-files)
  - [Limitations](#limitations)

## Credits

Fanatic Guru / SKAN / iseahound / Descolada plus
the work of dozens of people inspired this script.
Many more of them sited/listed in the source code:

| Script | Link |
| ------ | ------ |
|Screen Clipping|<https://www.autohotkey.com/boards/viewtopic.php?f=6&t=12088>|
|GDIp|<https://www.autohotkey.com/boards/viewtopic.php?t=72011>|
|Snipper|<https://www.autohotkey.com/boards/viewtopic.php?f=83&t=115622>|
|UWP OCR Library|<https://github.com/Descolada/OCR>|

## Requirements

Autohotkey 2.1 Alpha 14 or later (some 2.1 features are used including structures)

## Installation

Clone or download a zip of the repository to a local folder. If you have ahk2 associated with
Autohotkey 2.1 alpha then just click on the **SnipShape.ahk2** file in the base
folder. You may rename the SnipShape.ahk2 extension to something else if you are
using a different file association.

A tray icon like this ![image](https://github.com/user-attachments/assets/cefc4ca7-0a56-4161-82e4-4cd6d2e19176) appear. You may have to un-hide it in the tray.
You can click on the tray icon to bring up some actions or use hotkeys as described below. The right-mouse-button brings up the default Autohotkey menu.
Additional tray icon options are shown in [Tray Menu Options](#tray-menu-options).

## Snipping Overview

### Snip Annotation Examples

|![image](https://github.com/user-attachments/assets/fd92e254-630b-4a71-8e19-38bf83763307)|
|![image](https://github.com/user-attachments/assets/5735443f-1795-4545-ac45-db7e03526897)|

There are two ways to initiate a Snip. The One step method where you press a hotkey and click-drag the mouse. The other is a two step
method (similar to windows snipping) where you initiate the snip with one hotkey then later click-drag the mouse. Both are described below.

### One step - hold down activation keys

Current key assignments for screen selection (can be modified in the **SnipShape.ahk2** file)

| **Global HotKey** | **Action** |
| ------ | ------ |
| Ctrl-Win | Then drag selection to create a snip |
| Alt-Win  | Then drag a selection to copy to the clipboard directly.<br>You can click the tray message to edit the snip if desired or directly paste into your application. |

### Two Step - Don't have to hold down activation keys, similar to built-in snipper

- Activation will change cursor (and optionally dim/freeze screen)
- Selection is done with mouse button-drag-release

| **Global HotKey** | **Action** |
| ------ | ------ |
| Left Win-s | Initiates a snip and changes the cursor to a cross. |
| AppsKey | Initiates a snip and changes the cursor to a cross. I only use this to bind a macro to my mouse. <br>You may want to comment this one out if you otherwise use the AppsKey.|
| Left Win-z | Initiates a snip to copy to the clipboard (like built in capability). |
| Left Win-o | Initiates a OCR of the user selection |

### While dragging the mouse to create a snip

- Holding shift key down forces a square capture
- Holding space bar or right mouse button down will allow shifting of the entire box as you draw it
- Esc key will abort the snipping operation
- If the selected area is very small the snip will be ignored.

### Other snipping actions that don't use the mouse

| **Global HotKey** | **Action** |
| ------ | ------ |
| Shift-PrtScn | To capture current window to the clipboard |
| Alt-PrtScn   | To create a snip of the current window |
| Ctrl-PrtScn  | To create a snip of the current monitor screen |
| Shift-Win-e  | To create a snip of the current clipboard (if it has a bitmap) |
| Shift-Win-c  | To copy the selection to the clipboard and create a snip of it (if it has a  bitmap). |

### Hotkeys on Snip Surface

Right-mouse-button click over Snip blank area should show a menu of various snip actions. Below are some additional hotkeys that work on the active snip surface.
One option is to paste in a small picture from the Stickies subfolder of the distribution. You can add/remove your own bitmaps here.

| **Snip HotKey** | **Snip Action** |
| ------ | ------ |
| Ctrl-C |Copy the snip and shapes to the clipboard (minimizing snip)|
| Ctrl-X |Cut the snip and shapes to the clipboard (closing snip)|
| Ctrl-S |Save the snip and shapes to the a file in the Screenshot folder|
| Ctrl-V |Try to paste a new picture shape from the clipboard as either a bitmap (if available), or text (if bitmap not available)|
| Ctrl-P |Open a dialog box to add a picture from a file as a new shape|
| Ctrl-D |Clone the current snip if on snip background, if focus is on a shape, it will duplicate the shape|
| Del |will remove all the shapes on a Snip|
| Shift-Del |will remove all the shapes on a Snip|
| Ctrl-O | OCR the text to lines of text|
| Ctrl-T | OCR the text to html table|
| Ctrl-Down|Minimize all Snips|
| **Snip HotKey** | **Scaling Action** |
|=     | Duplicate and 2x the size of the current snip (without shapes)|
|Alt=  | Duplicate and 2.5x the size of the current snip (without shapes)|
|Ctrl-=| Duplicate and 3x the size of the current snip (without shapes)|
|-     | Duplicate and 1/2x the size of the current snip (without shapes)|
|RMB (Right Mouse Button)|Will bring up a menu with additional options for the snip|

## Buttonbar - F1 to toggle while snip active

![image](https://github.com/user-attachments/assets/9738fdbe-cc70-45f7-9743-932304beec63)

A toolbar will optionally appear above each active snip to provide a list of
actions grouped into the following. You can toggle the visibility of the toolbar
with the F1 key (while snip is active), or the leftmost X button. Left mouse
button on each button will select the shape to draw next. Right mouse button
will allow you to set options for each of the shapes via a menu. The buttonbar
is useful in case you cannot remember the various shape shortcut keys.

When the focus changes to a different Snip, the button bar will jump to that Snip. If the
focus shifts to another non-SnipShape window then the button bar will disappear.

## Drawing on snips

A number of drawing shapes are provided to cover most of the annotations a user
would need. Use the Buttonbar menus or press one of the keys below to change
into a drawing mode. The cursor will change to the shape you are drawing and
snip border color will change to the active color. After drawing pressing the
space bar or Esc key will cancel drawing mode. These
options are also available from the Buttonbar described below.

### Hotkeys to create shapes

| **Snip HotKey** | **Action** |
| ------ | ------ |
|**Arrows**|**Arrowheads are draw at the finish point**|
|V |Creates an arrow from end to arrow-head|
|&#92;|Creates an tapered outline arrow from end to arrow-head|
|J |Creates a freehand outlined arc arrow (with slow bends)|
|A|Creates a single outlined arrow|
|/|Creates a double outline arrow|
|**Shapes**||
|L|Creates a line, shift to keep on an axis|
|[|Creates a connector line (vert/horz segments only)|
|R|Creates an outlined rectangle, shift to keep square|
|D|Creates an outlined rounded rectangle, shift to keep square|
|E|Creates an outlined ellipse/circle, shift to keep a circle|
|[|Connector to join two areas|
|]|Creates an bracket/brace|
|**Free Hand**||
|P|Free-hand Pen in current color, shift to stay on an axis|
|M|Free-hand Wide (Marker) Pen in current color, shift to stay on an axis|
|**Highlighters**||
|H|Free-Hand Highlighter starting in yellow, shift to stay on an axis|
|Y|Yellow highlighter block|
|**Redaction-Obfuscation**||
|W|Free-Hand White Out Pen in background color, shift to stay on an axis|
|S|Free-Hand Smudge Pen to blur underneath, shift to stay on axis|
|X|Block/Redact rectangle with color under the cursor|
|I|Pixelate Rectangle, good for text obscuring|
|B|Blur Rectangle, good for photo obscuring|
|G|Gaussian Blur Rectangle, good for text obscuring|
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

### Buttonbar Menus to create shapes

- Arrow Shapes

![image](https://github.com/user-attachments/assets/59ed9a88-ea3b-4612-b9cf-43feddea85b8)

- Line/Connector/Rectangle/Ellipse Shapes/Bracket

![image](https://github.com/user-attachments/assets/75659e76-b3df-449b-89da-4b475b7fdee5)

- Free pen shapes

![image](https://github.com/user-attachments/assets/c49524e1-227b-44f9-9916-aa8157d11b46)

- Highlighter shapes

![image](https://github.com/user-attachments/assets/b86dd264-367f-41db-94ae-c72ef5dc9270)

- Redact/Obfuscate shapes

![image](https://github.com/user-attachments/assets/cbb2445b-5e4b-4e3c-8875-3b81ed634db2)

- Text shapes (text/tag/fill/outline/arrow text), Text entered via a dialog box.

![image](https://github.com/user-attachments/assets/72a71a6f-5e66-471c-86d7-32678318f93d)

- Crop tool/OCR tool

![image](https://github.com/user-attachments/assets/357bf8ca-aaad-4d69-ab2e-02bbf337079d)

- Color menu

![image](https://github.com/user-attachments/assets/97f64679-f106-48a0-8c6e-cc46c0344220)

### Drawing/Resizing Shapes

- Extra padding is provided outside the snip surface to allow shapes to extend beyond the snip. Once drawn
  only the portion enclosed within the snip will be shown.
- Hold Alt key down when drawing shape to get a 2x a thicker border, pen/text size, blur amount on most shapes.
- When drawing or resizing, hold space bar to shift start location with mouse (except free hand).
- When drawing or resizing, press up/down arrow keys to increase/decrease line thickness for shapes with edges/lines/freehand path.
- When drawing free-hand shapes, Shift key will keep ling straight until shift key is let go, this
  can be used to draw connected line segments, just stop at corner and quickly let go shift and press again
  before continuing.
- When drawing/resizing rectangles/ellipse, Shift key will keep the width/height the same (i.e. square).
- While in draw mode, single clicking on the snip surface (or Esc key) will change back to select mode.
- While in draw mode, holding Ctrl key and moving the mouse will move the snip. Release Ctrl to resume draw mode.
- While drawing line or arrow shapes, the shape will snap at 45° angles.
- When drawing freehand Pen shapes, if you stop the path near the start of the path, the path will enclose itself as if a circle was intended.

## Shape Notes

- When you hover over a shape a focus rectangle/line will appear over it
  allowing you to move or resize it (cursor will change to a resize). If you see
  small focus circle (lines) or rectangle (other shapes), you can click those points
  to resize the shape. All shapes/text can be resized in this manner.
- While hovering over a focus rectangle/line when the cursor changes to a hand you can click, then move/drag the shape around the
  snip.
- When drawing or moving line based shapes, they will snap to a focus point if they are near it.  
- When the focus rectangle is active over a shape, and you click on the shape,
  you may use the right mouse button to bring up a shape dependent menu or use
  shortcut keys to perform operations on the shape as described below
  
Focus Rectangle Example

![image](https://github.com/user-attachments/assets/de6827c9-e247-4a72-95a4-7650a996e9d9)

### Hotkeys on Shapes

| **Snip HotKey** | **Action** |
| ------ | ------ |
|Ctrl-Click<br>Ctrl-D|Duplicate the shape|
|Del | Remove the shape|
|1 to 9| Apply the associated color to the shape|

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
- OCR - Uses Descolada's OCR engine to read text from snip
  - Read lines of text, with line breaks as shown
  - Read words of text, that only have line breaks at paragraph breaks, good of pasting
  - Read table text attempting to create an HTML table that represents each cell (with limitations)
  - Press O (or button-bar icon) to select a region of a snip to put the OCR on the clipboard

## Tray Menu options

Left clicking on the SnipShape Tray Icon brings up the following choices

![image](https://github.com/user-attachments/assets/5f01e6b9-d382-43d4-89cf-f2237b70b69a)

- Initiate snip or clipboard selection
- OCR selection to clipboard (Words, Lines or Table)
- Freeze Screen Before Selection (toggle)
- Dim Screen When Selecting - similar to built in snipping tool (toggle)
- Show snip guides when selecting (toggle)
- Snips Always On Top (toggle)
- Confirm Snip Close if it has Shapes (toggle)
- Auto Update Clipboard on snip (toggle)
- Auto Save clips to the screenshot folder on snip (toggle)
- Include Border With Clipboard Copy (toggle)
- Include Border With File Save (toggle)
- Open Saved Snips dialog to retrieve a prior saved snip or screenshot
- Edit Settings File in Notepad to edit additional settings

## Configuration Files

- By default a configuration file called SnipShape.ini is created in your local AppData folder.
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
- Optional Keycode Bindings, these are defaults so not shown unless you add them. Notification menu select Edit Settings File to add them

[Hotkey]  
ClipActiveWindow=+PrintScreen  
SnipActiveWindow=!PrintScreen  
SnipActiveMonitor=^PrintScreen  
EditClipboard=+#e  
SnipAndEditClipboard=+#c  
ShowHideAllSnips=!F12  
SnipArea2Step1=<#S  
ClipArea2Step=<#Z  
OCRArea2Step1=<#O  
SnipArea1Step1=^#Lbutton  
ClipArea1Step1=!#Lbutton  

## Limitations

- Saves snips with shapes are not later editable
- Undo has not been implemented
