#include "LVGLWidgetModelInput.h"

#include <QDataStream>
#include <QMimeData>

#include "LVGLCore.h"
#include "widgets/LVGLWidget.h"

LVGLWidgetModelInput::LVGLWidgetModelInput(QObject *parent)
    : QAbstractListModel(parent) {}

int LVGLWidgetModelInput::rowCount(const QModelIndex &parent) const {
  Q_UNUSED(parent)
  return lvgl.widgetsInputW().size();
}

QVariant LVGLWidgetModelInput::data(const QModelIndex &index, int role) const {
  if (role == Qt::DisplayRole)
    return lvgl.widgetsInputW().at(index.row())->name();
  return QVariant();
}

Qt::ItemFlags LVGLWidgetModelInput::flags(const QModelIndex &index) const {
  Qt::ItemFlags defaultFlags = QAbstractListModel::flags(index);
  if (index.isValid())
    return Qt::ItemIsDragEnabled | defaultFlags;
  else
    return defaultFlags;
}

QStringList LVGLWidgetModelInput::mimeTypes() const {
  return QStringList() << "application/x-widget";
}

QMimeData *LVGLWidgetModelInput::mimeData(
    const QModelIndexList &indexes) const {
  QMimeData *mimeData = new QMimeData();
  QByteArray encodedData;

  LVGLWidgetCast cast;
  QDataStream stream(&encodedData, QIODevice::WriteOnly);
  for (const QModelIndex &index : indexes) {
    if (index.isValid() && index.column() == 0) {
      cast.ptr =
          const_cast<LVGLWidget *>(lvgl.widgetsInputW().at(index.row()));
      stream << cast.i;
    }
  }
  mimeData->setData("application/x-widget", encodedData);
  return mimeData;
}
