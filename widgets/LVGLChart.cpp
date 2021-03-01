#include "LVGLChart.h"

#include <QIcon>

#include "LVGLObject.h"
#include "lvgl/src/lv_core/lv_obj.h"
#include "lvgl/src/lv_widgets/lv_chart.h"
#include "properties/LVGLPropertyAnyFunc.h"
#include "properties/LVGLPropertyFlags.h"
#include "properties/LVGLPropertyRange.h"
#include "properties/LVGLPropertySeries.h"

// class LVGLPropertyChartType : public LVGLPropertyFlags {
// public:
//  LVGLPropertyChartType()
//      : LVGLPropertyFlags(
//            {"None", "Line", "Columns", "Points", "Area", "Vertical lines"},
//            {"LV_CHART_TYPE_NONE", "LV_CHART_TYPE_LINE",
//            "LV_CHART_TYPE_COLUMN",
//             "LV_CHART_TYPE_POINT", "LV_CHART_TYPE_AREA",
//             "LV_CHART_TYPE_VERTICAL_LINE"},
//            {LV_CHART_TYPE_NONE, LV_CHART_TYPE_LINE, LV_CHART_TYPE_COLUMN,
//             LV_CHART_TYPE_POINT, LV_CHART_TYPE_AREA,
//             LV_CHART_TYPE_VERTICAL_LINE}) {}

//  QString name() const { return "Type"; }

//  QStringList function(LVGLObject *obj) const {
//    return QStringList() << QString("lv_chart_set_type(%1, %2);")
//                                .arg(obj->codeName())
//                                .arg(codeValue(obj));
//  }

// protected:
//  int get(LVGLObject *obj) const { return lv_chart_get_type(obj->obj()); }
//  void set(LVGLObject *obj, int index) {
//    lv_chart_set_type(obj->obj(), index & 0xff);
//  }
//};

class LVGLPropertyChartRange : public LVGLPropertyRange {
 public:
  QString name() const override { return "Y range"; }

  QStringList function(LVGLObject *obj) const override {
    return QStringList() << QString("lv_chart_set_range(%1, %2, %3);")
                                .arg(obj->codeName())
                                .arg(getMin(obj))
                                .arg(getMax(obj));
  }

 protected:
  int getMin(LVGLObject *obj) const override {
    lv_chart_ext_t *ext = (lv_chart_ext_t *)lv_obj_get_ext_attr(obj->obj());
    return ext->ymin[LV_CHART_AXIS_PRIMARY_Y];
  }
  int getMax(LVGLObject *obj) const override {
    lv_chart_ext_t *ext = (lv_chart_ext_t *)lv_obj_get_ext_attr(obj->obj());
    return ext->ymax[LV_CHART_AXIS_PRIMARY_Y];
  }
  void set(LVGLObject *obj, int min, int max) override {
    lv_chart_set_range(obj->obj(), static_cast<lv_coord_t>(min),
                       static_cast<lv_coord_t>(max));
  }
};

class LVGLPropertyChartXDiv : public LVGLPropertyInt {
 public:
  LVGLPropertyChartXDiv() : LVGLPropertyInt(0, 255) {}

  QString name() const override { return "X division lines"; }

 protected:
  virtual int get(LVGLObject *obj) const override {
    return reinterpret_cast<lv_chart_ext_t *>(lv_obj_get_ext_attr(obj->obj()))
        ->vdiv_cnt;
  }
  virtual void set(LVGLObject *obj, int value) override {
    lv_chart_ext_t *ext =
        reinterpret_cast<lv_chart_ext_t *>(lv_obj_get_ext_attr(obj->obj()));
    lv_chart_set_div_line_count(obj->obj(), ext->hdiv_cnt,
                                static_cast<uint8_t>(value));
  }
};

class LVGLPropertyChartYDiv : public LVGLPropertyInt {
 public:
  LVGLPropertyChartYDiv() : LVGLPropertyInt(0, 255) {}

  QString name() const override { return "Y division lines"; }

 protected:
  virtual int get(LVGLObject *obj) const override {
    return reinterpret_cast<lv_chart_ext_t *>(lv_obj_get_ext_attr(obj->obj()))
        ->hdiv_cnt;
  }
  virtual void set(LVGLObject *obj, int value) override {
    lv_chart_ext_t *ext =
        reinterpret_cast<lv_chart_ext_t *>(lv_obj_get_ext_attr(obj->obj()));
    lv_chart_set_div_line_count(obj->obj(), static_cast<uint8_t>(value),
                                ext->vdiv_cnt);
  }
};

// class LVGLPropertyChartWidth : public LVGLPropertyCoord {
// public:
//  LVGLPropertyChartWidth(LVGLProperty *p) : LVGLPropertyCoord(p) {}

//  QString name() const override { return "Width"; }

// protected:
//  virtual lv_coord_t get(LVGLObject *obj) const override {
//    return lvchgets(obj->obj());
//  }
//  virtual void set(LVGLObject *obj, lv_coord_t value) override {
//    lv_chart_set_series_width(obj->obj(), value);
//  }
//};

// class LVGLPropertyChartOpa : public LVGLPropertyInt {
// public:
//  LVGLPropertyChartOpa(LVGLProperty *p) : LVGLPropertyInt(0, 255, p) {}

//  QString name() const override { return "Opacity"; }

// protected:
//  virtual int get(LVGLObject *obj) const override {
//    return lv_chart_get_series_opa(obj->obj());
//  }
//  virtual void set(LVGLObject *obj, int value) override {
//    lv_chart_set_series_opa(obj->obj(), static_cast<lv_opa_t>(value));
//  }
//};

// class LVGLPropertyChartDarking : public LVGLPropertyInt {
// public:
//  LVGLPropertyChartDarking(LVGLProperty *p) : LVGLPropertyInt(0, 255, p) {}

//  QString name() const override { return "Dark fade"; }

// protected:
//  virtual int get(LVGLObject *obj) const override {
//    return lv_chart_get_series_darking(obj->obj());
//  }
//  virtual void set(LVGLObject *obj, int value) override {
//    lv_chart_set_series_darking(obj->obj(), static_cast<lv_opa_t>(value));
//  }
//};

// class LVGLPropertyChartSeries : public LVGLProperty {
//  LVGLPropertyChartWidth *m_w;
//  LVGLPropertyChartOpa *m_opa;
//  LVGLPropertyChartDarking *m_darking;

// public:
//  LVGLPropertyChartSeries()
//      : m_w(new LVGLPropertyChartWidth(this)),
//        m_opa(new LVGLPropertyChartOpa(this)),
//        m_darking(new LVGLPropertyChartDarking(this)) {
//    m_childs << m_w << m_opa << m_darking;
//  }

//  QString name() const override { return "Series"; }
//};

// class LVGLPropertyChartMargin : public LVGLPropertyInt {
// public:
//  LVGLPropertyChartMargin(LVGLProperty *p = nullptr)
//      : LVGLPropertyInt(0, UINT16_MAX, p) {}

//  QString name() const override { return "Margin"; }

// protected:
//  virtual int get(LVGLObject *obj) const override {
//    return lv_chart_get_margin(obj->obj());
//  }
//  virtual void set(LVGLObject *obj, int value) override {
//    lv_chart_set_margin(obj->obj(), static_cast<uint16_t>(value));
//  }
//};

class LVGLPropertyChartSeries : public LVGLPropertyAnyFunc {
 public:
  LVGLPropertyChartSeries(const AnyFuncColType arr[], int size)
      : LVGLPropertyAnyFunc(arr, size), m_list(QStringList() << "Empty list") {}
  QString name() const { return "Add Series"; }

  QStringList function(LVGLObject *obj) const {
    QStringList list;
    if (!m_sermap.isEmpty()) {
      // var define
      const QString str1 = "lv_chart_series_t *";
      const QString str2 = "series";
      for (int i = 1; i <= m_sermap.size(); ++i) {
        QString name = str2 + QString::number(i);
        QString tmp = str1 + name +
                      QString(" = lv_chart_add_series(%1, lv_color_hex(0x%2));")
                          .arg(obj->codeName())
                          .arg(m_colorresult[i].mid(1, 6));
        list << tmp;
        auto datalist = m_coderesult[i];
        for (int j = 0; j < datalist.size(); ++j) {
          if (datalist[j] != "")
            list << QString("%1->points[%2] = %3;")
                        .arg(name)
                        .arg(j)
                        .arg(datalist[j]);
        }
      }
      list << QString("lv_chart_refresh(%1);").arg(obj->codeName());
    }
    return list;
  }

 protected:
  QStringList get(LVGLObject *obj) const {
    if (m_list[0] != "Empty list") return m_list;
    return QStringList();
  }
  void set(LVGLObject *obj, QStringList list) {
    m_list = list;
    for (auto s : list) {
      QStringList strlist = s.split('@');
      int index = strlist[0].toInt();
      if (m_colorresult.contains(index) && m_colorresult[index] != strlist[1]) {
        lv_chart_remove_series(obj->obj(), m_sermap[index]);
      }

      if (!m_sermap.contains(index) || (m_colorresult.contains(index) &&
                                        m_colorresult[index] != strlist[1])) {
        m_colorresult[index] = strlist[1];
        m_sermap[index] = lv_chart_add_series(
            obj->obj(), lvgl->fromColor(QColor(strlist[1])));
      } else
        lv_chart_clear_serie(obj->obj(), m_sermap[index]);

      QStringList datalist = strlist[2].split(',');
      m_coderesult[index] = datalist;
      for (int i = 0; i < datalist.size(); ++i)
        if (datalist[i] != "") m_sermap[index]->points[i] = datalist[i].toInt();
    }
    lv_chart_refresh(obj->obj());
  }

 private:
  QStringList m_list;
  QMap<int, QString> m_colorresult;
  QMap<int, QStringList> m_coderesult;
  QMap<int, lv_chart_series_t *> m_sermap;
};

LVGLChart::LVGLChart() {
  initStateStyles();
  m_parts << LV_CHART_PART_BG << LV_CHART_PART_SERIES_BG << LV_CHART_PART_SERIES
          << LV_CHART_PART_CURSOR;
  //  m_properties << new LVGLPropertyChartType;
  m_properties << new LVGLPropertyChartRange;
  m_properties << new LVGLPropertyChartXDiv;
  m_properties << new LVGLPropertyChartYDiv;
  //  m_properties << new LVGLPropertyChartSeries;
  // m_properties << new LVGLPropertyChartMargin;
  // m_properties << new LVGLPropertySeries;

  static const AnyFuncColType arr[3] = {e_Seqlabel, e_ColorPick, e_QLineEdit};
  m_properties << new LVGLPropertyChartSeries(arr, 3);

  m_editableStyles << LVGL::ChartBG;        // LV_CHART_PART_BG
  m_editableStyles << LVGL::ChartSERIESBG;  // LV_CHART_PART_SERIES_BG
  m_editableStyles << LVGL::ChartSERIES;    // LV_CHART_PART_SERIES
  m_editableStyles << LVGL::ChartCURSOR;    // LV_CHART_PART_CURSOR
}

QString LVGLChart::name() const { return "Chart"; }

QString LVGLChart::className() const { return "lv_chart"; }

LVGLWidget::Type LVGLChart::type() const { return Chart; }

QIcon LVGLChart::icon() const { return QIcon(); }

lv_obj_t *LVGLChart::newObject(lv_obj_t *parent) const {
  lv_obj_t *obj = lv_chart_create(parent, nullptr);
  return obj;
}

QSize LVGLChart::minimumSize() const { return QSize(200, 150); }

QStringList LVGLChart::styles() const {
  return QStringList() << "LV_CHART_PART_BG"
                       << "LV_CHART_PART_SERIES_BG"
                       << "LV_CHART_PART_SERIES"
                       << "LV_CHART_PART_CURSOR";
}

lv_style_t *LVGLChart::style(lv_obj_t *obj, lv_obj_part_t part) const {
  return lv_obj_get_local_style(obj, part);
}

void LVGLChart::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const {
  lv_obj_add_style(obj, LV_BTN_PART_MAIN, style);
}

void LVGLChart::addStyle(lv_obj_t *obj, lv_style_t *style,
                         lv_obj_part_t part) const {
  lv_obj_add_style(obj, part, style);
}

void LVGLChart::initStateStyles() {
  for (int i = 0; i < 4; ++i) {
    lv_style_t *de = new lv_style_t;
    lv_style_t *ch = new lv_style_t;
    lv_style_t *fo = new lv_style_t;
    lv_style_t *ed = new lv_style_t;
    lv_style_t *ho = new lv_style_t;
    lv_style_t *pr = new lv_style_t;
    lv_style_t *di = new lv_style_t;
    lv_style_init(de);
    lv_style_init(ch);
    lv_style_init(fo);
    lv_style_init(ed);
    lv_style_init(ho);
    lv_style_init(pr);
    lv_style_init(di);
    QList<lv_style_t *> stateStyles;
    stateStyles << de << ch << fo << ed << ho << pr << di;
    m_partsStyles[i] = stateStyles;
  }
}
