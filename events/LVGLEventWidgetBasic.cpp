#include "LVGLEventWidgetBasic.h"

#include "LVGLEventStateResume.h"
#include "MainWindow.h"
#include "core/LVGLCore.h"
#include "core/LVGLHelper.h"
#include "core/LVGLObject.h"
#include "core/LVGLTabWidget.h"
#include "lvgl/lvgl.h"

LVGLEventWidgetBasic::LVGLEventWidgetBasic() {}

void LVGLEventWidgetBasic::eventRun(lv_obj_t *obj) {
  QString name = m_result.at(4);
  lv_obj_t *targert = nullptr;
  LVGLHelper::getInstance().updatetabDate();
  QList<LVGLObject *> objs;
  auto tabw = LVGLHelper::getInstance().getMainW()->getTabW();
  for (int i = 0; i < tabw->count(); ++i) {
    auto tab = static_cast<LVGLTabWidget *>(tabw->widget(i));
    auto os = tab->allObject();
    for (auto o : os) {
      if (o->obj() == obj) {
        objs = os;
        break;
      }
    }
  }

  for (auto o : objs) {
    if (o->name() == name) {
      targert = o->obj();
      LVGLEventStateResume::getInstance().addEvent(o);
      break;
    }
  }

  QString proprety = m_result.at(5);
  QString way = m_result.at(6);
  if (proprety == "X") {
    int x = m_result.at(7).toInt();
    lv_obj_set_x(targert, x);
  } else if (proprety == "Y") {
    int y = m_result.at(7).toInt();
    lv_obj_set_y(targert, y);
  } else if (proprety == "Width") {
    int w = m_result.at(7).toInt();
    lv_obj_set_width(targert, w);
  } else if (proprety == "Height") {
    int h = m_result.at(7).toInt();
    lv_obj_set_height(targert, h);
  } else if (proprety == "Clickable") {
    bool b = m_result.at(7).toInt() == 0 ? false : true;
    lv_obj_set_click(targert, b);
  } else if (proprety == "Dragable") {
    bool b = m_result.at(7).toInt() == 0 ? false : true;
    lv_obj_set_drag(targert, b);
  } else if (proprety == "Hidden") {
    bool b = m_result.at(7).toInt() == 0 ? false : true;
    lv_obj_set_hidden(targert, b);
  }
}

QStringList LVGLEventWidgetBasic::eventCode() {
  QStringList list;
  return list;
}
