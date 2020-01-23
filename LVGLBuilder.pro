# LVGL Builder
#
# MIT licence
# Copyright (c) 2016 Gábor Kiss-Vámosi, 2019 Fabian Hauser
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
# documentation files (the “Software”), to deal in the Software without restriction, including without limitation 
# the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
# and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
# THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
# TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#
# ------------------------------------------------------------
#
# FreeType
#
# Copyright (C) 2006-2019 by David Turner, Robert Wilhelm, and Werner Lemberg.
# See freetype/docs/LICENSE.TXT for the freetype licence

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = LVGLBuilder

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(lvgl/lvgl.pri)
include(freetype/freetype.pri)

RC_FILE = resources/icon.rc

MOC_DIR = build/moc
OBJECTS_DIR = build/obj
RCC_DIR = build/rcc
UI_DIR = build/ui

SOURCES += \
    LVGLCore.cpp \
    LVGLDialog.cpp \
    LVGLFontData.cpp \
    LVGLFontDialog.cpp \
    LVGLImageData.cpp \
    LVGLNewDialog.cpp \
    LVGLObject.cpp \
    LVGLObjectModel.cpp \
    LVGLProject.cpp \
    LVGLStyle.cpp \
    LVGLStyleModel.cpp \
    properties/LVGLPropertyAssignTextArea.cpp \
    properties/LVGLPropertyColor.cpp \
    properties/LVGLPropertyDate.cpp \
    properties/LVGLPropertyFlags.cpp \
    properties/LVGLPropertyGeometry.cpp \
    properties/LVGLPropertyImage.cpp \
    properties/LVGLPropertyList.cpp \
    properties/LVGLPropertyPoints.cpp \
    properties/LVGLPropertyRange.cpp \
    properties/LVGLPropertyScale.cpp \
    properties/LVGLPropertySeries.cpp \
    properties/LVGLPropertyTextList.cpp \
    widgets/LVGLBar.cpp \
    widgets/LVGLButton.cpp \
    widgets/LVGLButtonMatrix.cpp \
    widgets/LVGLCalendar.cpp \
    widgets/LVGLCanvas.cpp \
    widgets/LVGLChart.cpp \
    widgets/LVGLCheckBox.cpp \
    widgets/LVGLColorPicker.cpp \
    widgets/LVGLContainer.cpp \
    widgets/LVGLDropDownList.cpp \
    widgets/LVGLGauge.cpp \
    widgets/LVGLImage.cpp \
    LVGLItem.cpp \
    widgets/LVGLImageButton.cpp \
    widgets/LVGLImageSlider.cpp \
    widgets/LVGLKeyboard.cpp \
    widgets/LVGLLED.cpp \
    widgets/LVGLLabel.cpp \
    LVGLProperty.cpp \
    LVGLPropertyModel.cpp \
    LVGLSimulator.cpp \
    widgets/LVGLLine.cpp \
    widgets/LVGLList.cpp \
    widgets/LVGLPage.cpp \
    widgets/LVGLPreloader.cpp \
    widgets/LVGLSlider.cpp \
    widgets/LVGLSwitch.cpp \
    widgets/LVGLTabview.cpp \
    widgets/LVGLTextArea.cpp \
    widgets/LVGLWidget.cpp \
    LVGLWidgetModel.cpp \
    LVGLWorker.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    LVGLCore.h \
    LVGLDialog.h \
    LVGLFontData.h \
    LVGLFontDialog.h \
    LVGLImageData.h \
    LVGLNewDialog.h \
    LVGLObject.h \
    LVGLObjectModel.h \
    LVGLProject.h \
    LVGLStyle.h \
    LVGLStyleModel.h \
    properties/LVGLPropertyAssignTextArea.h \
    properties/LVGLPropertyColor.h \
    properties/LVGLPropertyDate.h \
    properties/LVGLPropertyFlags.h \
    properties/LVGLPropertyGeometry.h \
    properties/LVGLPropertyImage.h \
    properties/LVGLPropertyList.h \
    properties/LVGLPropertyPoints.h \
    properties/LVGLPropertyRange.h \
    properties/LVGLPropertyScale.h \
    properties/LVGLPropertySeries.h \
    properties/LVGLPropertyTextList.h \
    widgets/LVGLBar.h \
    widgets/LVGLButton.h \
    widgets/LVGLButtonMatrix.h \
    widgets/LVGLCalendar.h \
    widgets/LVGLCanvas.h \
    widgets/LVGLChart.h \
    widgets/LVGLCheckBox.h \
    widgets/LVGLColorPicker.h \
    widgets/LVGLContainer.h \
    widgets/LVGLDropDownList.h \
    widgets/LVGLGauge.h \
    widgets/LVGLImage.h \
    LVGLItem.h \
    widgets/LVGLImageButton.h \
    widgets/LVGLImageSlider.h \
    widgets/LVGLKeyboard.h \
    widgets/LVGLLED.h \
    widgets/LVGLLabel.h \
    LVGLProperty.h \
    LVGLPropertyModel.h \
    LVGLSimulator.h \
    widgets/LVGLLine.h \
    widgets/LVGLList.h \
    widgets/LVGLPage.h \
    widgets/LVGLPreloader.h \
    widgets/LVGLSlider.h \
    widgets/LVGLSwitch.h \
    widgets/LVGLTabview.h \
    widgets/LVGLTextArea.h \
    widgets/LVGLWidget.h \
    LVGLWidgetModel.h \
    LVGLWorker.h \
    MainWindow.h \
    widgets/LVGLWidgets.h

FORMS += \
    LVGLFontDialog.ui \
    LVGLNewDialog.ui \
    MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources/res.qrc
