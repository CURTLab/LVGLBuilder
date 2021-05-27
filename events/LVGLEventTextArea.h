#ifndef LVGLEVENTTEXTAREA_H
#define LVGLEVENTTEXTAREA_H

#include "LVGLEvent.h"

class LVGLEventTextArea : public LVGLEvent {
 public:
  LVGLEventTextArea();
  void eventRun(lv_obj_t *obj) override;
  QStringList eventCode() override;
};

#endif  // LVGLEVENTTEXTAREA_H
