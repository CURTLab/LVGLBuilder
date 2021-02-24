#ifndef LVGLWIDGETMODELINPUT_H
#define LVGLWIDGETMODELINPUT_H

#include <QAbstractListModel>

class LVGLWidget;

class LVGLWidgetModelInput : public QAbstractListModel {
  Q_OBJECT

 public:
  explicit LVGLWidgetModelInput(QObject *parent = nullptr);

  // Basic functionality:
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;

  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;

  Qt::ItemFlags flags(const QModelIndex &index) const override;

  virtual QStringList mimeTypes() const override;
  virtual QMimeData *mimeData(const QModelIndexList &indexes) const override;
};

#endif  // LVGLWIDGETMODEL_H
