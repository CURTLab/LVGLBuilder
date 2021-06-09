#include "LVGLImageButton.h"

#include <QIcon>

#include "MainWindow.h"
#include "core/LVGLCore.h"
#include "core/LVGLHelper.h"
#include "core/LVGLObject.h"
#include "properties/LVGLPropertyAnyFunc.h"
#include "properties/LVGLPropertyImage.h"

class LVGLPropertyImgBtnSrc : public LVGLPropertyImage {
 public:
  LVGLPropertyImgBtnSrc(lv_btn_state_t state, LVGLProperty *parent = nullptr)
      : LVGLPropertyImage(parent),
        m_values({"Released", "Pressed", "Disabled", "Checked released",
                  "Checked pressed", "Checked Disabled"}),
        m_trvalues({QObject::tr("Released"), QObject::tr("Pressed"),
                    QObject::tr("Disabled"), QObject::tr("Checked released"),
                    QObject::tr("Checked pressed"),
                    QObject::tr("Checked Disabled")}),
        m_types({"LV_BTN_STATE_RELEASED", "LV_BTN_STATE_PRESSED",
                 "LV_BTN_STATE_DISABLED", "LV_BTN_STATE_CHECKED_RELEASED",
                 "LV_BTN_STATE_CHECKED_PRESSED",
                 "LV_BTN_STATE_CHECKED_DISABLED"}),
        m_state(state) {}

  QString name() const override {
    return QObject::tr("Source ") + m_trvalues.at(m_state);
  }
  QString codename() const override { return "Source " + m_values.at(m_state); }

  QStringList function(LVGLObject *obj) const override {
    LVGLImageData *img = lvgl.imageByDesc(get(obj));
    if (img == nullptr) return QStringList();
    return QStringList() << QString("lv_imgbtn_set_src(%1, %2, &%3);")
                                .arg(obj->codeName())
                                .arg(m_types.at(m_state))
                                .arg(img->codeName());
  }

 protected:
  QStringList m_trvalues;
  QStringList m_values;
  QStringList m_types;
  lv_btn_state_t m_state;

  virtual const lv_img_dsc_t *get(LVGLObject *obj) const override {
    return reinterpret_cast<const lv_img_dsc_t *>(
        lv_imgbtn_get_src(obj->obj(), m_state));
  }
  virtual void set(LVGLObject *obj, const lv_img_dsc_t *img) override {
    lv_imgbtn_set_src(obj->obj(), m_state, img);
  }
};

class LVGLPropertyImgBtnState : public LVGLPropertyEnum {
 public:
  LVGLPropertyImgBtnState()
      : LVGLPropertyEnum(QStringList()
                         << QObject::tr("Released") << QObject::tr("Pressed")
                         << QObject::tr("Disabled")
                         << QObject::tr("Checked released")
                         << QObject::tr("Checked pressed")
                         << QObject::tr("Check disabled")),
        m_values({"LV_BTN_STATE_RELEASED", "LV_BTN_STATE_PRESSED",
                  "LV_BTN_STATE_DISABLED", "LV_BTN_STATE_CHECKED_RELEASED",
                  "LV_BTN_STATE_CHECKED_PRESSED",
                  "LV_BTN_STATE_CHECKED_DISABLED"}) {}

  QString name() const { return QObject::tr("State"); }
  QString codename() const { return "State"; }

  QStringList function(LVGLObject *obj) const {
    return QStringList() << QString("lv_imgbtn_set_state(%1, %2);")
                                .arg(obj->codeName())
                                .arg(m_values.at(get(obj)));
  }

 protected:
  int get(LVGLObject *obj) const { return lv_imgbtn_get_state(obj->obj()); }
  void set(LVGLObject *obj, int index) {
    lv_imgbtn_set_state(obj->obj(), index & 0xff);
  }

  QStringList m_values;
};

LVGLImageButton::LVGLImageButton() {
  initStateStyles();
  m_parts << LV_IMGBTN_PART_MAIN;
  m_properties << new LVGLPropertyImgBtnState;
  m_properties << new LVGLPropertyBool(
      "checkable", QObject::tr("checkable"), "lv_imgbtn_set_checkable",
      lv_imgbtn_set_checkable, lv_imgbtn_get_checkable);
  m_properties << new LVGLPropertyImgBtnSrc(LV_BTN_STATE_RELEASED);
  m_properties << new LVGLPropertyImgBtnSrc(LV_BTN_STATE_PRESSED);
  // m_properties << new LVGLPropertyImgBtnSrc(LV_BTN_STATE_DISABLED);
  m_properties << new LVGLPropertyImgBtnSrc(LV_BTN_STATE_CHECKED_RELEASED);
  m_properties << new LVGLPropertyImgBtnSrc(LV_BTN_STATE_CHECKED_PRESSED);

  m_editableStyles << LVGL::ImgMAIN;  // LV_IMGBTN_PART_MAIN
}

QString LVGLImageButton::name() const { return QObject::tr("Image button"); }

QString LVGLImageButton::className() const { return "lv_imgbtn"; }

LVGLWidget::Type LVGLImageButton::type() const { return ImageButton; }

QIcon LVGLImageButton::icon() const { return QIcon(); }

QSize LVGLImageButton::minimumSize() const { return QSize(100, 35); }

lv_obj_t *LVGLImageButton::newObject(lv_obj_t *parent) const {
  lv_obj_t *obj = lv_imgbtn_create(parent, nullptr);
  return obj;
}

QStringList LVGLImageButton::styles() const {
  return QStringList() << "LV_IMGBTN_PART_MAIN";
}

lv_style_t *LVGLImageButton::style(lv_obj_t *obj, lv_obj_part_t part) const {
  return lv_obj_get_local_style(obj, part);
}

void LVGLImageButton::setStyle(lv_obj_t *obj, int type,
                               lv_style_t *style) const {
  lv_obj_add_style(obj, LV_BTN_PART_MAIN, style);
}

void LVGLImageButton::addStyle(lv_obj_t *obj, lv_style_t *style,
                               lv_obj_part_t part) const {
  lv_obj_add_style(obj, part, style);
}

void LVGLImageButton::initStateStyles() {
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
