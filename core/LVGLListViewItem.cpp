#include "LVGLListViewItem.h"

#include <QFile>
#include <QTextStream>

#include "ui_LVGLListViewItem.h"

LVGLListViewItem::LVGLListViewItem(QWidget *parent)
    : QWidget(parent), ui(new Ui::LVGLListViewItem), m_clicked(false) {
  ui->setupUi(this);
  ui->toolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  connect(ui->toolButton, &QToolButton::clicked, this,
          &LVGLListViewItem::slot_toolbtnclicked);

  ui->list_widgets->hide();
  ui->list_widgets->setSpacing(2);
  ui->list_widgets->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  ui->list_widgets->setIconSize(QSize(25, 25));
  ui->list_widgets->setSpacing(0);

  QFile file(":/qss/normal/LVGLListViewItem_1.qss");
  file.open(QFile::ReadOnly);
  QTextStream filetext(&file);
  QString stylesheet1 = filetext.readAll().replace("\r\n", "");

  file.close();
  file.setFileName(":/qss/normal/LVGLListViewItem_2.qss");
  file.open(QFile::ReadOnly);
  filetext.setDevice(&file);
  QString stylesheet2 = filetext.readAll().replace("\r\n", "");
  m_toolbtnQSS1 = stylesheet1;
  m_toolbtnQSS2 = stylesheet2;
  this->setStyleSheet(m_toolbtnQSS1);
}

LVGLListViewItem::~LVGLListViewItem() { delete ui; }

LVGLWidgetListView *LVGLListViewItem::getlistview() {
  return this->ui->list_widgets;
}

void LVGLListViewItem::settoolbtnText(const QString &str) {
  ui->toolButton->setText(str);
}

void LVGLListViewItem::slot_toolbtnclicked() {
  m_clicked = !m_clicked;
  if (m_clicked) {
    ui->list_widgets->show();
    this->setStyleSheet(m_toolbtnQSS2);

    int btnheight = ui->toolButton->height();
    int row = ui->list_widgets->model()->rowCount();
    int rowheight = 0;
    if (row == 3)
      rowheight = row * 37;
    else
      rowheight = row * 33;

    this->setFixedHeight(btnheight + rowheight);
  } else {
    ui->list_widgets->hide();
    this->setStyleSheet(m_toolbtnQSS1);
    int btnheight = ui->toolButton->height();
    this->setFixedHeight(btnheight);
  }

  ui->toolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
}

void LVGLListViewItem::slot_toshowtab() { ui->list_widgets->show(); }
