#include "LVGLTextArea.h"

#include <QIcon>
#include "LVGLObject.h"

class LVGLPropertyTAText : public LVGLPropertyString
{
public:
	QString name() const { return "Text"; }

	QStringList function(LVGLObject *obj) const {
		return QStringList() << QString("lv_ta_set_text(%1, \"%2\");").arg(obj->codeName()).arg(get(obj));
	}

protected:
	QString get(LVGLObject *obj) const { return lv_ta_get_text(obj->obj()); }
	void set(LVGLObject *obj, QString string) { lv_ta_set_text(obj->obj(), qPrintable(string)); }
};

class LVGLPropertyTAPlaceholder : public LVGLPropertyString
{
public:
	QString name() const { return "Placeholder"; }

	QStringList function(LVGLObject *obj) const {
		return QStringList() << QString("lv_ta_set_placeholder_text(%1, \"%2\");").arg(obj->codeName()).arg(get(obj));
	}

protected:
	QString get(LVGLObject *obj) const { return lv_ta_get_placeholder_text(obj->obj()); }
	void set(LVGLObject *obj, QString string) { lv_ta_set_placeholder_text(obj->obj(), qPrintable(string)); }
};

class LVGLPropertyTACursorPos : public LVGLPropertyInt
{
public:
	LVGLPropertyTACursorPos() : LVGLPropertyInt(0, LV_TA_CURSOR_LAST) {}

	QString name() const { return "Cursor position"; }

protected:
	int get(LVGLObject *obj) const { return lv_ta_get_cursor_pos(obj->obj()); }
	void set(LVGLObject *obj, int value) { lv_ta_set_cursor_pos(obj->obj(), static_cast<int16_t>(value)); }
};

class LVGLPropertyTACursorType : public LVGLPropertyEnum
{
public:
	LVGLPropertyTACursorType()
		: LVGLPropertyEnum(QStringList() << "None" << "Line" << "Block" << "Outline" << "Underline")
		, m_values({"LV_CURSOR_NONE", "LV_CURSOR_LINE", "LV_CURSOR_BLOCK", "LV_CURSOR_OUTLINE", "LV_CURSOR_UNDERLINE"})
	{}

	QString name() const { return "Cursor type"; }

	QStringList function(LVGLObject *obj) const {
		return QStringList() << QString("lv_ta_set_cursor_type(%1, %2);").arg(obj->codeName()).arg(m_values.at(get(obj)));
	}

protected:
	int get(LVGLObject *obj) const { return lv_ta_get_cursor_type(obj->obj()) & 0x7f; }
	void set(LVGLObject *obj, int index) { lv_ta_set_cursor_type(obj->obj(), index & 0x7f); }

	QStringList m_values;
};

class LVGLPropertyTACursorBlinkTime : public LVGLPropertyInt
{
public:
	LVGLPropertyTACursorBlinkTime() : LVGLPropertyInt(0, UINT16_MAX) {}

	QString name() const { return "Cursor blink time"; }

	QStringList function(LVGLObject *obj) const {
		return QStringList() << QString("lv_ta_set_cursor_blink_time(%1, %2);").arg(obj->codeName()).arg(get(obj));
	}

protected:
	int get(LVGLObject *obj) const { return lv_ta_get_cursor_blink_time(obj->obj()); }
	void set(LVGLObject *obj, int index) { lv_ta_set_cursor_blink_time(obj->obj(), index & 0xff); }
};

class LVGLPropertyTAOneLineMode : public LVGLPropertyBool
{
public:
	QString name() const { return "One line mode"; }

	QStringList function(LVGLObject *obj) const {
		return QStringList() << QString("lv_ta_set_one_line(%1, %2);").arg(obj->codeName()).arg(QVariant(get(obj)).toString());
	}

protected:
	bool get(LVGLObject *obj) const { return lv_ta_get_one_line(obj->obj()); }
	void set(LVGLObject *obj, bool statue) { lv_ta_set_one_line(obj->obj(), statue); }
};

class LVGLPropertyTAPasswordMode : public LVGLPropertyBool
{
public:
	QString name() const { return "Password mode"; }

	QStringList function(LVGLObject *obj) const {
		return QStringList() << QString("lv_ta_set_pwd_mode(%1, %2);").arg(obj->codeName()).arg(QVariant(get(obj)).toString());
	}

protected:
	bool get(LVGLObject *obj) const { return lv_ta_get_pwd_mode(obj->obj()); }
	void set(LVGLObject *obj, bool statue) { lv_ta_set_pwd_mode(obj->obj(), statue); }
};

class LVGLPropertyTAMaxLen : public LVGLPropertyInt
{
public:
	LVGLPropertyTAMaxLen() : LVGLPropertyInt(0, LV_TA_CURSOR_LAST-1) {}

	QStringList function(LVGLObject *obj) const {
		return QStringList() << QString("lv_ta_set_max_length(%1, %2);").arg(obj->codeName()).arg(get(obj));
	}

	QString name() const { return "Max length"; }

protected:
	int get(LVGLObject *obj) const { return lv_ta_get_max_length(obj->obj()); }
	void set(LVGLObject *obj, int index) { lv_ta_set_max_length(obj->obj(), index & 0xff); }
};

class LVGLPropertyTATextAlign : public LVGLPropertyEnum
{
public:
	LVGLPropertyTATextAlign()
		: LVGLPropertyEnum(QStringList() << "Left" << "Center" << "Right") {}

	QString name() const { return "Text align"; }

protected:
	int get(LVGLObject *obj) const { return lv_label_get_align(lv_ta_get_label(obj->obj()));  }
	void set(LVGLObject *obj, int index) { lv_ta_set_text_align(obj->obj(), index & 0xff); }
};

class LVGLPropertyTAScrollbars : public LVGLPropertyEnum
{
public:
	LVGLPropertyTAScrollbars()
		: LVGLPropertyEnum(QStringList() << "Off" << "On" << "Drag" << "Auto") {}

	QString name() const { return "Scrollbars"; }

protected:
	int get(LVGLObject *obj) const { return lv_ta_get_sb_mode(lv_ta_get_label(obj->obj())) & 0x3;  }
	void set(LVGLObject *obj, int index) { lv_ta_set_sb_mode(obj->obj(), index & 0xff); }
};

class LVGLPropertyTAScrollPropagation : public LVGLPropertyBool
{
public:
	QString name() const { return "Scroll propagation"; }

	QStringList function(LVGLObject *obj) const {
		return QStringList() << QString("lv_ta_set_scroll_propagation(%1, %2);").arg(obj->codeName()).arg(QVariant(get(obj)).toString());
	}

protected:
	bool get(LVGLObject *obj) const { return lv_ta_get_scroll_propagation(obj->obj()); }
	void set(LVGLObject *obj, bool boolean) { lv_ta_set_scroll_propagation(obj->obj(), boolean); }
};

class LVGLPropertyTAEdgeFlash : public LVGLPropertyBool
{
public:
	QString name() const { return "Edge flash"; }

	QStringList function(LVGLObject *obj) const {
		return QStringList() << QString("lv_ta_set_edge_flash(%1, %2);").arg(obj->codeName()).arg(QVariant(get(obj)).toString());
	}

protected:
	bool get(LVGLObject *obj) const { return lv_ta_get_edge_flash(obj->obj()); }
	void set(LVGLObject *obj, bool boolean) { lv_ta_set_edge_flash(obj->obj(), boolean); }
};

LVGLTextArea::LVGLTextArea()
{
	m_properties << new LVGLPropertyTAText;
	m_properties << new LVGLPropertyTAPlaceholder;
	m_properties << new LVGLPropertyTACursorPos;
	m_properties << new LVGLPropertyTACursorType;
	m_properties << new LVGLPropertyTACursorBlinkTime;
	m_properties << new LVGLPropertyTAOneLineMode;
	m_properties << new LVGLPropertyTAPasswordMode;
	m_properties << new LVGLPropertyTAMaxLen;
	m_properties << new LVGLPropertyTATextAlign;
	m_properties << new LVGLPropertyTAScrollbars;
	m_properties << new LVGLPropertyTAScrollPropagation;
	m_properties << new LVGLPropertyTAEdgeFlash;

	m_editableStyles << LVGL::StyleParts(LVGL::Body | LVGL::Text); // LV_TA_STYLE_BG
	m_editableStyles << LVGL::Body; // LV_TA_STYLE_SB
	m_editableStyles << LVGL::StyleParts(LVGL::BodyPadding | LVGL::Line); // LV_TA_STYLE_CURSOR
	m_editableStyles << LVGL::Body; // LV_TA_STYLE_EDGE_FLASH
	m_editableStyles << LVGL::StyleParts(LVGL::Body | LVGL::Text); // LV_TA_STYLE_PLACEHOLDER
}

QString LVGLTextArea::name() const
{
	return "Text area";
}

QString LVGLTextArea::className() const
{
	return "lv_ta";
}

LVGLWidget::Type LVGLTextArea::type() const
{
	return TextArea;
}

QIcon LVGLTextArea::icon() const
{
	return QIcon();
}

lv_obj_t *LVGLTextArea::newObject(lv_obj_t *parent) const
{
	lv_obj_t *obj = lv_ta_create(parent, nullptr);
	return obj;
}

QSize LVGLTextArea::minimumSize() const
{
	return QSize(100, 30);
}

QStringList LVGLTextArea::styles() const
{
	return QStringList() << "LV_TA_STYLE_BG"
								<< "LV_TA_STYLE_SB"
								<< "LV_TA_STYLE_CURSOR"
								<< "LV_TA_STYLE_EDGE_FLASH"
								<< "LV_TA_STYLE_PLACEHOLDER";
}

lv_style_t *LVGLTextArea::style(lv_obj_t *obj, int type) const
{
	return const_cast<lv_style_t*>(lv_ta_get_style(obj, type & 0xff));
}

void LVGLTextArea::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const
{
	lv_ta_set_style(obj, static_cast<lv_ta_style_t>(type), style);
}

lv_style_t *LVGLTextArea::defaultStyle(int type) const
{
	if (type == LV_TA_STYLE_BG)
		return &lv_style_pretty;
	else if (type == LV_TA_STYLE_SB)
		return &lv_style_pretty_color;
	return nullptr;
}
