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

QString LVGLHelper::getStringWithSymbol(const QString &s) {
  QString result;
  QString sbl;
  bool findit = false;
  for (int i = 0; i < s.size(); ++i) {
    if (!findit) {
      if (s[i] == '(') {
        if (i + 5 < s.size()) {
          if (s.mid(i, 6) == "(LV_S_") {
            findit = true;
            i += 5;
            continue;
          }
        }
      }
      result += s[i];
    } else {
      if (s[i] != ')') {
        sbl += s[i];
      } else {
        QString str = "LV_SYMBOL_" + sbl;
        result += QByteArray(lvgl.symbol(str));
        sbl.clear();
        findit = false;
      }
    }
  }
  return result;
}

void LVGLHelper::updatetabDate() {
  auto tabw = m_pMainW->getTabW();
  int curindex = tabw->currentIndex();
  auto tab = static_cast<LVGLTabWidget *>(tabw->widget(curindex));

  tab->setAllObjects(lvgl.allObjects());
  tab->setAllImages(lvgl.allImages());
}
