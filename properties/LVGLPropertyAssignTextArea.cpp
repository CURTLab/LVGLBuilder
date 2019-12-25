#include "LVGLPropertyAssignTextArea.h"

#include <QComboBox>
#include "LVGLObject.h"
#include "LVGLCore.h"

LVGLPropertyAssignTextArea::LVGLPropertyAssignTextArea(LVGLProperty *parent)
	: LVGLProperty(parent)
{
}

QString LVGLPropertyAssignTextArea::name() const
{
	return "Assign text area";
}

bool LVGLPropertyAssignTextArea::hasEditor() const
{
	return true;
}

QWidget *LVGLPropertyAssignTextArea::editor(QWidget *parent)
{
	m_widget = new QComboBox(parent);
	QStringList list = QStringList() << "None";
	for (LVGLObject *o:lvgl.objectsByType("lv_ta"))
		list << o->name();
	m_widget->addItems(list);
	return m_widget;
}

void LVGLPropertyAssignTextArea::updateEditor(LVGLObject *obj)
{
	LVGLObject *ta = lvgl.object(lv_kb_get_ta(obj->obj()));
	if (ta == nullptr)
		m_widget->setCurrentIndex(0);
	else
		m_widget->setCurrentText(ta->name());
}

void LVGLPropertyAssignTextArea::updateWidget(LVGLObject *obj)
{
	LVGLObject *ta = lvgl.object(m_widget->currentText());
	if (ta)
		lv_kb_set_ta(obj->obj(), ta->obj());
	else
		lv_kb_set_ta(obj->obj(), nullptr);
}

QVariant LVGLPropertyAssignTextArea::value(LVGLObject *obj) const
{
	LVGLObject *ta = lvgl.object(lv_kb_get_ta(obj->obj()));
	if (ta == nullptr)
		return "None";
	else
		return ta->name();
}

void LVGLPropertyAssignTextArea::setValue(LVGLObject *obj, QVariant value)
{
	LVGLObject *ta = lvgl.object(value.toString());
	if (ta)
		lv_kb_set_ta(obj->obj(), ta->obj());
	else
		lv_kb_set_ta(obj->obj(), nullptr);
}

QStringList LVGLPropertyAssignTextArea::function(LVGLObject *obj) const
{
	LVGLObject *ta = lvgl.object(lv_kb_get_ta(obj->obj()));
	if (ta == nullptr) return QStringList();
	return QStringList() << QString("lv_kb_set_ta(%1, %2);").arg(obj->codeName()).arg(ta->codeName());
}
