#include "LVGLPage.h"

#include <QIcon>

#include "LVGLObject.h"

class LVGLPropertyPageScrollbars : public LVGLPropertyEnum
{
public:
	LVGLPropertyPageScrollbars()
		: LVGLPropertyEnum(QStringList() << "Off" << "On" << "Drag" << "Auto")
		, m_values({"LV_SB_MODE_OFF", "LV_SB_MODE_ON", "LV_SB_MODE_DRAG", "LV_SB_MODE_AUTO"})
	{}

	QString name() const { return "Scrollbars"; }

	QStringList function(LVGLObject *obj) const {
		return QStringList() << QString("lv_page_set_sb_mode(%1, %2);").arg(obj->codeName()).arg(m_values.at(get(obj)));
	}

protected:
	int get(LVGLObject *obj) const { return lv_page_get_sb_mode(obj->obj()) & 0x03; }
	void set(LVGLObject *obj, int index) { lv_page_set_sb_mode(obj->obj(), index & 0xff); }

	QStringList m_values;
};

class LVGLPropertyPageWidth : public LVGLPropertyCoord
{
public:
	inline LVGLPropertyPageWidth(LVGLProperty *p = nullptr) : LVGLPropertyCoord(Qt::Horizontal, p) {}
	inline QString name() const override { return "Scroll width"; }

	inline lv_coord_t get(LVGLObject *obj) const override { return lv_page_get_scrl_width(obj->obj()); }
	inline void set(LVGLObject *obj, lv_coord_t value) override { lv_page_set_scrl_width(obj->obj(), value); }

};

class LVGLPropertyPageHeight : public LVGLPropertyCoord
{
public:
	inline LVGLPropertyPageHeight(LVGLProperty *p = nullptr) : LVGLPropertyCoord(Qt::Vertical, p) {}
	inline QString name() const override { return "Scroll height"; }

	inline lv_coord_t get(LVGLObject *obj) const override { return lv_page_get_scrl_height(obj->obj()); }
	inline void set(LVGLObject *obj, lv_coord_t value) override { lv_page_set_scrl_height(obj->obj(), value); }

};

class LVGLPropertyPageEdgeFlash : public LVGLPropertyBool
{
public:
	QString name() const { return "Edge flash"; }

	QStringList function(LVGLObject *obj) const {
		return QStringList() << QString("lv_page_set_edge_flash(%1, %2);").arg(obj->codeName()).arg(QVariant(get(obj)).toString());
	}

protected:
	bool get(LVGLObject *obj) const { return lv_page_get_edge_flash(obj->obj()); }
	void set(LVGLObject *obj, bool boolean) { lv_page_set_edge_flash(obj->obj(), boolean); }
};

class LVGLPropertyPageScrollPropagation : public LVGLPropertyBool
{
public:
	QString name() const { return "Scroll propagation"; }

	QStringList function(LVGLObject *obj) const {
		return QStringList() << QString("lv_page_set_scroll_propagation(%1, %2);").arg(obj->codeName()).arg(QVariant(get(obj)).toString());
	}

protected:
	bool get(LVGLObject *obj) const { return lv_page_get_scroll_propagation(obj->obj()); }
	void set(LVGLObject *obj, bool boolean) { lv_page_set_scroll_propagation(obj->obj(), boolean); }
};

LVGLPage::LVGLPage()
{
	m_properties << new LVGLPropertyPageScrollbars;
	m_properties << new LVGLPropertyPageWidth;
	m_properties << new LVGLPropertyPageHeight;
	m_properties << new LVGLPropertyPageEdgeFlash;
	m_properties << new LVGLPropertyPageScrollPropagation;

	m_editableStyles << LVGL::Body; // LV_PAGE_STYLE_BG
	m_editableStyles << LVGL::Body; // LV_PAGE_STYLE_SCRL
	m_editableStyles << LVGL::Body; // LV_PAGE_STYLE_SB
}

QString LVGLPage::name() const
{
	return "Page";
}

QString LVGLPage::className() const
{
	return "lv_page";
}

LVGLWidget::Type LVGLPage::type() const
{
	return Page;
}

QPixmap LVGLPage::preview() const
{
	return QPixmap();
}

QIcon LVGLPage::icon() const
{
	return QIcon();
}

lv_obj_t *LVGLPage::newObject(lv_obj_t *parent) const
{
	lv_obj_t *obj = lv_page_create(parent, nullptr);
	return obj;
}

QSize LVGLPage::minimumSize() const
{
	return QSize(100, 35);
}

QStringList LVGLPage::styles() const
{
	return QStringList() << "LV_PAGE_STYLE_BG"
								<< "LV_PAGE_STYLE_SCRL"
								<< "LV_PAGE_STYLE_SB";
}

lv_style_t *LVGLPage::style(lv_obj_t *obj, int type) const
{
	return const_cast<lv_style_t*>(lv_page_get_style(obj, type & 0xff));
}

void LVGLPage::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const
{
	lv_page_set_style(obj, static_cast<lv_btn_style_t>(type), style);
}

lv_style_t *LVGLPage::defaultStyle(int type) const
{
	if (type == LV_PAGE_STYLE_BG)
		return &lv_style_pretty_color;
	else if (type == LV_PAGE_STYLE_SCRL)
		return &lv_style_pretty;
	else if (type == LV_PAGE_STYLE_SB)
		return &lv_style_pretty_color;
	return nullptr;
}
