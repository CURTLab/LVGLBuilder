#include "EventSelectWIdget.h"

#include <QMessageBox>

#include "EventSettingWidgeet.h"
#include "LVGLEventScreen.h"
#include "LVGLEventType.h"
#include "LVGLEventWidgetBasic.h"
#include "core/LVGLHelper.h"
#include "ui_EventSelectWIdget.h"
#include "widgets/LVGLWidget.h"

EventSelectWIdget::EventSelectWIdget(LVGLWidget *w, QWidget *parent)
    : QDialog(parent),
      ui(new Ui::EventSelectWIdget),
      m_setWidget(nullptr),
      m_ev(nullptr) {
  ui->setupUi(this);
  ui->nameEdit->setText(LVGLHelper::getInstance().newEventName());
  QStringList triggerlist;

  switch (w->type()) {
    case LVGLWidget::Arc:
    case LVGLWidget::Bar:
    case LVGLWidget::Slider:
    case LVGLWidget::Roller: {
      triggerlist = QStringList() << "Value_Changed"
                                  << "Clicked"
                                  << "Pressed"
                                  << "Long_Pressed"
                                  << "Long_Pressed_Repeat"
                                  << "Focused"
                                  << "Defocused";
    } break;
    default: {
      triggerlist = QStringList() << "Clicked"
                                  << "Pressed"
                                  << "Long_Pressed"
                                  << "Value_Changed"
                                  << "Long_Pressed_Repeat"
                                  << "Focused"
                                  << "Defocused";
    }
  }

  ui->triggercomb->addItems(triggerlist);
  QStringList typelist = QStringList() << "Set Property"
                                       << "Change Screen"
                                       << "Play Animation";
  //<< "Change Style";
  ui->typecomb->addItems(typelist);
  QStringList objlist = QStringList() << "Basic"
                                      << "Arc"
                                      << "Bar"
                                      << "Button"
                                      << "CheckBox"
                                      << "DropDownList"
                                      << "Image"
                                      << "Image Button"
                                      << "Label"
                                      << "Led"
                                      << "Roller"
                                      << "Slider"
                                      << "Spinbox"
                                      << "Switch"
                                      << "TextArea";
  ui->objectcomb->addItems(objlist);
  connect(ui->typecomb, SIGNAL(currentIndexChanged(int)), this,
          SLOT(slotTypeChanged(int)));
  connect(ui->nameEdit, &QLineEdit::editingFinished, this,
          &EventSelectWIdget::slotFinishEdit);
}

EventSelectWIdget::~EventSelectWIdget() { delete ui; }

void EventSelectWIdget::setTextList(const QStringList &list) {
  if (list.isEmpty()) return;

  auto eventname = list[0];
  ui->nameEdit->setText(eventname);

  auto eventtrigger = list[1];
  int trindex = ui->triggercomb->findText(eventtrigger);
  ui->triggercomb->setCurrentIndex(trindex);

  auto eventtype = list[2];
  int type = ui->typecomb->findText(eventtype);
  ui->typecomb->setCurrentIndex(type);

  int index = 0;
  if (type == 0) {
    index = ui->objectcomb->findText(list[3]);
    ui->objectcomb->setCurrentIndex(index);
  }

  if (m_ev) {
    delete m_ev;
    m_ev = nullptr;
  }

  switch (type) {
    case 0:
      getEventType(m_ev, index);
      break;
    case 1:
      m_ev = new LVGLEventType(1);
      break;
    case 2:
      m_ev = new LVGLEventType(2);
  }
  m_setWidget = new EventSettingWidgeet(m_ev, type, this);
  m_setWidget->setTextList(list);
  connect(m_setWidget, &QDialog::finished, this,
          &EventSelectWIdget::slotSetWFinished);
  m_setWidget->open();
}

QStringList EventSelectWIdget::textList() {
  QStringList list;
  list << ui->nameEdit->text() + "#";
  list << ui->triggercomb->currentText() + "#";
  list << ui->typecomb->currentText() + "#";
  if (ui->typecomb->currentIndex() == 0)
    list << ui->objectcomb->currentText() + "#";
  list += m_setWidget->textList();
  return list;
}

void EventSelectWIdget::on_selectbtn_clicked() {
  if (ui->nameEdit->text().isEmpty()) {
    QMessageBox::warning(this, tr("Error"), tr("Need the name of the event!"));
    return;
  }
  int index = ui->objectcomb->currentIndex();
  int type = ui->typecomb->currentIndex();
  if (m_ev) {
    delete m_ev;
    m_ev = nullptr;
  }
  switch (type) {
    case 0:
      getEventType(m_ev, index);
      break;
    case 1:
      m_ev = new LVGLEventType(1);
      break;
    case 2:
      m_ev = new LVGLEventType(2);
  }
  m_setWidget = new EventSettingWidgeet(m_ev, type, this);
  connect(m_setWidget, &QDialog::finished, this,
          &EventSelectWIdget::slotSetWFinished);
  m_setWidget->open();
}

void EventSelectWIdget::on_cancelbtn_clicked() { emit QDialog::reject(); }

void EventSelectWIdget::slotSetWFinished() {
  if (QDialog::Accepted == m_setWidget->result()) {
    emit accept();
  }
}

void EventSelectWIdget::slotTypeChanged(int index) {
  if (0 == index) {
    ui->objectlab->show();
    ui->objectcomb->show();
  } else {
    ui->objectlab->hide();
    ui->objectcomb->hide();
  }
}

void EventSelectWIdget::slotFinishEdit() {
  auto &list = LVGLHelper::getInstance().getEventName();
  if (list.contains(ui->nameEdit->text())) {
    ui->nameEdit->clear();
    QMessageBox::warning(this, tr("Error"),
                         tr("The name is already used by other events!"));
  }
}

void EventSelectWIdget::getEventType(LVGLEventType *&ev, int index) {
  switch (index) {
    case LVGLEventType::BASIC:
      ev = new LVGLEventType(0, LVGLEventType::BASIC);
      break;
    case LVGLEventType::ARC:
      ev = new LVGLEventType(0, LVGLEventType::ARC);
      break;
    case LVGLEventType::BAR:
      ev = new LVGLEventType(0, LVGLEventType::BAR);
      break;
    case LVGLEventType::BUTTON:
      ev = new LVGLEventType(0, LVGLEventType::BUTTON, 1);
      ev->setneedCusVal(false);
      break;
    case LVGLEventType::CHECKBOX:
      ev = new LVGLEventType(0, LVGLEventType::CHECKBOX, 1);
      ev->setneedCusVal(false);
      break;
    case LVGLEventType::DROPDOWNLIST:
      ev = new LVGLEventType(0, LVGLEventType::DROPDOWNLIST);
      break;
    case LVGLEventType::IMAGE:
      ev = new LVGLEventType(0, LVGLEventType::IMAGE, 1);
      ev->setneedCusVal(false);
      break;
    case LVGLEventType::IMAGEBUTTON:
      ev = new LVGLEventType(0, LVGLEventType::IMAGEBUTTON, 1);
      ev->setneedCusVal(false);
      break;
    case LVGLEventType::LABEL:
      ev = new LVGLEventType(0, LVGLEventType::LABEL);
      break;
    case LVGLEventType::LED:
      ev = new LVGLEventType(0, LVGLEventType::LED, 1);
      ev->setneedCusVal(false);
      break;
    case LVGLEventType::ROLLER:
      ev = new LVGLEventType(0, LVGLEventType::ROLLER);
      break;
    case LVGLEventType::SLIDER:
      ev = new LVGLEventType(0, LVGLEventType::SLIDER);
      break;
    case LVGLEventType::SWITCH:
      ev = new LVGLEventType(0, LVGLEventType::SWITCH, 1);
      ev->setneedCusVal(false);
      break;
    case LVGLEventType::TEXTAREA:
      ev = new LVGLEventType(0, LVGLEventType::TEXTAREA, 2);
      ev->setneedCusVal(false);
      break;
  }
}
