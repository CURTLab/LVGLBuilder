#include "EventSettingWidgeet.h"

#include "LVGLEventType.h"
#include "QMessageBox"
#include "ui_EventSettingWidgeet.h"

EventSettingWidgeet::EventSettingWidgeet(LVGLEventType *event, int type,
                                         QWidget *parent)
    : QDialog(parent),
      ui(new Ui::EventSettingWidgeet),
      m_event(event),
      m_iscomb(false),
      m_type(type) {
  ui->setupUi(this);
  init();
}

EventSettingWidgeet::~EventSettingWidgeet() { delete ui; }

void EventSettingWidgeet::setTextList(const QStringList &list) {}

QStringList EventSettingWidgeet::textList() {
  QStringList list;
  if (0 == m_type) {
    list << ui->objcomb->currentText() + "#";
    list << ui->propcomb->currentText() + "#";
    list << ui->Sendercomb->currentText() + "#";
    if (m_iscomb)
      list << ui->value2comb->currentText() + "#";
    else
      list << ui->valueedit->text() + "#";
  } else if (1 == m_type) {
    list << ui->objcomb->currentText() + "#";
    list << ui->propcomb->currentText() + "#";
    list << ui->valueedit->text() + "#";
    list << ui->value3edit->text() + "#";
  } else if (2 == m_type) {
  } else if (3 == m_type) {
  }
  return list;
}

void EventSettingWidgeet::on_pushButton_clicked() {
  if (0 == ui->objcomb->count()) {
    auto str = ui->objectlab->text();
    QMessageBox::warning(NULL, tr("Error"), str + tr(" can't be empty!"));
    return;
  }
  emit accept();
}

void EventSettingWidgeet::on_pushButton_2_clicked() { emit reject(); }

void EventSettingWidgeet::init() {
  switch (m_type) {
    case 0: {
      ui->objectlab->setText(tr("Object"));
      ui->objcomb->addItems(m_event->getObjName());
      ui->proplab->setText(tr("Property"));
      ui->propcomb->addItems(m_event->getPropertyList());
      ui->Senderlab->setText(tr("Sender Value"));
      ui->Sendercomb->addItem(tr("Not use"));
      ui->Sendercomb->addItem(tr("Use"));

      int Role = m_event->getRole();
      if (Role == 0) {
        ui->value2lab->hide();
        ui->value2comb->hide();
        ui->valuelab->setText(tr("Value"));
      } else if (Role == 1) {
        ui->valuelab->hide();
        ui->valueedit->hide();
        ui->value2lab->setText(tr("Value"));
        ui->value2comb->addItems(m_event->getValuelist());
      }
      ui->value3lab->hide();
      ui->value3edit->hide();
    } break;
    case 1: {
      ui->objectlab->setText(tr("Screen to"));
      ui->proplab->setText(tr("Fade mode"));
      ui->valuelab->setText(tr("Speed"));
      ui->value3lab->setText(tr("Delay"));
      ui->value2lab->hide();
      ui->value2comb->hide();
      ui->objcomb->addItems(m_event->getScreenName());
      ui->propcomb->addItems(m_event->getFadeModeList());
      ui->valueedit->setText("150");
      ui->value3edit->setText("0");
      ui->Sendercomb->hide();
      ui->Senderlab->hide();
    } break;
    case 2:
      break;
    case 3:
      break;
  }
}
