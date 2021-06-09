#include "LVGLSpinner.h"

#include <QIcon>

#include "core/LVGLObject.h"

LVGLSpinner::LVGLSpinner() {
  initStateStyles();
  m_parts << LV_SPINNER_PART_BG << LV_SPINNER_PART_INDIC;
  m_editableStyles << LVGL::ArcBG      // LV_SPINNER_PART_BG
                   << LVGL::ArcINDIC;  // LV_SPINNER_PART_INDIC
}

QString LVGLSpinner::name() const { return QObject::tr("Spinner"); }

QString LVGLSpinner::className() const { return "lv_spinner"; }

LVGLWidget::Type LVGLSpinner::type() const { return Spinner; }

QIcon LVGLSpinner::icon() const { return QIcon(); }

lv_obj_t *LVGLSpinner::newObject(lv_obj_t *parent) const {
  lv_obj_t *obj = lv_spinner_create(parent, nullptr);
  return obj;
}

QSize LVGLSpinner::minimumSize() const { return QSize(100, 100); }

QStringList LVGLSpinner::styles() const {
  return QStringList() << "SPINNER_PART_BG"
                       << "SPINNER_PART_INDIC";
}

lv_style_t *LVGLSpinner::style(lv_obj_t *obj, lv_obj_part_t part) const {
  return lv_obj_get_local_style(obj, part);
}

void LVGLSpinner::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const {
  lv_obj_add_style(obj, LV_BTN_PART_MAIN, style);
}

void LVGLSpinner::addStyle(lv_obj_t *obj, lv_style_t *style,
                           lv_obj_part_t part) const {
  lv_obj_add_style(obj, part, style);
}

void LVGLSpinner::initStateStyles() {
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
