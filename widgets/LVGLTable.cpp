#include "LVGLTable.h"

#include <QIcon>

#include "core/LVGLObject.h"

LVGLTable::LVGLTable() {
  initStateStyles();
  m_parts << LV_TABLE_PART_BG << LV_TABLE_PART_CELL1 << LV_TABLE_PART_CELL2
          << LV_TABLE_PART_CELL3 << LV_TABLE_PART_CELL4;

  m_editableStyles << LVGL::TableBG;    // LV_TABLE_PART_BG
  m_editableStyles << LVGL::TableCELL;  // LV_TABLE_PART_CELL1
  m_editableStyles << LVGL::TableCELL;  // LV_TABLE_PART_CELL2
  m_editableStyles << LVGL::TableCELL;  // LV_TABLE_PART_CELL3
  m_editableStyles << LVGL::TableCELL;  // LV_TABLE_PART_CELL4
}

QString LVGLTable::name() const { return "Table"; }

QString LVGLTable::className() const { return "lv_table"; }

LVGLWidget::Type LVGLTable::type() const { return Table; }

QIcon LVGLTable::icon() const { return QIcon(); }

lv_obj_t *LVGLTable::newObject(lv_obj_t *parent) const {
  lv_obj_t *obj = lv_table_create(parent, nullptr);
  lv_table_set_col_cnt(obj, 2);
  lv_table_set_row_cnt(obj, 4);
  lv_table_set_cell_value(obj, 0, 0, "Col1");
  lv_table_set_cell_value(obj, 0, 1, "Col2");
  return obj;
}

QSize LVGLTable::minimumSize() const { return QSize(150, 150); }

QStringList LVGLTable::styles() const {
  return QStringList() << "LV_TABLE_PART_BG"
                       << "LV_TABLE_PART_CELL1"
                       << "LV_TABLE_PART_CELL2"
                       << "LV_TABLE_PART_CELL3"
                       << "LV_TABLE_PART_CELL4;";
}

lv_style_t *LVGLTable::style(lv_obj_t *obj, lv_obj_part_t part) const {
  return lv_obj_get_local_style(obj, part);
}

void LVGLTable::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const {
  lv_obj_add_style(obj, LV_BTN_PART_MAIN, style);
}

void LVGLTable::addStyle(lv_obj_t *obj, lv_style_t *style,
                         lv_obj_part_t part) const {
  lv_obj_add_style(obj, part, style);
}

void LVGLTable::initStateStyles() {
  for (int i = 0; i < 5; ++i) {
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
