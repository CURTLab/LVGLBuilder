#include "LVGLEventSwitch.h"

#include "LVGLEventStateResume.h"
#include "MainWindow.h"
#include "core/LVGLCore.h"
#include "core/LVGLHelper.h"
#include "core/LVGLObject.h"
#include "core/LVGLTabWidget.h"
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

QStringList LVGLEventSwitch::eventCode() {
  QStringList list;
  list << QString("%1{").arg(m_eventHeadCode) << "\n";
  if (m_result[1] == "Pressed") {
    list << "\t"
         << "if (event == LV_EVENT_PRESSED){"
         << "\n";
  } else if (m_result[1] == "Clicked") {
    list << "\t"
         << "if (event == LV_EVENT_CLICKED){"
         << "\n";
  } else if (m_result[1] == "Long_Pressed") {
    list << "\t"
         << "if (event == LV_EVENT_LONG_PRESSED){"
         << "\n";
  } else if (m_result[1] == "Long_Pressed_Repeat") {
    list << "\t"
         << "if (event == LV_EVENT_LONG_PRESSED_REPEAT){"
         << "\n";
  } else if (m_result[1] == "Focused") {
    list << "\t"
         << "if (event == LV_EVENT_FOCUSED){"
         << "\n";
  } else if (m_result[1] == "Defocused") {
    list << "\t"
         << "if (event == LV_EVENT_DEFOCUSED){"
         << "\n";
  } else if (m_result[1] == "Value_Changed") {
    list << "\t"
         << "if (event == LV_EVENT_VALUE_CHANGED){"
         << "\n";
  }
  list << "\t\t";

  QString proprety = m_result.at(5);
  QString way = m_result.at(6);

  auto tabw = LVGLHelper::getInstance().getMainW()->getTabW();
  int tabindex = tabw->currentIndex();
  auto objs = lvgl.allObjects();
  auto nowtab = static_cast<LVGLTabWidget *>(tabw->widget(tabindex));
  LVGLWidget::Type type;
  QString codename;
  int nameindex = tabindex;

  nowtab->setAllObjects(lvgl.allObjects());
  nowtab->setAllImages(lvgl.allImages());

  QString name;
  QMap<lv_obj_t *, QList<LVGLEvent *>> &objevs =
      LVGLHelper::getInstance().getObjEvents();
  lv_obj_t *obb = nullptr;
  auto iter = objevs.begin();
  for (; iter != objevs.end(); ++iter) {
    QList<LVGLEvent *> &listev = iter.value();
    for (auto e : listev)
      if (e == this) {
        obb = iter.key();
        break;
      }
  }

  for (int i = 0; i < tabw->count(); ++i) {
    auto tb = static_cast<LVGLTabWidget *>(tabw->widget(i));
    auto os = tb->allObject();
    for (auto o : os) {
      if (obb == o->obj()) {
        tabindex = i;
        nameindex = i;
        codename = o->codeName();
        type = o->widgetType();
      }
    }
  }

  auto tb = static_cast<LVGLTabWidget *>(tabw->widget(nameindex));
  auto os = tb->allObject();
  for (auto o : os)
    if (o->name() == m_result[4]) name = o->codeName();

  if (way == "Click") {
    list << QString("auto b = lv_switch_get_state(%1_ev_%2);\n")
                .arg(name)
                .arg(tabindex);
    list << "\t\t" << QString("if (b)") << "\n";
    list << "\t\t\t"
         << QString("lv_switch_off(%1_ev_%2,LV_ANIM_OFF);\n")
                .arg(name)
                .arg(tabindex);
    list << "\t\t" << QString("else ") << "\n";
    list << "\t\t\t"
         << QString("lv_switch_on(%1_ev_%2,LV_ANIM_OFF);\n")
                .arg(name)
                .arg(tabindex);
  } else if (way == "On") {
    list << QString("lv_switch_on(%1_ev_%2,LV_ANIM_OFF);\n")
                .arg(name)
                .arg(tabindex);
  } else if (way == "Off") {
    list << QString("lv_switch_off(%1_ev_%2,LV_ANIM_OFF);\n")
                .arg(name)
                .arg(tabindex);
  } else if (way == "Click with anim") {
    list << QString("auto b = lv_switch_get_state(%1_ev_%2);\n")
                .arg(name)
                .arg(tabindex);
    list << "\t\t" << QString("if (b)") << "\n";
    list << "\t\t\t"
         << QString("lv_switch_off(%1_ev_%2,LV_ANIM_ON);\n")
                .arg(name)
                .arg(tabindex);
    list << "\t\t" << QString("else") << "\n";
    list << "\t\t\t"
         << QString("lv_switch_on(%1_ev_%2,LV_ANIM_ON);\n")
                .arg(name)
                .arg(tabindex);
  } else if (way == "On with anim") {
    list << QString("lv_switch_off(%1_ev_%2,LV_ANIM_ON);\n")
                .arg(name)
                .arg(tabindex);
  } else {
    list << QString("lv_switch_on(%1_ev_%2,LV_ANIM_ON);\n")
                .arg(name)
                .arg(tabindex);
  }
  list << "\t}\n";
  list << "}";
  return list;
}
