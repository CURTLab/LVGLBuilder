#include "LVGLArc.h"

#include <QIcon>

#include "core/LVGLObject.h"
#include "properties/LVGLPropertyAnyFunc.h"
#include "properties/LVGLPropertyVal2.h"

class LVGLPropertyArcBgAngle : public LVGLPropertyAnyFunc {
 public:
  LVGLPropertyArcBgAngle(const AnyFuncColType arr[], int size)
      : LVGLPropertyAnyFunc(arr, size, true), m_frun(true) {}
  QString name() const { return QObject::tr("Set Bg Angle"); }
  QString codename() const { return "Set Bg Angle"; }

  QStringList function(LVGLObject *obj) const {
    QStringList list;
    if (!m_list.isEmpty()) {
      QStringList strlist = m_list[0].split('@');
      int min = strlist[0].toInt();
      int max = strlist[1].toInt();
      list << QString("lv_arc_set_bg_angles(%1,%2,%3);")
                  .arg(obj->codeName())
                  .arg(min)
                  .arg(max);
    }
    return list;
  }

 protected:
  QStringList get(LVGLObject *obj) const {
    Q_UNUSED(obj)
    if (m_frun) {
      updateData(0, 0, false);
      updateData(0, 360, true);
      updateData(1, 0, false);
      updateData(1, 360, true);
    }
    int begin = lv_arc_get_bg_angle_start(obj->obj());
    int end = lv_arc_get_bg_angle_end(obj->obj());
    return QStringList() << QString("%1@%2@").arg(begin).arg(end);
  }
  void set(LVGLObject *obj, QStringList list) {
    m_list = list;
    QStringList strlist = m_list[0].split('@');
    int min = strlist[0].toInt();
    int max = strlist[1].toInt();
    lv_arc_set_bg_angles(obj->obj(), min, max);
  }

 private:
  QStringList m_list;
  mutable bool m_frun;
};

class LVGLPropertyArcAngle : public LVGLPropertyAnyFunc {
 public:
  LVGLPropertyArcAngle(const AnyFuncColType arr[], int size)
      : LVGLPropertyAnyFunc(arr, size, true), m_frun(true) {}
  QString name() const { return QObject::tr("Set Angle"); }
  QString codename() const { return "Set Angle"; }

  QStringList function(LVGLObject *obj) const {
    QStringList list;
    int begin = lv_arc_get_angle_start(obj->obj());
    int end = lv_arc_get_angle_end(obj->obj());
    m_list = QStringList() << QString("%1@%2@").arg(begin).arg(end);
    QStringList strlist = m_list[0].split('@');
    int min = strlist[0].toInt();
    int max = strlist[1].toInt();
    list << QString("lv_arc_set_angles(%1,%2,%3);")
                .arg(obj->codeName())
                .arg(min)
                .arg(max);
    return list;
  }

 protected:
  QStringList get(LVGLObject *obj) const {
    Q_UNUSED(obj)
    if (m_frun) {
      updateData(0, 0, false);
      updateData(0, 360, true);
      updateData(1, 0, false);
      updateData(1, 360, true);
    }
    int begin = lv_arc_get_angle_start(obj->obj());
    int end = lv_arc_get_angle_end(obj->obj());
    return QStringList() << QString("%1@%2@").arg(begin).arg(end);
  }
  void set(LVGLObject *obj, QStringList list) {
    m_list = list;
    QStringList strlist = m_list[0].split('@');
    int min = strlist[0].toInt();
    int max = strlist[1].toInt();
    lv_arc_set_angles(obj->obj(), min, max);
  }

 private:
  mutable QStringList m_list;
  mutable bool m_frun;
};

class LVGLPropertyArcRotation : public LVGLPropertyInt {
 public:
  LVGLPropertyArcRotation() : LVGLPropertyInt(0, 360, ""), m_rations(0) {}

  QString name() const { return QObject::tr("Rotation"); }
  QString codename() const { return "Rotation"; }

  QStringList function(LVGLObject *obj) const {
    return QStringList() << QString("lv_arc_set_rotation(%1,%2);")
                                .arg(obj->codeName())
                                .arg(m_rations);
  }

 protected:
  int get(LVGLObject *obj) const {
    Q_UNUSED(obj)
    return m_rations;
  }
  void set(LVGLObject *obj, int value) {
    m_rations = value;
    lv_arc_set_rotation(obj->obj(), static_cast<uint16_t>(value));
  }

 private:
  int m_rations;
};

class LVGLPropertyArcValue : public LVGLPropertyInt {
 public:
  LVGLPropertyArcValue() : LVGLPropertyInt(0, 360, "") {}

  QString name() const { return QObject::tr("Value"); }
  QString codename() const { return "Value"; }

  QStringList function(LVGLObject *obj) const {
    return QStringList() << QString("lv_arc_set_value(%1,%2);")
                                .arg(obj->codeName())
                                .arg(get(obj));
  }

 protected:
  int get(LVGLObject *obj) const { return lv_arc_get_value(obj->obj()); }
  void set(LVGLObject *obj, int value) {
    lv_arc_set_value(obj->obj(), static_cast<uint16_t>(value));
  }
};

class LVGLPropertyArcStyle : public LVGLPropertyEnum {
 public:
  LVGLPropertyArcStyle()
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
      lv_obj_set_size(obj->obj(), 75, 75);
      lv_obj_set_style_local_border_width(obj->obj(), LV_ARC_PART_BG, 0, 2);
      lv_obj_set_style_local_bg_opa(obj->obj(), 0, 0, 255);
      lv_obj_set_style_local_line_color(obj->obj(), 0, 0,
                                        lv_color_hex(0xd4d7d9));
      lv_obj_set_style_local_line_color(obj->obj(), LV_ARC_PART_INDIC, 0,
                                        lv_color_hex(0x01a2b1));
      lv_obj_set_style_local_line_rounded(obj->obj(), 0, 0, true);
      lv_obj_set_style_local_line_rounded(obj->obj(), LV_ARC_PART_INDIC, 0,
                                          true);
      lv_obj_set_style_local_line_width(obj->obj(), 0, 0, 20);
      lv_obj_set_style_local_line_width(obj->obj(), LV_ARC_PART_INDIC, 0, 20);
      lv_obj_set_style_local_line_opa(obj->obj(), 0, 0, 255);
    }
    switch (index) {
      case 1: {
      } break;
      case 2: {
        lv_obj_set_size(obj->obj(), 150, 150);
        lv_obj_set_style_local_border_width(obj->obj(), LV_ARC_PART_BG, 0, 0);
        lv_obj_set_style_local_bg_opa(obj->obj(), 0, 0, 0);
        lv_obj_set_style_local_line_color(obj->obj(), 0, 0,
                                          lv_color_hex(0xefefef));
        lv_obj_set_style_local_line_color(obj->obj(), LV_ARC_PART_INDIC, 0,
                                          lv_color_hex(0x2abf55));
      } break;
      case 3: {
        lv_obj_set_size(obj->obj(), 150, 150);
        lv_obj_set_style_local_border_width(obj->obj(), LV_ARC_PART_BG, 0, 0);
        lv_obj_set_style_local_bg_opa(obj->obj(), 0, 0, 0);
        lv_obj_set_style_local_line_color(obj->obj(), 0, 0,
                                          lv_color_hex(0xefefef));
        lv_obj_set_style_local_line_color(obj->obj(), LV_ARC_PART_INDIC, 0,
                                          lv_color_hex(0xf72111));
        lv_obj_set_style_local_line_rounded(obj->obj(), 0, 0, false);
        lv_obj_set_style_local_line_rounded(obj->obj(), LV_ARC_PART_INDIC, 0,
                                            false);
      } break;
      case 4: {
        lv_obj_set_size(obj->obj(), 150, 150);
        lv_obj_set_style_local_border_width(obj->obj(), LV_ARC_PART_BG, 0, 0);
        lv_obj_set_style_local_bg_opa(obj->obj(), 0, 0, 0);
        lv_obj_set_style_local_line_color(obj->obj(), 0, 0,
                                          lv_color_hex(0xefefef));
        lv_obj_set_style_local_line_color(obj->obj(), LV_ARC_PART_INDIC, 0,
                                          lv_color_hex(0x2abf55));
        lv_obj_set_style_local_line_opa(obj->obj(), 0, 0, 0);
      } break;
      case 5: {
        lv_obj_set_size(obj->obj(), 150, 150);
        lv_obj_set_style_local_border_width(obj->obj(), LV_ARC_PART_BG, 0, 0);
        lv_obj_set_style_local_bg_opa(obj->obj(), 0, 0, 0);
        lv_obj_set_style_local_line_color(obj->obj(), 0, 0,
                                          lv_color_hex(0xefefef));
        lv_obj_set_style_local_line_color(obj->obj(), LV_ARC_PART_INDIC, 0,
                                          lv_color_hex(0x2abf55));
        lv_obj_set_style_local_line_width(obj->obj(), 0, 0, 8);
        lv_obj_set_style_local_line_width(obj->obj(), LV_ARC_PART_INDIC, 0, 8);
      } break;
      case 6: {
        lv_obj_set_size(obj->obj(), 150, 150);
        lv_obj_set_style_local_border_width(obj->obj(), LV_ARC_PART_BG, 0, 0);
        lv_obj_set_style_local_bg_opa(obj->obj(), 0, 0, 0);
        lv_obj_set_style_local_line_color(obj->obj(), 0, 0,
                                          lv_color_hex(0xefefef));
        lv_obj_set_style_local_line_color(obj->obj(), LV_ARC_PART_INDIC, 0,
                                          lv_color_hex(0xf72111));
        lv_obj_set_style_local_line_rounded(obj->obj(), 0, 0, false);
        lv_obj_set_style_local_line_rounded(obj->obj(), LV_ARC_PART_INDIC, 0,
                                            false);
        lv_obj_set_style_local_line_width(obj->obj(), 0, 0, 8);
        lv_obj_set_style_local_line_width(obj->obj(), LV_ARC_PART_INDIC, 0, 8);
      } break;
      case 7: {
        lv_obj_set_size(obj->obj(), 150, 150);
        lv_obj_set_style_local_border_width(obj->obj(), LV_ARC_PART_BG, 0, 0);
        lv_obj_set_style_local_bg_opa(obj->obj(), 0, 0, 0);
        lv_obj_set_style_local_line_color(obj->obj(), 0, 0,
                                          lv_color_hex(0xefefef));
        lv_obj_set_style_local_line_color(obj->obj(), LV_ARC_PART_INDIC, 0,
                                          lv_color_hex(0x2abf55));
        lv_obj_set_style_local_line_opa(obj->obj(), 0, 0, 0);
        lv_obj_set_style_local_line_width(obj->obj(), 0, 0, 8);
        lv_obj_set_style_local_line_width(obj->obj(), LV_ARC_PART_INDIC, 0, 8);
      } break;
    }
  }
};

LVGLArc::LVGLArc() {
  initStateStyles();
  m_parts << LV_ARC_PART_BG << LV_ARC_PART_INDIC << LV_ARC_PART_KNOB;

  static AnyFuncColType arr[2] = {e_QSpinBox, e_QSpinBox};

  m_properties << new LVGLPropertyArcBgAngle(arr, 2);
  m_properties << new LVGLPropertyArcAngle(arr, 2);

  // m_properties << new LVGLPropertyArcValue;
  m_properties << new LVGLPropertyArcRotation;
  m_properties << new LVGLPropertyArcStyle;

  m_editableStyles << LVGL::ArcBG     // LV_ARC_PART_BG
                   << LVGL::ArcINDIC  // LV_ARC_PART_INDIC
                   << LVGL::ArcKNOB;  // LV_ARC_PART_KNOB
}

QString LVGLArc::name() const { return QObject::tr("Arc"); }

QString LVGLArc::className() const { return "lv_arc"; }

LVGLWidget::Type LVGLArc::type() const { return Arc; }

QIcon LVGLArc::icon() const { return QIcon(); }

lv_obj_t *LVGLArc::newObject(lv_obj_t *parent) const {
  lv_obj_t *obj = lv_arc_create(parent, nullptr);
  return obj;
}

QSize LVGLArc::minimumSize() const { return {75, 75}; }

QStringList LVGLArc::styles() const {
  return QStringList() << "ARC_PART_BG"
                       << "ARC_PART_INDIC";
  // << "ARC_PART_KNOB";
}

lv_style_t *LVGLArc::style(lv_obj_t *obj, lv_obj_part_t part) const {
  return lv_obj_get_local_style(obj, part);
}

void LVGLArc::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const {
  lv_obj_add_style(obj, LV_BTN_PART_MAIN, style);
}

void LVGLArc::addStyle(lv_obj_t *obj, lv_style_t *style,
                       lv_obj_part_t part) const {
  lv_obj_add_style(obj, part, style);
}

void LVGLArc::initStateStyles() {
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
