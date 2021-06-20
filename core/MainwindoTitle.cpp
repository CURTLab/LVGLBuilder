#include "MainwindoTitle.h"

#include <QMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QTextStream>

#include "ui_MainwindoTitle.h"

MainwindoTitle::MainwindoTitle(QWidget *parent)
    : QWidget(parent), ui(new Ui::MainwindoTitle) {
  ui->setupUi(this);
  QFile file(":/qss/normal/MainwindoTitle.qss");
  file.open(QFile::ReadOnly);
  QTextStream filetext(&file);
  QString stylesheet = filetext.readAll().replace("\r\n", "");
  this->setStyleSheet(stylesheet);
  file.close();
}

MainwindoTitle::~MainwindoTitle() { delete ui; }

void MainwindoTitle::paintEvent(QPaintEvent *event) {
  QStyleOption opt;
  opt.initFrom(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
  QWidget::paintEvent(event);
}

void MainwindoTitle::mousePressEvent(QMouseEvent *event) {
  if (event->button() & Qt::LeftButton) {
    auto parent = this->parentWidget();
    m_curPos = event->globalPos() - parent->frameGeometry().topLeft();
  }

  QWidget::mousePressEvent(event);
}

void MainwindoTitle::mouseMoveEvent(QMouseEvent *event) {
  if (event->buttons() & Qt::LeftButton) {
    auto parent = this->parentWidget();
    parent->move(event->globalPos() - m_curPos);
  }

  QWidget::mouseMoveEvent(event);
}

void MainwindoTitle::mouseDoubleClickEvent(QMouseEvent *event) {
  auto parent = this->parentWidget();

  if (parent->isMaximized())
    parent->showNormal();
  else
    parent->showMaximized();

  parent->move(0, 0);

  QWidget::mouseDoubleClickEvent(event);
}

void MainwindoTitle::on_pushButton_clicked() {
  auto parent = this->parentWidget();
  if (parent->isMaximized())
    parent->showNormal();
  else
    parent->showMaximized();
}

void MainwindoTitle::on_pushButton_2_clicked() {
  auto parent = this->parentWidget();
  parent->showMinimized();
}

void MainwindoTitle::on_pushButton_3_clicked() {
  auto parent = this->parentWidget();
  parent->close();
}
