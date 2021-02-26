#include "LVGLCalendar.h"

#include <QIcon>

#include "LVGLCore.h"
#include "LVGLObject.h"
#include "properties/LVGLPropertyDate.h"
#include "properties/LVGLPropertyDateList.h"
#include "properties/LVGLPropertyTextList.h"

class LVGLPropertyDayNames : public LVGLPropertyTextList {
 public:
  inline LVGLPropertyDayNames() : LVGLPropertyTextList(false) {}
  inline ~LVGLPropertyDayNames() {
    for (const char **d : m_garbageCollector) {
      for (uint8_t i = 0; i < N; ++i) delete[] d[i];
      delete[] d;
    }
  }

  inline QString name() const override { return "Day names"; }

  inline QStringList function(LVGLObject *obj) const override {
    const QStringList list = get(obj);
    if (!isDifferent(list)) return {};
    QStringList days;
    for (uint8_t i = 0; i < N; ++i) days << "\"" + list[i] + "\"";
    QStringList ret;
    const QString varName = QString("days_%1").arg(obj->codeName());
    ret << QString("static const char *%1[7] = {%2};")
               .arg(varName)
               .arg(days.join(", "));
    ret << QString("lv_calendar_set_day_names(%1, %2);")
               .arg(obj->codeName())
               .arg(varName);
    return ret;
  }

 protected:
  QList<const char **> m_garbageCollector;
  static constexpr uint8_t N = 7;

  inline bool isDifferent(QStringList list) const {
    for (uint8_t i = 0; i < N; ++i) {
      if (list[i] != LVGLCore::DEFAULT_DAYS[i]) return true;
    }
    return false;
  }

  inline QStringList get(LVGLObject *obj) const override {
    const char **names = lv_calendar_get_day_names(obj->obj());
    if (names == nullptr) return {"Su", "Mo", "Tu", "We", "Th", "Fr", "Sa"};
    QStringList ret;
    for (uint8_t i = 0; i < N; ++i) ret << QString(names[i]);
    return ret;
  }

  inline void set(LVGLObject *obj, QStringList list) override {
    if (!isDifferent(list)) return;
    const char **data = new const char *[N];
    for (uint8_t i = 0; i < N; ++i) {
      const auto &byte = list[i].toUtf8();
      char *str = new char[byte.size() + 1];
      strcpy(str, byte.data());
      str[byte.size()] = '\0';

      //      const QString &s = list[i];
      //      char *string = new char[s.size() + 1];
      //      memcpy(string, qUtf8Printable(s), s.size());
      //      string[s.size()] = '\0';
      data[i] = str;
    }
    m_garbageCollector << data;
    lv_calendar_set_day_names(obj->obj(), data);
  }
};

class LVGLPropertyMonthNames : public LVGLPropertyTextList {
 public:
  inline LVGLPropertyMonthNames() : LVGLPropertyTextList(false) {}
  inline ~LVGLPropertyMonthNames() {
    for (const char **d : m_garbageCollector) {
      for (uint8_t i = 0; i < N; ++i) delete[] d[i];
      delete[] d;
    }
  }

  inline QString name() const override { return "Month names"; }

  inline QStringList function(LVGLObject *obj) const override {
    const QStringList list = get(obj);
    if (!isDifferent(list)) return {};
    QStringList months;
    for (uint8_t i = 0; i < N; ++i) months << "\"" + list[i] + "\"";
    QStringList ret;
    const QString varName = QString("months_%1").arg(obj->codeName());
    ret << QString("static const char *%1[12] = {%2};")
               .arg(varName)
               .arg(months.join(", "));
    ret << QString("lv_calendar_set_month_names(%1, %2);")
               .arg(obj->codeName())
               .arg(varName);
    return ret;
  }

 protected:
  QList<const char **> m_garbageCollector;
  static constexpr uint8_t N = 12;

  inline bool isDifferent(QStringList list) const {
    for (uint8_t i = 0; i < N; ++i) {
      if (list[i] != LVGLCore::DEFAULT_MONTHS[i]) return true;
    }
    return false;
  }

  inline QStringList get(LVGLObject *obj) const override {
    const char **names = lv_calendar_get_month_names(obj->obj());
    if (names == nullptr) names = LVGLCore::DEFAULT_MONTHS;
    QStringList ret;
    for (uint8_t i = 0; i < N; ++i) ret << QString(names[i]);
    return ret;
  }

  inline void set(LVGLObject *obj, QStringList list) override {
    if (!isDifferent(list)) return;
    const char **data = new const char *[N];
    for (uint8_t i = 0; i < N; ++i) {
      const auto &byte = list[i].toUtf8();
      char *str = new char[byte.size() + 1];
      strcpy(str, byte.data());
      str[byte.size()] = '\0';
      data[i] = str;
      //      const QString &s = list[i];
      //      char *string = new char[s.size() + 1];
      //      memcpy(string, qUtf8Printable(s), s.size());
      //      string[s.size()] = '\0';
      //      data[i] = string;
    }
    m_garbageCollector << data;
    lv_calendar_set_month_names(obj->obj(), data);
  }
};

LVGLCalendar::LVGLCalendar() {
  initStateStyles();
  m_parts << LV_CALENDAR_PART_BG << LV_CALENDAR_PART_HEADER
          << LV_CALENDAR_PART_DAY_NAMES << LV_CALENDAR_PART_DATE;
  m_properties << new LVGLPropertyDate(
      "Current date", "lv_calendar_set_today_date", lv_calendar_set_today_date,
      lv_calendar_get_today_date);
  m_properties << new LVGLPropertyDate(
      "Shown date", "lv_calendar_set_showed_date", lv_calendar_set_showed_date,
      lv_calendar_get_showed_date);
  m_properties << new LVGLPropertyDateList(
      "Highlighted dates", "lv_calendar_set_highlighted_dates",
      lv_calendar_set_highlighted_dates, lv_calendar_get_highlighted_dates,
      lv_calendar_get_highlighted_dates_num);

  m_properties << new LVGLPropertyDayNames;
  m_properties << new LVGLPropertyMonthNames;

  m_editableStyles << LVGL::Body;          // CALENDAR_PART_BG
  m_editableStyles << LVGL::CalendarHead;  // CALENDAR_PART_HEADER
  m_editableStyles << LVGL::Text;          // CALENDAR_PART_DAY_NAMES
  m_editableStyles << LVGL::Text;          // CALENDAR_PART_DATE
}

QString LVGLCalendar::name() const { return "Calendar"; }

QString LVGLCalendar::className() const { return "lv_calendar"; }

LVGLWidget::Type LVGLCalendar::type() const { return Calendar; }

QIcon LVGLCalendar::icon() const { return QIcon(); }

lv_obj_t *LVGLCalendar::newObject(lv_obj_t *parent) const {
  lv_obj_t *obj = lv_calendar_create(parent, nullptr);
  return obj;
}

QSize LVGLCalendar::minimumSize() const { return QSize(250, 200); }

QStringList LVGLCalendar::styles() const {
  return QStringList() << "CALENDAR_PART_BG"
                       << "CALENDAR_PART_HEADER"
                       << "CALENDAR_PART_DAY_NAMES"
                       << "CALENDAR_PART_DATE";
}

lv_style_t *LVGLCalendar::style(lv_obj_t *obj, lv_obj_part_t part) const {
  return lv_obj_get_local_style(obj, part);
}

void LVGLCalendar::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const {
  lv_obj_add_style(obj, LV_BTN_PART_MAIN, style);
}

void LVGLCalendar::addStyle(lv_obj_t *obj, lv_style_t *style,
                            lv_obj_part_t part) const {
  lv_obj_add_style(obj, part, style);
}

void LVGLCalendar::initStateStyles() {
  for (int i = 0; i < 4; ++i) {
    lv_style_t *de = new lv_style_t;
    lv_style_t *ch = new lv_style_t;
    lv_style_t *fo = new lv_style_t;
    lv_style_t *ed = new lv_style_t;
    lv_style_t *ho = new lv_style_t;
    lv_style_t *pr = new lv_style_t;
    lv_style_t *di = new lv_style_t;
    lv_style_init(de);
    lv_style_init(ch);
    lv_style_init(fo);
    lv_style_init(ed);
    lv_style_init(ho);
    lv_style_init(pr);
    lv_style_init(di);
    QList<lv_style_t *> stateStyles;
    stateStyles << de << ch << fo << ed << ho << pr << di;
    m_partsStyles[i] = stateStyles;
  }
}
