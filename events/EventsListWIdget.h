#ifndef EVENTSLISTWIDGET_H
#define EVENTSLISTWIDGET_H

#include <QDialog>
class EventSelectWIdget;

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

 private slots:
  void on_toolButton_clicked();
  void on_toolButton_2_clicked();
  void slotSelectWFinished();

 private:
  Ui::EventsListWIdget *ui;
  EventSelectWIdget *m_selectW;
  QStringList m_resultlist;
};
#endif  // EVENTSLISTWIDGET_H
