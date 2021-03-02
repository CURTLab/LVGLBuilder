#ifndef LVGLHELPER_H
#define LVGLHELPER_H
#include <QComboBox>

class MainWindow;

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
    return QString("Page-%1").arg(++m_filecount);
  }

  // set
  void setMainW(MainWindow* m) { m_pMainW = m; }
  void reduceFileindex() { --m_filecount; }

 private:
  LVGLHelper() : m_pMainW(nullptr), m_filecount(0){};
  static LVGLHelper* m_instance;
  MainWindow* m_pMainW;
  int m_filecount;
};
#endif
