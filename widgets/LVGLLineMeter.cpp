#include "LVGLLineMeter.h"

#include <QIcon>
#include "LVGLObject.h"
#include "properties/LVGLPropertyRange.h"
#include "properties/LVGLPropertyVal2.h"

class LVGLPropertyLMeterRange : public LVGLPropertyRange
{
public:
	QStringList function(LVGLObject *obj) const {
		return { QString("lv_lmeter_set_range(%1, %2, %2);").arg(obj->codeName()).arg(getMin(obj)).arg(getMax(obj)) };
	}

protected:
	int getMin(LVGLObject *obj) const { return lv_lmeter_get_min_value(obj->obj()); }
	int getMax(LVGLObject *obj) const { return lv_lmeter_get_max_value(obj->obj()); }
	void set(LVGLObject *obj, int min, int max) {
		lv_lmeter_set_range(obj->obj(), static_cast<int16_t>(min), static_cast<int16_t>(max));
	}
};

LVGLLineMeter::LVGLLineMeter()
{
	m_properties << new LVGLPropertyLMeterRange;
	m_properties << new LVGLPropertyValInt16(INT16_MIN, INT16_MAX,
														  "Value", "lv_lmeter_set_value",
														  lv_lmeter_set_value, lv_lmeter_get_value);
	m_properties << new LVGLPropertyValUInt16(0, 360,
														  "Angle", "lv_lmeter_set_angle_offset",
														  lv_lmeter_set_angle_offset, lv_lmeter_get_angle_offset);
	m_properties << new LVGLPropertyVal2UInt16(0, 360, "Angle", lv_lmeter_get_scale_angle,
															 0, UINT16_MAX, "Lines", lv_lmeter_get_line_count,
															 "lv_lmeter_set_scale", lv_lmeter_set_scale, "Scale");


	m_editableStyles << LVGL::StyleParts(LVGL::Body | LVGL::Line); // LV_GAUGE_STYLE_MAIN
}

QString LVGLLineMeter::name() const
{
	return "Line meter";
}

QString LVGLLineMeter::className() const
{
	return "lv_lmeter";
}

LVGLWidget::Type LVGLLineMeter::type() const
{
	return LineMeter;
}

QIcon LVGLLineMeter::icon() const
{
	return QIcon();
}

lv_obj_t *LVGLLineMeter::newObject(lv_obj_t *parent) const
{
	lv_obj_t *obj = lv_lmeter_create(parent, nullptr);
	return obj;
}

QSize LVGLLineMeter::minimumSize() const
{
	return QSize(100, 100);
}

QStringList LVGLLineMeter::styles() const
{
	return QStringList() << "LV_LMETER_STYLE_MAIN";
}

lv_style_t *LVGLLineMeter::style(lv_obj_t *obj, int type) const
{
	return const_cast<lv_style_t*>(lv_lmeter_get_style(obj, type & 0xff));
}

void LVGLLineMeter::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const
{
	lv_lmeter_set_style(obj, static_cast<lv_lmeter_style_t>(type), style);
}

lv_style_t *LVGLLineMeter::defaultStyle(int type) const
{
	if (type == LV_LMETER_STYLE_MAIN)
		return &lv_style_pretty_color;
	return nullptr;
}
