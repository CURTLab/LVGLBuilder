#ifndef LVGLPROPERTYDATE_H
#define LVGLPROPERTYDATE_H

#include "LVGLProperty.h"

class LVGLPropertyDate : public LVGLProperty
{
public:
	LVGLPropertyDate(LVGLProperty *parent = nullptr);

	bool hasEditor() const;
	QWidget *editor(QWidget *parent);
	void updateEditor(LVGLObject *obj);
	void updateWidget(LVGLObject *obj);

	QVariant value(LVGLObject *obj) const;
	void setValue(LVGLObject *obj, QVariant value);

protected:
	virtual lv_calendar_date_t *get(LVGLObject *obj) const = 0;
	virtual void set(LVGLObject *obj, lv_calendar_date_t *value) = 0;

	class QDateTimeEdit *m_widget;

};

#endif // LVGLPROPERTYDATE_H
