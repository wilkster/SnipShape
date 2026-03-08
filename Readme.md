# SnipShape Screen Capture and Annotation Tool

**SnipShape** is a lightweight screen-capture utility written in
**AutoHotkey 2.1**. It provides an alternative to the Windows Snipping Tool while
using fewer system resources.

The tool captures a screen region and immediately displays it as an **overlay
GUI positioned directly over the captured area**. This overlay can be edited in
place using annotation and shape tools, allowing you to quickly highlight or
mark up the captured content without opening additional windows.

The application is based on the AutoHotkey **Snipper** concept by keeping the
editing surface directly on the screen rather than moving the image to a
separate editor or clipboard. Powerful annotation tools are provided to further
markup the snip.

- [SnipShape Screen Capture and Annotation Tool](#snipshape-screen-capture-and-annotation-tool)
  - [Credits](#credits)
  - [Requirements](#requirements)
  - [Installation](#installation)
  - [Snipping Overview](#snipping-overview)
    - [Snip Annotation Examples](#snip-annotation-examples)
    - [One step capture - hold down activation keys](#one-step-capture---hold-down-activation-keys)
    - [Two Step capture - Hotkey to initiate snipping, the click-drag to select, similar to built-in snipper](#two-step-capture---hotkey-to-initiate-snipping-the-click-drag-to-select-similar-to-built-in-snipper)
    - [While dragging the mouse to create a snip](#while-dragging-the-mouse-to-create-a-snip)
    - [Other snipping actions that don't use the mouse](#other-snipping-actions-that-dont-use-the-mouse)
    - [Hotkeys on Snip Surface](#hotkeys-on-snip-surface)
  - [Toolbar - F1 to toggle while snip active](#toolbar---f1-to-toggle-while-snip-active)
    - [Show or hide the toolbar](#show-or-hide-the-toolbar)
    - [Use toolbar buttons](#use-toolbar-buttons)
    - [Toolbar behavior](#toolbar-behavior)
  - [Drawing on snips](#drawing-on-snips)
    - [Snip Hotkeys to create shapes](#snip-hotkeys-to-create-shapes)
  - [Arrow shapes](#arrow-shapes)
  - [Shape tools](#shape-tools)
  - [Freehand drawing](#freehand-drawing)
  - [Highlighters](#highlighters)
  - [Redaction and obscuring](#redaction-and-obscuring)
  - [Text tools - Text is entered via a dialog box allowing you set text options](#text-tools---text-is-entered-via-a-dialog-box-allowing-you-set-text-options)
  - [Colors](#colors)
  - [Snip Tools](#snip-tools)
    - [Toolbar Menus to create shapes](#toolbar-menus-to-create-shapes)
      - [Arrow Shapes](#arrow-shapes-1)
      - [Line/Connector/Rectangle/Ellipse Shapes/Bracket](#lineconnectorrectangleellipse-shapesbracket)
      - [Free pen shapes](#free-pen-shapes)
      - [Highlighter shapes](#highlighter-shapes)
      - [Redact/Obfuscate shapes](#redactobfuscate-shapes)
      - [Text shapes (text/tag/fill/outline/arrow text), Text entered via a dialog box.](#text-shapes-texttagfilloutlinearrow-text-text-entered-via-a-dialog-box)
      - [Tool menu](#tool-menu)
      - [Color menu](#color-menu)
    - [Drawing/Resizing Shapes](#drawingresizing-shapes)
  - [Hovering and Focus Rectangle](#hovering-and-focus-rectangle)
    - [Hotkeys on Shapes](#hotkeys-on-shapes)
    - [Text / Graphics](#text--graphics)
    - [Deleting Shapes](#deleting-shapes)
    - [Clicking on Shapes](#clicking-on-shapes)
  - [Tray Menu options](#tray-menu-options)
  - [Configuration Files](#configuration-files)
  - [Limitations/Future](#limitationsfuture)

## Credits

Fanatic Guru / SKAN / iseahound / Descolada plus
the work of dozens of people inspired this script.
Additional contributors are referenced in the source code.

| Script          | Link                                                            |
| --------------- | --------------------------------------------------------------- |
| Screen Clipping | <https://www.autohotkey.com/boards/viewtopic.php?f=6&t=12088>   |
| GDIp            | <https://www.autohotkey.com/boards/viewtopic.php?t=72011>       |
| Snipper         | <https://www.autohotkey.com/boards/viewtopic.php?f=83&t=115622> |
| UWP OCR Library | <https://github.com/Descolada/OCR>                              |

## Requirements

**Autohotkey 2.1 Alpha 14** or later (some 2.1 features are used including **structures**)

## Installation

1. Clone or download a zip of the repository to a local folder.
2. Extract the files to a local folder
3. If you have ahk2 associated with Autohotkey 2.1 alpha, then just click on the
`SnipShape.ahk2` file in the base folder. You may rename the SnipShape.ahk2
extension to SnipShape.ahk or something else if you are using a different file
association. Do not rename the other **Authotkey** files as they are referenced in
include files.

A tray icon like this ![image](https://github.com/user-attachments/assets/cefc4ca7-0a56-4161-82e4-4cd6d2e19176) will appear. You may have to un-hide it in the tray if it is initially hidden by Windows.

- **Left-click**: Opens the SnipShape menu
- **Right-click**: Opens the default AutoHotkey menu

Additional tray icon options are shown in [Tray Menu Options](#tray-menu-options).

## Snipping Overview

### Snip Annotation Examples

|![image](https://github.com/user-attachments/assets/fd92e254-630b-4a71-8e19-38bf83763307)|
|![image](https://github.com/user-attachments/assets/5735443f-1795-4545-ac45-db7e03526897)|

SnipShape supports **two methods for capturing a screen region**.

### One step capture - hold down activation keys

Current key assignments for screen selection (can be modified in the `SnipShape.ini` file)

| **Global HotKey** | **Action**                                                                                                                                                      |
| ----------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Ctrl-Win          | Then drag selection to create a snip                                                                                                                            |
| Alt-Win           | Then drag a selection to copy to the clipboard directly.<br>You can click the tray message to edit the snip if desired or directly paste into your application. |

### Two Step capture - Hotkey to initiate snipping, the click-drag to select, similar to built-in snipper

- Activation will change cursor (and optionally dim/freeze screen)
- Selection is done with mouse button-drag-release

| **Global HotKey** | **Action**                                                            |
| ----------------- | --------------------------------------------------------------------- |
| Left Win-s        | Initiates a snip and changes the cursor to a cross.                   |
| Left Win-z        | Initiates a snip to copy to the clipboard (like built in capability). |
| Left Win-o        | Initiates a OCR of the user selection                                 |

### While dragging the mouse to create a snip

- Holding `shift` key down forces a square capture
- Holding `space` bar or right mouse button down will allow shifting of the entire box as you draw it
- `Esc` key will abort the snipping operation
- If the selected area is very small the snip will be ignored.

### Other snipping actions that don't use the mouse

| **Global HotKey** | **Action**                                                                            |
| ----------------- | ------------------------------------------------------------------------------------- |
| Shift-PrtScn      | To capture current window to the clipboard                                            |
| Alt-PrtScn        | To create a snip of the current window                                                |
| Ctrl-PrtScn       | To create a snip of the current monitor screen                                        |
| Shift-Win-e       | To create a snip of the current clipboard (if it has a bitmap)                        |
| Shift-Win-c       | To copy the selection to the clipboard and create a snip of it (if it has a  bitmap). |

### Hotkeys on Snip Surface

Right-click over Snip blank area should show a menu of various snip actions. Below are some additional hotkeys that work on the active snip surface.

One option is to paste in a small picture from the Stickies subfolder of the distribution. You can add/remove your own bitmaps in the Stickies folder created during installation.

| **Snip HotKey**          | **Snip Action**                                                                                                          |
| ------------------------ | ------------------------------------------------------------------------------------------------------------------------ |
| Ctrl-C                   | Copy the snip and shapes to the clipboard (minimizing snip)                                                              |
| Ctrl-X                   | Cut the snip and shapes to the clipboard (closing snip)                                                                  |
| Ctrl-S                   | Save the snip and shapes to a file in the Screenshot folder                                                              |
| Ctrl-V                   | Try to paste a new picture shape from the clipboard as either a bitmap (if available), or text (if bitmap not available) |
| Ctrl-P                   | Open a dialog box to add a picture from a file as a new shape                                                            |
| Ctrl-D                   | Clone the current snip if on snip background, if focus is on a shape, it will duplicate the shape                        |
| Del                      | will remove all the shapes on a Snip                                                                                     |
| Shift-Del                | will remove all the shapes on a Snip                                                                                     |
| Ctrl-O                   | OCR the text to lines of text                                                                                            |
| Ctrl-T                   | OCR the text to html table                                                                                               |
| Ctrl-I                   | Invert the Snip Colors                                                                                                   |
| Ctrl-B                   | Black and White the Snip Colors                                                                                          |
| Ctrl-G                   | Gray Scale the Snip Colors                                                                                               |
| Ctrl-R                   | Restore the Snip bitmap                                                                                                  |
| Ctrl-Down                | Minimize all Snips                                                                                                       |
| **Snip HotKey**          | **Scaling Action**                                                                                                       |
| =                        | Duplicate and 2x the size of the current snip (without shapes)                                                           |
| Alt=                     | Duplicate and 2.5x the size of the current snip (without shapes)                                                         |
| Ctrl-=                   | Duplicate and 3x the size of the current snip (without shapes)                                                           |
| -                        | Duplicate and 1/2x the size of the current snip (without shapes)                                                         |
| RMB (Right Mouse Button) | Will bring up a menu with additional options for the snip                                                                |

## Toolbar - F1 to toggle while snip active

![image](https://github.com/user-attachments/assets/9738fdbe-cc70-45f7-9743-932304beec63)

A toolbar can appear above each active snip to provide quick access to common actions and annotation tools.

### Show or hide the toolbar

- Press `F1` while a snip is active to toggle the toolbar on or off.
- Click the **X** button on the left side of the toolbar to close it.

### Use toolbar buttons

Each toolbar button represents a shape or tool.

- Left-click a button to select the shape or tool to draw next.
- Right-click a button to open a menu with configuration options for that shape.

The toolbar is helpful if you prefer selecting tools visually instead of using keyboard shortcuts.

### Toolbar behavior

- The toolbar automatically moves to the currently active snip.
- If focus changes to a non-snip window, the toolbar hides automatically.
- When focus returns to a snip, the toolbar reappears.

## Drawing on snips

SnipShape provides a variety of annotation tools.

You can activate drawing modes by:

- pressing a **hotkey**
- selecting a tool from the **toolbar**

While drawing, the cursor changes to the selected shape and the snip border indicates the active color.

### Snip Hotkeys to create shapes

## Arrow shapes

| Key   | Action                                                  |
| ----- | ------------------------------------------------------- |
| V     | Creates an arrow from end to arrow-head                 |
| A     | Creates a single outlined arrow                         |
| &#92; | Creates an tapered outline arrow from end to arrow-head |
| J     | Creates a freehand outlined arc arrow (with slow bends) |
| /     | Creates a double outline arrow                          |

## Shape tools

| Key | Action                                                                            |
| --- | --------------------------------------------------------------------------------- |
| L   | Creates a line, shift to keep on an axis                                          |
| R   | Creates an outlined rectangle, shift to keep square                               |
| D   | Creates an outlined rounded rectangle, shift to keep square                       |
| E   | Creates an outlined ellipse/circle, shift to keep a circle, draws from center out |
| [   | Creates a connector line (vert/horz segments only)                                |
| ]   | Creates a bracket/brace                                                           |

## Freehand drawing

| Key | Action                                                                 |
| --- | ---------------------------------------------------------------------- |
| P   | Free-hand Pen in current color, shift to stay on an axis               |
| M   | Free-hand Wide (Marker) Pen in current color, shift to stay on an axis |

## Highlighters

| Key | Action                                                             |
| --- | ------------------------------------------------------------------ |
| H   | Free-Hand Highlighter starting in yellow, shift to stay on an axis |
| Y   | Yellow highlighter block                                           |

## Redaction and obscuring

| Key | Action                                                                |
| --- | --------------------------------------------------------------------- |
| W   | Free-Hand White Out Pen in background color, shift to stay on an axis |
| S   | Free-Hand Smudge Pen to blur underneath, shift to stay on axis        |
| X   | Block/Redact rectangle with color under the cursor                    |
| I   | Pixelate Rectangle, good for text obscuring                           |
| B   | Blur Rectangle, good for photo obscuring                              |
| G   | Gaussian Blur Rectangle, good for text obscuring                      |

## Text tools - Text is entered via a dialog box allowing you set text options

| Key        | Action                                                                                   |
| ---------- | ---------------------------------------------------------------------------------------- |
| N          | Increasing Numbered tag on click, resets when no text shapes exist, Alt key for 2x size  |
| T          | Add Text to cursor location                                                              |
| U          | Add Outline Text to cursor location                                                      |
| F          | Add Text with adaptive translucent background to stand out from busy backgrounds         |
| <          | Text inside a left point arrow                                                           |
| >          | Text inside a right point arrow                                                          |

## Colors

| Key        | Action                                                                                   |
| ---------- | ---------------------------------------------------------------------------------------- |
| 1-9        | To set the drawing color of the next/active shape (also while drawing & resizing). Will also change text/shape color if shape has the focus rectangle. |

## Snip Tools

| Key | Action                                                                          |
| --- | ------------------------------------------------------------------------------- |
| C   | Cropping rectangle to make the current snip smaller by cropping it to selection |
| O   | OCR the selected area                                                           |
| K   | Slice out the selected area of Snip                                             |
| Q   | Extend the snip using the background color under the selection                  |

### Toolbar Menus to create shapes

#### Arrow Shapes

<img width="200" alt="image" src="https://github.com/user-attachments/assets/59ed9a88-ea3b-4612-b9cf-43feddea85b8"/>

#### Line/Connector/Rectangle/Ellipse Shapes/Bracket

<img width="200" alt="image" src="https://github.com/user-attachments/assets/75659e76-b3df-449b-89da-4b475b7fdee5"/>

#### Free pen shapes

<img width="200" alt="image" src="https://github.com/user-attachments/assets/c49524e1-227b-44f9-9916-aa8157d11b46"/>

#### Highlighter shapes

<img width="200" alt="image" src="https://github.com/user-attachments/assets/b86dd264-367f-41db-94ae-c72ef5dc9270"/>

#### Redact/Obfuscate shapes

<img width="200" alt="image" src="https://github.com/user-attachments/assets/cbb2445b-5e4b-4e3c-8875-3b81ed634db2"/>

#### Text shapes (text/tag/fill/outline/arrow text), Text entered via a dialog box.

<img width="200" alt="image" src="https://github.com/user-attachments/assets/72a71a6f-5e66-471c-86d7-32678318f93d"/>

#### Tool menu

<img width="200" alt="image" src="https://github.com/user-attachments/assets/9d2c7488-3bc6-436b-9ed1-3dec837eabba"/>

#### Color menu

<img width="200" alt="image" src="https://github.com/user-attachments/assets/97f64679-f106-48a0-8c6e-cc46c0344220"/>

### Drawing/Resizing Shapes

- Extra padding is provided outside the snip surface to allow shapes to extend beyond the snip boundary.
  Once drawn, only the portion within the snip will be shown (adjustable in `SnipShape.ini`).
- While in draw mode:
  - Single-click on the snip surface (or press Esc) to switch back to select mode.
  - Hold Space and move the mouse to pan the snip; release Space to resume drawing.
- When drawing or resizing:
  - Hold Space to shift the start location with the mouse (except freehand).
  - Press Up/Down arrow keys to increase/decrease line thickness for shapes with edges, lines, or freehand paths.
  - Hold Alt for a 2x thicker border, pen/text size, or blur amount on most shapes.
- When drawing rectangles or ellipses, hold Shift to constrain width/height equally (i.e., square/circle).
- When drawing or resizing lines or arrows, the shape snaps at 45° angles or to nearby line endpoints.
- When drawing freehand shapes:
  - Hold Shift to keep the line straight; release to resume freehand. Use this to draw connected line segments
    by pausing at each corner, releasing Shift, then pressing it again before continuing.
  - If you end the path near its starting point, the path will automatically close itself.

## Hovering and Focus Rectangle

<table>
   <tr>
      <td align="center"><img width="75%" alt="image" src="https://github.com/user-attachments/assets/c210a753-5c76-45e0-9754-aece56219494"/></td>
      <td align="center"><img width="75%" alt="image" src="https://github.com/user-attachments/assets/c1b2f31a-0c17-43dd-8fee-4bc3beef80f1"/></td>
   </tr>
   <tr>
      <td align="center">Body Shapes</td>
      <td align="center">Line Shapes</td>
   </tr>
</table>

- When you hover over a shape, a focus rectangle appears around it. This
indicates the shape can be moved, resized, or adjusted. The cursor will change
to a resize icon or hand icon depending on where you hover.
- Resize a shape by clicking and dragging one of the resize handles (shown with
the resize cursor). All shapes and text objects can be resized this way.
- Move a shape by clicking and dragging the focus rectangle when the hand cursor is displayed.
- Use the arrow keys (Up, Down, Left, Right) to move the shape without using the
mouse. Movement snaps to the grid.
- Use the mouse scroll wheel to adjust properties of the selected shape:
  - Line thickness for drawn shapes
  - Text size for text shapes
  - Blur or pixelation level for obscure shapes
- When a shape has an active focus rectangle, right-click to open a context menu
with options specific to that shape. You can also use keyboard shortcuts to
perform operations on the shape. Available options vary by shape type.

While hovering over a shape, you can also use the hotkeys listed below to perform additional actions.

### Hotkeys on Shapes

| **Shape HotKey**     | **Action**                              |
| -------------------- | --------------------------------------- |
| Ctrl-Click<br>Ctrl-D | Duplicate the shape                     |
| Del                  | Remove the shape                        |
| 1 to 9               | Apply the associated color to the shape |

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

<img width="300" alt="image" src="https://github.com/user-attachments/assets/5f01e6b9-d382-43d4-89cf-f2237b70b69a"/>

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
- Most of the options are set via the Tray Menu and Toolbar, however some options may only be modified by editing the file. Restart is required after changing these. You can direct edit the SnipShape.ini file from the tray icon menu `Edit Settings File`.
  - `[User Shape Color]`, custom colors 1 through 20. Named colors or hex RGG (e.g 0xCE1126) are allowed.
  - `[User Snips]`, location of the cursor files, saved snip files and icon clips that are available from the snip menu.
    - `SnipPath` - default is user\Pictures\Screenshots folder
    - `IconPath` - default is Stickies subfolder of installation, these are small images that can be pasted on a snip from the snip menu.
    - `CursorPath` - default is the Cursors subfolder of the installation, these are used as icons & cursors throughout the program.
    - `Border Width` is used to adjust width of snip borders. Fractional sizes are allowed. Default is 2.0.
    - `Vertical Sync on Select` - Set to 1 to prevent artifacts rectangles when drawing. Only needed if they appear on your system. Default is 0.
  - `[User Shapes]`, Shape related settings.
    - `ScaleFreehand` - Set to 1 to also scale line width when scaling a line based shape. Default is 0 for no scaling.
    - `Padding` - Amount of padding in pixels outside the snip to allow shapes to be drawn. Default is 450 pixels.
  - `[User Button Bar]`, size of the button bar font/buttons
    - `Font Size` - increase or decrease to change the default size of the Toolbar icons. Default is 8.0. Fractional sizes are allowed.
  - The Tray menu allows you to edit these directly in Notepad.
- Optional Keycode Bindings, these are defaults so not shown unless you add them. Notification menu select Edit Settings File to add them

- Below are the various hotkeys and their default values.

| `[Hotkey]`                       | Description                                                                                         |
| -------------------------------- | --------------------------------------------------------------------------------------------------- |
| `ClipActiveWindow=+PrintScreen`  | Copy the active window to the clipboard.                                                            |
| `SnipActiveWindow=!PrintScreen`  | Create a snip of the active window.                                                                 |
| `SnipActiveMonitor=^PrintScreen` | Create a snip of the active screen                                                                  |
| `EditClipboard=+#e`              | Create a snip of the clipboard image (if one exists)                                                |
| `SnipAndEditClipboard=+#c`       | If there is an active selection, copy it to the clipboard and create a snip of it (graphics only)   |
| `ShowHideAllSnips=!F12`          | Toggle view state of any active snip windows (minimize/restore)                                     |
| `SnipArea2Step1=<#S`             | Start the snip an area of the screen using two step method                                          |
| `ClipArea2Step=<#Z`              | Start the clipboard capture of an area of the screen using the two step method                      |
| `OCRArea2Step1=<#O`              | Start an OCR of an area of the screen using the two step method                                     |
| `SnipArea1Step1=^#Lbutton`       | Start a snip of the screen using the one step method (keep hotkey down while dragging)              |
| `ClipArea1Step1=!#Lbutton`       | Start a clipboard capture of the screen using the one step method (keep hotkey down while dragging) |

## Limitations/Future

- Shapes on saved snips are merged with snip when saved, and no longer editable. (future idea)
- Undo/Redo for shapes has not been implemented
