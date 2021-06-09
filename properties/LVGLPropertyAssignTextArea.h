#ifndef LVGLPROPERTYASSIGNTEXTAREA_H
#define LVGLPROPERTYASSIGNTEXTAREA_H

#include "core/LVGLProperty.h"

class LVGLPropertyAssignTextArea : public LVGLProperty {
 public:
  LVGLPropertyAssignTextArea(LVGLProperty *parent = nullptr);

  QString name() const;
  QString codename() const;

  bool hasEditor() const;
  QWidget *editor(QWidget *parent);
  void updateEditor(LVGLObject *obj);
  void updateWidget(LVGLObject *obj);

  QVariant value(LVGLObject *obj) const;
  void setValue(LVGLObject *obj, QVariant value);

  QStringList function(LVGLObject *obj) const;

 protected:
  QComboBox *m_widget;
};

#endif  // LVGLPROPERTYASSIGNTEXTAREA_H
