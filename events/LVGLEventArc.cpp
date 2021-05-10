#include "LVGLEventArc.h"

#include "LVGLCore.h"
#include "LVGLObject.h"
#include "lvgl/lvgl.h"

LVGLEventArc::LVGLEventArc() {}

void LVGLEventArc::eventRun(lv_obj_t *obj) {
  QString name = m_result.at(4);
  lv_obj_t *targert = nullptr;
  auto objs = lvgl.allObjects();

  for (auto o : objs)
    if (o->name() == name) {
      targert = o->obj();
      break;
    }

  QString proprety = m_result.at(5);
  QString way = m_result.at(6);
  if (way == "Not use") {
    int value = m_result.at(7).toInt();
    if (proprety == "Start angle")
      lv_arc_set_start_angle(targert, value);
    else
      lv_arc_set_end_angle(targert, value);
  } else {
    LVGLWidget::Type type;
    for (auto o : objs) {
      if (obj == o->obj()) {
        type = o->widgetType();
      }
      int value = 0;
      switch ((int)type) {
        case LVGLWidget::Slider: {
          value = lv_slider_get_value(obj);
        } break;
        case LVGLWidget::Roller: {
          char arr[10]{0};
          lv_roller_get_selected_str(obj, arr, 10);
          value = QString(arr).toInt();
        } break;
      }
      value %= 361;
      if (proprety == "Start angle")
        lv_arc_set_start_angle(targert, value);
      else
        lv_arc_set_end_angle(targert, value);
    }
  }
}

QStringList LVGLEventArc::objCode(const QString &objName) {
  return QStringList();
}

QString LVGLEventArc::eventHeadCode() { return QString(); }

QStringList LVGLEventArc::eventCode() { return QStringList(); }
