#ifndef LVGLPROPERTYDATE_H
#define LVGLPROPERTYDATE_H

#include "LVGLProperty.h"

class LVGLPropertyDate : public LVGLProperty
{
public:
	LVGLPropertyDate(QString title, QString functionName,
						  std::function<void(lv_obj_t*, lv_calendar_date_t*)> setter,
						  std::function<lv_calendar_date_t*(lv_obj_t*)> getter,
						  LVGLProperty *parent = nullptr);

	bool hasEditor() const override;
	QWidget *editor(QWidget *parent) override;
	void updateEditor(LVGLObject *obj) override;
	void updateWidget(LVGLObject *obj) override;

	QVariant value(LVGLObject *obj) const override;
	void setValue(LVGLObject *obj, QVariant value) override;

	QString name() const override;
	QStringList function(LVGLObject *obj) const override;

protected:
	virtual lv_calendar_date_t *get(LVGLObject *obj) const;
	virtual void set(LVGLObject *obj, lv_calendar_date_t *value);

	class QDateTimeEdit *m_widget;
	QString m_title;
	QString m_functionName;
	std::function<void(lv_obj_t*, lv_calendar_date_t*)> m_setter;
	std::function<lv_calendar_date_t*(lv_obj_t*)> m_getter;

};

#endif // LVGLPROPERTYDATE_H
