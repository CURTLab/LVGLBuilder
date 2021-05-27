#ifndef LVGLEVENTSLIDER_H
#define LVGLEVENTSLIDER_H

#include "LVGLEvent.h"

class LVGLEventSlider : public LVGLEvent {
 public:
  LVGLEventSlider();
  void eventRun(lv_obj_t *obj) override;
  QStringList eventCode() override;
};

#endif  // LVGLEVENTSLIDER_H
