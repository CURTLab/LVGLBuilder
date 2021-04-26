#include "EventSelectWIdget.h"

#include "EventSettingWidgeet.h"
#include "ui_EventSelectWIdget.h"

EventSelectWIdget::EventSelectWIdget(QWidget *parent)
    : QDialog(parent), ui(new Ui::EventSelectWIdget), m_setWidget(nullptr) {
  ui->setupUi(this);
}

EventSelectWIdget::~EventSelectWIdget() { delete ui; }

void EventSelectWIdget::setTextList(const QStringList &list) {}

QStringList EventSelectWIdget::textList() const { return QStringList(); }

void EventSelectWIdget::on_selectbtn_clicked() {
  m_setWidget = new EventSettingWidgeet(this);
  if (m_setWidget->exec()) {
  }
}

void EventSelectWIdget::on_cancelbtn_clicked() { emit QDialog::reject(); }
