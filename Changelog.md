# Change Log

## 1.0.3

- LABEL/TEXTAREA support LVGL's SYMBOL,just enter (LV_S_XXX) in the text box, such as (LV_S_WIFI) 
- Some widgets have new functions, such as animation switch and type selection
- Fixed the issue of compiling errors caused by using a custom page name
- New export method, hierarchical export will classify pictures, fonts and c codes, and then classify them into different page folders

## 1.0.2 

 - Auto-save function only keeps three stored files per page
 - Support event function modification
 - Completed all event functions
 - Add test cases for all events
 - Support for reading relative path files (The configuration file and the picture need to be in the same directory)

## 1.0.1
 - Added auto save file function
 - When the simulation stops running, widgets are restored to their original state immediately
 - Added btn, checkbox, ddlist, img, imgbtn and label event functions
 - undo/redo supports multiple pages
 - Label adds the event function of dynamically displaying time
    ![timedes](https://raw.githubusercontent.com/CURTLab/LVGLBuilder/master/doc/timedes.png)
