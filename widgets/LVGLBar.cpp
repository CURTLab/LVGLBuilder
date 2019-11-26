#include "LVGLBar.h"

#include <QIcon>
#include "LVGLObject.h"
#include "properties/LVGLPropertyRange.h"

class LVGLPropertyBarValue : public LVGLPropertyInt
{
public:
	LVGLPropertyBarValue() : LVGLPropertyInt(INT16_MIN, INT16_MAX) {}

	QString name() const { return "Value"; }

	QStringList function(LVGLObject *obj) const {
		return QStringList() << QString("lv_bar_set_value(%1, %2, LV_ANIM_ON);").arg(obj->codeName()).arg(get(obj));
	}

protected:
	int get(LVGLObject *obj) const { return lv_bar_get_value(obj->obj()); }
	void set(LVGLObject *obj, int value) { lv_bar_set_value(obj->obj(), static_cast<int16_t>(value), LV_ANIM_OFF); }
};

class LVGLPropertyBarRange : public LVGLPropertyRange
{
protected:
	int getMin(LVGLObject *obj) const { return lv_bar_get_min_value(obj->obj()); }
	int getMax(LVGLObject *obj) const { return lv_bar_get_max_value(obj->obj()); }
	void set(LVGLObject *obj, int min, int max) {
		lv_bar_set_range(obj->obj(), static_cast<int16_t>(min), static_cast<int16_t>(max));
	}
};

LVGLBar::LVGLBar()
{
	m_properties << new LVGLPropertyBarValue;
	m_properties << new LVGLPropertyBarRange;

	m_editableStyles << LVGL::Body; // LV_BAR_STYLE_BG
	m_editableStyles << LVGL::Body; // LV_BAR_STYLE_INDIC
}

QString LVGLBar::name() const
{
	return "Bar";
}

QString LVGLBar::className() const
{
	return "lv_bar";
}

LVGLWidget::Type LVGLBar::type() const
{
	return Bar;
}

QPixmap LVGLBar::preview() const
{
	return QPixmap();
}

QIcon LVGLBar::icon() const
{
	return QIcon();
}

lv_obj_t *LVGLBar::newObject(lv_obj_t *parent) const
{
	lv_obj_t *obj = lv_bar_create(parent, nullptr);
	return obj;
}

QSize LVGLBar::minimumSize() const
{
	return QSize(200, 30);
}

QStringList LVGLBar::styles() const
{
	return QStringList() << "LV_BAR_STYLE_BG"
								<< "LV_BAR_STYLE_INDIC";
}

lv_style_t *LVGLBar::style(lv_obj_t *obj, int type) const
{
	return const_cast<lv_style_t*>(lv_bar_get_style(obj, type & 0xff));
}

void LVGLBar::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const
{
	lv_bar_set_style(obj, static_cast<lv_bar_style_t>(type), style);
}

lv_style_t *LVGLBar::defaultStyle(int type) const
{
	if (type == LV_BAR_STYLE_BG)
		return &lv_style_pretty;
	else if (type == LV_BAR_STYLE_INDIC)
		return &lv_style_pretty_color;
	return nullptr;
}
