#include "LVGLSlider.h"

#include <QIcon>

#include "core/LVGLObject.h"
#include "properties/LVGLPropertyRange.h"

class LVGLPropertySliderType : public LVGLPropertyEnum {
 public:
  LVGLPropertySliderType()
      : LVGLPropertyEnum(QStringList()
                         << QObject::tr("Normal") << QObject::tr("SYMMETRICAL")
                         << QObject::tr("CUSTOM")),
        m_values({"LV_SLIDER_TYPE_NORMAL", "LV_SLIDER_TYPE_SYMMETRICAL",
                  "LV_SLIDER_TYPE_RANGE"}) {}

  QString name() const { return QObject::tr("Type"); }
  QString codename() const { return "Type"; }

  QStringList function(LVGLObject *obj) const {
    if (get(obj) == 0) return QStringList();
    return QStringList() << QString("lv_slider_set_type(%1, %2);")
                                .arg(obj->codeName())
                                .arg(m_values.at(get(obj)));
  }

 protected:
  int get(LVGLObject *obj) const { return lv_slider_get_type(obj->obj()); }
  void set(LVGLObject *obj, int index) {
    lv_slider_set_type(obj->obj(), index & 0xff);
  }

  QStringList m_values;
};

class LVGLPropertySliderAnimalState : public LVGLPropertyEnum {
 public:
  LVGLPropertySliderAnimalState()
      : LVGLPropertyEnum(QStringList()
                         << QObject::tr("On") << QObject::tr("Off")) {}

  QString name() const { return QObject::tr("Animation state"); }
  QString codename() const { return "Animation state"; }

  int getIndex() { return m_index; }

 protected:
  int get(LVGLObject *obj) const {
    (void)obj;
    return m_index;
  }
  void set(LVGLObject *obj, int index) {
    (void)obj;
    m_index = index;
  }

  int m_index = 0;
};

class LVGLPropertySliderValue : public LVGLPropertyInt {
 public:
  LVGLPropertySliderValue(LVGLPropertySliderAnimalState *lpsas)
      : LVGLPropertyInt(INT16_MIN, INT16_MAX), m_lpsas(lpsas) {}

  QString name() const { return QObject::tr("Value"); }
  QString codename() const { return "Value"; }

  QStringList function(LVGLObject *obj) const {
    if (0 == m_lpsas->getIndex())
      return QStringList() << QString(
                                  "lv_slider_set_value(%1, %2, LV_ANIM_ON);")
                                  .arg(obj->codeName())
                                  .arg(get(obj));
    else
      return QStringList() << QString(
                                  "lv_slider_set_value(%1, %2, LV_ANIM_OFF);")
                                  .arg(obj->codeName())
                                  .arg(get(obj));
  }

 protected:
  int get(LVGLObject *obj) const { return lv_slider_get_value(obj->obj()); }
  void set(LVGLObject *obj, int value) {
    if (0 == m_lpsas)
      lv_slider_set_value(obj->obj(), static_cast<int16_t>(value), LV_ANIM_ON);
    else
      lv_slider_set_value(obj->obj(), static_cast<int16_t>(value), LV_ANIM_OFF);
  }

 private:
  LVGLPropertySliderAnimalState *m_lpsas;
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

class LVGLPropertySliderAnimationTime : public LVGLPropertyInt {
 public:
  LVGLPropertySliderAnimationTime()
      : LVGLPropertyInt(0, UINT16_MAX, QObject::tr(" ms")), m_value(200) {}

  QString name() const { return QObject::tr("Animation time"); }
  QString codename() const { return "Animation time"; }

  QStringList function(LVGLObject *obj) const {
    return QStringList() << QString("lv_slider_set_anim_time(%1,%2);")
                                .arg(obj->codeName())
                                .arg(m_value);
  }

 protected:
  int get(LVGLObject *obj) const { return lv_slider_get_anim_time(obj->obj()); }
  void set(LVGLObject *obj, int value) {
    m_value = value;
    lv_slider_set_anim_time(obj->obj(), static_cast<uint16_t>(value));
  }
  int m_value;
};

class LVGLPropertySliderStyle : public LVGLPropertyEnum {
 public:
  LVGLPropertySliderStyle()
      : LVGLPropertyEnum({QObject::tr("Custome Style"),
                          QObject::tr("Default Style"), QObject::tr("Style1"),
                          QObject::tr("Style2"), QObject::tr("Style3"),
                          QObject::tr("Style4"), QObject::tr("Style5"),
                          QObject::tr("Style6"), QObject::tr("Style7"),
                          QObject::tr("Style8")}) {}

  QString name() const { return QObject::tr("Style"); }
  QString codename() const { return "Style"; }

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
      lv_obj_set_style_local_border_width(obj->obj(), LV_SLIDER_PART_INDIC, 0,
                                          0);
      lv_obj_set_style_local_border_color(obj->obj(), LV_SLIDER_PART_INDIC, 0,
                                          lv_color_hex(0x000000));
      lv_obj_set_style_local_outline_width(obj->obj(), 0, 0, 2);
      lv_obj_set_style_local_bg_color(obj->obj(), LV_SLIDER_PART_INDIC, 0,
                                      lv_color_hex(0x01a2b1));
      lv_obj_set_style_local_bg_grad_dir(obj->obj(), LV_SLIDER_PART_INDIC, 0,
                                         LV_GRAD_DIR_NONE);
      lv_obj_set_style_local_bg_grad_color(obj->obj(), LV_SLIDER_PART_INDIC, 0,
                                           lv_color_hex(0xffffff));
      lv_obj_set_style_local_bg_color(obj->obj(), 0, 0, lv_color_hex(0xd4d7d9));
      lv_obj_set_style_local_bg_color(obj->obj(), LV_SLIDER_PART_KNOB, 0,
                                      lv_color_hex(0x01a2b1));
      lv_obj_set_style_local_pad_top(obj->obj(), LV_SLIDER_PART_KNOB, 0, 6);
      lv_obj_set_style_local_pad_bottom(obj->obj(), LV_SLIDER_PART_KNOB, 0, 6);
      lv_obj_set_style_local_pad_left(obj->obj(), LV_SLIDER_PART_KNOB, 0, 6);
      lv_obj_set_style_local_pad_right(obj->obj(), LV_SLIDER_PART_KNOB, 0, 6);
      lv_obj_set_style_local_radius(obj->obj(), 0, 0, 32767);
      lv_obj_set_style_local_radius(obj->obj(), LV_SLIDER_PART_INDIC, 0, 32767);
      lv_obj_set_style_local_radius(obj->obj(), LV_SLIDER_PART_KNOB, 0, 32767);
      lv_obj_set_style_local_bg_opa(obj->obj(), LV_SLIDER_PART_KNOB, 0, 255);
    }
    switch (index) {
      case 1: {
        lv_obj_set_width(obj->obj(), 150);
        lv_obj_set_height(obj->obj(), 25);
      } break;
      case 2: {
        lv_obj_set_width(obj->obj(), 150);
        lv_obj_set_height(obj->obj(), 12);
        lv_obj_set_style_local_border_width(obj->obj(), LV_SLIDER_PART_INDIC, 0,
                                            1);
        lv_obj_set_style_local_outline_width(obj->obj(), 0, 0, 0);
        lv_obj_set_style_local_bg_color(obj->obj(), LV_SLIDER_PART_INDIC, 0,
                                        lv_color_hex(0x409cff));
        lv_obj_set_style_local_bg_grad_dir(obj->obj(), LV_SLIDER_PART_INDIC, 0,
                                           LV_GRAD_DIR_HOR);
        lv_obj_set_style_local_bg_grad_color(obj->obj(), LV_SLIDER_PART_INDIC,
                                             0, lv_color_hex(0x396afc));
        lv_obj_set_style_local_bg_color(obj->obj(), 0, 0,
                                        lv_color_hex(0xe8e8e8));
        lv_obj_set_style_local_bg_color(obj->obj(), LV_SLIDER_PART_KNOB, 0,
                                        lv_color_hex(0x585858));
      } break;
      case 3: {
        lv_obj_set_width(obj->obj(), 150);
        lv_obj_set_height(obj->obj(), 12);
        lv_obj_set_style_local_outline_width(obj->obj(), 0, 0, 0);
        lv_obj_set_style_local_bg_color(obj->obj(), LV_SLIDER_PART_INDIC, 0,
                                        lv_color_hex(0x409cff));
        lv_obj_set_style_local_bg_grad_dir(obj->obj(), LV_SLIDER_PART_INDIC, 0,
                                           LV_GRAD_DIR_HOR);
        lv_obj_set_style_local_bg_grad_color(obj->obj(), LV_SLIDER_PART_INDIC,
                                             0, lv_color_hex(0x396afc));
        lv_obj_set_style_local_bg_color(obj->obj(), 0, 0,
                                        lv_color_hex(0xe8e8e8));
        lv_obj_set_style_local_bg_color(obj->obj(), LV_SLIDER_PART_KNOB, 0,
                                        lv_color_hex(0x585858));
        lv_obj_set_style_local_radius(obj->obj(), LV_SLIDER_PART_KNOB, 0, 0);
      } break;
      case 4: {
        lv_obj_set_width(obj->obj(), 150);
        lv_obj_set_height(obj->obj(), 12);
        lv_obj_set_style_local_outline_width(obj->obj(), 0, 0, 0);
        lv_obj_set_style_local_border_width(obj->obj(), LV_SLIDER_PART_INDIC, 0,
                                            1);
        lv_obj_set_style_local_bg_color(obj->obj(), LV_SLIDER_PART_INDIC, 0,
                                        lv_color_hex(0x409cff));
        lv_obj_set_style_local_bg_grad_dir(obj->obj(), LV_SLIDER_PART_INDIC, 0,
                                           LV_GRAD_DIR_HOR);
        lv_obj_set_style_local_bg_grad_color(obj->obj(), LV_SLIDER_PART_INDIC,
                                             0, lv_color_hex(0x396afc));
        lv_obj_set_style_local_bg_color(obj->obj(), 0, 0,
                                        lv_color_hex(0xe8e8e8));
        lv_obj_set_style_local_bg_color(obj->obj(), LV_SLIDER_PART_KNOB, 0,
                                        lv_color_hex(0x585858));
        lv_obj_set_style_local_pad_top(obj->obj(), LV_SLIDER_PART_KNOB, 0, 6);
        lv_obj_set_style_local_pad_bottom(obj->obj(), LV_SLIDER_PART_KNOB, 0,
                                          6);
        lv_obj_set_style_local_pad_left(obj->obj(), LV_SLIDER_PART_KNOB, 0, 0);
        lv_obj_set_style_local_pad_right(obj->obj(), LV_SLIDER_PART_KNOB, 0, 0);
        lv_obj_set_style_local_radius(obj->obj(), LV_SLIDER_PART_KNOB, 0, 0);
      } break;

      case 5: {
        lv_obj_set_width(obj->obj(), 150);
        lv_obj_set_height(obj->obj(), 12);
        lv_obj_set_style_local_outline_width(obj->obj(), 0, 0, 0);
        lv_obj_set_style_local_bg_color(obj->obj(), LV_SLIDER_PART_INDIC, 0,
                                        lv_color_hex(0x409cff));
        lv_obj_set_style_local_bg_grad_dir(obj->obj(), LV_SLIDER_PART_INDIC, 0,
                                           LV_GRAD_DIR_HOR);
        lv_obj_set_style_local_bg_grad_color(obj->obj(), LV_SLIDER_PART_INDIC,
                                             0, lv_color_hex(0x396afc));
        lv_obj_set_style_local_bg_color(obj->obj(), 0, 0,
                                        lv_color_hex(0xe8e8e8));
        lv_obj_set_style_local_bg_color(obj->obj(), LV_SLIDER_PART_KNOB, 0,
                                        lv_color_hex(0x585858));
        lv_obj_set_style_local_pad_top(obj->obj(), LV_SLIDER_PART_KNOB, 0, 6);
        lv_obj_set_style_local_pad_bottom(obj->obj(), LV_SLIDER_PART_KNOB, 0,
                                          6);
        lv_obj_set_style_local_pad_left(obj->obj(), LV_SLIDER_PART_KNOB, 0, 0);
        lv_obj_set_style_local_pad_right(obj->obj(), LV_SLIDER_PART_KNOB, 0, 0);
        lv_obj_set_style_local_radius(obj->obj(), 0, 0, 0);
        lv_obj_set_style_local_radius(obj->obj(), LV_SLIDER_PART_INDIC, 0, 0);
        lv_obj_set_style_local_radius(obj->obj(), LV_SLIDER_PART_KNOB, 0, 0);
      } break;
      case 6: {
        lv_obj_set_width(obj->obj(), 150);
        lv_obj_set_height(obj->obj(), 30);
        lv_obj_set_style_local_outline_width(obj->obj(), 0, 0, 0);
        lv_obj_set_style_local_bg_color(obj->obj(), LV_SLIDER_PART_INDIC, 0,
                                        lv_color_hex(0x409cff));
        lv_obj_set_style_local_bg_grad_dir(obj->obj(), LV_SLIDER_PART_INDIC, 0,
                                           LV_GRAD_DIR_HOR);
        lv_obj_set_style_local_bg_grad_color(obj->obj(), LV_SLIDER_PART_INDIC,
                                             0, lv_color_hex(0x396afc));
        lv_obj_set_style_local_bg_color(obj->obj(), 0, 0,
                                        lv_color_hex(0xe8e8e8));
        lv_obj_set_style_local_radius(obj->obj(), 0, 0, 6);
        lv_obj_set_style_local_radius(obj->obj(), LV_SLIDER_PART_INDIC, 0, 6);
        lv_obj_set_style_local_bg_opa(obj->obj(), LV_SLIDER_PART_KNOB, 0, 0);
      } break;
      case 7: {
        lv_obj_set_width(obj->obj(), 150);
        lv_obj_set_height(obj->obj(), 30);
        lv_obj_set_style_local_outline_width(obj->obj(), 0, 0, 0);
        lv_obj_set_style_local_bg_color(obj->obj(), LV_SLIDER_PART_INDIC, 0,
                                        lv_color_hex(0x409cff));
        lv_obj_set_style_local_bg_grad_dir(obj->obj(), LV_SLIDER_PART_INDIC, 0,
                                           LV_GRAD_DIR_HOR);
        lv_obj_set_style_local_bg_grad_color(obj->obj(), LV_SLIDER_PART_INDIC,
                                             0, lv_color_hex(0x396afc));
        lv_obj_set_style_local_bg_color(obj->obj(), 0, 0,
                                        lv_color_hex(0xe8e8e8));
        lv_obj_set_style_local_bg_opa(obj->obj(), LV_SLIDER_PART_KNOB, 0, 0);
      } break;
      case 8: {
        lv_obj_set_width(obj->obj(), 150);
        lv_obj_set_height(obj->obj(), 30);
        lv_obj_set_style_local_outline_width(obj->obj(), 0, 0, 0);
        lv_obj_set_style_local_bg_color(obj->obj(), LV_SLIDER_PART_INDIC, 0,
                                        lv_color_hex(0x409cff));
        lv_obj_set_style_local_bg_grad_dir(obj->obj(), LV_SLIDER_PART_INDIC, 0,
                                           LV_GRAD_DIR_HOR);
        lv_obj_set_style_local_bg_grad_color(obj->obj(), LV_SLIDER_PART_INDIC,
                                             0, lv_color_hex(0x396afc));
        lv_obj_set_style_local_bg_color(obj->obj(), 0, 0,
                                        lv_color_hex(0xe8e8e8));
        lv_obj_set_style_local_bg_color(obj->obj(), LV_SLIDER_PART_KNOB, 0,
                                        lv_color_hex(0x585858));
        lv_obj_set_style_local_radius(obj->obj(), 0, 0, 6);
        lv_obj_set_style_local_radius(obj->obj(), LV_SLIDER_PART_INDIC, 0, 6);
        lv_obj_set_style_local_pad_top(obj->obj(), LV_SLIDER_PART_KNOB, 0, 0);
        lv_obj_set_style_local_pad_bottom(obj->obj(), LV_SLIDER_PART_KNOB, 0,
                                          0);
        lv_obj_set_style_local_pad_left(obj->obj(), LV_SLIDER_PART_KNOB, 0, -5);
        lv_obj_set_style_local_pad_right(obj->obj(), LV_SLIDER_PART_KNOB, 0,
                                         -5);
      } break;
      case 9: {
        lv_obj_set_width(obj->obj(), 150);
        lv_obj_set_height(obj->obj(), 30);
        lv_obj_set_style_local_outline_width(obj->obj(), 0, 0, 0);
        lv_obj_set_style_local_bg_color(obj->obj(), LV_SLIDER_PART_INDIC, 0,
                                        lv_color_hex(0x409cff));
        lv_obj_set_style_local_bg_grad_dir(obj->obj(), LV_SLIDER_PART_INDIC, 0,
                                           LV_GRAD_DIR_HOR);
        lv_obj_set_style_local_bg_grad_color(obj->obj(), LV_SLIDER_PART_INDIC,
                                             0, lv_color_hex(0x396afc));
        lv_obj_set_style_local_bg_color(obj->obj(), 0, 0,
                                        lv_color_hex(0xe8e8e8));
        lv_obj_set_style_local_bg_color(obj->obj(), LV_SLIDER_PART_KNOB, 0,
                                        lv_color_hex(0x585858));
        lv_obj_set_style_local_pad_top(obj->obj(), LV_SLIDER_PART_KNOB, 0, 0);
        lv_obj_set_style_local_pad_bottom(obj->obj(), LV_SLIDER_PART_KNOB, 0,
                                          0);
        lv_obj_set_style_local_pad_left(obj->obj(), LV_SLIDER_PART_KNOB, 0, 2);
        lv_obj_set_style_local_pad_right(obj->obj(), LV_SLIDER_PART_KNOB, 0,
                                         -2);
      } break;
    }
  }
};

LVGLSlider::LVGLSlider() {
  initStateStyles();
  m_parts << LV_SLIDER_PART_BG << LV_SLIDER_PART_INDIC << LV_SLIDER_PART_KNOB;

  m_properties << new LVGLPropertySliderType;
  m_properties << new LVGLPropertySliderRange;
  auto lpsas = new LVGLPropertySliderAnimalState;
  m_properties << lpsas;
  m_properties << new LVGLPropertySliderAnimationTime;
  m_properties << new LVGLPropertySliderValue(lpsas);
  m_properties << new LVGLPropertySliderStyle;

  m_editableStyles << LVGL::SliderBG;     // LV_SLIDER_PART_BG
  m_editableStyles << LVGL::SliderINDIC;  // LV_SLIDER_PART_INDIC
  m_editableStyles << LVGL::SliderKNOB;   // LV_SLIDER_PART_KNOB
}

QString LVGLSlider::name() const { return QObject::tr("Slider"); }

QString LVGLSlider::codename() const { return "Slider"; }

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
