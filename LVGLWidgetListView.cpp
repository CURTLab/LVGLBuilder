#include "LVGLWidgetListView.h"

#include "LVGLWidgetModel.h"

#include <QMimeData>
#include <QDrag>

#include "LVGLCore.h"

LVGLWidgetListView::LVGLWidgetListView(QWidget *parent) : QListView(parent)
{
	setDragEnabled(true);
	setDragDropMode(QAbstractItemView::DragOnly);
}

void LVGLWidgetListView::startDrag(Qt::DropActions supportedActions)
{
	const QModelIndexList indexes = selectedIndexes();
	QMimeData *mimeData = model()->mimeData(indexes);

	if (!mimeData->hasFormat("application/x-widget"))
		return QListView::startDrag(supportedActions);

	LVGLWidgetCast cast;

	QByteArray encoded = mimeData->data("application/x-widget");
    QDataStream stream(&encoded, QIODeviceBase::ReadOnly);
	stream >> cast.i;
	LVGLWidget *widgetClass = cast.ptr;
	QPixmap preview = widgetClass->preview();
	if (!preview.isNull()) {
		QDrag *drag = new QDrag(this);
		drag->setMimeData(mimeData);
		drag->setPixmap(preview);
		drag->setHotSpot({0, 0});
		drag->exec(supportedActions);
	} else {
		QListView::startDrag(supportedActions);
	}
}
