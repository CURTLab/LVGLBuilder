#ifndef LISTVIEWITEM_H
#define LISTVIEWITEM_H

#include <QWidget>

class LVGLWidgetListView;

QT_BEGIN_NAMESPACE
namespace Ui {
class LVGLListViewItem;
}
QT_END_NAMESPACE

class LVGLListViewItem : public QWidget {
  Q_OBJECT

 public:
  LVGLListViewItem(QWidget *parent = nullptr);
  ~LVGLListViewItem();

  LVGLWidgetListView *getlistview();

  void settoolbtnText(const QString &str);

 public slots:
  void slot_toolbtnclicked();
  void slot_toshowtab();

 private:
  Ui::LVGLListViewItem *ui;
  bool m_clicked;
  QString m_toolbtnQSS1;
  QString m_toolbtnQSS2;
};
#endif  // LISTVIEWITEM_H
