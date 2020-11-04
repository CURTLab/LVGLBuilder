#ifndef LVGLPROPERTYRANGE_H
#define LVGLPROPERTYRANGE_H

#include "LVGLProperty.h"

// forward declaration
class LVGLPropertyMin;
class LVGLPropertyMax;

class LVGLPropertyRange : public LVGLProperty
{
public:
	LVGLPropertyRange(LVGLProperty *parent = nullptr);

	QString name() const override;
	QVariant value(LVGLObject *obj) const override;
	void setValue(LVGLObject *obj, QVariant value) override;

	QJsonValue toJson(LVGLObject *obj) const override;

protected:
	friend class LVGLPropertyMin;
	friend class LVGLPropertyMax;

	LVGLPropertyMin *m_min;
	LVGLPropertyMax *m_max;

	virtual int getMin(LVGLObject *obj) const = 0;
	virtual int getMax(LVGLObject *obj) const = 0;
	virtual void set(LVGLObject *obj, int min, int max) = 0;

};

#endif // LVGLPROPERTYRANGE_H
