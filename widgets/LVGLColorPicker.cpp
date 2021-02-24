#include "LVGLColorPicker.h"

#include <QIcon>

#include "LVGLObject.h"
#include "properties/LVGLPropertyColor.h"

class LVGLPropertyCPickerType : public LVGLPropertyEnum {
 public:
  LVGLPropertyCPickerType()
      : LVGLPropertyEnum(QStringList() << "Rectange"
                                       << "Disc") {}

  QString name() const { return "Type"; }

 protected:
  int get(LVGLObject *obj) const {
    lv_cpicker_ext_t *ext =
        reinterpret_cast<lv_cpicker_ext_t *>(lv_obj_get_ext_attr(obj->obj()));
    return ext->type;
  }
  void set(LVGLObject *obj, int index) {
    lv_cpicker_set_type(obj->obj(), index & 0xff);
  }
};

class LVGLPropertyCPickerHue : public LVGLPropertyInt {
 public:
  LVGLPropertyCPickerHue() : LVGLPropertyInt(0, 360) {}

  QString name() const { return "Hue"; }

 protected:
  int get(LVGLObject *obj) const { return lv_cpicker_get_hue(obj->obj()); }
  void set(LVGLObject *obj, int value) {
    lv_cpicker_set_hue(obj->obj(), static_cast<uint16_t>(value));
  }
};

class LVGLPropertyCPickerSaturation : public LVGLPropertyInt {
 public:
  LVGLPropertyCPickerSaturation() : LVGLPropertyInt(0, 100) {}

  QString name() const { return "Saturation"; }

 protected:
  int get(LVGLObject *obj) const {
    return lv_cpicker_get_saturation(obj->obj());
  }
  void set(LVGLObject *obj, int value) {
    lv_cpicker_set_saturation(obj->obj(), static_cast<uint8_t>(value));
  }
};

class LVGLPropertyCPickerValue : public LVGLPropertyInt {
 public:
  LVGLPropertyCPickerValue() : LVGLPropertyInt(0, 100) {}

  QString name() const { return "Value"; }

 protected:
  int get(LVGLObject *obj) const { return lv_cpicker_get_value(obj->obj()); }
  void set(LVGLObject *obj, int value) {
    lv_cpicker_set_value(obj->obj(), static_cast<uint8_t>(value));
  }
};

class LVGLPropertyCPickerMode : public LVGLPropertyEnum {
 public:
  LVGLPropertyCPickerMode()
      : LVGLPropertyEnum(QStringList() << "Hue"
                                       << "Saturation"
                                       << "Value") {}

  QString name() const { return "Color mode"; }

 protected:
  int get(LVGLObject *obj) const {
    return lv_cpicker_get_color_mode(obj->obj());
  }
  void set(LVGLObject *obj, int index) {
    lv_cpicker_set_color_mode(obj->obj(), index & 0xff);
  }
};

class LVGLPropertyCPickerColor : public LVGLPropertyColor {
 public:
  QString name() const { return "Color"; }

 protected:
  lv_color_t get(LVGLObject *obj) const {
    return lv_cpicker_get_color(obj->obj());
  }
  void set(LVGLObject *obj, lv_color_t boolean) {
    lv_cpicker_set_color(obj->obj(), boolean);
  }
};

LVGLColorPicker::LVGLColorPicker() {
  initStateStyles();
  m_parts << LV_CPICKER_PART_MAIN << LV_CPICKER_PART_KNOB;
  m_properties << new LVGLPropertyCPickerType;
  m_properties << new LVGLPropertyCPickerHue;
  m_properties << new LVGLPropertyCPickerSaturation;
  m_properties << new LVGLPropertyCPickerValue;
  m_properties << new LVGLPropertyCPickerMode;
  m_properties << new LVGLPropertyBool(
      "Knob Colored", "lv_cpicker_set_knob_colored",
      lv_cpicker_set_knob_colored, lv_cpicker_get_knob_colored);
  m_properties << new LVGLPropertyBool("Value", "lv_cpicker_set_value",
                                       lv_cpicker_set_value,
                                       lv_cpicker_get_value);
  m_properties << new LVGLPropertyCPickerColor;

  m_editableStyles << LVGL::CPickerBG;    // LV_CPICKER_PART_MAIN
  m_editableStyles << LVGL::CpickerKNOB;  // LV_CPICKER_PART_KNOB
}

QString LVGLColorPicker::name() const { return "Color picker"; }

QString LVGLColorPicker::className() const { return "lv_cpicker"; }

LVGLWidget::Type LVGLColorPicker::type() const { return ColorPicker; }

QIcon LVGLColorPicker::icon() const { return QIcon(); }

QSize LVGLColorPicker::minimumSize() const { return QSize(100, 100); }

lv_obj_t *LVGLColorPicker::newObject(lv_obj_t *parent) const {
  lv_obj_t *obj = lv_cpicker_create(parent, nullptr);
  // for (const LVGLProperty &p:m_properties)
  //	p.set(obj, p.defaultVal);
  return obj;
}

QStringList LVGLColorPicker::styles() const {
  return QStringList() << "CPICKER_PART_MAIN"
                       << "CPICKER_PART_KNOB";
}

lv_style_t *LVGLColorPicker::style(lv_obj_t *obj, lv_obj_part_t part) const {
  return lv_obj_get_local_style(obj, part);
}

void LVGLColorPicker::setStyle(lv_obj_t *obj, int type,
                               lv_style_t *style) const {
  lv_obj_add_style(obj, LV_BTN_PART_MAIN, style);
}

void LVGLColorPicker::addStyle(lv_obj_t *obj, lv_style_t *style,
                               lv_obj_part_t part) const {
  lv_obj_add_style(obj, part, style);
}

void LVGLColorPicker::initStateStyles() {
  for (int i = 0; i < 2; ++i) {
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
