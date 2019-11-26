#ifndef LVGLPROPERTY_H
#define LVGLPROPERTY_H

#include <QVariant>
#include <QJsonValue>
#include <lvgl/lvgl.h>

class LVGLObject;

class QComboBox;
class QLineEdit;
class QSpinBox;

class LVGLProperty {
public:
	LVGLProperty(LVGLProperty *parent = nullptr);
	virtual ~LVGLProperty();

	virtual QString name() const = 0;

	virtual bool hasEditor() const;
	virtual QWidget *editor(QWidget *parent);
	virtual void updateEditor(LVGLObject *obj);
	virtual void updateWidget(LVGLObject *obj);

	virtual QVariant defaultValue() const;
	virtual QVariant value(LVGLObject *obj) const;
	virtual void setValue(LVGLObject *obj, QVariant value);

	virtual QJsonValue toJson(LVGLObject *obj) const;

	const LVGLProperty *parent() const;
	int count() const;
	const LVGLProperty *child(int index) const;
	int row() const;
	int indexOf(const LVGLProperty *item) const;

	virtual QStringList function(LVGLObject *obj) const;

protected:
	const LVGLProperty *m_parent;
	QList<const LVGLProperty*> m_childs;

};

template <class T>
class LVGLPropertyType : public LVGLProperty
{
public:
	inline LVGLPropertyType(LVGLProperty *parent = nullptr) : LVGLProperty(parent) {}

	inline QVariant value(LVGLObject *obj) const override { return get(obj); }
	inline void setValue(LVGLObject *obj, QVariant value) override { return set(obj, value.value<T>()); }
	inline bool hasEditor() const override { return true; }

protected:
	virtual T get(LVGLObject *obj) const = 0;
	virtual void set(LVGLObject *obj, T value) = 0;
};

class LVGLPropertyEnum : public LVGLProperty
{
public:
	LVGLPropertyEnum(QStringList enumText, LVGLProperty *parent = nullptr);

	QVariant value(LVGLObject *obj) const override;
	void setValue(LVGLObject *obj, QVariant value) override;

	bool hasEditor() const override;

	QWidget *editor(QWidget *parent) override;
	void updateEditor(LVGLObject *obj) override;
	void updateWidget(LVGLObject *obj) override;

protected:
	QStringList m_enum;
	QComboBox *m_widget;

	virtual int get(LVGLObject *obj) const = 0;
	virtual void set(LVGLObject *obj, int value) = 0;
};

class LVGLPropertyBool : public LVGLPropertyType<bool>
{
public:
	QWidget *editor(QWidget *parent) override;
	void updateEditor(LVGLObject *obj) override;
	void updateWidget(LVGLObject *obj) override;

protected:
	QComboBox *m_widget;

};

class LVGLPropertyString : public LVGLPropertyType<QString>
{
public:
	QWidget *editor(QWidget *parent) override;
	void updateEditor(LVGLObject *obj) override;
	void updateWidget(LVGLObject *obj) override;

protected:
	QLineEdit *m_widget;

};

class LVGLPropertyCoord : public LVGLPropertyType<lv_coord_t>
{
public:
	LVGLPropertyCoord(Qt::Orientation orientation, LVGLProperty *parent = nullptr);

	QWidget *editor(QWidget *parent) override;
	void updateEditor(LVGLObject *obj) override;
	void updateWidget(LVGLObject *obj) override;

protected:
	QSpinBox *m_widget;
	Qt::Orientation m_orientation;

};

class LVGLPropertyInt : public LVGLPropertyType<int>
{
public:
	LVGLPropertyInt(int min, int max, LVGLProperty *parent = nullptr);
	LVGLPropertyInt(int min, int max, QString surfix, LVGLProperty *parent = nullptr);

	QWidget *editor(QWidget *parent) override;
	void updateEditor(LVGLObject *obj) override;
	void updateWidget(LVGLObject *obj) override;

protected:
	QSpinBox *m_widget;
	int m_min;
	int m_max;
	QString m_surfix;

};

class LVGLPropertyFont : public LVGLProperty
{
public:
	LVGLPropertyFont(LVGLProperty *parent = nullptr);

	QVariant value(LVGLObject *obj) const override;
	void setValue(LVGLObject *obj, QVariant value) override;

	bool hasEditor() const override;

	QWidget *editor(QWidget *parent) override;
	void updateEditor(LVGLObject *obj) override;
	void updateWidget(LVGLObject *obj) override;

protected:
	QComboBox *m_widget;

	virtual const lv_font_t *get(LVGLObject *obj) const = 0;
	virtual void set(LVGLObject *obj, const lv_font_t *value) = 0;
};

#endif // LVGLPROPERTY_H
