#include "LVGLPropertyRange.h"

#include <QJsonObject>

class LVGLPropertyMin : public LVGLPropertyInt
{
public:
	inline LVGLPropertyMin(LVGLProperty *p) : LVGLPropertyInt(INT16_MIN, INT16_MAX, p) {}
	inline QString name() const override { return "Min"; }

	inline int get(LVGLObject *obj) const override {
		const LVGLPropertyRange *r = reinterpret_cast<const LVGLPropertyRange *>(m_parent);
		return r->getMin(obj);
	}
	inline void set(LVGLObject *obj, int value) override {
		LVGLPropertyRange *r = reinterpret_cast<LVGLPropertyRange *>(const_cast<LVGLProperty*>(m_parent));
		r->set(obj, value, r->getMax(obj));
	}

};

class LVGLPropertyMax : public LVGLPropertyInt
{
public:
	inline LVGLPropertyMax(LVGLProperty *p) : LVGLPropertyInt(INT16_MIN, INT16_MAX, p) {}
	inline QString name() const override { return "Max"; }

	inline int get(LVGLObject *obj) const override {
		const LVGLPropertyRange *r = reinterpret_cast<const LVGLPropertyRange *>(m_parent);
		return r->getMax(obj);
	}
	inline void set(LVGLObject *obj, int value) override {
		LVGLPropertyRange *r = reinterpret_cast<LVGLPropertyRange *>(const_cast<LVGLProperty*>(m_parent));
		r->set(obj, r->getMin(obj), value);
	}

};

LVGLPropertyRange::LVGLPropertyRange(LVGLProperty *parent)
	: LVGLProperty(parent)
	, m_min(new LVGLPropertyMin(this))
	, m_max(new LVGLPropertyMax(this))
{
	m_childs << m_min << m_max;
}

QString LVGLPropertyRange::name() const
{
	return "Range";
}

QVariant LVGLPropertyRange::value(LVGLObject *obj) const
{
	return QString("[%1,%2]").arg(getMin(obj)).arg(getMax(obj));
}

void LVGLPropertyRange::setValue(LVGLObject *obj, QVariant value)
{
	if (value.type() == QVariant::Map) {
		QVariantMap map = value.toMap();
		set(obj, map["min"].toInt(), map["max"].toInt());
	}
}

QJsonValue LVGLPropertyRange::toJson(LVGLObject *obj) const
{
	return QJsonObject({{"min", getMin(obj)},
							  {"max", getMax(obj)}});
}
