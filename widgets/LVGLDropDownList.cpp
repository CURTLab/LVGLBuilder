#include "LVGLDropDownList.h"

#include <QDebug>
#include <QIcon>

#include "core/LVGLObject.h"

class LVGLPropertyDropdownDir : public LVGLPropertyEnum {
 public:
  LVGLPropertyDropdownDir()
      : LVGLPropertyEnum(QStringList() << "Down"
                                       << "Up"
                                       << "Left"
                                       << "Right"),
        m_values({"LV_DROPDOWN_DIR_DOWN", "LV_DROPDOWN_DIR_UP",
                  "LV_DROPDOWN_DIR_LEFT", "LV_DROPDOWN_DIR_RIGHT"}) {}

  QString name() const { return "Dir"; }

  QStringList function(LVGLObject *obj) const {
    if (get(obj) == LV_LABEL_ALIGN_LEFT) return QStringList();
    return QStringList() << QString("lv_dropdown_set_dir(%1, %2);")
                                .arg(obj->codeName())
                                .arg(m_values.at(get(obj)));
  }

 protected:
  int get(LVGLObject *obj) const { return lv_dropdown_get_dir(obj->obj()); }
  void set(LVGLObject *obj, int index) {
    lv_dropdown_set_dir(obj->obj(), index & 0xff);
  }

  QStringList m_values;
};

class LVGLPropertyDropdownOptions : public LVGLPropertyStringPlus {
 public:
  QString name() const { return "Options"; }

  QStringList function(LVGLObject *obj) const {
    QString tmp = get(obj);
    QStringList list;
    if (!tmp.isEmpty()) {
      QString str;
      for (auto c : tmp) {
        if (c != '\n')
          str += c;
        else
          str += "\\n";
      }
      list << QString("lv_dropdown_set_options(%1,\"%2\");")
                  .arg(obj->codeName())
                  .arg(str);
    }
    return list;
  }

 protected:
  QString get(LVGLObject *obj) const {
    return lv_dropdown_get_options(obj->obj());
  }
  void set(LVGLObject *obj, QString string) {
    lv_dropdown_set_options(obj->obj(), qUtf8Printable(string));
  }
};

class LVGLPropertyDropdownArrow : public LVGLPropertyEnum {
 public:
  LVGLPropertyDropdownArrow()
      : LVGLPropertyEnum(QStringList() << "Down"
                                       << "Up"
                                       << "Left"
                                       << "Right"
                                       << "None"),
        m_values({"LV_SYMBOL_DOWN", "LV_SYMBOL_UP", "LV_SYMBOL_LEFT",
                  "LV_SYMBOL_RIGHT"}) {}

  QString name() const { return "Arrow"; }

  QStringList function(LVGLObject *obj) const {
    if (get(obj) == 0) return QStringList();
    return QStringList() << QString("lv_dropdown_set_symbol(%1, %2);")
                                .arg(obj->codeName())
                                .arg(m_values.at(get(obj)));
  }

 protected:
  int get(LVGLObject *obj) const {
    auto arrow = lv_dropdown_get_symbol(obj->obj());
    QByteArray a(arrow);
    QByteArray b(LV_SYMBOL_DOWN);
    QByteArray c(LV_SYMBOL_UP);
    QByteArray d(LV_SYMBOL_LEFT);
    QByteArray e(LV_SYMBOL_RIGHT);

    if (a == b)
      return 0;
    else if (a == c)
      return 1;
    else if (a == d)
      return 2;
    else if (a == e)
      return 3;
    else
      return 4;
  }

  void set(LVGLObject *obj, int index) {
    auto arrow = LV_SYMBOL_DOWN;
    switch (index) {
      case 0:
        arrow = LV_SYMBOL_DOWN;
        break;
      case 1:
        arrow = LV_SYMBOL_UP;
        break;
      case 2:
        arrow = LV_SYMBOL_LEFT;
        break;
      case 3:
        arrow = LV_SYMBOL_RIGHT;
        break;
      case 4:
        arrow = NULL;
      default:
        arrow = LV_SYMBOL_DOWN;
    }

    lv_dropdown_set_symbol(obj->obj(), arrow);
  }

  QStringList m_values;
};

class LVGLPropertyDropdownMaxHeight : public LVGLPropertyInt {
 public:
  LVGLPropertyDropdownMaxHeight() : LVGLPropertyInt(0, UINT16_MAX, "") {}

  QString name() const { return "Max height"; }

  QStringList function(LVGLObject *obj) const {
    lv_dropdown_open(obj->obj());
    m_value = lv_dropdown_get_max_height(obj->obj());
    lv_dropdown_close(obj->obj());
    return QStringList() << QString("lv_dropdown_set_max_height(%1,%2);")
                                .arg(obj->codeName())
                                .arg(m_value);
  }

 protected:
  int get(LVGLObject *obj) const {
    return lv_dropdown_get_max_height(obj->obj());
  }
  void set(LVGLObject *obj, int value) {
    lv_dropdown_set_max_height(obj->obj(), static_cast<uint16_t>(value));
  }

 private:
  mutable uint16_t m_value;
};

class LVGLPropertyDPOpen : public LVGLPropertyBool {
 public:
  LVGLPropertyDPOpen() : m_bool(false) {}

  QString name() const { return "Open"; }

  QStringList function(LVGLObject *obj) const {
    QStringList list;
    lv_dropdown_ext_t *ext =
        (lv_dropdown_ext_t *)lv_obj_get_ext_attr(obj->obj());
    if (ext->page != NULL)
      list << QString("lv_dropdown_open(%1);").arg(obj->codeName());
    return list;
  }

 protected:
  bool get(LVGLObject *obj) const {
    lv_dropdown_ext_t *ext =
        (lv_dropdown_ext_t *)lv_obj_get_ext_attr(obj->obj());
    return ext->page == NULL ? false : true;
  }
  void set(LVGLObject *obj, bool boolean) {
    m_bool = boolean;
    if (m_bool)
      lv_dropdown_open(obj->obj());
    else
      lv_dropdown_close(obj->obj());
  }
  bool m_bool;
};

LVGLDropDownList::LVGLDropDownList() {
  initStateStyles();
  m_parts << LV_DROPDOWN_PART_MAIN << LV_DROPDOWN_PART_LIST
          << LV_DROPDOWN_PART_SCROLLBAR << LV_DROPDOWN_PART_SELECTED;

  m_properties << new LVGLPropertyDropdownOptions;
  m_properties << new LVGLPropertyDropdownDir;
  m_properties << new LVGLPropertyDropdownArrow;
  m_properties << new LVGLPropertyDropdownMaxHeight;
  m_properties << new LVGLPropertyDPOpen;

  m_editableStyles << LVGL::DropdownMAIN;      // LV_DROPDOWN_PART_MAIN
  m_editableStyles << LVGL::DropdownLIST;      // LV_DROPDOWN_PART_LIST
  m_editableStyles << LVGL::DropdownDCRLBAR;   // LV_DROPDOWN_PART_SCROLLBAR
  m_editableStyles << LVGL::DropdownSELECTED;  // LV_DROPDOWN_PART_SELECTED
}

QString LVGLDropDownList::name() const { return "Dropdown"; }

QString LVGLDropDownList::className() const { return "lv_dropdown"; }

LVGLWidget::Type LVGLDropDownList::type() const { return DropDownList; }

QIcon LVGLDropDownList::icon() const { return QIcon(); }

lv_obj_t *LVGLDropDownList::newObject(lv_obj_t *parent) const {
  lv_obj_t *obj = lv_dropdown_create(parent, nullptr);
  lv_dropdown_open(obj);
  return obj;
}

QSize LVGLDropDownList::minimumSize() const { return QSize(100, 35); }

QStringList LVGLDropDownList::styles() const {
  return QStringList() << "DROPDOWN_PART_MAIN"
                       << "DROPDOWN_PART_LIST"
                       << "DROPDOWN_PART_SCROLLBAR"
                       << "DROPDOWN_PART_SELECTED";
}

lv_style_t *LVGLDropDownList::style(lv_obj_t *obj, lv_obj_part_t part) const {
  return lv_obj_get_local_style(obj, part);
}

void LVGLDropDownList::setStyle(lv_obj_t *obj, int type,
                                lv_style_t *style) const {
  lv_obj_add_style(obj, LV_BTN_PART_MAIN, style);
}

void LVGLDropDownList::addStyle(lv_obj_t *obj, lv_style_t *style,
                                lv_obj_part_t part) const {
  lv_obj_add_style(obj, part, style);
}

void LVGLDropDownList::initStateStyles() {
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
