#ifndef LVGLTAB_H
#define LVGLTAB_H

#include <QHash>
#include <QString>
#include <QWidget>

#include "lvgl/lvgl.h"

class LVGLImageData;
class LVGLObject;
class LVGLFontData;
class LVGLSimulator;
class QUndoStack;

class LVGLTab : public QWidget {
  Q_OBJECT
 public:
  LVGLTab(QWidget *parent = nullptr);
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

  inline void setfilename(const QString &name) { m_name = name; }
  inline QString getfilename() { return m_name; }

  void removeAll();
  void removeObject(LVGLObject *object);
  void removeAllObjects();
  void removeAllImages();

  void clean();

  inline QUndoStack *getundostack() { return m_undoStack; }

 private:
  lv_obj_t *m_parent;
  QList<LVGLObject *> m_objects;
  QHash<QString, LVGLImageData *> m_images;
  QList<LVGLFontData *> m_fonts;
  QString m_name;
  QUndoStack *m_undoStack;
};

#endif  // LVGLTAB_H
