#include "LVGLCheckBox.h"

#include <QIcon>

#include "LVGLObject.h"

class LVGLPropertyCBText : public LVGLPropertyString
{
public:
	QString name() const { return "Text"; }

protected:
	QString get(LVGLObject *obj) const { return lv_cb_get_text(obj->obj()); }
	void set(LVGLObject *obj, QString string) { lv_cb_set_text(obj->obj(), qPrintable(string)); }
};

class LVGLPropertyCBChecked : public LVGLPropertyBool
{
public:
	QString name() const { return "Checked"; }

protected:
	bool get(LVGLObject *obj) const { return lv_cb_is_checked(obj->obj()); }
	void set(LVGLObject *obj, bool boolean) { lv_cb_set_checked(obj->obj(), boolean); }
};

class LVGLPropertyCBInactive : public LVGLPropertyBool
{
public:
	QString name() const { return "Inactive"; }

protected:
	bool get(LVGLObject *obj) const { return lv_cb_is_inactive(obj->obj()); }
	void set(LVGLObject *obj, bool boolean) { lv_btn_set_state(obj->obj(), boolean ? LV_BTN_STATE_INA : LV_BTN_STATE_TGL_REL); }
};

LVGLCheckBox::LVGLCheckBox()
{
	m_properties << new LVGLPropertyCBText;
	m_properties << new LVGLPropertyCBChecked;
	m_properties << new LVGLPropertyCBInactive;

	m_editableStyles << LVGL::Body; // LV_CB_STYLE_BG
	m_editableStyles << LVGL::Body; // LV_CB_STYLE_BOX_REL
	m_editableStyles << LVGL::Body; // LV_CB_STYLE_BOX_PR
	m_editableStyles << LVGL::Body; // LV_CB_STYLE_BOX_TGL_REL
	m_editableStyles << LVGL::Body; // LV_CB_STYLE_BOX_TGL_PR
	m_editableStyles << LVGL::Body; // LV_CB_STYLE_BOX_INA
}

QString LVGLCheckBox::name() const
{
	return "Check box";
}

QString LVGLCheckBox::className() const
{
	return "lv_cb";
}

LVGLWidget::Type LVGLCheckBox::type() const
{
	return CheckBox;
}

QIcon LVGLCheckBox::icon() const
{
	return QIcon();
}

lv_obj_t *LVGLCheckBox::newObject(lv_obj_t *parent) const
{
	lv_obj_t *obj = lv_cb_create(parent, nullptr);
	return obj;
}

QSize LVGLCheckBox::minimumSize() const
{
	return QSize(100, 35);
}

QStringList LVGLCheckBox::styles() const
{
	return QStringList() << "LV_CB_STYLE_BG"
								<< "LV_CB_STYLE_BOX_REL"
								<< "LV_CB_STYLE_BOX_PR"
								<< "LV_CB_STYLE_BOX_TGL_REL"
								<< "LV_CB_STYLE_BOX_TGL_PR"
								<< "LV_CB_STYLE_BOX_INA";
}

lv_style_t *LVGLCheckBox::style(lv_obj_t *obj, int type) const
{
	return const_cast<lv_style_t*>(lv_cb_get_style(obj, type & 0xff));
}

void LVGLCheckBox::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const
{
	lv_cb_set_style(obj, static_cast<lv_cb_style_t>(type), style);
}

lv_style_t *LVGLCheckBox::defaultStyle(int type) const
{
	if (type == LV_CB_STYLE_BG)
		return &lv_style_transp;
	else if (type == LV_CB_STYLE_BOX_REL)
		return &lv_style_btn_rel;
	else if (type == LV_CB_STYLE_BOX_PR)
		return &lv_style_btn_pr;
	else if (type == LV_CB_STYLE_BOX_TGL_REL)
		return &lv_style_btn_tgl_rel;
	else if (type == LV_CB_STYLE_BOX_TGL_PR)
		return &lv_style_btn_tgl_pr;
	else if (type == LV_CB_STYLE_BOX_INA)
		return &lv_style_btn_ina;
	return nullptr;
}
