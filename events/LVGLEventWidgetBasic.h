#ifndef LVGLEVENTWIDGETBASIC_H
#define LVGLEVENTWIDGETBASIC_H
#include "LVGLEvent.h"

class LVGLEventWidgetBasic : public LVGLEvent {
 public:
  LVGLEventWidgetBasic();
  WIDGETTYPE whichwidget() override;
};

#endif  // LVGLEVENTWIDGETBASIC_H
