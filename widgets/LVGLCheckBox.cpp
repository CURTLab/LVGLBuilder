#include "LVGLCheckBox.h"

#include <QIcon>

#include "LVGLObject.h"

class LVGLPropertyCBChecked : public LVGLPropertyBool {
 public:
  QString name() const { return "Checked"; }

 protected:
  bool get(LVGLObject *obj) const { return lv_checkbox_is_checked(obj->obj()); }
  void set(LVGLObject *obj, bool boolean) {
    lv_checkbox_set_checked(obj->obj(), boolean);
  }
};

class LVGLPropertyCBInactive : public LVGLPropertyBool {
 public:
  QString name() const { return "Inactive"; }

 protected:
  bool get(LVGLObject *obj) const {
    return lv_checkbox_is_inactive(obj->obj());
  }
  void set(LVGLObject *obj, bool boolean) {
    lv_btn_set_state(obj->obj(),
                     boolean ? LV_BTN_STATE_DISABLED : LV_BTN_STATE_RELEASED);
  }
};

LVGLCheckBox::LVGLCheckBox() {
  initStateStyles();
  m_parts << LV_CHECKBOX_PART_BG << LV_CHECKBOX_PART_BULLET;

  m_properties << new LVGLPropertyString(
      "Text", "lv_cb_set_text", lv_checkbox_set_text, lv_checkbox_get_text);
  m_properties << new LVGLPropertyCBChecked;
  m_properties << new LVGLPropertyCBInactive;

  m_editableStyles << LVGL::Checkbox;  // LV_CHECKBOX_PART_BG
  m_editableStyles << LVGL::Checkbox;  // LV_CHECKBOX_PART_BULLET
}

QString LVGLCheckBox::name() const { return "Check box"; }

QString LVGLCheckBox::className() const { return "lv_cb"; }

LVGLWidget::Type LVGLCheckBox::type() const { return CheckBox; }

QIcon LVGLCheckBox::icon() const { return QIcon(); }

lv_obj_t *LVGLCheckBox::newObject(lv_obj_t *parent) const {
  lv_obj_t *obj = lv_checkbox_create(parent, nullptr);
  return obj;
}

QSize LVGLCheckBox::minimumSize() const { return QSize(100, 35); }

QStringList LVGLCheckBox::styles() const {
  return QStringList() << "CHECKBOX_PART_BG"
                       << "CHECKBOX_PART_BULLET";
}

lv_style_t *LVGLCheckBox::style(lv_obj_t *obj, lv_obj_part_t part) const {
  return lv_obj_get_local_style(obj, part);
}

void LVGLCheckBox::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const {
  lv_obj_add_style(obj, LV_BTN_PART_MAIN, style);
}

void LVGLCheckBox::addStyle(lv_obj_t *obj, lv_style_t *style,
                            lv_obj_part_t part) const {
  lv_obj_add_style(obj, part, style);
}

void LVGLCheckBox::initStateStyles() {
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
