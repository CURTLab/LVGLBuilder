#include "LVGLImage.h"

#include <QIcon>
#include <QComboBox>

#include "properties/LVGLPropertyImage.h"
#include "LVGLObject.h"
#include "LVGLCore.h"

class LVGLPropertyImgOffsetX : public LVGLPropertyCoord
{
public:
	inline LVGLPropertyImgOffsetX(LVGLProperty *p) : LVGLPropertyCoord(Qt::Horizontal, p) {}
	inline QString name() const override { return "X"; }

	inline lv_coord_t get(LVGLObject *obj) const override { return lv_img_get_offset_x(obj->obj()); }
	inline void set(LVGLObject *obj, lv_coord_t value) override { lv_img_set_offset_x(obj->obj(), value); }

};

class LVGLPropertyImgOffsetY : public LVGLPropertyCoord
{
public:
	inline LVGLPropertyImgOffsetY(LVGLProperty *p) : LVGLPropertyCoord(Qt::Vertical, p) {}
	inline QString name() const override { return "Y"; }

	inline lv_coord_t get(LVGLObject *obj) const override { return lv_img_get_offset_y(obj->obj()); }
	inline void set(LVGLObject *obj, lv_coord_t value) override { lv_img_set_offset_y(obj->obj(), value); }

};

class LVGLPropertyImgOffset : public LVGLProperty
{
public:
	inline LVGLPropertyImgOffset(LVGLProperty *parent = nullptr)
		: LVGLProperty(parent)
		, m_x(new LVGLPropertyImgOffsetX(this))
		, m_y(new LVGLPropertyImgOffsetY(this))
	{
		m_childs << m_x << m_y;
	}

	inline QString name() const override { return "Offset"; }
	inline QVariant value(LVGLObject *obj) const override {
		return QString("(%1,%2)").arg(child(0)->value(obj).toInt())
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
		QJsonObject object({{"X", m_x->get(obj)},
								  {"Y", m_y->get(obj)}
								 });
		return object;
	}

	QStringList function(LVGLObject *obj) const override {
		QStringList ret;
		if (m_x->get(obj) > 0)
			ret << QString("lv_img_set_offset_x(%1, %2);").arg(obj->codeName()).arg(m_x->get(obj));
		if (m_y->get(obj) > 0)
			ret << QString("lv_img_set_offset_y(%1, %2);").arg(obj->codeName()).arg(m_y->get(obj));
		return ret;
	}

private:
	LVGLPropertyImgOffsetX *m_x;
	LVGLPropertyImgOffsetY *m_y;

};

class LVGLPropertyImgAutoSize : public LVGLPropertyBool
{
public:
	inline QString name() const { return "Auto Size"; }

	QStringList function(LVGLObject *obj) const {
		if (get(obj)) return QStringList();
		return QStringList() << QString("lv_img_set_auto_size(%1, %2);").arg(obj->codeName()).arg(QVariant(get(obj)).toString());
	}

protected:
	inline bool get(LVGLObject *obj) const { return lv_img_get_auto_size(obj->obj()); }
	inline void set(LVGLObject *obj, bool statue) { lv_img_set_auto_size(obj->obj(), statue); }
};

class LVGLPropertyImgSource : public LVGLPropertyImage
{
public:
	inline QString name() const override { return "Source"; }

	QStringList function(LVGLObject *obj) const override {
		LVGLImageData *img = lvgl.imageByDesc(get(obj));
		if (img == nullptr) return QStringList();
		return QStringList() << QString("lv_img_set_src(%1, &%2);").arg(obj->codeName()).arg(img->codeName());
	}

protected:
	const lv_img_dsc_t *get(LVGLObject *obj) const override {
		return reinterpret_cast<const lv_img_dsc_t*>(lv_img_get_src(obj->obj()));
	}
	void set(LVGLObject *obj, const lv_img_dsc_t *img) override { lv_img_set_src(obj->obj(), img); }
};

LVGLImage::LVGLImage()
{
	m_properties << new LVGLPropertyImgOffset;
	m_properties << new LVGLPropertyImgAutoSize;
	m_properties << new LVGLPropertyImgSource;

	m_editableStyles << LVGL::Image; // LV_IMG_STYLE_MAIN
}

QString LVGLImage::name() const
{
	return "Image";
}

QString LVGLImage::className() const
{
	return "lv_img";
}

LVGLWidget::Type LVGLImage::type() const
{
	return Image;
}

QIcon LVGLImage::icon() const
{
	return QIcon();
}

QSize LVGLImage::minimumSize() const
{
	return QSize(50, 50);
}

lv_obj_t *LVGLImage::newObject(lv_obj_t *parent) const
{
	lv_obj_t *obj = lv_img_create(parent, nullptr);
	lv_img_set_src(obj, lvgl.defaultImage());
	return obj;
}

QStringList LVGLImage::styles() const
{
	return QStringList() << "LV_IMG_STYLE_MAIN";
}

lv_style_t *LVGLImage::style(lv_obj_t *obj, int type) const
{
	return const_cast<lv_style_t*>(lv_img_get_style(obj, type & 0xff));
}

void LVGLImage::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const
{
	lv_img_set_style(obj, static_cast<lv_img_style_t>(type), style);
}

lv_style_t *LVGLImage::defaultStyle(int type) const
{
	if (type == LV_IMG_STYLE_MAIN)
		return &lv_style_plain;
	return nullptr;
}
