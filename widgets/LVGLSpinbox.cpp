#include "LVGLSpinbox.h"

#include <QIcon>

#include "core/LVGLObject.h"

LVGLSpinbox::LVGLSpinbox() {
  initStateStyles();
  m_parts << LV_SPINBOX_PART_BG << LV_SPINBOX_PART_CURSOR;

  m_editableStyles << LVGL::SpinboxBG;      // LV_SPINBOX_PART_BG
  m_editableStyles << LVGL::SpinboxCURSOR;  // LV_SPINBOX_PART_CURSOR
}

QString LVGLSpinbox::name() const { return QObject::tr("Spinbox"); }

QString LVGLSpinbox::codename() const { return "Spinbox"; }

QString LVGLSpinbox::className() const { return "lv_spinbox"; }

LVGLWidget::Type LVGLSpinbox::type() const { return SpinBox; }

QIcon LVGLSpinbox::icon() const { return QIcon(); }

lv_obj_t *LVGLSpinbox::newObject(lv_obj_t *parent) const {
  lv_obj_t *obj = lv_spinbox_create(parent, nullptr);
  return obj;
}

QSize LVGLSpinbox::minimumSize() const { return QSize(80, 30); }

QStringList LVGLSpinbox::styles() const {
  return QStringList() << "LV_SPINBOX_PART_BG"
                       << "LV_SPINBOX_PART_CURSOR";
}

lv_style_t *LVGLSpinbox::style(lv_obj_t *obj, lv_obj_part_t part) const {
  return lv_obj_get_local_style(obj, part);
}

void LVGLSpinbox::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const {
  lv_obj_add_style(obj, LV_BTN_PART_MAIN, style);
}

void LVGLSpinbox::addStyle(lv_obj_t *obj, lv_style_t *style,
                           lv_obj_part_t part) const {
  lv_obj_add_style(obj, part, style);
}

void LVGLSpinbox::initStateStyles() {
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
