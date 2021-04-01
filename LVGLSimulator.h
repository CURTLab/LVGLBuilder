#ifndef LVGLSIMULATOR_H
#define LVGLSIMULATOR_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTextStream>

class LVGLObject;
class LVGLItem;
class LVGLObjectModel;
class LVGLCore;

class LVGLScene : public QGraphicsScene {
  Q_OBJECT
 public:
  LVGLScene(QObject *parent = nullptr);

  LVGLObject *selected() const;

  LVGLObject *hoverObject() const;
  void setHoverObject(LVGLObject *hoverObject);

  void setlvgl(LVGLCore *lvgl) { m_lvgl = lvgl; }

 public slots:
  void setSelected(LVGLObject *selected);

 protected:
  virtual void drawBackground(QPainter *painter, const QRectF &rect) override;

 private:
  LVGLObject *m_selected;
  LVGLObject *m_hoverObject;
  LVGLCore *m_lvgl;
};

class LVGLSimulator : public QGraphicsView {
  Q_OBJECT
 public:
  explicit LVGLSimulator(LVGLCore *lvgl, QWidget *parent = nullptr);
  ~LVGLSimulator() override;

  LVGLObject *selectedObject() const;

  LVGLItem *item() const;

  void moveObject(LVGLObject *obj, int dx, int dy);
  void addObject(LVGLObject *obj);
  void removeObject(LVGLObject *obj);

  void setObjectModel(LVGLObjectModel *objectModel);
  bool getMouseEnable() { return m_mouseEnabled; }

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
  LVGLCore *m_lvgl;
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

#endif  // LVGLSIMULATOR_H
