#include "LVGLImageSlider.h"

#include <QIcon>

#include "LVGLCore.h"
#include "LVGLObject.h"
#include "properties/LVGLPropertyImage.h"
#include "properties/LVGLPropertyRange.h"

class LVGLPropertySliderImgBg : public LVGLPropertyImage
{
public:
	inline QString name() const override { return "Image background"; }

	QStringList function(LVGLObject *obj) const override {
		LVGLImageData *img = lvgl.imageByDesc(get(obj));
		if (img == nullptr) return QStringList();
		return QStringList() << QString("lv_imgslider_set_bg_src(%1, &%2);").arg(obj->codeName()).arg(img->codeName());
	}

protected:
	const lv_img_dsc_t *get(LVGLObject *obj) const override {
		return reinterpret_cast<const lv_img_dsc_t *>(lv_imgslide_get_bg_src(obj->obj()));
	}
	void set(LVGLObject *obj, const lv_img_dsc_t *img) override { lv_imgslider_set_bg_src(obj->obj(), img); }
};

class LVGLPropertySliderImgIndic : public LVGLPropertyImage
{
public:
	inline QString name() const override { return "Image indicator"; }

	QStringList function(LVGLObject *obj) const override {
		LVGLImageData *img = lvgl.imageByDesc(get(obj));
		if (img == nullptr) return QStringList();
		return QStringList() << QString("lv_imgslide_get_indic_src(%1, &%2);").arg(obj->codeName()).arg(img->codeName());
	}

protected:
	const lv_img_dsc_t *get(LVGLObject *obj) const override {
		return reinterpret_cast<const lv_img_dsc_t *>(lv_imgslide_get_indic_src(obj->obj()));
	}
	void set(LVGLObject *obj, const lv_img_dsc_t *img) override { lv_imgslider_set_indic_src(obj->obj(), img); }
};


class LVGLPropertySliderImgKnob : public LVGLPropertyImage
{
public:
	inline QString name() const override { return "Image knob"; }

	QStringList function(LVGLObject *obj) const override {
		LVGLImageData *img = lvgl.imageByDesc(get(obj));
		if (img == nullptr) return QStringList();
		return QStringList() << QString("lv_imgslider_set_knob_src(%1, &%2);").arg(obj->codeName()).arg(img->codeName());
	}

protected:
	const lv_img_dsc_t *get(LVGLObject *obj) const override {
		return reinterpret_cast<const lv_img_dsc_t *>(lv_imgslide_get_knob_src(obj->obj()));
	}
	void set(LVGLObject *obj, const lv_img_dsc_t *img) override { lv_imgslider_set_knob_src(obj->obj(), img); }
};

class LVGLPropertyImgSliderValue : public LVGLPropertyInt
{
public:
	LVGLPropertyImgSliderValue() : LVGLPropertyInt(INT16_MIN, INT16_MAX) {}

	QString name() const override { return "Value"; }

	QStringList function(LVGLObject *obj) const override {
		return QStringList() << QString("lv_imgslider_set_value(%1, %2, LV_ANIM_OFF);").arg(obj->codeName()).arg(get(obj));
	}

protected:
	int get(LVGLObject *obj) const override { return lv_imgslider_get_value(obj->obj()); }
	void set(LVGLObject *obj, int value) override { lv_imgslider_set_value(obj->obj(), static_cast<int16_t>(value), LV_ANIM_OFF); }
};

class LVGLPropertyImgSliderRange : public LVGLPropertyRange
{
public:
	QStringList function(LVGLObject *obj) const override {
		return QStringList() << QString("lv_imgslider_set_range(%1, %2, %3);").arg(obj->codeName()).arg(getMin(obj)).arg(getMax(obj));
	}

protected:
	int getMin(LVGLObject *obj) const override { return lv_imgslider_get_min_value(obj->obj()); }
	int getMax(LVGLObject *obj) const override { return lv_imgslider_get_max_value(obj->obj()); }
	void set(LVGLObject *obj, int min, int max) override {
		lv_imgslider_set_range(obj->obj(), static_cast<int16_t>(min), static_cast<int16_t>(max));
	}

};

class LVGLPropertyImgSliderOffsetX : public LVGLPropertyCoord
{
public:
	inline LVGLPropertyImgSliderOffsetX(LVGLProperty *p) : LVGLPropertyCoord(Qt::Horizontal, p) {}
	inline QString name() const override { return "X"; }

	inline lv_coord_t get(LVGLObject *obj) const override { return lv_imgslider_get_offset_x(obj->obj()); }
	inline void set(LVGLObject *obj, lv_coord_t value) override { lv_imgslider_set_offset_x(obj->obj(), value); }

};

class LVGLPropertyImgSliderOffsetY : public LVGLPropertyCoord
{
public:
	inline LVGLPropertyImgSliderOffsetY(LVGLProperty *p) : LVGLPropertyCoord(Qt::Vertical, p) {}
	inline QString name() const override { return "Y"; }

	inline lv_coord_t get(LVGLObject *obj) const override { return lv_imgslider_get_offset_y(obj->obj()); }
	inline void set(LVGLObject *obj, lv_coord_t value) override { lv_imgslider_set_offset_y(obj->obj(), value); }

};

class LVGLPropertyImgSliderOffset : public LVGLProperty
{
public:
	inline LVGLPropertyImgSliderOffset(LVGLProperty *parent = nullptr)
		: LVGLProperty(parent)
		, m_x(new LVGLPropertyImgSliderOffsetX(this))
		, m_y(new LVGLPropertyImgSliderOffsetY(this))
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
		if (m_x->get(obj) != 0)
			ret << QString("lv_imgslider_set_offset_x(%1, %2);").arg(obj->codeName()).arg(m_x->get(obj));
		if (m_y->get(obj) != 0)
			ret << QString("lv_imgslider_set_offset_y(%1, %2);").arg(obj->codeName()).arg(m_y->get(obj));
		return ret;
	}

private:
	LVGLPropertyImgSliderOffsetX *m_x;
	LVGLPropertyImgSliderOffsetY *m_y;

};

LVGLImageSlider::LVGLImageSlider()
{
	m_properties << new LVGLPropertySliderImgBg;
	m_properties << new LVGLPropertySliderImgIndic;
	m_properties << new LVGLPropertySliderImgKnob;
	m_properties << new LVGLPropertyImgSliderValue;
	m_properties << new LVGLPropertyImgSliderRange;
	m_properties << new LVGLPropertyImgSliderOffset;

	m_editableStyles << LVGL::Image; // LV_IMGSLIDER_STYLE_BG
	m_editableStyles << LVGL::Image; // LV_IMGSLIDER_STYLE_INDIC
	m_editableStyles << LVGL::Image; // LV_IMGSLIDER_STYLE_KNOB
}

QString LVGLImageSlider::name() const
{
	return "Image slider";
}

QString LVGLImageSlider::className() const
{
	return "lv_imgslider";
}

LVGLWidget::Type LVGLImageSlider::type() const
{
	return ImageSlider;
}

QIcon LVGLImageSlider::icon() const
{
	return QIcon();
}

QSize LVGLImageSlider::minimumSize() const
{
	return QSize(100, 35);
}

lv_obj_t *LVGLImageSlider::newObject(lv_obj_t *parent) const
{
	lv_obj_t *obj = lv_imgslider_create(parent, nullptr);
	return obj;
}

QStringList LVGLImageSlider::styles() const
{
	return QStringList() << "LV_IMGSLIDER_STYLE_BG"
								<< "LV_IMGSLIDER_STYLE_INDIC"
								<< "LV_IMGSLIDER_STYLE_KNOB";
}

lv_style_t *LVGLImageSlider::style(lv_obj_t *obj, int type) const
{
	return const_cast<lv_style_t*>(lv_imgslider_get_style(obj, type & 0xff));
}

void LVGLImageSlider::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const
{
	lv_imgslider_set_style(obj, static_cast<lv_imgslider_style_t>(type), style);
}

lv_style_t *LVGLImageSlider::defaultStyle(int type) const
{
	Q_UNUSED(type)
	return nullptr;
}
