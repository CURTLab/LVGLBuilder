#include "LVGLButtonMatrix.h"

#include <QIcon>

#include "LVGLObject.h"

LVGLButtonMatrix::LVGLButtonMatrix()
{

	m_editableStyles << LVGL::Body; // LV_BTNM_STYLE_BG
	m_editableStyles << LVGL::Body; // LV_BTNM_STYLE_BTN_REL
	m_editableStyles << LVGL::Body; // LV_BTNM_STYLE_BTN_PR
	m_editableStyles << LVGL::Body; // LV_BTNM_STYLE_BTN_TGL_REL
	m_editableStyles << LVGL::Body; // LV_BTNM_STYLE_BTN_TGL_PR
	m_editableStyles << LVGL::Body; // LV_BTNM_STYLE_BTN_INA
}

QString LVGLButtonMatrix::name() const
{
	return "Button Matrix";
}

QString LVGLButtonMatrix::className() const
{
	return "lv_btnm";
}

LVGLWidget::Type LVGLButtonMatrix::type() const
{
	return ButtonMatrix;
}

QPixmap LVGLButtonMatrix::preview() const
{
	return QPixmap();
}

QIcon LVGLButtonMatrix::icon() const
{
	return QIcon();
}

lv_obj_t *LVGLButtonMatrix::newObject(lv_obj_t *parent) const
{
	lv_obj_t *obj = lv_btnm_create(parent, nullptr);
	return obj;
}

QSize LVGLButtonMatrix::minimumSize() const
{
	return QSize(150, 100);
}

QStringList LVGLButtonMatrix::styles() const
{
	return QStringList() << "LV_BTNM_STYLE_BG"
								<< "LV_BTNM_STYLE_BTN_REL"
								<< "LV_BTNM_STYLE_BTN_PR"
								<< "LV_BTNM_STYLE_BTN_TGL_REL"
								<< "LV_BTNM_STYLE_BTN_TGL_PR"
								<< "LV_BTNM_STYLE_BTN_INA";
}

lv_style_t *LVGLButtonMatrix::style(lv_obj_t *obj, int type) const
{
	return const_cast<lv_style_t*>(lv_btnm_get_style(obj, type & 0xff));
}

void LVGLButtonMatrix::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const
{
	lv_btnm_set_style(obj, static_cast<lv_btn_style_t>(type), style);
}

lv_style_t *LVGLButtonMatrix::defaultStyle(int type) const
{
	if (type == LV_BTNM_STYLE_BG)
		return &lv_style_pretty;
	else if (type == LV_BTNM_STYLE_BTN_REL)
		return &lv_style_btn_rel;
	else if (type == LV_BTNM_STYLE_BTN_PR)
		return &lv_style_btn_pr;
	else if (type == LV_BTNM_STYLE_BTN_TGL_REL)
		return &lv_style_btn_tgl_rel;
	else if (type == LV_BTNM_STYLE_BTN_TGL_PR)
		return &lv_style_btn_tgl_pr;
	else if (type == LV_BTNM_STYLE_BTN_INA)
		return &lv_style_btn_ina;
	return nullptr;
}
