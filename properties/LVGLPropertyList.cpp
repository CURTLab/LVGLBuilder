#include "LVGLPropertyList.h"

#include <QComboBox>
#include <QDebug>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QJsonArray>
#include <QLineEdit>
#include <QStyledItemDelegate>
#include <QToolButton>
#include <QTreeWidget>
#include <QVBoxLayout>

#include "LVGLCore.h"
#include "LVGLImageData.h"
#include "LVGLObject.h"

class LVGLPropertyListDelegate : public QStyledItemDelegate {
 public:
  inline QWidget *createEditor(QWidget *parent,
                               const QStyleOptionViewItem &option,
                               const QModelIndex &index) const override {
    Q_UNUSED(option)
    if (!index.isValid()) return nullptr;

    if (index.column() == 0) {
      return new QLineEdit(parent);
    } else if (index.column() == 1) {
      QComboBox *editor = new QComboBox(parent);
      editor->addItem("None");
      editor->addItems(lvgl.imageNames());
      editor->addItems(lvgl.symbolNames());
      return editor;
    }

    return nullptr;
  }

  inline void setEditorData(QWidget *editor,
                            const QModelIndex &index) const override {
    if (!index.isValid()) return;

    if (index.column() == 0) {
      QLineEdit *widget = qobject_cast<QLineEdit *>(editor);
      widget->setText(index.data().toString());
    } else if (index.column() == 1) {
      QComboBox *widget = qobject_cast<QComboBox *>(editor);
      widget->setCurrentText(index.data().toString());
    }
  }

  inline void setModelData(QWidget *editor, QAbstractItemModel *model,
                           const QModelIndex &index) const override {
    if (!index.isValid()) return;

    if (index.column() == 0) {
      QLineEdit *widget = qobject_cast<QLineEdit *>(editor);
      model->setData(index, widget->text());
    } else if (index.column() == 1) {
      QComboBox *widget = qobject_cast<QComboBox *>(editor);
      model->setData(index, widget->currentText());
    }
  }
};

LVGLPropertyListDialog::LVGLPropertyListDialog(QWidget *parent)
    : QDialog(parent) {
  m_list = new QTreeWidget(this);
  m_list->setColumnCount(2);
  m_list->setHeaderLabels(QStringList() << "Name"
                                        << "Image");
  m_list->setItemDelegate(new LVGLPropertyListDelegate);
  QDialogButtonBox *box =
      new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  QToolButton *add = new QToolButton();
  add->setIcon(QIcon(":/icons/add.png"));
  add->setIconSize(QSize(24, 24));
  QToolButton *rem = new QToolButton();
  rem->setIcon(QIcon(":/icons/delete.png"));
  rem->setIconSize(QSize(24, 24));

  box->addButton(add, QDialogButtonBox::ApplyRole);
  box->addButton(rem, QDialogButtonBox::ApplyRole);

  connect(box, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(box, &QDialogButtonBox::rejected, this, &QDialog::reject);
  connect(box, &QDialogButtonBox::clicked,
          [this, add, rem](QAbstractButton *b) {
            if (b == add) {
              QTreeWidgetItem *item = new QTreeWidgetItem;
              item->setText(0, "New item");
              item->setText(1, "None");
              item->setFlags(item->flags() | Qt::ItemIsEditable);
              m_list->addTopLevelItem(item);
            } else if (b == rem) {
              int index = m_list->indexOfTopLevelItem(m_list->currentItem());
              m_list->takeTopLevelItem(index);
            }
          });

  QVBoxLayout *layout1 = new QVBoxLayout;
  layout1->addWidget(m_list);
  layout1->addWidget(box);

  setLayout(layout1);
}

void LVGLPropertyListDialog::setListItems(QList<LVGLListItem> items) {
  for (const LVGLListItem &i : items) {
    QTreeWidgetItem *item = new QTreeWidgetItem;
    item->setText(0, i.name);
    QString img_name = "None";
    if (i.image_src != nullptr) {
      const lv_img_dsc_t *img =
          reinterpret_cast<const lv_img_dsc_t *>(i.image_src);
      const LVGLImageData *img_data = lvgl.imageByDesc(img);
      if (img_data) img_name = img_data->name();
    }
    item->setText(1, img_name);
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    m_list->addTopLevelItem(item);
  }
}

QList<LVGLListItem> LVGLPropertyListDialog::listItems() const {
  QList<LVGLListItem> ret;
  for (int i = 0; i < m_list->topLevelItemCount(); ++i) {
    QTreeWidgetItem *item = m_list->topLevelItem(i);
    void *img_src = nullptr;
    if (item->text(1) != "None") {
      img_src = lvgl.image(item->text(1));
      if (img_src == nullptr)
        img_src = const_cast<char *>(lvgl.symbol(item->text(1)));
    }
    ret << LVGLListItem{item->text(0), img_src};
  }
  return ret;
}

LVGLPropertyList::LVGLPropertyList(LVGLProperty *parent)
    : LVGLProperty(parent) {}

QString LVGLPropertyList::name() const { return "Items"; }

bool LVGLPropertyList::hasEditor() const { return true; }

QWidget *LVGLPropertyList::editor(QWidget *parent) {
  m_widget = new LVGLPropertyListDialog(parent);
  return m_widget;
}

void LVGLPropertyList::updateWidget(LVGLObject *obj) {
  lv_list_clean(obj->obj());
  for (const LVGLListItem &item : m_widget->listItems())
    lv_list_add_btn(obj->obj(), item.image_src, qUtf8Printable(item.name));
}

QVariant LVGLPropertyList::value(LVGLObject *obj) const {
  (void)obj;
  return "(items)";

  /*QStringList ret;
  lv_obj_t * e = lv_list_get_next_btn(obj->obj(), nullptr);
  while (e != nullptr) {
          ret << QString(lv_list_get_btn_text(e));
          e = lv_list_get_next_btn(obj->obj(), e);
  }
  if (ret.isEmpty())
          return "(empty)";
  return ret;*/
}

void LVGLPropertyList::setValue(LVGLObject *obj, QVariant value) {
  if (value.type() == QVariant::List) {
    lv_list_clean(obj->obj());
#if 0
		for (const QVariant &v:value.toList())
            lv_list_add_btn(obj->obj(), nullptr, qUtf8Printable(v.toString()));
#else
    for (const QVariant &v : value.toList()) {
      QVariantMap m = v.toMap();
      void *img_src = nullptr;
      if (m["img"].toString() != "None")
        img_src = lvgl.image(m["img"].toString());
      lv_list_add_btn(obj->obj(), img_src,
                      qUtf8Printable(m["name"].toString()));
    }
#endif
  }
}

QJsonValue LVGLPropertyList::toJson(LVGLObject *obj) const {
#if 0
	QJsonArray ret;
	lv_obj_t * e = lv_list_get_next_btn(obj->obj(), nullptr);
	while (e != nullptr) {
		ret << QString(lv_list_get_btn_text(e));
		e = lv_list_get_next_btn(obj->obj(), e);
	}
	return ret;
#else
  QJsonArray ret;
  lv_obj_t *e = lv_list_get_next_btn(obj->obj(), nullptr);
  while (e != nullptr) {
    QString img_name = "None";
    lv_obj_t *img_obj = lv_list_get_btn_img(e);
    if (img_obj) {
      const void *img_src = lv_img_get_src(img_obj);
      if (img_src != nullptr) {
        const lv_img_dsc_t *img =
            reinterpret_cast<const lv_img_dsc_t *>(img_src);
        const LVGLImageData *img_data = lvgl.imageByDesc(img);
        if (img_data) img_name = img_data->name();
      }
    }

    ret << QJsonObject({{"name", lv_list_get_btn_text(e)}, {"img", img_name}});
    e = lv_list_get_next_btn(obj->obj(), e);
  }
  return ret;
#endif
}

QStringList LVGLPropertyList::function(LVGLObject *obj) const {
  QStringList ret;
  lv_obj_t *e = lv_list_get_next_btn(obj->obj(), nullptr);
  while (e != nullptr) {
    QString img_name = "NULL";
    lv_obj_t *img_obj = lv_list_get_btn_img(e);
    if (img_obj) {
      const void *img_src = lv_img_get_src(lv_list_get_btn_img(e));
      if (img_src != nullptr) {
        const lv_img_dsc_t *img =
            reinterpret_cast<const lv_img_dsc_t *>(img_src);
        const LVGLImageData *img_data = lvgl.imageByDesc(img);
        if (img_data) img_name = "&" + img_data->codeName();
      }
    }
    ret << QString("lv_list_add_btn(%1, %2, \"%3\");")
               .arg(obj->codeName())
               .arg(img_name)
               .arg(lv_list_get_btn_text(e));
    e = lv_list_get_next_btn(obj->obj(), e);
  }
  return ret;
}

void LVGLPropertyList::updateEditor(LVGLObject *obj) {
  QList<LVGLListItem> items;
  lv_obj_t *e = lv_list_get_next_btn(obj->obj(), nullptr);
  while (e != nullptr) {
    items << LVGLListItem{QString(lv_list_get_btn_text(e)),
                          lv_list_get_btn_img(e)};
    e = lv_list_get_next_btn(obj->obj(), e);
  }
  m_widget->setListItems(items);
}
