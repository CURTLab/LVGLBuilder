#include "LVGLEventType.h"

#include "MainWindow.h"
#include "core/LVGLCore.h"
#include "core/LVGLHelper.h"
#include "core/LVGLObject.h"

LVGLEventType::LVGLEventType(int type)
    : m_type(type), m_wtype(0), m_role(0), m_needCustomeValue(true) {
  init();
}

LVGLEventType::LVGLEventType(int type, int wtype)
    : m_type(type), m_wtype(wtype), m_role(0), m_needCustomeValue(true) {
  init();
}

LVGLEventType::LVGLEventType(int type, int wtype, int Role)
    : m_type(type), m_wtype(wtype), m_role(Role), m_needCustomeValue(true) {
  init();
}

void LVGLEventType::init() {
  if (m_type == 1) {
    // screen name
    auto tabw = LVGLHelper::getInstance().getMainW()->getTabW();
    for (int i = 0; i < tabw->count(); ++i) m_screenName << tabw->tabText(i);
    m_fademodelist << "None"
                   << "Over Left"
                   << "Over Right"
                   << "Over Top"
                   << "Over Bottom"
                   << "Fade On";
    //                 << "Move Left"
    //                 << "Move Right"
    //                 << "Move Top"
    //                 << "Move Bottom";
  } else if (m_type == 2) {
    // play anim
    auto objs = lvgl.allObjects();
    for (auto o : objs) m_animObjList << o->name();
    m_animPathList << "Linear"
                   << "Ease_in"
                   << "Ease_out"
                   << "Ease_in_out"
                   << "Overshoot"
                   << "Bounce"
                   << "Step";
  } else if (m_type == 0) {
    auto objs = lvgl.allObjects();
    switch (m_wtype) {
      case LVGLEventType::BASIC: {
        for (auto o : objs) m_objName << o->name();
        m_propertyList << "X"
                       << "Y"
                       << "Width"
                       << "Height"
                       //<< "Background Color"
                       //<< "Background gradient color"
                       //<< "Background opa"
                       << "Clickable"
                       << "Dragable"
                       << "Hidden";

        m_valueList << QObject::tr("On") << QObject::tr("Off");
      } break;
      case LVGLEventType::ARC: {
        for (auto o : objs)
          if (o->widgetType() == LVGLWidget::Arc) m_objName << o->name();

        m_propertyList << "End angle"
                       << "Start angle"
                       << "Rotation";
      } break;
      case LVGLEventType::BAR: {
        for (auto o : objs)
          if (o->widgetType() == LVGLWidget::Bar) m_objName << o->name();

        m_propertyList << "Value"
                       << "Value With anim";

      } break;
      case LVGLEventType::BUTTON: {
        for (auto o : objs)
          if (o->widgetType() == LVGLWidget::Button) m_objName << o->name();
        m_propertyList << "Checkable";
        m_valueList << "On"
                    << "Off";
      } break;
      case LVGLEventType::CHECKBOX: {
        for (auto o : objs)
          if (o->widgetType() == LVGLWidget::CheckBox) m_objName << o->name();
        m_propertyList << "Check";
        m_valueList << "Click"
                    << "On"
                    << "Off";
      } break;
      case LVGLEventType::DROPDOWNLIST: {
        for (auto o : objs)
          if (o->widgetType() == LVGLWidget::DropDownList)
            m_objName << o->name();
        m_propertyList << "Set Select By Index";
      } break;
      case LVGLEventType::IMAGE: {
        for (auto o : objs)
          if (o->widgetType() == LVGLWidget::Image) m_objName << o->name();
        m_propertyList << "Change Image";
        m_valueList << "dafault";
        m_valueList += lvgl.imageNames();
      } break;
      case LVGLEventType::IMAGEBUTTON: {
        for (auto o : objs)
          if (o->widgetType() == LVGLWidget::ImageButton)
            m_objName << o->name();
        m_propertyList << "Release"
                       << "Pressed"
                       << "Checked Release"
                       << "Checked Pressed";
        m_valueList << "dafault";
        m_valueList += lvgl.imageNames();
      } break;
      case LVGLEventType::LABEL: {
        for (auto o : objs)
          if (o->widgetType() == LVGLWidget::Label) m_objName << o->name();
        m_propertyList << "Text"
                       << "Time";
      } break;
      case LVGLEventType::ROLLER: {
        auto objs = lvgl.allObjects();
        for (auto o : objs)
          if (o->widgetType() == LVGLWidget::Roller) m_objName << o->name();

        m_propertyList << "Selected with anim"
                       << "Selected";
      } break;
      case LVGLEventType::SLIDER: {
        for (auto o : objs)
          if (o->widgetType() == LVGLWidget::Slider) m_objName << o->name();

        m_propertyList << "Value"
                       << "Value With anim";
      } break;
      case LVGLEventType::SWITCH: {
        for (auto o : objs)
          if (o->widgetType() == LVGLWidget::Switch) m_objName << o->name();

        m_propertyList << "Switch";
        m_valueList << "Click"
                    << "On"
                    << "Off"
                    << "Click with anim"
                    << "On with anim"
                    << "Off with anim";
      } break;
    }
  }
}
