#ifndef LVGLHELPER_H
#define LVGLHELPER_H
#include <QComboBox>
#include <QSet>

#include "lvgl/lvgl.h"

class MainWindow;
class LVGLObject;
class LVGLSimulator;
class LVGLEvent;

class LVGLHelper {
 public:
  ~LVGLHelper(){};
  LVGLHelper(const LVGLHelper&) = delete;
  LVGLHelper operator=(const LVGLHelper&) = delete;
  static LVGLHelper& getInstance() {
    static LVGLHelper instance;
    return instance;
  }
  // get
  MainWindow* getMainW() { return m_pMainW; }
  const QString generateFileName() {
    return QString("Page_%1").arg(++m_filecount);
  }
  QStringList& pageName();
  QMap<LVGLObject*, int>& getBtnGoPage() { return m_btnGoPage; }
  bool IsBtngoPageEmpty() { return m_btnGoPage.isEmpty(); }
  QSet<QString>& getSaveFontName() { return m_saveFontN; }
  LVGLSimulator* getcursim();
  QMap<lv_obj_t*, QList<LVGLEvent*>>& getObjEvents() { return m_objEvents; }
  QMap<lv_obj_t*, QString>& getTimeCmd() { return m_timeCmd; }
  bool getNeedSetTime() { return m_needSetTime; }

  // set
  void setMainW(MainWindow* m) { m_pMainW = m; }
  void reduceFileindex() { --m_filecount; }
  void setNeedSetTime(bool b) { m_needSetTime = b; }

  // other
  void updatetabDate();
  int getobjeventsize() {
    int size = 0;
    auto iter = m_objEvents.begin();
    for (; iter != m_objEvents.end(); ++iter) size += iter.value().size();
    return size;
  }

 private:
  LVGLHelper() : m_pMainW(nullptr), m_filecount(0), m_needSetTime(false){};
  MainWindow* m_pMainW;
  int m_filecount;
  QStringList pagelist;
  QMap<LVGLObject*, int> m_btnGoPage;
  QSet<QString> m_saveFontN;
  QMap<lv_obj_t*, QList<LVGLEvent*>> m_objEvents;
  QMap<lv_obj_t*, QString> m_timeCmd;
  bool m_needSetTime;
};
#endif
