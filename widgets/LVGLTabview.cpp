#include "LVGLTabview.h"

#include <QIcon>

#include "LVGLCore.h"
#include "LVGLObject.h"
#include "properties/LVGLPropertyTextList.h"

class LVGLPropertyTabBtnPos : public LVGLPropertyEnum {
 public:
  LVGLPropertyTabBtnPos()
      : LVGLPropertyEnum({"None", "Top", "Bottom", "Left", "Right"}),
        m_values({"LV_TABVIEW_TAB_POS_NONE", "LV_TABVIEW_TAB_POS_TOP",
                  "LV_TABVIEW_TAB_POS_BOTTOM", "LV_TABVIEW_TAB_POS_LEFT",
                  "LV_TABVIEW_TAB_POS_RIGHT"}) {}

  QString name() const { return "Tab position"; }

  QStringList function(LVGLObject *obj) const {
    if (get(obj) != LV_TABVIEW_TAB_POS_TOP)
      return QStringList() << QString("lv_tabview_set_btns_pos(%1, %2);")
                                  .arg(obj->codeName())
                                  .arg(m_values.at(get(obj)));
    return QStringList();
  }

 protected:
  int get(LVGLObject *obj) const { return lv_tabview_get_btns_pos(obj->obj()); }
  void set(LVGLObject *obj, int index) {
    lv_tabview_set_btns_pos(obj->obj(), index & 0xff);
  }

  QStringList m_values;
};

class LVGLPropertyTabs : public LVGLPropertyTextList {
 public:
  inline LVGLPropertyTabs(LVGLProperty *parent = nullptr)
      : LVGLPropertyTextList(true, parent) {}

  inline QString name() const { return "Add Tabs"; }

 protected:
  inline QStringList get(LVGLObject *obj) const {
    QStringList ret;
    lv_tabview_ext_t *ext =
        reinterpret_cast<lv_tabview_ext_t *>(lv_obj_get_ext_attr(obj->obj()));
    for (uint16_t i = 0; i < ext->tab_cnt; ++i)
      ret << QString(ext->tab_name_ptr[i]);
    return ret;
  }

  inline void set(LVGLObject *obj, QStringList list) {
    lv_tabview_ext_t *ext =
        reinterpret_cast<lv_tabview_ext_t *>(lv_obj_get_ext_attr(obj->obj()));
    // rename
    for (uint16_t i = 0;
         i < qMin(ext->tab_cnt, static_cast<uint16_t>(list.size())); ++i) {
      QByteArray name = list.at(i).toUtf8();
      if (strcmp(ext->tab_name_ptr[i], name.data()) == 0) continue;

      char *name_dm = reinterpret_cast<char *>(lv_mem_alloc(name.size()));
      if (name_dm == nullptr) continue;

      memcpy(name_dm, name, name.size());
      name_dm[name.size()] = '\0';
      ext->tab_name_ptr[i] = name_dm;

      lv_btnmatrix_set_map(ext->btns, ext->tab_name_ptr);
      lv_btnmatrix_set_btn_ctrl(ext->btns, ext->tab_cur,
                                LV_BTNMATRIX_CTRL_NO_REPEAT);
    }

    // add new
    for (uint16_t i = ext->tab_cnt; i < list.size(); ++i) {
      lv_obj_t *page_obj =
          lv_tabview_add_tab(obj->obj(), qUtf8Printable(list.at(i)));
      LVGLObject *page =
          new LVGLObject(page_obj, lvgl->widget("lv_page"), obj, false, i);
      lvgl->addObject(page);
    }
  }
};

class LVGLPropertyTabCurrent : public LVGLPropertyInt {
 public:
  LVGLPropertyTabCurrent() : LVGLPropertyInt(0, UINT16_MAX) {}

  QString name() const { return "Current tab"; }

  QStringList function(LVGLObject *obj) const {
    return QStringList() << QString(
                                "lv_tabview_set_tab_act(%1, %2, LV_ANIM_OFF);")
                                .arg(obj->codeName())
                                .arg(get(obj));
  }

 protected:
  int get(LVGLObject *obj) const { return lv_tabview_get_tab_act(obj->obj()); }
  void set(LVGLObject *obj, int value) {
    lv_tabview_set_tab_act(obj->obj(), static_cast<uint16_t>(value),
                           LV_ANIM_OFF);
  }
};

class LVGLPropertyTabAnimTime : public LVGLPropertyInt {
 public:
  LVGLPropertyTabAnimTime() : LVGLPropertyInt(0, UINT16_MAX) {}

  QString name() const { return "Animation time"; }

  QStringList function(LVGLObject *obj) const {
    return QStringList() << QString("lv_tabview_set_anim_time(%1, %2);")
                                .arg(obj->codeName())
                                .arg(get(obj));
  }

 protected:
  int get(LVGLObject *obj) const {
    return lv_tabview_get_anim_time(obj->obj());
  }
  void set(LVGLObject *obj, int value) {
    lv_tabview_set_anim_time(obj->obj(), static_cast<uint16_t>(value));
  }
};

class LVGLPropertyTabScrollbars : public LVGLPropertyEnum {
 public:
  LVGLPropertyTabScrollbars()
      : LVGLPropertyEnum(QStringList() << "Off"
                                       << "On"
                                       << "Drag"
                                       << "Auto"),
        m_values({"LV_SB_MODE_OFF", "LV_SB_MODE_ON", "LV_SB_MODE_DRAG",
                  "LV_SB_MODE_AUTO"}) {}

  QString name() const { return "Scrollbars"; }

 protected:
  int get(LVGLObject *obj) const {
    lv_scrollbar_mode_t mode = LV_SCROLLBAR_MODE_AUTO;
    for (uint16_t i = 0; i < lv_tabview_get_tab_count(obj->obj()); ++i) {
      lv_obj_t *page = lv_tabview_get_tab(obj->obj(), i);
      mode = lv_page_get_scrollbar_mode(page);
    }
    return mode;
  }
  void set(LVGLObject *obj, int index) {
    for (uint16_t i = 0; i < lv_tabview_get_tab_count(obj->obj()); ++i) {
      lv_obj_t *page = lv_tabview_get_tab(obj->obj(), i);
      lv_page_set_scrollbar_mode(page, index & 0xff);
    }
  }

  QStringList m_values;
};

LVGLTabview::LVGLTabview() {
  initStateStyles();
  m_parts << LV_TABVIEW_PART_BG << LV_TABVIEW_PART_BG_SCROLLABLE
          << LV_TABVIEW_PART_TAB_BG << LV_TABVIEW_PART_TAB_BTN
          << LV_TABVIEW_PART_INDIC;
  m_properties << new LVGLPropertyTabs;
  m_properties << new LVGLPropertyTabBtnPos;
  m_properties << new LVGLPropertyTabCurrent;
  m_properties << new LVGLPropertyTabScrollbars;

  m_editableStyles << LVGL::PageSCRL;     // LV_TABVIEW_PART_BG
  m_editableStyles << LVGL::Button;       // LV_TABVIEW_PART_BG_SCROLLABLE
  m_editableStyles << LVGL::Background;   // LV_TABVIEW_PART_TAB_BG
  m_editableStyles << LVGL::TableTABBTN;  // LV_TABVIEW_PART_TAB_BTN
  m_editableStyles << LVGL::Background;   // LV_TABVIEW_PART_INDIC
}

QString LVGLTabview::name() const { return "Tabview"; }

QString LVGLTabview::className() const { return "lv_tabview"; }

LVGLWidget::Type LVGLTabview::type() const { return TabView; }

QIcon LVGLTabview::icon() const { return QIcon(); }

lv_obj_t *LVGLTabview::newObject(lv_obj_t *parent) const {
  lv_obj_t *obj = lv_tabview_create(parent, nullptr);
  return obj;
}

QSize LVGLTabview::minimumSize() const { return QSize(100, 150); }

QStringList LVGLTabview::styles() const {
  return QStringList() << "LV_TABVIEW_PART_BG"
                       << "LV_TABVIEW_PART_BG_SCROLLABLE"
                       << "LV_TABVIEW_PART_TAB_BG"
                       << "LV_TABVIEW_PART_TAB_BTN"
                       << "LV_TABVIEW_PART_INDIC";
}

lv_style_t *LVGLTabview::style(lv_obj_t *obj, lv_obj_part_t part) const {
  return lv_obj_get_local_style(obj, part);
}

void LVGLTabview::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const {
  lv_obj_add_style(obj, LV_BTN_PART_MAIN, style);
}

void LVGLTabview::addStyle(lv_obj_t *obj, lv_style_t *style,
                           lv_obj_part_t part) const {
  lv_obj_add_style(obj, part, style);
}

void LVGLTabview::initStateStyles() {
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
