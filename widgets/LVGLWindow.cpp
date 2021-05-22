#include "LVGLWindow.h"

#include <QIcon>

#include "core/LVGLObject.h"

LVGLWindow::LVGLWindow() {
  initStateStyles();
  m_parts << LV_WIN_PART_BG << LV_WIN_PART_HEADER
          << LV_WIN_PART_CONTENT_SCROLLABLE << LV_WIN_PART_SCROLLBAR;

  m_editableStyles << LVGL::Background;  // LV_WIN_PART_BG
  m_editableStyles << LVGL::WindowHEAD;  // LV_WIN_PART_HEADER
  m_editableStyles << LVGL::Background;  // LV_WIN_PART_CONTENT_SCROLLABLE
  m_editableStyles << LVGL::PageSCRL;    // LV_WIN_PART_SCROLLBAR
}

QString LVGLWindow::name() const { return "Window"; }

QString LVGLWindow::className() const { return "lv_win"; }

LVGLWidget::Type LVGLWindow::type() const { return Window; }

QIcon LVGLWindow::icon() const { return QIcon(); }

lv_obj_t *LVGLWindow::newObject(lv_obj_t *parent) const {
  lv_obj_t *obj = lv_win_create(parent, nullptr);
  return obj;
}

QSize LVGLWindow::minimumSize() const { return QSize(150, 150); }

QStringList LVGLWindow::styles() const {
  return QStringList() << "WIN_PART_BG"
                       << "WIN_PART_HEADER"
                       << "WIN_PART_CONTENT_SCROLLABLE"
                       << "WIN_PART_SCROLLBAR";
}

lv_style_t *LVGLWindow::style(lv_obj_t *obj, lv_obj_part_t part) const {
  return lv_obj_get_local_style(obj, part);
}

void LVGLWindow::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const {
  lv_obj_add_style(obj, LV_BTN_PART_MAIN, style);
}

void LVGLWindow::addStyle(lv_obj_t *obj, lv_style_t *style,
                          lv_obj_part_t part) const {
  lv_obj_add_style(obj, part, style);
}

void LVGLWindow::initStateStyles() {
  for (int i = 0; i < 4; ++i) {
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
