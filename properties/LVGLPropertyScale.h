#ifndef LVGLPROPERTYSCALE_H
#define LVGLPROPERTYSCALE_H

#include "LVGLProperty.h"

class LVGLPropertyScale : public LVGLProperty
{
public:
	LVGLPropertyScale(LVGLProperty *parent = nullptr);

	QString name() const override;
	QVariant value(LVGLObject *obj) const override;
	void setValue(LVGLObject *obj, QVariant value) override;

	QJsonValue toJson(LVGLObject *obj) const override;

protected:
	LVGLProperty *m_angle;
	LVGLProperty *m_line;
	LVGLProperty *m_label;

};

#endif // LVGLPROPERTYSCALE_H
