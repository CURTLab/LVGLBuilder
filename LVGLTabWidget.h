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

  inline QList<LVGLObject *> allObject() { return m_objects; }
  inline void setAllObjects(QList<LVGLObject *> objs) { m_objects = objs; }

  inline QHash<QString, LVGLImageData *> allImages() { return m_images; }
  inline void setAllImages(QHash<QString, LVGLImageData *> imgs) {
    m_images = imgs;
  }

  inline QList<LVGLFontData *> allFonts() { return m_fonts; }
  inline void setAllFonts(QList<LVGLFontData *> fonts) { m_fonts = fonts; }

  inline void setname(const QString &name) { m_name = name; }
  inline QString getname() { return m_name; }

  void removeAll();
  void removeObject(LVGLObject *object);
  void removeAllObjects();
  void removeAllImages();

 private:
  lv_obj_t *m_parent;
  QList<LVGLObject *> m_objects;
  QHash<QString, LVGLImageData *> m_images;
  QList<LVGLFontData *> m_fonts;
  QString m_name;
};

#endif  // LVGLTABWIDGET_H
