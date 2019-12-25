#ifndef LVGLOBJECTMODEL_H
#define LVGLOBJECTMODEL_H

#include <QAbstractItemModel>
#include "LVGLCore.h"
#include "LVGLObject.h"

class LVGLObjectModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	explicit LVGLObjectModel(QObject *parent = nullptr);

	// Header:
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

	// Basic functionality:
	QModelIndex index(int row, int column,
							const QModelIndex &parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex &index) const override;

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

	// Add data:
	bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
	bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

	// Remove data:
	bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
	bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

	LVGLObject *object(const QModelIndex &index) const;

public slots:
	void setCurrentObject(LVGLObject *obj);

private:
	QModelIndex objIndex(LVGLObject *obj, int col) const;

	LVGLObject *m_current;

};

#endif // LVGLOBJECTMODEL_H
