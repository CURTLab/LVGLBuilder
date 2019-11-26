#include "LVGLSlider.h"

#include <QIcon>

#include "LVGLObject.h"
#include "properties/LVGLPropertyRange.h"

class LVGLPropertySliderValue : public LVGLPropertyInt
{
public:
	LVGLPropertySliderValue() : LVGLPropertyInt(INT16_MIN, INT16_MAX) {}

	QString name() const { return "Value"; }

	QStringList function(LVGLObject *obj) const {
		return QStringList() << QString("lv_slider_set_value(%1, %2, LV_ANIM_OFF);").arg(obj->codeName()).arg(get(obj));
	}

protected:
	int get(LVGLObject *obj) const { return lv_slider_get_value(obj->obj()); }
	void set(LVGLObject *obj, int value) { lv_slider_set_value(obj->obj(), static_cast<int16_t>(value), LV_ANIM_OFF); }
};

class LVGLPropertySliderRange : public LVGLPropertyRange
{
public:
	QStringList function(LVGLObject *obj) const override {
		return QStringList() << QString("lv_slider_set_range(%1, %2, %3);").arg(obj->codeName()).arg(getMin(obj)).arg(getMax(obj));
	}

protected:
	int getMin(LVGLObject *obj) const override { return lv_slider_get_min_value(obj->obj()); }
	int getMax(LVGLObject *obj) const override { return lv_slider_get_max_value(obj->obj()); }
	void set(LVGLObject *obj, int min, int max) override {
		lv_slider_set_range(obj->obj(), static_cast<int16_t>(min), static_cast<int16_t>(max));
	}
};

class LVGLPropertySliderKnob : public LVGLPropertyBool
{
public:
	QString name() const { return "Knob inside"; }

	QStringList function(LVGLObject *obj) const {
		return QStringList() << QString("lv_slider_set_knob_in(%1, %2);").arg(obj->codeName()).arg(QVariant(get(obj)).toString());
	}

protected:
	bool get(LVGLObject *obj) const { return lv_slider_get_knob_in(obj->obj()); }
	void set(LVGLObject *obj, bool boolean) { lv_slider_set_knob_in(obj->obj(), boolean); }
};

LVGLSlider::LVGLSlider()
{
	m_properties << new LVGLPropertySliderValue;
	m_properties << new LVGLPropertySliderRange;
	m_properties << new LVGLPropertySliderKnob;

	m_editableStyles << LVGL::Body; // LV_SLIDER_STYLE_BG
	m_editableStyles << LVGL::Body; // LV_SLIDER_STYLE_INDIC
	m_editableStyles << LVGL::Body; // LV_SLIDER_STYLE_KNOB
}

QString LVGLSlider::name() const
{
	return "Slider";
}

QString LVGLSlider::className() const
{
	return "lv_slider";
}

LVGLWidget::Type LVGLSlider::type() const
{
	return Slider;
}

QPixmap LVGLSlider::preview() const
{
	return QPixmap();
}

QIcon LVGLSlider::icon() const
{
	return QIcon();
}

lv_obj_t *LVGLSlider::newObject(lv_obj_t *parent) const
{
	lv_obj_t *obj = lv_slider_create(parent, nullptr);
	/*for (const LVGLProperty &p:m_properties)
		p.set(obj, p.defaultVal);*/
	return obj;
}

QSize LVGLSlider::minimumSize() const
{
	return QSize(150, 25);
}

QStringList LVGLSlider::styles() const
{
	return QStringList() << "LV_SLIDER_STYLE_BG"
								<< "LV_SLIDER_STYLE_INDIC"
								<< "LV_SLIDER_STYLE_KNOB";
}

lv_style_t *LVGLSlider::style(lv_obj_t *obj, int type) const
{
	return const_cast<lv_style_t*>(lv_slider_get_style(obj, type & 0xff));
}

void LVGLSlider::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const
{
	lv_slider_set_style(obj, static_cast<lv_slider_style_t>(type), style);
}

lv_style_t *LVGLSlider::defaultStyle(int type) const
{
	if (type == LV_SLIDER_STYLE_BG)
		return &lv_style_pretty;
	else if (type == LV_SLIDER_STYLE_INDIC)
		return &lv_style_pretty_color;
	else if (type == LV_SLIDER_STYLE_KNOB)
		return &lv_style_pretty;
	return nullptr;
}
