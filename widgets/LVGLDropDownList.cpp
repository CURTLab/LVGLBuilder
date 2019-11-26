#include "LVGLDropDownList.h"

#include <QIcon>

#include "LVGLObject.h"

class LVGLPropertyDDListAlign : public LVGLPropertyEnum
{
public:
	LVGLPropertyDDListAlign() : LVGLPropertyEnum(QStringList() << "Left" << "Center" << "Right") {}
	QString name() const { return "Align"; }

protected:
	int get(LVGLObject *obj) const { return lv_ddlist_get_align(obj->obj()); }
	void set(LVGLObject *obj, int index) { lv_ddlist_set_align(obj->obj(), index & 0xff); }
};

class LVGLPropertyDDListFixedWidth : public LVGLPropertyCoord
{
public:
	inline LVGLPropertyDDListFixedWidth(LVGLProperty *p = nullptr) : LVGLPropertyCoord(Qt::Vertical, p) {}
	inline QString name() const override { return "Fixed width"; }

protected:
	inline lv_coord_t get(LVGLObject *obj) const override { return lv_obj_get_width(obj->obj()); }
	inline void set(LVGLObject *obj, lv_coord_t value) override { lv_ddlist_set_fix_width(obj->obj(), value); }

};

class LVGLPropertyDDListFixedHeight : public LVGLPropertyCoord
{
public:
	inline LVGLPropertyDDListFixedHeight(LVGLProperty *p = nullptr) : LVGLPropertyCoord(Qt::Vertical, p) {}
	inline QString name() const override { return "Fixed height"; }

protected:
	inline lv_coord_t get(LVGLObject *obj) const override { return lv_ddlist_get_fix_height(obj->obj()); }
	inline void set(LVGLObject *obj, lv_coord_t value) override { lv_ddlist_set_fix_height(obj->obj(), value); }

};

class LVGLPropertyDDListScrollbars : public LVGLPropertyEnum
{
public:
	LVGLPropertyDDListScrollbars() : LVGLPropertyEnum(QStringList() << "Off"  << "On" << "Drag" << "Auto") {}
	QString name() const { return "Scrollbars"; }

protected:
	int get(LVGLObject *obj) const { return lv_ddlist_get_sb_mode(obj->obj()) & 0x3; }
	void set(LVGLObject *obj, int index) { lv_ddlist_set_sb_mode(obj->obj(), index & 0x3); }
};

class LVGLPropertyDDListAnimationTime : public LVGLPropertyInt
{
public:
	LVGLPropertyDDListAnimationTime() : LVGLPropertyInt(0, UINT16_MAX, " ms") {}

	QString name() const { return "Animation time"; }

protected:
	int get(LVGLObject *obj) const { return lv_ddlist_get_anim_time(obj->obj()); }
	void set(LVGLObject *obj, int value) { lv_ddlist_set_anim_time(obj->obj(), static_cast<uint16_t>(value)); }
};

class LVGLPropertyDDListDrawArrow : public LVGLPropertyBool
{
public:
	QString name() const { return "Decoration arrow"; }

protected:
	bool get(LVGLObject *obj) const { return lv_ddlist_get_draw_arrow(obj->obj()); }
	void set(LVGLObject *obj, bool statue) { lv_ddlist_set_draw_arrow(obj->obj(), statue); }
};

class LVGLPropertyDDListStayOpen : public LVGLPropertyBool
{
public:
	QString name() const { return "Stay open"; }

protected:
	bool get(LVGLObject *obj) const { return lv_ddlist_get_stay_open(obj->obj()); }
	void set(LVGLObject *obj, bool statue) { lv_ddlist_set_stay_open(obj->obj(), statue); }
};

LVGLDropDownList::LVGLDropDownList()
{
	m_properties << new LVGLPropertyDDListAlign;
	m_properties << new LVGLPropertyDDListFixedWidth;
	m_properties << new LVGLPropertyDDListFixedHeight;
	m_properties << new LVGLPropertyDDListScrollbars;
	m_properties << new LVGLPropertyDDListAnimationTime;
	m_properties << new LVGLPropertyDDListDrawArrow;
	m_properties << new LVGLPropertyDDListStayOpen;

	m_editableStyles << LVGL::StyleParts(LVGL::Body | LVGL::Text); // LV_DDLIST_STYLE_BG
	m_editableStyles << LVGL::StyleParts(LVGL::Body | LVGL::Text); // LV_DDLIST_STYLE_SEL
	m_editableStyles << LVGL::Body; // LV_DDLIST_STYLE_SB
}

QString LVGLDropDownList::name() const
{
	return "Drop down list";
}

QString LVGLDropDownList::className() const
{
	return "lv_ddlist";
}

LVGLWidget::Type LVGLDropDownList::type() const
{
	return DropDownList;
}

QPixmap LVGLDropDownList::preview() const
{
	return QPixmap();
}

QIcon LVGLDropDownList::icon() const
{
	return QIcon();
}

lv_obj_t *LVGLDropDownList::newObject(lv_obj_t *parent) const
{
	lv_obj_t *obj = lv_ddlist_create(parent, nullptr);
	return obj;
}

QSize LVGLDropDownList::minimumSize() const
{
	return QSize(100, 35);
}

QStringList LVGLDropDownList::styles() const
{
	return QStringList() << "LV_DDLIST_STYLE_BG"
								<< "LV_DDLIST_STYLE_SEL"
								<< "LV_DDLIST_STYLE_SB";
}

lv_style_t *LVGLDropDownList::style(lv_obj_t *obj, int type) const
{
	return const_cast<lv_style_t*>(lv_ddlist_get_style(obj, type & 0xff));
}

void LVGLDropDownList::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const
{
	lv_ddlist_set_style(obj, static_cast<lv_btn_style_t>(type), style);
}

lv_style_t *LVGLDropDownList::defaultStyle(int type) const
{
	if (type == LV_DDLIST_STYLE_BG)
		return &lv_style_pretty;
	else if (type == LV_DDLIST_STYLE_SEL)
		return &lv_style_plain_color;
	else if (type == LV_DDLIST_STYLE_SB)
		return &lv_style_plain_color;
	return nullptr;
}
