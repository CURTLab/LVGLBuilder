#include "LVGLEventLabel.h"

#include <QDateTime>

#include "LVGLEventStateResume.h"
#include "MainWindow.h"
#include "core/LVGLCore.h"
#include "core/LVGLHelper.h"
#include "core/LVGLObject.h"
#include "core/LVGLTabWidget.h"
#include "lvgl/lvgl.h"

static void time_start(void *var, lv_anim_value_t value) {
  QDateTime cdt = QDateTime::currentDateTime();
  auto &timecmd = LVGLHelper::getInstance().getTimeCmd();
  QString date = cdt.toString(timecmd[(lv_obj_t *)var]);
  lv_label_set_text((lv_obj_t *)var, date.toUtf8().data());
}

LVGLEventLabel::LVGLEventLabel() {
  LVGLHelper::getInstance().setNeedSetTime(true);
}

void LVGLEventLabel::eventRun(lv_obj_t *obj) {
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
  if (proprety == "Text") {
    lv_label_set_text(targert, m_result[7].toUtf8().data());
  } else if (proprety == "Time") {
    QDateTime cdt = QDateTime::currentDateTime();
    QString cmd = m_result[7].isEmpty() ? "h:m:s" : m_result[7];
    auto &timecmd = LVGLHelper::getInstance().getTimeCmd();
    timecmd[targert] = cmd;
    lv_anim_t animtime;
    lv_anim_init(&animtime);
    lv_anim_set_var(&animtime, targert);
    lv_anim_set_values(&animtime, 0, 59);
    lv_anim_set_repeat_count(&animtime, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_exec_cb(&animtime, (lv_anim_exec_xcb_t)time_start);
    lv_anim_start(&animtime);
  }
}

QStringList LVGLEventLabel::eventCode() {
  QStringList list;
  QString proprety = m_result.at(5);
  if (proprety == "Time") {
    QString cmd = m_result[7].isEmpty() ? "yyyy-MM-dd hh:mm:ss" : m_result[7];
    list << QString("static char %1_timepare[] = \"%2\";\n")
                .arg(m_result[0])
                .arg(cmd);
    list << QString("void %1_settime(void *var, lv_anim_value_t value) {")
                .arg(m_result[0]);
    list << "\t" << QString("lv_obj_t *obj = (lv_obj_t *)var;\n");
    list << "\t" << QString("char timestr[80] = {0};\n");
    list
        << "\t"
        << QString(
               "dateTimeToString(%1_timepare, sizeof(%2_timepare), timestr);\n")
               .arg(m_result[0])
               .arg(m_result[0]);
    list << "\t" << QString("lv_label_set_text(obj, timestr);\n");
    list << "}\n";
  }

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

  QString way = m_result.at(6);

  if (proprety == "Text") {
    list << QString("lv_label_set_text(%1_ev_%2, \"%3\");\n")
                .arg(name)
                .arg(tabindex)
                .arg(m_result[7]);
  } else if (proprety == "Time") {
    list << QString("lv_anim_t animtime;\n");
    list << "\t\t" << QString("lv_anim_init(&animtime);\n");
    list << "\t\t"
         << QString("lv_anim_set_var(&animtime, %1_ev_%2);\n")
                .arg(name)
                .arg(tabindex);
    list << "\t\t" << QString("lv_anim_set_values(&animtime, 0, 59);\n");
    list << "\t\t"
         << QString(
                "lv_anim_set_repeat_count(&animtime, "
                "LV_ANIM_REPEAT_INFINITE);\n");
    list << "\t\t"
         << QString(
                "lv_anim_set_exec_cb(&animtime, "
                "(lv_anim_exec_xcb_t)%1_settime);\n")
                .arg(m_result[0]);
    list << "\t\t" << QString("lv_anim_start(&animtime);\n");
  }

  list << "\t}\n";
  list << "}";
  return list;
}
