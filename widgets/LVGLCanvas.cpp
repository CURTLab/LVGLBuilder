#include "LVGLCanvas.h"

#include <QIcon>

#include "core/LVGLObject.h"

LVGLCanvas::LVGLCanvas() {
  initStateStyles();
  m_parts << LV_CANVAS_PART_MAIN;
  m_editableStyles << LVGL::Canvas;  // LV_CANVAS_STYLE_MAIN
}

QString LVGLCanvas::name() const { return QObject::tr("Canvas"); }

QString LVGLCanvas::codename() const { return "Canvas"; }

QString LVGLCanvas::className() const { return "lv_canvas"; }

LVGLWidget::Type LVGLCanvas::type() const { return Canvas; }

QIcon LVGLCanvas::icon() const { return QIcon(); }

lv_obj_t *LVGLCanvas::newObject(lv_obj_t *parent) const {
  lv_color_t *cbuf = new lv_color_t[LV_CANVAS_BUF_SIZE_TRUE_COLOR(
      minimumSize().width(), minimumSize().height())];
  lv_obj_t *obj = lv_canvas_create(parent, nullptr);
  lv_canvas_set_buffer(obj, cbuf, minimumSize().width(), minimumSize().height(),
                       LV_IMG_CF_TRUE_COLOR);
  lv_canvas_fill_bg(obj, lv_color_hex(0xc0c0c0), LV_OPA_COVER);
  return obj;
}

QSize LVGLCanvas::minimumSize() const { return QSize(100, 75); }

QStringList LVGLCanvas::styles() const {
  return QStringList() << "CANVAS_PART_MAIN";
}

lv_style_t *LVGLCanvas::style(lv_obj_t *obj, lv_obj_part_t part) const {
  return lv_obj_get_local_style(obj, part);
}

void LVGLCanvas::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const {
  lv_obj_add_style(obj, LV_BTN_PART_MAIN, style);
}

void LVGLCanvas::addStyle(lv_obj_t *obj, lv_style_t *style,
                          lv_obj_part_t part) const {
  lv_obj_add_style(obj, part, style);
}

void LVGLCanvas::initStateStyles() {
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
