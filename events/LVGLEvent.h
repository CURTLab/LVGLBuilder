#ifndef LVGLEVENT_H
#define LVGLEVENT_H

#include <QObject>

class LVGLEvent {
 public:
  LVGLEvent();
  inline QStringList getObjName() const { return m_objName; }
  inline QStringList getPropertyList() const { return m_propertyList; }
  inline QStringList getValuelist() const { return m_valueList; }

 protected:
  QStringList m_objName;
  QStringList m_propertyList;
  QStringList m_valueList;
};

#endif  // LVGLEVENT_H
