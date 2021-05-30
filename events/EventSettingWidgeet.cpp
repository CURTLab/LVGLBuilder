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

void EventSettingWidgeet::setTextList(const QStringList &list) {
  auto type = list[2];
  if (type == "Set Property") {
    setSetPropvalue(list);
  } else if (type == "Change Screen") {
    setscreenValue(list);
  } else if (type == "Play Animation") {
    setAnimalValue(list);
  }
}

QStringList EventSettingWidgeet::textList() {
  QStringList list;
  if (!m_isobjcomhide) list << ui->objcomb->currentText() + "#";
  if (!m_ispropcomhide) list << ui->propcomb->currentText() + "#";
  if (!m_issendercomhide) list << ui->Sendercomb->currentText() + "#";
  if (!m_isvaledithide) list << ui->valueedit->text() + "#";
  if (!m_isval2comhide) list << ui->value2comb->currentText() + "#";
  if (!m_isval3edithide) list << ui->value3edit->text() + "#";
  if (!m_isvalue4edithide) list << ui->value4edit->text() + "#";
  if (!m_isvalue5textedithide) list << ui->valut5textEdit->toPlainText() + "#";
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
  m_isvalue5textedithide = true;

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
      } else if (Role == 2) {
        m_isval2comhide = true;
        m_isvaledithide = true;
        m_isvalue5textedithide = false;
        ui->value5lab->setText(tr("Value"));
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

  if (m_type == 0) m_issendercomhide = !m_event->getneedCusVal();

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
  if (m_isvalue5textedithide) {
    ui->value5lab->hide();
    ui->valut5textEdit->hide();
  }
}

void EventSettingWidgeet::setSetPropvalue(const QStringList &list) {
  if (!m_isobjcomhide) {
    int objindex = ui->objcomb->findText(list[4]);
    ui->objcomb->setCurrentIndex(objindex);
  }

  if (!m_ispropcomhide) {
    int propindex = ui->propcomb->findText(list[5]);
    ui->propcomb->setCurrentIndex(propindex);
  }

  if (!m_issendercomhide) {
    int isdindex = ui->Sendercomb->findText(list[6]);
    ui->Sendercomb->setCurrentIndex(isdindex);
    if (!m_isvaledithide) {
      ui->valueedit->setText(list[7]);
      if (!m_isval2comhide) {
        auto v2index = ui->value2comb->findText(list[8]);
        ui->value2comb->setCurrentIndex(v2index);
      }
      if (!m_isval3edithide) {
        ui->value3edit->setText(list[9]);
      }
      if (!m_isvalue4edithide) {
        ui->value4edit->setText(list[10]);
      }
      if (!m_isvalue5textedithide) {
        ui->valut5textEdit->setText(list[11]);
      }
    }
    if (!m_isval2comhide) {
      auto v2index = ui->value2comb->findText(list[7]);
      ui->value2comb->setCurrentIndex(v2index);

      if (!m_isval3edithide) {
        ui->value3edit->setText(list[8]);
      }
      if (!m_isvalue4edithide) {
        ui->value4edit->setText(list[9]);
      }
      if (!m_isvalue5textedithide) {
        ui->valut5textEdit->setText(list[10]);
      }
    }
    if (!m_isval3edithide) {
      ui->value3edit->setText(list[7]);

      if (!m_isvalue4edithide) {
        ui->value4edit->setText(list[8]);
      }
      if (!m_isvalue5textedithide) {
        ui->valut5textEdit->setText(list[9]);
      }
    }
    if (!m_isvalue4edithide) {
      ui->value4edit->setText(list[7]);
      if (!m_isvalue5textedithide) {
        ui->valut5textEdit->setText(list[8]);
      }
    }
    if (!m_isvalue5textedithide) {
      ui->valut5textEdit->setText(list[7]);
    }
  } else {
    if (!m_isvaledithide) {
      ui->valueedit->setText(list[6]);
      if (!m_isval2comhide) {
        auto v2index = ui->value2comb->findText(list[7]);
        ui->value2comb->setCurrentIndex(v2index);
      }
      if (!m_isval3edithide) {
        ui->value3edit->setText(list[8]);
      }
      if (!m_isvalue4edithide) {
        ui->value4edit->setText(list[9]);
      }
      if (!m_isvalue5textedithide) {
        ui->valut5textEdit->setText(list[10]);
      }
    }
    if (!m_isval2comhide) {
      auto v2index = ui->value2comb->findText(list[6]);
      ui->value2comb->setCurrentIndex(v2index);

      if (!m_isval3edithide) {
        ui->value3edit->setText(list[7]);
      }
      if (!m_isvalue4edithide) {
        ui->value4edit->setText(list[8]);
      }
      if (!m_isvalue5textedithide) {
        ui->valut5textEdit->setText(list[9]);
      }
    }
    if (!m_isval3edithide) {
      ui->value3edit->setText(list[6]);

      if (!m_isvalue4edithide) {
        ui->value4edit->setText(list[7]);
      }
      if (!m_isvalue5textedithide) {
        ui->valut5textEdit->setText(list[8]);
      }
    }
    if (!m_isvalue4edithide) {
      ui->value4edit->setText(list[6]);
      if (!m_isvalue5textedithide) {
        ui->valut5textEdit->setText(list[7]);
      }
    }
    if (!m_isvalue5textedithide) {
      ui->valut5textEdit->setText(list[6]);
    }
  }
}

void EventSettingWidgeet::setscreenValue(const QStringList &list) {
  int objindex = ui->objcomb->findText(list[3]);
  ui->objcomb->setCurrentIndex(objindex);

  int propindex = ui->propcomb->findText(list[4]);
  ui->propcomb->setCurrentIndex(propindex);

  ui->valueedit->setText(list[5]);
  ui->value3edit->setText(list[6]);
}

void EventSettingWidgeet::setAnimalValue(const QStringList &list) {
  int objindex = ui->objcomb->findText(list[3]);
  ui->objcomb->setCurrentIndex(objindex);

  int propindex = ui->propcomb->findText(list[4]);
  ui->propcomb->setCurrentIndex(propindex);

  ui->valueedit->setText(list[5]);
  ui->value3edit->setText(list[6]);
  ui->value4edit->setText(list[7]);
}
