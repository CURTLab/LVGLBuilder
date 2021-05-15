#include "LVGLEventSwitch.h"

#include "LVGLCore.h"
#include "LVGLHelper.h"
#include "LVGLObject.h"
#include "LVGLTabWidget.h"
#include "MainWindow.h"
#include "lvgl/lvgl.h"

LVGLEventSwitch::LVGLEventSwitch() {}

void LVGLEventSwitch::eventRun(lv_obj_t *obj) {
  QString name = m_result.at(4);
  lv_obj_t *targert = nullptr;
  LVGLHelper::getInstance().updatetabDate();
  QList<LVGLObject *> objs;
  auto tabw = LVGLHelper::getInstance().getMainW()->getTabW();
  for (int i = 0; i < tabw->count(); ++i) {
    auto tab = static_cast<LVGLTabWidget *>(tabw->widget(i));
    auto os = tab->allObject();
    for (auto o : os) {
      if (o->name() == name) {
        targert = o->obj();
        objs = os;
        break;
      }
    }
  }

  QString proprety = m_result.at(5);
  QString way = m_result.at(6);
  if (way == "Click") {
    auto b = lv_switch_get_state(targert);
    if (b)
      lv_switch_off(targert, LV_ANIM_OFF);
    else
      lv_switch_on(targert, LV_ANIM_OFF);
  } else if (way == "On") {
    lv_switch_on(targert, LV_ANIM_OFF);
  } else if (way == "Off") {
    lv_switch_off(targert, LV_ANIM_OFF);
  } else if (way == "Click with anim") {
    auto b = lv_switch_get_state(targert);
    if (b)
      lv_switch_off(targert, LV_ANIM_ON);
    else
      lv_switch_on(targert, LV_ANIM_ON);
  } else if (way == "On with anim") {
    lv_switch_on(targert, LV_ANIM_ON);
  } else if (way == "Off with anim") {
    lv_switch_off(targert, LV_ANIM_ON);
  }
}

QStringList LVGLEventSwitch::objCode(const QString &objName) {
  return QStringList();
}

QString LVGLEventSwitch::eventHeadCode() { return QString(); }

QStringList LVGLEventSwitch::eventCode() { return QStringList(); }
