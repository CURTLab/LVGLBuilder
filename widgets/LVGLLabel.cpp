#include "LVGLLabel.h"

#include <QIcon>
#include "LVGLObject.h"
#include "properties/LVGLPropertyColor.h"

class LVGLPropertyLabelText : public LVGLPropertyString
{
public:
	QString name() const { return "Text"; }

	QStringList function(LVGLObject *obj) const {
		return QStringList() << QString("lv_label_set_text(%1, \"%2\");").arg(obj->codeName()).arg(get(obj));
	}

protected:
	QString get(LVGLObject *obj) const { return lv_label_get_text(obj->obj()); }
	void set(LVGLObject *obj, QString string) { lv_label_set_text(obj->obj(), qPrintable(string)); }

};

class LVGLPropertyLabelAlign : public LVGLPropertyEnum
{
public:
	LVGLPropertyLabelAlign()
		: LVGLPropertyEnum(QStringList() << "Left" << "Center" << "Right")
		, m_values({"LV_LABEL_ALIGN_LEFT", "LV_LABEL_ALIGN_CENTER", "LV_LABEL_ALIGN_RIGHT"})
	{}

	QString name() const { return "Align"; }

	QStringList function(LVGLObject *obj) const {
		if (get(obj) == LV_LABEL_ALIGN_LEFT) return QStringList();
		return QStringList() << QString("lv_label_set_align(%1, %2);").arg(obj->codeName()).arg(m_values.at(get(obj)));
	}

protected:
	int get(LVGLObject *obj) const { return lv_label_get_align(obj->obj()); }
	void set(LVGLObject *obj, int index) { lv_label_set_align(obj->obj(), index & 0xff); }

	QStringList m_values;
};

class LVGLPropertyLabelLongMode : public LVGLPropertyEnum
{
public:
	LVGLPropertyLabelLongMode()
		: LVGLPropertyEnum(QStringList() << "Expand" << "Break" << "Dot" << "Scroll" << "Circular scroll" << "Corp")
		, m_values({"LV_LABEL_LONG_EXPAND", "LV_LABEL_LONG_BREAK", "LV_LABEL_LONG_DOT", "LV_LABEL_LONG_SROLL",
						"LV_LABEL_LONG_SROLL_CIRC", "LV_LABEL_LONG_CROP"})
	{}

	QString name() const { return "Long mode"; }

	QStringList function(LVGLObject *obj) const {
		if (get(obj) == LV_LABEL_LONG_EXPAND) return QStringList();
		return QStringList() << QString("lv_label_set_long_mode(%1, %2);").arg(obj->codeName()).arg(m_values.at(get(obj)));
	}

protected:
	int get(LVGLObject *obj) const { return lv_label_get_long_mode(obj->obj()); }
	void set(LVGLObject *obj, int index) { lv_label_set_long_mode(obj->obj(), index & 0xff); }

	QStringList m_values;
};

class LVGLPropertyLabelRecolor : public LVGLPropertyBool
{
public:
	QString name() const { return "Recolor"; }

	QStringList function(LVGLObject *obj) const {
		if (!get(obj)) return QStringList();
		return QStringList() << QString("lv_label_set_recolor(%1, %2);").arg(obj->codeName()).arg(QVariant(get(obj)).toString());
	}

protected:
	bool get(LVGLObject *obj) const { return lv_label_get_recolor(obj->obj()); }
	void set(LVGLObject *obj, bool boolean) { lv_label_set_recolor(obj->obj(), boolean); }
};

class LVGLPropertyLabelBodyDraw : public LVGLPropertyBool
{
public:
	QString name() const { return "Body draw"; }

	QStringList function(LVGLObject *obj) const {
		if (!get(obj)) return QStringList();
		return QStringList() << QString("lv_label_set_body_draw(%1, %2);").arg(obj->codeName()).arg(QVariant(get(obj)).toString());
	}

protected:
	bool get(LVGLObject *obj) const { return lv_label_get_body_draw(obj->obj()); }
	void set(LVGLObject *obj, bool boolean) { lv_label_set_body_draw(obj->obj(), boolean); }
};

LVGLLabel::LVGLLabel()
{
	m_properties << new LVGLPropertyLabelAlign;
	m_properties << new LVGLPropertyLabelLongMode;
	m_properties << new LVGLPropertyLabelRecolor;
	m_properties << new LVGLPropertyLabelBodyDraw;
	m_properties << new LVGLPropertyLabelText;

	// swap geometry in order to stop autosize
	const int index = m_properties.indexOf(m_geometryProp);
	qSwap(m_properties[index], m_properties.last());

	m_editableStyles << LVGL::Text; // LV_LABEL_STYLE_MAIN
}

QString LVGLLabel::name() const
{
	return "Label";
}

QString LVGLLabel::className() const
{
	return "lv_label";
}

LVGLWidget::Type LVGLLabel::type() const
{
	return Label;
}

QPixmap LVGLLabel::preview() const
{
	return QPixmap();
}

QIcon LVGLLabel::icon() const
{
	return QIcon();
}

lv_obj_t *LVGLLabel::newObject(lv_obj_t *parent) const
{
	lv_obj_t *obj = lv_label_create(parent, nullptr);
	return obj;
}

QSize LVGLLabel::minimumSize() const
{
	return QSize(100, 35);
}

QStringList LVGLLabel::styles() const
{
	return QStringList() << "LV_LABEL_STYLE_MAIN";
}

lv_style_t *LVGLLabel::style(lv_obj_t *obj, int type) const
{
	return const_cast<lv_style_t*>(lv_label_get_style(obj, type & 0xff));
}

void LVGLLabel::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const
{
	lv_label_set_style(obj, static_cast<lv_label_style_t>(type), style);
}

lv_style_t *LVGLLabel::defaultStyle(int type) const
{
	Q_UNUSED(type)
	return nullptr;
}
