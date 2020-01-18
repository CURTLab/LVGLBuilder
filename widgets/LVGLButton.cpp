#include "LVGLButton.h"

#include <QIcon>

#include "LVGLObject.h"

class LVGLPropertyButtonState : public LVGLPropertyEnum
{
public:
	LVGLPropertyButtonState()
		: LVGLPropertyEnum({"Released", "Pressed", "Toggled released", "Toggled pressed", "Inactive"})
		, m_values({"LV_BTN_STATE_REL", "LV_BTN_STATE_PR", "LV_BTN_STATE_TGL_REL", "LV_BTN_STATE_TGL_PR", "LV_BTN_STATE_INA"})
	{}

	QString name() const { return "State"; }

	QStringList function(LVGLObject *obj) const {
		if (get(obj) != LV_BTN_STATE_REL)
			return QStringList() << QString("lv_btn_set_state(%1, %2);").arg(obj->codeName()).arg(m_values.at(get(obj)));
		return QStringList();
	}

protected:
	int get(LVGLObject *obj) const { return lv_btn_get_state(obj->obj()); }
	void set(LVGLObject *obj, int index) { lv_btn_set_state(obj->obj(), index & 0xff); }

	QStringList m_values;
};

class LVGLPropertyButtonLayout : public LVGLPropertyEnum
{
public:
	LVGLPropertyButtonLayout()
		: LVGLPropertyEnum({"Off", "Center", "Column left align", "Column middle align", "Column right align",
								 "Row top align", "Row middle align", "Row bottom align", "Pretty", "Grid"})
		, m_values({"LV_LAYOUT_OFF", "LV_LAYOUT_CENTER", "LV_LAYOUT_COL_L", "LV_LAYOUT_COL_M", "LV_LAYOUT_COL_R",
						"LV_LAYOUT_ROW_T", "LV_LAYOUT_ROW_M", "LV_LAYOUT_ROW_B", "LV_LAYOUT_PRETTY", "LV_LAYOUT_GRID"})
	{}

	QString name() const { return "Layout"; }

	QStringList function(LVGLObject *obj) const {
		if (get(obj) != LV_LAYOUT_CENTER)
			return QStringList() << QString("lv_btn_set_layout(%1, %2);").arg(obj->codeName()).arg(m_values.at(get(obj)));
		return QStringList();
	}

protected:
	int get(LVGLObject *obj) const { return lv_btn_get_layout(obj->obj()); }
	void set(LVGLObject *obj, int index) { lv_btn_set_layout(obj->obj(), index & 0xff); }

	QStringList m_values;
};

class LVGLPropertyButtonToggle : public LVGLPropertyBool
{
public:
	QString name() const { return "Toggle"; }

	QStringList function(LVGLObject *obj) const {
		if (!get(obj)) return QStringList();
		return QStringList() << QString("lv_btn_set_toggle(%1, %2);").arg(obj->codeName()).arg(QVariant(get(obj)).toString());
	}

protected:
	bool get(LVGLObject *obj) const { return lv_btn_get_toggle(obj->obj()); }
	void set(LVGLObject *obj, bool statue) { lv_btn_set_toggle(obj->obj(), statue); }
};

LVGLButton::LVGLButton()
{
	m_properties << new LVGLPropertyButtonState;
	m_properties << new LVGLPropertyButtonToggle;
	m_properties << new LVGLPropertyButtonLayout;

	m_editableStyles << LVGL::Body; // LV_BTN_STYLE_REL
	m_editableStyles << LVGL::Body; // LV_BTN_STYLE_PR
	m_editableStyles << LVGL::Body; // LV_BTN_STYLE_TGL_REL
	m_editableStyles << LVGL::Body; // LV_BTN_STYLE_TGL_PR
	m_editableStyles << LVGL::Body; // LV_BTN_STYLE_INA
}

QString LVGLButton::name() const
{
	return "Button";
}

QString LVGLButton::className() const
{
	return "lv_btn";
}

LVGLWidget::Type LVGLButton::type() const
{
	return Button;
}

QPixmap LVGLButton::preview() const
{
	return QPixmap();
}

QIcon LVGLButton::icon() const
{
	return QIcon();
}

lv_obj_t *LVGLButton::newObject(lv_obj_t *parent) const
{
	lv_obj_t *obj = lv_btn_create(parent, nullptr);
	return obj;
}

QSize LVGLButton::minimumSize() const
{
	return QSize(100, 35);
}

QStringList LVGLButton::styles() const
{
	return QStringList() << "LV_BTN_STYLE_REL"
								<< "LV_BTN_STYLE_PR"
								<< "LV_BTN_STYLE_TGL_REL"
								<< "LV_BTN_STYLE_TGL_PR"
								<< "LV_BTN_STYLE_INA";
}

lv_style_t *LVGLButton::style(lv_obj_t *obj, int type) const
{
	return const_cast<lv_style_t*>(lv_btn_get_style(obj, type & 0xff));
}

void LVGLButton::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const
{
	lv_btn_set_style(obj, static_cast<lv_btn_style_t>(type), style);
}

lv_style_t *LVGLButton::defaultStyle(int type) const
{
	if (type == LV_BTN_STYLE_REL)
		return &lv_style_btn_rel;
	else if (type == LV_BTN_STYLE_PR)
		return &lv_style_btn_pr;
	else if (type == LV_BTN_STYLE_TGL_REL)
		return &lv_style_btn_tgl_rel;
	else if (type == LV_BTN_STYLE_TGL_PR)
		return &lv_style_btn_tgl_pr;
	else if (type == LV_BTN_STYLE_INA)
		return &lv_style_btn_ina;
	return nullptr;
}
