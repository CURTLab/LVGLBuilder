#ifndef LISTVIEWITEM_H
#define LISTVIEWITEM_H

#include <QWidget>

class LVGLWidgetListView;

QT_BEGIN_NAMESPACE
namespace Ui {
class ListViewItem;
}
QT_END_NAMESPACE

class ListViewItem : public QWidget {
  Q_OBJECT

 public:
  ListViewItem(QWidget *parent = nullptr);
  ~ListViewItem();

  LVGLWidgetListView *getlistview();

  void settoolbtnText(const QString &str);

 public slots:
  void slot_toolbtnclicked();

 private:
  Ui::ListViewItem *ui;
  bool m_clicked;
};
#endif  // LISTVIEWITEM_H
