#include "LVGLPreloader.h"

#include <QIcon>
#include "LVGLObject.h"

class LVGLPropertyPreloadArcLen : public LVGLPropertyInt
{
public:
	LVGLPropertyPreloadArcLen() : LVGLPropertyInt(0, 360) {}

	QString name() const { return "Arc length"; }

protected:
	int get(LVGLObject *obj) const { return lv_preload_get_arc_length(obj->obj()); }
	void set(LVGLObject *obj, int value) { lv_preload_set_arc_length(obj->obj(), static_cast<lv_anim_value_t>(value)); }
};

class LVGLPropertyPreloadSpinTime : public LVGLPropertyInt
{
public:
	LVGLPropertyPreloadSpinTime() : LVGLPropertyInt(0, UINT16_MAX) {}

	QString name() const { return "Spin time"; }

protected:
	int get(LVGLObject *obj) const { return lv_preload_get_spin_time(obj->obj()); }
	void set(LVGLObject *obj, int value) { lv_preload_set_spin_time(obj->obj(), static_cast<uint16_t>(value)); }
};

class LVGLPropertyPreloadType : public LVGLPropertyEnum
{
public:
	LVGLPropertyPreloadType()
		: LVGLPropertyEnum(QStringList() << "Spinning arc" << "Fill and spin arc")
	{}

	QString name() const { return "Spin type"; }

protected:
	int get(LVGLObject *obj) const { return lv_preload_get_type(obj->obj()); }
	void set(LVGLObject *obj, int index) { lv_preload_set_type(obj->obj(), index & 0xff); }
};

class LVGLPropertyPreloadDir : public LVGLPropertyEnum
{
public:
	LVGLPropertyPreloadDir()
		: LVGLPropertyEnum(QStringList() << "Forward" << "Backward")
	{}

	QString name() const { return "Spin direction"; }

protected:
	int get(LVGLObject *obj) const { return lv_preload_get_dir(obj->obj()); }
	void set(LVGLObject *obj, int index) { lv_preload_set_dir(obj->obj(), index & 0xff); }
};

LVGLPreloader::LVGLPreloader()
{
	m_properties << new LVGLPropertyPreloadArcLen;
	m_properties << new LVGLPropertyPreloadSpinTime;
	m_properties << new LVGLPropertyPreloadType;
	m_properties << new LVGLPropertyPreloadDir;

	m_editableStyles << LVGL::StyleParts(LVGL::BodyBorder | LVGL::BodyPadding | LVGL::Line); // LV_PRELOAD_STYLE_MAIN
}

QString LVGLPreloader::name() const
{
	return "Preload";
}

QString LVGLPreloader::className() const
{
	return "lv_preload";
}

LVGLWidget::Type LVGLPreloader::type() const
{
	return Preloader;
}

QIcon LVGLPreloader::icon() const
{
	return QIcon();
}

lv_obj_t *LVGLPreloader::newObject(lv_obj_t *parent) const
{
	lv_obj_t *obj = lv_preload_create(parent, nullptr);
	return obj;
}

QSize LVGLPreloader::minimumSize() const
{
	return QSize(100, 100);
}

QStringList LVGLPreloader::styles() const
{
	return QStringList() << "LV_PRELOAD_STYLE_MAIN";
}

lv_style_t *LVGLPreloader::style(lv_obj_t *obj, int type) const
{
	return const_cast<lv_style_t*>(lv_preload_get_style(obj, type & 0xff));
}

void LVGLPreloader::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const
{
	lv_preload_set_style(obj, static_cast<lv_preload_style_t>(type), style);
}

lv_style_t *LVGLPreloader::defaultStyle(int type) const
{
	if (type == LV_PRELOAD_STYLE_MAIN)
		return &lv_style_pretty_color;
	return nullptr;
}
