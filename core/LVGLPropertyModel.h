#ifndef LVGLPROPERTYMODEL_H
#define LVGLPROPERTYMODEL_H

#include <QAbstractItemModel>
#include <QStyledItemDelegate>

#include "LVGLObject.h"
#include "LVGLProperty.h"

class LVGLWidget;

class LVGLPropertyModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	explicit LVGLPropertyModel(QObject *parent = nullptr);

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

	// Basic functionality:
	QModelIndex index(int row, int column,
							const QModelIndex &parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex &index) const override;

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

	LVGLObject *object() const;

	Qt::ItemFlags flags(const QModelIndex &index) const override;

	QModelIndex propIndex(const LVGLProperty *prop, const LVGLWidget *widget, int column);

public slots:
	void setObject(LVGLObject *obj);

private:
	LVGLObject *m_obj;

};

class LVGLPropertyDelegate : public QStyledItemDelegate
{
public:
	LVGLPropertyDelegate(QObject* parent = nullptr);

	QWidget *createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	void setEditorData(QWidget* editor, const QModelIndex& index) const override;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

};

#endif // LVGLPROPERTYMODEL_H
