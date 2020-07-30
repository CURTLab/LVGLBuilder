#include "LVGLWidget.h"

#include "properties/LVGLPropertyGeometry.h"
#include "LVGLObject.h"

class LVGLPropertyName : public LVGLPropertyString
{
public:
	inline QString name() const override { return "Name"; }

protected:
	inline QString get(LVGLObject *obj) const override { return obj->name(); }
	inline void set(LVGLObject *obj, QString string) override { obj->setName(string); }
};

class LVGLPropertyAccessible : public LVGLPropertyBool
{
public:
	inline LVGLPropertyAccessible() : LVGLPropertyBool("Accessible") {}

protected:
	inline bool get(LVGLObject *obj) const override { return obj->isAccessible(); }
	inline void set(LVGLObject *obj, bool statue) override { obj->setAccessible(statue); }
};

class LVGLPropertyLocked : public LVGLPropertyBool
{
public:
	inline LVGLPropertyLocked() : LVGLPropertyBool("Locked") {}

protected:
	inline bool get(LVGLObject *obj) const override { return obj->isLocked(); }
	inline void set(LVGLObject *obj, bool statue) override { obj->setLocked(statue); }
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
