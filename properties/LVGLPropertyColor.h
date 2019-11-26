#ifndef LVGLPROPERTYCOLOR_H
#define LVGLPROPERTYCOLOR_H

#include "LVGLProperty.h"
#include <QColor>

class LVGLPropertyColor : public LVGLProperty
{
public:
	LVGLPropertyColor(LVGLProperty *parent = nullptr);

	bool hasEditor() const;
	QWidget *editor(QWidget *parent);
	void updateEditor(LVGLObject *obj);
	void updateWidget(LVGLObject *obj);

	QVariant value(LVGLObject *obj) const;
	void setValue(LVGLObject *obj, QVariant value);

	QJsonValue toJson(LVGLObject *obj) const;

protected:
	virtual lv_color_t get(LVGLObject *obj) const = 0;
	virtual void set(LVGLObject *obj, lv_color_t value) = 0;

	QColor m_color;
	class QColorDialog *m_widget;

};

#endif // LVGLPROPERTYCOLOR_H
