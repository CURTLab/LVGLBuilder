#ifndef LVGLPROPERTYEVENT_H
#define LVGLPROPERTYEVENT_H

#include "LVGLProperty.h"
class EventsListWIdget;
class LVGLWidget;

class LVGLPropertyEvent : public LVGLProperty {
 public:
  LVGLPropertyEvent(LVGLWidget *w, LVGLProperty *parent = nullptr);
  bool hasEditor() const;
  QWidget *editor(QWidget *parent);
  void updateEditor(LVGLObject *obj);
  void updateWidget(LVGLObject *obj);

  QVariant value(LVGLObject *obj) const;
  void setValue(LVGLObject *obj, QVariant value);

  QJsonValue toJson(LVGLObject *obj) const;

 protected:
  class EventsListWIdget *m_widget;

  virtual QStringList get(LVGLObject *obj) const = 0;
  virtual void set(LVGLObject *obj, QStringList list) = 0;

 private:
  LVGLWidget *m_w;
};

#endif  // LVGLPROPERTYEVENT_H
