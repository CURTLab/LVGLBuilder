# New Todo List
- [ ] Add Event

- [ ] Add Animation

- [ ] Dialog for changing keyboard layout


# LVGLBuilder

 GUI Builder for littlevgl release 7.0+ written in C++ and using LVGL for preview of the created GUI and Qt for the higher-level user interface for drag and drop of widgets as well as editing of properties and styles. 

![Preview](https://raw.githubusercontent.com/CURTLab/LVGLBuilder/dev-v7/preview.png)



# Qt Setting
window
![setting](https://raw.githubusercontent.com/CURTLab/LVGLBuilder/dev-v7/QtSetting.png)
ubuntu
![ubuntuSetting](https://user-images.githubusercontent.com/37052865/116019312-85701900-a676-11eb-8e70-45df873e94e1.jpg)
maxos
![macSetting](https://user-images.githubusercontent.com/37052865/116173418-23c7b180-a73f-11eb-9e53-daf3b777263b.jpg)
![macSetting2](https://user-images.githubusercontent.com/37052865/116173424-24f8de80-a73f-11eb-894e-d1a6b15601a9.jpg)




### Prerequisites

* [Qt 5.11+](https://www.qt.io/) - The Qt framework
* C++11
* LVGL 7.11  (included in source since a few adaptions are applied)

Tested:
 * Windows 10 + MSVC 2019 + Qt 5.15.1
 * Windows 10 + MinGW 8.1 + Qt 5.12.9
 * Ubuntu 18.04.4 + Qt 5.14.2
 * Window 10 + MSVC 2015 32/64 + Qt 5.12.10
 * Window 10 + MSVC 2017 32/64 + Qt 5.12.10

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
 * Support keyboard shortcuts: Ctrl+F new file; Ctrl+W load file; Ctrl+E export c code;1 to page1; 2 to page2; etc...

### ToDo General
- [x] Saving GUI as JSON
- [x] Loading GUI as JSON
- [x] Export C-project
- [x] Change style of single widgets
- [x] Support different display formats
- [x] Fix object inspector (insertion and deletion of widgets)
- [x] Fix absolute and relative positions of widgets
- [x] Undo and redo
- [x] Default values for widgets
- [x] Support multiple screens
- [x] Preview for widgets
- [x] Icons for widgets
- [x] Support for lvgl 7.x

### Wishlist
- [ ] C code driver generator (Touch, Buttons, Displays for µC)

### ToDo Widgets
- [x] Arc (lv_arc)
- [x] Bar (lv_bar)
- [x] Button (lv_btn)
- [x] Button matrix (lv_btnm)
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
- [x] Page (lv_page)
- [x] Preloader (lv_preload)
- [x] Roller (lv_roller)
- [x] Slider (lv_slider)
- [x] Spinbox (lv_spinbox)
- [x] Switch (lv_sw)
- [ ] Table (lv_table)
- [x] Tabview (lv_tabview)
- [x] Text area (lv_ta)
- [ ] Tile view (lv_tileview)
- [ ] Window (lv_win)
