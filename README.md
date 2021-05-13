# Widget Style Select

* Switch & slider Style

![swsldsyle](https://raw.githubusercontent.com/CURTLab/LVGLBuilder/master/sw_sld_style.gif)

# Event System

* support sceen screen
* support arc event

![event1](https://raw.githubusercontent.com/CURTLab/LVGLBuilder/master/event1.gif)


# New version
Now, the dev-7 branch based on lvgl7.11.0 has completed most of the todo list, so everyone can switch to the dev-7 branch for testing.

![Preview1](https://raw.githubusercontent.com/CURTLab/LVGLBuilder/master/preview1.png)
![Preview2](https://raw.githubusercontent.com/CURTLab/LVGLBuilder/master/preview2.png)
![Preview3](https://raw.githubusercontent.com/CURTLab/LVGLBuilder/master/preview3.png)

# LVGLBuilder
 GUI Builder for littlevgl release 6.0+ written in C++ and using LVGL for preview of the created GUI and Qt for the higher-level user interface for drag and drop of widgets as well as editing of properties and styles. 

![Preview](https://raw.githubusercontent.com/CURTLab/LVGLBuilder/master/preview.png)

### Prerequisites
* [Qt 5.11+](https://www.qt.io/) - The Qt framework
* C++11
* LVGL 6.1 (included in source since a few adaptions are applied)

Tested:
 * Windows 10 + MSVC 2019 + Qt 5.15.1
 * Windows 10 + MinGW 8.1 + Qt 5.15.2
 * Ubuntu 18.04.4 + Qt 5.14.2
 * Ubuntu 20.04 + Qt 5.12.8

### Build
Building with console:
* Move to the cloned LVGLBuild directory
* Run ```qmake LVGLBuilder.pro```
* For gcc (Mac/Linux): ```make```
* For mingw: ```mingw32-make```
* For windows: ```jom``` (make sure that jom.exe is in the environment path)

Building with QtCreator:
* Download Qt from https://www.qt.io/download-open-source
* Install Qt with QtCreator
* Open the LVGLBuilder.pro file with QtCreator
* Click build project
* There are no special requironments, therefor it should build with QtCreator out of the box. 

### Usage
 * Create a new project and select the screen resolution
 * Simply drag and drop widgets from the list into the canvas
 * Widgets can be moved with the arrow keys or by dragging
 * Lock widgets to prevent editing (Unlock via right click context menu)
 * Drop a widget into another widget to set the parent
 * Save and load a project as json file using the menu or tool buttons
 * Mark widgets within the property editor as accessable via the c header
 * Export the project as c code using the menu (File -> Export -> C Project ...)

### ToDo General
- [x] Saving GUI as JSON
- [x] Loading GUI as JSON
- [x] Export C-project
- [x] Change style of single widgets
- [x] Support different display formats
- [x] Fix object inspector (insertion and deletion of widgets)
- [ ] Fix absolute and relative positions of widgets
- [ ] Undo and redo
- [ ] Default values for widgets
- [ ] Support multiple screens
- [ ] Dialog for changing keyboard layout
- [x] Preview for widgets
- [ ] Icons for widgets
- [ ] Support for lvgl 7.x

### Wishlist
- [ ] C code driver generator (Touch, Buttons, Displays for µC)

### ToDo Widgets
- [x] Arc (lv_arc)
- [x] Bar (lv_bar)
- [x] Button (lv_btn)
- [ ] Button matrix (lv_btnm)
- [x] Calendar (lv_calendar)
- [ ] Canvas (lv_canvas)
- [x] Checkbox (lv_cb)
- [x] Chart (lv_chart)
- [x] Container (lv_cont)
- [x] Color picker (lv_cpicker)
- [x] Drop-down list (lv_ddlist)
- [x] Gauge (lv_gauge)
- [x] Image (lv_img)
- [x] Image button (lv_imgbtn)
- [x] Keyboard (lv_kb)
- [x] Label (lv_label)
- [x] LED (lv_led)
- [x] Line (lv_line)
- [x] List (lv_list)
- [x] Line meter (lv_lmeter)
- [x] Message box (lv_mbox)
- [ ] Page (lv_page)
- [x] Preloader (lv_preload)
- [ ] Roller (lv_roller)
- [x] Slider (lv_slider)
- [ ] Spinbox (lv_spinbox)
- [x] Switch (lv_sw)
- [ ] Table (lv_table)
- [x] Tabview (lv_tabview)
- [x] Text area (lv_ta)
- [ ] Tile view (lv_tileview)
- [ ] Window (lv_win)