#include "EventSelectWIdget.h"

#include "EventSettingWidgeet.h"
#include "LVGLEventScreen.h"
#include "LVGLEventType.h"
#include "LVGLEventWidgetBasic.h"
#include "ui_EventSelectWIdget.h"
#include "widgets/LVGLWidget.h"

int Index = 1;

EventSelectWIdget::EventSelectWIdget(LVGLWidget *w, QWidget *parent)
    : QDialog(parent),
      ui(new Ui::EventSelectWIdget),
      m_setWidget(nullptr),
      m_ev(nullptr) {
  ui->setupUi(this);
  ui->nameEdit->setText(QString("Event%1").arg(Index));
  QStringList triggerlist;

  switch (w->type()) {
    case LVGLWidget::Arc:
    case LVGLWidget::Bar:
    case LVGLWidget::Slider:
    case LVGLWidget::Roller: {
      triggerlist = QStringList() << "Value_Changed"
                                  << "Pressed"
                                  << "Clicked"
                                  << "Long_Pressed"
                                  << "Long_Pressed_Repeat"
                                  << "Focused"
                                  << "Defocused";
    } break;
    default: {
      triggerlist = QStringList() << "Pressed"
                                  << "Clicked"
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
                                      << "Line"
                                      << "Roller"
                                      << "Slider"
                                      << "Spinbox"
                                      << "Switch"
                                      << "TextArea";
  ui->objectcomb->addItems(objlist);
  connect(ui->typecomb, SIGNAL(currentIndexChanged(int)), this,
          SLOT(slotTypeChanged(int)));
}

EventSelectWIdget::~EventSelectWIdget() { delete ui; }

void EventSelectWIdget::setTextList(const QStringList &list) {}

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

void EventSelectWIdget::getEventType(LVGLEventType *&ev, int index) {
  switch (index) {
    case 0:
      ev = new LVGLEventType(0, LVGLEventType::BASIC);
      break;
    case 1:
      ev = new LVGLEventType(0, LVGLEventType::ARC);
      break;
    default:
      break;
  }
}
