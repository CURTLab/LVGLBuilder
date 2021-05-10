#include "EventsListWIdget.h"

#include "EventSelectWIdget.h"
#include "ui_EventsListWIdget.h"

EventsListWIdget::EventsListWIdget(LVGLWidget *w, QWidget *parent)
    : QDialog(parent),
      ui(new Ui::EventsListWIdget),
      m_selectW(nullptr),
      m_w(w) {
  ui->setupUi(this);
  ui->add->setIcon(QIcon(":/icons/add.png"));
  ui->remove->setIcon(QIcon(":/icons/delete.png"));
}

EventsListWIdget::~EventsListWIdget() { delete ui; }

void EventsListWIdget::setTextList(const QStringList &list) {
  for (int i = 0; i < list.size(); ++i) {
    ui->listWidget->addItem(list[i]);
  }
}

QStringList EventsListWIdget::textList() {
  QStringList list;
  for (int i = 0; i < ui->listWidget->count(); ++i) {
    list << ui->listWidget->item(i)->text();
  }
  return list;
}

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

void EventsListWIdget::on_ok_clicked() { accept(); }

void EventsListWIdget::on_cancel_clicked() { reject(); }

void EventsListWIdget::on_add_clicked() {
  m_selectW = new EventSelectWIdget(m_w, this);
  connect(m_selectW, &QDialog::finished, this,
          &EventsListWIdget::slotSelectWFinished);
  m_selectW->open();
}

void EventsListWIdget::on_remove_clicked() {
  if (Index > 1) --Index;
  QList<QListWidgetItem *> list = ui->listWidget->selectedItems();
  if (list.size() != 0) {
    QListWidgetItem *sel = list[0];
    if (sel) {
      int r = ui->listWidget->row(sel);
      ui->listWidget->takeItem(r);
    }
  } else {
    int r = ui->listWidget->count() - 1;
    ui->listWidget->takeItem(r);
  }
}
