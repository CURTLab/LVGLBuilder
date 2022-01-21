#include "LVGLWidgetModelDisplay.h"

#include <QDataStream>
#include <QMimeData>

#include "LVGLCore.h"
#include "widgets/LVGLWidget.h"

LVGLWidgetModelDisplay::LVGLWidgetModelDisplay(QObject *parent)
    : QAbstractListModel(parent) {}

int LVGLWidgetModelDisplay::rowCount(const QModelIndex &parent) const {
  Q_UNUSED(parent)
  return lvgl.widgetsDisplayW().size();
}

QVariant LVGLWidgetModelDisplay::data(const QModelIndex &index,
                                      int role) const {
  if (role == Qt::DisplayRole)
    return lvgl.widgetsDisplayW().at(index.row())->name();
  else if (Qt::DecorationRole == role) {
    auto name = lvgl.widgetsDisplayW().at(index.row())->codename();
    return QIcon(QString(":/icons/%1.png").arg(name));
  }
  return QVariant();
}

Qt::ItemFlags LVGLWidgetModelDisplay::flags(const QModelIndex &index) const {
  Qt::ItemFlags defaultFlags = QAbstractListModel::flags(index);
  if (index.isValid())
    return Qt::ItemIsDragEnabled | defaultFlags;
  else
    return defaultFlags;
}

QStringList LVGLWidgetModelDisplay::mimeTypes() const {
  return QStringList() << "application/x-widget";
}

QMimeData *LVGLWidgetModelDisplay::mimeData(
    const QModelIndexList &indexes) const {
  QMimeData *mimeData = new QMimeData();
  QByteArray encodedData;

  LVGLWidgetCast cast;
  QDataStream stream(&encodedData, QIODeviceBase::WriteOnly);
  for (const QModelIndex &index : indexes) {
    if (index.isValid() && index.column() == 0) {
      cast.ptr =
          const_cast<LVGLWidget *>(lvgl.widgetsDisplayW().at(index.row()));
      stream << cast.i;
    }
  }
  mimeData->setData("application/x-widget", encodedData);
  return mimeData;
}
