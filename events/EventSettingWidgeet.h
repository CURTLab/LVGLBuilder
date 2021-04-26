#ifndef EVENTSETTINGWIDGEET_H
#define EVENTSETTINGWIDGEET_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class EventSettingWidgeet;
}
QT_END_NAMESPACE

class EventSettingWidgeet : public QDialog {
  Q_OBJECT

 public:
  EventSettingWidgeet(QWidget *parent = nullptr);
  ~EventSettingWidgeet();

 private:
  Ui::EventSettingWidgeet *ui;
};
#endif  // EVENTSETTINGWIDGEET_H
