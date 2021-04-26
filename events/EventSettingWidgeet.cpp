#include "EventSettingWidgeet.h"

#include "LVGLEvent.h"
#include "ui_EventSettingWidgeet.h"

EventSettingWidgeet::EventSettingWidgeet(LVGLEvent *event, QWidget *parent)
    : QDialog(parent),
      ui(new Ui::EventSettingWidgeet),
      m_event(event),
      m_iscomb(false) {
  ui->setupUi(this);
  ui->value2comb->hide();
  ui->value2lab->hide();
  ui->objcomb->addItems(m_event->getObjName());
  ui->propcomb->addItems(m_event->getPropertyList());
}

EventSettingWidgeet::~EventSettingWidgeet() { delete ui; }

void EventSettingWidgeet::setTextList(const QStringList &list) {}

QStringList EventSettingWidgeet::textList() {
  QStringList list;
  list << ui->objcomb->currentText() + "@";
  list << ui->propcomb->currentText() + "@";
  if (m_iscomb)
    list << ui->value2comb->currentText() + "@";
  else
    list << ui->valueedit->text() + "@";
  return list;
}

void EventSettingWidgeet::on_pushButton_clicked() { emit accept(); }

void EventSettingWidgeet::on_pushButton_2_clicked() { emit reject(); }
