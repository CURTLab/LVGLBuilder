# LVGLBuilder
 GUI Builder for littlevgl written in C++ and using LVGL for preview of the created GUI and Qt for the higher-level user interface for drag and drop of widgets as well as editing of properties and styles. 

![Preview](https://raw.githubusercontent.com/CURTLab/LVGLBuilder/master/preview.png)

### Prerequisites
* [Qt 5.11+](https://www.qt.io/) - The Qt framework
* C++11
* LVGL 6.1 (included in source since a few adaptions are applied)

Tested on Windows 10 with MSVC 2017 & Qt 5.13.1

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

### ToDo General
- [x] Saving GUI as JSON
- [x] Loading GUI as JSON
- [x] Export C-project
- [x] Change style of single widgets
- [ ] Default values for widgets
- [ ] Preview for widgets
- [ ] Icons for widgets
- [ ] Support different display formats
- [ ] Support multiple screens
- [ ] Fix object inspector (insertion and deletion of widgets)

### ToDo Widgets
- [ ] Arc (lv_arc)
- [x] Bar (lv_bar)
- [x] Button (lv_btn)
- [ ] Button matrix (lv_btnm)
- [x] Calendar (lv_calendar)
- [ ] Canvas (lv_canvas)
- [x] Checkbox (lv_cb)
- [ ] Chart (lv_chart)
- [x] Container (lv_cont)
- [x] Color picker (lv_cpicker)
- [x] Drop-down list (lv_ddlist)
- [x] Gauge (lv_gauge)
- [x] Image (lv_img)
- [x] Image button (lv_imgbtn)
- [x] Keyboard (lv_kb)
- [x] Label (lv_label)
- [ ] LED (lv_led)
- [x] Line (lv_line)
- [x] List (lv_list)
- [ ] Line meter (lv_lmeter)
- [ ] Message box (lv_mbox)
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
