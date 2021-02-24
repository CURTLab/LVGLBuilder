#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QObject>
#include <QWidget>

class LVGLCore;
class LVGLSimulator;
class LVGLProject;

class TabWidget : public QWidget {
  Q_OBJECT
 public:
  explicit TabWidget(const QString& name, QWidget* parent = nullptr);
  ~TabWidget();
  // LvglWidgetFunc;
  void initLvglWidget();
  void initLvglCore(int w, int h);

  LVGLCore* getCore() { return m_core; }
  LVGLSimulator* getSimulator() { return m_simulator; }
  LVGLProject* getProject() { return m_project; }
  const QString& getName() { return m_name; }

  void setProject(LVGLProject* pj) { m_project = pj; }

 private:
  LVGLCore* m_core;
  LVGLSimulator* m_simulator;
  LVGLProject* m_project;

  QString m_name;

  // TextWidgetVars
};

#endif
