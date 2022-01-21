#include "ListDelegate.h"

#include <QDebug>
#include <QFont>
#include <QPainter>

ListDelegate::ListDelegate(QObject *parent) {}

ListDelegate::~ListDelegate() {}

void ListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                         const QModelIndex &index) const {
  QStyleOptionViewItem viewoption(option);
  QStyledItemDelegate::paint(painter, viewoption, index);
  QString name = index.data().toString();
  int x = option.rect.x();
  int y = option.rect.y();
  QRect seletrec = option.rect;
  painter->setPen(QColor(0xff, 0xff, 0xff));
  painter->setBrush(QColor(0xff, 0xff, 0xff));
  painter->drawRect(seletrec);

  seletrec.adjust(2, 2, -2, -2);
  if (option.state & QStyle::State_Selected) {
    painter->setBrush(QColor(0xd1, 0xe5, 0xca));
    painter->drawRoundedRect(seletrec, 2, 2);
    painter->setBrush(Qt::NoBrush);
  }

  QPoint left(x + 8, y + 5);
  QPoint right(x + 42, y + 16);
  QRect rec(left, right);
  QPoint left2(x + 62, y + 3);
  QPoint right2(x + 285, y + 17);
  QRect rec2(left2, right2);
  QFont font("黑体", 10);
  painter->setFont(font);
  painter->setPen(Qt::black);
  painter->setRenderHint(QPainter::SmoothPixmapTransform);

  if (name == QObject::tr("Arc")) {
    QPixmap pixmap = QPixmap(":/icons/Arc.png");
    pixmap.scaled(QSize(62, 15), Qt::KeepAspectRatio);
    painter->drawPixmap(rec, pixmap);
  } else if (name == QObject::tr("Bar")) {
    QPixmap pixmap = QPixmap(":/icons/Bar.png");
    painter->drawPixmap(rec, pixmap);
  } else if (name == QObject::tr("Button")) {
    QPixmap pixmap = QPixmap(":/icons/Button.png");
    pixmap.scaled(QSize(62, 15), Qt::KeepAspectRatio);
    painter->drawPixmap(rec, pixmap);
  } else if (name == QObject::tr("Button Matrix")) {
    QPixmap pixmap = QPixmap(":/icons/Button Matrix.png");
    painter->drawPixmap(rec, pixmap);
  } else if (name == QObject::tr("Calendar")) {
    QPixmap pixmap = QPixmap(":/icons/Calendar.png");
    painter->drawPixmap(rec, pixmap);
  } else if (name == QObject::tr("Canvas")) {
    QPixmap pixmap = QPixmap(":/icons/Canvas.png");
    painter->drawPixmap(rec, pixmap);
  } else if (name == QObject::tr("Chart")) {
    QPixmap pixmap = QPixmap(":/icons/Chart.png");
    painter->drawPixmap(rec, pixmap);
  } else if (name == QObject::tr("Check box")) {
    QPixmap pixmap = QPixmap(":/icons/Check box.png");
    painter->drawPixmap(rec, pixmap);
  } else if (name == QObject::tr("Color picker")) {
    QPixmap pixmap = QPixmap(":/icons/Color picker.png");
    painter->drawPixmap(rec, pixmap);
  } else if (name == QObject::tr("Container")) {
    QPixmap pixmap = QPixmap(":/icons/Container.png");
    painter->drawPixmap(rec, pixmap);
  } else if (name == QObject::tr("Dropdown")) {
    QPixmap pixmap = QPixmap(":/icons/Dropdown.png");
    painter->drawPixmap(rec, pixmap);
  } else if (name == QObject::tr("Gauge")) {
    QPixmap pixmap = QPixmap(":/icons/Gauge.png");
    painter->drawPixmap(rec, pixmap);
  } else if (name == QObject::tr("Image button")) {
    QPixmap pixmap = QPixmap(":/icons/Image button.png");
    painter->drawPixmap(rec, pixmap);
  } else if (name == QObject::tr("Image")) {
    QPixmap pixmap = QPixmap(":/icons/Image.png");
    painter->drawPixmap(rec, pixmap);
  } else if (name == QObject::tr("Keyboard")) {
    QPixmap pixmap = QPixmap(":/icons/Keyboard.png");
    painter->drawPixmap(rec, pixmap);
  } else if (name == QObject::tr("Label")) {
    QPixmap pixmap = QPixmap(":/icons/Label.png");
    painter->drawPixmap(rec, pixmap);
  } else if (name == QObject::tr("LED")) {
    QPixmap pixmap = QPixmap(":/icons/LED.png");
    painter->drawPixmap(rec, pixmap);
  } else if (name == QObject::tr("Line meter")) {
    QPixmap pixmap = QPixmap(":/icons/Line meter.png");
    painter->drawPixmap(rec, pixmap);
  } else if (name == QObject::tr("Line")) {
    QPixmap pixmap = QPixmap(":/icons/Line.png");
    painter->drawPixmap(rec, pixmap);
  } else if (name == QObject::tr("List")) {
    QPixmap pixmap = QPixmap(":/icons/List.png");
    painter->drawPixmap(rec, pixmap);
  } else if (name == QObject::tr("Message box")) {
    QPixmap pixmap = QPixmap(":/icons/Message box.png");
    painter->drawPixmap(rec, pixmap);
  } else if (name == QObject::tr("ObjectMask")) {
    QPixmap pixmap = QPixmap(":/icons/ObjectMask.png");
    painter->drawPixmap(rec, pixmap);
  } else if (name == QObject::tr("Page")) {
    QPixmap pixmap = QPixmap(":/icons/Page.png");
    painter->drawPixmap(rec, pixmap);
  } else if (name == QObject::tr("Roller")) {
    QPixmap pixmap = QPixmap(":/icons/Roller.png");
    painter->drawPixmap(rec, pixmap);
  } else if (name == QObject::tr("Slider")) {
    QPixmap pixmap = QPixmap(":/icons/Slider.png");
    painter->drawPixmap(rec, pixmap);
  } else if (name == QObject::tr("Spinbox")) {
    QPixmap pixmap = QPixmap(":/icons/Spinbox.png");
    painter->drawPixmap(rec, pixmap);
  } else if (name == QObject::tr("Spinner")) {
    QPixmap pixmap = QPixmap(":/icons/Spinner.png");
    painter->drawPixmap(rec, pixmap);
  } else if (name == QObject::tr("Switch")) {
    QPixmap pixmap = QPixmap(":/icons/Switch.png");
    painter->drawPixmap(rec, pixmap);
  } else if (name == QObject::tr("Table")) {
    QPixmap pixmap = QPixmap(":/icons/Table.png");
    painter->drawPixmap(rec, pixmap);
  } else if (name == QObject::tr("Tabview")) {
    QPixmap pixmap = QPixmap(":/icons/Tabview.png");
    painter->drawPixmap(rec, pixmap);
  } else if (name == QObject::tr("Text area")) {
    QPixmap pixmap = QPixmap(":/icons/Text area.png");
    painter->drawPixmap(rec, pixmap);
  } else if (name == QObject::tr("TileView")) {
    QPixmap pixmap = QPixmap(":/icons/TileView.png");
    painter->drawPixmap(rec, pixmap);
  } else if (name == QObject::tr("Window")) {
    QPixmap pixmap = QPixmap(":/icons/Window.png");
    painter->drawPixmap(rec, pixmap);
  }

  painter->drawText(rec2, name);
}
