#include "LVGLEventSlider.h"

#include "LVGLCore.h"
#include "LVGLHelper.h"
#include "LVGLObject.h"
#include "LVGLTabWidget.h"
#include "MainWindow.h"
#include "lvgl/lvgl.h"

LVGLEventSlider::LVGLEventSlider() {}

void LVGLEventSlider::eventRun(lv_obj_t *obj) {
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
  int min = lv_slider_get_min_value(targert);
  int max = lv_slider_get_max_value(targert);
  if (way == "Not use") {
    int value = m_result.at(7).toInt();
    if (value < min) value = min;
    if (value > max) value = value % (max + 1);
    if (proprety == "Value")
      lv_slider_set_value(targert, value, LV_ANIM_OFF);
    else
      lv_slider_set_value(targert, value, LV_ANIM_ON);
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
      if (value < min) value = min;
      if (value > max) value = value % (max + 1);
      if (proprety == "Value")
        lv_slider_set_value(targert, value, LV_ANIM_OFF);
      else
        lv_slider_set_value(targert, value, LV_ANIM_ON);
    }
  }
}

QStringList LVGLEventSlider::objCode(const QString &objName) {
  return QStringList();
}

QString LVGLEventSlider::eventHeadCode() { return QString(); }

QStringList LVGLEventSlider::eventCode() { return QStringList(); }
