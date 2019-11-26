#include "LVGLSwitch.h"

#include <QIcon>

#include "LVGLObject.h"

class LVGLPropertyState : public LVGLPropertyBool
{
public:
	QString name() const { return "State"; }

protected:
	bool get(LVGLObject *obj) const { return lv_sw_get_state(obj->obj()); }
	void set(LVGLObject *obj, bool state) {
		if (state)
			lv_sw_on(obj->obj(), LV_ANIM_OFF);
		else
			lv_sw_off(obj->obj(), LV_ANIM_OFF);
	}
};

LVGLSwitch::LVGLSwitch()
{
	m_properties << new LVGLPropertyState;

	m_editableStyles << LVGL::Body; // LV_SW_STYLE_BG
	m_editableStyles << LVGL::Body; // LV_SW_STYLE_INDIC
	m_editableStyles << LVGL::Body; // LV_SW_STYLE_KNOB_OFF
	m_editableStyles << LVGL::Body; // LV_SW_STYLE_KNOB_ON
}

QString LVGLSwitch::name() const
{
	return "Switch";
}

QString LVGLSwitch::className() const
{
	return "lv_sw";
}

LVGLWidget::Type LVGLSwitch::type() const
{
	return Switch;
}

QPixmap LVGLSwitch::preview() const
{
	return QPixmap();
}

QIcon LVGLSwitch::icon() const
{
	return QIcon();
}

lv_obj_t *LVGLSwitch::newObject(lv_obj_t *parent) const
{
	lv_obj_t *obj = lv_sw_create(parent, nullptr);
	return obj;
}

QSize LVGLSwitch::minimumSize() const
{
	return QSize(70, 35);
}

QStringList LVGLSwitch::styles() const
{
	return QStringList() << "LV_SW_STYLE_BG"
								<< "LV_SW_STYLE_INDIC"
								<< "LV_SW_STYLE_KNOB_OFF"
								<< "LV_SW_STYLE_KNOB_ON";
}

lv_style_t *LVGLSwitch::style(lv_obj_t *obj, int type) const
{
	return const_cast<lv_style_t*>(lv_sw_get_style(obj, type & 0xff));
}

void LVGLSwitch::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const
{
	lv_sw_set_style(obj, static_cast<lv_sw_style_t>(type), style);
}

lv_style_t *LVGLSwitch::defaultStyle(int type) const
{
	if (type == LV_SW_STYLE_BG)
		return &lv_style_pretty;
	else if (type == LV_SW_STYLE_INDIC)
		return &lv_style_pretty_color;
	else if (type == LV_SW_STYLE_KNOB_OFF)
		return &lv_style_pretty;
	else if (type == LV_SW_STYLE_KNOB_ON)
		return &lv_style_pretty;
	return nullptr;
}
