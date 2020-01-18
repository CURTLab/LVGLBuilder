#ifndef LVGLITEM_H
#define LVGLITEM_H

#include <QGraphicsItem>

#include "LVGLObject.h"

class LVGLItem : public QObject, public QGraphicsItem
{
	Q_OBJECT
	Q_INTERFACES(QGraphicsItem)
public:
	LVGLItem(QGraphicsItem *parent = nullptr);

	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

	LVGLObject *object() const;
	void setObject(LVGLObject *obj);

	bool isManipolating() const;

signals:
	void geometryChanged();

public slots:
	void updateGeometry();

protected:
	QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
	void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
	struct ResizeDirections
	{
		enum { HorzNone, Left, Right } horizontal;
		enum { VertNone, Top, Bottom } vertical;
		bool any() const { return horizontal || vertical; }
		void clear() { horizontal = HorzNone; vertical = VertNone; }
	};

	LVGLObject *m_object;
	QRectF m_start;
	ResizeDirections m_direction;
	lv_coord_t m_minimumWidth;
	lv_coord_t m_minimumHeight;

};

#endif // LVGLITEM_H
