#ifndef LVGLTEXTAREA_H
#define LVGLTEXTAREA_H

#include "LVGLWidget.h"

class LVGLTextArea : public LVGLWidget
{
public:
	LVGLTextArea();

	virtual QString name() const;
	virtual QString className() const;
	virtual Type type() const;
	virtual QIcon icon() const;
	virtual lv_obj_t *newObject(lv_obj_t *parent) const;
	virtual QSize minimumSize() const;
	virtual QStringList styles() const;
	virtual lv_style_t *style(lv_obj_t *obj, int type) const;
	virtual void setStyle(lv_obj_t *obj, int type, lv_style_t *style) const;
	virtual lv_style_t *defaultStyle(int type) const;

};

#endif // LVGLTEXTAREA_H
