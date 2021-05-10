#ifndef LVGLEVENTDDLIST_H
#define LVGLEVENTDDLIST_H

#include "LVGLEvent.h"

class LVGLEventDDList : public LVGLEvent {
 public:
  LVGLEventDDList();
  void eventRun(lv_obj_t *obj) override;
  QStringList objCode(const QString& objName) override;
  QString eventHeadCode() override;
  QStringList eventCode() override;
};

#endif  // LVGLEVENTDDLIST_H
