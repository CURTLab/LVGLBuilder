#include "LVGLCalendar.h"

#include <QIcon>
#include "LVGLObject.h"
#include "properties/LVGLPropertyDate.h"
#include "properties/LVGLPropertyDateList.h"
#include "properties/LVGLPropertyTextList.h"
#include "LVGLCore.h"

class LVGLPropertyDayNames : public LVGLPropertyTextList
{
public:
	inline LVGLPropertyDayNames() : LVGLPropertyTextList(false) {}
	inline ~LVGLPropertyDayNames() {
		for (const char **d:m_garbageCollector) {
			for (uint8_t i = 0; i < N; ++i)
				delete [] d[i];
			delete [] d;
		}
	}

	inline QString name() const override { return "Day names"; }

	inline QStringList function(LVGLObject *obj) const override
	{
		const QStringList list = get(obj);
		if (!isDifferent(list))
			return {};
		QStringList days;
		for (uint8_t i = 0; i < N; ++i)
			days << "\"" + list[i] + "\"";
		QStringList ret;
		const QString varName = QString("days_%1").arg(obj->codeName());
		ret << QString("static const char *%1[7] = {%2};").arg(varName).arg(days.join(", "));
		ret << QString("lv_calendar_set_day_names(%1, %2);").arg(obj->codeName()).arg(varName);
		return ret;
	}

protected:
	QList<const char **> m_garbageCollector;
	static constexpr uint8_t N = 7;

	inline bool isDifferent(QStringList list) const {
		for (uint8_t i = 0; i < N; ++i) {
			if (list[i] != LVGLCore::DEFAULT_DAYS[i])
				return true;
		}
		return false;
	}

	inline QStringList get(LVGLObject *obj) const override {
		const char **names = lv_calendar_get_day_names(obj->obj());
		if (names == nullptr)
			return {"Su", "Mo", "Tu", "We", "Th", "Fr", "Sa"};
		QStringList ret;
		for (uint8_t i = 0; i < N; ++i)
			ret << QString(names[i]);
		return ret;
	}

	inline void set(LVGLObject *obj, QStringList list) override {
		if (!isDifferent(list))
			return;
		const char **data = new const char *[N];
		for (uint8_t i = 0; i < N; ++i) {
			const QString &s = list[i];
			char *string = new char[s.size()+1];
			memcpy(string, qPrintable(s), s.size());
			string[s.size()] = '\0';
			data[i] = string;
		}
		m_garbageCollector << data;
		lv_calendar_set_day_names(obj->obj(), data);
	}

};

class LVGLPropertyMonthNames : public LVGLPropertyTextList
{
public:
	inline LVGLPropertyMonthNames() : LVGLPropertyTextList(false) {}
	inline ~LVGLPropertyMonthNames() {
		for (const char **d:m_garbageCollector) {
			for (uint8_t i = 0; i < N; ++i)
				delete [] d[i];
			delete [] d;
		}
	}

	inline QString name() const override { return "Month names"; }

	inline QStringList function(LVGLObject *obj) const override
	{
		const QStringList list = get(obj);
		if (!isDifferent(list))
			return {};
		QStringList months;
		for (uint8_t i = 0; i < N; ++i)
			months << "\"" + list[i] + "\"";
		QStringList ret;
		const QString varName = QString("months_%1").arg(obj->codeName());
		ret << QString("static const char *%1[12] = {%2};").arg(varName).arg(months.join(", "));
		ret << QString("lv_calendar_set_month_names(%1, %2);").arg(obj->codeName()).arg(varName);
		return ret;
	}

protected:
	QList<const char **> m_garbageCollector;
	static constexpr uint8_t N = 12;
	inline bool isDifferent(QStringList list) const {
		for (uint8_t i = 0; i < N; ++i) {
			if (list[i] != LVGLCore::DEFAULT_MONTHS[i])
				return true;
		}
		return false;
	}

	inline QStringList get(LVGLObject *obj) const override {
		const char **names = lv_calendar_get_month_names(obj->obj());
		if (names == nullptr)
			names = LVGLCore::DEFAULT_MONTHS;
		QStringList ret;
		for (uint8_t i = 0; i < N; ++i)
			ret << QString(names[i]);
		return ret;
	}

	inline void set(LVGLObject *obj, QStringList list) override {
		if (!isDifferent(list))
			return;
		const char **data = new const char *[N];
		for (uint8_t i = 0; i < N; ++i) {
			const QString &s = list[i];
			char *string = new char[s.size()+1];
			memcpy(string, qPrintable(s), s.size());
			string[s.size()] = '\0';
			data[i] = string;
		}
		m_garbageCollector << data;
		lv_calendar_set_month_names(obj->obj(), data);
	}

};

LVGLCalendar::LVGLCalendar()
{
	m_properties << new LVGLPropertyDate("Current date", "lv_calendar_set_today_date", lv_calendar_set_today_date, lv_calendar_get_today_date);
	m_properties << new LVGLPropertyDate("Shown date", "lv_calendar_set_showed_date", lv_calendar_set_showed_date, lv_calendar_get_showed_date);
	m_properties << new LVGLPropertyDateList("Highlighted dates", "lv_calendar_set_highlighted_dates",
														  lv_calendar_set_highlighted_dates, lv_calendar_get_highlighted_dates, lv_calendar_get_highlighted_dates_num);

	m_properties << new LVGLPropertyDayNames;
	m_properties << new LVGLPropertyMonthNames;

	m_editableStyles << LVGL::StyleParts(LVGL::Body | LVGL::Text); // LV_CALENDAR_STYLE_BG
	m_editableStyles << LVGL::StyleParts(LVGL::Body | LVGL::Text); // LV_CALENDAR_STYLE_HEADER
	m_editableStyles << LVGL::StyleParts(LVGL::Body | LVGL::Text); // LV_CALENDAR_STYLE_HEADER_PR
	m_editableStyles << LVGL::Text; // LV_CALENDAR_STYLE_DAY_NAMES
	m_editableStyles << LVGL::Text; // LV_CALENDAR_STYLE_HIGHLIGHTED_DAYS
	m_editableStyles << LVGL::Text; // LV_CALENDAR_STYLE_INACTIVE_DAYS
	m_editableStyles << LVGL::Body; // LV_CALENDAR_STYLE_WEEK_BOX
	m_editableStyles << LVGL::StyleParts(LVGL::Body | LVGL::Text); // LV_CALENDAR_STYLE_TODAY_BOX
}

QString LVGLCalendar::name() const
{
	return "Calendar";
}

QString LVGLCalendar::className() const
{
	return "lv_calendar";
}

LVGLWidget::Type LVGLCalendar::type() const
{
	return Calendar;
}

QIcon LVGLCalendar::icon() const
{
	return QIcon();
}

lv_obj_t *LVGLCalendar::newObject(lv_obj_t *parent) const
{
	lv_obj_t *obj = lv_calendar_create(parent, nullptr);
	return obj;
}

QSize LVGLCalendar::minimumSize() const
{
	return QSize(250, 200);
}

QStringList LVGLCalendar::styles() const
{
	return QStringList() << "LV_CALENDAR_STYLE_BG"
								<< "LV_CALENDAR_STYLE_HEADER"
								<< "LV_CALENDAR_STYLE_HEADER_PR"
								<< "LV_CALENDAR_STYLE_DAY_NAMES"
								<< "LV_CALENDAR_STYLE_HIGHLIGHTED_DAYS"
								<< "LV_CALENDAR_STYLE_INACTIVE_DAYS"
								<< "LV_CALENDAR_STYLE_WEEK_BOX"
								<< "LV_CALENDAR_STYLE_TODAY_BOX";
}

lv_style_t *LVGLCalendar::style(lv_obj_t *obj, int type) const
{
	return const_cast<lv_style_t*>(lv_calendar_get_style(obj, type & 0xff));
}

void LVGLCalendar::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const
{
	lv_calendar_set_style(obj, static_cast<lv_bar_style_t>(type), style);
}

lv_style_t *LVGLCalendar::defaultStyle(int type) const
{
	if (type == LV_CALENDAR_STYLE_BG)
		return &lv_style_pretty;
	else if (type == LV_CALENDAR_STYLE_HEADER)
		return &lv_style_plain_color;
	else if (type == LV_CALENDAR_STYLE_HEADER_PR)
		return &lv_style_pretty_color;
	else if (type == LV_CALENDAR_STYLE_DAY_NAMES)
		return &lv_style_pretty;
	else if (type == LV_CALENDAR_STYLE_HIGHLIGHTED_DAYS)
		return &lv_style_plain_color;
	else if (type == LV_CALENDAR_STYLE_INACTIVE_DAYS)
		return &lv_style_btn_ina;
	else if (type == LV_CALENDAR_STYLE_WEEK_BOX)
		return &lv_style_plain_color;
	else if (type == LV_CALENDAR_STYLE_TODAY_BOX)
		return &lv_style_pretty_color;
	return nullptr;
}
