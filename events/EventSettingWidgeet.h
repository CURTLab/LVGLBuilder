#ifndef EVENTSETTINGWIDGEET_H
#define EVENTSETTINGWIDGEET_H

#include <QDialog>
class LVGLEventType;

QT_BEGIN_NAMESPACE
namespace Ui {
class EventSettingWidgeet;
}
QT_END_NAMESPACE

class EventSettingWidgeet : public QDialog {
  Q_OBJECT

 public:
  EventSettingWidgeet(LVGLEventType *event, int type,
                      QWidget *parent = nullptr);
  ~EventSettingWidgeet();

  void setTextList(const QStringList &list);
  QStringList textList();

 private slots:
  void on_pushButton_clicked();
  void on_pushButton_2_clicked();

 private:
  void init();
  void setSetPropvalue(const QStringList &list);
  void setscreenValue(const QStringList &list);
  void setAnimalValue(const QStringList &list);

 private:
  Ui::EventSettingWidgeet *ui;
  LVGLEventType *m_event;
  bool m_iscomb;
  int m_type;
  bool m_isobjcomhide;
  bool m_ispropcomhide;
  bool m_issendercomhide;
  bool m_isvaledithide;
  bool m_isval2comhide;
  bool m_isval3edithide;
  bool m_isvalue4edithide;
  bool m_isvalue5textedithide;
};
#endif  // EVENTSETTINGWIDGEET_H
