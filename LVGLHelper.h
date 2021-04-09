#ifndef LVGLHELPER_H
#define LVGLHELPER_H
#include <QComboBox>
#include <QSet>

class MainWindow;
class LVGLObject;

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

  // set
  void setMainW(MainWindow* m) { m_pMainW = m; }
  void reduceFileindex() { --m_filecount; }

 private:
  LVGLHelper() : m_pMainW(nullptr), m_filecount(0){};
  static LVGLHelper* m_instance;
  MainWindow* m_pMainW;
  int m_filecount;
  QStringList pagelist;
  QMap<LVGLObject*, int> m_btnGoPage;
  QSet<QString> m_saveFontN;
};
#endif
