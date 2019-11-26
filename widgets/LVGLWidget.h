#ifndef LVGLWIDGET_H
#define LVGLWIDGET_H

#include "LVGLStyle.h"
#include "LVGLProperty.h"
#include <QJsonObject>

class LVGLWidget
{
public:
	LVGLWidget();
	virtual ~LVGLWidget();

	enum Type {
		Bar,
		Button,
		ButtonMatrix,
		Canvas,
		Calendar,
		CheckBox,
		ColorPicker,
		Container,
		DropDownList,
		Gauge,
		Image,
		ImageButton,
		ImageSlider,
		ImageBar,
		Keyboard,
		Label,
		Line,
		List,
		Page,
		Preloader,
		Slider,
		Switch,
		Tabview,
		TextArea
	};

	virtual QString name() const = 0;
	virtual QString className() const = 0;
	virtual Type type() const = 0;
	virtual QPixmap preview() const = 0;
	virtual QIcon icon() const = 0;
	virtual QSize minimumSize() const = 0;
	virtual lv_obj_t *newObject(lv_obj_t *parent) const = 0;
	virtual QStringList styles() const = 0;
	virtual lv_style_t *style(lv_obj_t *obj, int type) const = 0;
	virtual void setStyle(lv_obj_t *obj, int type, lv_style_t *style) const = 0;
	virtual lv_style_t *defaultStyle(int type) const = 0;

	QVector<LVGLProperty*> properties() const;
	QList<LVGLProperty*> specialProperties() const;

	LVGLProperty *property(QString name) const;

	LVGL::StyleParts editableStyles(int type) const;

	LVGLProperty *geometryProp() const;

protected:
	LVGLProperty *m_geometryProp;
	QVector<LVGLProperty*> m_properties;
	QList<LVGL::StyleParts> m_editableStyles;

};
Q_DECLARE_METATYPE(LVGLWidget*)
Q_DECLARE_METATYPE(const LVGLWidget*)

#endif // LVGLWIDGET_H
