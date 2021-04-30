#ifndef LVGLTABWIDGET_H
#define LVGLTABWIDGET_H

#include <QWidget>

#include "lvgl/lvgl.h"

class LVGLImageData;
class LVGLObject;
class LVGLFontData;
class LVGLSimulator;

class LVGLTabWidget : public QWidget {
  Q_OBJECT
 public:
  LVGLTabWidget(QWidget *parent = nullptr);
  lv_obj_t *getparent() { return m_parent; }
  void setSimulator(LVGLSimulator *sim);
  QList<LVGLObject *> allObject() { return m_objects; }
  void setAllObjects(QList<LVGLObject *> objs) { m_objects = objs; }

  void setname(const QString &name) { m_name = name; }
  QString getname() { return m_name; }

  void removeObject(LVGLObject *object);
  void removeAllObjects();

 private:
  lv_obj_t *m_parent;
  QList<LVGLObject *> m_objects;
  QString m_name;
};

#endif  // LVGLTABWIDGET_H
