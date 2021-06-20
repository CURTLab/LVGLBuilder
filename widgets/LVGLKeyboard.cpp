#include "LVGLKeyboard.h"

#include <QIcon>

#include "core/LVGLObject.h"
#include "properties/LVGLPropertyAssignTextArea.h"

class LVGLPropertyKBMode : public LVGLPropertyEnum {
 public:
  LVGLPropertyKBMode()
      : LVGLPropertyEnum({QObject::tr("Text"), QObject::tr("Number"),
                          QObject::tr("Text Upper")}),
        m_values(
            {"LV_KB_MODE_TEXT", "LV_KB_MODE_NUM", "LV_KB_MODE_TEXT_UPPER"}) {}

  QString name() const { return QObject::tr("Mode"); }
  QString codename() const { return "Mode"; }

  QStringList function(LVGLObject *obj) const {
    return QStringList() << QString("lv_kb_set_mode(%1, %2);")
                                .arg(obj->codeName())
                                .arg(m_values.at(get(obj)));
  }

 protected:
  int get(LVGLObject *obj) const { return lv_keyboard_get_mode(obj->obj()); }
  void set(LVGLObject *obj, int index) {
    lv_keyboard_set_mode(obj->obj(), index & 0xff);
  }

  QStringList m_values;
};

class LVGLPropertyLBCursorManager : public LVGLPropertyBool {
 public:
  QString name() const { return QObject::tr("Cursor Manager"); }
  QString codename() const { return "Cursor Manager"; }

  QStringList function(LVGLObject *obj) const {
    return QStringList() << QString("lv_kb_set_cursor_manage(%1, %2);")
                                .arg(obj->codeName())
                                .arg(QVariant(get(obj)).toString());
  }

 protected:
  bool get(LVGLObject *obj) const {
    return lv_keyboard_get_cursor_manage(obj->obj());
  }
  void set(LVGLObject *obj, bool statue) {
    lv_keyboard_set_cursor_manage(obj->obj(), statue);
  }
};

LVGLKeyboard::LVGLKeyboard() {
  initStateStyles();
  m_parts << LV_KEYBOARD_PART_BG << LV_KEYBOARD_PART_BTN;
  m_properties << new LVGLPropertyKBMode;
  m_properties << new LVGLPropertyAssignTextArea;
  m_properties << new LVGLPropertyLBCursorManager;

  m_editableStyles << LVGL::Background;   // LV_KEYBOARD_PART_BG
  m_editableStyles << LVGL::KeyboardBTN;  // LV_KEYBOARD_PART_BTN
}

QString LVGLKeyboard::name() const { return QObject::tr("Keyboard"); }

QString LVGLKeyboard::codename() const { return "Keyboard"; }

QString LVGLKeyboard::className() const { return "lv_kb"; }

LVGLWidget::Type LVGLKeyboard::type() const { return Keyboard; }

QIcon LVGLKeyboard::icon() const { return QIcon(); }

lv_obj_t *LVGLKeyboard::newObject(lv_obj_t *parent) const {
  lv_obj_t *obj = lv_keyboard_create(parent, nullptr);
  /*static const char *kb_map[] = {
          "q", "w", "e", "r", "t", "z", "u", "i", "o", "p", "\n",
          "a", "s", "d", "f", "g", "h", "j", "k", "l", "\n",
          "ABC", "y", "x", "c", "v", "b", "n", "m", LV_SYMBOL_BACKSPACE, "\n",
          "#1", ".", "/", ".com", LV_SYMBOL_NEW_LINE, ""
  };
  lv_kb_set_map(obj, kb_map);
  static const lv_btnm_ctrl_t kb_ctrl_map[] = {

  };
  lv_kb_set_ctrl_map(obj, );*/
  return obj;
}

QSize LVGLKeyboard::minimumSize() const { return QSize(200, 80); }

QStringList LVGLKeyboard::styles() const {
  return QStringList() << "KEYBOARD_PART_BG"
                       << "KEYBOARD_PART_BTN";
}

lv_style_t *LVGLKeyboard::style(lv_obj_t *obj, lv_obj_part_t part) const {
  return lv_obj_get_local_style(obj, part);
}

void LVGLKeyboard::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const {
  lv_obj_add_style(obj, LV_BTN_PART_MAIN, style);
}

void LVGLKeyboard::addStyle(lv_obj_t *obj, lv_style_t *style,
                            lv_obj_part_t part) const {
  lv_obj_add_style(obj, part, style);
}

void LVGLKeyboard::initStateStyles() {
  for (int i = 0; i < 2; ++i) {
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
