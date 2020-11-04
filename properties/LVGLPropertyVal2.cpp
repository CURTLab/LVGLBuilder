#include "LVGLPropertyVal2.h"

#include <QSpinBox>
#include "LVGLObject.h"

template<class T>
LVGLPropertyVal2<T>::LVGLPropertyVal::LVGLPropertyVal(size_t idx, T min, T max, QString value, std::function<T (lv_obj_t *)> getter, LVGLPropertyVal2 *parent)
	: LVGLPropertyType<T>(parent)
	, m_idx(idx)
	, m_widget(nullptr)
	, m_min(min), m_max(max), m_value(value)
	, m_getter(getter)
{
}

template<class T>
QString LVGLPropertyVal2<T>::LVGLPropertyVal::name() const
{
	return m_value;
}

template<class T>
QWidget *LVGLPropertyVal2<T>::LVGLPropertyVal::editor(QWidget *parent)
{
	m_widget = new QSpinBox(parent);
	m_widget->setRange(m_min, m_max);
	return m_widget;
}

template<class T>
void LVGLPropertyVal2<T>::LVGLPropertyVal::updateEditor(LVGLObject *obj)
{
	m_widget->setValue(static_cast<int>(get(obj)));
}

template<class T>
void LVGLPropertyVal2<T>::LVGLPropertyVal::updateWidget(LVGLObject *obj)
{
	set(obj, static_cast<T>(m_widget->value()));
}

template<class T>
T LVGLPropertyVal2<T>::LVGLPropertyVal::get(LVGLObject *obj) const
{
	return m_getter(obj->obj());
}

template<class T>
void LVGLPropertyVal2<T>::LVGLPropertyVal::set(LVGLObject *obj, T value)
{
	LVGLPropertyVal2<T> *d = reinterpret_cast<LVGLPropertyVal2<T> *>(m_parent);
	if (m_idx == 0)
		d->m_setter(obj->obj(), value, d->m_item2->get(obj));
	else if (m_idx == 1)
		d->m_setter(obj->obj(), d->m_item1->get(obj), value);
}

template<class T>
LVGLPropertyVal2<T>::LVGLPropertyVal2(T min1, T max1, QString value1, std::function<T (lv_obj_t *)> getter1,
												  T min2, T max2, QString value2, std::function<T (lv_obj_t *)> getter2,
												  QString functionName, std::function<void (lv_obj_t *, T, T)> setter, QString title, LVGLProperty *parent)
	: LVGLProperty(parent)
	, m_title(title)
	, m_setter(setter)
{
	m_item1 = new LVGLPropertyVal(0, min1, max1, value1, getter1, this);
	m_item2 = new LVGLPropertyVal(1, min2, max2, value2, getter2, this);
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
	return QString("[%1,%2]").arg(m_item1->get(obj)).arg(m_item2->get(obj));
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
	return QJsonObject({{m_item1->name().toLower(), m_item1->get(obj)},
							  {m_item2->name().toLower(), m_item2->get(obj)}});
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
