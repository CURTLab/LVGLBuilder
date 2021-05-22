#ifndef LVGLSIMULATOR_H
#define LVGLSIMULATOR_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTextStream>

#include "lvgl/lvgl.h"

class LVGLObject;
class LVGLItem;
class LVGLObjectModel;
class LVGLCore;
class QUndoStack;
class QTimer;
class LVGLPaintTimer;

class LVGLScene : public QGraphicsScene {
  Q_OBJECT
 public:
  LVGLScene(QObject *parent = nullptr);

  LVGLObject *selected() const;

  LVGLObject *hoverObject() const;
  void setHoverObject(LVGLObject *hoverObject);

 public slots:
  void setSelected(LVGLObject *selected);

 protected:
  virtual void drawBackground(QPainter *painter, const QRectF &rect) override;

 private:
  LVGLObject *m_selected;
  LVGLObject *m_hoverObject;
};

class LVGLSimulator : public QGraphicsView {
  Q_OBJECT
 public:
  explicit LVGLSimulator(QWidget *parent = nullptr);
  ~LVGLSimulator() override;

  LVGLObject *selectedObject() const;

  LVGLItem *item() const;

  void moveObject(LVGLObject *obj, int dx, int dy);
  void addObject(LVGLObject *obj);
  void removeObject(LVGLObject *obj);
  LVGLObject *findObject(const QString &objname);

  void setobjParent(lv_obj_t *parent);
  void setObjectModel(LVGLObjectModel *objectModel);
  bool getMouseEnable() { return m_mouseEnabled; }
  void threadstop();
  void restartconnect();
  QUndoStack *undoStack() const;

 public slots:
  void setSelectedObject(LVGLObject *obj);
  void setZoomLevel(int level);
  void clear();
  void setMouseEnable(bool enable);
  void changeResolution(QSize size);

 signals:
  void objectSelected(LVGLObject *obj);
  void objectPositionChanged();
  void objectAdded(LVGLObject *obj);
  void objPressed(LVGLObject *obj);
  void startPaint();
  void stopPaint();

 protected:
  void mousePressEvent(QMouseEvent *event) override;
  void mouseDoubleClickEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;

  void dropEvent(QDropEvent *event) override;
  void dragMoveEvent(QDragMoveEvent *event) override;
  void dragEnterEvent(QDragEnterEvent *event) override;

 private slots:
  void update();

 private:
  LVGLObject *selectObject(QList<LVGLObject *> objs, bool doubleClick) const;
  QList<LVGLObject *> objectsUnderCoords(QPoint pos, bool includeLocked) const;
  void generateCodeObject(QTextStream &stream, LVGLObject *o);
  // void parseJson(QJsonObject object, LVGLObject *parent = nullptr);

  LVGLScene *m_scene;
  LVGLObject *m_selectedObject;
  bool m_mouseEnabled;
  LVGLItem *m_item;
  LVGLObjectModel *m_objectModel;
  QUndoStack *m_undoStack;
  bool m_mousePressed;
  QString selectobjName;
  QRect selectobjRect;
  QThread *m_patintThread;
  LVGLPaintTimer *m_paintTime;
  lv_obj_t *m_parent;
};

class LVGLKeyPressEventFilter : public QObject {
  Q_OBJECT
 public:
  explicit LVGLKeyPressEventFilter(LVGLSimulator *sim,
                                   QObject *parent = nullptr);

 protected:
  bool eventFilter(QObject *obj, QEvent *event) override;

 private:
  LVGLSimulator *m_sim;
};

class LVGLPaintTimer : public QObject {
  Q_OBJECT
 public:
  explicit LVGLPaintTimer(QObject *parent = nullptr);
  ~LVGLPaintTimer();
  void startrun();
  void stop();

 signals:
  void timeout();

 private:
  QTimer *m_timer;
};

#endif  // LVGLSIMULATOR_H
