#ifndef LVGLEVENTANIM_H
#define LVGLEVENTANIM_H

#include "LVGLEvent.h"

class LVGLEventAnim : public LVGLEvent {
 public:
  LVGLEventAnim();
  void eventRun(lv_obj_t* obj) override;
  QStringList objCode(const QString& objName) override;
  QString eventHeadCode() override;
  QStringList eventCode() override;
};

#endif  // LVGLEVENTANIM_H
