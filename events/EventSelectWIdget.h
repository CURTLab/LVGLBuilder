#ifndef EVENTSELECTWIDGET_H
#define EVENTSELECTWIDGET_H

#include <QDialog>

class EventSettingWidgeet;
class LVGLEventType;
class LVGLWidget;
extern int Ev_index;

QT_BEGIN_NAMESPACE
namespace Ui {
class EventSelectWIdget;
}
QT_END_NAMESPACE

class EventSelectWIdget : public QDialog {
  Q_OBJECT

 public:
  EventSelectWIdget(LVGLWidget *w, QWidget *parent = nullptr);
  ~EventSelectWIdget();
  void setTextList(const QStringList &list);
  QStringList textList();

 private slots:
  void on_selectbtn_clicked();
  void on_cancelbtn_clicked();
  void slotSetWFinished();
  void slotTypeChanged(int index);

 private:
  void getEventType(LVGLEventType *&ev, int index);

 private:
  Ui::EventSelectWIdget *ui;
  EventSettingWidgeet *m_setWidget;
  LVGLEventType *m_ev;
};
#endif  // EVENTSELECTWIDGET_H
