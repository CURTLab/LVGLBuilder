#ifndef LVGLPROPERTYFLAGS_H
#define LVGLPROPERTYFLAGS_H

#include "LVGLProperty.h"

class LVGLPropertyFlags : public LVGLProperty
{
public:
	LVGLPropertyFlags(QStringList flagNames, QStringList flagCodeName, QList<uint8_t> flags, LVGLProperty *parent = nullptr);

	bool hasEditor() const;
	QWidget *editor(QWidget *parent);
	void updateEditor(LVGLObject *obj);
	void updateWidget(LVGLObject *obj);

	virtual QVariant value(LVGLObject *obj) const;

	QString codeValue(LVGLObject *obj) const;
	void setValue(LVGLObject *obj, QVariant value);

protected:
	QStringList m_flagNames;
	QStringList m_flagCodeName;
	QList<uint8_t> m_flags;
	QComboBox *m_widget;

	virtual int get(LVGLObject *obj) const = 0;
	virtual void set(LVGLObject *obj, int value) = 0;

private:
	class QStandardItemModel *m_model;

};

#endif // LVGLPROPERTYFLAGS_H
