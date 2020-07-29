#include "LVGLLED.h"

#include <QIcon>

#include "LVGLObject.h"

#define LV_LED_WIDTH_DEF (LV_DPI / 3)
#define LV_LED_HEIGHT_DEF (LV_DPI / 3)

class LVGLPropertyLEDBrightness : public LVGLPropertyInt
{
public:
	LVGLPropertyLEDBrightness() : LVGLPropertyInt(0, 255) {}

	QString name() const { return "Brightness"; }

	QStringList function(LVGLObject *obj) const {
		return QStringList() << QString("lv_led_set_bright(%1, %2);").arg(obj->codeName()).arg(get(obj));
	}

protected:
	int get(LVGLObject *obj) const { return lv_led_get_bright(obj->obj()); }
	void set(LVGLObject *obj, int value) { lv_led_set_bright(obj->obj(), static_cast<uint8_t>(value)); }
};

LVGLLED::LVGLLED()
{
	m_properties << new LVGLPropertyLEDBrightness;

	m_editableStyles << LVGL::Body; // LV_LED_STYLE_MAIN
}

QString LVGLLED::name() const
{
	return "LED";
}

QString LVGLLED::className() const
{
	return "lv_led";
}

LVGLWidget::Type LVGLLED::type() const
{
	return LED;
}

QIcon LVGLLED::icon() const
{
	return QIcon();
}

lv_obj_t *LVGLLED::newObject(lv_obj_t *parent) const
{
	lv_obj_t *obj = lv_led_create(parent, nullptr);
	return obj;
}

QSize LVGLLED::minimumSize() const
{
	return QSize(LV_LED_WIDTH_DEF, LV_LED_HEIGHT_DEF);
}

QStringList LVGLLED::styles() const
{
	return QStringList() << "LV_LED_STYLE_MAIN";
}

lv_style_t *LVGLLED::style(lv_obj_t *obj, int type) const
{
	return const_cast<lv_style_t*>(lv_led_get_style(obj, type & 0xff));
}

void LVGLLED::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const
{
	lv_led_set_style(obj, static_cast<lv_btn_style_t>(type), style);
}

lv_style_t *LVGLLED::defaultStyle(int type) const
{
	if (type == LV_LED_STYLE_MAIN)
		return &lv_style_pretty_color;
	return nullptr;
}
