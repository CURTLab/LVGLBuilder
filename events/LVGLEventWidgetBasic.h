#ifndef LVGLEVENTWIDGETBASIC_H
#define LVGLEVENTWIDGETBASIC_H
#include "LVGLEvent.h"

class LVGLEventWidgetBasic : public LVGLEvent {
 public:
  LVGLEventWidgetBasic();
  void eventRun(lv_obj_t *obj) override;
  QStringList objCode(const QString& objName) override;
  QString eventHeadCode() override;
  QStringList eventCode() override;
};

#endif  // LVGLEVENTWIDGETBASIC_H
