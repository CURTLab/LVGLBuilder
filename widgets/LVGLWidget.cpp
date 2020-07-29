#include "LVGLWidget.h"

#include "properties/LVGLPropertyGeometry.h"
#include "LVGLObject.h"

class LVGLPropertyName : public LVGLPropertyString
{
public:
	QString name() const { return "Name"; }

protected:
	QString get(LVGLObject *obj) const { return obj->name(); }
	void set(LVGLObject *obj, QString string) { obj->setName(string); }
};

class LVGLPropertyAccessible : public LVGLPropertyBool
{
public:
	QString name() const { return "Accessible"; }

protected:
	bool get(LVGLObject *obj) const { return obj->isAccessible(); }
	void set(LVGLObject *obj, bool statue) { obj->setAccessible(statue); }
};

class LVGLPropertyLocked : public LVGLPropertyBool
{
public:
	QString name() const { return "Locked"; }

protected:
	bool get(LVGLObject *obj) const { return obj->isLocked(); }
	void set(LVGLObject *obj, bool statue) { obj->setLocked(statue); }
};


LVGLWidget::LVGLWidget()
{
	m_geometryProp = new LVGLPropertyGeometry;
	m_properties << new LVGLPropertyName;
	m_properties << new LVGLPropertyAccessible;
	m_properties << new LVGLPropertyLocked;
	m_properties << m_geometryProp;
}

LVGLWidget::~LVGLWidget()
{
	qDeleteAll(m_properties);
}

QPixmap LVGLWidget::preview() const
{
	return m_preview;
}

void LVGLWidget::setPreview(QPixmap preview)
{
	m_preview = preview;
}

QVector<LVGLProperty *> LVGLWidget::properties() const
{
	return m_properties;
}

QList<LVGLProperty *> LVGLWidget::specialProperties() const
{
	QList<LVGLProperty *> ret;
	for (LVGLProperty *p:m_properties) {
		if ((p->name() != "Name") &&
			 (p->name() != "Accessible") &&
			 (p->name() != "Locked") &&
			 (p->name() != "Geometry"))
			ret << p;
	}
	return ret;
}

LVGLProperty *LVGLWidget::property(QString name) const
{
	for (LVGLProperty *p:m_properties) {
		if (p->name() == name)
			return p;
	}
	return nullptr;
}

LVGL::StyleParts LVGLWidget::editableStyles(int type) const
{
	Q_ASSERT(!m_editableStyles.isEmpty());
	if (m_editableStyles.size() > type)
		return m_editableStyles.at(type);
	return LVGL::None;
}

LVGLProperty *LVGLWidget::geometryProp() const
{
	return m_geometryProp;
}
