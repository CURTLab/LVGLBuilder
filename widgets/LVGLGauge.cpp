#include "LVGLGauge.h"

#include <QIcon>

#include "core/LVGLObject.h"
#include "properties/LVGLPropertyScale.h"
#include "properties/LVGLPropertyVal2.h"

class LVGLPropertyGaugeCriticalValue : public LVGLPropertyInt {
 public:
  inline LVGLPropertyGaugeCriticalValue()
      : LVGLPropertyInt(INT16_MIN, INT16_MAX) {}

  inline QString name() const override { return QObject::tr("Critical value"); }
  inline QString codename() const override { return "Critical value"; }

  inline QStringList function(LVGLObject *obj) const override {
    return {QString("lv_gauge_set_critical_value(%1, %2);")
                .arg(obj->codeName())
                .arg(get(obj))};
  }

 protected:
  inline int get(LVGLObject *obj) const override {
    return lv_gauge_get_critical_value(obj->obj());
  }
  inline void set(LVGLObject *obj, int value) override {
    lv_gauge_set_critical_value(obj->obj(), static_cast<int16_t>(value));
  }
};

class LVGLPropertyGaugeVal : public LVGLPropertyInt {
 public:
  inline LVGLPropertyGaugeVal() : LVGLPropertyInt(INT16_MIN, INT16_MAX) {}

  inline QString name() const override { return QObject::tr("Value"); }
  inline QString codename() const override { return "Value"; }

  inline QStringList function(LVGLObject *obj) const override {
    return {QString("lv_gauge_set_value(%1, 0, %2);")
                .arg(obj->codeName())
                .arg(get(obj))};
  }

 protected:
  inline int get(LVGLObject *obj) const override {
    return lv_gauge_get_value(obj->obj(), 0);
  }
  inline void set(LVGLObject *obj, int value) override {
    lv_gauge_set_value(obj->obj(), 0, static_cast<int16_t>(value));
  }
};

LVGLGauge::LVGLGauge() {
  initStateStyles();
  m_parts << LV_GAUGE_PART_MAIN << LV_GAUGE_PART_MAJOR << LV_GAUGE_PART_NEEDLE;
  m_properties << new LVGLPropertyScale;
  m_properties << new LVGLPropertyValInt16(
      INT16_MIN, INT16_MAX, "Critical value", QObject::tr("Critical value"),
      "lv_gauge_set_critical_value", lv_gauge_set_critical_value,
      lv_gauge_get_critical_value);
  m_properties << new LVGLPropertyVal2Int16(
      INT16_MIN, INT16_MAX, "Min", lv_gauge_get_min_value, INT16_MIN, INT16_MAX,
      "Max", lv_gauge_get_max_value, "lv_gauge_set_range", lv_gauge_set_range,
      "Range", QObject::tr("Range"));
  m_properties << new LVGLPropertyGaugeVal;

  m_editableStyles << LVGL::GaugeMAIN;    // GAUGE_PART_MAIN
  m_editableStyles << LVGL::GaugeMAJOR;   // GAUGE_PART_MAJOR
  m_editableStyles << LVGL::GaugeNEEDLE;  // GAUGE_PART_NEEDLE
}

QString LVGLGauge::name() const { return QObject::tr("Gauge"); }

QString LVGLGauge::codename() const { return "Gauge"; }

QString LVGLGauge::className() const { return "lv_gauge"; }

LVGLWidget::Type LVGLGauge::type() const { return Gauge; }

QIcon LVGLGauge::icon() const { return QIcon(); }

lv_obj_t *LVGLGauge::newObject(lv_obj_t *parent) const {
  lv_obj_t *obj = lv_gauge_create(parent, nullptr);
  return obj;
}

QSize LVGLGauge::minimumSize() const { return QSize(100, 100); }

QStringList LVGLGauge::styles() const {
  return QStringList() << "GAUGE_PART_MAIN"
                       << "GAUGE_PART_MAJOR"
                       << "GAUGE_PART_NEEDLE";
}

lv_style_t *LVGLGauge::style(lv_obj_t *obj, lv_obj_part_t part) const {
  return lv_obj_get_local_style(obj, part);
}

void LVGLGauge::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const {
  lv_obj_add_style(obj, LV_BTN_PART_MAIN, style);
}

void LVGLGauge::addStyle(lv_obj_t *obj, lv_style_t *style,
                         lv_obj_part_t part) const {
  lv_obj_add_style(obj, part, style);
}

void LVGLGauge::initStateStyles() {
  for (int i = 0; i < 3; ++i) {
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
