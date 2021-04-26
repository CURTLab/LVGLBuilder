#include "EventsListWIdget.h"

#include "EventSelectWIdget.h"
#include "ui_EventsListWIdget.h"

EventsListWIdget::EventsListWIdget(QWidget *parent)
    : QDialog(parent), ui(new Ui::EventsListWIdget), m_selectW(nullptr) {
  ui->setupUi(this);
}

EventsListWIdget::~EventsListWIdget() { delete ui; }

void EventsListWIdget::setTextList(const QStringList &list) {}

QStringList EventsListWIdget::textList() const { return QStringList(); }

void EventsListWIdget::on_toolButton_clicked() {
  m_selectW = new EventSelectWIdget(this);
  connect(m_selectW, &QDialog::finished, this,
          &EventsListWIdget::slotSelectWFinished);
  m_selectW->open();
}

void EventsListWIdget::on_toolButton_2_clicked() {}

void EventsListWIdget::slotSelectWFinished() {
  setFocus();
  if (QDialog::Accepted == m_selectW->result()) {
    m_resultlist.clear();
    m_resultlist = m_selectW->textList();
    QString tmp;
    for (auto str : m_resultlist) tmp += str;
    ui->listWidget->addItem(tmp);
  }
}
