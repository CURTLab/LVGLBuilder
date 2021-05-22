#include "LVGLHelper.h"

#include "LVGLCore.h"
#include "LVGLTabWidget.h"
#include "MainWindow.h"

QStringList &LVGLHelper::pageName() {
  pagelist.clear();
  auto tabw = m_pMainW->getTabW();
  int curindex = tabw->currentIndex();
  for (int i = 0; i < tabw->count(); ++i) {
    if (i != curindex) {
      pagelist.push_back(tabw->tabText(i));
    }
  }
  return pagelist;
}

LVGLSimulator *LVGLHelper::getcursim() { return m_pMainW->simulator(); }

void LVGLHelper::updatetabDate() {
  auto tabw = m_pMainW->getTabW();
  int curindex = tabw->currentIndex();
  auto tab = static_cast<LVGLTabWidget *>(tabw->widget(curindex));

  tab->setAllObjects(lvgl.allObjects());
  tab->setAllImages(lvgl.allImages());
}
