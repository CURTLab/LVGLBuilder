#ifndef LVGLPROPERTYLIST_H
#define LVGLPROPERTYLIST_H

#include "LVGLProperty.h"
#include <QObject>
#include <QDialog>

struct LVGLListItem
{
	QString name;
	void *image_src;
};

class LVGLPropertyListDialog : public QDialog
{
public:
	LVGLPropertyListDialog(QWidget *parent = nullptr);

	void setListItems(QList<LVGLListItem> items);
	QList<LVGLListItem> listItems() const;

private:
	class QTreeWidget *m_list;

};

class LVGLPropertyList : public LVGLProperty
{
public:
	LVGLPropertyList(LVGLProperty *parent = nullptr);

	QString name() const;

	bool hasEditor() const;
	QWidget *editor(QWidget *parent);
	void updateEditor(LVGLObject *obj);
	void updateWidget(LVGLObject *obj);

	QVariant value(LVGLObject *obj) const;
	void setValue(LVGLObject *obj, QVariant value);

	QJsonValue toJson(LVGLObject *obj) const;

	QStringList function(LVGLObject *obj) const;

protected:
	class LVGLPropertyListDialog *m_widget;

};

#endif // LVGLPROPERTYLIST_H
