#include "LVGLNewDialog.h"

#include <QDebug>
#include <QMessageBox>
#include <QPixmap>
#include <QSettings>

#include "LVGLHelper.h"
#include "ui_LVGLNewDialog.h"

LVGLNewDialog::LVGLNewDialog(QWidget *parent)
    : QDialog(parent), m_ui(new Ui::LVGLNewDialog) {
  m_ui->setupUi(this);
  m_ui->title->setText(tr("New Project"));
  m_ui->filename->setText(tr("File Name:"));
  m_ui->res->setText(tr("Resolution:"));
  m_ui->width->setText(tr("Width:"));
  m_ui->height->setText(tr("Height:"));
  m_ui->pushButton->setText("Ok");
  m_ui->pushButton_2->setText("Cancel");
  m_ui->combo_resolution->addItem(tr("Costume"));
  m_ui->lineEdit->setText(LVGLHelper::getInstance().newProjectName());
  setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
  setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
  m_ui->close->setStyleSheet(
      "QPushButton{background-color: #D8D8D8;border:0px;}QPushButton:hover "
      "{background-color:#D8D8D8;border:2px solid #979797;}");

  m_ui->frame->setStyleSheet(
      "QFrame#frame{background:#D8D8D8;border:1px solid #979797;}");
  m_resolutions << qMakePair(480, 320);
  m_resolutions << qMakePair(320, 240);
  m_resolutions << qMakePair(160, 120);

  for (const auto &r : m_resolutions)
    m_ui->combo_resolution->addItem(
        QString("%1 x %2").arg(r.first).arg(r.second));

  connect(
      m_ui->combo_resolution,
      static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
      this, &LVGLNewDialog::resolutionChanged);

  QSettings settings("at.fhooe.lvgl", "LVGL Builder");

  m_ui->combo_resolution->setCurrentIndex(
      settings.value("resolution index", 0).toInt());
  const QSize size = settings.value("resolution", QSize(1, 1)).toSize();
  m_ui->spin_width->setValue(size.width());
  m_ui->spin_height->setValue(size.height());
  connect(m_ui->pushButton, &QPushButton::clicked, this,
          &LVGLNewDialog::accept);
  connect(m_ui->pushButton_2, &QPushButton::clicked, this,
          &LVGLNewDialog::reject);
  m_ui->lineEdit->setStyleSheet(
      "QLineEdit#lineEdit{border:0px;background:#F8F8F8;}");
  m_ui->spin_width->setStyleSheet(
      "QSpinBox{border: 1px solid gray;"
      "border-radius: 3px;"
      "padding: 1px 18px 1px 13px;};");
  m_ui->spin_height->setStyleSheet(
      "QSpinBox{border: 1px solid gray;"
      "border-radius: 3px;"
      "padding: 1px 18px 1px 13px;};");
  m_ui->combo_resolution->setStyleSheet(
      "QComboBox#combo_resolution{ border: 1px solid gray;"
      "border-radius: 3px;"
      "padding: 1px 18px 1px 13px; "
      "color: #000000;"
      "background:#ffffff;}"
      "QComboBox::down-arrow#combo_resolution {"
      "image: url(:/icons/arrows.png);}"
      "QComboBox::drop-down {"
      "subcontrol-origin: padding;"
      "subcontrol-position: top right;"
      "width: 30px;"
      "border-left-width: 0px;"
      "border-left-color: gray;"
      "border-left-style: solid;"
      "border-top-right-radius: 3px;"
      "border-bottom-right-radius: 3px;}");

  setStyleSheet("QDialog#LVGLNewDialog{border:0px;background:#F8F8F8;}");
  //#1473e6
  m_ui->pushButton->setStyleSheet(
      "QPushButton{color: white;border-radius: "
      "12px;background-color: #1473e6;min-width: "
      "80px;min-height:26px;}QPushButton:pressed {background-color: "
      "#0F64D2;}QPushButton:hover {background-color:#0F64D2;} ");
  m_ui->pushButton_2->setStyleSheet(
      "QPushButton{color: black;border:2px solid #979797;border-radius: "
      "12px;background-color: #F8F8F8;min-width: "
      "76px;min-height:22px;}QPushButton:pressed {background-color: "
      "#D8D8D8;}QPushButton:hover {background-color:#D8D8D8;} ");
  auto ms = m_ui->spin_width->contentsMargins();
  m_ui->spin_width->setContentsMargins(1000, ms.top(), ms.right(), ms.bottom());
}

LVGLNewDialog::~LVGLNewDialog() { delete m_ui; }

QString LVGLNewDialog::selectName() const {
  return m_ui->lineEdit->text().isEmpty() ? "Homepage" : m_ui->lineEdit->text();
}

QSize LVGLNewDialog::selectedResolution() const {
  int w = m_ui->spin_width->value();
  int h = m_ui->spin_height->value();
  return QSize(w, h);
}

void LVGLNewDialog::setoptenable(bool able) {
  m_ui->spin_height->setEnabled(able);
  m_ui->spin_width->setEnabled(able);
  m_ui->combo_resolution->setEnabled(able);
}

void LVGLNewDialog::accept() {
  if (m_ui->lineEdit->text().isEmpty()) {
    QMessageBox::warning(this, tr("Error"), tr("Project name is empty!"));
    return;
  }
  if (0 == m_ui->spin_width->value()) {
    QMessageBox::warning(this, tr("Error"), tr("Width can't set 0"));
    return;
  }
  if (0 == m_ui->spin_height->value()) {
    QMessageBox::warning(this, tr("Error"), tr("Height can't set 0"));
    return;
  }
  QSettings settings("at.fhooe.lvgl", "LVGL Builder");
  settings.setValue("resolution index", m_ui->combo_resolution->currentIndex());
  settings.setValue("resolution", QSize(m_ui->spin_width->value(),
                                        m_ui->spin_height->value()));
  QDialog::accept();
}

void LVGLNewDialog::mousePressEvent(QMouseEvent *e) {
  if (e->button() & Qt::LeftButton)
    m_curPos = e->globalPos() - frameGeometry().topLeft();

  QDialog::mousePressEvent(e);
}

void LVGLNewDialog::mouseMoveEvent(QMouseEvent *e) {
  if (e->buttons() & Qt::LeftButton) move(e->globalPos() - m_curPos);

  QDialog::mouseMoveEvent(e);
}

void LVGLNewDialog::keyPressEvent(QKeyEvent *e) {
  if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) accept();

  QDialog::keyPressEvent(e);
}

void LVGLNewDialog::resolutionChanged(int index) {
  m_ui->spin_width->setEnabled(index == 0);
  m_ui->spin_height->setEnabled(index == 0);

  if (index > 0) {
    m_ui->spin_width->setValue(m_resolutions[index - 1].first);
    m_ui->spin_height->setValue(m_resolutions[index - 1].second);
  }
}

void LVGLNewDialog::on_close_clicked() { emit reject(); }
