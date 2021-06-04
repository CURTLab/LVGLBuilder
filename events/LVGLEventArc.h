#ifndef LVGLEVENTARC_H
#define LVGLEVENTARC_H

#include "LVGLEvent.h"

class LVGLEventArc : public LVGLEvent {
 public:
  LVGLEventArc();
  void eventRun(lv_obj_t *obj) override;
  QStringList eventCode() override;
};

#endif  // LVGLEVENTARC_H