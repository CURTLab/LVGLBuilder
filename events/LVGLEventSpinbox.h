#ifndef LVGLEVENTSPINBOX_H
#define LVGLEVENTSPINBOX_H

#include "LVGLEvent.h"

class LVGLEventSpinbox : public LVGLEvent {
 public:
  LVGLEventSpinbox();
  void eventRun(lv_obj_t *obj) override;
  QStringList objCode(const QString& objName) override;
  QString eventHeadCode() override;
  QStringList eventCode() override;
};

#endif  // LVGLEVENTSPINBOX_H
