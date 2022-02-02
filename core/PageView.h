#ifndef PAGEVIEW_H
#define PAGEVIEW_H

#include <QTreeView>

class PageView : public QTreeView {
  Q_OBJECT
 public:
  PageView(QWidget *parent = nullptr);

 public:
  void setEdit(const QModelIndex &index);
};

#endif  // PAGEVIEW_H
