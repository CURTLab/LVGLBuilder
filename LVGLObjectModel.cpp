#include "LVGLObjectModel.h"

#include <QFont>
#include <QDebug>

LVGLObjectModel::LVGLObjectModel(QObject *parent)
	: QAbstractItemModel(parent)
	, m_current(nullptr)
{
}

QVariant LVGLObjectModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if ((orientation != Qt::Horizontal) || (role != Qt::DisplayRole))
		return QVariant();
	if (section == 0)
		return "Object";
	else if (section == 1)
		return "Class";
	return QVariant();
}

QModelIndex LVGLObjectModel::index(int row, int column, const QModelIndex &parent) const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	if (parent.isValid()) {
		const LVGLObject *p = static_cast<const LVGLObject*>(parent.internalPointer());
		return createIndex(row, column, p->childs().at(row));
	}
	return createIndex(row, column, lvgl.topLevelObjects().at(row));
}

QModelIndex LVGLObjectModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();

	LVGLObject *o = static_cast<LVGLObject*>(index.internalPointer());
	LVGLObject *p = o->parent();

	if (p == nullptr)
		return QModelIndex();

	const int row = p->childs().indexOf(o);
	return createIndex(row, 0, p);
}

int LVGLObjectModel::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid()) {
		const LVGLObject *p = static_cast<const LVGLObject*>(parent.internalPointer());
		const int size = p->childs().size();
		//qDebug() << size;
		return size;
	}
	auto top = lvgl.topLevelObjects();
	const int size = top.size();
	//qDebug() << size;
	return size;
}

int LVGLObjectModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent)
	return 2;
}

QVariant LVGLObjectModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	const LVGLObject *o = reinterpret_cast<const LVGLObject*>(index.internalPointer());

	if (role == Qt::DisplayRole) {
		if (index.column() == 0) {
			return o->name();
		} else if (index.column() == 1) {
			return o->widgetClass()->name();
		}
	} else if (role == Qt::FontRole) {
		QFont font;
		font.setBold(m_current == o);
		return font;
	}
	return QVariant();
}

void LVGLObjectModel::beginInsertObject(LVGLObject *obj)
{
	int row = 0;
	LVGLObject *p = obj->parent();
	QModelIndex parent;
	if (p) {
		row = p->childs().indexOf(obj);
		parent = objIndex(p, 0);
	}
	beginInsertRows(parent, row, row);
}

void LVGLObjectModel::endInsertObject()
{
	endInsertRows();
}

void LVGLObjectModel::beginRemoveObject(LVGLObject *obj)
{
	int row = 0;
	LVGLObject *p = obj->parent();
	QModelIndex parent;
	if (p) {
		row = p->childs().indexOf(obj);
		parent = objIndex(p, 0);
	}
	beginRemoveRows(parent, row, row);
}

void LVGLObjectModel::endRemoveObject()
{
	endRemoveRows();
}

LVGLObject *LVGLObjectModel::object(const QModelIndex &index) const
{
	if (index.isValid())
		return reinterpret_cast<LVGLObject*>(index.internalPointer());
	return nullptr;
}

void LVGLObjectModel::setCurrentObject(LVGLObject *obj)
{
	LVGLObject *old = m_current;
	m_current = obj;
	emit dataChanged(objIndex(obj, 0), objIndex(obj, 1), QVector<int>({Qt::FontRole}));
	emit dataChanged(objIndex(old, 0), objIndex(old, 1), QVector<int>({Qt::FontRole}));
}

QModelIndex LVGLObjectModel::objIndex(LVGLObject *obj, int col) const
{
	if (obj == nullptr)
		return QModelIndex();

	LVGLObject *p = obj->parent();
	if (p) return createIndex(p->childs().indexOf(obj), col, obj);
	return createIndex(lvgl.topLevelObjects().indexOf(obj), col, obj);
}
