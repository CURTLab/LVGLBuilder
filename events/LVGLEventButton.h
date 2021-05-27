#ifndef LVGLEVENTBUTTON_H
#define LVGLEVENTBUTTON_H

#include "LVGLEvent.h"

class LVGLEventButton : public LVGLEvent {
 public:
  LVGLEventButton();
  void eventRun(lv_obj_t *obj) override;
  QStringList eventCode() override;
};

#endif  // LVGLEVENTBUTTON_H
