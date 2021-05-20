#include "LVGLEventAnim.h"

#include "LVGLCore.h"
#include "LVGLHelper.h"
#include "LVGLObject.h"
#include "LVGLTabWidget.h"
#include "MainWindow.h"
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
      if (o->name() == name) {
        targert = o->obj();
        objs = os;
        if (!apos.contains(targert)) apos[targert] = o->position();
        break;
      }
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
  return QStringList();
}

QString LVGLEventAnim::eventHeadCode() { return QString(); }

QStringList LVGLEventAnim::eventCode() { return QStringList(); }
