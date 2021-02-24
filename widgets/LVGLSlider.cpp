#include "LVGLSlider.h"

#include <QIcon>

#include "LVGLObject.h"
#include "properties/LVGLPropertyRange.h"

class LVGLPropertySliderValue : public LVGLPropertyInt {
 public:
  LVGLPropertySliderValue() : LVGLPropertyInt(INT16_MIN, INT16_MAX) {}

  QString name() const { return "Value"; }

  QStringList function(LVGLObject *obj) const {
    return QStringList() << QString("lv_slider_set_value(%1, %2, LV_ANIM_OFF);")
                                .arg(obj->codeName())
                                .arg(get(obj));
  }

 protected:
  int get(LVGLObject *obj) const { return lv_slider_get_value(obj->obj()); }
  void set(LVGLObject *obj, int value) {
    lv_slider_set_value(obj->obj(), static_cast<int16_t>(value), LV_ANIM_OFF);
  }
};

class LVGLPropertySliderRange : public LVGLPropertyRange {
 public:
  QStringList function(LVGLObject *obj) const override {
    return QStringList() << QString("lv_slider_set_range(%1, %2, %3);")
                                .arg(obj->codeName())
                                .arg(getMin(obj))
                                .arg(getMax(obj));
  }

 protected:
  int getMin(LVGLObject *obj) const override {
    return lv_slider_get_min_value(obj->obj());
  }
  int getMax(LVGLObject *obj) const override {
    return lv_slider_get_max_value(obj->obj());
  }
  void set(LVGLObject *obj, int min, int max) override {
    lv_slider_set_range(obj->obj(), static_cast<int16_t>(min),
                        static_cast<int16_t>(max));
  }
};

LVGLSlider::LVGLSlider() {
  initStateStyles();
  m_parts << LV_SLIDER_PART_BG << LV_SLIDER_PART_INDIC << LV_SLIDER_PART_KNOB;

  m_properties << new LVGLPropertySliderValue;
  m_properties << new LVGLPropertySliderRange;

  m_editableStyles << LVGL::SliderBG;     // LV_SLIDER_PART_BG
  m_editableStyles << LVGL::SliderINDIC;  // LV_SLIDER_PART_INDIC
  m_editableStyles << LVGL::SliderKNOB;   // LV_SLIDER_PART_KNOB
}

QString LVGLSlider::name() const { return "Slider"; }

QString LVGLSlider::className() const { return "lv_slider"; }

LVGLWidget::Type LVGLSlider::type() const { return Slider; }

QIcon LVGLSlider::icon() const { return QIcon(); }

lv_obj_t *LVGLSlider::newObject(lv_obj_t *parent) const {
  lv_obj_t *obj = lv_slider_create(parent, nullptr);
  /*for (const LVGLProperty &p:m_properties)
          p.set(obj, p.defaultVal);*/
  return obj;
}

QSize LVGLSlider::minimumSize() const { return QSize(150, 25); }

QStringList LVGLSlider::styles() const {
  return QStringList() << "LV_SLIDER_PART_BG"
                       << "LV_SLIDER_PART_INDIC"
                       << "LV_SLIDER_PART_KNOB";
}

lv_style_t *LVGLSlider::style(lv_obj_t *obj, lv_obj_part_t part) const {
  return lv_obj_get_local_style(obj, part);
}

void LVGLSlider::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const {
  lv_obj_add_style(obj, LV_BTN_PART_MAIN, style);
}

void LVGLSlider::addStyle(lv_obj_t *obj, lv_style_t *style,
                          lv_obj_part_t part) const {
  lv_obj_add_style(obj, part, style);
}

void LVGLSlider::initStateStyles() {
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
