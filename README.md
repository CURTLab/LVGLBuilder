# 1.0.4 preview

**Version 1.0.4 is expected to be released in late July.**

## Support exporting all pages

![savebehavior](https://raw.githubusercontent.com/mrQzs/PICSTORE/main/doc/savebehavior.png)

## Support a single file to save all pages

![exportbehavior](https://raw.githubusercontent.com/mrQzs/PICSTORE/main/doc/exportbehavior.png)

## Added Color format option

* The loaded picture does not need to manually set the conversion format  **(PS: The loading of gif images will be supported in the future)**
* ![colordepth1](https://raw.githubusercontent.com/mrQzs/PICSTORE/main/doc/colordepth1.png)
* ![colordepth2](https://raw.githubusercontent.com/mrQzs/PICSTORE/main/doc/colordepth2.png)

## Support different themes

- Colorful style（LOL）：**You can quickly modify the style of the specified part according to these different colors**

  ![badstyle](https://raw.githubusercontent.com/mrQzs/PICSTORE/main/doc/newstyle2.gif)

- Dark theme

  ![newstyle](https://raw.githubusercontent.com/mrQzs/PICSTORE/main/doc/newstyle.png)

- The theme is realized by loading the configuration file, so **you can create your favorite style themes.**

## Complete testing process

- Many test cases will be added under the test file, and **the program will be released after all passed**

## Support multiple language

![setlanguage](https://raw.githubusercontent.com/mrQzs/PICSTORE/main/doc/language.gif)

- Now supports Chinese and English

- Automatically save language settings, no need to set the next restart

- **Switch language first, then drag widgets**

- **The name translation of the style needs to restart the client**

- **Only projects of the same language can be loaded, so projects of versions before 1.0.4 are only applicable to English.**

  Because the original design needs to be changed a lot, this will cause a lot of problems.

- Support customization, you can add languages according to the following pictures

- ![setlanguage](https://raw.githubusercontent.com/mrQzs/PICSTORE/main/doc/setlanguage.gif)

  

## Google Breakpad
The next version 1.0.4 version starts to support google breakpad
- Currently only used for window
- **If the program crashes, please check whether the dmp file is generated. If it exists, you can send it to me for debugging and tracking**
- If the release version is to generate a pdb file, please uncomment these two lines of code in the pro file
  QMAKE_CXXFLAGS_RELEASE += $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
  QMAKE_LFLAGS_RELEASE += $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO

# New features
The pre-release of v1.0.3 has been released. 
Changelog.md under the dev-v7 branch records the changes of the previous version.

## 1.0.3

- LABEL/TEXTAREA support LVGL's SYMBOL,just enter (LV_S_XXX) in the text box, such as (LV_S_WIFI) 
  ![SupportSyb](https://raw.githubusercontent.com/mrQzs/PICSTORE/main/doc/supportsyb.png)

- Some widgets have new functions, such as animation switch and type selection

- Fixed the issue of compiling errors caused by using a custom page name

- New export method, hierarchical export will classify pictures, fonts and c codes, and then classify them into different page folders 【**Experimental features**】

  ![epway](https://raw.githubusercontent.com/mrQzs/PICSTORE/main/doc/epway.gif)

- Improve the naming logic when creating a new page or event

- Optimize the export logic, will not copy the same font file repeatedly

- Fix that the image event function fails to compile when the image name contains uppercase letters

## Fixed crash bugs
These bugs exist in release versions before 1.0.3
- If the value_str attribute of the button is set or a configuration file containing this attribute is loaded, it will sometimes cause the program to crash.
- If the current page contains a page switching event, and the target page of the page switching event is closed, exporting the code at this time will cause the program to crash.

### Old Version

- The **changlog.md** of the dev-v7 branch records all the new features 

# New Test Tool

[**For Test**]arm-none-linux-gnueabi-i686-mingw32-2014 **on window** https://github.com/mrQzs/CreateLvglLib/releases

## Export LVGL Code Build
- Need to select hierarchical export 
- The cc path of Makefile originally needed to be modified, but this time the path of armgcc was added to the environment variable

program: https://github.com/mrQzs/LCBuild

![lbcode](https://raw.githubusercontent.com/mrQzs/PICSTORE/main/doc/lbcode.gif)

## Create LVGL LIB

- Select the output device and input device file of the corresponding platform

- lv_drv_conf.h should also enable the corresponding input device and output device

- Both lvgl and drive are the latest official v7 version

  This project modified the source code of lvgl to achieve some functions (such as tabwidth adjustment), so the official lvgl can be replaced by the lvgl of this project.

  ![tabwidth](https://raw.githubusercontent.com/mrQzs/PICSTORE/main/doc/tabwidth.gif)

program: https://github.com/mrQzs/CreateLvglLib

![createlib](https://raw.githubusercontent.com/mrQzs/PICSTORE/main/doc/createlib.gif)

## LVGL Simulator

program: https://github.com/mrQzs/Lvgl_Simulator
* support msvc 32/64 and mingw 32/64
* Put the code file generated by lvglbuilder in the testcode folder
* Just load the source code and modify the resolution in the lvconf.h

  ![lvglsim](https://raw.githubusercontent.com/mrQzs/PICSTORE/main/doc/lvglsim.gif)

# LVGLBuilder
 GUI Builder for littlevgl release 6.0+ written in C++ and using LVGL for preview of the created GUI and Qt for the higher-level user interface for drag and drop of widgets as well as editing of properties and styles. 

![Preview](https://raw.githubusercontent.com/CURTLab/LVGLBuilder/master/doc/preview.png)

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
