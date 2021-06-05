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
  ~LVGLHelper();
  LVGLHelper(const LVGLHelper&) = delete;
  LVGLHelper operator=(const LVGLHelper&) = delete;
  static LVGLHelper& getInstance() {
    static LVGLHelper instance;
    return instance;
  }

  // get
  bool getNeedSetTime() { return m_needSetTime; }
  int getLoadMethod() { return m_loadMethod; }
  int getExportMethod() { return m_exportMethod; }

  LVGLSimulator* getcursim();
  MainWindow* getMainW() { return m_pMainW; }

  QStringList& pageName();
  QStringList& getEventName();
  QList<QString>& getSaveFontName() { return m_saveFontN; }
  QList<QString>& getAllFontName() { return m_allFontN; }
  QMap<lv_obj_t*, QList<LVGLEvent*>>& getObjEvents() { return m_objEvents; }
  QMap<lv_obj_t*, QString>& getTimeCmd() { return m_timeCmd; }

  // set
  void setNeedSetTime(bool b) { m_needSetTime = b; }
  void setLoadMethod(int method) { m_loadMethod = method; }
  void setExportMethod(int method) { m_exportMethod = method; }
  void setMainW(MainWindow* m) { m_pMainW = m; }
  void setSaveFontNmae(const QString& name);

  // other
  void updatetabDate();
  int getobjeventsize();

  QString newProjectName();
  QString newEventName();
  QString getStringWithSymbol(const QString& s);
  void addNeedDelPoint(char* p) { m_needDelCharPoint.push_back(p); }

 private:
  LVGLHelper()
      : m_pMainW(nullptr),
        m_needSetTime(false),
        m_loadMethod(0),
        m_exportMethod(0) {
    m_saveFontN.push_back("lv_font_123abc_16");
  }

  MainWindow* m_pMainW;
  QStringList pagelist;
  QList<QString> m_saveFontN;
  QList<QString> m_allFontN;
  QMap<lv_obj_t*, QList<LVGLEvent*>> m_objEvents;
  QMap<lv_obj_t*, QString> m_timeCmd;
  bool m_needSetTime;
  int m_loadMethod;
  int m_exportMethod;
  QStringList m_eventName;
  QList<char*> m_needDelCharPoint;
};
#endif
