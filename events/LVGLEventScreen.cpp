#include "LVGLEventScreen.h"

#include "LVGLHelper.h"
#include "MainWindow.h"

LVGLEventScreen::LVGLEventScreen() {
  // screen name
  auto tabw = LVGLHelper::getInstance().getMainW()->getTabW();
  int currentindex = tabw->currentIndex();
  for (int i = 0; i < tabw->count(); ++i)
    if (i != currentindex) m_screenName << tabw->tabText(i);
  m_fademodelist << "Move Left"
                 << "Move Right"
                 << "Move Top"
                 << "Move Bottom"
                 << "Over Left"
                 << "Over Right"
                 << "Over Top"
                 << "Over Bottom"
                 << "Fade On";
}

LVGLEvent::WIDGETTYPE LVGLEventScreen::whichwidget() {
  return LVGLEvent::e_NONE;
}
