#ifndef LVGLEVENT_H
#define LVGLEVENT_H

#include <QObject>

class LVGLEvent {
 public:
  LVGLEvent();
  inline QStringList getObjName() const { return m_objName; }
  inline QStringList getPropertyList() const { return m_propertyList; }
  inline QStringList getValuelist() const { return m_valueList; }
  inline QStringList getAmName() const { return m_amName; }

 protected:
  QStringList m_objName;
  QStringList m_propertyList;
  QStringList m_valueList;
  QStringList m_amName;
  QStringList m_screenName;
  QStringList m_opaName;
  QStringList m_fademodelist;
};

#endif  // LVGLEVENT_H
