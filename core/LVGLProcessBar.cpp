#include "LVGLProcessBar.h"

#include <QDebug>
#include <QProcess>
#include <QThread>

#include "ui_LVGLProcessBar.h"

LVGLProcessBar::LVGLProcessBar(QWidget *parent)
    : QDialog(parent), ui(new Ui::LVGLProcessBar) {
  ui->setupUi(this);
  ui->progressBar->setRange(0, 0);
  ui->progressBar->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  setFixedSize(376, 38);
}

LVGLProcessBar::~LVGLProcessBar() { delete ui; }

void LVGLProcessBar::closeEvent(QCloseEvent *event) {
  emit sigclose();
  QDialog::closeEvent(event);
}
