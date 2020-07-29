#include "LVGLKeyboard.h"

#include <QIcon>
#include "LVGLObject.h"
#include "properties/LVGLPropertyAssignTextArea.h"

class LVGLPropertyKBMode : public LVGLPropertyEnum
{
public:
	LVGLPropertyKBMode()
		: LVGLPropertyEnum({"Text", "Number", "Text Upper"})
		, m_values({"LV_KB_MODE_TEXT", "LV_KB_MODE_NUM", "LV_KB_MODE_TEXT_UPPER"})
	{}

	QString name() const { return "Mode"; }

	QStringList function(LVGLObject *obj) const {
		return QStringList() << QString("lv_kb_set_mode(%1, %2);").arg(obj->codeName()).arg(m_values.at(get(obj)));
	}

protected:
	int get(LVGLObject *obj) const { return lv_kb_get_mode(obj->obj()); }
	void set(LVGLObject *obj, int index) { lv_kb_set_mode(obj->obj(), index & 0xff); }

	QStringList m_values;
};

class LVGLPropertyLBCursorManager : public LVGLPropertyBool
{
public:
	QString name() const { return "Cursor Manager"; }

	QStringList function(LVGLObject *obj) const {
		return QStringList() << QString("lv_kb_set_cursor_manage(%1, %2);").arg(obj->codeName()).arg(QVariant(get(obj)).toString());
	}

protected:
	bool get(LVGLObject *obj) const { return lv_kb_get_cursor_manage(obj->obj()); }
	void set(LVGLObject *obj, bool statue) { lv_kb_set_cursor_manage(obj->obj(), statue); }
};


LVGLKeyboard::LVGLKeyboard()
{
	m_properties << new LVGLPropertyKBMode;
	m_properties << new LVGLPropertyAssignTextArea;
	m_properties << new LVGLPropertyLBCursorManager;

	m_editableStyles << LVGL::Body; // LV_KB_STYLE_BG
	m_editableStyles << LVGL::StyleParts(LVGL::Body | LVGL::Text); // LV_KB_STYLE_BTN_REL
	m_editableStyles << LVGL::StyleParts(LVGL::Body | LVGL::Text); // LV_KB_STYLE_BTN_PR
	m_editableStyles << LVGL::StyleParts(LVGL::Body | LVGL::Text); // LV_KB_STYLE_BTN_TGL_REL
	m_editableStyles << LVGL::StyleParts(LVGL::Body | LVGL::Text); // LV_KB_STYLE_BTN_TGL_PR
	m_editableStyles << LVGL::StyleParts(LVGL::Body | LVGL::Text); // LV_KB_STYLE_BTN_INA
}

QString LVGLKeyboard::name() const
{
	return "Keyboard";
}

QString LVGLKeyboard::className() const
{
	return "lv_kb";
}

LVGLWidget::Type LVGLKeyboard::type() const
{
	return Keyboard;
}

QIcon LVGLKeyboard::icon() const
{
	return QIcon();
}

lv_obj_t *LVGLKeyboard::newObject(lv_obj_t *parent) const
{
	lv_obj_t *obj = lv_kb_create(parent, nullptr);
	/*static const char *kb_map[] = {
		"q", "w", "e", "r", "t", "z", "u", "i", "o", "p", "\n",
		"a", "s", "d", "f", "g", "h", "j", "k", "l", "\n",
		"ABC", "y", "x", "c", "v", "b", "n", "m", LV_SYMBOL_BACKSPACE, "\n",
		"#1", ".", "/", ".com", LV_SYMBOL_NEW_LINE, ""
	};
	lv_kb_set_map(obj, kb_map);
	static const lv_btnm_ctrl_t kb_ctrl_map[] = {

	};
	lv_kb_set_ctrl_map(obj, );*/
	return obj;
}

QSize LVGLKeyboard::minimumSize() const
{
	return QSize(200, 80);
}

QStringList LVGLKeyboard::styles() const
{
	return QStringList() << "LV_KB_STYLE_BG"
								<< "LV_KB_STYLE_BTN_REL"
								<< "LV_KB_STYLE_BTN_PR"
								<< "LV_KB_STYLE_BTN_TGL_REL"
								<< "LV_KB_STYLE_BTN_TGL_PR"
								<< "LV_KB_STYLE_BTN_INA";
}

lv_style_t *LVGLKeyboard::style(lv_obj_t *obj, int type) const
{
	return const_cast<lv_style_t*>(lv_kb_get_style(obj, type & 0xff));
}

void LVGLKeyboard::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const
{
	lv_kb_set_style(obj, static_cast<lv_kb_style_t>(type), style);
}

lv_style_t *LVGLKeyboard::defaultStyle(int type) const
{
	if (type == LV_KB_STYLE_BG)
		return &lv_style_pretty;
	else if (type == LV_KB_STYLE_BTN_REL)
		return &lv_style_btn_rel;
	else if (type == LV_KB_STYLE_BTN_PR)
		return &lv_style_btn_pr;
	else if (type == LV_KB_STYLE_BTN_TGL_REL)
		return &lv_style_btn_tgl_rel;
	else if (type == LV_KB_STYLE_BTN_TGL_PR)
		return &lv_style_btn_tgl_pr;
	else if (type == LV_KB_STYLE_BTN_INA)
		return &lv_style_btn_ina;
	return nullptr;
}
