#include "LVGLArc.h"

#include <QIcon>

#include "LVGLObject.h"

class LVGLPropertyAngleStart : public LVGLPropertyInt
{
public:
	inline LVGLPropertyAngleStart() : LVGLPropertyInt(0, 360) {}
	inline QString name() const override { return "Start angle"; }

	inline int get(LVGLObject *obj) const override { return lv_arc_get_angle_start(obj->obj()); }
	inline void set(LVGLObject *obj, int value) override {
		lv_obj_t *o = obj->obj();
		lv_arc_set_angles(o, static_cast<uint16_t>(value),
									lv_arc_get_angle_end(o));
	}

};

class LVGLPropertyAngleEnd : public LVGLPropertyInt
{
public:
	inline LVGLPropertyAngleEnd() : LVGLPropertyInt(0, 360) {}
	inline QString name() const override { return "End angle"; }

	inline int get(LVGLObject *obj) const override { return lv_arc_get_angle_end(obj->obj()); }
	inline void set(LVGLObject *obj, int value) override {
		lv_obj_t *o = obj->obj();
		lv_arc_set_angles(o, lv_arc_get_angle_start(o),
									static_cast<uint16_t>(value));
	}

};

LVGLArc::LVGLArc()
{
	m_properties << new LVGLPropertyAngleStart;
	m_properties << new LVGLPropertyAngleEnd;

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

QPixmap LVGLArc::preview() const
{
	return QPixmap();
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
