#include "LVGLSimulator.h"

#include <QApplication>
#include <QClipboard>
#include <QColorDialog>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QGraphicsSceneMouseEvent>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMenu>
#include <QMimeData>
#include <QMouseEvent>
#include <QTextStream>
#include <QThread>
#include <QUndoStack>

#include "LVGLCommands.h"
#include "LVGLCore.h"
#include "LVGLFontData.h"
#include "LVGLItem.h"
#include "LVGLObject.h"
#include "LVGLObjectModel.h"
#include "LVGLProject.h"
#include "properties/LVGLPropertyGeometry.h"
#include "widgets/LVGLWidget.h"

#define IS_PAGE_OF_TABVIEW(o)                                    \
  ((o->widgetType() == LVGLWidget::Page) && (o->index() >= 0) && \
   o->parent() && (o->parent()->widgetType() == LVGLWidget::Tabview))

LVGLScene::LVGLScene(QObject *parent)
    : QGraphicsScene(parent), m_selected(nullptr), m_hoverObject(nullptr) {}

void LVGLScene::drawBackground(QPainter *painter, const QRectF &rect) {
  m_lvgl->poll();
  painter->save();
  painter->drawPixmap(sceneRect(), m_lvgl->framebuffer(), rect);
  if (m_selected != nullptr) {
    painter->setPen(Qt::red);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(m_selected->geometry());
  }
  if (m_hoverObject != nullptr) {
    painter->setPen(QPen(Qt::red, 2));
    painter->setBrush(Qt::NoBrush);
    //    if (m_hoverObject->widgetType() == LVGLWidget::TabView) {
    //      lv_obj_t *obj = m_hoverObject->obj();
    //      lv_obj_t *tab = lv_tabview_get_tab(obj,
    //      lv_tabview_get_tab_act(obj));
    //      painter->drawRect(m_lvgl->get_object_rect(tab));
    //    } else {
    //      painter->drawRect(m_hoverObject->geometry());
    //    }
    painter->drawRect(m_hoverObject->geometry());
  }
  painter->setPen(Qt::black);
  painter->setBrush(Qt::NoBrush);
  painter->drawRect(sceneRect());
  painter->restore();
}

LVGLObject *LVGLScene::hoverObject() const { return m_hoverObject; }

void LVGLScene::setHoverObject(LVGLObject *hoverObject) {
  m_hoverObject = hoverObject;
}

LVGLObject *LVGLScene::selected() const { return m_selected; }

void LVGLScene::setSelected(LVGLObject *selected) { m_selected = selected; }

LVGLSimulator::LVGLSimulator(LVGLCore *lvgl, QWidget *parent)
    : QGraphicsView(parent),
      m_scene(new LVGLScene),
      m_selectedObject(nullptr),
      m_mouseEnabled(false),
      m_item(new LVGLItem),
      m_objectModel(nullptr),
      m_lvgl(lvgl),
      m_undoStack(new QUndoStack(this)),
      m_mousePressed(false),
      m_patintThread(new QThread),
      m_paintTime(new LVGLPaintTimer) {
  m_scene->setlvgl(m_lvgl);

  connect(m_item, &LVGLItem::geometryChanged, this, &LVGLSimulator::update);

  setAcceptDrops(true);

  m_scene->setSceneRect(0, 0, m_lvgl->width(), m_lvgl->height());
  setScene(m_scene);
  m_scene->addItem(m_item);

  m_paintTime->moveToThread(m_patintThread);
  QObject::connect(m_paintTime, &LVGLPaintTimer::timeout, this,
                   &LVGLSimulator::update);
  QObject::connect(this, &LVGLSimulator::startPaint, m_paintTime,
                   &LVGLPaintTimer::startrun);
  QObject::connect(this, &LVGLSimulator::stopPaint, m_paintTime,
                   &LVGLPaintTimer::stop);
  m_patintThread->start();
}

LVGLSimulator::~LVGLSimulator() {
  emit stopPaint();
  QThread::msleep(100);
  m_patintThread->requestInterruption();
  m_patintThread->exit(0);
  m_patintThread->wait();
  delete m_paintTime;
  delete m_patintThread;
  delete m_item;
  delete m_scene;
}

void LVGLSimulator::setSelectedObject(LVGLObject *obj) {
  if (m_selectedObject == obj) return;
  if (obj && obj->isLocked()) return;

  m_selectedObject = obj;
  m_item->setObject(obj);
  if (m_objectModel) m_objectModel->setCurrentObject(obj);

  emit objectSelected(m_selectedObject);
  update();
}

void LVGLSimulator::setZoomLevel(int level) {
  QTransform transform;
  const qreal zoom =
      (level < 0 ? -1.0 / (level - 1) : (level == 0 ? 1 : 1.0 + 0.125 * level));
  transform.scale(zoom, zoom);
  qDebug() << "Zoom:" << zoom;
  setTransform(transform);
}

void LVGLSimulator::clear() {
  setSelectedObject(nullptr);
  m_lvgl->removeAllObjects();
  m_lvgl->removeAllImages();
  m_lvgl->removeCustomFonts();
}

void LVGLSimulator::setMouseEnable(bool enable) { m_mouseEnabled = enable; }

void LVGLSimulator::changeResolution(QSize size) {
  m_scene->setSceneRect(0, 0, size.width(), size.height());
}

void LVGLSimulator::mousePressEvent(QMouseEvent *event) {
  const QPoint pos = mapToScene(event->pos()).toPoint();
  if (m_mouseEnabled) {
    emit objPressed(selectObject(objectsUnderCoords(pos, true), false));
    m_lvgl->sendMouseEvent(pos.x(), pos.y(), event->buttons() & Qt::LeftButton);
  } else {
    if (event->button() == Qt::RightButton) {
      auto obj = selectObject(objectsUnderCoords(pos, true), false);
      QAction *sel = nullptr;
      QAction *scolor = nullptr;
      if (obj) {
        // LVGL right click menu
        bool locked = obj->isLocked();
        QMenu menu(obj->name(), this);
        QAction *lock = menu.addAction(locked ? "Unlock" : "Lock");
        QAction *remove = menu.addAction("Remove");
        QAction *mfore = menu.addAction("Move foreground");
        QAction *mback = menu.addAction("Move background");
        scolor = menu.addAction("Set screen color ...");
        sel = menu.exec(mapToGlobal(event->pos()));
        if ((sel == lock) && locked) {
          obj->setLocked(false);
        } else if ((sel == lock) && !locked) {
          obj->setLocked(true);
          if (obj == m_selectedObject) setSelectedObject(nullptr);
        } else if (sel == remove) {
          if (obj == m_selectedObject) setSelectedObject(nullptr);
          m_undoStack->push(new RemoveWidgetCommand(this, obj));
        } else if (sel == mfore) {
          lv_obj_move_foreground(obj->obj());
        } else if (sel == mback) {
          lv_obj_move_background(obj->obj());
        }
      } else {
        QMenu menu(this);
        scolor = menu.addAction("Set screen color ...");
        sel = menu.exec(mapToGlobal(event->pos()));
      }

      if (sel == scolor) {
        QColorDialog dialog(this);
        dialog.setCurrentColor(m_lvgl->screenColor());
        if (dialog.exec() == QDialog::Accepted)
          m_lvgl->setScreenColor(dialog.selectedColor());
      }
    } else if (event->button() == Qt::LeftButton) {
      auto obj = selectObject(objectsUnderCoords(pos, false), false);
      if (!m_item->isManipolating()) setSelectedObject(obj);
      if (obj) {
        if (!m_mousePressed) {
          m_mousePressed = true;
          selectobjName = obj->name();
          selectobjRect = obj->geometry();
        }
      }
    }
  }
  QGraphicsView::mousePressEvent(event);
}

void LVGLSimulator::mouseDoubleClickEvent(QMouseEvent *event) {
  if (!m_item->isManipolating() && !m_mouseEnabled) {
    const QPoint pos = mapToScene(event->pos()).toPoint();
    auto obj = selectObject(objectsUnderCoords(pos, false), true);
    if (obj == nullptr)
      setSelectedObject(nullptr);
    else if (!obj->isLocked())
      setSelectedObject(obj);
  }
  QGraphicsView::mouseDoubleClickEvent(event);
}

void LVGLSimulator::mouseReleaseEvent(QMouseEvent *event) {
  if (m_mouseEnabled) {
    const QPoint pos = mapToScene(event->pos()).toPoint();
    m_lvgl->sendMouseEvent(pos.x(), pos.y(), false);
  } else {
    if (m_mousePressed) {
      m_mousePressed = false;
      auto obj = findObject(selectobjName);
      if (obj) {
        QRect newRect = obj->geometry();
        if (newRect != selectobjRect)
          m_undoStack->push(
              new SetWidgetRectCommand(this, obj, selectobjRect, newRect));
      }
    }
  }
  QGraphicsView::mouseReleaseEvent(event);
}

void LVGLSimulator::mouseMoveEvent(QMouseEvent *event) {
  if (m_mouseEnabled) {
    const QPoint pos = mapToScene(event->pos()).toPoint();
    m_lvgl->sendMouseEvent(pos.x(), pos.y(), event->buttons() & Qt::LeftButton);
  }
  QGraphicsView::mouseMoveEvent(event);
}

void LVGLSimulator::dropEvent(QDropEvent *event) {
  if (m_mouseEnabled) return;

  m_scene->setHoverObject(nullptr);

  LVGLWidgetCast cast;

  const QMimeData *mimeData = event->mimeData();
  if (mimeData->hasFormat("application/x-widget")) {
    QByteArray encoded = mimeData->data("application/x-widget");
    QDataStream stream(&encoded, QIODevice::ReadOnly);
    stream >> cast.i;
    LVGLWidget *widgetClass = cast.ptr;
    auto wclass = lvgl->widget(widgetClass->className());
    LVGLObject *newObj = nullptr;

    // check if moved into another widget
    QPoint pos = mapToScene(event->pos()).toPoint();
    auto parent = selectObject(objectsUnderCoords(pos, true), false);

    // create new widget
    if (parent) {
      QPoint parentPos;
      if (parent->widgetType() == LVGLWidget::TabView) {
        lv_obj_t *obj = parent->obj();
        parent = parent->findChildByIndex(lv_tabview_get_tab_act(obj));
        Q_ASSERT(parent);
      }

      newObj = new LVGLObject(wclass, "", parent);
      parentPos = parent->absolutePosition();
      newObj->setGeometry(QRect(pos - parentPos, wclass->minimumSize()));
    } else {
      newObj = new LVGLObject(wclass, "", m_lvgl->getdispt()->act_scr);
      QSize size(std::min(wclass->minimumSize().width(), m_lvgl->width()),
                 std::min(wclass->minimumSize().height(), m_lvgl->height()));
      if (pos.x() + size.width() >= m_lvgl->width())
        pos.setX(m_lvgl->width() - size.width());
      if (pos.y() + size.height() >= m_lvgl->height())
        pos.setY(m_lvgl->height() - size.height());
      newObj->setGeometry(QRect(pos, size));
    }

    qDebug().noquote() << "Class:" << wclass->className()
                       << "Id:" << newObj->name();
    m_undoStack->push(new AddWidgetCommand(this, newObj));
    addObject(newObj);
  }
}

void LVGLSimulator::dragMoveEvent(QDragMoveEvent *event) {
  if (m_mouseEnabled) return;

  const QPoint pos = mapToScene(event->pos()).toPoint();
  auto sel = selectObject(objectsUnderCoords(pos, true), false);
  m_scene->setHoverObject(sel);

  event->acceptProposedAction();
}

void LVGLSimulator::dragEnterEvent(QDragEnterEvent *event) {
  if (m_mouseEnabled) return;
  event->acceptProposedAction();
}

void LVGLSimulator::update() {
  m_scene->invalidate(m_scene->sceneRect(), QGraphicsScene::BackgroundLayer);
}

LVGLObject *LVGLSimulator::selectObject(QList<LVGLObject *> objs,
                                        bool doubleClick) const {
  if (objs.empty()) return nullptr;

  if (objs.size() > 0) {
    if (doubleClick) {
      for (LVGLObject *obj : objs) {
        if (obj->widgetType() == LVGLWidget::Label) return obj;
      }
    }
    return objs.last();
  }
  return nullptr;
}

QList<LVGLObject *> LVGLSimulator::objectsUnderCoords(
    QPoint pos, bool includeLocked) const {
  QList<LVGLObject *> ret;
  QRect screen = QRect(QPoint(0, 0), m_lvgl->size()).adjusted(-50, -50, 50, 50);
  for (LVGLObject *o : m_lvgl->allObjects()) {
    if (!includeLocked && o->isLocked()) continue;
    QRect geo = o->geometry();
    if (screen.contains(geo) && geo.contains(pos)) ret << o;
  }
  return ret;
}

void LVGLSimulator::setObjectModel(LVGLObjectModel *objectModel) {
  m_objectModel = objectModel;
}

void LVGLSimulator::threadstop() { emit stopPaint(); }

void LVGLSimulator::restartconnect() { emit startPaint(); }

QUndoStack *LVGLSimulator::undoStack() const { return m_undoStack; }

LVGLItem *LVGLSimulator::item() const { return m_item; }

void LVGLSimulator::moveObject(LVGLObject *obj, int dx, int dy) {
  if (obj) {
    if ((dx != 0) && (dy != 0)) {
      obj->setPosition(obj->position() + QPoint(dx, dy));
    } else if (dx != 0) {
      obj->setX(qBound(0, obj->x() + dx, m_lvgl->width() - obj->width() - 1));
    } else if (dy != 0) {
      obj->setY(qBound(0, obj->y() + dy, m_lvgl->height() - obj->height() - 1));
    }
  }
}

void LVGLSimulator::addObject(LVGLObject *obj) {
  connect(obj, &LVGLObject::positionChanged, m_item, &LVGLItem::updateGeometry);

  // add to object viewer
  if (m_objectModel) m_objectModel->beginInsertObject(obj);

  // add object to interal list
  m_lvgl->addObject(obj);

  if (m_objectModel) m_objectModel->endInsertObject();

  setSelectedObject(obj);
  setFocus();

  emit objectAdded(obj);
}

void LVGLSimulator::removeObject(LVGLObject *obj) {
  if (obj) {
    setSelectedObject(nullptr);
    if (m_objectModel) m_objectModel->beginRemoveObject(obj);
    m_lvgl->removeObject(obj);
    if (m_objectModel) m_objectModel->endRemoveObject();
  }
}

LVGLObject *LVGLSimulator::findObject(const QString &objname) {
  return m_lvgl->findObjByName(objname);
}

LVGLObject *LVGLSimulator::selectedObject() const { return m_selectedObject; }

LVGLKeyPressEventFilter::LVGLKeyPressEventFilter(LVGLSimulator *sim,
                                                 QObject *parent)
    : QObject(parent), m_sim(sim) {}

bool LVGLKeyPressEventFilter::eventFilter(QObject *obj, QEvent *event) {
  if (event->type() != QEvent::KeyRelease)
    return QObject::eventFilter(obj, event);

  LVGLSimulator *sim =
      qobject_cast<LVGLSimulator *>(QApplication::focusWidget());
  if (sim == nullptr) return QObject::eventFilter(obj, event);

  QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
  if (keyEvent->key() == Qt::Key_Delete) {
    LVGLObject *obj = m_sim->selectedObject();
    m_sim->undoStack()->push(new RemoveWidgetCommand(m_sim, obj));
    m_sim->setSelectedObject(nullptr);
    return true;
  } else if (keyEvent->key() == Qt::Key_Left) {
    m_sim->moveObject(m_sim->selectedObject(), -1, 0);
    return true;
  } else if (keyEvent->key() == Qt::Key_Right) {
    m_sim->moveObject(m_sim->selectedObject(), 1, 0);
    return true;
  } else if (keyEvent->key() == Qt::Key_Up) {
    m_sim->moveObject(m_sim->selectedObject(), 0, -1);
    return true;
  } else if (keyEvent->key() == Qt::Key_Down) {
    m_sim->moveObject(m_sim->selectedObject(), 0, 1);
    return true;
  } else if (keyEvent->modifiers() & Qt::ControlModifier &&
             ((keyEvent->key() == Qt::Key_C) ||
              (keyEvent->key() == Qt::Key_X))) {
    LVGLObject *obj = m_sim->selectedObject();
    if (obj) {
      QJsonDocument doc(obj->toJson());
      QApplication::clipboard()->setText(doc.toJson(QJsonDocument::Compact));
      if (keyEvent->key() == Qt::Key_X) {
        m_sim->undoStack()->push(new RemoveWidgetCommand(m_sim, obj));
      }
    }

    return true;
  } else if (keyEvent->modifiers() & Qt::ControlModifier &&
             keyEvent->key() == Qt::Key_V) {
    QClipboard *clipboard = QApplication::clipboard();
    if (clipboard->mimeData()->hasText()) {
      QJsonDocument doc = QJsonDocument::fromJson(clipboard->text().toLatin1());
      LVGLObject *newObj =
          LVGLObject::parse(doc.object(), m_sim->selectedObject());
      if (newObj == nullptr) return false;
      connect(newObj, &LVGLObject::positionChanged, m_sim->item(),
              &LVGLItem::updateGeometry);
      for (const LVGLObject *child : newObj->childs())
        connect(child, &LVGLObject::positionChanged, m_sim->item(),
                &LVGLItem::updateGeometry);
      m_sim->undoStack()->push(new AddWidgetCommand(m_sim, newObj));
      m_sim->setSelectedObject(newObj);
      m_sim->setFocus();
    }
    return true;
  }
  return QObject::eventFilter(obj, event);
}

LVGLPaintTimer::LVGLPaintTimer(QObject *parent)
    : QObject(parent), m_timer(nullptr) {}

LVGLPaintTimer::~LVGLPaintTimer() {}

void LVGLPaintTimer::startrun() {
  if (!m_timer) {
    m_timer = new QTimer;
    connect(m_timer, &QTimer::timeout, this, &LVGLPaintTimer::timeout);
  }

  m_timer->start(20);
}

void LVGLPaintTimer::stop() {
  if (m_timer) {
    m_timer->stop();
    delete m_timer;
    m_timer = nullptr;
  }
}
