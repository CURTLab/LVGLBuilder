#include "LVGLSimulator.h"

#include <QApplication>
#include <QMouseEvent>
#include <QDebug>
#include <QThread>
#include <QGraphicsSceneMouseEvent>
#include <QMimeData>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMenu>
#include <QColorDialog>
#include <QDir>
#include <QTextStream>
#include <QClipboard>

#include "widgets/LVGLWidget.h"
#include "LVGLCore.h"
#include "properties/LVGLPropertyGeometry.h"
#include "LVGLFontData.h"

#define IS_PAGE_OF_TABVIEW(o) ((o->widgetType() == LVGLWidget::Page) && (o->index() >= 0) && o->parent() && (o->parent()->widgetType() == LVGLWidget::Tabview))

LVGLScene::LVGLScene(QObject *parent)
	: QGraphicsScene(parent)
	, m_selected(nullptr)
	, m_hoverObject(nullptr)
{
}

void LVGLScene::drawBackground(QPainter *painter, const QRectF &rect)
{
	lvgl.poll();
	painter->save();
	painter->drawPixmap(sceneRect(), lvgl.framebuffer(), rect);
	if (m_selected != nullptr) {
		painter->setPen(Qt::red);
		painter->setBrush(Qt::NoBrush);
		painter->drawRect(m_selected->geometry());
	}
	if (m_hoverObject != nullptr) {
		painter->setPen(QPen(Qt::red, 2));
		painter->setBrush(Qt::NoBrush);
		if (m_hoverObject->widgetType() == LVGLWidget::Tabview) {
			lv_obj_t *obj = m_hoverObject->obj();
			lv_obj_t *tab = lv_tabview_get_tab(obj, lv_tabview_get_tab_act(obj));
			painter->drawRect(lvgl.get_object_rect(tab));
		} else {
			painter->drawRect(m_hoverObject->geometry());
		}
	}
	painter->setPen(Qt::black);
	painter->setBrush(Qt::NoBrush);
	painter->drawRect(sceneRect());
	painter->restore();
}

LVGLObject *LVGLScene::hoverObject() const
{
	return m_hoverObject;
}

void LVGLScene::setHoverObject(LVGLObject *hoverObject)
{
	m_hoverObject = hoverObject;
}

LVGLObject *LVGLScene::selected() const
{
	return m_selected;
}

void LVGLScene::setSelected(LVGLObject *selected)
{
	m_selected = selected;
}

LVGLSimulator::LVGLSimulator(QWidget *parent)
	: QGraphicsView(parent)
	, m_scene(new LVGLScene)
	, m_selectedObject(nullptr)
	, m_mouseEnabled(false)
	, m_item(new LVGLItem)
{
	//setMinimumSize(LV_HOR_RES_MAX, LV_VER_RES_MAX);
	//setMaximumSize(LV_HOR_RES_MAX, LV_VER_RES_MAX);

	connect(m_item, &LVGLItem::geometryChanged,
			  this, &LVGLSimulator::update
			  );

	setAcceptDrops(true);

	m_scene->setSceneRect(0, 0, lvgl.width(), lvgl.height());
	setScene(m_scene);
	m_scene->addItem(m_item);

	QThread *thread = new QThread;
	LVGLSimulator *self = this;
	connect(thread, &QThread::started, [=]() {
		qInfo() << "LVGL Thread started";
		QTimer* timer = new QTimer;
		connect(timer, &QTimer::timeout, self, &LVGLSimulator::update);
		timer->start(20);
	});
	thread->start();
}

LVGLSimulator::~LVGLSimulator()
{
}

void LVGLSimulator::setSelectedObject(LVGLObject *obj)
{
	if (m_selectedObject == obj) return;
	if (obj && obj->isLocked())
		return;

	m_selectedObject = obj;
	m_item->setObject(obj);
	emit objectSelected(m_selectedObject);
	update();
}

void LVGLSimulator::setZoomLevel(int level)
{
	QMatrix matrix;
	const qreal zoom = (level < 0?-1.0/(level - 1):(level == 0 ? 1 : 1.0 + 0.5 * level));
	matrix.scale(zoom, zoom);
	qDebug() << "Zoom:" << zoom;
	setMatrix(matrix);
}

void LVGLSimulator::clear()
{
	setSelectedObject(nullptr);
	lvgl.removeAllObjects();
	lvgl.removeAllImages();
	lvgl.removeCustomFonts();
}

void LVGLSimulator::setMouseEnable(bool enable)
{
	m_mouseEnabled = enable;
}

void LVGLSimulator::changeResolution(lv_coord_t width, lv_coord_t height)
{
	m_scene->setSceneRect(0, 0, width, height);
}

void LVGLSimulator::mousePressEvent(QMouseEvent *event)
{
	const QPoint pos = mapToScene(event->pos()).toPoint();
	if (m_mouseEnabled) {
		lvgl.send_mouse_event(pos.x(), pos.y(), event->buttons() & Qt::LeftButton);
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
					if (obj == m_selectedObject)
						setSelectedObject(nullptr);
				} else if (sel == remove) {
					if (obj == m_selectedObject)
						setSelectedObject(nullptr);
					lvgl.removeObject(obj);
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
				dialog.setCurrentColor(lvgl.screenColor());
				if (dialog.exec() == QDialog::Accepted)
					lvgl.setScreenColor(dialog.selectedColor());
			}
		} else if (event->button() == Qt::LeftButton) {
			if (!m_item->isManipolating())
				setSelectedObject(selectObject(objectsUnderCoords(pos, false), false));
		}
	}
	QGraphicsView::mousePressEvent(event);
}

void LVGLSimulator::mouseDoubleClickEvent(QMouseEvent *event)
{
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

void LVGLSimulator::mouseReleaseEvent(QMouseEvent *event)
{
	if (m_mouseEnabled) {
		const QPoint pos = mapToScene(event->pos()).toPoint();
		lvgl.send_mouse_event(pos.x(), pos.y(), false);
	}
	QGraphicsView::mouseReleaseEvent(event);
}

void LVGLSimulator::mouseMoveEvent(QMouseEvent *event)
{
	if (m_mouseEnabled) {
		const QPoint pos = mapToScene(event->pos()).toPoint();
		lvgl.send_mouse_event(pos.x(), pos.y(), event->buttons() & Qt::LeftButton);
	}
	QGraphicsView::mouseMoveEvent(event);
}

void LVGLSimulator::dropEvent(QDropEvent *event)
{
	if (m_mouseEnabled)
		return;

	m_scene->setHoverObject(nullptr);

	union {
		LVGLWidget *ptr;
		qintptr i;
	} cast;

	const QMimeData *mimeData = event->mimeData();
	if (mimeData->hasFormat("application/x-widget")) {
		QByteArray encoded = mimeData->data("application/x-widget");
		QDataStream stream(&encoded, QIODevice::ReadOnly);
		stream >> cast.i;
		LVGLWidget *widgetClass = cast.ptr;

		LVGLObject *newObj = nullptr;

		// check if moved into another widget
		const QPoint pos = mapToScene(event->pos()).toPoint();
		auto parent = selectObject(objectsUnderCoords(pos, true), false);

		// create new widget
		if (parent) {
			QPoint parentPos;
			if (parent->widgetType() == LVGLWidget::Tabview) {
				lv_obj_t *obj = parent->obj();
				parent = parent->findChildByIndex(lv_tabview_get_tab_act(obj));
				assert(parent);
			}
			newObj = new LVGLObject(widgetClass, "", parent);
			parentPos = parent->absolutePosition();
			newObj->setGeometry(QRect(pos - parentPos, widgetClass->minimumSize()));
		} else {
			newObj = new LVGLObject(widgetClass, "", lv_scr_act());
			newObj->setGeometry(QRect(pos, widgetClass->minimumSize()));
		}
		qDebug().noquote() << "Class:" << widgetClass->className() << "Id:" << newObj->name();

		connect(newObj, &LVGLObject::positionChanged,
				  m_item, &LVGLItem::updateGeometry
				  );
		lvgl.addObject(newObj);
		setSelectedObject(newObj);
		setFocus();

		emit objectAdded(newObj);
	}
}

void LVGLSimulator::dragMoveEvent(QDragMoveEvent *event)
{
	if (m_mouseEnabled)
		return;

	const QPoint pos = mapToScene(event->pos()).toPoint();
	auto sel = selectObject(objectsUnderCoords(pos, true), false);
	m_scene->setHoverObject(sel);

	event->acceptProposedAction();
}

void LVGLSimulator::dragEnterEvent(QDragEnterEvent *event)
{
	if (m_mouseEnabled)
		return;

	event->acceptProposedAction();
}

void LVGLSimulator::update()
{
	m_scene->invalidate(m_scene->sceneRect(), QGraphicsScene::BackgroundLayer);
}

LVGLObject *LVGLSimulator::selectObject(QList<LVGLObject *> objs, bool doubleClick) const
{
	if (objs.empty()) return nullptr;

	if (objs.size() > 0) {
		if (doubleClick) {
			for (LVGLObject *obj:objs) {
				if (obj->widgetType() == LVGLWidget::Label)
					return obj;
			}
		}
		return objs.last();
	}
	return nullptr;
}

QList<LVGLObject *> LVGLSimulator::objectsUnderCoords(QPoint pos, bool includeLocked) const
{
	QList<LVGLObject *> ret;
	QRect screen = QRect(QPoint(0, 0), lvgl.size()).adjusted(-50, -50, 50, 50);
	for (LVGLObject *o:lvgl.allObjects()) {
		if (!includeLocked && o->isLocked())
			continue;
		QRect geo = o->geometry();
		if (screen.contains(geo) && geo.contains(pos))
			ret << o;
	}
	return ret;
}

LVGLItem *LVGLSimulator::item() const
{
	return m_item;
}

void LVGLSimulator::moveObject(LVGLObject *obj, int dx, int dy)
{
	if (obj) {
		if ((dx != 0) && (dy != 0)) {
			obj->setPosition(obj->position() + QPoint(dx, dy));
		} else if (dx != 0) {
			obj->setX(qBound(0, obj->x() + dx, lvgl.width() - obj->width() - 1));
		} else if (dy != 0) {
			obj->setY(qBound(0, obj->y() + dy, lvgl.height() - obj->height() - 1));
		}
	}
}

LVGLObject *LVGLSimulator::selectedObject() const
{
	return m_selectedObject;
}

LVGLKeyPressEventFilter::LVGLKeyPressEventFilter(LVGLSimulator *sim, QObject *parent)
	: QObject(parent), m_sim(sim)
{
}

bool LVGLKeyPressEventFilter::eventFilter(QObject *obj, QEvent *event)
{
	if (event->type() != QEvent::KeyRelease)
		return QObject::eventFilter(obj, event);

	LVGLSimulator *sim = qobject_cast<LVGLSimulator*>(QApplication::focusWidget());
	if (sim == nullptr)
		return QObject::eventFilter(obj, event);

	QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
	if (keyEvent->key() == Qt::Key_Delete) {
		LVGLObject *obj = m_sim->selectedObject();
		m_sim->setSelectedObject(nullptr);
		lvgl.removeObject(obj);
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
	} else if (keyEvent->modifiers() & Qt::ControlModifier && ((keyEvent->key() == Qt::Key_C) || (keyEvent->key() == Qt::Key_X))) {
		LVGLObject *obj = m_sim->selectedObject();
		if (obj) {
			QJsonDocument doc(obj->toJson());
			QApplication::clipboard()->setText(doc.toJson(QJsonDocument::Compact));
			if (keyEvent->key() == Qt::Key_X) {
				m_sim->setSelectedObject(nullptr);
				lvgl.removeObject(obj);
			}
		}
		return true;
	} else if (keyEvent->modifiers() & Qt::ControlModifier && keyEvent->key() == Qt::Key_V) {
		QClipboard *clipboard = QApplication::clipboard();
		if (clipboard->mimeData()->hasText()) {
			QJsonDocument doc = QJsonDocument::fromJson(clipboard->text().toLatin1());
			LVGLObject *newObj = LVGLObject::parse(doc.object(), m_sim->selectedObject());
			if (newObj == nullptr)
				return false;
			connect(newObj, &LVGLObject::positionChanged,
					  m_sim->item(), &LVGLItem::updateGeometry
					  );
			for (const LVGLObject *child:newObj->childs())
				connect(child, &LVGLObject::positionChanged,
						  m_sim->item(), &LVGLItem::updateGeometry
						  );
			m_sim->setSelectedObject(newObj);
			m_sim->setFocus();
		}
		return true;
	}
	return QObject::eventFilter(obj, event);
}
