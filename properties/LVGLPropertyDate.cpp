#include "LVGLPropertyDate.h"

#include <QDateTimeEdit>

#include "core/LVGLObject.h"

LVGLPropertyDate::LVGLPropertyDate(
    QString title, QString functionName,
    std::function<void(lv_obj_t *, lv_calendar_date_t *)> setter,
    std::function<lv_calendar_date_t *(lv_obj_t *)> getter,
    LVGLProperty *parent)
    : LVGLProperty(parent),
      m_widget(nullptr),
      m_title(title),
      m_functionName(functionName),
      m_setter(setter),
      m_getter(getter) {}

bool LVGLPropertyDate::hasEditor() const { return true; }

QWidget *LVGLPropertyDate::editor(QWidget *parent) {
  m_widget = new QDateTimeEdit(parent);
  m_widget->setCalendarPopup(true);
  m_widget->setDisplayFormat("dd.MM.yyyy");
  return m_widget;
}

void LVGLPropertyDate::updateEditor(LVGLObject *obj) {
  lv_calendar_date_t *date = get(obj);
  m_widget->setDate(QDate(date->year, date->month, date->day));
}

void LVGLPropertyDate::updateWidget(LVGLObject *obj) {
  const QDate date = m_widget->date();
  lv_calendar_date_t d;
  d.day = static_cast<int8_t>(date.day());
  d.month = static_cast<int8_t>(date.month());
  d.year = static_cast<uint16_t>(date.year());
  set(obj, &d);
}

QVariant LVGLPropertyDate::value(LVGLObject *obj) const {
  lv_calendar_date_t *date = get(obj);
  return QDate(date->year, date->month, date->day);
}

void LVGLPropertyDate::setValue(LVGLObject *obj, QVariant value) {
  const QDate date = value.toDate();
  lv_calendar_date_t d;
  d.day = static_cast<int8_t>(date.day());
  d.month = static_cast<int8_t>(date.month());
  d.year = static_cast<uint16_t>(date.year());
  set(obj, &d);
}

QString LVGLPropertyDate::name() const { return m_title; }

QStringList LVGLPropertyDate::function(LVGLObject *obj) const {
  if (m_functionName.isEmpty()) return {};
  QStringList ret;
  lv_calendar_date_t *data = get(obj);
  const QString varName = QString("date_%1_%2")
                              .arg(m_title.toLower().replace(' ', '_'))
                              .arg(obj->codeName());
  ret << QString(
             "lv_calendar_date_t %1 = {.year = %2, .month = %3, .day = %4};")
             .arg(varName)
             .arg(data->year)
             .arg(data->month)
             .arg(data->day);
  ret << QString("%1(%2, &%3);")
             .arg(m_functionName)
             .arg(obj->codeName())
             .arg(varName);
  return ret;
}

lv_calendar_date_t *LVGLPropertyDate::get(LVGLObject *obj) const {
  return m_getter(obj->obj());
}

void LVGLPropertyDate::set(LVGLObject *obj, lv_calendar_date_t *value) {
  m_setter(obj->obj(), value);
}
