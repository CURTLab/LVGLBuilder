#include "PageModel.h"

#include <QEvent>

#include "LVGLHelper.h"
#include "MainWindow.h"
#include "Page.h"

PageModel::PageModel(Page *root, QObject *parent)
    : QAbstractItemModel{parent}, m_root{root} {}

PageModel::~PageModel() { delete m_root; }

QVariant PageModel::headerData(int section, Qt::Orientation orientation,
                               int role) const {
  if ((orientation != Qt::Horizontal) || (role != Qt::DisplayRole))
    return QVariant();
  if (section == 0) return m_root->name();
  return QVariant();
}

QModelIndex PageModel::index(int row, int column,
                             const QModelIndex &parent) const {
  if (!hasIndex(row, column, parent)) return QModelIndex();

  auto *p = m_root;
  if (parent.isValid()) p = static_cast<Page *>(parent.internalPointer());

  auto pChild = p->child(row);
  if (pChild) return createIndex(row, column, pChild);
  return QModelIndex();
}

QModelIndex PageModel::parent(const QModelIndex &child) const {
  if (!child.isValid()) return QModelIndex();
  auto c = static_cast<Page *>(child.internalPointer());
  auto p = c->parent();
  if (p == m_root) return QModelIndex();
  return createIndex(p->row(), 0, p);
}

int PageModel::rowCount(const QModelIndex &parent) const {
  auto *p = m_root;
  if (parent.isValid()) p = static_cast<Page *>(parent.internalPointer());
  return p->childSize();
}

int PageModel::columnCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return 1;
}

QVariant PageModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid()) return QVariant();

  const Page *o = reinterpret_cast<Page *>(index.internalPointer());

  if (Qt::DisplayRole == role)
    return o->name();
  else if (Qt::DecorationRole == role)
    return QIcon(o->Icon());
  else if ((Qt::UserRole + 1) == role)
    return o->Icon();
  return QVariant();
}

bool PageModel::setData(const QModelIndex &index, const QVariant &value,
                        int role) {
  if (role != Qt::DisplayRole && role != Qt::EditRole) {
    return false;
  }

  auto *p = m_root;
  if (index.isValid()) {
    p = static_cast<Page *>(index.internalPointer());
  }

  p->setName(value.toString());
  emit dataChanged(index, index);
  return true;
}

Qt::ItemFlags PageModel::flags(const QModelIndex &index) const {
  if (!index.isValid()) {
    return Qt::ItemIsEnabled;
  }
  return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable |
         Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
}

bool PageModel::insertRows(int row, int count, const QModelIndex &parent) {
  beginInsertRows(parent, row, row + count - 1);
  auto *p = m_root;
  if (parent.isValid()) {
    p = static_cast<Page *>(parent.internalPointer());
  }

  p->insertChild(row, new Page(QString(), p));
  endInsertRows();
  return true;
}

bool PageModel::removeRows(int row, int count, const QModelIndex &parent) {
  beginRemoveRows(parent, row, row + count - 1);
  auto *p = m_root;
  if (parent.isValid()) p = static_cast<Page *>(parent.internalPointer());

  p->removeChild(p->child(row));
  endRemoveRows();
  return true;
}

QModelIndex PageModel::addTree(QString &str, const QModelIndex &parent) {
  auto *p = m_root;
  if (parent.isValid()) p = static_cast<Page *>(parent.internalPointer());
  if (p->parent() && p->parent()->parent() == m_root) return QModelIndex();

  int row = p->childSize();

  insertRows(row, 1, parent);

  QModelIndex index = this->index(row, 0, parent);
  setData(index, str, Qt::DisplayRole);
  return index;
}

void PageModel::removeTree(QModelIndex &index) {
  if (index.isValid()) {
    removeRows(index.row(), 1, index.parent());
  }
}

Page *PageModel::getRoot() { return m_root; }

PageDeletegate::PageDeletegate(QObject *parent) : QStyledItemDelegate{parent} {}

QWidget *PageDeletegate::createEditor(QWidget *parent,
                                      const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const {
  Q_UNUSED(option);
  if (!index.isValid() || (index.column() != 0)) return nullptr;
  auto task = reinterpret_cast<Page *>(index.internalPointer());
  if (task) return task->editor(parent);
  return nullptr;
}

void PageDeletegate::setEditorData(QWidget *editor,
                                   const QModelIndex &index) const {
  if (!index.isValid() || (index.column() != 0)) return;

  PageWidget *YDEditor = qobject_cast<PageWidget *>(editor);
  auto str = index.data(Qt::DisplayRole).toString();
  YDEditor->setName(str);
}

void PageDeletegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                  const QModelIndex &index) const {
  if (index.data(Qt::DisplayRole).canConvert<QString>()) {
    PageWidget *YDEditor = qobject_cast<PageWidget *>(editor);
    model->setData(index, YDEditor->getName(), Qt::DisplayRole);
  } else {
    QStyledItemDelegate::setModelData(editor, model, index);
  }
}

bool PageDeletegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                                 const QStyleOptionViewItem &option,
                                 const QModelIndex &index) {
  if (event->type() == QEvent::MouseButtonDblClick)  //禁止双击编辑
  {
    return true;
  }
  return QStyledItemDelegate::editorEvent(event, model, option, index);
}

QSize PageDeletegate::sizeHint(const QStyleOptionViewItem &option,
                               const QModelIndex &index) const {
  auto size = QStyledItemDelegate::sizeHint(option, index);
  return size;
}

void PageDeletegate::commitAndCloseEditor() {
  PageWidget *editor = qobject_cast<PageWidget *>(sender());
  emit commitData(editor);
  emit closeEditor(editor);
}
