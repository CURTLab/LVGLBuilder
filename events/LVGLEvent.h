#ifndef LVGLEVENT_H
#define LVGLEVENT_H

#include <QObject>

class LVGLEvent {
 public:
  enum WIDGETTYPE { e_LINEEDIT = 0, e_COMBOBOX };

 public:
  LVGLEvent();
  virtual ~LVGLEvent();
  inline QStringList getObjName() const { return m_objName; }
  inline QStringList getPropertyList() const { return m_propertyList; }
  inline QStringList getValuelist() const { return m_valueList; }
  inline QStringList getAmName() const { return m_amName; }
  inline QStringList getScreenName() const { return m_screenName; }
  inline QStringList getOpaName() const { return m_opaName; }
  inline QStringList getFadeModeList() const { return m_fademodelist; }

  virtual WIDGETTYPE whichwidget() = 0;

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
