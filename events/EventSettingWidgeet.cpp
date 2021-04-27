#include "EventSettingWidgeet.h"

#include "LVGLEvent.h"
#include "ui_EventSettingWidgeet.h"

EventSettingWidgeet::EventSettingWidgeet(LVGLEvent *event, int type,
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
    list << ui->objcomb->currentText() + "@";
    list << ui->propcomb->currentText() + "@";
    if (m_iscomb)
      list << ui->value2comb->currentText() + "@";
    else
      list << ui->valueedit->text() + "@";
  } else if (1 == m_type) {
  } else if (2 == m_type) {
  } else if (3 == m_type) {
  }
  return list;
}

void EventSettingWidgeet::on_pushButton_clicked() { emit accept(); }

void EventSettingWidgeet::on_pushButton_2_clicked() { emit reject(); }

void EventSettingWidgeet::init() {
  switch (m_type) {
    case 0: {
      ui->objectlab->setText(tr("Object"));
      ui->objcomb->addItems(m_event->getObjName());
      ui->proplab->setText(tr("Property"));
      ui->propcomb->addItems(m_event->getPropertyList());
      int wtype = m_event->whichwidget();
      if (LVGLEvent::e_LINEEDIT == wtype) {
        ui->value2lab->hide();
        ui->value2comb->hide();
        ui->valuelab->setText(tr("Value"));
      } else if (LVGLEvent::e_COMBOBOX == wtype) {
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
    } break;
    case 2:
      break;
    case 3:
      break;
  }
}
