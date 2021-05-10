#ifndef LVGLEVENT_H
#define LVGLEVENT_H

#include <QObject>

#include "lvgl/lvgl/src/lv_core/lv_obj.h"

class LVGLEvent {
 public:
 public:
  LVGLEvent();
  virtual ~LVGLEvent();

  inline QStringList getResult() { return m_result; }
  inline void setResule(QStringList res) { m_result = res; }

  virtual void eventRun(lv_obj_t* obj) = 0;
  virtual QStringList objCode(const QString& objName) = 0;
  virtual QString eventHeadCode() = 0;
  virtual QStringList eventCode() = 0;

 protected:
  QStringList m_result;
  QString m_eventHeadCode;
};

#endif  // LVGLEVENT_H
