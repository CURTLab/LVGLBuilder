#include "LVGLImage.h"

#include <QComboBox>
#include <QIcon>

#include "LVGLCore.h"
#include "LVGLObject.h"
#include "properties/LVGLPropertyImage.h"

class LVGLPropertyImgOffsetX : public LVGLPropertyCoord {
 public:
  inline LVGLPropertyImgOffsetX(LVGLProperty *p)
      : LVGLPropertyCoord(Qt::Horizontal, p) {}
  inline QString name() const override { return "X"; }

  inline lv_coord_t get(LVGLObject *obj) const override {
    return lv_img_get_offset_x(obj->obj());
  }
  inline void set(LVGLObject *obj, lv_coord_t value) override {
    lv_img_set_offset_x(obj->obj(), value);
  }
};

class LVGLPropertyImgOffsetY : public LVGLPropertyCoord {
 public:
  inline LVGLPropertyImgOffsetY(LVGLProperty *p)
      : LVGLPropertyCoord(Qt::Vertical, p) {}
  inline QString name() const override { return "Y"; }

  inline lv_coord_t get(LVGLObject *obj) const override {
    return lv_img_get_offset_y(obj->obj());
  }
  inline void set(LVGLObject *obj, lv_coord_t value) override {
    lv_img_set_offset_y(obj->obj(), value);
  }
};

class LVGLPropertyImgOffset : public LVGLProperty {
 public:
  inline LVGLPropertyImgOffset(LVGLProperty *parent = nullptr)
      : LVGLProperty(parent),
        m_x(new LVGLPropertyImgOffsetX(this)),
        m_y(new LVGLPropertyImgOffsetY(this)) {
    m_childs << m_x << m_y;
  }

  inline QString name() const override { return "Offset"; }
  inline QVariant value(LVGLObject *obj) const override {
    return QString("(%1,%2)")
        .arg(child(0)->value(obj).toInt())
        .arg(child(1)->value(obj).toInt());
  }

  inline void setValue(LVGLObject *obj, QVariant value) override {
    if (value.type() == QVariant::Map) {
      QVariantMap map = value.toMap();
      m_x->set(obj, static_cast<lv_coord_t>(map["X"].toInt()));
      m_y->set(obj, static_cast<lv_coord_t>(map["Y"].toInt()));
    }
  }

  inline QJsonValue toJson(LVGLObject *obj) const override {
    QJsonObject object({{"X", m_x->get(obj)}, {"Y", m_y->get(obj)}});
    return object;
  }

  QStringList function(LVGLObject *obj) const override {
    QStringList ret;
    if (m_x->get(obj) > 0)
      ret << QString("lv_img_set_offset_x(%1, %2);")
                 .arg(obj->codeName())
                 .arg(m_x->get(obj));
    if (m_y->get(obj) > 0)
      ret << QString("lv_img_set_offset_y(%1, %2);")
                 .arg(obj->codeName())
                 .arg(m_y->get(obj));
    return ret;
  }

 private:
  LVGLPropertyImgOffsetX *m_x;
  LVGLPropertyImgOffsetY *m_y;
};

class LVGLPropertyImgAutoSize : public LVGLPropertyBool {
 public:
  inline QString name() const { return "Auto Size"; }

  QStringList function(LVGLObject *obj) const {
    if (get(obj)) return QStringList();
    return QStringList() << QString("lv_img_set_auto_size(%1, %2);")
                                .arg(obj->codeName())
                                .arg(QVariant(get(obj)).toString());
  }

 protected:
  inline bool get(LVGLObject *obj) const {
    return lv_img_get_auto_size(obj->obj());
  }
  inline void set(LVGLObject *obj, bool statue) {
    lv_img_set_auto_size(obj->obj(), statue);
  }
};

class LVGLPropertyImgSource : public LVGLPropertyImage {
 public:
  inline QString name() const override { return "Source"; }

  QStringList function(LVGLObject *obj) const override {
    LVGLImageData *img = lvgl.imageByDesc(get(obj));
    if (img == nullptr) return QStringList();
    return QStringList() << QString("lv_img_set_src(%1, &%2);")
                                .arg(obj->codeName())
                                .arg(img->codeName());
  }

 protected:
  const lv_img_dsc_t *get(LVGLObject *obj) const override {
    return reinterpret_cast<const lv_img_dsc_t *>(lv_img_get_src(obj->obj()));
  }
  void set(LVGLObject *obj, const lv_img_dsc_t *img) override {
    lv_img_set_src(obj->obj(), img);
  }
};

LVGLImage::LVGLImage() {
  initStateStyles();
  m_parts << LV_IMG_PART_MAIN;
  m_properties << new LVGLPropertyImgOffset;
  m_properties << new LVGLPropertyImgAutoSize;
  m_properties << new LVGLPropertyImgSource;

  m_editableStyles << LVGL::ImgMAIN;  // LV_IMG_PART_MAIN
}

QString LVGLImage::name() const { return "Image"; }

QString LVGLImage::className() const { return "lv_img"; }

LVGLWidget::Type LVGLImage::type() const { return Image; }

QIcon LVGLImage::icon() const { return QIcon(); }

QSize LVGLImage::minimumSize() const { return QSize(60, 60); }

lv_obj_t *LVGLImage::newObject(lv_obj_t *parent) const {
  lv_obj_t *obj = lv_img_create(parent, nullptr);
  return obj;
}

QStringList LVGLImage::styles() const {
  return QStringList() << "LV_IMG_PART_MAIN";
}

lv_style_t *LVGLImage::style(lv_obj_t *obj, lv_obj_part_t part) const {
  return lv_obj_get_local_style(obj, part);
}

void LVGLImage::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const {
  lv_obj_add_style(obj, LV_BTN_PART_MAIN, style);
}

void LVGLImage::addStyle(lv_obj_t *obj, lv_style_t *style,
                         lv_obj_part_t part) const {
  lv_obj_add_style(obj, part, style);
}

void LVGLImage::initStateStyles() {
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
