#ifndef LVGLEVENTLABEL_H
#define LVGLEVENTLABEL_H

#include "LVGLEvent.h"

class LVGLEventLabel : public LVGLEvent {
 public:
  LVGLEventLabel();
  void eventRun(lv_obj_t *obj) override;
  QStringList objCode(const QString& objName) override;
  QString eventHeadCode() override;
  QStringList eventCode() override;
};

#endif  // LVGLEVENTLABEL_H
