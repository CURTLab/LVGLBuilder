#ifndef LVGLEVENTIMAGE_H
#define LVGLEVENTIMAGE_H

#include "LVGLEvent.h"

class LVGLEventImage : public LVGLEvent {
 public:
  LVGLEventImage();
  void eventRun(lv_obj_t *obj) override;
  QStringList objCode(const QString& objName) override;
  QString eventHeadCode() override;
  QStringList eventCode() override;
};

#endif  // LVGLEVENTIMAGE_H
