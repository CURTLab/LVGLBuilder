#include "PageView.h"

PageView::PageView(QWidget *parent) : QTreeView{parent} {}

void PageView::setEdit(const QModelIndex &index) {
  edit(index, QTreeView::DoubleClicked, nullptr);
}
