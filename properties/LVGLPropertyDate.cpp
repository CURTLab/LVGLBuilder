#include "LVGLPropertyDate.h"

#include <QDateTimeEdit>

LVGLPropertyDate::LVGLPropertyDate(LVGLProperty *parent)
	: LVGLProperty(parent)
{
}

bool LVGLPropertyDate::hasEditor() const
{
	return true;
}

QWidget *LVGLPropertyDate::editor(QWidget *parent)
{
	m_widget = new QDateTimeEdit(parent);
	m_widget->setCalendarPopup(true);
	m_widget->setDisplayFormat("dd.MM.yyyy");
	return m_widget;
}

void LVGLPropertyDate::updateEditor(LVGLObject *obj)
{
	lv_calendar_date_t *date = get(obj);
	m_widget->setDate(QDate(date->year, date->month, date->day));
}

void LVGLPropertyDate::updateWidget(LVGLObject *obj)
{
	const QDate date = m_widget->date();
	lv_calendar_date_t d;
	d.day = static_cast<int8_t>(date.day());
	d.month = static_cast<int8_t>(date.month());
	d.year = static_cast<uint16_t>(date.year());
	set(obj, &d);
}

QVariant LVGLPropertyDate::value(LVGLObject *obj) const
{
	lv_calendar_date_t *date = get(obj);
	return QDate(date->year, date->month, date->day);
}

void LVGLPropertyDate::setValue(LVGLObject *obj, QVariant value)
{
	const QDate date = value.toDate();
	lv_calendar_date_t d;
	d.day = static_cast<int8_t>(date.day());
	d.month = static_cast<int8_t>(date.month());
	d.year = static_cast<uint16_t>(date.year());
	set(obj, &d);
}
