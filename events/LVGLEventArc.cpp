#include "LVGLEventArc.h"

#include "MainWindow.h"
#include "core/LVGLCore.h"
#include "core/LVGLHelper.h"
#include "core/LVGLObject.h"
#include "core/LVGLTabWidget.h"
#include "lvgl/lvgl.h"

LVGLEventArc::LVGLEventArc() {}

void LVGLEventArc::eventRun(lv_obj_t *obj) {
  QString name = m_result.at(4);
  lv_obj_t *targert = nullptr;
  LVGLHelper::getInstance().updatetabDate();
  QList<LVGLObject *> objs;
  auto tabw = LVGLHelper::getInstance().getMainW()->getTabW();
  auto &apos = LVGLHelper::getInstance().getanimobjPos();
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
      break;
    }
  }

  QString proprety = m_result.at(5);
  QString way = m_result.at(6);
  if (way == "Not use") {
    int value = m_result.at(7).toInt();
    if (value > 360) value %= 360;
    if (proprety == "Start angle")
      lv_arc_set_start_angle(targert, value);
    else if (proprety == "Rotation")
      lv_arc_set_rotation(targert, value);
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
      if (value > 360) value %= 360;
      if (proprety == "Start angle")
        lv_arc_set_start_angle(targert, value);
      else if (proprety == "Rotation")
        lv_arc_set_rotation(targert, value);
      else
        lv_arc_set_end_angle(targert, value);
    }
  }
}

QStringList LVGLEventArc::objCode(const QString &objName) {
  QStringList list;
  QString str =
      QString("lv_obj_set_event_cb(%1,%2);").arg(objName).arg(m_result[0]);
  list << "\t" << str << "\n";
  return list;
}

QString LVGLEventArc::eventHeadCode() {
  m_eventHeadCode =
      QString("void %1(lv_obj_t *obj, lv_event_t event)").arg(m_result[0]);
  return m_eventHeadCode + ";\n";
}

QStringList LVGLEventArc::eventCode() {
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

  if (way == "Not use") {
    if (proprety == "Start angle") {
      list << QString("lv_arc_set_start_angle(%1_ev_%2,%3);\n")
                  .arg(name)
                  .arg(tabindex)
                  .arg(m_result.at(7));
    } else {
      list << QString("lv_arc_set_end_angle(%1_ev_%2,%3);\n")
                  .arg(name)
                  .arg(tabindex)
                  .arg(m_result.at(7));
    }
  } else {
    switch ((int)type) {
      case LVGLWidget::Slider: {
        list << QString("int value = lv_slider_get_value(%1_ev_%2);\n")
                    .arg(codename)
                    .arg(tabindex);
        list << "\t\t";
      } break;
      case LVGLWidget::Roller: {
        list << QString("char arr[10] = {0};\n");
        list << "\t\t"
             << QString("lv_roller_get_selected_str(%1_ev_%2, arr, 10);\n")
                    .arg(codename)
                    .arg(tabindex);
        list << "\t\t" << QString("int value = atoi(arr);\n");
        list << "\t\t";
      } break;
    }

    list << "if (value > 360) value %= 360;\n"
         << "\t\t";

    if (proprety == "Start angle")
      list << QString("lv_arc_set_start_angle(%1_ev_%2,value);\n")
                  .arg(name)
                  .arg(tabindex);
    else if (proprety == "Rotation")
      list << QString("lv_arc_set_rotation(%1_ev_%2,value);\n")
                  .arg(name)
                  .arg(tabindex);
    else
      list << QString("lv_arc_set_end_angle(%1_ev_%2,value);\n")
                  .arg(name)
                  .arg(tabindex);
  }

  list << "\t}\n";
  list << "}";

  return list;
}
