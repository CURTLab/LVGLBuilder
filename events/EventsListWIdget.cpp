#include "EventsListWIdget.h"

#include "ui_EventsListWIdget.h"

EventsListWIdget::EventsListWIdget(QWidget *parent)
    : QDialog(parent), ui(new Ui::EventsListWIdget) {
  ui->setupUi(this);
}

EventsListWIdget::~EventsListWIdget() { delete ui; }

void EventsListWIdget::setTextList(const QStringList &list) {}

QStringList EventsListWIdget::textList() const { return QStringList(); }
