#include "LVGLTileView.h"

#include <QIcon>

#include "core/LVGLObject.h"

LVGLTileView::LVGLTileView() {
  initStateStyles();
  m_parts << LV_TILEVIEW_PART_BG << LV_TILEVIEW_PART_SCROLLBAR
          << LV_TILEVIEW_PART_EDGE_FLASH;

  m_editableStyles << LVGL::PageBG;         // LV_TILEVIEW_PART_BG
  m_editableStyles << LVGL::PageSCROLLBAR;  // LV_TILEVIEW_PART_SCROLLBAR
  m_editableStyles << LVGL::PageEDGEFLASH;  // LV_TILEVIEW_PART_EDGE_FLASH
}

QString LVGLTileView::name() const { return "TileView"; }

QString LVGLTileView::className() const { return "lv_tileview"; }

LVGLWidget::Type LVGLTileView::type() const { return TileView; }

QIcon LVGLTileView::icon() const { return QIcon(); }

lv_obj_t *LVGLTileView::newObject(lv_obj_t *parent) const {
  lv_obj_t *obj = lv_tileview_create(parent, nullptr);
  return obj;
}

QSize LVGLTileView::minimumSize() const { return QSize(100, 100); }

QStringList LVGLTileView::styles() const {
  return QStringList() << "TILEVIEW_PART_BG"
                       << "TILEVIEW_PART_SCROLLBAR"
                       << "TILEVIEW_PART_EDGE_FLASH";
}

lv_style_t *LVGLTileView::style(lv_obj_t *obj, lv_obj_part_t part) const {
  return lv_obj_get_local_style(obj, part);
}

void LVGLTileView::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const {
  lv_obj_add_style(obj, LV_BTN_PART_MAIN, style);
}

void LVGLTileView::addStyle(lv_obj_t *obj, lv_style_t *style,
                            lv_obj_part_t part) const {
  lv_obj_add_style(obj, part, style);
}

void LVGLTileView::initStateStyles() {
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
