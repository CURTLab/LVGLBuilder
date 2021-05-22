#ifndef LVGLSTYLEMODEL_H
#define LVGLSTYLEMODEL_H

#include <QAbstractItemModel>
#include <QDialog>
#include <QStyledItemDelegate>

#include "LVGLStyle.h"
#include "widgets/LVGLWidget.h"

class LVGLObject;

class LVGLStyleModel : public QAbstractItemModel {
  Q_OBJECT

 public:
  explicit LVGLStyleModel(QObject *parent = nullptr);
  ~LVGLStyleModel();

  // Header:
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;

  // Basic functionality:
  QModelIndex index(int row, int column,
                    const QModelIndex &parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex &index) const override;

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;

  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;

  Qt::ItemFlags flags(const QModelIndex &index) const override;

  LVGLStyle *styleBase() const;

  void setStyle(lv_style_t *style,
                LVGL::StyleParts editableStyles = LVGL::None);

  void setObj(lv_obj_t *obj) { m_styleBase->setCurobj(obj); }
  void setLvglObj(LVGLObject *obj) { m_styleBase->setLvglObj(obj); }
  void setState(lv_state_t state) { m_styleBase->setState(state); }
  void setPart(lv_obj_part_t part) { m_styleBase->setPart(part); }

 signals:
  void styleChanged();

 public slots:

 private:
  void updateValue(const LVGLStyleItem *item);
  QModelIndex indexOf(const LVGLStyleItem *item, int column);

  LVGLStyle *m_styleBase;
  LVGL::StyleParts m_editableStyles;
};

class LVGLBodyPartDialog : public QDialog {
  Q_OBJECT
 public:
  LVGLBodyPartDialog(QWidget *parent = nullptr);

  void setBodyParts(lv_border_side_t parts);
  lv_border_side_t bodyParts() const;

 private:
  class QListWidget *m_list;
};

class LVGLStyleDelegate : public QStyledItemDelegate {
 public:
  LVGLStyleDelegate(LVGLStyle *styleBase, QObject *parent = nullptr);

  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                        const QModelIndex &index) const override;
  void setEditorData(QWidget *editor, const QModelIndex &index) const override;
  void setModelData(QWidget *editor, QAbstractItemModel *model,
                    const QModelIndex &index) const override;

 private:
  LVGLStyle *m_styleBase;
};

#endif  // LVGLSTYLEMODEL_H
