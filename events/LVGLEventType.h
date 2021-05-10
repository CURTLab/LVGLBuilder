#ifndef LVGLEVENTTYPE_H
#define LVGLEVENTTYPE_H

#include <QObject>

class LVGLEventType {
 public:
  enum WType {
    BASIC = 0,
    ARC,
    BAR,
    BUTTON,
    CHECKBOX,
    DROPDOWNLIST,
    IMAGE,
    IMAGEBUTTON,
    LABEL,
    LED,
    LINE,
    ROLLER,
    SLIDER,
    SPINBOX,
    SWITCH,
    TEXTAREA
  };

 public:
  LVGLEventType(int type);
  LVGLEventType(int type, int wtype);
  LVGLEventType(int type, int wtype, int Role);

  void init();

  inline QStringList getObjName() const { return m_objName; }
  inline QStringList getPropertyList() const { return m_propertyList; }
  inline QStringList getValuelist() const { return m_valueList; }
  inline QStringList getAmName() const { return m_amName; }
  inline QStringList getScreenName() const { return m_screenName; }
  inline QStringList getOpaName() const { return m_opaName; }
  inline QStringList getFadeModeList() const { return m_fademodelist; }
  inline int getRole() { return m_role; }

 private:
  QStringList m_objName;
  QStringList m_propertyList;
  QStringList m_valueList;
  QStringList m_amName;
  QStringList m_screenName;
  QStringList m_opaName;
  QStringList m_fademodelist;
  int m_type;
  int m_wtype;
  int m_role;
};

#endif  // LVGLEVENTTYPE_H
