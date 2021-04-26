#include "LVGLPropertyModel.h"

#include <QComboBox>
#include <QDebug>
#include <QLineEdit>
#include <QSpinBox>

#include "LVGLCommands.h"
#include "LVGLCore.h"
#include "LVGLHelper.h"
#include "LVGLSimulator.h"
#include "widgets/LVGLWidget.h"

LVGLPropertyModel::LVGLPropertyModel(QObject *parent)
    : QAbstractItemModel(parent), m_obj(nullptr) {}

QVariant LVGLPropertyModel::headerData(int section, Qt::Orientation orientation,
                                       int role) const {
  if ((orientation != Qt::Horizontal) || (role != Qt::DisplayRole))
    return QVariant();
  if (section == 0)
    return "Property";
  else if (section == 1)
    return "Value";
  return QVariant();
}

QModelIndex LVGLPropertyModel::index(int row, int column,
                                     const QModelIndex &parent) const {
  if (!hasIndex(row, column, parent) || (m_obj == nullptr))
    return QModelIndex();

  if (parent.isValid()) {
    const LVGLProperty *p =
        static_cast<const LVGLProperty *>(parent.internalPointer());
    const LVGLProperty *c = p->child(row);
    return createIndex(row, column, const_cast<LVGLProperty *>(c));
  }

  auto props = m_obj->widgetClass()->properties();
  if (row < props.size())
    return createIndex(row, column, const_cast<LVGLProperty *>(props.at(row)));

  return QModelIndex();
}

QModelIndex LVGLPropertyModel::parent(const QModelIndex &index) const {
  if (!index.isValid() || (m_obj == nullptr)) return QModelIndex();

  const LVGLProperty *item =
      static_cast<const LVGLProperty *>(index.internalPointer());
  const LVGLProperty *p = item->parent();
  if (p == nullptr) return QModelIndex();
  return createIndex(p->row(), 0, const_cast<LVGLProperty *>(p));
}

int LVGLPropertyModel::rowCount(const QModelIndex &parent) const {
  if (m_obj == nullptr) return 0;

  if (parent.isValid()) {
    const LVGLProperty *p =
        static_cast<const LVGLProperty *>(parent.internalPointer());
    return p->count();
  }

  return m_obj->widgetClass()->properties().size();
}

int LVGLPropertyModel::columnCount(const QModelIndex &parent) const {
  Q_UNUSED(parent)
  return 2;
}

QVariant LVGLPropertyModel::data(const QModelIndex &index, int role) const {
  if ((m_obj == nullptr) || !index.isValid()) return QVariant();
  auto prop = reinterpret_cast<LVGLProperty *>(index.internalPointer());
  if (role == Qt::DisplayRole) {
    if (index.column() == 0) {
      return prop->name();
    } else if (index.column() == 1) {
      QVariant val = prop->value(m_obj);
      if (val.type() != QVariant::List) return val;

      QStringList ret;
      for (const QVariant &v : val.toList()) ret << v.toString();
      return "[" + ret.join(",") + "]";
    }
  } else if (role == (Qt::UserRole + 1)) {
    LVGLObjectCast cast;
    cast.ptr = m_obj;
    return cast.i;
  }
  return QVariant();
}

LVGLObject *LVGLPropertyModel::object() const { return m_obj; }

Qt::ItemFlags LVGLPropertyModel::flags(const QModelIndex &index) const {
  Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);
  auto prop = reinterpret_cast<LVGLProperty *>(index.internalPointer());
  if (index.isValid() && (index.column() == 1) && prop && prop->hasEditor())
    return Qt::ItemIsEditable | defaultFlags;
  else
    return defaultFlags;
}

QModelIndex LVGLPropertyModel::propIndex(const LVGLProperty *prop,
                                         const LVGLWidget *widget, int column) {
  const LVGLProperty *p = prop->parent();
  if (p == nullptr) {
    int row = widget->properties().indexOf(const_cast<LVGLProperty *>(prop));
    return createIndex(row, column, const_cast<LVGLProperty *>(prop));
  }
  int row = p->indexOf(prop);
  return createIndex(row, column, const_cast<LVGLProperty *>(prop));
}

void LVGLPropertyModel::setObject(LVGLObject *obj) {
  const int count1 = obj ? obj->widgetClass()->properties().size() : 0;
  const int count2 = m_obj ? m_obj->widgetClass()->properties().size() : 0;

  if (count1 > count2) {
    beginInsertRows(QModelIndex(), count2, count1 - 1);
    m_obj = obj;
    endInsertRows();
  } else if (count1 < count2) {
    beginRemoveRows(QModelIndex(), count1, count2 - 1);
    m_obj = obj;
    endRemoveRows();
  } else {
    m_obj = obj;
  }
}

LVGLPropertyDelegate::LVGLPropertyDelegate(QObject *parent)
    : QStyledItemDelegate(parent) {}

QWidget *LVGLPropertyDelegate::createEditor(QWidget *parent,
                                            const QStyleOptionViewItem &option,
                                            const QModelIndex &index) const {
  Q_UNUSED(option)
  if (!index.isValid() || (index.column() != 1)) return nullptr;

  auto prop = reinterpret_cast<LVGLProperty *>(index.internalPointer());
  if (prop) return prop->editor(parent);
  return nullptr;
}

void LVGLPropertyDelegate::setEditorData(QWidget *editor,
                                         const QModelIndex &index) const {
  (void)editor;

  if (!index.isValid() || (index.column() != 1)) return;

  LVGLObjectCast cast;
  auto prop = reinterpret_cast<LVGLProperty *>(index.internalPointer());
  cast.i = index.data(Qt::UserRole + 1).toLongLong();
  if ((cast.ptr == nullptr) || (prop == nullptr)) return;
  prop->updateEditor(cast.ptr);
}

void LVGLPropertyDelegate::setModelData(QWidget *editor,
                                        QAbstractItemModel *model,
                                        const QModelIndex &index) const {
  (void)editor;

  if (!index.isValid() || (index.column() != 1)) return;

  LVGLObjectCast cast;
  auto prop = reinterpret_cast<LVGLProperty *>(index.internalPointer());
  cast.i = index.data(Qt::UserRole + 1).toLongLong();
  if ((cast.ptr == nullptr) || (prop == nullptr)) return;
  LVGLObject *obj = cast.ptr;
  auto oldprop = obj->propToJson();
  prop->updateWidget(cast.ptr);
  auto str = prop->name();
  if (str != "Geometry" && str != "x" && str != "y" && str != "Width" &&
      str != "Height") {
    auto sim = LVGLHelper::getInstance().getcursim();
    auto newprop = obj->propToJson();
    sim->undoStack()->push(
        new SetWidgetPropCommand(sim, obj, oldprop, newprop, prop->name()));
  }
  emit model->dataChanged(index, index);
};

#if 0
void LVGLPropertyDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	if (!index.isValid() || (index.column() != 1))
		return;

	LVGLObjectCast cast;
	cast.i = index.data(Qt::UserRole + 1).toLongLong();
	if (cast.ptr == nullptr)
		return;

	auto prop = reinterpret_cast<LVGLProperty*>(index.internalPointer());
	if (prop->name == "Source") {
		QComboBox *widget = qobject_cast<QComboBox*>(editor);
		widget->setCurrentText(prop->get(cast.ptr).toString());
	} else if (prop->type == QVariant::String) {
		QLineEdit *widget = qobject_cast<QLineEdit*>(editor);
		widget->setText(prop->get(cast.ptr).toString());
	} else if (prop->type == QVariant::Int) {
		QSpinBox *widget = qobject_cast<QSpinBox*>(editor);
		widget->setValue(prop->get(cast.ptr).toInt());
	} else if (prop->type == QVariant::Bool) {
		QComboBox *widget = qobject_cast<QComboBox*>(editor);
		widget->setCurrentIndex(prop->get(cast.ptr).toBool() ? 0 : 1);
	} else if (prop->type == QVariant::StringList) {
		QComboBox *widget = qobject_cast<QComboBox*>(editor);
		widget->setCurrentIndex(prop->defaultVal.toStringList().indexOf(prop->get(cast.ptr).toString()));
	}
}

void LVGLPropertyDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	Q_UNUSED(model)

	if (!index.isValid() || (index.column() != 1))
		return;

	LVGLObjectCast cast;
	cast.i = index.data(Qt::UserRole + 1).toLongLong();
	if (cast.ptr == nullptr)
		return;

	auto prop = reinterpret_cast<LVGLProperty*>(index.internalPointer());
	if (prop->name == "Source") {
		QComboBox *widget = qobject_cast<QComboBox*>(editor);
		prop->set(cast.ptr, widget->currentText());
	} else if (prop->type == QVariant::String) {
		QLineEdit *widget = qobject_cast<QLineEdit*>(editor);
		prop->set(cast.ptr, widget->text());
	} else if (prop->type == QVariant::Int) {
		QSpinBox *widget = qobject_cast<QSpinBox*>(editor);
		prop->set(cast.ptr, widget->value());
	} else if (prop->type == QVariant::Bool) {
		QComboBox *widget = qobject_cast<QComboBox*>(editor);
		prop->set(cast.ptr, widget->currentIndex() == 0);
	} else if (prop->type == QVariant::StringList) {
		QComboBox *widget = qobject_cast<QComboBox*>(editor);
		prop->set(cast.ptr, prop->defaultVal.toStringList().at(widget->currentIndex()));
	}
	emit model->dataChanged(index, index);
}
#endif
