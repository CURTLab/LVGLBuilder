#include "LVGLStyleModel.h"

#include <QColorDialog>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QListWidget>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QtDebug>

#include "LVGLCore.h"
#include "LVGLObject.h"

LVGLStyleModel::LVGLStyleModel(QObject *parent)
    : QAbstractItemModel(parent), m_styleBase(new LVGLStyle) {}

QVariant LVGLStyleModel::headerData(int section, Qt::Orientation orientation,
                                    int role) const {
  if ((orientation != Qt::Horizontal) || (role != Qt::DisplayRole))
    return QVariant();
  if (section == 0)
    return "Property";
  else if (section == 1)
    return "Value";
  return QVariant();
}

QModelIndex LVGLStyleModel::index(int row, int column,
                                  const QModelIndex &parent) const {
  if (!hasIndex(row, column, parent)) return QModelIndex();

  if (parent.isValid()) {
    const LVGLStyleItem *p =
        static_cast<const LVGLStyleItem *>(parent.internalPointer());
    const LVGLStyleItem *c = p->child(row);
    return createIndex(row, column, const_cast<LVGLStyleItem *>(c));
  }

  return createIndex(row, column,
                     const_cast<LVGLStyleItem *>(m_styleBase->child(row)));
}

QModelIndex LVGLStyleModel::parent(const QModelIndex &index) const {
  if (!index.isValid()) return QModelIndex();

  const LVGLStyleItem *item =
      static_cast<const LVGLStyleItem *>(index.internalPointer());
  const LVGLStyleItem *p = item->parent();
  if (p == m_styleBase) return QModelIndex();
  return createIndex(p->row(), 0, const_cast<LVGLStyleItem *>(p));
}

int LVGLStyleModel::rowCount(const QModelIndex &parent) const {
  if (parent.isValid()) {
    const LVGLStyleItem *p =
        static_cast<const LVGLStyleItem *>(parent.internalPointer());
    return p->count();
  }
  return m_styleBase->count();
}

int LVGLStyleModel::columnCount(const QModelIndex &parent) const {
  Q_UNUSED(parent)
  return 2;
}

QVariant LVGLStyleModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid()) return QVariant();

  LVGLStyleItem *item =
      reinterpret_cast<LVGLStyleItem *>(index.internalPointer());
  if (role == Qt::DisplayRole) {
    if (index.column() == 0) {
      return item->name();
    } else if (index.column() == 1) {
      auto x = m_styleBase->get(item);
      m_styleBase->updateDate();
      return x;
    }
  }

  // FIXME: Implement me!
  return QVariant();
}

Qt::ItemFlags LVGLStyleModel::flags(const QModelIndex &index) const {
  if (!index.isValid()) return Qt::NoItemFlags;

  LVGLStyleItem *item =
      reinterpret_cast<LVGLStyleItem *>(index.internalPointer());
  auto flags = QAbstractItemModel::flags(index);
  if (!(m_editableStyles & item->stylePart())) return Qt::NoItemFlags;
  if (index.column() == 1 && (item->type() == LVGLStyleItem::Coord ||
                              item->type() == LVGLStyleItem::Opacity ||
                              item->type() == LVGLStyleItem::Color ||
                              item->type() == LVGLStyleItem::Font ||
                              item->type() == LVGLStyleItem::BorderPart ||
                              item->type() == LVGLStyleItem::BoolType ||
                              item->type() == LVGLStyleItem::Blendmode ||
                              item->type() == LVGLStyleItem::Graddir ||
                              item->type() == LVGLStyleItem::Align ||
                              item->type() == LVGLStyleItem::PointChar ||
                              item->type() == LVGLStyleItem::PointImg ||
                              item->type() == LVGLStyleItem::TextDecor ||
                              item->type() == LVGLStyleItem::ShadowType))
    return flags | Qt::ItemIsEditable;
  return flags;
}

void LVGLStyleModel::setStyle(lv_style_t *style,
                              LVGL::StyleParts editableStyles) {
  m_styleBase->seteditableStyle(m_editableStyles);
  m_styleBase->updateItems();
  m_styleBase->setStyle(style);
  m_editableStyles = editableStyles;
  updateValue(m_styleBase);
}

void LVGLStyleModel::updateValue(const LVGLStyleItem *item) {
  QModelIndex index = indexOf(item, 1);
  emit dataChanged(index, index);
  for (int i = 0; i < item->count(); ++i) updateValue(item->child(i));
}

QModelIndex LVGLStyleModel::indexOf(const LVGLStyleItem *item, int column) {
  const LVGLStyleItem *p = item->parent();
  if (p == nullptr) {
    int row = m_styleBase->indexOf(const_cast<LVGLStyleItem *>(item));
    return createIndex(row, column, const_cast<LVGLStyleItem *>(item));
  }
  int row = p->indexOf(item);
  return createIndex(row, column, const_cast<LVGLStyleItem *>(item));
}

LVGLStyle *LVGLStyleModel::styleBase() const { return m_styleBase; }

LVGLBodyPartDialog::LVGLBodyPartDialog(QWidget *parent) : QDialog(parent) {
  m_list = new QListWidget;
  m_list->addItems(QStringList() << "Bottom"
                                 << "Top"
                                 << "Left"
                                 << "Right");

  QListWidgetItem *item = nullptr;
  for (int i = 0; i < m_list->count(); ++i) {
    item = m_list->item(i);
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
    item->setCheckState(Qt::Unchecked);
  }

  QDialogButtonBox *box =
      new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

  connect(box, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(box, &QDialogButtonBox::rejected, this, &QDialog::reject);

  QVBoxLayout *layout1 = new QVBoxLayout;
  layout1->addWidget(m_list);
  layout1->addWidget(box);

  setLayout(layout1);
}

void LVGLBodyPartDialog::setBodyParts(lv_border_side_t parts) {
  QListWidgetItem *item = nullptr;
  for (int i = 0; i < m_list->count(); ++i) {
    item = m_list->item(i);
    lv_border_side_t p = (uint8_t(1) << i);
    if (parts & p) item->setCheckState(Qt::Checked);
  }
}

lv_border_side_t LVGLBodyPartDialog::bodyParts() const {
  QListWidgetItem *item = nullptr;
  lv_border_side_t ret = 0;
  for (int i = 0; i < m_list->count(); ++i) {
    item = m_list->item(i);
    if (item->checkState() == Qt::Checked) ret |= (uint8_t(1) << i);
  }
  return ret;
}

LVGLStyleDelegate::LVGLStyleDelegate(LVGLStyle *styleBase, QObject *parent)
    : QStyledItemDelegate(parent), m_styleBase(styleBase) {}

QWidget *LVGLStyleDelegate::createEditor(QWidget *parent,
                                         const QStyleOptionViewItem &option,
                                         const QModelIndex &index) const {
  Q_UNUSED(option)

  if (!index.isValid()) return nullptr;

  LVGLStyleItem *item =
      reinterpret_cast<LVGLStyleItem *>(index.internalPointer());
  if (item->type() == LVGLStyleItem::Coord) {
    QSpinBox *editor = new QSpinBox(parent);
    editor->setRange(INT16_MIN, INT16_MAX);
    return editor;
  } else if (item->type() == LVGLStyleItem::Opacity) {
    QSpinBox *editor = new QSpinBox(parent);
    editor->setRange(0, 255);
    return editor;
  } else if (item->type() == LVGLStyleItem::Color) {
    QColorDialog *dialog = new QColorDialog(parent);
    return dialog;
  } else if (item->type() == LVGLStyleItem::Font) {
    QComboBox *editor = new QComboBox(parent);
    editor->addItems(lvgl->fontNames());
    return editor;
  } else if (item->type() == LVGLStyleItem::BorderPart) {
    LVGLBodyPartDialog *dialog = new LVGLBodyPartDialog(parent);
    return dialog;
  } else if (item->type() == LVGLStyleItem::BoolType) {
    QComboBox *editor = new QComboBox(parent);
    editor->addItem("false");
    editor->addItem("true");
    return editor;
  } else if (item->type() == LVGLStyleItem::Blendmode) {
    QComboBox *editor = new QComboBox(parent);
    editor->addItem("Normal");
    editor->addItem("Additive");
    editor->addItem("SubtrActive");
    return editor;
    return editor;
  } else if (item->type() == LVGLStyleItem::Graddir) {
    QComboBox *editor = new QComboBox(parent);
    editor->addItem("None");
    editor->addItem("Vertical");
    editor->addItem("Horizontal");
    return editor;
    return editor;
  } else if (item->type() == LVGLStyleItem::PointChar) {
    QLineEdit *editor = new QLineEdit(parent);
    return editor;
  }

  return nullptr;
}

void LVGLStyleDelegate::setEditorData(QWidget *editor,
                                      const QModelIndex &index) const {
  if (!index.isValid()) return;

  LVGLStyleItem *item =
      reinterpret_cast<LVGLStyleItem *>(index.internalPointer());
  if ((item->type() == LVGLStyleItem::Coord) ||
      (item->type() == LVGLStyleItem::Opacity)) {
    QSpinBox *widget = qobject_cast<QSpinBox *>(editor);
    widget->setValue(m_styleBase->get(item).toInt());
  } else if (item->type() == LVGLStyleItem::Color) {
    QColorDialog *dialog = qobject_cast<QColorDialog *>(editor);
    QColor c = m_styleBase->get(item).value<QColor>();
    dialog->setCurrentColor(c);
  } else if (item->type() == LVGLStyleItem::Font) {
    QComboBox *widget = qobject_cast<QComboBox *>(editor);
    QString f = m_styleBase->get(item).toString();
    widget->setCurrentIndex(lvgl->fontNames().indexOf(f));
  } else if (item->type() == LVGLStyleItem::BorderPart) {
    LVGLBodyPartDialog *dialog = qobject_cast<LVGLBodyPartDialog *>(editor);
    dialog->setBodyParts(m_styleBase->getBorderPart(item));
  } else if (item->type() == LVGLStyleItem::BoolType) {
    QComboBox *widget = qobject_cast<QComboBox *>(editor);
    widget->setCurrentIndex(m_styleBase->get(item).toInt());
  } else if (item->type() == LVGLStyleItem::Blendmode) {
    QComboBox *widget = qobject_cast<QComboBox *>(editor);
    int index = widget->findText(m_styleBase->get(item).toString());
    widget->setCurrentIndex(index);
  } else if (item->type() == LVGLStyleItem::Graddir) {
    QComboBox *widget = qobject_cast<QComboBox *>(editor);
    int index = widget->findText(m_styleBase->get(item).toString());
    widget->setCurrentIndex(index);
  } else if (item->type() == LVGLStyleItem::PointChar) {
    QLineEdit *widget = qobject_cast<QLineEdit *>(editor);
    auto str = m_styleBase->get(item).toString();
    widget->setText(str);
  }
}

void LVGLStyleDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                     const QModelIndex &index) const {
  if (!index.isValid()) return;

  LVGLStyleModel *m = qobject_cast<LVGLStyleModel *>(model);
  LVGLStyleItem *item =
      reinterpret_cast<LVGLStyleItem *>(index.internalPointer());
  if ((item->type() == LVGLStyleItem::Coord) ||
      (item->type() == LVGLStyleItem::Opacity)) {
    QSpinBox *widget = qobject_cast<QSpinBox *>(editor);
    m_styleBase->set(item, widget->value());
    emit m->styleChanged();
    emit m->dataChanged(index, index);
  } else if (item->type() == LVGLStyleItem::Color) {
    QColorDialog *dialog = qobject_cast<QColorDialog *>(editor);
    if (dialog->result() == QDialog::Accepted) {
      QColor c = dialog->selectedColor();
      m_styleBase->set(item, c);
      emit m->styleChanged();
      emit m->dataChanged(index, index);
    }
  } else if (item->type() == LVGLStyleItem::Font) {
    QComboBox *widget = qobject_cast<QComboBox *>(editor);
    m_styleBase->set(item, widget->currentText());
    emit m->styleChanged();
    emit m->dataChanged(index, index);
  } else if (item->type() == LVGLStyleItem::BorderPart) {
    LVGLBodyPartDialog *dialog = qobject_cast<LVGLBodyPartDialog *>(editor);
    if (dialog->result() == QDialog::Accepted) {
      m_styleBase->set(item, dialog->bodyParts());
      emit m->styleChanged();
      emit m->dataChanged(index, index);
    }
  } else if (item->type() == LVGLStyleItem::BoolType) {
    QComboBox *widget = qobject_cast<QComboBox *>(editor);
    m_styleBase->set(item, widget->currentIndex());
    emit m->styleChanged();
    emit m->dataChanged(index, index);
  } else if (item->type() == LVGLStyleItem::Blendmode) {
    QComboBox *widget = qobject_cast<QComboBox *>(editor);
    m_styleBase->set(item, widget->currentIndex());
    emit m->styleChanged();
    emit m->dataChanged(index, index);
  } else if (item->type() == LVGLStyleItem::Graddir) {
    QComboBox *widget = qobject_cast<QComboBox *>(editor);
    m_styleBase->set(item, widget->currentIndex());
    emit m->styleChanged();
    emit m->dataChanged(index, index);
  } else if (item->type() == LVGLStyleItem::PointChar) {
    QLineEdit *widget = qobject_cast<QLineEdit *>(editor);
    m_styleBase->set(item, widget->text());
    emit m->styleChanged();
    emit m->dataChanged(index, index);
  }
}
