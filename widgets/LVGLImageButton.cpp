#include "LVGLImageButton.h"

#include <QIcon>

#include "LVGLCore.h"
#include "LVGLObject.h"
#include "properties/LVGLPropertyImage.h"

class LVGLPropertyImgBtnSrc : public LVGLPropertyImage
{
public:
	LVGLPropertyImgBtnSrc(lv_btn_state_t state, LVGLProperty *parent = nullptr)
		: LVGLPropertyImage(parent)
		, m_values({"Released", "Pressed", "Toggled released", "Toggled pressed", "Inactive"})
		, m_types({"LV_BTN_STATE_REL", "LV_BTN_STATE_PR", "LV_BTN_STATE_TGL_REL", "LV_BTN_STATE_TGL_PR", "LV_BTN_STATE_INA"})
		, m_state(state)
	{
	}

	QString name() const override { return "Source " + m_values.at(m_state); }

	QStringList function(LVGLObject *obj) const override {
		LVGLImageData *img = lvgl.imageByDesc(get(obj));
		if (img == nullptr) return QStringList();
		return QStringList() << QString("lv_imgbtn_set_src(%1, %2, &%3);").arg(obj->codeName()).arg(m_types.at(m_state)).arg(img->codeName());
	}

protected:
	QStringList m_values;
	QStringList m_types;
	lv_btn_state_t m_state;

	virtual const lv_img_dsc_t *get(LVGLObject *obj) const override { return reinterpret_cast<const lv_img_dsc_t*>(lv_imgbtn_get_src(obj->obj(), m_state)); }
	virtual void set(LVGLObject *obj, const lv_img_dsc_t *img) override { lv_imgbtn_set_src(obj->obj(), m_state, img); }

};

class LVGLPropertyImgBtnState : public LVGLPropertyEnum
{
public:
	LVGLPropertyImgBtnState()
		: LVGLPropertyEnum(QStringList() << "Released" << "Pressed" << "Toggled released" << "Toggled pressed" << "Inactive")
		, m_values({"LV_BTN_STATE_REL", "LV_BTN_STATE_PR", "LV_BTN_STATE_TGL_REL", "LV_BTN_STATE_TGL_PR", "LV_BTN_STATE_INA"})
	{}

	QString name() const { return "State"; }

	QStringList function(LVGLObject *obj) const {
		return QStringList() << QString("lv_imgbtn_set_state(%1, %2);").arg(obj->codeName()).arg(m_values.at(get(obj)));
	}

protected:
	int get(LVGLObject *obj) const { return lv_imgbtn_get_state(obj->obj()); }
	void set(LVGLObject *obj, int index) { lv_imgbtn_set_state(obj->obj(), index & 0xff); }

	QStringList m_values;
};

class LVGLPropertyImgBtnToggle : public LVGLPropertyBool
{
public:
	QString name() const { return "Toggle"; }

	QStringList function(LVGLObject *obj) const {
		return QStringList() << QString("lv_imgbtn_set_toggle(%1, %2);").arg(obj->codeName()).arg(QVariant(get(obj)).toString());
	}

protected:
	bool get(LVGLObject *obj) const { return lv_imgbtn_get_toggle(obj->obj()); }
	void set(LVGLObject *obj, bool statue) { lv_imgbtn_set_toggle(obj->obj(), statue); }
};

LVGLImageButton::LVGLImageButton()
{
	m_properties << new LVGLPropertyImgBtnState;
	m_properties << new LVGLPropertyImgBtnToggle;
	m_properties << new LVGLPropertyImgBtnSrc(LV_IMGBTN_STYLE_REL);
	m_properties << new LVGLPropertyImgBtnSrc(LV_IMGBTN_STYLE_PR);
	m_properties << new LVGLPropertyImgBtnSrc(LV_IMGBTN_STYLE_TGL_REL);
	m_properties << new LVGLPropertyImgBtnSrc(LV_IMGBTN_STYLE_TGL_PR);
	m_properties << new LVGLPropertyImgBtnSrc(LV_IMGBTN_STYLE_INA);

	m_editableStyles << LVGL::Image; // LV_IMGBTN_STYLE_REL
	m_editableStyles << LVGL::Image; // LV_IMGBTN_STYLE_PR
	m_editableStyles << LVGL::Image; // LV_IMGBTN_STYLE_TGL_REL
	m_editableStyles << LVGL::Image; // LV_IMGBTN_STYLE_TGL_PR
	m_editableStyles << LVGL::Image; // LV_IMGBTN_STYLE_INA
}

QString LVGLImageButton::name() const
{
	return "Image button";
}

QString LVGLImageButton::className() const
{
	return "lv_imgbtn";
}

LVGLWidget::Type LVGLImageButton::type() const
{
	return ImageButton;
}

QPixmap LVGLImageButton::preview() const
{
	return QPixmap();
}

QIcon LVGLImageButton::icon() const
{
	return QIcon();
}

QSize LVGLImageButton::minimumSize() const
{
	return QSize(100, 35);
}

lv_obj_t *LVGLImageButton::newObject(lv_obj_t *parent) const
{
	lv_obj_t *obj = lv_imgbtn_create(parent, nullptr);
	/*for (const LVGLProperty &p:m_properties)
		p.set(obj, p.defaultVal);*/
	return obj;
}

QStringList LVGLImageButton::styles() const
{
	return QStringList() << "LV_IMGBTN_STYLE_REL"
								<< "LV_IMGBTN_STYLE_PR"
								<< "LV_IMGBTN_STYLE_TGL_REL"
								<< "LV_IMGBTN_STYLE_TGL_PR"
								<< "LV_IMGBTN_STYLE_INA";
}

lv_style_t *LVGLImageButton::style(lv_obj_t *obj, int type) const
{
	return const_cast<lv_style_t*>(lv_imgbtn_get_style(obj, type & 0xff));
}

void LVGLImageButton::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const
{
	lv_imgbtn_set_style(obj, static_cast<lv_imgbtn_style_t>(type), style);
}

lv_style_t *LVGLImageButton::defaultStyle(int type) const
{
	if (type == LV_IMGBTN_STYLE_REL)
		return &lv_style_btn_rel;
	else if (type == LV_IMGBTN_STYLE_PR)
		return &lv_style_btn_pr;
	else if (type == LV_IMGBTN_STYLE_TGL_REL)
		return &lv_style_btn_tgl_rel;
	else if (type == LV_IMGBTN_STYLE_TGL_PR)
		return &lv_style_btn_tgl_pr;
	else if (type == LV_IMGBTN_STYLE_INA)
		return &lv_style_btn_ina;
	return nullptr;
}
