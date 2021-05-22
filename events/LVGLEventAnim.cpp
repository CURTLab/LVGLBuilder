#include "LVGLEventAnim.h"

#include "MainWindow.h"
#include "core/LVGLCore.h"
#include "core/LVGLHelper.h"
#include "core/LVGLObject.h"
#include "core/LVGLTabWidget.h"
#include "lvgl/lvgl.h"

LVGLEventAnim::LVGLEventAnim() {}

void LVGLEventAnim::eventRun(lv_obj_t *obj) {
  QString name = m_result.at(3);
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
      if (!apos.contains(targert)) apos[targert] = o->position();
      break;
    }
  }

  QString waypath = m_result[4];
  if (m_result[5].isEmpty() && m_result.isEmpty()) return;

  lv_anim_path_t path;
  lv_anim_path_init(&path);

  if (waypath == "Linear")
    lv_anim_path_set_cb(&path, lv_anim_path_linear);
  else if (waypath == "Ease_in")
    lv_anim_path_set_cb(&path, lv_anim_path_ease_in);
  else if (waypath == "Ease_out")
    lv_anim_path_set_cb(&path, lv_anim_path_ease_out);
  else if (waypath == "Ease_in_out")
    lv_anim_path_set_cb(&path, lv_anim_path_ease_in_out);
  else if (waypath == "Overshoot")
    lv_anim_path_set_cb(&path, lv_anim_path_overshoot);
  else if (waypath == "Bounce")
    lv_anim_path_set_cb(&path, lv_anim_path_bounce);
  else if (waypath == "Step")
    lv_anim_path_set_cb(&path, lv_anim_path_step);

  lv_anim_t animx;
  lv_anim_t animy;
  int duration = m_result[7].toInt();
  if (!m_result[5].isEmpty()) {
    int x = m_result[5].toInt();
    lv_anim_init(&animx);
    lv_anim_set_var(&animx, targert);
    lv_anim_set_time(&animx, duration);
    lv_anim_set_exec_cb(&animx, (lv_anim_exec_xcb_t)lv_obj_set_x);
    lv_anim_set_values(&animx, lv_obj_get_x(targert), x);
    lv_anim_set_path(&animx, &path);
  }
  if (!m_result[6].isEmpty()) {
    int y = m_result[6].toInt();
    lv_anim_init(&animy);
    lv_anim_set_var(&animy, targert);
    lv_anim_set_time(&animy, duration);
    lv_anim_set_exec_cb(&animy, (lv_anim_exec_xcb_t)lv_obj_set_y);
    lv_anim_set_values(&animy, lv_obj_get_y(targert), y);
    lv_anim_set_path(&animy, &path);
  }
  if (!m_result[5].isEmpty()) lv_anim_start(&animx);
  if (!m_result[6].isEmpty()) lv_anim_start(&animy);
}

QStringList LVGLEventAnim::objCode(const QString &objName) {
  QStringList list;
  QString str =
      QString("lv_obj_set_event_cb(%1,%2);").arg(objName).arg(m_result[0]);
  list << "\t" << str << "\n";
  return list;
}

QString LVGLEventAnim::eventHeadCode() {
  m_eventHeadCode =
      QString("void %1(lv_obj_t *obj, lv_event_t event)").arg(m_result[0]);
  return m_eventHeadCode + ";\n";
}

QStringList LVGLEventAnim::eventCode() {
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

  QString proprety = m_result.at(4);

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
    if (o->name() == m_result[3]) name = o->codeName();

  QString waypath = m_result[4];
  list << "lv_anim_path_t path;\n";
  list << "\t\t"
       << "lv_anim_path_init(&path);\n";
  list << "\t\t";

  if (waypath == "Linear")
    list << "lv_anim_path_set_cb(&path, lv_anim_path_linear);\n";
  else if (waypath == "Ease_in")
    list << "lv_anim_path_set_cb(&path, lv_anim_path_ease_in);\n";
  else if (waypath == "Ease_out")
    list << "lv_anim_path_set_cb(&path, lv_anim_path_ease_out);\n";
  else if (waypath == "Ease_in_out")
    list << "lv_anim_path_set_cb(&path, lv_anim_path_ease_in_out);\n";
  else if (waypath == "Overshoot")
    list << "lv_anim_path_set_cb(&path, lv_anim_path_overshoot);\n";
  else if (waypath == "Bounce")
    list << "lv_anim_path_set_cb(&path, lv_anim_path_bounce);\n";
  else if (waypath == "Step")
    list << "lv_anim_path_set_cb(&path, lv_anim_path_step);\n";

  if (!m_result[5].isEmpty()) {
    list << "\t\t"
         << "lv_anim_t animx;\n";
    list << "\t\t"
         << "lv_anim_init(&animx);\n";
    list << "\t\t"
         << QString("lv_anim_set_var(&animx, %1_ev_%2);\n")
                .arg(name)
                .arg(tabindex);
    list << "\t\t"
         << QString("lv_anim_set_time(&animx, %1);\n").arg(m_result[7]);
    list << "\t\t"
         << QString(
                "lv_anim_set_exec_cb(&animx, "
                "(lv_anim_exec_xcb_t)lv_obj_set_x);\n");
    list << "\t\t"
         << QString("lv_anim_set_values(&animx, lv_obj_get_x(%1_ev_%2), %3);\n")
                .arg(name)
                .arg(tabindex)
                .arg(m_result[5]);
    list << "\t\t" << QString("lv_anim_set_path(&animx, &path);\n");
  }

  if (!m_result[6].isEmpty()) {
    list << "\t\t"
         << "lv_anim_t animy;\n";
    list << "\t\t"
         << "lv_anim_init(&animy);\n";
    list << "\t\t"
         << QString("lv_anim_set_var(&animy, %1_ev_%2);\n")
                .arg(name)
                .arg(tabindex);
    list << "\t\t"
         << QString("lv_anim_set_time(&animy, %1);\n").arg(m_result[7]);
    list << "\t\t"
         << QString(
                "lv_anim_set_exec_cb(&animy, "
                "(lv_anim_exec_xcb_t)lv_obj_set_y);\n");
    list << "\t\t"
         << QString("lv_anim_set_values(&animy, lv_obj_get_y(%1_ev_%2), %3);\n")
                .arg(name)
                .arg(tabindex)
                .arg(m_result[6]);
    list << "\t\t" << QString("lv_anim_set_path(&animy, &path);\n");
  }

  if (!m_result[5].isEmpty()) {
    list << "\t\t"
         << "lv_anim_start(&animx);\n";
  }

  if (!m_result[6].isEmpty()) {
    list << "\t\t"
         << "lv_anim_start(&animy);\n";
  }

  list << "\t}\n";
  list << "}";

  return list;
}
