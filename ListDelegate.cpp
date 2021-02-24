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
  painter->setPen(QColor("#ffffff"));
  painter->setBrush(QColor("#ffffff"));
  painter->drawRect(seletrec);

  seletrec.adjust(2, 2, -2, -2);
  if (option.state & QStyle::State_Selected) {
    painter->setBrush(QColor("#d1e5ca"));
    painter->drawRoundRect(seletrec, 2, 2);
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

  if (name == tr("Arc")) {
    QPixmap pixmap = QPixmap(":/icons/Arc.png");
    pixmap.scaled(QSize(62, 15), Qt::KeepAspectRatio);
    painter->drawPixmap(rec, pixmap);
    painter->drawText(rec2, tr("Arc"));
  } else if (name == tr("Bar")) {
    QPixmap pixmap = QPixmap(":/icons/Bar.png");
    painter->drawPixmap(rec, pixmap);
    painter->drawText(rec2, tr("Bar"));
  } else if (name == tr("Button")) {
    QPixmap pixmap = QPixmap(":/icons/Button.png");
    pixmap.scaled(QSize(62, 15), Qt::KeepAspectRatio);
    painter->drawPixmap(rec, pixmap);
    painter->drawText(rec2, tr("Button"));
  } else if (name == tr("Button Matrix")) {
    QPixmap pixmap = QPixmap(":/icons/Button Matrix.png");
    painter->drawPixmap(rec, pixmap);
    painter->drawText(rec2, tr("Button Matrix"));
  } else if (name == tr("Calendar")) {
    QPixmap pixmap = QPixmap(":/icons/Calendar.png");
    painter->drawPixmap(rec, pixmap);
    painter->drawText(rec2, tr("Calendar"));
  } else if (name == tr("Canvas")) {
    QPixmap pixmap = QPixmap(":/icons/Canvas.png");
    painter->drawPixmap(rec, pixmap);
    painter->drawText(rec2, tr("Canvas"));
  } else if (name == tr("Chart")) {
    QPixmap pixmap = QPixmap(":/icons/Chart.png");
    painter->drawPixmap(rec, pixmap);
    painter->drawText(rec2, tr("Chart"));
  } else if (name == tr("Check box")) {
    QPixmap pixmap = QPixmap(":/icons/Check box.png");
    painter->drawPixmap(rec, pixmap);
    painter->drawText(rec2, tr("Check box"));
  } else if (name == tr("Color picker")) {
    QPixmap pixmap = QPixmap(":/icons/Color picker.png");
    painter->drawPixmap(rec, pixmap);
    painter->drawText(rec2, tr("Color picker"));
  } else if (name == tr("Container")) {
    QPixmap pixmap = QPixmap(":/icons/Container.png");
    painter->drawPixmap(rec, pixmap);
    painter->drawText(rec2, tr("Container"));
  } else if (name == tr("Dropdown")) {
    QPixmap pixmap = QPixmap(":/icons/Dropdown.png");
    painter->drawPixmap(rec, pixmap);
    painter->drawText(rec2, tr("Dropdown"));
  } else if (name == tr("Gauge")) {
    QPixmap pixmap = QPixmap(":/icons/Gauge.png");
    painter->drawPixmap(rec, pixmap);
    painter->drawText(rec2, tr("Gauge"));
  } else if (name == tr("Image button")) {
    QPixmap pixmap = QPixmap(":/icons/Image button.png");
    painter->drawPixmap(rec, pixmap);
    painter->drawText(rec2, tr("Image button"));
  } else if (name == tr("Image")) {
    QPixmap pixmap = QPixmap(":/icons/Image.png");
    painter->drawPixmap(rec, pixmap);
    painter->drawText(rec2, tr("Image"));
  } else if (name == tr("Keyboard")) {
    QPixmap pixmap = QPixmap(":/icons/Keyboard.png");
    painter->drawPixmap(rec, pixmap);
    painter->drawText(rec2, tr("Keyboard"));
  } else if (name == tr("Label")) {
    QPixmap pixmap = QPixmap(":/icons/Label.png");
    painter->drawPixmap(rec, pixmap);
    painter->drawText(rec2, tr("Label"));
  } else if (name == tr("LED")) {
    QPixmap pixmap = QPixmap(":/icons/LED.png");
    painter->drawPixmap(rec, pixmap);
    painter->drawText(rec2, tr("LED"));
  } else if (name == tr("Line meter")) {
    QPixmap pixmap = QPixmap(":/icons/Line meter.png");
    painter->drawPixmap(rec, pixmap);
    painter->drawText(rec2, tr("Line meter"));
  } else if (name == tr("Line")) {
    QPixmap pixmap = QPixmap(":/icons/Line.png");
    painter->drawPixmap(rec, pixmap);
    painter->drawText(rec2, tr("Line"));
  } else if (name == tr("List")) {
    QPixmap pixmap = QPixmap(":/icons/List.png");
    painter->drawPixmap(rec, pixmap);
    painter->drawText(rec2, tr("List"));
  } else if (name == tr("Message box")) {
    QPixmap pixmap = QPixmap(":/icons/Message box.png");
    painter->drawPixmap(rec, pixmap);
    painter->drawText(rec2, tr("Message box"));
  } else if (name == tr("ObjectMask")) {
    QPixmap pixmap = QPixmap(":/icons/ObjectMask.png");
    painter->drawPixmap(rec, pixmap);
    painter->drawText(rec2, tr("ObjectMask"));
  } else if (name == tr("Page")) {
    QPixmap pixmap = QPixmap(":/icons/Page.png");
    painter->drawPixmap(rec, pixmap);
    painter->drawText(rec2, tr("Page"));
  } else if (name == tr("Roller")) {
    QPixmap pixmap = QPixmap(":/icons/Roller.png");
    painter->drawPixmap(rec, pixmap);
    painter->drawText(rec2, tr("Roller"));
  } else if (name == tr("Slider")) {
    QPixmap pixmap = QPixmap(":/icons/Slider.png");
    painter->drawPixmap(rec, pixmap);
    painter->drawText(rec2, tr("Slider"));
  } else if (name == tr("Spinbox")) {
    QPixmap pixmap = QPixmap(":/icons/Spinbox.png");
    painter->drawPixmap(rec, pixmap);
    painter->drawText(rec2, tr("Spinbox"));
  } else if (name == tr("Spinner")) {
    QPixmap pixmap = QPixmap(":/icons/Spinner.png");
    painter->drawPixmap(rec, pixmap);
    painter->drawText(rec2, tr("Spinner"));
  } else if (name == tr("Switch")) {
    QPixmap pixmap = QPixmap(":/icons/Switch.png");
    painter->drawPixmap(rec, pixmap);
    painter->drawText(rec2, tr("Switch"));
  } else if (name == tr("Table")) {
    QPixmap pixmap = QPixmap(":/icons/Table.png");
    painter->drawPixmap(rec, pixmap);
    painter->drawText(rec2, tr("Table"));
  } else if (name == tr("Tabview")) {
    QPixmap pixmap = QPixmap(":/icons/Tabview.png");
    painter->drawPixmap(rec, pixmap);
    painter->drawText(rec2, tr("Tabview"));
  } else if (name == tr("Text area")) {
    QPixmap pixmap = QPixmap(":/icons/Text area.png");
    painter->drawPixmap(rec, pixmap);
    painter->drawText(rec2, tr("Text area"));
  } else if (name == tr("TileView")) {
    QPixmap pixmap = QPixmap(":/icons/TileView.png");
    painter->drawPixmap(rec, pixmap);
    painter->drawText(rec2, tr("TileView"));
  } else if (name == tr("Window")) {
    QPixmap pixmap = QPixmap(":/icons/Window.png");
    painter->drawPixmap(rec, pixmap);
    painter->drawText(rec2, tr("Window"));
  }
}
