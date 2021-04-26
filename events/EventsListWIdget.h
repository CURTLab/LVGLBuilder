#ifndef EVENTSLISTWIDGET_H
#define EVENTSLISTWIDGET_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class EventsListWIdget;
}
QT_END_NAMESPACE

class EventsListWIdget : public QDialog {
  Q_OBJECT

 public:
  EventsListWIdget(QWidget *parent = nullptr);
  ~EventsListWIdget();

  void setTextList(const QStringList &list);
  inline QStringList textList() const;

 private:
  Ui::EventsListWIdget *ui;
};
#endif  // EVENTSLISTWIDGET_H
