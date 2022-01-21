#include "ColorPick.h"

#include <QColorDialog>

#include "ui_ColorPick.h"

ColorPick::ColorPick(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::ColorPick),
      m_colordig(new QColorDialog(this)),
      m_pix(nullptr) {
  ui->setupUi(this);
  m_curColor = QColor(Qt::red);
  m_colordig->setCurrentColor(m_curColor);
  m_pix = new QPixmap(this->width(), this->height());
  m_pix->fill(m_curColor);
  ui->toolButton->setIcon(QIcon(*m_pix));
  m_colordig->hide();
}

ColorPick::~ColorPick() {
  delete ui;
  delete m_pix;
}

QString ColorPick::colorString() {
  return QString().asprintf("#%02X%02X%02X", m_curColor.red(),
                            m_curColor.green(), m_curColor.blue());
}

void ColorPick::setColor(const QColor &col) {
  m_curColor = col;
  m_pix->fill(col);
  ui->toolButton->setIcon(QIcon(*m_pix));
}

void ColorPick::on_toolButton_clicked() {
  if (m_pix) delete m_pix;
  m_pix = new QPixmap(this->width(), this->height());
  m_colordig->setCurrentColor(m_curColor);
  m_colordig->show();
  if (m_colordig->exec() == QDialog::Accepted) {
    m_curColor = m_colordig->selectedColor();
    m_pix->fill(m_curColor);
    ui->toolButton->setIcon(QIcon(*m_pix));
  }
}
