#include "LVGLLineMeter.h"

#include <QIcon>

#include "core/LVGLObject.h"
#include "properties/LVGLPropertyRange.h"
#include "properties/LVGLPropertyVal2.h"

class LVGLPropertyLMeterRange : public LVGLPropertyRange {
 public:
  QStringList function(LVGLObject *obj) const {
    return {QString("lv_lmeter_set_range(%1, %2, %2);")
                .arg(obj->codeName())
                .arg(getMin(obj))
                .arg(getMax(obj))};
  }

 protected:
  int getMin(LVGLObject *obj) const {
    return lv_linemeter_get_min_value(obj->obj());
  }
  int getMax(LVGLObject *obj) const {
    return lv_linemeter_get_max_value(obj->obj());
  }
  void set(LVGLObject *obj, int min, int max) {
    lv_linemeter_set_range(obj->obj(), static_cast<int16_t>(min),
                           static_cast<int16_t>(max));
  }
};

LVGLLineMeter::LVGLLineMeter() {
  initStateStyles();
  m_parts << LV_LINEMETER_PART_MAIN;
  m_properties << new LVGLPropertyLMeterRange;
  m_properties << new LVGLPropertyValInt16(
      INT16_MIN, INT16_MAX, "Value", QObject::tr("Value"),
      "lv_linemeter_set_value", lv_linemeter_set_value, lv_linemeter_get_value);
  m_properties << new LVGLPropertyValUInt16(
      0, 360, "Angle", QObject::tr("Angle"), "lv_linemeter_set_angle_offset",
      lv_linemeter_set_angle_offset, lv_linemeter_get_angle_offset);
  m_properties << new LVGLPropertyVal2UInt16(
      0, 360, QObject::tr("Angle"), lv_linemeter_get_scale_angle, 0, UINT16_MAX,
      QObject::tr("Lines"), lv_linemeter_get_line_count,
      "lv_linemeter_set_scale", lv_linemeter_set_scale, "Scale",
      QObject::tr("Scale"));

  m_editableStyles << LVGL::LinemeterMAIN;  // LV_LINEMETER_PART_MAIN
}

QString LVGLLineMeter::name() const { return QObject::tr("Line meter"); }

QString LVGLLineMeter::className() const { return "lv_lmeter"; }

LVGLWidget::Type LVGLLineMeter::type() const { return LineMeter; }

QIcon LVGLLineMeter::icon() const { return QIcon(); }

lv_obj_t *LVGLLineMeter::newObject(lv_obj_t *parent) const {
  lv_obj_t *obj = lv_linemeter_create(parent, nullptr);
  return obj;
}

QSize LVGLLineMeter::minimumSize() const { return QSize(100, 100); }

QStringList LVGLLineMeter::styles() const {
  return QStringList() << "LINEMETER_PART_MAIN";
}

lv_style_t *LVGLLineMeter::style(lv_obj_t *obj, lv_obj_part_t part) const {
  return lv_obj_get_local_style(obj, part);
}

void LVGLLineMeter::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const {
  lv_obj_add_style(obj, LV_BTN_PART_MAIN, style);
}

void LVGLLineMeter::addStyle(lv_obj_t *obj, lv_style_t *style,
                             lv_obj_part_t part) const {
  lv_obj_add_style(obj, part, style);
}

void LVGLLineMeter::initStateStyles() {
  for (int i = 0; i < 1; ++i) {
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
