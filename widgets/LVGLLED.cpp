#include "LVGLLED.h"

#include <QIcon>

#include "core/LVGLObject.h"

#define LV_LED_WIDTH_DEF (LV_DPI / 3)
#define LV_LED_HEIGHT_DEF (LV_DPI / 3)

class LVGLPropertyLEDBrightness : public LVGLPropertyInt {
 public:
  LVGLPropertyLEDBrightness() : LVGLPropertyInt(0, 255) {}

  QString name() const { return "Brightness"; }

  QStringList function(LVGLObject *obj) const {
    return QStringList() << QString("lv_led_set_bright(%1, %2);")
                                .arg(obj->codeName())
                                .arg(get(obj));
  }

 protected:
  int get(LVGLObject *obj) const { return lv_led_get_bright(obj->obj()); }
  void set(LVGLObject *obj, int value) {
    lv_led_set_bright(obj->obj(), static_cast<uint8_t>(value));
  }
};

class LVGLPropertySwitch : public LVGLPropertyEnum {
 public:
  LVGLPropertySwitch()
      : LVGLPropertyEnum(QStringList() << "On"
                                       << "Off"),
        m_index(0) {}

  QString name() const { return "Switch"; }

  QStringList function(LVGLObject *obj) const {
    if (m_index)
      return QStringList() << QString("lv_led_off(%1);").arg(obj->codeName());
    else
      return QStringList() << QString("lv_led_on(%1);").arg(obj->codeName());
  }

 protected:
  int get(LVGLObject *obj) const {
    Q_UNUSED(obj)
    return m_index;
  }
  void set(LVGLObject *obj, int index) {
    m_index = index;
    if (index)
      lv_led_off(obj->obj());
    else
      lv_led_on(obj->obj());
  }
  int m_index;
};
LVGLLED::LVGLLED() {
  initStateStyles();
  m_parts << LV_LED_PART_MAIN;
  m_properties << new LVGLPropertyLEDBrightness;
  m_properties << new LVGLPropertySwitch;

  m_editableStyles << LVGL::Background;  // LV_LED_PART_MAIN
}

QString LVGLLED::name() const { return "LED"; }

QString LVGLLED::className() const { return "lv_led"; }

LVGLWidget::Type LVGLLED::type() const { return LED; }

QIcon LVGLLED::icon() const { return QIcon(); }

lv_obj_t *LVGLLED::newObject(lv_obj_t *parent) const {
  lv_obj_t *obj = lv_led_create(parent, nullptr);
  return obj;
}

QSize LVGLLED::minimumSize() const {
  return QSize(LV_LED_WIDTH_DEF, LV_LED_HEIGHT_DEF);
}

QStringList LVGLLED::styles() const { return QStringList() << "LED_PART_MAIN"; }

lv_style_t *LVGLLED::style(lv_obj_t *obj, lv_obj_part_t part) const {
  return lv_obj_get_local_style(obj, part);
}

void LVGLLED::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const {
  lv_obj_add_style(obj, LV_BTN_PART_MAIN, style);
}

void LVGLLED::addStyle(lv_obj_t *obj, lv_style_t *style,
                       lv_obj_part_t part) const {
  lv_obj_add_style(obj, part, style);
}

void LVGLLED::initStateStyles() {
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
