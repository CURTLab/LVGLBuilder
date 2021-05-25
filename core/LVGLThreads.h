#ifndef LVGLEXPORTTHREAD_H
#define LVGLEXPORTTHREAD_H

#include <QObject>

class LVGLProject;
class QTimer;

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

class LVGLAutoSaveThread : public QObject {
  Q_OBJECT
 public:
  LVGLAutoSaveThread();
  ~LVGLAutoSaveThread();
  void startrun(int state);
  void saveFile();
  void stop();
 signals:
  void successful();
  void failed();

 private:
  volatile bool stopped;
  QTimer *m_time;
};

#endif  // LVGLEXPORTTHREAD_H
