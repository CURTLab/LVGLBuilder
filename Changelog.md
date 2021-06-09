# Change Log

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

## 1.0.2 

 - Auto-save function only keeps three stored files per page
 - Support event function modification
 - Completed all event functions
 - Add test cases for all events
 - Support for reading relative path files (The configuration file and the picture need to be in the same directory)
 
  ![ev2](https://raw.githubusercontent.com/mrQzs/PICSTORE/main/doc/Event2.gif)
  ![ev3](https://raw.githubusercontent.com/mrQzs/PICSTORE/main/doc/Event3.gif)

## 1.0.1
 - Added auto save file function
 - When the simulation stops running, widgets are restored to their original state immediately
 - Added btn, checkbox, ddlist, img, imgbtn and label event functions
 - undo/redo supports multiple pages
 - Label adds the event function of dynamically displaying time
    ![timedes](https://raw.githubusercontent.com/CURTLab/LVGLBuilder/master/doc/timedes.png)

## older

### Pre Release

- Windows: builded by msvc201564 and msvc201732
- Macos: builder by Clang64
- ubuntu20: builder by gcc8_64

![lvglsim](https://raw.githubusercontent.com/CURTLab/LVGLBuilder/master/doc/pre-release.png)

### New version
Now, the dev-7 branch based on lvgl7.11.0 has completed most of the todo list, so everyone can switch to the dev-7 branch for testing.

![Preview1](https://raw.githubusercontent.com/CURTLab/LVGLBuilder/master/doc/preview1.png)
![Preview2](https://raw.githubusercontent.com/CURTLab/LVGLBuilder/master/doc/preview2.png)
![Preview3](https://raw.githubusercontent.com/CURTLab/LVGLBuilder/master/doc/preview3.png)

