#include "LVGLContainer.h"

#include <QIcon>

#include "core/LVGLObject.h"

class LVGLPropertyContainerLayout : public LVGLPropertyEnum {
 public:
  LVGLPropertyContainerLayout()
      : LVGLPropertyEnum({"Off", "Center", "Left-justified column",
                          "Centered column", "Right-justified column",
                          "Top-justified row", "Centered row",
                          "Bottom-justified row", "Pretty", "Grid"}),
        m_values({"LV_LAYOUT_OFF", "LV_LAYOUT_CENTER", "LV_LAYOUT_COL_L",
                  "LV_LAYOUT_COL_M", "LV_LAYOUT_COL_R", "LV_LAYOUT_ROW_T",
                  "LV_LAYOUT_ROW_M", "LV_LAYOUT_ROW_B", "LV_LAYOUT_PRETTY",
                  "LV_LAYOUT_GRID"}) {}

  QString name() const { return "Layout"; }

  QStringList function(LVGLObject *obj) const {
    if (get(obj) != LV_BTN_STATE_RELEASED)
      return QStringList() << QString("lv_cont_set_layout(%1, %2);")
                                  .arg(obj->codeName())
                                  .arg(m_values.at(get(obj)));
    return QStringList();
  }

 protected:
  int get(LVGLObject *obj) const { return lv_cont_get_layout(obj->obj()); }
  void set(LVGLObject *obj, int index) {
    lv_cont_set_layout(obj->obj(), index & 0xff);
  }

  QStringList m_values;
};

LVGLContainer::LVGLContainer() {
  initStateStyles();
  m_parts << LV_CONT_PART_MAIN;
  m_properties << new LVGLPropertyContainerLayout;

  m_editableStyles << LVGL::Container;  // LV_CONT_PART_MAIN
}

QString LVGLContainer::name() const { return "Container"; }

QString LVGLContainer::className() const { return "lv_cont"; }

LVGLWidget::Type LVGLContainer::type() const { return Container; }

QIcon LVGLContainer::icon() const { return QIcon(); }

lv_obj_t *LVGLContainer::newObject(lv_obj_t *parent) const {
  lv_obj_t *obj = lv_cont_create(parent, nullptr);
  return obj;
}

QSize LVGLContainer::minimumSize() const { return QSize(100, 100); }

QStringList LVGLContainer::styles() const {
  return QStringList() << "CONT_PART_MAIN";
}

lv_style_t *LVGLContainer::style(lv_obj_t *obj, lv_obj_part_t part) const {
  return lv_obj_get_local_style(obj, part);
}

void LVGLContainer::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const {
  lv_obj_add_style(obj, LV_BTN_PART_MAIN, style);
}

void LVGLContainer::addStyle(lv_obj_t *obj, lv_style_t *style,
                             lv_obj_part_t part) const {
  lv_obj_add_style(obj, part, style);
}

void LVGLContainer::initStateStyles() {
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
