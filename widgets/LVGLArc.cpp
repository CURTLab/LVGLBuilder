#include "LVGLArc.h"

#include <QIcon>

#include "LVGLObject.h"
#include "properties/LVGLPropertyVal2.h"

LVGLArc::LVGLArc()
{
	m_properties << new LVGLPropertyVal2UInt16(0, 360, "Start", lv_arc_get_angle_start,
															 0, 360, "End", lv_arc_get_angle_end,
															 "lv_arc_set_angles", lv_arc_set_angles, "Angles");

	m_editableStyles << LVGL::StyleParts(LVGL::Body | LVGL::Line); // LV_GAUGE_STYLE_MAIN
}

QString LVGLArc::name() const
{
	return "Arc";
}

QString LVGLArc::className() const
{
	return "lv_arc";
}

LVGLWidget::Type LVGLArc::type() const
{
	return Arc;
}

QIcon LVGLArc::icon() const
{
	return QIcon();
}

lv_obj_t *LVGLArc::newObject(lv_obj_t *parent) const
{
	lv_obj_t *obj = lv_arc_create(parent, nullptr);
	return obj;
}

QSize LVGLArc::minimumSize() const
{
	return {75, 75};
}

QStringList LVGLArc::styles() const
{
	return QStringList() << "LV_ARC_STYLE_MAIN";
}

lv_style_t *LVGLArc::style(lv_obj_t *obj, int type) const
{
	return const_cast<lv_style_t*>(lv_arc_get_style(obj, type & 0xff));
}

void LVGLArc::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const
{
	lv_arc_set_style(obj, static_cast<lv_arc_style_t>(type), style);
}

lv_style_t *LVGLArc::defaultStyle(int type) const
{
	if (type == LV_ARC_STYLE_MAIN)
		return &lv_style_plain_color;
	return nullptr;
}
