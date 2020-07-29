#include "LVGLList.h"

#include <QIcon>
#include "LVGLObject.h"
#include "properties/LVGLPropertyList.h"

class LVGLPropertyListSingleMode : public LVGLPropertyBool
{
public:
	QString name() const { return "Single mode"; }

	QStringList function(LVGLObject *obj) const {
		return QStringList() << QString("lv_list_set_single_mode(%1, %2);").arg(obj->codeName()).arg(QVariant(get(obj)).toString());
	}

protected:
	bool get(LVGLObject *obj) const { return lv_list_get_single_mode(obj->obj()); }
	void set(LVGLObject *obj, bool boolean) { lv_list_set_single_mode(obj->obj(), boolean); }
};

class LVGLPropertyListScrollPropagation : public LVGLPropertyBool
{
public:
	QString name() const { return "Scroll propagation"; }

	QStringList function(LVGLObject *obj) const {
		return QStringList() << QString("lv_list_set_scroll_propagation(%1, %2);").arg(obj->codeName()).arg(QVariant(get(obj)).toString());
	}

protected:
	bool get(LVGLObject *obj) const { return lv_list_get_scroll_propagation(obj->obj()); }
	void set(LVGLObject *obj, bool boolean) { lv_list_set_scroll_propagation(obj->obj(), boolean); }
};

class LVGLPropertyListEdgeFlash : public LVGLPropertyBool
{
public:
	QString name() const { return "Edge flash"; }

	QStringList function(LVGLObject *obj) const {
		return QStringList() << QString("lv_list_set_edge_flash(%1, %2);").arg(obj->codeName()).arg(QVariant(get(obj)).toString());
	}

protected:
	bool get(LVGLObject *obj) const { return lv_list_get_edge_flash(obj->obj()); }
	void set(LVGLObject *obj, bool boolean) { lv_list_set_edge_flash(obj->obj(), boolean); }
};

LVGLList::LVGLList()
{
	m_properties << new LVGLPropertyList;
	m_properties << new LVGLPropertyListSingleMode;
	m_properties << new LVGLPropertyListScrollPropagation;
	m_properties << new LVGLPropertyListEdgeFlash;

	m_editableStyles << LVGL::Body; // LV_LIST_STYLE_BG
	m_editableStyles << LVGL::Body; // LV_LIST_STYLE_SCRL
	m_editableStyles << LVGL::Body; // LV_LIST_STYLE_SB
	m_editableStyles << LVGL::StyleParts(LVGL::Body | LVGL::Text | LVGL::Image); // LV_LIST_STYLE_BTN_REL
	m_editableStyles << LVGL::StyleParts(LVGL::Body | LVGL::Text | LVGL::Image); // LV_LIST_STYLE_BTN_PR
	m_editableStyles << LVGL::StyleParts(LVGL::Body | LVGL::Text | LVGL::Image); // LV_LIST_STYLE_BTN_TGL_REL
	m_editableStyles << LVGL::StyleParts(LVGL::Body | LVGL::Text | LVGL::Image); // LV_LIST_STYLE_BTN_TGL_PR
	m_editableStyles << LVGL::StyleParts(LVGL::Body | LVGL::Text | LVGL::Image); // LV_LIST_STYLE_BTN_INA
}

QString LVGLList::name() const
{
	return "List";
}

QString LVGLList::className() const
{
	return "lv_list";
}

LVGLWidget::Type LVGLList::type() const
{
	return List;
}

QIcon LVGLList::icon() const
{
	return QIcon();
}

lv_obj_t *LVGLList::newObject(lv_obj_t *parent) const
{
	lv_obj_t *obj = lv_list_create(parent, nullptr);
	return obj;
}

QSize LVGLList::minimumSize() const
{
	return QSize(75, 60);
}

QStringList LVGLList::styles() const
{
	return QStringList() << "LV_LIST_STYLE_BG"
								<< "LV_LIST_STYLE_SCRL"
								<< "LV_LIST_STYLE_SB"
								<< "LV_LIST_STYLE_EDGE_FLASH"
								<< "LV_LIST_STYLE_BTN_REL"
								<< "LV_LIST_STYLE_BTN_PR"
								<< "LV_LIST_STYLE_BTN_TGL_REL"
								<< "LV_LIST_STYLE_BTN_TGL_PR"
								<< "LV_LIST_STYLE_BTN_INA";
}

lv_style_t *LVGLList::style(lv_obj_t *obj, int type) const
{
	return const_cast<lv_style_t*>(lv_list_get_style(obj, type & 0xff));
}

void LVGLList::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const
{
	lv_list_set_style(obj, static_cast<lv_list_style_t>(type), style);
}

lv_style_t *LVGLList::defaultStyle(int type) const
{
	if (type == LV_LIST_STYLE_BG)
		return &lv_style_transp_fit;
	else if (type == LV_LIST_STYLE_SCRL)
		return &lv_style_pretty;
	else if (type == LV_LIST_STYLE_SB)
		return &lv_style_pretty_color;
	else if (type == LV_LIST_STYLE_BTN_REL)
		return &lv_style_btn_rel;
	else if (type == LV_LIST_STYLE_BTN_PR)
		return &lv_style_btn_pr;
	else if (type == LV_LIST_STYLE_BTN_TGL_REL)
		return &lv_style_btn_tgl_rel;
	else if (type == LV_LIST_STYLE_BTN_TGL_PR)
		return &lv_style_btn_tgl_pr;
	else if (type == LV_LIST_STYLE_BTN_INA)
		return &lv_style_btn_ina;
	return nullptr;
}
