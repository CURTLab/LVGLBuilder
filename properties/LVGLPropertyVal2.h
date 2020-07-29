#ifndef LVGLPROPERTYVAL2_H
#define LVGLPROPERTYVAL2_H

#include "LVGLProperty.h"

template<class T>
class LVGLPropertyVal2 : public LVGLProperty
{
public:
	LVGLPropertyVal2(T min1, T max1, QString value1, std::function<T(lv_obj_t*)> getter1,
						  T min2, T max2, QString value2, std::function<T(lv_obj_t*)> getter2,
						  QString functionName, std::function<void(lv_obj_t*, T, T)> setter,
						  QString title, LVGLProperty *parent = nullptr);

	QString name() const override;
	QVariant value(LVGLObject *obj) const override;
	void setValue(LVGLObject *obj, QVariant value) override;

	QJsonValue toJson(LVGLObject *obj) const override;

private:
	class LVGLPropertyVal : public LVGLPropertyType<T>
	{
	public:
		LVGLPropertyVal(size_t idx, T min, T max, QString value, std::function<T (lv_obj_t *)> getter, LVGLPropertyVal2 *parent);
		QString name() const override;
		QWidget *editor(QWidget *parent) override;
		void updateEditor(LVGLObject *obj) override;
		void updateWidget(LVGLObject *obj) override;
		T get(LVGLObject *obj) const override;
		void set(LVGLObject *obj, T value) override;
	protected:
		LVGLPropertyVal2 *m_p;
		size_t m_idx;
		QSpinBox *m_widget;
		T m_min;
		T m_max;
		QString m_value;
		std::function<T(lv_obj_t*)> m_getter;
	};

	LVGLPropertyVal *m_item1;
	LVGLPropertyVal *m_item2;
	QString m_title;
	std::function<void (lv_obj_t *, T, T)> m_setter;

};

class LVGLPropertyVal2Int16 : public LVGLPropertyVal2<int16_t>
{
public:
	LVGLPropertyVal2Int16(int16_t min1, int16_t max1, QString value1, std::function<int16_t(lv_obj_t*)> getter1,
								 int16_t min2, int16_t max2, QString value2, std::function<int16_t(lv_obj_t*)> getter2,
								 QString functionName, std::function<void(lv_obj_t*, int16_t, int16_t)> setter,
								 QString title, LVGLProperty *parent = nullptr);

};

class LVGLPropertyVal2UInt16 : public LVGLPropertyVal2<uint16_t>
{
public:
	LVGLPropertyVal2UInt16(uint16_t min1, uint16_t max1, QString value1, std::function<uint16_t(lv_obj_t*)> getter1,
								  uint16_t min2, uint16_t max2, QString value2, std::function<uint16_t(lv_obj_t*)> getter2,
								  QString functionName, std::function<void(lv_obj_t*, uint16_t, uint16_t)> setter,
								  QString title, LVGLProperty *parent = nullptr);

};

#endif // LVGLPROPERTYVAL2_H
