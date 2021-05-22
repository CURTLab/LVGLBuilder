#include "LVGLPage.h"

#include <QIcon>

#include "core/LVGLObject.h"

class LVGLPropertyPageScrollbars : public LVGLPropertyEnum {
 public:
  LVGLPropertyPageScrollbars()
      : LVGLPropertyEnum(QStringList() << "Off"
                                       << "On"
                                       << "Drag"
                                       << "Auto"
                                       << "Hide"
                                       << "Unhide"),
        m_values({"LV_SCROLLBAR_MODE_OFF", "LV_SCROLLBAR_MODE_ON",
                  "LV_SCROLLBAR_MODE_DRAG", "LV_SCROLLBAR_MODE_AUTO",
                  "LV_SCROLLBAR_MODE_HIDE", "LV_SCROLLBAR_MODE_UNHIDE"}) {}

  QString name() const { return "Scrollbars"; }

  QStringList function(LVGLObject *obj) const {
    QStringList list;
    if (m_values.at(get(obj)) != "LV_SCROLLBAR_MODE_AUTO")
      list << QString("lv_page_set_scrlbar_mode(%1, %2);")
                  .arg(obj->codeName())
                  .arg(m_values.at(get(obj)));
    return list;
  }

 protected:
  int get(LVGLObject *obj) const {
    return lv_page_get_scrlbar_mode(obj->obj()) & 0x03;
  }
  void set(LVGLObject *obj, int index) {
    lv_page_set_scrlbar_mode(obj->obj(), index & 0xff);
  }

  QStringList m_values;
};

class LVGLPropertyPageWidth : public LVGLPropertyCoord {
 public:
  inline LVGLPropertyPageWidth(LVGLProperty *p = nullptr)
      : LVGLPropertyCoord(Qt::Horizontal, p) {}
  inline QString name() const override { return "Scroll width"; }

  inline lv_coord_t get(LVGLObject *obj) const override {
    return lv_page_get_scrl_width(obj->obj());
  }
  inline void set(LVGLObject *obj, lv_coord_t value) override {
    lv_page_set_scrl_width(obj->obj(), value);
  }
};

class LVGLPropertyPageHeight : public LVGLPropertyCoord {
 public:
  inline LVGLPropertyPageHeight(LVGLProperty *p = nullptr)
      : LVGLPropertyCoord(Qt::Vertical, p) {}
  inline QString name() const override { return "Scroll height"; }

  inline lv_coord_t get(LVGLObject *obj) const override {
    return lv_page_get_scrl_height(obj->obj());
  }
  inline void set(LVGLObject *obj, lv_coord_t value) override {
    lv_page_set_scrl_height(obj->obj(), value);
  }
};

class LVGLPropertyPageEdgeFlash : public LVGLPropertyBool {
 public:
  QString name() const { return "Edge flash"; }

  QStringList function(LVGLObject *obj) const {
    return QStringList() << QString("lv_page_set_edge_flash(%1, %2);")
                                .arg(obj->codeName())
                                .arg(QVariant(get(obj)).toString());
  }

 protected:
  bool get(LVGLObject *obj) const { return lv_page_get_edge_flash(obj->obj()); }
  void set(LVGLObject *obj, bool boolean) {
    lv_page_set_edge_flash(obj->obj(), boolean);
  }
};

class LVGLPropertyPageScrollPropagation : public LVGLPropertyBool {
 public:
  QString name() const { return "Scroll propagation"; }

  QStringList function(LVGLObject *obj) const {
    return QStringList() << QString("lv_page_set_scroll_propagation(%1, %2);")
                                .arg(obj->codeName())
                                .arg(QVariant(get(obj)).toString());
  }

 protected:
  bool get(LVGLObject *obj) const {
    return lv_page_get_scroll_propagation(obj->obj());
  }
  void set(LVGLObject *obj, bool boolean) {
    lv_page_set_scroll_propagation(obj->obj(), boolean);
  }
};

LVGLPage::LVGLPage() {
  initStateStyles();
  m_parts << LV_PAGE_PART_BG << LV_PAGE_PART_SCROLLABLE
          << LV_PAGE_PART_SCROLLBAR << LV_PAGE_PART_EDGE_FLASH;
  m_properties << new LVGLPropertyPageScrollbars;
  m_properties << new LVGLPropertyPageWidth;
  m_properties << new LVGLPropertyPageHeight;
  m_properties << new LVGLPropertyPageEdgeFlash;
  m_properties << new LVGLPropertyPageScrollPropagation;

  m_editableStyles << LVGL::PageBG;         // LV_PAGE_PART_BG
  m_editableStyles << LVGL::PageSCRL;       // LV_PAGE_PART_SCROLLABLE
  m_editableStyles << LVGL::PageSCROLLBAR;  // LV_PAGE_PART_SCROLLBAR
  m_editableStyles << LVGL::PageEDGEFLASH;  // LV_PAGE_PART_EDGE_FLASH
}

QString LVGLPage::name() const { return "Page"; }

QString LVGLPage::className() const { return "lv_page"; }

LVGLWidget::Type LVGLPage::type() const { return Page; }

QIcon LVGLPage::icon() const { return QIcon(); }

lv_obj_t *LVGLPage::newObject(lv_obj_t *parent) const {
  lv_obj_t *obj = lv_page_create(parent, nullptr);
  return obj;
}

QSize LVGLPage::minimumSize() const { return QSize(100, 35); }

QStringList LVGLPage::styles() const {
  return QStringList() << "PAGE_PART_BG"
                       << "PAGE_PART_SCROLLABLE"
                       << "PAGE_PART_SCROLLBAR"
                       << "PAGE_PART_EDGE_FLASH";
}

lv_style_t *LVGLPage::style(lv_obj_t *obj, lv_obj_part_t part) const {
  return lv_obj_get_local_style(obj, part);
}

void LVGLPage::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const {
  lv_obj_add_style(obj, LV_BTN_PART_MAIN, style);
}

void LVGLPage::addStyle(lv_obj_t *obj, lv_style_t *style,
                        lv_obj_part_t part) const {
  lv_obj_add_style(obj, part, style);
}

void LVGLPage::initStateStyles() {
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
