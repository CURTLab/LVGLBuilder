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

  // set
  void setMainW(MainWindow* m) { m_pMainW = m; }

 private:
  LVGLHelper(){};
  static LVGLHelper* m_instance;
  MainWindow* m_pMainW;
};
#endif
