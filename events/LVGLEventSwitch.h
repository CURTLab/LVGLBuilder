#ifndef LVGLEVENTSWITCH_H
#define LVGLEVENTSWITCH_H

#include "LVGLEvent.h"

class LVGLEventSwitch : public LVGLEvent {
 public:
  LVGLEventSwitch();
  void eventRun(lv_obj_t *obj) override;
  QStringList eventCode() override;
};

#endif  // LVGLEVENTSWITCH_H
