#include "LVGLMessageBox.h"

#include <QIcon>

#include "LVGLObject.h"
#include "properties/LVGLPropertyTextList.h"


class LVGLPropertyMBoxButtons : public LVGLPropertyTextList
{
public:
	inline QString name() const override { return "Buttons"; }
	inline ~LVGLPropertyMBoxButtons()
	{
	}

protected:
	inline QStringList get(LVGLObject *obj) const override {
		QStringList ret;
		lv_obj_t *btnm = reinterpret_cast<lv_mbox_ext_t*>(lv_obj_get_ext_attr(obj->obj()))->btnm;
		if (btnm == nullptr)
			return {};

		lv_btnm_ext_t *ext = reinterpret_cast<lv_btnm_ext_t*>(lv_obj_get_ext_attr(btnm));
		for (uint16_t i = 0; i < ext->btn_cnt; ++i)
			ret << QString(ext->map_p[i]);
		return ret;
	}

	inline void set(LVGLObject *obj, QStringList list) override {
		char **map = new char*[list.size() + 1];
		for (int i = 0; i < list.size(); ++i) {
			map[i] = new char[list[i].size() + 1];
			memcpy(map[i], qPrintable(list[i]), list[i].size());
			map[i][list[i].size()] = '\0';
		}
		map[list.size()] = new char[1];
		map[list.size()][0] = '\0';
		lv_mbox_add_btns(obj->obj(), const_cast<const char**>(map));
	}

};

LVGLMessageBox::LVGLMessageBox()
{
	m_properties << new LVGLPropertyMBoxButtons;
	m_properties << new LVGLPropertyString("Text", "lv_mbox_set_text", lv_mbox_set_text, lv_mbox_get_text);
	m_properties << new LVGLPropertyValUInt16(0, UINT16_MAX, "Animation time", "lv_mbox_set_anim_time",
															lv_mbox_set_anim_time, lv_mbox_get_anim_time);
	m_properties << new LVGLPropertyBool("Recolor", "lv_mbox_set_recolor", lv_mbox_set_recolor, lv_mbox_get_recolor);

	m_editableStyles << LVGL::StyleParts(LVGL::Body | LVGL::Text) // LV_MBOX_STYLE_BG
						  << LVGL::Body // LV_MBOX_STYLE_BTN_BG
						  << LVGL::Body // LV_MBOX_STYLE_BTN_REL
						  << LVGL::Body // LV_MBOX_STYLE_BTN_PR
						  << LVGL::Body // LV_MBOX_STYLE_BTN_TGL_REL
						  << LVGL::Body // LV_MBOX_STYLE_BTN_TGL_PR
						  << LVGL::Body;// LV_MBOX_STYLE_BTN_INA
}

QString LVGLMessageBox::name() const
{
	return "Message box";
}

QString LVGLMessageBox::className() const
{
	return "lv_mbox";
}

LVGLWidget::Type LVGLMessageBox::type() const
{
	return MessageBox;
}

QIcon LVGLMessageBox::icon() const
{
	return QIcon();
}

lv_obj_t *LVGLMessageBox::newObject(lv_obj_t *parent) const
{
	lv_obj_t *obj = lv_mbox_create(parent, nullptr);
	return obj;
}

QSize LVGLMessageBox::minimumSize() const
{
	return {200, 85};
}

QStringList LVGLMessageBox::styles() const
{
	return {"LV_MBOX_STYLE_BG",
			  "LV_MBOX_STYLE_BTN_BG",
			  "LV_MBOX_STYLE_BTN_REL",
			  "LV_MBOX_STYLE_BTN_PR",
			  "LV_MBOX_STYLE_BTN_TGL_REL",
			  "LV_MBOX_STYLE_BTN_TGL_PR",
			  "LV_MBOX_STYLE_BTN_INA"};
}

lv_style_t *LVGLMessageBox::style(lv_obj_t *obj, int type) const
{
	return const_cast<lv_style_t*>(lv_mbox_get_style(obj, type & 0xff));
}

void LVGLMessageBox::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const
{
	lv_mbox_set_style(obj, static_cast<lv_mbox_style_t>(type), style);
}

lv_style_t *LVGLMessageBox::defaultStyle(int type) const
{
	if (type == LV_MBOX_STYLE_BG)
		return &lv_style_pretty;
	else if (type == LV_MBOX_STYLE_BTN_BG)
		return &lv_style_transp;
	else if (type == LV_MBOX_STYLE_BTN_REL)
		return &lv_style_btn_rel;
	else if (type == LV_MBOX_STYLE_BTN_PR)
		return &lv_style_btn_pr;
	else if (type == LV_MBOX_STYLE_BTN_TGL_REL)
		return &lv_style_btn_tgl_rel;
	else if (type == LV_MBOX_STYLE_BTN_TGL_PR)
		return &lv_style_btn_tgl_pr;
	else if (type == LV_MBOX_STYLE_BTN_INA)
		return &lv_style_btn_ina;
	return nullptr;
}
