#include "LVGLRoller.h"

#include <QComboBox>
#include <QDebug>
#include <QIcon>
#include <QSpinBox>

#include "LVGLObject.h"

class LVGLPropertyRolleroptionsN : public LVGLPropertyStringPlus {
 public:
  QString name() const { return "Options"; }

  QStringList function(LVGLObject *obj) const {
    return QStringList()
           << QString(
                  "lv_roller_set_options(%1, \"%2\",LV_ROLLER_MODE_NORMAL);")
                  .arg(obj->codeName())
                  .arg(get(obj));
  }

 protected:
  QString get(LVGLObject *obj) const {
    return lv_roller_get_options(obj->obj());
  }
  void set(LVGLObject *obj, QString string) {
    lv_roller_set_options(obj->obj(), qUtf8Printable(string),
                          LV_ROLLER_MODE_NORMAL);
  }
};

class LVGLPropertyRollerAlign : public LVGLPropertyEnum {
 public:
  LVGLPropertyRollerAlign()
      : LVGLPropertyEnum(QStringList() << "Left"
                                       << "Center"
                                       << "Right"),
        m_values({"LV_LABEL_ALIGN_LEFT", "LV_LABEL_ALIGN_CENTER",
                  "LV_LABEL_ALIGN_RIGHT"}) {}

  QString name() const { return "Align"; }

  QStringList function(LVGLObject *obj) const {
    if (get(obj) == LV_LABEL_ALIGN_LEFT) return QStringList();
    return QStringList() << QString("lv_roller_set_align(%1, %2);")
                                .arg(obj->codeName())
                                .arg(m_values.at(get(obj)));
  }

 protected:
  int get(LVGLObject *obj) const { return lv_roller_get_align(obj->obj()); }
  void set(LVGLObject *obj, int index) {
    lv_roller_set_align(obj->obj(), index & 0xff);
  }

  QStringList m_values;
};

class LVGLPropertyRollerVisibleRows : public LVGLPropertyInt {
 public:
  LVGLPropertyRollerVisibleRows()
      : LVGLPropertyInt(0, UINT8_MAX, ""), m_value(3) {}

  QString name() const { return "Visible rows"; }

  QStringList function(LVGLObject *obj) const {
    return QStringList() << QString("lv_roller_set_visible_row_count(%1,%2);")
                                .arg(obj->codeName())
                                .arg(m_widget->value());
  }

 protected:
  int get(LVGLObject *obj) const {
    Q_UNUSED(obj)
    return m_value;
  }
  void set(LVGLObject *obj, int value) {
    m_value = value;
    lv_roller_set_visible_row_count(obj->obj(), static_cast<uint16_t>(value));
  }

 private:
  mutable int m_value;
};

class LVGLPropertyRollerAnimationTime : public LVGLPropertyInt {
 public:
  LVGLPropertyRollerAnimationTime() : LVGLPropertyInt(0, UINT16_MAX, " ms") {}

  QString name() const { return "Animation time"; }

  QStringList function(LVGLObject *obj) const {
    return QStringList() << QString("lv_roller_set_anim_time(%1,%2);")
                                .arg(obj->codeName())
                                .arg(m_value);
  }

 protected:
  int get(LVGLObject *obj) const {
    m_value = lv_roller_get_anim_time(obj->obj());
    return m_value;
  }
  void set(LVGLObject *obj, int value) {
    lv_roller_set_anim_time(obj->obj(), static_cast<uint16_t>(value));
  }

 private:
  mutable uint16_t m_value;
};

LVGLRoller::LVGLRoller() {
  initStateStyles();
  m_properties << new LVGLPropertyRolleroptionsN << new LVGLPropertyRollerAlign;
  m_properties << new LVGLPropertyRollerVisibleRows;
  m_properties << new LVGLPropertyRollerAnimationTime;
  m_properties << new LVGLPropertyBool("Auto fit", "lv_roller_set_fit",
                                       lv_roller_set_auto_fit,
                                       lv_roller_get_auto_fit);
  m_parts << LV_ROLLER_PART_BG << LV_ROLLER_PART_SELECTED;
  m_editableStyles << LVGL::RollerMAIN;  // LV_ROLLER_PART_BG
  m_editableStyles << LVGL::RollerMAIN;  // LV_ROLLER_PART_SELECTED
}

QString LVGLRoller::name() const { return "Roller"; }

QString LVGLRoller::className() const { return "lv_roller"; }

LVGLWidget::Type LVGLRoller::type() const { return Roller; }

QIcon LVGLRoller::icon() const { return QIcon(); }

lv_obj_t *LVGLRoller::newObject(lv_obj_t *parent) const {
  lv_obj_t *obj = lv_roller_create(parent, nullptr);
  return obj;
}

QSize LVGLRoller::minimumSize() const { return QSize(100, 100); }

QStringList LVGLRoller::styles() const {
  return QStringList() << "LV_ROLLER_PART_BG"
                       << "LV_ROLLER_PART_SELECTED";
}

lv_style_t *LVGLRoller::style(lv_obj_t *obj, lv_obj_part_t part) const {
  return lv_obj_get_local_style(obj, part);
}

void LVGLRoller::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const {
  lv_obj_add_style(obj, LV_BTN_PART_MAIN, style);
}

void LVGLRoller::addStyle(lv_obj_t *obj, lv_style_t *style,
                          lv_obj_part_t part) const {
  lv_obj_add_style(obj, part, style);
}

void LVGLRoller::initStateStyles() {
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
