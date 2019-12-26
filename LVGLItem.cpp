#include "LVGLItem.h"

#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsSceneHoverEvent>
#include <QCursor>
#include <QtMath>
#include <QDebug>
#include "LVGLCore.h"

LVGLItem::LVGLItem(QGraphicsItem *parent)
	: QGraphicsItem(parent)
	, m_object(nullptr)
	, m_minimumWidth(20)
	, m_minimumHeight(20)
{
	setFlag(QGraphicsItem::ItemSendsGeometryChanges);
	setFlag(QGraphicsItem::ItemIsMovable);
	setAcceptHoverEvents(true);

	m_direction.clear();
}

QRectF LVGLItem::boundingRect() const
{
	if (m_object)
		return QRectF(QPointF(0, 0), m_object->size());
	else
		return QRectF();
}

void LVGLItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option)
	Q_UNUSED(widget)
	if (m_object) {
		painter->save();

		QRectF obj(QPointF(0, 0), m_object->size());
		QRectF tl(0, 0, 6, 6);
		QRectF tr(obj.width() - 6, 0, 6, 6);
		QRectF bl(0, obj.height() - 6, 6, 6);
		QRectF br(obj.width() - 6, obj.height() - 6, 6, 6);

		const double centerH = obj.width() * 0.5;
		const double centerV = obj.height() * 0.5;
		QRectF ct(centerH - 3, 0, 6, 6);
		QRectF cb(centerH - 3, obj.height() - 6, 6, 6);
		QRectF cl(0, centerV - 3, 6, 6);
		QRectF cr(obj.width() - 6, centerV - 3, 6, 6);

		painter->setPen(m_object->isMovable() ? Qt::blue : Qt::darkGray);
		painter->setBrush(QColor(160, 160, 160));
		painter->drawRect(tl);
		painter->drawRect(tr);
		painter->drawRect(bl);
		painter->drawRect(br);

		painter->setBrush(m_object->isMovable() ? Qt::black : Qt::darkGray);
		painter->drawRect(ct);
		painter->drawRect(cb);
		painter->drawRect(cl);
		painter->drawRect(cr);

		painter->restore();
	}
}

QVariant LVGLItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
	if (change == ItemPositionChange && scene() && m_object && m_object->isMovable()) {
		QPoint newPos = value.toPoint();
		QPoint absPos = m_object->absolutePosition();
		if (newPos == absPos)
			return absPos;
		QRect rect;
		QPoint offset(0, 0);
		lv_obj_t *parent = lv_obj_get_parent(m_object->obj());
		if (parent) {
			offset = lvgl.get_absolute_position(parent);
			rect = QRect(QPoint(0, 0), lvgl.get_object_size(parent) - m_object->size());
		} else {
			rect = QRect(QPoint(0, 0), lvgl.size() - m_object->size());
		}

		newPos.setX(qBound(rect.left(), newPos.x() - offset.x(), rect.right()));
		newPos.setY(qBound(rect.top(), newPos.y() - offset.y(), rect.bottom()));

		// important use lv_obj_set_pos not setPosition since it has a signal
		lv_obj_set_pos(m_object->obj(), static_cast<lv_coord_t>(newPos.x()),
												  static_cast<lv_coord_t>(newPos.y()));
		emit geometryChanged();

		return newPos + offset;
	}
	return QGraphicsItem::itemChange(change, value);
}

void LVGLItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
	if (m_object && m_object->isMovable()) {
		QRectF obj(QPointF(0, 0), m_object->size());
		QRectF tl(0, 0, 6, 6);
		QRectF tr(obj.width() - 6, 0, 6, 6);
		QRectF bl(0, obj.height() - 6, 6, 6);
		QRectF br(obj.width() - 6, obj.height() - 6, 6, 6);

		const double centerH = obj.width() * 0.5;
		const double centerV = obj.height() * 0.5;
		QRectF ct(centerH - 3, 0, 6, 6);
		QRectF cb(centerH - 3, obj.height() - 6, 6, 6);
		QRectF cl(0, centerV - 3, 6, 6);
		QRectF cr(obj.width() - 6, centerV - 3, 6, 6);

		const QPointF &pos = event->pos();
		if (tl.contains(pos) || br.contains(pos)) {
			setCursor(Qt::SizeFDiagCursor);
		} else if (tr.contains(pos) || bl.contains(pos)) {
			setCursor(Qt::SizeBDiagCursor);
		} else if (ct.contains(pos) || cb.contains(pos)) {
			setCursor(Qt::SizeVerCursor);
		} else if (cl.contains(pos) || cr.contains(pos)) {
			setCursor(Qt::SizeHorCursor);
		} else {
			setCursor(Qt::ArrowCursor);
		}
	}
}

void LVGLItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if (m_object && m_object->isMovable()) {
		QRectF obj(QPointF(0, 0), m_object->size());
		QRectF tl(0, 0, 6, 6);
		QRectF tr(obj.width() - 6, 0, 6, 6);
		QRectF bl(0, obj.height() - 6, 6, 6);
		QRectF br(obj.width() - 6, obj.height() - 6, 6, 6);

		const double centerH = obj.width() * 0.5;
		const double centerV = obj.height() * 0.5;
		QRectF ct(centerH - 3, 0, 6, 6);
		QRectF cb(centerH - 3, obj.height() - 6, 6, 6);
		QRectF cl(0, centerV - 3, 6, 6);
		QRectF cr(obj.width() - 6, centerV - 3, 6, 6);

		const QPointF &pos = event->pos();
		if (cl.contains(pos) || tl.contains(pos) || bl.contains(pos)) {
			m_direction.horizontal = ResizeDirections::Left;
			m_start = QRectF(pos, obj.size());
		} else if (cr.contains(pos) || tr.contains(pos) || br.contains(pos)) {
			m_direction.horizontal = ResizeDirections::Right;
			m_start = QRectF(pos, obj.size());
		} else {
			m_direction.horizontal = ResizeDirections::HorzNone;
		}

		if (ct.contains(pos) || tl.contains(pos) || tr.contains(pos)) {
			m_direction.vertical = ResizeDirections::Top;
			m_start = QRectF(pos, obj.size());
		} else if (cb.contains(pos) || bl.contains(pos) || br.contains(pos)) {
			m_direction.vertical = ResizeDirections::Bottom;
			m_start = QRectF(pos, obj.size());
		} else {
			m_direction.vertical = ResizeDirections::VertNone;
		}
	} else {
		QGraphicsItem::mousePressEvent(event);
	}
}

void LVGLItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if (m_direction.any() && m_object && m_object->isMovable()) {
		lv_obj_t *obj = m_object->obj();
		const QPointF &pos = event->pos();
		QRect bounds = (m_object->parent() ? m_object->parent()->geometry() : QRect(QPoint(), lvgl.size()));
		if (m_direction.horizontal == ResizeDirections::Right) {
			const QPoint abs = m_object->absolutePosition();
			const auto delta = pos.x() - m_start.x();
			const lv_coord_t newSize = static_cast<lv_coord_t>(m_start.width() + delta);
			const lv_coord_t maximumWidth = static_cast<lv_coord_t>(lvgl.width() - abs.x());
			m_object->setWidth(qBound(m_minimumWidth, newSize, maximumWidth));
		} else if (m_direction.horizontal == ResizeDirections::Left) {
			int delta = pos.toPoint().x() - m_start.toRect().x();

			if (obj->coords.x1 + delta < bounds.x())
				delta = bounds.x() - obj->coords.x1;
			if (delta == 0) return;

			int newWidth = m_object->width() - delta;
			if (newWidth < m_minimumWidth) return;

			moveBy(delta, 0);
			m_object->setWidth(newWidth);
			emit geometryChanged();
		}
		if (m_direction.vertical == ResizeDirections::Bottom) {
			const QPoint abs = m_object->absolutePosition();
			const auto delta = pos.y() - m_start.y();
			const lv_coord_t newSize = static_cast<lv_coord_t>(m_start.height() + delta);
			const lv_coord_t maximumWidth = static_cast<lv_coord_t>(lvgl.height() - abs.y());
			m_object->setHeight(qBound(m_minimumHeight, newSize, maximumWidth));
		} else if (m_direction.vertical == ResizeDirections::Top) {
			int delta = pos.toPoint().y() - m_start.toRect().y();

			if (obj->coords.y1 + delta < bounds.y())
				delta = bounds.y() - obj->coords.y1;
			if (delta == 0) return;

			int newHeight = m_object->height() - delta;
			if (newHeight < m_minimumHeight) return;

			moveBy(0, delta);
			m_object->setHeight(newHeight);
			emit geometryChanged();
		}
	} else {
		QGraphicsItem::mouseMoveEvent(event);
	}
}

void LVGLItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if (m_direction.any()) {
		m_direction.clear();
	} else {
		QGraphicsItem::mouseReleaseEvent(event);
	}
}

LVGLObject *LVGLItem::object() const
{
	return m_object;
}

void LVGLItem::setObject(LVGLObject *obj)
{
	if (obj == m_object)
		return;

	if (m_object) {
		disconnect(m_object, &LVGLObject::positionChanged,
					  this, &LVGLItem::updateGeometry
					  );
	}

	m_object = obj;
	m_direction.clear();
	if (obj) {
		setFlag(QGraphicsItem::ItemSendsGeometryChanges, obj->isMovable());
		setFlag(QGraphicsItem::ItemIsMovable, obj->isMovable());
		setAcceptHoverEvents(obj->isMovable());
		setPos(obj->absolutePosition());
		connect(m_object, &LVGLObject::positionChanged,
				  this, &LVGLItem::updateGeometry
				  );
	}
}

bool LVGLItem::isManipolating() const
{
	return m_direction.any();
}

void LVGLItem::updateGeometry()
{
	if (m_object == nullptr)
		return;
	if (m_object->position() != pos())
		setPos(m_object->position());
	update();
}
