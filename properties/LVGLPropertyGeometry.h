#ifndef LVGLPROPERTYGEOMETRY_H
#define LVGLPROPERTYGEOMETRY_H

#include "core/LVGLProperty.h"

class LVGLPropertyGeometry : public LVGLProperty {
 public:
  LVGLPropertyGeometry(LVGLProperty *parent = nullptr);

  QString name() const override;
  QVariant value(LVGLObject *obj) const override;
  void setValue(LVGLObject *obj, QVariant value) override;

  QJsonValue toJson(LVGLObject *obj) const override;

  QStringList function(LVGLObject *obj) const override;

 protected:
  LVGLProperty *m_x;
  LVGLProperty *m_y;
  LVGLProperty *m_w;
  LVGLProperty *m_h;
};

#endif  // LVGLPROPERTYGEOMETRY_H
