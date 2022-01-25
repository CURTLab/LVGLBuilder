#include "LVGLNewDialog.h"

#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPixmap>
#include <QSettings>
#include <cmath>

#include "LVGLHelper.h"
#include "ui_LVGLNewDialog.h"

LVGLNewDialog::LVGLNewDialog(QWidget *parent)
    : QDialog(parent), m_ui(new Ui::LVGLNewDialog) {
  m_ui->setupUi(this);
  setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
  setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
  setAttribute(Qt::WA_StyledBackground);
  setAttribute(Qt::WA_TranslucentBackground);

  m_ui->title->setText(tr("New Project"));
  m_ui->filename->setText(tr("File Name:"));
  m_ui->res->setText(tr("Resolution:"));
  m_ui->width->setText(tr("Width:"));
  m_ui->height->setText(tr("Height:"));
  m_ui->pushButton->setText("Ok");
  m_ui->pushButton_2->setText("Cancel");
  m_ui->combo_resolution->addItem(tr("Costume"));
  m_ui->lineEdit->setText(LVGLHelper::getInstance().newProjectName());

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

  //#1473e6
  auto ms = m_ui->spin_width->contentsMargins();
  m_ui->spin_width->setContentsMargins(1000, ms.top(), ms.right(), ms.bottom());

  QFile file(":/qss/normal/LVGLNewDialog.qss");
  file.open(QFile::ReadOnly);
  QTextStream filetext(&file);
  QString stylesheet = filetext.readAll().replace("\r\n", "");
  this->setStyleSheet(stylesheet);
  file.close();

  int index = stylesheet.indexOf("QDialog#LVGLNewDialog{");
  QString str = stylesheet.mid(index);
  index = str.indexOf("background");
  m_bgcolor = QColor(str.mid(index + 11, 7));
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

void LVGLNewDialog::paintEvent(QPaintEvent *event) {
  QPainterPath path;
  path.setFillRule(Qt::WindingFill);
  path.addRect(10, 10, this->width() - 20, this->height() - 20);

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing, true);
  painter.fillPath(path, QBrush(m_bgcolor));

  int r = m_bgcolor.red() - 50;
  int g = m_bgcolor.green() - 50;
  int b = m_bgcolor.blue() - 50;
  r = r < 0 ? 0 : r;
  g = g < 0 ? 0 : g;
  b = b < 0 ? 0 : b;
  QColor shadowcolor(r, g, b, 50);
  for (int i = 0; i < 10; i++) {
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRect(10 - i, 10 - i, this->width() - (10 - i) * 2,
                 this->height() - (10 - i) * 2);
    shadowcolor.setAlpha(150 - sqrt(i) * 50);
    painter.setPen(shadowcolor);
    painter.drawPath(path);
  }
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
