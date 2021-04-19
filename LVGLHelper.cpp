#include "LVGLHelper.h"

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
