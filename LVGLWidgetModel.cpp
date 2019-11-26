#include "LVGLWidgetModel.h"

#include <QMimeData>
#include <QDataStream>

#include "widgets/LVGLWidget.h"
#include "LVGL.h"

LVGLWidgetModel::LVGLWidgetModel(QObject *parent)
	: QAbstractListModel(parent)
{
}

int LVGLWidgetModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent)
	return lvgl.widgets().size();
}

QVariant LVGLWidgetModel::data(const QModelIndex &index, int role) const
{
	if (role == Qt::DisplayRole)
		return lvgl.widgets().at(index.row())->name();
	return QVariant();
}

Qt::ItemFlags LVGLWidgetModel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags defaultFlags = QAbstractListModel::flags(index);
	if (index.isValid())
		return Qt::ItemIsDragEnabled | defaultFlags;
	else
		return defaultFlags;
}

QStringList LVGLWidgetModel::mimeTypes() const
{
	return QStringList()  << "application/x-widget";
}

QMimeData *LVGLWidgetModel::mimeData(const QModelIndexList &indexes) const
{
	QMimeData * mimeData = new QMimeData();
	QByteArray encodedData;

	union {
		const LVGLWidget *ptr;
		qintptr i;
	} cast;

	QDataStream stream(&encodedData, QIODevice::WriteOnly);
	for (const QModelIndex &index:indexes) {
		if (index.isValid() && index.column() == 0) {
			cast.ptr = lvgl.widgets().at(index.row());
			stream << cast.i;
		}
	}
	mimeData->setData("application/x-widget", encodedData);
	return mimeData;
}
