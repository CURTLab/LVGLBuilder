#ifndef LVGLEVNETROLLER_H
#define LVGLEVNETROLLER_H
#include "LVGLEvent.h"

class LVGLEventRoller : public LVGLEvent {
 public:
  LVGLEventRoller();
  void eventRun(lv_obj_t *obj) override;
  QStringList objCode(const QString& objName) override;
  QString eventHeadCode() override;
  QStringList eventCode() override;
};

#endif  // LVGLEVNETROLLER_H
