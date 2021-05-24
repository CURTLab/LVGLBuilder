#include "LVGLTextArea.h"

#include <QIcon>

#include "core/LVGLObject.h"

class LVGLPropertyTAText : public LVGLPropertyStringPlus {
 public:
  QString name() const { return "Text"; }

  QStringList function(LVGLObject *obj) const {
    return QStringList() << QString("lv_textarea_set_text(%1, \"%2\");")
                                .arg(obj->codeName())
                                .arg(get(obj));
  }

 protected:
  QString get(LVGLObject *obj) const {
    return lv_textarea_get_text(obj->obj());
  }
  void set(LVGLObject *obj, QString string) {
    lv_textarea_set_text(obj->obj(), qUtf8Printable(string));
  }
};

class LVGLPropertyTAPlaceholder : public LVGLPropertyStringPlus {
 public:
  QString name() const { return "Placeholder"; }

  QStringList function(LVGLObject *obj) const {
    return QStringList() << QString(
                                "lv_textarea_set_placeholder_text(%1, \"%2\");")
                                .arg(obj->codeName())
                                .arg(get(obj));
  }

 protected:
  QString get(LVGLObject *obj) const {
    return lv_textarea_get_placeholder_text(obj->obj());
  }
  void set(LVGLObject *obj, QString string) {
    lv_textarea_set_placeholder_text(obj->obj(), qUtf8Printable(string));
  }
};

class LVGLPropertyTACursorPos : public LVGLPropertyInt {
 public:
  LVGLPropertyTACursorPos() : LVGLPropertyInt(0, LV_TEXTAREA_CURSOR_LAST) {}

  QString name() const { return "Cursor position"; }

 protected:
  int get(LVGLObject *obj) const {
    return lv_textarea_get_cursor_pos(obj->obj());
  }
  void set(LVGLObject *obj, int value) {
    lv_textarea_set_cursor_pos(obj->obj(), static_cast<int16_t>(value));
  }
};

class LVGLPropertyTACursorBlinkTime : public LVGLPropertyInt {
 public:
  LVGLPropertyTACursorBlinkTime() : LVGLPropertyInt(0, UINT16_MAX) {}

  QString name() const { return "Cursor blink time"; }

  QStringList function(LVGLObject *obj) const {
    return QStringList() << QString(
                                "lv_textarea_set_cursor_blink_time(%1, %2);")
                                .arg(obj->codeName())
                                .arg(get(obj));
  }

 protected:
  int get(LVGLObject *obj) const {
    return lv_textarea_get_cursor_blink_time(obj->obj());
  }
  void set(LVGLObject *obj, int index) {
    lv_textarea_set_cursor_blink_time(obj->obj(), index & 0xff);
  }
};

class LVGLPropertyTAOneLineMode : public LVGLPropertyBool {
 public:
  QString name() const { return "One line mode"; }

  QStringList function(LVGLObject *obj) const {
    return QStringList() << QString("lv_textarea_set_one_line(%1, %2);")
                                .arg(obj->codeName())
                                .arg(QVariant(get(obj)).toString());
  }

 protected:
  bool get(LVGLObject *obj) const {
    return lv_textarea_get_one_line(obj->obj());
  }
  void set(LVGLObject *obj, bool statue) {
    lv_textarea_set_one_line(obj->obj(), statue);
  }
};

class LVGLPropertyTAPasswordMode : public LVGLPropertyBool {
 public:
  QString name() const { return "Password mode"; }

  QStringList function(LVGLObject *obj) const {
    return QStringList() << QString("lv_textarea_set_pwd_mode(%1, %2);")
                                .arg(obj->codeName())
                                .arg(QVariant(get(obj)).toString());
  }

 protected:
  bool get(LVGLObject *obj) const {
    return lv_textarea_get_pwd_mode(obj->obj());
  }
  void set(LVGLObject *obj, bool statue) {
    lv_textarea_set_pwd_mode(obj->obj(), statue);
  }
};

class LVGLPropertyTAMaxLen : public LVGLPropertyInt {
 public:
  LVGLPropertyTAMaxLen() : LVGLPropertyInt(0, LV_TEXTAREA_CURSOR_LAST - 1) {}

  QStringList function(LVGLObject *obj) const {
    return QStringList() << QString("lv_textarea_set_max_length(%1, %2);")
                                .arg(obj->codeName())
                                .arg(get(obj));
  }

  QString name() const { return "Max length"; }

 protected:
  int get(LVGLObject *obj) const {
    return lv_textarea_get_max_length(obj->obj());
  }
  void set(LVGLObject *obj, int index) {
    lv_textarea_set_max_length(obj->obj(), index & 0xff);
  }
};

class LVGLPropertyTATextAlign : public LVGLPropertyEnum {
 public:
  LVGLPropertyTATextAlign()
      : LVGLPropertyEnum(QStringList() << "Left"
                                       << "Center"
                                       << "Right") {}

  QString name() const { return "Text align"; }

 protected:
  int get(LVGLObject *obj) const {
    return lv_label_get_align(lv_textarea_get_label(obj->obj()));
  }
  void set(LVGLObject *obj, int index) {
    lv_textarea_set_text_align(obj->obj(), index & 0xff);
  }
};

class LVGLPropertyTAScrollbars : public LVGLPropertyEnum {
 public:
  LVGLPropertyTAScrollbars()
      : LVGLPropertyEnum(QStringList() << "Off"
                                       << "On"
                                       << "Drag"
                                       << "Auto") {}

  QString name() const { return "Scrollbars"; }

 protected:
  int get(LVGLObject *obj) const {
    return lv_textarea_get_scrollbar_mode(obj->obj()) & 0x3;
  }
  void set(LVGLObject *obj, int index) {
    lv_textarea_set_scrollbar_mode(obj->obj(), index & 0xff);
  }
};

class LVGLPropertyTAScrollPropagation : public LVGLPropertyBool {
 public:
  QString name() const { return "Scroll propagation"; }

  QStringList function(LVGLObject *obj) const {
    return QStringList() << QString(
                                "lv_textarea_set_scroll_propagation(%1, %2);")
                                .arg(obj->codeName())
                                .arg(QVariant(get(obj)).toString());
  }

 protected:
  bool get(LVGLObject *obj) const {
    return lv_textarea_get_scroll_propagation(obj->obj());
  }
  void set(LVGLObject *obj, bool boolean) {
    lv_textarea_set_scroll_propagation(obj->obj(), boolean);
  }
};

class LVGLPropertyTAEdgeFlash : public LVGLPropertyBool {
 public:
  QString name() const { return "Edge flash"; }

  QStringList function(LVGLObject *obj) const {
    return QStringList() << QString("lv_textarea_set_edge_flash(%1, %2);")
                                .arg(obj->codeName())
                                .arg(QVariant(get(obj)).toString());
  }

 protected:
  bool get(LVGLObject *obj) const {
    return lv_textarea_get_edge_flash(obj->obj());
  }
  void set(LVGLObject *obj, bool boolean) {
    lv_textarea_set_edge_flash(obj->obj(), boolean);
  }
};

LVGLTextArea::LVGLTextArea() {
  initStateStyles();
  m_parts << LV_TEXTAREA_PART_BG << LV_TEXTAREA_PART_SCROLLBAR
          << LV_TEXTAREA_PART_EDGE_FLASH << LV_TEXTAREA_PART_CURSOR
          << LV_TEXTAREA_PART_PLACEHOLDER;
  m_properties << new LVGLPropertyTAText;
  m_properties << new LVGLPropertyTAPlaceholder;
  m_properties << new LVGLPropertyTACursorPos;
  //  m_properties << new LVGLPropertyTACursorType;
  m_properties << new LVGLPropertyTACursorBlinkTime;
  m_properties << new LVGLPropertyTAOneLineMode;
  m_properties << new LVGLPropertyTAPasswordMode;
  m_properties << new LVGLPropertyTAMaxLen;
  m_properties << new LVGLPropertyTATextAlign;
  m_properties << new LVGLPropertyTAScrollbars;
  m_properties << new LVGLPropertyTAScrollPropagation;
  m_properties << new LVGLPropertyTAEdgeFlash;

  m_editableStyles << LVGL::PageBG;         // LV_TEXTAREA_PART_BG
  m_editableStyles << LVGL::PageSCROLLBAR;  // LV_TEXTAREA_PART_SCROLLBAR
  m_editableStyles << LVGL::PageEDGEFLASH;  // LV_TEXTAREA_PART_EDGE_FLASH
  m_editableStyles << LVGL::Body;           // LV_TEXTAREA_PART_CURSOR
  m_editableStyles << LVGL::TextPlaceHode;  // LV_TEXTAREA_PART_PLACEHOLDER
}

QString LVGLTextArea::name() const { return "Text area"; }

QString LVGLTextArea::className() const { return "lv_textarea"; }

LVGLWidget::Type LVGLTextArea::type() const { return TextArea; }

QIcon LVGLTextArea::icon() const { return QIcon(); }

lv_obj_t *LVGLTextArea::newObject(lv_obj_t *parent) const {
  lv_obj_t *obj = lv_textarea_create(parent, nullptr);
  return obj;
}

QSize LVGLTextArea::minimumSize() const { return QSize(100, 30); }

QStringList LVGLTextArea::styles() const {
  return QStringList() << "LV_TEXTAREA_PART_BG"
                       << "LV_TEXTAREA_PART_SCROLLBAR"
                       << "LV_TEXTAREA_PART_EDGE_FLASH"
                       << "LV_TEXTAREA_PART_CURSOR"
                       << "LV_TEXTAREA_PART_PLACEHOLDER";
}

lv_style_t *LVGLTextArea::style(lv_obj_t *obj, lv_obj_part_t part) const {
  return lv_obj_get_local_style(obj, part);
}

void LVGLTextArea::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const {
  lv_obj_add_style(obj, LV_BTN_PART_MAIN, style);
}

void LVGLTextArea::addStyle(lv_obj_t *obj, lv_style_t *style,
                            lv_obj_part_t part) const {
  lv_obj_add_style(obj, part, style);
}

void LVGLTextArea::initStateStyles() {
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
