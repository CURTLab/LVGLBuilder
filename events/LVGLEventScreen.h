#ifndef LVGLEVENTSCREEN_H
#define LVGLEVENTSCREEN_H
#include "LVGLEvent.h"

class LVGLEventScreen : public LVGLEvent {
 public:
  LVGLEventScreen();
  WIDGETTYPE whichwidget() override;
};

#endif  // LVGLEVENTSCREEN_H
