#ifndef LVGLPROPERTYPOINTS_H
#define LVGLPROPERTYPOINTS_H

#include <QDialog>
#include <QStyledItemDelegate>

#include "core/LVGLProperty.h"

class LVGLPropertyPointsDialog : public QDialog {
 public:
  LVGLPropertyPointsDialog(QWidget *parent = nullptr);

  void setPoints(QVector<lv_point_t> points);
  QVector<lv_point_t> points() const;

 private:
  class QTreeWidget *m_list;
};

class LVGLPropertyPointsDelegate : public QStyledItemDelegate {
 public:
  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                        const QModelIndex &index) const override;
  void setEditorData(QWidget *editor, const QModelIndex &index) const override;
  void setModelData(QWidget *editor, QAbstractItemModel *model,
                    const QModelIndex &index) const override;
};

class LVGLPropertyPoints : public LVGLProperty {
 public:
  LVGLPropertyPoints(LVGLProperty *parent = nullptr);

  bool hasEditor() const override;
  QWidget *editor(QWidget *parent) override;
  void updateEditor(LVGLObject *obj) override;
  void updateWidget(LVGLObject *obj) override;

  QVariant value(LVGLObject *obj) const override;
  void setValue(LVGLObject *obj, QVariant value) override;

  QJsonValue toJson(LVGLObject *obj) const override;

 protected:
  class LVGLPropertyPointsDialog *m_widget;
  virtual QVector<lv_point_t> get(LVGLObject *obj) const = 0;
  virtual void set(LVGLObject *obj, QVector<lv_point_t> data) = 0;
};

#endif  // LVGLPROPERTYPOINTS_H
