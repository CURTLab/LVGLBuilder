#ifndef LVGLPROPERTYSERIES_H
#define LVGLPROPERTYSERIES_H

// only for lv_chart!

#include "core/LVGLProperty.h"

class LVGLPropertySeries : public LVGLProperty {
 public:
  LVGLPropertySeries(LVGLProperty *parent = nullptr);

  QString name() const override;

  bool hasEditor() const override;
  QWidget *editor(QWidget *parent) override;
  void updateEditor(LVGLObject *obj) override;
  void updateWidget(LVGLObject *obj) override;

  QStringList function(LVGLObject *obj) const override;

  QJsonValue toJson(LVGLObject *obj) const override;
  void setValue(LVGLObject *obj, QVariant value) override;

 protected:
  class LVGLPropertySeriesDialog *m_widget;
};

#endif  // LVGLPROPERTYSERIES_H
