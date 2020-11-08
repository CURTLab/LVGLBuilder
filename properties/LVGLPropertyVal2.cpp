#include "LVGLPropertyVal2.h"

#include <QSpinBox>
#include "LVGLObject.h"

template<class T>
LVGLPropertyVal2<T>::LVGLPropertyVal2(T min1, T max1, QString value1, std::function<T (lv_obj_t *)> getter1,
												  T min2, T max2, QString value2, std::function<T (lv_obj_t *)> getter2,
												  QString functionName, std::function<void (lv_obj_t *, T, T)> setter, QString title, LVGLProperty *parent)
	: LVGLProperty(parent)
	, m_title(title)
	, m_functionName(functionName)
	, m_setter(setter)
	, m_getter1(getter1)
	, m_getter2(getter2)
{
	m_setter1 = [&](lv_obj_t *obj, T val) {
		m_setter(obj, val, m_getter2(obj));
	};
	m_setter2 = [&](lv_obj_t *obj, T val) {
		m_setter(obj, m_getter1(obj), val);
	};
	m_item1 = new LVGLPropertyValT<T>(min1, max1, value1, "", m_setter1, m_getter1, this);
	m_item2 = new LVGLPropertyValT<T>(min2, max2, value2, "", m_setter2, m_getter2, this);
	m_childs << m_item1 << m_item2;
}

template<class T>
QString LVGLPropertyVal2<T>::name() const
{
	return m_title;
}

template<class T>
QVariant LVGLPropertyVal2<T>::value(LVGLObject *obj) const
{
	return QString("[%1,%2]").arg(m_getter1(obj->obj())).arg(m_getter2(obj->obj()));
}

template<class T>
void LVGLPropertyVal2<T>::setValue(LVGLObject *obj, QVariant value)
{
	if (value.type() == QVariant::Map) {
		QVariantMap map = value.toMap();
		m_setter(obj->obj(), map[m_item1->name().toLower()].toInt(), map[m_item2->name().toLower()].toInt());
	}
}

template<class T>
QJsonValue LVGLPropertyVal2<T>::toJson(LVGLObject *obj) const
{
	return QJsonObject({{m_item1->name().toLower(), m_getter1(obj->obj())},
							  {m_item2->name().toLower(), m_getter2(obj->obj())}});
}

template<class T>
QStringList LVGLPropertyVal2<T>::function(LVGLObject *obj) const
{
	if (m_functionName.isEmpty())
		return {};
	return { QString("%1(%2, %3, %3);").arg(m_functionName).arg(obj->codeName()).arg(m_getter1(obj->obj())).arg(m_getter1(obj->obj())) };
}

LVGLPropertyVal2Int16::LVGLPropertyVal2Int16(int16_t min1, int16_t max1, QString value1, std::function<int16_t (lv_obj_t *)> getter1,
															int16_t min2, int16_t max2, QString value2, std::function<int16_t (lv_obj_t *)> getter2,
															QString functionName, std::function<void (lv_obj_t *, int16_t, int16_t)> setter, QString title, LVGLProperty *parent)
	: LVGLPropertyVal2<int16_t>(min1, max1, value1, getter1, min2, max2, value2, getter2, functionName, setter, title, parent)
{
}

LVGLPropertyVal2UInt16::LVGLPropertyVal2UInt16(uint16_t min1, uint16_t max1, QString value1, std::function<uint16_t (lv_obj_t *)> getter1,
															  uint16_t min2, uint16_t max2, QString value2, std::function<uint16_t (lv_obj_t *)> getter2,
															  QString functionName, std::function<void (lv_obj_t *, uint16_t, uint16_t)> setter, QString title, LVGLProperty *parent)
	: LVGLPropertyVal2<uint16_t>(min1, max1, value1, getter1, min2, max2, value2, getter2, functionName, setter, title, parent)
{
}
