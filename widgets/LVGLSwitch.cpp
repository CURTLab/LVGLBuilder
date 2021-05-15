#include "LVGLSwitch.h"

#include <QIcon>

#include "LVGLObject.h"

class LVGLPropertyState : public LVGLPropertyBool {
 public:
  QString name() const { return "State"; }

 protected:
  bool get(LVGLObject *obj) const { return lv_switch_get_state(obj->obj()); }
  void set(LVGLObject *obj, bool state) {
    if (state)
      lv_switch_on(obj->obj(), LV_ANIM_OFF);
    else
      lv_switch_off(obj->obj(), LV_ANIM_OFF);
  }
};

class LVGLPropertySwitchStyle : public LVGLPropertyEnum {
 public:
  LVGLPropertySwitchStyle()
      : LVGLPropertyEnum({"Custome Style", "Default Style", "Style1", "Style2",
                          "Style3", "Style4", "Style5", "Style6", "Style7"}) {}

  QString name() const { return "Style"; }

  QStringList function(LVGLObject *obj) const {
    Q_UNUSED(obj)
    return QStringList();
  }

 protected:
  int get(LVGLObject *obj) const {
    Q_UNUSED(obj)
    return 0;
  }
  void set(LVGLObject *obj, int index) {
    if (index != 0) {
      lv_obj_set_style_local_pad_top(obj->obj(), LV_SWITCH_PART_KNOB, 0, -3);
      lv_obj_set_style_local_pad_bottom(obj->obj(), LV_SWITCH_PART_KNOB, 0, -3);
      lv_obj_set_style_local_pad_left(obj->obj(), LV_SWITCH_PART_KNOB, 0, -3);
      lv_obj_set_style_local_pad_right(obj->obj(), LV_SWITCH_PART_KNOB, 0, -3);
      lv_obj_set_style_local_radius(obj->obj(), 0, 0, 32767);
      lv_obj_set_style_local_radius(obj->obj(), LV_SWITCH_PART_INDIC, 0, 32767);
      lv_obj_set_style_local_radius(obj->obj(), LV_SWITCH_PART_KNOB, 0, 32767);
      lv_obj_set_style_local_bg_opa(obj->obj(), 0, 0, 255);
      lv_obj_set_style_local_border_opa(obj->obj(), 0, 0, 0);
      lv_obj_set_style_local_border_width(obj->obj(), 0, 0, 0);
      lv_obj_set_style_local_border_color(obj->obj(), 0, 0,
                                          lv_color_hex(0x000000));
      lv_obj_set_style_local_bg_color(obj->obj(), 0, 0, lv_color_hex(0xd4d7d9));
      lv_obj_set_style_local_bg_color(obj->obj(), LV_SWITCH_PART_KNOB, 0,
                                      lv_color_hex(0xffffff));
      lv_obj_set_style_local_bg_color(obj->obj(), LV_SWITCH_PART_INDIC, 0,
                                      lv_color_hex(0x01a2b1));
    }
    switch (index) {
      case 1: {
        lv_obj_set_style_local_outline_width(obj->obj(), 0, 0, 2);
        lv_obj_set_size(obj->obj(), 70, 35);

      } break;
      case 2: {
        lv_obj_set_style_local_bg_color(obj->obj(), 0, 0,
                                        lv_color_hex(0xeceff6));
        lv_obj_set_style_local_bg_color(obj->obj(), LV_SWITCH_PART_INDIC, 0,
                                        lv_color_hex(0x1677ff));
        lv_obj_set_style_local_outline_width(obj->obj(), 0, 0, 0);
        lv_obj_set_size(obj->obj(), 70, 35);
      } break;
      case 3: {
        lv_obj_set_style_local_bg_color(obj->obj(), 0, 0,
                                        lv_color_hex(0xeceff6));
        lv_obj_set_style_local_bg_color(obj->obj(), LV_SWITCH_PART_INDIC, 0,
                                        lv_color_hex(0x1677ff));
        lv_obj_set_style_local_outline_width(obj->obj(), 0, 0, 0);
        lv_obj_set_style_local_radius(obj->obj(), 0, 0, 8);
        lv_obj_set_style_local_radius(obj->obj(), LV_SWITCH_PART_INDIC, 0, 8);
        lv_obj_set_style_local_radius(obj->obj(), LV_SWITCH_PART_KNOB, 0, 8);
        lv_obj_set_size(obj->obj(), 70, 35);
      }
      case 4: {
        lv_obj_set_style_local_bg_color(obj->obj(), 0, 0,
                                        lv_color_hex(0xeceff6));
        lv_obj_set_style_local_bg_color(obj->obj(), LV_SWITCH_PART_INDIC, 0,
                                        lv_color_hex(0x1677ff));
        lv_obj_set_style_local_outline_width(obj->obj(), 0, 0, 0);
        lv_obj_set_style_local_radius(obj->obj(), 0, 0, 8);
        lv_obj_set_style_local_radius(obj->obj(), LV_SWITCH_PART_INDIC, 0, 8);
        lv_obj_set_size(obj->obj(), 70, 35);
      } break;
      case 5: {
        lv_obj_set_style_local_bg_color(obj->obj(), 0, 0,
                                        lv_color_hex(0xeceff6));
        lv_obj_set_style_local_bg_color(obj->obj(), LV_SWITCH_PART_INDIC, 0,
                                        lv_color_hex(0x1677ff));
        lv_obj_set_style_local_outline_width(obj->obj(), 0, 0, 0);
        lv_obj_set_style_local_pad_top(obj->obj(), LV_SWITCH_PART_KNOB, 0, 8);
        lv_obj_set_style_local_pad_bottom(obj->obj(), LV_SWITCH_PART_KNOB, 0,
                                          8);
        lv_obj_set_style_local_pad_left(obj->obj(), LV_SWITCH_PART_KNOB, 0, 8);
        lv_obj_set_style_local_pad_right(obj->obj(), LV_SWITCH_PART_KNOB, 0, 8);
        lv_obj_set_size(obj->obj(), 44, 10);
        lv_obj_set_style_local_bg_color(obj->obj(), LV_SWITCH_PART_KNOB, 0,
                                        lv_color_hex(0xe96565));
      } break;
      case 6: {
        lv_obj_set_style_local_bg_color(obj->obj(), 0, 0,
                                        lv_color_hex(0xeceff6));
        lv_obj_set_style_local_bg_color(obj->obj(), LV_SWITCH_PART_INDIC, 0,
                                        lv_color_hex(0x1677ff));
        lv_obj_set_style_local_outline_width(obj->obj(), 0, 0, 0);
        lv_obj_set_style_local_radius(obj->obj(), 0, 0, 4);
        lv_obj_set_style_local_radius(obj->obj(), LV_SWITCH_PART_INDIC, 0, 4);
        lv_obj_set_style_local_radius(obj->obj(), LV_SWITCH_PART_KNOB, 0, 4);
        lv_obj_set_style_local_pad_top(obj->obj(), LV_SWITCH_PART_KNOB, 0, 8);
        lv_obj_set_style_local_pad_bottom(obj->obj(), LV_SWITCH_PART_KNOB, 0,
                                          8);
        lv_obj_set_style_local_pad_left(obj->obj(), LV_SWITCH_PART_KNOB, 0, 8);
        lv_obj_set_style_local_pad_right(obj->obj(), LV_SWITCH_PART_KNOB, 0, 8);
        lv_obj_set_size(obj->obj(), 44, 10);
        lv_obj_set_style_local_bg_color(obj->obj(), LV_SWITCH_PART_KNOB, 0,
                                        lv_color_hex(0xe96565));
      } break;
      case 7: {
        lv_obj_set_style_local_bg_color(obj->obj(), 0, 0,
                                        lv_color_hex(0xeceff6));
        lv_obj_set_style_local_bg_color(obj->obj(), LV_SWITCH_PART_INDIC, 0,
                                        lv_color_hex(0x1677ff));
        lv_obj_set_style_local_outline_width(obj->obj(), 0, 0, 0);
        lv_obj_set_style_local_bg_opa(obj->obj(), 0, 0, 0);
        lv_obj_set_style_local_border_opa(obj->obj(), 0, 0, 255);
        lv_obj_set_style_local_border_width(obj->obj(), 0, 0, 2);
        lv_obj_set_style_local_border_color(obj->obj(), 0, 0,
                                            lv_color_hex(0x585858));
        lv_obj_set_style_local_bg_color(obj->obj(), LV_SWITCH_PART_KNOB, 0,
                                        lv_color_hex(0xe96565));
        lv_obj_set_size(obj->obj(), 70, 35);
      } break;
      case 8: {
        lv_obj_set_style_local_bg_color(obj->obj(), 0, 0,
                                        lv_color_hex(0xeceff6));
        lv_obj_set_style_local_bg_color(obj->obj(), LV_SWITCH_PART_INDIC, 0,
                                        lv_color_hex(0x1677ff));
        lv_obj_set_style_local_outline_width(obj->obj(), 0, 0, 0);
        lv_obj_set_style_local_pad_top(obj->obj(), LV_SWITCH_PART_KNOB, 0, -2);
        lv_obj_set_style_local_pad_bottom(obj->obj(), LV_SWITCH_PART_KNOB, 0,
                                          -2);
        lv_obj_set_style_local_pad_left(obj->obj(), LV_SWITCH_PART_KNOB, 0, -6);
        lv_obj_set_style_local_pad_right(obj->obj(), LV_SWITCH_PART_KNOB, 0, 8);
        lv_obj_set_style_local_bg_color(obj->obj(), 0, 0,
                                        lv_color_hex(0xefefef));
        lv_obj_set_style_local_bg_color(obj->obj(), LV_SWITCH_PART_KNOB, 0,
                                        lv_color_hex(0xe96565));
      } break;
    }
  }
};

LVGLSwitch::LVGLSwitch() {
  initStateStyles();
  m_parts << LV_SWITCH_PART_BG << LV_SWITCH_PART_INDIC << LV_SWITCH_PART_KNOB;

  m_properties << new LVGLPropertyState;
  m_properties << new LVGLPropertySwitchStyle;

  m_editableStyles << LVGL::SliderBG;     // LV_SWITCH_PART_BG
  m_editableStyles << LVGL::SliderINDIC;  // LV_SWITCH_PART_INDIC
  m_editableStyles << LVGL::SliderKNOB;   // LV_SWITCH_PART_KNOB
}

QString LVGLSwitch::name() const { return "Switch"; }

QString LVGLSwitch::className() const { return "lv_switch"; }

LVGLWidget::Type LVGLSwitch::type() const { return Switch; }

QIcon LVGLSwitch::icon() const { return QIcon(); }

lv_obj_t *LVGLSwitch::newObject(lv_obj_t *parent) const {
  lv_obj_t *obj = lv_switch_create(parent, nullptr);
  return obj;
}

QSize LVGLSwitch::minimumSize() const { return QSize(70, 35); }

QStringList LVGLSwitch::styles() const {
  return QStringList() << "SWITCH_PART_BG"
                       << "SWITCH_PART_INDIC"
                       << "SWITCH_PART_KNOB";
}

lv_style_t *LVGLSwitch::style(lv_obj_t *obj, lv_obj_part_t part) const {
  return lv_obj_get_local_style(obj, part);
}

void LVGLSwitch::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const {
  lv_obj_add_style(obj, LV_BTN_PART_MAIN, style);
}

void LVGLSwitch::addStyle(lv_obj_t *obj, lv_style_t *style,
                          lv_obj_part_t part) const {
  lv_obj_add_style(obj, part, style);
}

void LVGLSwitch::initStateStyles() {
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
