#include "LVGLMessageBox.h"

#include <QIcon>

#include "LVGLObject.h"
#include "properties/LVGLPropertyTextList.h"

// class LVGLPropertyMBoxButtons : public LVGLPropertyTextList {
// public:
//  inline QString name() const override { return "Buttons"; }
//  inline ~LVGLPropertyMBoxButtons() {}

// protected:
//  inline QStringList get(LVGLObject *obj) const override {
//    QStringList ret;
//    lv_obj_t *btnm =
//        reinterpret_cast<lv_msgbox_ext_t *>(lv_obj_get_ext_attr(obj->obj()))
//            ->btnm;
//    if (btnm == nullptr) return {};

//    lv_msgbox_ext_t *ext =
//        reinterpret_cast<lv_msgbox_ext_t *>(lv_obj_get_ext_attr(btnm));
//    for (uint16_t i = 0; i < ext->btn_cnt; ++i) ret << QString(ext->map_p[i]);
//    return ret;
//  }

//  inline void set(LVGLObject *obj, QStringList list) override {
//    char **map = new char *[list.size() + 1];
//    for (int i = 0; i < list.size(); ++i) {
//      map[i] = new char[list[i].size() + 1];
//      memcpy(map[i], qUtf8Printable(list[i]), list[i].size());
//      map[i][list[i].size()] = '\0';
//    }
//    map[list.size()] = new char[1];
//    map[list.size()][0] = '\0';
//    lv_msgbox_add_btns(obj->obj(), const_cast<const char **>(map));
//  }
//};

LVGLMessageBox::LVGLMessageBox() {
  initStateStyles();
  m_parts << LV_MSGBOX_PART_BG << LV_MSGBOX_PART_BTN_BG << LV_MSGBOX_PART_BTN;

  // m_properties << new LVGLPropertyMBoxButtons;
  m_properties << new LVGLPropertyString(
      "Text", "lv_msgbox_set_text", lv_msgbox_set_text, lv_msgbox_get_text);
  m_properties << new LVGLPropertyValUInt16(
      0, UINT16_MAX, "Animation time", "lv_msgbox_set_anim_time",
      lv_msgbox_set_anim_time, lv_msgbox_get_anim_time);
  m_properties << new LVGLPropertyBool("Recolor", "lv_mbox_set_recolor",
                                       lv_msgbox_set_recolor,
                                       lv_msgbox_get_recolor);

  m_editableStyles << LVGL::MsgboxMAIN  // LV_MSGBOX_PART_BG
                   << LVGL::Background  // LV_MSGBOX_PART_BTN_BG
                   << LVGL::Button;     // LV_MSGBOX_PART_BTN
}

QString LVGLMessageBox::name() const { return "Message box"; }

QString LVGLMessageBox::className() const { return "lv_msgbox"; }

LVGLWidget::Type LVGLMessageBox::type() const { return MessageBox; }

QIcon LVGLMessageBox::icon() const { return QIcon(); }

lv_obj_t *LVGLMessageBox::newObject(lv_obj_t *parent) const {
  lv_obj_t *obj = lv_msgbox_create(parent, nullptr);
  return obj;
}

QSize LVGLMessageBox::minimumSize() const { return {200, 85}; }

QStringList LVGLMessageBox::styles() const {
  return {"MSGBOX_PART_BG", "MSGBOX_PART_BTN_BG", "MSGBOX_PART_BTN"};
}

lv_style_t *LVGLMessageBox::style(lv_obj_t *obj, lv_obj_part_t part) const {
  return lv_obj_get_local_style(obj, part);
}

void LVGLMessageBox::setStyle(lv_obj_t *obj, int type,
                              lv_style_t *style) const {
  lv_obj_add_style(obj, LV_BTN_PART_MAIN, style);
}

void LVGLMessageBox::addStyle(lv_obj_t *obj, lv_style_t *style,
                              lv_obj_part_t part) const {
  lv_obj_add_style(obj, part, style);
}

void LVGLMessageBox::initStateStyles() {
  for (int i = 0; i < 3; ++i) {
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
