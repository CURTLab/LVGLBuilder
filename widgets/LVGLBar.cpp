#include "LVGLBar.h"

#include <QIcon>

#include "core/LVGLCore.h"
#include "core/LVGLObject.h"
#include "properties/LVGLPropertyAnyFunc.h"
#include "properties/LVGLPropertyRange.h"

class LVGLPropertyBarRange : public LVGLPropertyRange {
 public:
  LVGLPropertyBarRange() : m_min(0), m_max(100) {}
  QStringList function(LVGLObject *obj) const {
    return {QString("lv_bar_set_range(%1, %2, %3);")
                .arg(obj->codeName())
                .arg(m_min)
                .arg(m_max)};
  }
  int getmin() { return m_min; }
  int getmax() { return m_max; }

 protected:
  int getMin(LVGLObject *obj) const { return lv_bar_get_min_value(obj->obj()); }
  int getMax(LVGLObject *obj) const { return lv_bar_get_max_value(obj->obj()); }
  void set(LVGLObject *obj, int min, int max) {
    m_min = min;
    m_max = max;
    lv_bar_set_range(obj->obj(), static_cast<int16_t>(min),
                     static_cast<int16_t>(max));
  }
  int m_min;
  int m_max;
};

class LVGLPropertyBarType : public LVGLPropertyEnum {
 public:
  LVGLPropertyBarType()
      : LVGLPropertyEnum(QStringList()
                         << QObject::tr("Normal") << QObject::tr("SYMMETRICAL")
                         << QObject::tr("CUSTOM")),
        m_values({"LV_BAR_TYPE_NORMAL", "LV_BAR_TYPE_SYMMETRICAL",
                  "LV_BAR_TYPE_CUSTOM"}) {}

  QString name() const { return QObject::tr("Type"); }
  QString codename() const { return "Type"; }

  QStringList function(LVGLObject *obj) const {
    if (get(obj) == 0) return QStringList();
    return QStringList() << QString("lv_bar_set_type(%1, %2);")
                                .arg(obj->codeName())
                                .arg(m_values.at(get(obj)));
  }

 protected:
  int get(LVGLObject *obj) const { return lv_bar_get_type(obj->obj()); }
  void set(LVGLObject *obj, int index) {
    lv_bar_set_type(obj->obj(), index & 0xff);
  }

  QStringList m_values;
};

class LVGLPropertyBarAnimationTime : public LVGLPropertyInt {
 public:
  LVGLPropertyBarAnimationTime()
      : LVGLPropertyInt(0, UINT16_MAX, " ms"), m_value(200) {}

  QString name() const { return QObject::tr("Animation time"); }
  QString codename() const { return "Animation time"; }

  QStringList function(LVGLObject *obj) const {
    return QStringList() << QString("lv_bar_set_anim_time(%1,%2);")
                                .arg(obj->codeName())
                                .arg(m_value);
  }

 protected:
  int get(LVGLObject *obj) const { return lv_bar_get_anim_time(obj->obj()); }
  void set(LVGLObject *obj, int value) {
    m_value = value;
    lv_bar_set_anim_time(obj->obj(), static_cast<uint16_t>(value));
  }
  int m_value;
};

class LVGLPropertyBarAnimalState : public LVGLPropertyEnum {
 public:
  LVGLPropertyBarAnimalState()
      : LVGLPropertyEnum(QStringList() << "On"
                                       << "Off") {}

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

class LVGLPropertyBarValue : public LVGLPropertyInt {
 public:
  LVGLPropertyBarValue(LVGLPropertyBarAnimalState *lpbas)
      : LVGLPropertyInt(INT16_MIN, INT16_MAX), m_lpbas(lpbas) {}

  QString name() const { return QObject::tr("Value"); }
  QString codename() const { return "Value"; }

  QStringList function(LVGLObject *obj) const {
    if (0 == m_lpbas->getIndex())
      return QStringList() << QString("lv_bar_set_value(%1, %2, LV_ANIM_ON);")
                                  .arg(obj->codeName())
                                  .arg(get(obj));
    else
      return QStringList() << QString("lv_bar_set_value(%1, %2, LV_ANIM_OFF);")
                                  .arg(obj->codeName())
                                  .arg(get(obj));
  }

 protected:
  int get(LVGLObject *obj) const { return lv_bar_get_value(obj->obj()); }
  void set(LVGLObject *obj, int value) {
    if (0 == m_lpbas->getIndex())
      lv_bar_set_value(obj->obj(), static_cast<int16_t>(value), LV_ANIM_ON);
    else
      lv_bar_set_value(obj->obj(), static_cast<int16_t>(value), LV_ANIM_OFF);
  }

 private:
  LVGLPropertyBarAnimalState *m_lpbas;
};

class LVGLPropertyBarStyle : public LVGLPropertyEnum {
 public:
  LVGLPropertyBarStyle()
      : LVGLPropertyEnum({QObject::tr("Custome Style"),
                          QObject::tr("Default Style"), QObject::tr("Style1"),
                          QObject::tr("Style2"), QObject::tr("Style3"),
                          QObject::tr("Style4"), QObject::tr("Style5"),
                          QObject::tr("Style6")}) {}

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
      lv_obj_set_style_local_bg_grad_dir(obj->obj(), 0, 0, LV_GRAD_DIR_NONE);
      lv_obj_set_style_local_bg_grad_dir(obj->obj(), LV_BAR_PART_INDIC, 0,
                                         LV_GRAD_DIR_NONE);
      lv_obj_set_style_local_bg_color(obj->obj(), 0, 0, lv_color_hex(0xefefef));
      lv_obj_set_style_local_bg_grad_color(obj->obj(), 0, 0,
                                           lv_color_hex(0xffffff));
      lv_obj_set_style_local_bg_color(obj->obj(), 1, 0, lv_color_hex(0x01a2b1));
      lv_obj_set_style_local_bg_grad_color(obj->obj(), 1, 0,
                                           lv_color_hex(0xffffff));
      lv_obj_set_style_local_radius(obj->obj(), 0, 0, 32767);
      lv_obj_set_style_local_radius(obj->obj(), 1, 0, 32767);
      lv_obj_set_style_local_bg_opa(obj->obj(), 0, 0, 255);
      lv_obj_set_style_local_border_opa(obj->obj(), 0, 0, 0);
      lv_obj_set_style_local_border_width(obj->obj(), 0, 0, 0);
    }
    switch (index) {
      case 1: {
      } break;
      case 2: {
        lv_obj_set_style_local_bg_grad_dir(obj->obj(), LV_BAR_PART_INDIC, 0,
                                           LV_GRAD_DIR_HOR);
        lv_obj_set_style_local_bg_color(obj->obj(), LV_BAR_PART_INDIC, 0,
                                        lv_color_hex(0x2ab5ff));
        lv_obj_set_style_local_bg_grad_color(obj->obj(), LV_BAR_PART_INDIC, 0,
                                             lv_color_hex(0x1677ff));
      } break;
      case 3: {
        lv_obj_set_style_local_bg_grad_dir(obj->obj(), LV_BAR_PART_INDIC, 0,
                                           LV_GRAD_DIR_VER);
        lv_obj_set_style_local_bg_color(obj->obj(), LV_BAR_PART_INDIC, 0,
                                        lv_color_hex(0x2ab5ff));
        lv_obj_set_style_local_bg_grad_color(obj->obj(), LV_BAR_PART_INDIC, 0,
                                             lv_color_hex(0x1677ff));
        lv_obj_set_style_local_radius(obj->obj(), 0, 0, 0);
        lv_obj_set_style_local_radius(obj->obj(), 1, 0, 0);
      } break;
      case 4: {
        lv_obj_set_style_local_bg_grad_dir(obj->obj(), LV_BAR_PART_INDIC, 0,
                                           LV_GRAD_DIR_HOR);
        lv_obj_set_style_local_bg_color(obj->obj(), LV_BAR_PART_INDIC, 0,
                                        lv_color_hex(0xf12711));
        lv_obj_set_style_local_bg_grad_color(obj->obj(), LV_BAR_PART_INDIC, 0,
                                             lv_color_hex(0xf5af19));
        lv_obj_set_style_local_bg_opa(obj->obj(), 0, 0, 0);
        lv_obj_set_style_local_border_opa(obj->obj(), 0, 0, 255);
        lv_obj_set_style_local_border_width(obj->obj(), 0, 0, 2);
      } break;
      case 5: {
        lv_obj_set_style_local_bg_grad_dir(obj->obj(), LV_BAR_PART_INDIC, 0,
                                           LV_GRAD_DIR_VER);
        lv_obj_set_style_local_bg_color(obj->obj(), LV_BAR_PART_INDIC, 0,
                                        lv_color_hex(0xf12711));
        lv_obj_set_style_local_bg_grad_color(obj->obj(), LV_BAR_PART_INDIC, 0,
                                             lv_color_hex(0xf5af19));
        lv_obj_set_style_local_radius(obj->obj(), 0, 0, 0);
        lv_obj_set_style_local_radius(obj->obj(), 1, 0, 0);
        lv_obj_set_style_local_bg_opa(obj->obj(), 0, 0, 0);
        lv_obj_set_style_local_border_opa(obj->obj(), 0, 0, 255);
        lv_obj_set_style_local_border_width(obj->obj(), 0, 0, 2);
      } break;
      case 6: {
        lv_obj_set_style_local_bg_color(obj->obj(), 0, 0,
                                        lv_color_hex(0xffffff));
        lv_obj_set_style_local_bg_grad_color(obj->obj(), 0, 0,
                                             lv_color_hex(0xefefef));
        lv_obj_set_style_local_bg_grad_dir(obj->obj(), 0, 0, LV_GRAD_DIR_HOR);
        lv_obj_set_style_local_bg_grad_dir(obj->obj(), LV_BAR_PART_INDIC, 0,
                                           LV_GRAD_DIR_HOR);
        lv_obj_set_style_local_bg_color(obj->obj(), 1, 0,
                                        lv_color_hex(0xc6c6c6));
        lv_obj_set_style_local_bg_grad_color(obj->obj(), 1, 0,
                                             lv_color_hex(0x000000));
      } break;
      case 7: {
        lv_obj_set_style_local_bg_color(obj->obj(), 0, 0,
                                        lv_color_hex(0xffffff));
        lv_obj_set_style_local_bg_grad_color(obj->obj(), 0, 0,
                                             lv_color_hex(0xefefef));
        lv_obj_set_style_local_bg_grad_dir(obj->obj(), 0, 0, LV_GRAD_DIR_VER);
        lv_obj_set_style_local_bg_grad_dir(obj->obj(), LV_BAR_PART_INDIC, 0,
                                           LV_GRAD_DIR_VER);
        lv_obj_set_style_local_bg_color(obj->obj(), 1, 0,
                                        lv_color_hex(0xc6c6c6));
        lv_obj_set_style_local_bg_grad_color(obj->obj(), 1, 0,
                                             lv_color_hex(0x000000));
        lv_obj_set_style_local_radius(obj->obj(), 0, 0, 0);
        lv_obj_set_style_local_radius(obj->obj(), 1, 0, 0);
      } break;
    }
  }
};

LVGLBar::LVGLBar() {
  initStateStyles();
  m_parts << LV_BAR_PART_BG;
  m_parts << LV_BAR_PART_INDIC;
  auto p = new LVGLPropertyBarRange;
  m_properties << p;
  m_properties << new LVGLPropertyBarType;
  m_properties << new LVGLPropertyBarAnimationTime;
  auto lpbas = new LVGLPropertyBarAnimalState;
  m_properties << lpbas;
  m_properties << new LVGLPropertyBarValue(lpbas);
  m_properties << new LVGLPropertyBarStyle;

  m_editableStyles << LVGL::Body;  // LV_BAR_STYLE_BG
  m_editableStyles << LVGL::Body;  // LV_BAR_STYLE_INDIC
}

QString LVGLBar::name() const { return QObject::tr("Bar"); }

QString LVGLBar::className() const { return "lv_bar"; }

LVGLWidget::Type LVGLBar::type() const { return Bar; }

QIcon LVGLBar::icon() const { return QIcon(); }

lv_obj_t *LVGLBar::newObject(lv_obj_t *parent) const {
  lv_obj_t *obj = lv_bar_create(parent, nullptr);
  _lv_obj_set_style_local_color(
      obj, LV_BAR_PART_BG,
      (LV_STATE_DEFAULT << LV_STYLE_STATE_POS) | LV_STYLE_BG_COLOR,
      lvgl.fromColor(QColor("#e8e8e8")));
  return obj;
}

QSize LVGLBar::minimumSize() const { return QSize(200, 30); }

QStringList LVGLBar::styles() const {
  return QStringList() << "BAR_PART_BG"
                       << "BAR_PART_INDIC";
}

lv_style_t *LVGLBar::style(lv_obj_t *obj, lv_obj_part_t part) const {
  return lv_obj_get_local_style(obj, part);
}

void LVGLBar::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const {
  lv_obj_add_style(obj, LV_BAR_PART_BG, style);
}

void LVGLBar::addStyle(lv_obj_t *obj, lv_style_t *style,
                       lv_obj_part_t part) const {
  lv_obj_add_style(obj, part, style);
}

void LVGLBar::initStateStyles() {
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
    if (i == 0)
      lv_style_set_bg_color(de, LV_STATE_DEFAULT,
                            lvgl.fromColor(QColor("#e8e8e8")));
    QList<lv_style_t *> stateStyles;
    stateStyles << de << ch << fo << ed << ho << pr << di;
    m_partsStyles[i] = stateStyles;
  }
}
