# LVGL Builder
#
# MIT licence
# Copyright (c) 2016 G??bor Kiss-V??mosi, 2019 Fabian Hauser
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
# documentation files (the a??Softwarea??), to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
# and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED a??AS ISa??, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
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

greaterThan(QT_MAJOR_VERSION,4){
        TARGET_ARCH=$${QT_ARCH}
}else{
        TARGET_ARCH=$${QMAKE_HOST.arch}
}
contains(TARGET_ARCH, x86_64){
    CONFIG(debug, debug|release){
        MOC_DIR = build/x64/moc/debug
        OBJECTS_DIR = build/x64/obj/debug
        RCC_DIR = build/x64/rcc/debug
        UI_DIR = build/x64/ui/debug
        DESTDIR = bin/x64/debug
    }else{
        MOC_DIR = build/x64/moc/release
        OBJECTS_DIR = build/x64/obj/release
        RCC_DIR = build/x64/rcc/release
        UI_DIR = build/x64/ui/release
        DESTDIR = bin/x64/release
    }
}else{

    CONFIG(debug, debug|release){
        MOC_DIR = build/win32/moc/debug
        OBJECTS_DIR = build/win32/obj/debug
        RCC_DIR = build/win32/rcc/debug
        UI_DIR = build/win32/ui/debug
        DESTDIR = bin/win32/debug
    }else{
        MOC_DIR = build/win32/moc/release
        OBJECTS_DIR = build/win32/obj/release
        RCC_DIR = build/win32/rcc/release
        UI_DIR = build/win32/ui/release
        DESTDIR = bin/win32/release
    }
}

#CONFIG(debug,debug|release){
#    INCLUDEPATH += C:/vld/include
#    DEPENDPATH += C:/vld/include
#    LIBS += -LC:/vld/lib/Win64 -lvld
#}


SOURCES += \
    core/ColorPick.cpp \
    core/LVGLCommands.cpp \
    core/LVGLCore.cpp \
    core/LVGLDialog.cpp \
    core/LVGLExportThread.cpp \
    core/LVGLFontData.cpp \
    core/LVGLFontDialog.cpp \
    core/LVGLHelper.cpp \
    core/LVGLImageData.cpp \
    core/LVGLItem.cpp \
    core/LVGLLog.cpp \
    core/LVGLNewDialog.cpp \
    core/LVGLObject.cpp \
    core/LVGLObjectModel.cpp \
    core/LVGLProcessBar.cpp \
    core/LVGLProject.cpp \
    core/LVGLProperty.cpp \
    core/LVGLPropertyModel.cpp \
    core/LVGLSimulator.cpp \
    core/LVGLStyle.cpp \
    core/LVGLStyleModel.cpp \
    core/LVGLTabWidget.cpp \
    core/LVGLWidgetListView.cpp \
    core/LVGLWidgetModel.cpp \
    core/LVGLWidgetModelDisplay.cpp \
    core/LVGLWidgetModelInput.cpp \
    core/LVGLWorker.cpp \
    core/ListDelegate.cpp \
    core/ListViewItem.cpp \
    events/EventSelectWIdget.cpp \
    events/EventSettingWidgeet.cpp \
    events/EventsListWIdget.cpp \
    events/LVGLEvent.cpp \
    events/LVGLEventAnim.cpp \
    events/LVGLEventArc.cpp \
    events/LVGLEventBar.cpp \
    events/LVGLEventButton.cpp \
    events/LVGLEventCheckBox.cpp \
    events/LVGLEventDDList.cpp \
    events/LVGLEventImage.cpp \
    events/LVGLEventImageButton.cpp \
    events/LVGLEventLabel.cpp \
    events/LVGLEventLed.cpp \
    events/LVGLEventLine.cpp \
    events/LVGLEventRoller.cpp \
    events/LVGLEventScreen.cpp \
    events/LVGLEventSlider.cpp \
    events/LVGLEventSpinbox.cpp \
    events/LVGLEventSwitch.cpp \
    events/LVGLEventTextArea.cpp \
    events/LVGLEventType.cpp \
    events/LVGLEventWidgetBasic.cpp \
    events/LVGLPropertyEvent.cpp \
    properties/LVGLPropertyAnyFunc.cpp \
    properties/LVGLPropertyAssignTextArea.cpp \
    properties/LVGLPropertyColor.cpp \
    properties/LVGLPropertyDate.cpp \
    properties/LVGLPropertyDateList.cpp \
    properties/LVGLPropertyFlags.cpp \
    properties/LVGLPropertyGeometry.cpp \
    properties/LVGLPropertyImage.cpp \
    properties/LVGLPropertyList.cpp \
    properties/LVGLPropertyPoints.cpp \
    properties/LVGLPropertyRange.cpp \
    properties/LVGLPropertyScale.cpp \
    properties/LVGLPropertySeries.cpp \
    properties/LVGLPropertyTextList.cpp \
    properties/LVGLPropertyVal2.cpp \
    widgets/LVGLArc.cpp \
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
    widgets/LVGLImageButton.cpp \
    widgets/LVGLKeyboard.cpp \
    widgets/LVGLLED.cpp \
    widgets/LVGLLabel.cpp \
    widgets/LVGLLine.cpp \
    widgets/LVGLLineMeter.cpp \
    widgets/LVGLList.cpp \
    widgets/LVGLMessageBox.cpp \
    widgets/LVGLObjectMask.cpp \
    widgets/LVGLPage.cpp \
    widgets/LVGLRoller.cpp \
    widgets/LVGLSlider.cpp \
    widgets/LVGLSpinbox.cpp \
    widgets/LVGLSpinner.cpp \
    widgets/LVGLSwitch.cpp \
    widgets/LVGLTable.cpp \
    widgets/LVGLTabview.cpp \
    widgets/LVGLTextArea.cpp \
    widgets/LVGLTileView.cpp \
    widgets/LVGLWidget.cpp \
    widgets/LVGLWindow.cpp \
    main.cpp \
    MainWindow.cpp \

HEADERS += \
    core/ColorPick.h \
    core/LVGLCommands.h \
    core/LVGLCore.h \
    core/LVGLDialog.h \
    core/LVGLExportThread.h \
    core/LVGLFontData.h \
    core/LVGLFontDialog.h \
    core/LVGLHelper.h \
    core/LVGLImageData.h \
    core/LVGLItem.h \
    core/LVGLLog.h \
    core/LVGLNewDialog.h \
    core/LVGLObject.h \
    core/LVGLObjectModel.h \
    core/LVGLProcessBar.h \
    core/LVGLProject.h \
    core/LVGLProperty.h \
    core/LVGLPropertyModel.h \
    core/LVGLSimulator.h \
    core/LVGLStyle.h \
    core/LVGLStyleModel.h \
    core/LVGLTabWidget.h \
    core/LVGLWidgetListView.h \
    core/LVGLWidgetModel.h \
    core/LVGLWidgetModelDisplay.h \
    core/LVGLWidgetModelInput.h \
    core/LVGLWorker.h \
    core/ListDelegate.h \
    core/ListViewItem.h \
    events/EventSelectWIdget.h \
    events/EventSettingWidgeet.h \
    events/EventsListWIdget.h \
    events/LVGLEvent.h \
    events/LVGLEventAnim.h \
    events/LVGLEventArc.h \
    events/LVGLEventBar.h \
    events/LVGLEventButton.h \
    events/LVGLEventCheckBox.h \
    events/LVGLEventDDList.h \
    events/LVGLEventImage.h \
    events/LVGLEventImageButton.h \
    events/LVGLEventLabel.h \
    events/LVGLEventLed.h \
    events/LVGLEventLine.h \
    events/LVGLEventRoller.h \
    events/LVGLEventScreen.h \
    events/LVGLEventSlider.h \
    events/LVGLEventSpinbox.h \
    events/LVGLEventSwitch.h \
    events/LVGLEventTextArea.h \
    events/LVGLEventType.h \
    events/LVGLEventWidgetBasic.h \
    events/LVGLEventWidgts.h \
    events/LVGLPropertyEvent.h \
    properties/LVGLPropertyAnyFunc.h \
    properties/LVGLPropertyAssignTextArea.h \
    properties/LVGLPropertyColor.h \
    properties/LVGLPropertyDate.h \
    properties/LVGLPropertyDateList.h \
    properties/LVGLPropertyFlags.h \
    properties/LVGLPropertyGeometry.h \
    properties/LVGLPropertyImage.h \
    properties/LVGLPropertyList.h \
    properties/LVGLPropertyPoints.h \
    properties/LVGLPropertyRange.h \
    properties/LVGLPropertyScale.h \
    properties/LVGLPropertySeries.h \
    properties/LVGLPropertyTextList.h \
    properties/LVGLPropertyVal2.h \
    widgets/LVGLArc.h \
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
    widgets/LVGLImageButton.h \
    widgets/LVGLKeyboard.h \
    widgets/LVGLLED.h \
    widgets/LVGLLabel.h \
    widgets/LVGLLine.h \
    widgets/LVGLLineMeter.h \
    widgets/LVGLList.h \
    widgets/LVGLMessageBox.h \
    widgets/LVGLObjectMask.h \
    widgets/LVGLPage.h \
    widgets/LVGLRoller.h \
    widgets/LVGLSlider.h \
    widgets/LVGLSpinbox.h \
    widgets/LVGLSpinner.h \
    widgets/LVGLSwitch.h \
    widgets/LVGLTable.h \
    widgets/LVGLTabview.h \
    widgets/LVGLTextArea.h \
    widgets/LVGLTileView.h \
    widgets/LVGLWidget.h \
    widgets/LVGLWidgets.h \
    widgets/LVGLWindow.h \
    MainWindow.h \

FORMS += \
    MainWindow.ui \
    core/ColorPick.ui \
    core/LVGLFontDialog.ui \
    core/LVGLNewDialog.ui \
    core/LVGLProcessBar.ui \
    core/ListViewItem.ui \
    events/EventSelectWIdget.ui \
    events/EventSettingWidgeet.ui \
    events/EventsListWIdget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources/res.qrc
