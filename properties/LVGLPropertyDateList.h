#ifndef LVGLPROPERTYDATELIST_H
#define LVGLPROPERTYDATELIST_H

#include "core/LVGLProperty.h"

class LVGLPropertyDateList : public LVGLProperty {
 public:
  LVGLPropertyDateList(
      QString title, QString trtitle, QString functionName,
      std::function<void(lv_obj_t *, lv_calendar_date_t *, uint16_t)> setter,
      std::function<lv_calendar_date_t *(lv_obj_t *)> getter,
      std::function<uint16_t(lv_obj_t *)> count,
      LVGLProperty *parent = nullptr);
  ~LVGLPropertyDateList();

  bool hasEditor() const override;
  QWidget *editor(QWidget *parent) override;
  void updateEditor(LVGLObject *obj) override;
  void updateWidget(LVGLObject *obj) override;

  QVariant value(LVGLObject *obj) const override;
  void setValue(LVGLObject *obj, QVariant value) override;

  QJsonValue toJson(LVGLObject *obj) const override;

  QString name() const override;
  QString codename() const override;
  QStringList function(LVGLObject *obj) const override;

 protected:
  class LVGLPropertyDateListDialog *m_widget;

  virtual uint16_t count(LVGLObject *obj) const;
  virtual lv_calendar_date_t *get(LVGLObject *obj) const;
  virtual void set(LVGLObject *obj, lv_calendar_date_t *value, uint16_t num);

  QString m_title;
  QString m_trtitle;
  QString m_functionName;
  std::function<void(lv_obj_t *, lv_calendar_date_t *, uint16_t)> m_setter;
  std::function<lv_calendar_date_t *(lv_obj_t *)> m_getter;
  std::function<uint16_t(lv_obj_t *)> m_count;

  QList<lv_calendar_date_t *> m_garbageCollector;
};

#endif  // LVGLPROPERTYDATELIST_H
