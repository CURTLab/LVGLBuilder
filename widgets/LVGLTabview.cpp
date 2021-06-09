#include "LVGLTabview.h"

#include <QIcon>

#include "core/LVGLCore.h"
#include "core/LVGLObject.h"
#include "properties/LVGLPropertyAnyFunc.h"
#include "properties/LVGLPropertyTextList.h"

class LVGLPropertyTabBtnPos : public LVGLPropertyEnum {
 public:
  LVGLPropertyTabBtnPos()
      : LVGLPropertyEnum({QObject::tr("None"), QObject::tr("Top"),
                          QObject::tr("Bottom"), QObject::tr("Left"),
                          QObject::tr("Right")}),
        m_values({"LV_TABVIEW_TAB_POS_NONE", "LV_TABVIEW_TAB_POS_TOP",
                  "LV_TABVIEW_TAB_POS_BOTTOM", "LV_TABVIEW_TAB_POS_LEFT",
                  "LV_TABVIEW_TAB_POS_RIGHT"}) {}

  QString name() const { return QObject::tr("Tab position"); }
  QString codename() const { return "Tab position"; }

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

class LVGLPropertyTabs : public LVGLPropertyAnyFunc {
 public:
  LVGLPropertyTabs(const AnyFuncColType arr[], int size)
      : LVGLPropertyAnyFunc(arr, size) {}
  QString name() const { return QObject::tr("Add Tabs"); }
  QString codename() const { return "Add Tabs"; }

 protected:
  QStringList get(LVGLObject *obj) const {
    if (!m_list.isEmpty() && m_list[0] != "Empty list") return m_list;
    return QStringList();
  }
  void set(LVGLObject *obj, QStringList list) {
    m_list = list;
    for (int i = 0; i < m_list.size(); ++i) {
      QStringList strlist = m_list[i].split('@');
      int index = strlist[0].toInt();
      m_tabNames[index] = strlist[1];
    }

    if (!m_tabNames.isEmpty()) {
      if (m_result.size() > m_tabNames.size()) {
      } else {
        for (int i = 1; i <= m_tabNames.size(); ++i) {
          auto byte = m_tabNames[i].toUtf8();
          char *name = new char[byte.size() + 1];
          name[byte.size()] = '\0';
          strcpy(name, byte.data());
          if (m_result.contains(i)) {
            if (m_resultstr[i] != QString(name))
              lv_tabview_set_tab_name(obj->obj(), i - 1, name);
          } else {
            m_result.insert(i);
            m_resultstr[i] = QString(name);
            lv_obj_t *page = lv_tabview_add_tab(obj->obj(), name);
            lvgl.addObject(
                new LVGLObject(page, lvgl.widget("lv_page"), obj, false, i));
          }
        }
      }
    }
  }

 private:
  QStringList m_list;
  QMap<int, QString> m_tabNames;
  QSet<int> m_result;
  QMap<int, QString> m_resultstr;
};

class LVGLPropertyTabCurrent : public LVGLPropertyInt {
 public:
  LVGLPropertyTabCurrent() : LVGLPropertyInt(0, UINT16_MAX) {}

  QString name() const { return QObject::tr("Current tab"); }
  QString codename() const { return "Current tab"; }

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

  QString name() const { return QObject::tr("Animation time"); }
  QString codename() const { return "Animation time"; }

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
      : LVGLPropertyEnum(QStringList()
                         << QObject::tr("Off") << QObject::tr("On")
                         << QObject::tr("Drag") << QObject::tr("Auto")),
        m_values({"LV_SB_MODE_OFF", "LV_SB_MODE_ON", "LV_SB_MODE_DRAG",
                  "LV_SB_MODE_AUTO"}) {}

  QString name() const { return QObject::tr("Scrollbars"); }
  QString codename() const { return "Scrollbars"; }

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

class LVGLPropertyTabWidthZoom : public LVGLPropertyInt {
 public:
  LVGLPropertyTabWidthZoom() : LVGLPropertyInt(0, UINT16_MAX) {}

  QString name() const { return QObject::tr("Tab Width Zoom"); }
  QString codename() const { return "Tab Width Zoom"; }

  QStringList function(LVGLObject *obj) const { return QStringList(); }

 protected:
  int get(LVGLObject *obj) const {
    return lv_tabview_get_tab_width_zoom(obj->obj());
  }
  void set(LVGLObject *obj, int value) {
    lv_tabview_set_tab_width_zoom(obj->obj(), value);
  }
};

class LVGLPropertyTabViewStyle : public LVGLPropertyEnum {
 public:
  LVGLPropertyTabViewStyle()
      : LVGLPropertyEnum({QObject::tr("Custome Style"),
                          QObject::tr("Default Style"), QObject::tr("Style1"),
                          QObject::tr("Style2"), QObject::tr("Style3"),
                          QObject::tr("Style4"), QObject::tr("Style5"),
                          QObject::tr("Style6")}) {}

  QString name() const { return QObject::tr("Style"); }
  QString codename() const { return "Style"; }

  QStringList function(LVGLObject *obj) const {
    Q_UNUSED(obj)
    return QStringList();
  }

 protected:
  int get(LVGLObject *obj) const {
    Q_UNUSED(obj)
    return 0;
  }
  void set(LVGLObject *obj, int index) {
    if (index != 0) {
      lv_obj_set_style_local_radius(obj->obj(), 0, 0, 0);
      lv_obj_set_style_local_radius(obj->obj(), LV_TABVIEW_PART_TAB_BG, 0, 0);

      lv_obj_set_style_local_radius(obj->obj(), LV_TABVIEW_PART_TAB_BTN, 0, 0);
      lv_obj_set_style_local_radius(obj->obj(), LV_TABVIEW_PART_TAB_BTN,
                                    LV_STATE_CHECKED, 0);

      lv_obj_set_style_local_bg_opa(obj->obj(), 0, 0, 255);
      lv_obj_set_style_local_bg_opa(obj->obj(), LV_TABVIEW_PART_INDIC, 0, 255);

      lv_obj_set_style_local_text_color(obj->obj(), LV_TABVIEW_PART_TAB_BTN, 0,
                                        lv_color_hex(0x3b3e42));
      lv_obj_set_style_local_text_color(obj->obj(), LV_TABVIEW_PART_TAB_BTN,
                                        LV_STATE_CHECKED,
                                        lv_color_hex(0x3b3e42));

      lv_obj_set_style_local_bg_color(obj->obj(), LV_TABVIEW_PART_TAB_BTN, 0,
                                      lv_color_hex(0xffffff));
      lv_obj_set_style_local_bg_color(obj->obj(), LV_TABVIEW_PART_TAB_BTN,
                                      LV_STATE_CHECKED, lv_color_hex(0xffffff));

      lv_obj_set_style_local_bg_opa(obj->obj(), LV_TABVIEW_PART_TAB_BTN, 0, 0);
      lv_obj_set_style_local_bg_opa(obj->obj(), LV_TABVIEW_PART_TAB_BTN,
                                    LV_STATE_CHECKED, 0);

      lv_obj_set_style_local_pad_top(obj->obj(), LV_TABVIEW_PART_TAB_BG, 0, 6);
      lv_obj_set_style_local_pad_bottom(obj->obj(), LV_TABVIEW_PART_TAB_BG, 0,
                                        0);
      lv_obj_set_style_local_pad_left(obj->obj(), LV_TABVIEW_PART_TAB_BG, 0, 6);
      lv_obj_set_style_local_pad_right(obj->obj(), LV_TABVIEW_PART_TAB_BG, 0,
                                       6);

      lv_obj_set_style_local_border_color(obj->obj(), LV_TABVIEW_PART_TAB_BTN,
                                          0, lv_color_hex(0x00000));
      lv_obj_set_style_local_border_color(obj->obj(), LV_TABVIEW_PART_TAB_BTN,
                                          LV_STATE_CHECKED,
                                          lv_color_hex(0x00000));
      lv_obj_set_style_local_border_width(obj->obj(), LV_TABVIEW_PART_TAB_BTN,
                                          0, 0);
      lv_obj_set_style_local_border_width(obj->obj(), LV_TABVIEW_PART_TAB_BTN,
                                          LV_STATE_CHECKED, 0);
      lv_obj_set_style_local_border_color(obj->obj(), LV_TABVIEW_PART_TAB_BG, 0,
                                          lv_color_hex(0x000000));
      lv_obj_set_style_local_border_width(obj->obj(), LV_TABVIEW_PART_TAB_BG, 0,
                                          0);
    }
    switch (index) {
      case 1: {
      } break;
      case 2: {
        lv_obj_set_style_local_radius(obj->obj(), LV_TABVIEW_PART_TAB_BTN, 0,
                                      32767);
        lv_obj_set_style_local_radius(obj->obj(), LV_TABVIEW_PART_TAB_BTN,
                                      LV_STATE_CHECKED, 32767);
        lv_obj_set_style_local_bg_opa(obj->obj(), 0, 0, 0);
        lv_obj_set_style_local_bg_opa(obj->obj(), LV_TABVIEW_PART_INDIC, 0, 0);
        lv_obj_set_style_local_text_color(obj->obj(), LV_TABVIEW_PART_TAB_BTN,
                                          LV_STATE_CHECKED,
                                          lv_color_hex(0xffffff));
        lv_obj_set_style_local_bg_color(obj->obj(), LV_TABVIEW_PART_TAB_BTN, 0,
                                        lv_color_hex(0xf2f2f2));
        lv_obj_set_style_local_bg_color(obj->obj(), LV_TABVIEW_PART_TAB_BTN,
                                        LV_STATE_CHECKED,
                                        lv_color_hex(0x1677ff));

        lv_obj_set_style_local_bg_opa(obj->obj(), LV_TABVIEW_PART_TAB_BTN, 0,
                                      255);
        lv_obj_set_style_local_bg_opa(obj->obj(), LV_TABVIEW_PART_TAB_BTN,
                                      LV_STATE_CHECKED, 255);
        lv_obj_set_style_local_pad_top(obj->obj(), LV_TABVIEW_PART_TAB_BG, 0,
                                       3);
        lv_obj_set_style_local_pad_bottom(obj->obj(), LV_TABVIEW_PART_TAB_BG, 0,
                                          3);
        lv_obj_set_style_local_pad_left(obj->obj(), LV_TABVIEW_PART_TAB_BG, 0,
                                        8);
        lv_obj_set_style_local_pad_right(obj->obj(), LV_TABVIEW_PART_TAB_BG, 0,
                                         8);
      } break;
      case 3: {
        lv_obj_set_style_local_radius(obj->obj(), LV_TABVIEW_PART_TAB_BG, 0, 8);
        lv_obj_set_style_local_radius(obj->obj(), LV_TABVIEW_PART_TAB_BTN, 0,
                                      8);
        lv_obj_set_style_local_radius(obj->obj(), LV_TABVIEW_PART_TAB_BTN,
                                      LV_STATE_CHECKED, 8);
        lv_obj_set_style_local_bg_opa(obj->obj(), 0, 0, 0);
        lv_obj_set_style_local_bg_opa(obj->obj(), LV_TABVIEW_PART_INDIC, 0, 0);
        lv_obj_set_style_local_text_color(obj->obj(), LV_TABVIEW_PART_TAB_BTN,
                                          LV_STATE_CHECKED,
                                          lv_color_hex(0xffffff));
        lv_obj_set_style_local_bg_color(obj->obj(), LV_TABVIEW_PART_TAB_BTN,
                                        LV_STATE_CHECKED,
                                        lv_color_hex(0x1677ff));
        lv_obj_set_style_local_bg_opa(obj->obj(), LV_TABVIEW_PART_TAB_BTN,
                                      LV_STATE_CHECKED, 255);
        lv_obj_set_style_local_pad_top(obj->obj(), LV_TABVIEW_PART_TAB_BG, 0,
                                       3);
        lv_obj_set_style_local_pad_bottom(obj->obj(), LV_TABVIEW_PART_TAB_BG, 0,
                                          3);
        lv_obj_set_style_local_pad_left(obj->obj(), LV_TABVIEW_PART_TAB_BG, 0,
                                        3);
        lv_obj_set_style_local_pad_right(obj->obj(), LV_TABVIEW_PART_TAB_BG, 0,
                                         3);
      } break;
      case 4: {
        lv_obj_set_style_local_radius(obj->obj(), LV_TABVIEW_PART_TAB_BG, 0,
                                      32767);
        lv_obj_set_style_local_radius(obj->obj(), LV_TABVIEW_PART_TAB_BTN, 0,
                                      32767);
        lv_obj_set_style_local_radius(obj->obj(), LV_TABVIEW_PART_TAB_BTN,
                                      LV_STATE_CHECKED, 32767);
        lv_obj_set_style_local_bg_opa(obj->obj(), 0, 0, 0);
        lv_obj_set_style_local_bg_opa(obj->obj(), LV_TABVIEW_PART_INDIC, 0, 0);
        lv_obj_set_style_local_text_color(obj->obj(), LV_TABVIEW_PART_TAB_BTN,
                                          LV_STATE_CHECKED,
                                          lv_color_hex(0xffffff));
        lv_obj_set_style_local_bg_color(obj->obj(), LV_TABVIEW_PART_TAB_BTN,
                                        LV_STATE_CHECKED,
                                        lv_color_hex(0x1677ff));
        lv_obj_set_style_local_bg_opa(obj->obj(), LV_TABVIEW_PART_TAB_BTN,
                                      LV_STATE_CHECKED, 255);
        lv_obj_set_style_local_pad_top(obj->obj(), LV_TABVIEW_PART_TAB_BG, 0,
                                       0);
        lv_obj_set_style_local_pad_bottom(obj->obj(), LV_TABVIEW_PART_TAB_BG, 0,
                                          0);
        lv_obj_set_style_local_pad_left(obj->obj(), LV_TABVIEW_PART_TAB_BG, 0,
                                        0);
        lv_obj_set_style_local_pad_right(obj->obj(), LV_TABVIEW_PART_TAB_BG, 0,
                                         0);
      } break;
      case 5: {
        lv_obj_set_style_local_radius(obj->obj(), LV_TABVIEW_PART_TAB_BG, 0,
                                      32767);
        lv_obj_set_style_local_radius(obj->obj(), LV_TABVIEW_PART_TAB_BTN, 0,
                                      32767);
        lv_obj_set_style_local_radius(obj->obj(), LV_TABVIEW_PART_TAB_BTN,
                                      LV_STATE_CHECKED, 32767);
        lv_obj_set_style_local_bg_opa(obj->obj(), 0, 0, 0);
        lv_obj_set_style_local_bg_opa(obj->obj(), LV_TABVIEW_PART_INDIC, 0, 0);
        lv_obj_set_style_local_bg_opa(obj->obj(), LV_TABVIEW_PART_TAB_BTN,
                                      LV_STATE_CHECKED, 255);
        lv_obj_set_style_local_pad_top(obj->obj(), LV_TABVIEW_PART_TAB_BG, 0,
                                       0);
        lv_obj_set_style_local_pad_bottom(obj->obj(), LV_TABVIEW_PART_TAB_BG, 0,
                                          0);
        lv_obj_set_style_local_pad_left(obj->obj(), LV_TABVIEW_PART_TAB_BG, 0,
                                        0);
        lv_obj_set_style_local_pad_right(obj->obj(), LV_TABVIEW_PART_TAB_BG, 0,
                                         0);
        lv_obj_set_style_local_border_color(obj->obj(), LV_TABVIEW_PART_TAB_BTN,
                                            LV_STATE_CHECKED,
                                            lv_color_hex(0x1677ff));
        lv_obj_set_style_local_border_width(obj->obj(), LV_TABVIEW_PART_TAB_BTN,
                                            LV_STATE_CHECKED, 2);
      } break;
      case 6: {
        lv_obj_set_style_local_radius(obj->obj(), LV_TABVIEW_PART_TAB_BG, 0,
                                      32767);
        lv_obj_set_style_local_radius(obj->obj(), LV_TABVIEW_PART_TAB_BTN, 0,
                                      32767);
        lv_obj_set_style_local_radius(obj->obj(), LV_TABVIEW_PART_TAB_BTN,
                                      LV_STATE_CHECKED, 32767);
        lv_obj_set_style_local_bg_opa(obj->obj(), 0, 0, 0);
        lv_obj_set_style_local_bg_opa(obj->obj(), LV_TABVIEW_PART_INDIC, 0, 0);
        lv_obj_set_style_local_text_color(obj->obj(), LV_TABVIEW_PART_TAB_BTN,
                                          LV_STATE_CHECKED,
                                          lv_color_hex(0xffffff));
        lv_obj_set_style_local_bg_color(obj->obj(), LV_TABVIEW_PART_TAB_BTN,
                                        LV_STATE_CHECKED,
                                        lv_color_hex(0x1677ff));
        lv_obj_set_style_local_bg_opa(obj->obj(), LV_TABVIEW_PART_TAB_BTN,
                                      LV_STATE_CHECKED, 255);
        lv_obj_set_style_local_pad_top(obj->obj(), LV_TABVIEW_PART_TAB_BG, 0,
                                       0);
        lv_obj_set_style_local_pad_bottom(obj->obj(), LV_TABVIEW_PART_TAB_BG, 0,
                                          0);
        lv_obj_set_style_local_pad_left(obj->obj(), LV_TABVIEW_PART_TAB_BG, 0,
                                        0);
        lv_obj_set_style_local_pad_right(obj->obj(), LV_TABVIEW_PART_TAB_BG, 0,
                                         0);
        lv_obj_set_style_local_border_color(obj->obj(), LV_TABVIEW_PART_TAB_BG,
                                            0, lv_color_hex(0x1677ff));
        lv_obj_set_style_local_border_width(obj->obj(), LV_TABVIEW_PART_TAB_BG,
                                            0, 2);
      } break;
      case 7: {
        lv_obj_set_style_local_bg_opa(obj->obj(), 0, 0, 0);
        lv_obj_set_style_local_bg_opa(obj->obj(), LV_TABVIEW_PART_INDIC, 0, 0);
        lv_obj_set_style_local_text_color(obj->obj(), LV_TABVIEW_PART_TAB_BTN,
                                          LV_STATE_CHECKED,
                                          lv_color_hex(0xffffff));
        lv_obj_set_style_local_bg_color(obj->obj(), LV_TABVIEW_PART_TAB_BTN,
                                        LV_STATE_CHECKED,
                                        lv_color_hex(0x1677ff));
        lv_obj_set_style_local_bg_opa(obj->obj(), LV_TABVIEW_PART_TAB_BTN,
                                      LV_STATE_CHECKED, 255);
        lv_obj_set_style_local_pad_top(obj->obj(), LV_TABVIEW_PART_TAB_BG, 0,
                                       0);
        lv_obj_set_style_local_pad_bottom(obj->obj(), LV_TABVIEW_PART_TAB_BG, 0,
                                          0);
        lv_obj_set_style_local_pad_left(obj->obj(), LV_TABVIEW_PART_TAB_BG, 0,
                                        0);
        lv_obj_set_style_local_pad_right(obj->obj(), LV_TABVIEW_PART_TAB_BG, 0,
                                         0);
        lv_obj_set_style_local_border_color(obj->obj(), LV_TABVIEW_PART_TAB_BG,
                                            0, lv_color_hex(0x1677ff));
        lv_obj_set_style_local_border_width(obj->obj(), LV_TABVIEW_PART_TAB_BG,
                                            0, 2);
      } break;
    }
  }
};

LVGLTabview::LVGLTabview() {
  initStateStyles();
  m_parts << LV_TABVIEW_PART_BG << LV_TABVIEW_PART_BG_SCROLLABLE
          << LV_TABVIEW_PART_TAB_BG << LV_TABVIEW_PART_TAB_BTN
          << LV_TABVIEW_PART_INDIC;
  m_properties << new LVGLPropertyTabBtnPos;
  static AnyFuncColType arr[] = {e_Seqlabel, e_QLineEdit};
  m_properties << new LVGLPropertyTabs(arr, 2);

  m_properties << new LVGLPropertyTabCurrent;
  m_properties << new LVGLPropertyTabScrollbars;
  m_properties << new LVGLPropertyTabWidthZoom;
  m_properties << new LVGLPropertyTabViewStyle;

  m_editableStyles << LVGL::TabBG;      // LV_TABVIEW_PART_BG
  m_editableStyles << LVGL::TabBGScro;  // LV_TABVIEW_PART_BG_SCROLLABLE
  m_editableStyles << LVGL::TabTaBBG;   // LV_TABVIEW_PART_TAB_BG
  m_editableStyles << LVGL::TabTabBTN;  // LV_TABVIEW_PART_TAB_BTN
  m_editableStyles << LVGL::TabINDIC;   // LV_TABVIEW_PART_INDIC
}

QString LVGLTabview::name() const { return QObject::tr("Tabview"); }

QString LVGLTabview::className() const { return "lv_tabview"; }

LVGLWidget::Type LVGLTabview::type() const { return TabView; }

QIcon LVGLTabview::icon() const { return QIcon(); }

lv_obj_t *LVGLTabview::newObject(lv_obj_t *parent) const {
  lv_obj_t *obj = lv_tabview_create(parent, nullptr);
  lv_obj_set_style_local_bg_color(obj, 0, 0, lv_color_hex(0xeaeff3));
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
    if (i == 0) {
      lv_style_set_bg_color(de, 0, lv_color_hex(0xeaeff3));
    }
    QList<lv_style_t *> stateStyles;
    stateStyles << de << ch << fo << ed << ho << pr << di;
    m_partsStyles[i] = stateStyles;
  }
}
