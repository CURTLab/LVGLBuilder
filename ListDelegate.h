#ifndef LISTDELEGATE_H
#define LISTDELEGATE_H
#include <QModelIndex>
#include <QStyledItemDelegate>

class ListDelegate : public QStyledItemDelegate {
  Q_OBJECT
 public:
  explicit ListDelegate(QObject *parent = 0);
  ~ListDelegate();
  void paint(QPainter *painter, const QStyleOptionViewItem &option,
             const QModelIndex &index) const override;
};

#endif  // LISTDELEGATE_H
