#ifndef LVGLEXPORTTHREAD_H
#define LVGLEXPORTTHREAD_H

#include <QObject>

class LVGLProject;

class LVGLExportThread : public QObject {
  Q_OBJECT
 public:
  LVGLExportThread();
  ~LVGLExportThread();
  void startrun(const QStringList &list);
  void stop();
 signals:
  void successful();
  void failed();

 private:
  volatile bool stopped;
  LVGLProject *m_project;
};
#endif  // LVGLEXPORTTHREAD_H
