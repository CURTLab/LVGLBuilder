#include "LVGLPropertyDateList.h"

#include <QDate>
#include <QDateTimeEdit>
#include <QDialog>
#include <QDialogButtonBox>
#include <QJsonArray>
#include <QStyledItemDelegate>
#include <QToolButton>
#include <QTreeWidget>
#include <QVBoxLayout>

#include "core/LVGLObject.h"

class LVGLPropertyDateListDelegate : public QStyledItemDelegate {
 public:
  inline QWidget *createEditor(QWidget *parent,
                               const QStyleOptionViewItem &option,
                               const QModelIndex &index) const override {
    Q_UNUSED(option)
    if (index.isValid()) {
      QDateTimeEdit *widget = new QDateTimeEdit(parent);
      // widget->setCalendarPopup(true);
      widget->setDisplayFormat("dd.MM.yyyy");
      return widget;
    }
    return nullptr;
  }

  inline void setEditorData(QWidget *editor,
                            const QModelIndex &index) const override {
    if (index.isValid()) {
      QDateTimeEdit *widget = qobject_cast<QDateTimeEdit *>(editor);
      widget->setDate(index.data().toDate());
    }
  }

  inline void setModelData(QWidget *editor, QAbstractItemModel *model,
                           const QModelIndex &index) const override {
    if (index.isValid()) {
      QDateTimeEdit *widget = qobject_cast<QDateTimeEdit *>(editor);
      model->setData(index, widget->date());
    }
  }
};

class LVGLPropertyDateListDialog : public QDialog {
 public:
  inline LVGLPropertyDateListDialog(QWidget *parent = nullptr)
      : QDialog(parent) {
    m_list = new QTreeWidget(this);
    m_list->setColumnCount(1);
    m_list->setHeaderLabels(QStringList() << "Date");
    m_list->setItemDelegate(new LVGLPropertyDateListDelegate);

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
                item->setData(0, Qt::DisplayRole, QDate::currentDate());
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

  inline QVariant get() const {
    QVariantList ret;
    for (int i = 0; i < m_list->topLevelItemCount(); ++i) {
      QTreeWidgetItem *item = m_list->topLevelItem(i);
      ret << item->data(0, Qt::DisplayRole);
    }
    return ret;
  }

  inline void set(QVariant value) {
    m_list->clear();
    if (value.type() != QVariant::List) return;
    QVariantList list = value.toList();
    for (const QVariant &p : list) {
      if (p.type() != QVariant::Date) continue;
      QTreeWidgetItem *item = new QTreeWidgetItem;
      item->setData(0, Qt::DisplayRole, p);
      item->setFlags(item->flags() | Qt::ItemIsEditable);
      m_list->addTopLevelItem(item);
    }
  }

 private:
  QTreeWidget *m_list;
};

LVGLPropertyDateList::LVGLPropertyDateList(
    QString title, QString functionName,
    std::function<void(lv_obj_t *, lv_calendar_date_t *, uint16_t)> setter,
    std::function<lv_calendar_date_t *(lv_obj_t *)> getter,
    std::function<uint16_t(lv_obj_t *)> count, LVGLProperty *parent)
    : LVGLProperty(parent),
      m_widget(nullptr),
      m_title(title),
      m_functionName(functionName),
      m_setter(setter),
      m_getter(getter),
      m_count(count) {}

LVGLPropertyDateList::~LVGLPropertyDateList() {
  for (lv_calendar_date_t *d : m_garbageCollector) delete[] d;
}

bool LVGLPropertyDateList::hasEditor() const { return true; }

QWidget *LVGLPropertyDateList::editor(QWidget *parent) {
  m_widget = new LVGLPropertyDateListDialog(parent);
  return m_widget;
}

void LVGLPropertyDateList::updateEditor(LVGLObject *obj) {
  m_widget->set(value(obj));
}

void LVGLPropertyDateList::updateWidget(LVGLObject *obj) {
  if (m_widget->result() == QDialog::Accepted) setValue(obj, m_widget->get());
}

QVariant LVGLPropertyDateList::value(LVGLObject *obj) const {
  const uint16_t num = count(obj);
  if (num == 0) return "None";
  const lv_calendar_date_t *items = get(obj);
  QVariantList ret;
  for (uint16_t i = 0; i < num; ++i)
    ret << QDate(items[i].year, items[i].month, items[i].day);
  return ret;
}

void LVGLPropertyDateList::setValue(LVGLObject *obj, QVariant value) {
  if (value.type() != QVariant::List) return;
  QVariantList list = value.toList();
  QVector<lv_calendar_date_t> dateVector;
  for (const QVariant &p : list) {
    QDate d = p.toDate();
    if (!d.isValid() || d.isNull()) continue;
    lv_calendar_date_t data{static_cast<uint16_t>(d.year()),
                            static_cast<int8_t>(d.month()),
                            static_cast<int8_t>(d.day())};
    dateVector << data;
  }

  // in order to not allocate unneeded memory
  if (dateVector.isEmpty()) return;

  // Due to the doc: ONLY A POINTER WILL BE SAVED! CAN’T BE LOCAL ARRAY.
  lv_calendar_date_t *data = new lv_calendar_date_t[dateVector.size()];
  memcpy(data, dateVector.data(),
         dateVector.size() * sizeof(lv_calendar_date_t));
  m_garbageCollector << data;
  set(obj, data, dateVector.size());
}

QJsonValue LVGLPropertyDateList::toJson(LVGLObject *obj) const {
  QJsonArray ret;
  const uint16_t num = count(obj);
  if (num == 0) return {};

  const lv_calendar_date_t *items = get(obj);
  for (uint16_t i = 0; i < num; ++i)
    ret << QJsonValue::fromVariant(
        QDate(items[i].year, items[i].month, items[i].day));
  return ret;
}

QString LVGLPropertyDateList::name() const { return m_title; }

QStringList LVGLPropertyDateList::function(LVGLObject *obj) const {
  const uint16_t num = count(obj);
  if (m_functionName.isEmpty() || (num == 0)) return {};
  QStringList ret;
  const QString varName = QString("highlihted_days_%1_%2")
                              .arg(m_title.toLower().replace(' ', '_'))
                              .arg(obj->codeName());

  QStringList items;
  const lv_calendar_date_t *dates = get(obj);
  for (uint16_t i = 0; i < num; ++i)
    items << QString("{.year = %2, .month = %3, .day = %4}")
                 .arg(dates[i].year)
                 .arg(dates[i].month)
                 .arg(dates[i].day);
  ret << QString("static lv_calendar_date_t %1[%2] = {%3};")
             .arg(varName)
             .arg(num)
             .arg(items.join(","));
  ret << QString("%1(%2, %3, %4);")
             .arg(m_functionName)
             .arg(obj->codeName())
             .arg(varName)
             .arg(num);
  return ret;
}

uint16_t LVGLPropertyDateList::count(LVGLObject *obj) const {
  return m_count(obj->obj());
}

lv_calendar_date_t *LVGLPropertyDateList::get(LVGLObject *obj) const {
  return m_getter(obj->obj());
}

void LVGLPropertyDateList::set(LVGLObject *obj, lv_calendar_date_t *value,
                               uint16_t num) {
  m_setter(obj->obj(), value, num);
}
