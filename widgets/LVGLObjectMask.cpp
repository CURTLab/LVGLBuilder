#include "LVGLObjectMask.h"

#include <QIcon>

#include "core/LVGLObject.h"

LVGLObjectMask::LVGLObjectMask() {
  initStateStyles();
  m_parts << LV_OBJMASK_PART_MAIN;
  m_editableStyles << LVGL::Body;
}

QString LVGLObjectMask::name() const { return QObject::tr("ObjectMask"); }

QString LVGLObjectMask::className() const { return "lv_objmask"; }

LVGLWidget::Type LVGLObjectMask::type() const { return ObjectMask; }

QIcon LVGLObjectMask::icon() const { return QIcon(); }

lv_obj_t *LVGLObjectMask::newObject(lv_obj_t *parent) const {
  lv_obj_t *obj = lv_objmask_create(parent, nullptr);
  return obj;
}

QSize LVGLObjectMask::minimumSize() const { return QSize(100, 100); }

QStringList LVGLObjectMask::styles() const {
  return QStringList() << "OBJMASK_PART_MAIN";
}

lv_style_t *LVGLObjectMask::style(lv_obj_t *obj, lv_obj_part_t part) const {
  return lv_obj_get_local_style(obj, part);
}

void LVGLObjectMask::setStyle(lv_obj_t *obj, int type,
                              lv_style_t *style) const {
  lv_obj_add_style(obj, LV_BTN_PART_MAIN, style);
}

void LVGLObjectMask::addStyle(lv_obj_t *obj, lv_style_t *style,
                              lv_obj_part_t part) const {
  lv_obj_add_style(obj, part, style);
}

void LVGLObjectMask::initStateStyles() {
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
