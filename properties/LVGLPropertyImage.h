#ifndef LVGLPROPERTYIMAGE_H
#define LVGLPROPERTYIMAGE_H

#include "LVGLProperty.h"

class LVGLPropertyImage: public LVGLProperty
{
public:
	LVGLPropertyImage(LVGLProperty *parent = nullptr);

	bool hasEditor() const;
	QWidget *editor(QWidget *parent);
	void updateEditor(LVGLObject *obj);
	void updateWidget(LVGLObject *obj);

	QVariant value(LVGLObject *obj) const;
	void setValue(LVGLObject *obj, QVariant value);

protected:
	QComboBox *m_widget;

	virtual const lv_img_dsc_t *get(LVGLObject *obj) const = 0;
	virtual void set(LVGLObject *obj, const lv_img_dsc_t *img) = 0;

};

#endif // LVGLPROPERTYIMAGE_H
