#include "LVGLBar.h"

#include <QIcon>

#include "LVGLCore.h"
#include "LVGLObject.h"
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
      : LVGLPropertyEnum(QStringList() << "Normal"
                                       << "SYMMETRICAL"
                                       << "CUSTOM"),
        m_values({"LV_BAR_TYPE_NORMAL", "LV_BAR_TYPE_SYMMETRICAL",
                  "LV_BAR_TYPE_CUSTOM"}) {}

  QString name() const { return "Type"; }

  QStringList function(LVGLObject *obj) const {
    if (get(obj) == LV_LABEL_ALIGN_LEFT) return QStringList();
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

  QString name() const { return "Animation time"; }

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

class LVGLPropertyBarValue : public LVGLPropertyAnyFunc {
 public:
  LVGLPropertyBarValue(const AnyFuncColType arr[], int arrsize,
                       LVGLPropertyBarRange *p)
      : LVGLPropertyAnyFunc(arr, arrsize, true), m_lpbr(p), m_frun(true) {}

  QString name() const { return "Value"; }

  QStringList function(LVGLObject *obj) const {
    if (!m_list.empty()) {
      QStringList strlist = m_list[0].split(' ');
      int value = strlist[0].toInt();
      QString str = strlist[1];
      QString codestr =
          QString("lv_bar_set_value(%1, %2, ").arg(obj->codeName()).arg(value);
      codestr += str + ");";
      return QStringList() << codestr;
    }
    return QStringList();
  }

 protected:
  QStringList get(LVGLObject *obj) const {
    Q_UNUSED(obj)
    if (m_frun) {
      m_frun = false;
      updateData(0, m_lpbr->getmax(), true);
      updateData(0, m_lpbr->getmin(), false);
      updateData(1, QStringList() << "LV_ANIM_ON"
                                  << "LV_ANIM_OFF");
    }
    if (!m_list.isEmpty() && m_list[0] != "Empty list") return m_list;
    return QStringList();
  }
  void set(LVGLObject *obj, QStringList list) {
    m_list = list;
    if (!m_list.empty()) {
      QStringList strlist = m_list[0].split('@');
      int value = strlist[0].toInt();
      QString str = strlist[1];
      lv_anim_enable_t ae;
      if (str == "LV_ANIM_ON")
        ae = LV_ANIM_ON;
      else
        ae = LV_ANIM_OFF;
      lv_bar_set_value(obj->obj(), value, ae);
    }
  }

 private:
  QStringList m_list;
  LVGLPropertyBarRange *m_lpbr;
  mutable bool m_frun;
};

class LVGLPropertyBarStyle : public LVGLPropertyEnum {
 public:
  LVGLPropertyBarStyle()
      : LVGLPropertyEnum({"Custome Style", "Default Style", "Style1", "Style2",
                          "Style3", "Style4", "Style5", "Style6"}) {}

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
  static const AnyFuncColType arr[2] = {e_QSpinBox, e_QComboBox};
  m_properties << new LVGLPropertyBarValue(arr, 2, p);
  m_properties << new LVGLPropertyBarStyle;

  m_editableStyles << LVGL::Body;  // LV_BAR_STYLE_BG
  m_editableStyles << LVGL::Body;  // LV_BAR_STYLE_INDIC
}

QString LVGLBar::name() const { return "Bar"; }

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
