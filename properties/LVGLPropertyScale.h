#ifndef LVGLPROPERTYSCALE_H
#define LVGLPROPERTYSCALE_H

#include "core/LVGLProperty.h"

class LVGLPropertyScale : public LVGLProperty {
 public:
  LVGLPropertyScale(LVGLProperty *parent = nullptr);

  QString name() const override;
  QVariant value(LVGLObject *obj) const override;
  void setValue(LVGLObject *obj, QVariant value) override;

  QJsonValue toJson(LVGLObject *obj) const override;

  QStringList function(LVGLObject *obj) const override;

 protected:
  class LVGLPropertyAngle *m_angle;
  class LVGLPropertyLines *m_line;
  class LVGLPropertyLabels *m_label;
};

#endif  // LVGLPROPERTYSCALE_H
