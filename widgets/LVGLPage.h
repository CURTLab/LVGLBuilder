#ifndef LVGLPAGE_H
#define LVGLPAGE_H

#include "LVGLWidget.h"

class LVGLPage : public LVGLWidget
{
public:
	LVGLPage();

	virtual QString name() const;
	virtual QString className() const;
	virtual Type type() const;
	virtual QPixmap preview() const;
	virtual QIcon icon() const;
	virtual lv_obj_t *newObject(lv_obj_t *parent) const;
	virtual QSize minimumSize() const;
	virtual QStringList styles() const;
	virtual lv_style_t *style(lv_obj_t *obj, int type) const;
	virtual void setStyle(lv_obj_t *obj, int type, lv_style_t *style) const;
	virtual lv_style_t *defaultStyle(int type) const;

};

#endif // LVGLPAGE_H
