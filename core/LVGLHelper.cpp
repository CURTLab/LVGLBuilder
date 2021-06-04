#include "LVGLHelper.h"

#include "LVGLCore.h"
#include "LVGLTabWidget.h"
#include "MainWindow.h"
#include "events/LVGLEvent.h"

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

QStringList &LVGLHelper::getEventName() { return m_eventName; }

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

int LVGLHelper::getobjeventsize() {
  int size = 0;
  auto iter = m_objEvents.begin();
  for (; iter != m_objEvents.end(); ++iter) size += iter.value().size();
  return size;
}

QString LVGLHelper::newProjectName() {
  auto tabw = m_pMainW->getTabW();
  QSet<int> set;
  for (int i = 0; i < tabw->count(); ++i) {
    auto tab = static_cast<LVGLTabWidget *>(tabw->widget(i));
    QString name = tab->getfilename();
    if (name.size() > 5) {
      set.insert(name.mid(5, name.size() - 5).toUInt());
    }
  }
  int index = 1;
  for (; index < tabw->count() + 1; ++index)
    if (!set.contains(index)) break;
  return QString("Page_%1").arg(index);
}

QString LVGLHelper::newEventName() {
  QSet<int> set;
  for (auto s : m_eventName)
    if (s.size() > 6) {
      set.insert(s.mid(6, s.size() - 6).toUInt());
    }
  int index = 1;
  for (; index < m_eventName.size() + 1; ++index)
    if (!set.contains(index)) break;
  return QString("Event_%1").arg(index);
}
