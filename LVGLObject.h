#ifndef LVGLOBJECT_H
#define LVGLOBJECT_H

#include <QObject>
#include <QJsonObject>

#include <lvgl/lvgl.h>

#include "widgets/LVGLWidget.h"

class LVGLObject : public QObject
{
	Q_OBJECT
public:
	LVGLObject(const LVGLWidget *widgetClass, QString name, LVGLObject *parent);
	LVGLObject(const LVGLWidget *widgetClass, QString name, lv_obj_t *parent);
	LVGLObject(const LVGLWidget *widgetClass, LVGLObject *parent, lv_obj_t *parentObj);
	LVGLObject(lv_obj_t *obj, const LVGLWidget *widgetClass, LVGLObject *parent, bool movable = true, int index = -1);
	~LVGLObject();

	lv_obj_t *obj() const;

	QString name() const;
	void setName(const QString &name);
	QString codeName() const;

	bool isLocked() const;
	void setLocked(bool locked);

	bool isMovable() const;

	const LVGLWidget *widgetClass() const;
	LVGLWidget::Type widgetType() const;

	QPoint absolutePosition() const;
	QPoint position() const;
	int x() const;
	int y() const;
	void setPosition(const QPoint &pos);
	void setX(int x);
	void setY(int y);

	QSize size() const;
	QRect geometry() const;
	void setGeometry(QRect geometry);

	int width() const;
	int height() const;
	void setWidth(int width);
	void setHeight(int height);

	QJsonObject toJson();
	QJsonArray jsonStyles() const;
	QStringList codeStyle(QString styleVar, int type) const;
	void parseStyles(const QJsonArray &styles);

	LVGLObject *parent() const;

	bool hasCustomStyle(int type) const;
	lv_style_t *style(int type);
	QString styleCodeName(int type) const;

	QList<LVGLObject *> childs() const;

	void removeChild(LVGLObject *child);

	bool isAccessible() const;
	void setAccessible(bool accessible);

	bool doesNameExists() const;
	static bool doesNameExists(const QString &name, LVGLObject *except = nullptr);
	void generateName();

	static LVGLObject *parse(QJsonObject object, LVGLObject *parent);

	int index() const;

	LVGLObject *findChildByIndex(int index) const;

signals:
	void positionChanged();

private:
	lv_obj_t *m_obj;
	const LVGLWidget *m_widgetClass;
	QString m_name;
	bool m_locked;
	bool m_accessible;
	bool m_movable;
	int m_index;
	LVGLObject *m_parent;
	QList<LVGLObject*> m_childs;
	QHash<int,lv_style_t> m_styles;

};

#endif // LVGLOBJECT_H
