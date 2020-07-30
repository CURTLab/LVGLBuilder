#ifndef LVGLPROPERTYTEXTLIST_H
#define LVGLPROPERTYTEXTLIST_H

#include "LVGLProperty.h"

class LVGLPropertyTextList : public LVGLProperty
{
public:
	LVGLPropertyTextList(bool canInsert = true, LVGLProperty *parent = nullptr);

	bool hasEditor() const;
	QWidget *editor(QWidget *parent);
	void updateEditor(LVGLObject *obj);
	void updateWidget(LVGLObject *obj);

	QVariant value(LVGLObject *obj) const;
	void setValue(LVGLObject *obj, QVariant value);

	QJsonValue toJson(LVGLObject *obj) const;

protected:
	class LVGLPropertyTextListDialog *m_widget;
	bool m_canInsert;

	virtual QStringList get(LVGLObject *obj) const = 0;
	virtual void set(LVGLObject *obj, QStringList list) = 0;

};

#endif // LVGLPROPERTYTEXTLIST_H
