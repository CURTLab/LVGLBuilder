#ifndef EVENTSETTINGWIDGEET_H
#define EVENTSETTINGWIDGEET_H

#include <QDialog>
class LVGLEvent;

QT_BEGIN_NAMESPACE
namespace Ui {
class EventSettingWidgeet;
}
QT_END_NAMESPACE

class EventSettingWidgeet : public QDialog {
  Q_OBJECT

 public:
  EventSettingWidgeet(LVGLEvent *event, int type, QWidget *parent = nullptr);
  ~EventSettingWidgeet();

  void setTextList(const QStringList &list);
  QStringList textList();

 private slots:
  void on_pushButton_clicked();
  void on_pushButton_2_clicked();

 private:
  void init();

 private:
  Ui::EventSettingWidgeet *ui;
  LVGLEvent *m_event;
  bool m_iscomb;
  int m_type;
};
#endif  // EVENTSETTINGWIDGEET_H
