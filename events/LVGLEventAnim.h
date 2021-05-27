#ifndef LVGLEVENTANIM_H
#define LVGLEVENTANIM_H

#include "LVGLEvent.h"

class LVGLEventAnim : public LVGLEvent {
 public:
  LVGLEventAnim();
  void eventRun(lv_obj_t* obj) override;
  QStringList eventCode() override;
};

#endif  // LVGLEVENTANIM_H
