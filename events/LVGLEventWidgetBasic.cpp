#include "LVGLEventWidgetBasic.h"

#include <LVGLObject.h>

#include "LVGLCore.h"
#include "LVGLSimulator.h"

LVGLEventWidgetBasic::LVGLEventWidgetBasic() {
  auto objs = lvgl->allObjects();

  for (auto o : objs) m_objName << o->name();

  m_propertyList << "X"
                 << "Y"
                 << "Width"
                 << "Height"
                 << "Clickable"
                 << "Hidden"
                 << "Disabled"
                 << "Dragable";
  m_valueList << QObject::tr("On") << QObject::tr("Off");
}

LVGLEvent::WIDGETTYPE LVGLEventWidgetBasic::whichwidget() {
  return LVGLEvent::e_LINEEDIT;
}
