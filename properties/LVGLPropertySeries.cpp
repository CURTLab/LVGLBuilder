#include "LVGLPropertySeries.h"

#include <QColorDialog>
#include <QDialog>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QJsonArray>
#include <QListWidget>
#include <QPixmap>
#include <QPushButton>
#include <QSpinBox>
#include <QStyledItemDelegate>
#include <QToolButton>

#include "LVGLCore.h"
#include "LVGLObject.h"

struct LVGLChartSeries {
  QColor color;
  QVector<lv_coord_t> points;
  lv_chart_series_t *ser;
};

class LVGLPropertySeriesDialog : public QDialog {
 public:
  LVGLPropertySeriesDialog(QWidget *parent = nullptr);

  QListWidget *m_seriesList;
  QListWidget *m_dataList;
  QVector<LVGLChartSeries> m_series;

  friend class LVGLPropertySeries;
};

class LVGLPropertySeriesDelegate : public QStyledItemDelegate {
  LVGLPropertySeriesDialog *m_dialog;

 public:
  LVGLPropertySeriesDelegate(LVGLPropertySeriesDialog *dialog)
      : m_dialog(dialog) {}

  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                        const QModelIndex &index) const override {
    Q_UNUSED(option)
    if (index.isValid()) {
      QSpinBox *widget = new QSpinBox(parent);
      widget->setRange(0, std::min(lvgl->width(), lvgl->height()));
      return widget;
    }
    return nullptr;
  }

  void setEditorData(QWidget *editor, const QModelIndex &index) const override {
    if (index.isValid()) {
      QSpinBox *widget = qobject_cast<QSpinBox *>(editor);
      widget->setValue(index.data().toInt());
    }
  }

  void setModelData(QWidget *editor, QAbstractItemModel *model,
                    const QModelIndex &index) const override {
    if (index.isValid()) {
      QSpinBox *widget = qobject_cast<QSpinBox *>(editor);
      model->setData(index, widget->value());

      int idx = m_dialog->m_dataList->currentRow();
      if (idx > -1)
        m_dialog->m_series[m_dialog->m_seriesList->currentRow()].points[idx] =
            static_cast<lv_coord_t>(widget->value());
    }
  }
};

LVGLPropertySeriesDialog::LVGLPropertySeriesDialog(QWidget *parent)
    : QDialog(parent) {
  m_seriesList = new QListWidget;
  m_dataList = new QListWidget;
  m_dataList->setItemDelegate(new LVGLPropertySeriesDelegate(this));

  QToolButton *color = new QToolButton;
  QPushButton *add1 = new QPushButton("Add series");
  QPushButton *rem1 = new QPushButton("Remove series");
  QPushButton *add2 = new QPushButton("Add point");
  QPushButton *rem2 = new QPushButton("Remove point");

  QPixmap pix(16, 16);
  pix.fill(Qt::red);
  color->setIcon(QIcon(pix));

  QDialogButtonBox *box =
      new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  connect(box, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(box, &QDialogButtonBox::rejected, this, &QDialog::reject);

  QGridLayout *layout1 = new QGridLayout;
  layout1->addWidget(add1, 0, 0);
  layout1->addWidget(rem1, 1, 0);
  layout1->addWidget(m_seriesList, 2, 0, 2, 1);
  layout1->addWidget(add2, 0, 1);
  layout1->addWidget(rem2, 1, 1);
  layout1->addWidget(color, 2, 1);
  layout1->addWidget(m_dataList, 3, 1);
  layout1->addWidget(box, 4, 0, 1, 2);

  add2->setEnabled(false);
  rem2->setEnabled(false);
  color->setEnabled(false);

  connect(add1, &QPushButton::clicked, [this](bool) {
    QList<int> numbers;
    for (int i = 0; i < m_seriesList->count(); ++i) {
      QString text = m_seriesList->item(i)->text();
      int num = text.mid(text.indexOf(' ') + 1).toInt();
      auto ind = std::lower_bound(numbers.begin(), numbers.end(), num);
      numbers.insert(ind, num);
    }
    int id = 1;
    for (int num : numbers) {
      if (num == id)
        id++;
      else
        break;
    }
    LVGLChartSeries series;
    series.color = Qt::red;
    m_series << series;
    m_seriesList->addItem(QString("Series %1").arg(id));
  });
  connect(rem1, &QPushButton::clicked, [this](bool) {
    int idx = m_seriesList->currentRow();
    if (idx > -1) {
      m_seriesList->takeItem(idx);
      m_series.removeAt(idx);
    }
  });
  connect(m_seriesList, &QListWidget::currentRowChanged, [=](int idx) {
    const bool isValid = (idx >= 0);
    add2->setEnabled(isValid);
    rem2->setEnabled(isValid);
    color->setEnabled(isValid);

    if (isValid) {
      QPixmap pix(16, 16);
      pix.fill(m_series[idx].color);
      color->setIcon(QIcon(pix));

      m_dataList->clear();
      for (const auto &p : m_series[idx].points) {
        QListWidgetItem *item = new QListWidgetItem(QString::number(p));
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        m_dataList->addItem(item);
      }
    }
  });
  connect(color, &QPushButton::clicked, [this, color](bool) {
    QColorDialog dialog(this);
    int idx = m_seriesList->currentRow();
    dialog.setCurrentColor(m_series[idx].color);
    if (dialog.exec() == QDialog::Accepted) {
      QPixmap pix(16, 16);
      pix.fill(dialog.selectedColor());
      color->setIcon(QIcon(pix));
      m_series[idx].color = dialog.selectedColor();
    }
  });
  connect(add2, &QPushButton::clicked, [this](bool) {
    m_series[m_seriesList->currentRow()].points << 0;
    QListWidgetItem *item = new QListWidgetItem("0");
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    m_dataList->addItem(item);
  });
  connect(rem2, &QPushButton::clicked, [this](bool) {
    int idx = m_seriesList->currentRow();
    if (idx > -1) {
      m_dataList->takeItem(idx);
      m_series[m_seriesList->currentRow()].points.removeAt(idx);
    }
  });

  setLayout(layout1);
}

LVGLPropertySeries::LVGLPropertySeries(LVGLProperty *parent)
    : LVGLProperty(parent) {}

QString LVGLPropertySeries::name() const { return "Series"; }

bool LVGLPropertySeries::hasEditor() const { return true; }

QWidget *LVGLPropertySeries::editor(QWidget *parent) {
  m_widget = new LVGLPropertySeriesDialog(parent);
  return m_widget;
}

void LVGLPropertySeries::updateEditor(LVGLObject *obj) {
  lv_obj_t *chart = obj->obj();
  Q_ASSERT(lv_debug_check_obj_type(chart, "lv_chart"));

  lv_chart_ext_t *ext =
      reinterpret_cast<lv_chart_ext_t *>(lv_obj_get_ext_attr(chart));

  lv_chart_series_t *ser;
  size_t i = 1;
  for (ser = reinterpret_cast<lv_chart_series_t *>(
           _lv_ll_get_tail(&ext->series_ll));
       ser != nullptr; ser = reinterpret_cast<lv_chart_series_t *>(
                           _lv_ll_get_prev(&ext->series_ll, ser)),
      ++i) {
    LVGLChartSeries series;
    series.color = lvgl->toColor(ser->color);
    const uint16_t ctx = ser->start_point == 0 ? lv_chart_get_point_cnt(chart)
                                               : ser->start_point;
    for (uint16_t j = 0; j < ctx; ++j) series.points << ser->points[j];
    m_widget->m_series << series;
    m_widget->m_seriesList->addItem(QString("Series %1").arg(i));
  }
}

void LVGLPropertySeries::updateWidget(LVGLObject *obj) {
  lv_obj_t *chart = obj->obj();
  Q_ASSERT(lv_debug_check_obj_type(chart, "lv_chart"));

  lv_chart_ext_t *ext =
      reinterpret_cast<lv_chart_ext_t *>(lv_obj_get_ext_attr(chart));

  int maxPoints = 0;
  for (const auto &s : m_widget->m_series)
    maxPoints = std::max(maxPoints, s.points.size());

  // clear the chart series (maybe lvgl should add a function for this)
  lv_chart_series_t *ser;
  for (ser = reinterpret_cast<lv_chart_series_t *>(
           _lv_ll_get_tail(&ext->series_ll));
       ser != nullptr; ser = reinterpret_cast<lv_chart_series_t *>(
                           _lv_ll_get_prev(&ext->series_ll, ser))) {
    lv_mem_free(ser->points);
    lv_mem_free(ser);
  }
  _lv_ll_clear(&ext->series_ll);

  // add new charts
  lv_chart_set_point_count(chart, static_cast<uint16_t>(maxPoints));
  for (const auto &s : m_widget->m_series) {
    lv_chart_series_t *ser =
        lv_chart_add_series(chart, lvgl->fromColor(s.color));
    for (int i = 0; i < s.points.size(); ++i)
      lv_chart_set_next(chart, ser, s.points[i]);
  }
}

QStringList LVGLPropertySeries::function(LVGLObject *obj) const {
  QStringList ret;

  lv_obj_t *chart = obj->obj();
  Q_ASSERT(lv_debug_check_obj_type(chart, "lv_chart"));

  lv_chart_ext_t *ext =
      reinterpret_cast<lv_chart_ext_t *>(lv_obj_get_ext_attr(chart));

  lv_chart_series_t *ser;
  size_t i = 1;
  for (ser = reinterpret_cast<lv_chart_series_t *>(
           _lv_ll_get_tail(&ext->series_ll));
       ser != nullptr; ser = reinterpret_cast<lv_chart_series_t *>(
                           _lv_ll_get_prev(&ext->series_ll, ser)),
      ++i) {
    const QString codeSerName =
        QString("ser_%1_%2").arg(obj->codeName()).arg(i);
    const QString color =
        QVariant(lvgl->toColor(ser->color)).toString().replace("#", "0x");
    ret << QString(
               "lv_chart_series_t * %1 = lv_chart_add_series(%2, "
               "lv_color_hex(%3));")
               .arg(codeSerName)
               .arg(obj->codeName())
               .arg(color);
    const uint16_t ctx = ser->start_point == 0 ? lv_chart_get_point_cnt(chart)
                                               : ser->start_point;
    for (uint16_t j = 0; j < ctx; ++j)
      ret << QString("lv_chart_set_next(%1, %2, %3);")
                 .arg(obj->codeName())
                 .arg(codeSerName)
                 .arg(ser->points[j]);
    ret << "";
  }
  return ret;
}

QJsonValue LVGLPropertySeries::toJson(LVGLObject *obj) const {
  QJsonArray ret;

  lv_obj_t *chart = obj->obj();
  Q_ASSERT(lv_debug_check_obj_type(chart, "lv_chart"));

  lv_chart_ext_t *ext =
      reinterpret_cast<lv_chart_ext_t *>(lv_obj_get_ext_attr(chart));

  lv_chart_series_t *ser;
  for (ser = reinterpret_cast<lv_chart_series_t *>(
           _lv_ll_get_tail(&ext->series_ll));
       ser != nullptr; ser = reinterpret_cast<lv_chart_series_t *>(
                           _lv_ll_get_prev(&ext->series_ll, ser))) {
    QJsonArray values;
    const uint16_t ctx = ser->start_point == 0 ? lv_chart_get_point_cnt(chart)
                                               : ser->start_point;
    for (uint16_t j = 0; j < ctx; ++j) values.append(ser->points[j]);
    QJsonObject series(
        {{"color", lvgl->colorToJson(ser->color)}, {"values", values}});
    ret.append(series);
  }
  return ret;
}

void LVGLPropertySeries::setValue(LVGLObject *obj, QVariant value) {
  lv_obj_t *chart = obj->obj();
  Q_ASSERT(lv_debug_check_obj_type(chart, "lv_chart"));

  if (value.type() == QVariant::List) {
    QVariantList series = value.toList();
    QVector<LVGLChartSeries> seriesList;
    int maxPoints = 0;
    for (const QVariant &s : series) {
      QVariantMap map = s.toMap();
      if (!map.contains("color") || !map.contains("values")) continue;
      LVGLChartSeries item;
      item.ser = lv_chart_add_series(chart, lvgl->fromColor(map["color"]));
      for (const QVariant &v : map["values"].toList()) {
        item.points << static_cast<lv_coord_t>(v.toInt());

        // lv_chart_set_next(chart, ser, static_cast<lv_coord_t>(v.toInt()));
      }
      maxPoints = std::max(maxPoints, item.points.size());
      seriesList << item;
    }

    lv_chart_set_point_count(chart, static_cast<uint16_t>(maxPoints));
    for (const auto &s : seriesList) {
      for (int i = 0; i < s.points.size(); ++i)
        lv_chart_set_next(chart, s.ser, s.points[i]);
    }
  }
}
