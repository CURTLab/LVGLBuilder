#include "LVGLButton.h"

#include <QIcon>

#include "LVGLObject.h"

class LVGLPropertyButtonState : public LVGLPropertyEnum {
 public:
  LVGLPropertyButtonState()
      : LVGLPropertyEnum({"Released", "Pressed", "Disabled", "Checked released",
                          "Checked pressed", "Checked Disabled"}),
        m_values({"LV_BTN_STATE_RELEASED", "LV_BTN_STATE_PRESSED",
                  "LV_BTN_STATE_DISABLED", "LV_BTN_STATE_CHECKED_RELEASED",
                  "LV_BTN_STATE_CHECKED_PRESSED",
                  "LV_BTN_STATE_CHECKED_DISABLED"}) {}

  QString name() const { return "State"; }

  QStringList function(LVGLObject *obj) const {
    if (get(obj) != LV_BTN_STATE_RELEASED)
      return QStringList() << QString("lv_btn_set_state(%1, %2);")
                                  .arg(obj->codeName())
                                  .arg(m_values.at(get(obj)));
    return QStringList();
  }

 protected:
  int get(LVGLObject *obj) const { return lv_btn_get_state(obj->obj()); }
  void set(LVGLObject *obj, int index) {
    lv_btn_set_state(obj->obj(), index & 0xff);
  }

  QStringList m_values;
};

class LVGLPropertyButtonLayout : public LVGLPropertyEnum {
 public:
  LVGLPropertyButtonLayout()
      : LVGLPropertyEnum(
            {"Off", "Center", "Column left align", "Column middle align",
             "Column right align", "Row top align", "Row middle align",
             "Row bottom align", "Pretty top align", "Pretty middle align",
             "Pretty bottom align", "Grid"}),
        m_values({"LV_LAYOUT_OFF", "LV_LAYOUT_CENTER", "LV_LAYOUT_COL_L",
                  "LV_LAYOUT_COL_M", "LV_LAYOUT_COL_R", "LV_LAYOUT_ROW_T",
                  "LV_LAYOUT_ROW_M", "LV_LAYOUT_ROW_B", "LV_LAYOUT_PRETTY_T",
                  "LV_LAYOUT_PRETTY_M", "LV_LAYOUT_PRETTY_B",
                  "LV_LAYOUT_GRID"}) {}

  QString name() const { return "Layout"; }

  QStringList function(LVGLObject *obj) const {
    if (get(obj) != LV_LAYOUT_CENTER)
      return QStringList() << QString("lv_btn_set_layout(%1, %2);")
                                  .arg(obj->codeName())
                                  .arg(m_values.at(get(obj)));
    return QStringList();
  }

 protected:
  int get(LVGLObject *obj) const { return lv_btn_get_layout(obj->obj()); }
  void set(LVGLObject *obj, int index) {
    lv_btn_set_layout(obj->obj(), index & 0xff);
  }

  QStringList m_values;
};

class LVGLPropertyButtonFit : public LVGLPropertyEnum {
 public:
  LVGLPropertyButtonFit()
      : LVGLPropertyEnum({"None", "Tight", "Parent", "Max"}),
        m_values(
            {"LV_FIT_NONE", "LV_FIT_TIGHT", "LV_FIT_PARENT", "LV_FIT_MAX"}),
        m_index(0) {}

  QString name() const { return "Fit"; }

  QStringList function(LVGLObject *obj) const {
    return QStringList() << QString("lv_btn_set_fit(%1, %2);")
                                .arg(obj->codeName())
                                .arg(m_values.at(m_index));
    return QStringList();
  }

 protected:
  int get(LVGLObject *obj) const {
    Q_UNUSED(obj)
    return m_index;
  }
  void set(LVGLObject *obj, int index) {
    m_index = index;
    lv_btn_set_fit(obj->obj(), m_index & 0xff);
  }
  QStringList m_values;
  int m_index;
};

LVGLButton::LVGLButton() {
  //  lv_fit_t t;
  initStateStyles();
  m_parts << LV_BTN_PART_MAIN;
  m_properties << new LVGLPropertyButtonState;
  m_properties << new LVGLPropertyBool("Checkable", "lv_btn_set_checkable",
                                       lv_btn_set_checkable,
                                       lv_btn_get_checkable);
  m_properties << new LVGLPropertyButtonLayout;
  m_properties << new LVGLPropertyButtonFit;

  m_editableStyles << LVGL::Button;  // LV_BTN_PART_MAIN
}

QString LVGLButton::name() const { return "Button"; }

QString LVGLButton::className() const { return "lv_btn"; }

LVGLWidget::Type LVGLButton::type() const { return Button; }

QIcon LVGLButton::icon() const { return QIcon(); }

lv_obj_t *LVGLButton::newObject(lv_obj_t *parent) const {
  lv_obj_t *obj = lv_btn_create(parent, nullptr);
  return obj;
}

QSize LVGLButton::minimumSize() const { return QSize(100, 35); }

QStringList LVGLButton::styles() const {
  return QStringList() << "BTN_PART_MAIN";
}

lv_style_t *LVGLButton::style(lv_obj_t *obj, lv_obj_part_t part) const {
  return lv_obj_get_local_style(obj, part);
}

void LVGLButton::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const {
  lv_obj_add_style(obj, LV_BTN_PART_MAIN, style);
}

void LVGLButton::addStyle(lv_obj_t *obj, lv_style_t *style,
                          lv_obj_part_t part) const {
  lv_obj_add_style(obj, part, style);
}

void LVGLButton::initStateStyles() {
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
