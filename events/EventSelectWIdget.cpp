#include "EventSelectWIdget.h"

#include "EventSettingWidgeet.h"
#include "LVGLEvent.h"
#include "LVGLEventWidgetBasic.h"
#include "ui_EventSelectWIdget.h"

int Index = 1;

EventSelectWIdget::EventSelectWIdget(QWidget *parent)
    : QDialog(parent), ui(new Ui::EventSelectWIdget), m_setWidget(nullptr) {
  ui->setupUi(this);
  ui->nameEdit->setText(QString("Event%1").arg(Index));
  QStringList triggerlist = QStringList() << "Pressed"
                                          << "Clicked"
                                          << "Long_Pressed"
                                          << "Long_Pressed_Repeat"
                                          << "Focused"
                                          << "Defocused"
                                          << "Value_Changed";
  ui->triggercomb->addItems(triggerlist);
  QStringList typelist = QStringList() << "Set Property"
                                       << "Change Screen"
                                       << "Set Opacity"
                                       << "Play Animation";
  ui->typecomb->addItems(typelist);
  QStringList objlist = QStringList() << "Basic"
                                      << "Arc"
                                      << "Bar"
                                      << "Button"
                                      << "ButtonMatric"
                                      << "Calendar"
                                      << "Cancas"
                                      << "Chart"
                                      << "CheckBox";
  ui->objectcomb->addItems(objlist);
}

EventSelectWIdget::~EventSelectWIdget() { delete ui; }

void EventSelectWIdget::setTextList(const QStringList &list) {}

QStringList EventSelectWIdget::textList() {
  QStringList list;
  list << ui->nameEdit->text() + "@";
  list << ui->triggercomb->currentText() + "@";
  list << ui->typecomb->currentText() + "@";
  if (ui->typecomb->currentIndex() == 0)
    list << ui->objectcomb->currentText() + "@";
  list += m_setWidget->textList();
  return list;
}

void EventSelectWIdget::on_selectbtn_clicked() {
  int index = ui->objectcomb->currentIndex();
  LVGLEvent *ev = nullptr;
  switch (index) {
    default:
      ev = new LVGLEventWidgetBasic;
  }
  m_setWidget = new EventSettingWidgeet(ev, this);
  connect(m_setWidget, &QDialog::finished, this,
          &EventSelectWIdget::slotSetWFinished);
  m_setWidget->open();
}

void EventSelectWIdget::on_cancelbtn_clicked() { emit QDialog::reject(); }

void EventSelectWIdget::slotSetWFinished() {
  if (QDialog::Accepted == m_setWidget->result()) {
    ++Index;
    emit accept();
  }
}
