#include "LVGLList.h"

#include <QIcon>

#include "core/LVGLObject.h"
#include "properties/LVGLPropertyList.h"

// class LVGLPropertyListSingleMode : public LVGLPropertyBool {
// public:
//  QString name() const { return "Single mode"; }

//  QStringList function(LVGLObject *obj) const {
//    return QStringList() << QString("lv_list_set_single_mode(%1, %2);")
//                                .arg(obj->codeName())
//                                .arg(QVariant(get(obj)).toString());
//  }

// protected:
//  bool get(LVGLObject *obj) const {
//    return lv_list_get_single_mode(obj->obj());
//  }
//  void set(LVGLObject *obj, bool boolean) {
//    lv_list_set_single_mode(obj->obj(), boolean);
//  }
//};

class LVGLPropertyListScrollPropagation : public LVGLPropertyBool {
 public:
  QString name() const { return "Scroll propagation"; }

  QStringList function(LVGLObject *obj) const {
    return QStringList() << QString("lv_list_set_scroll_propagation(%1, %2);")
                                .arg(obj->codeName())
                                .arg(QVariant(get(obj)).toString());
  }

 protected:
  bool get(LVGLObject *obj) const {
    return lv_list_get_scroll_propagation(obj->obj());
  }
  void set(LVGLObject *obj, bool boolean) {
    lv_list_set_scroll_propagation(obj->obj(), boolean);
  }
};

class LVGLPropertyListEdgeFlash : public LVGLPropertyBool {
 public:
  QString name() const { return "Edge flash"; }

  QStringList function(LVGLObject *obj) const {
    return QStringList() << QString("lv_list_set_edge_flash(%1, %2);")
                                .arg(obj->codeName())
                                .arg(QVariant(get(obj)).toString());
  }

 protected:
  bool get(LVGLObject *obj) const { return lv_list_get_edge_flash(obj->obj()); }
  void set(LVGLObject *obj, bool boolean) {
    lv_list_set_edge_flash(obj->obj(), boolean);
  }
};

LVGLList::LVGLList() {
  initStateStyles();
  m_parts << LV_LIST_PART_BG << LV_LIST_PART_SCROLLBAR
          << LV_LIST_PART_SCROLLABLE << LV_LIST_PART_EDGE_FLASH;

  m_properties << new LVGLPropertyList;
  //  m_properties << new LVGLPropertyListSingleMode;
  m_properties << new LVGLPropertyListScrollPropagation;
  m_properties << new LVGLPropertyListEdgeFlash;

  m_editableStyles << LVGL::PageBG;         // LV_LIST_PART_BG
  m_editableStyles << LVGL::PageSCRL;       // LV_LIST_PART_SCROLLBAR
  m_editableStyles << LVGL::PageSCROLLBAR;  // LV_LIST_PART_SCROLLABLE
  m_editableStyles << LVGL::PageEDGEFLASH;  // LV_LIST_PART_EDGE_FLASH
}

QString LVGLList::name() const { return "List"; }

QString LVGLList::className() const { return "lv_list"; }

LVGLWidget::Type LVGLList::type() const { return List; }

QIcon LVGLList::icon() const { return QIcon(); }

lv_obj_t *LVGLList::newObject(lv_obj_t *parent) const {
  lv_obj_t *obj = lv_list_create(parent, nullptr);
  return obj;
}

QSize LVGLList::minimumSize() const { return QSize(75, 60); }

QStringList LVGLList::styles() const {
  return QStringList() << "LIST_PART_BG"
                       << "LIST_PART_SCROLLBAR"
                       << "LIST_PART_SCROLLABLE"
                       << "LIST_PART_EDGE_FLASH";
}

lv_style_t *LVGLList::style(lv_obj_t *obj, lv_obj_part_t part) const {
  return lv_obj_get_local_style(obj, part);
}

void LVGLList::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const {
  lv_obj_add_style(obj, LV_BTN_PART_MAIN, style);
}

void LVGLList::addStyle(lv_obj_t *obj, lv_style_t *style,
                        lv_obj_part_t part) const {
  lv_obj_add_style(obj, part, style);
}

void LVGLList::initStateStyles() {
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
