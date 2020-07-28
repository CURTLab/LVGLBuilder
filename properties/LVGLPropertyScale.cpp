#include "LVGLPropertyScale.h"

#include "LVGLObject.h"

class LVGLPropertyAngle : public LVGLPropertyInt
{
public:
	inline LVGLPropertyAngle(LVGLProperty *p) : LVGLPropertyInt(0, 360, p) {}
	inline QString name() const override { return "Angle"; }

	inline int get(LVGLObject *obj) const override { return lv_gauge_get_scale_angle(obj->obj()); }
	inline void set(LVGLObject *obj, int value) override {
		lv_obj_t *o = obj->obj();
		lv_gauge_set_scale(o, static_cast<uint16_t>(value),
									 lv_gauge_get_line_count(o),
									 lv_gauge_get_label_count(o));
	}

};

class LVGLPropertyLines : public LVGLPropertyInt
{
public:
	inline LVGLPropertyLines(LVGLProperty *p) : LVGLPropertyInt(0, 255, p) {}
	inline QString name() const override { return "Lines"; }

	inline int get(LVGLObject *obj) const override { return lv_gauge_get_line_count(obj->obj()); }
	inline void set(LVGLObject *obj, int value) override {
		lv_obj_t *o = obj->obj();
		lv_gauge_set_scale(o, lv_gauge_get_scale_angle(o),
									 static_cast<uint8_t>(value),
									 lv_gauge_get_label_count(o));
	}

};

class LVGLPropertyLabels : public LVGLPropertyInt
{
public:
	inline LVGLPropertyLabels(LVGLProperty *p) : LVGLPropertyInt(0, 255, p) {}
	inline QString name() const override { return "Labels"; }

	inline int get(LVGLObject *obj) const override { return lv_gauge_get_label_count(obj->obj()); }
	inline void set(LVGLObject *obj, int value) override {
		lv_obj_t *o = obj->obj();
		lv_gauge_set_scale(o, lv_gauge_get_scale_angle(o),
									 lv_gauge_get_line_count(o),
									 static_cast<uint8_t>(value));
	}

};

LVGLPropertyScale::LVGLPropertyScale(LVGLProperty *parent)
	: LVGLProperty(parent)
	, m_angle(new LVGLPropertyAngle(this))
	, m_line(new LVGLPropertyLines(this))
	, m_label(new LVGLPropertyLabels(this))
{
	// children will be deleted by LVGLProperty
	m_childs << m_angle;
	m_childs << m_line;
	m_childs << m_label;
}

QString LVGLPropertyScale::name() const
{
	return "Scale";
}

QVariant LVGLPropertyScale::value(LVGLObject *obj) const
{
	return QString("(%1,%2,%3)").arg(m_angle->value(obj).toInt())
										 .arg(m_line->value(obj).toInt())
										 .arg(m_label->value(obj).toInt());
}

void LVGLPropertyScale::setValue(LVGLObject *obj, QVariant value)
{
	if (value.type() == QVariant::Map) {
		QVariantMap map = value.toMap();
		lv_gauge_set_scale(obj->obj(), map["angle"].toInt() & 0xfff,
												 map["lines"].toInt() & 0xff,
												 map["labels"].toInt() & 0xff);
	}
}

QJsonValue LVGLPropertyScale::toJson(LVGLObject *obj) const
{
	return QJsonObject({{"angle", m_angle->value(obj).toInt()},
							  {"lines", m_line->value(obj).toInt()},
							  {"labels", m_label->value(obj).toInt()}});
}

QStringList LVGLPropertyScale::function(LVGLObject *obj) const
{
	QStringList ret;
	ret << QString("lv_gauge_set_scale(%1, %2, %3, %4);").arg(obj->codeName())
																		  .arg(m_angle->get(obj))
																		  .arg(m_line->get(obj))
																		  .arg(m_label->get(obj));
	return ret;
}
