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
      m_type(type),
      m_isobjcomhide(false),
      m_ispropcomhide(false),
      m_issendercomhide(false),
      m_isvaledithide(false),
      m_isval2comhide(false),
      m_isval3edithide(false) {
  ui->setupUi(this);
  init();
}

EventSettingWidgeet::~EventSettingWidgeet() { delete ui; }

void EventSettingWidgeet::setTextList(const QStringList &list) {}

QStringList EventSettingWidgeet::textList() {
  QStringList list;
  if (!m_isobjcomhide) list << ui->objcomb->currentText() + "#";
  if (!m_ispropcomhide) list << ui->propcomb->currentText() + "#";
  if (!m_issendercomhide) list << ui->Sendercomb->currentText() + "#";
  if (!m_isvaledithide) list << ui->valueedit->text() + "#";
  if (!m_isval2comhide) list << ui->value2comb->currentText() + "#";
  if (!m_isval3edithide) list << ui->value3edit->text() + "#";
  if (!m_isvalue4edithide) list << ui->value4edit->text() + "#";
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
  m_isobjcomhide = false;
  m_ispropcomhide = false;
  m_issendercomhide = false;
  m_isvaledithide = false;
  m_isval2comhide = false;
  m_isval3edithide = false;
  m_isvalue4edithide = true;

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
        m_isval2comhide = true;
        ui->valuelab->setText(tr("Value"));
      } else if (Role == 1) {
        m_isvaledithide = true;
        ui->value2lab->setText(tr("Value"));
        ui->value2comb->addItems(m_event->getValuelist());
      }
      m_isval3edithide = true;

    } break;
    case 1: {
      ui->objectlab->setText(tr("Screen to"));
      ui->proplab->setText(tr("Fade mode"));
      ui->valuelab->setText(tr("Speed"));
      ui->value3lab->setText(tr("Delay"));
      ui->objcomb->addItems(m_event->getScreenName());
      ui->propcomb->addItems(m_event->getFadeModeList());
      ui->valueedit->setText("150");
      ui->value3edit->setText("0");
      m_isval2comhide = true;
      m_issendercomhide = true;
    } break;
    case 2: {
      ui->objectlab->setText(tr("Target"));
      ui->proplab->setText(tr("Animation path"));
      ui->valuelab->setText(tr("X"));
      ui->value3lab->setText(tr("Y"));
      ui->objcomb->addItems(m_event->getanimObjList());
      ui->propcomb->addItems(m_event->getanimPathList());
      ui->valueedit->setText("0");
      ui->value3edit->setText("0");
      ui->value4lab->setText("Duration");
      ui->value4edit->setText("100");
      m_isval2comhide = true;
      m_issendercomhide = true;
      m_isvalue4edithide = false;
    } break;
    case 3:
      break;
  }

  if (m_event->getwtype() == LVGLEventType::SWITCH) m_issendercomhide = true;

  if (m_issendercomhide) {
    ui->Sendercomb->hide();
    ui->Senderlab->hide();
  }
  if (m_isvaledithide) {
    ui->valuelab->hide();
    ui->valueedit->hide();
  }
  if (m_isval2comhide) {
    ui->value2lab->hide();
    ui->value2comb->hide();
  }
  if (m_isval3edithide) {
    ui->value3lab->hide();
    ui->value3edit->hide();
  }
  if (m_isvalue4edithide) {
    ui->value4lab->hide();
    ui->value4edit->hide();
  }
}
