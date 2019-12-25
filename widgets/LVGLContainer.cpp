#include "LVGLContainer.h"

#include <QIcon>
#include "LVGLObject.h"

class LVGLPropertyContainerLayout : public LVGLPropertyEnum
{
public:
	LVGLPropertyContainerLayout()
		: LVGLPropertyEnum({"Off", "Center", "Left-justified column", "Centered column", "Right-justified column",
								  "Top-justified row", "Centered row", "Bottom-justified row", "Pretty", "Grid"})
		, m_values({"LV_LAYOUT_OFF", "LV_LAYOUT_CENTER", "LV_LAYOUT_COL_L", "LV_LAYOUT_COL_M", "LV_LAYOUT_COL_R",
						"LV_LAYOUT_ROW_T", "LV_LAYOUT_ROW_M", "LV_LAYOUT_ROW_B", "LV_LAYOUT_PRETTY", "LV_LAYOUT_GRID"})
	{}

	QString name() const { return "Layout"; }

	QStringList function(LVGLObject *obj) const {
		if (get(obj) != LV_BTN_STATE_REL)
			return QStringList() << QString("lv_cont_set_layout(%1, %2);").arg(obj->codeName()).arg(m_values.at(get(obj)));
		return QStringList();
	}

protected:
	int get(LVGLObject *obj) const { return lv_cont_get_layout(obj->obj()); }
	void set(LVGLObject *obj, int index) { lv_cont_set_layout(obj->obj(), index & 0xff); }

	QStringList m_values;
};


LVGLContainer::LVGLContainer()
{
	m_properties << new LVGLPropertyContainerLayout;

	m_editableStyles << LVGL::StyleParts(LVGL::Body | LVGL::Image); // LV_CONT_STYLE_MAIN
}

QString LVGLContainer::name() const
{
	return "Container";
}

QString LVGLContainer::className() const
{
	return "lv_cont";
}

LVGLWidget::Type LVGLContainer::type() const
{
	return Container;
}

QPixmap LVGLContainer::preview() const
{
	return QPixmap();
}

QIcon LVGLContainer::icon() const
{
	return QIcon();
}

lv_obj_t *LVGLContainer::newObject(lv_obj_t *parent) const
{
	lv_obj_t *obj = lv_cont_create(parent, nullptr);
	return obj;
}

QSize LVGLContainer::minimumSize() const
{
	return QSize(100, 100);
}

QStringList LVGLContainer::styles() const
{
	return QStringList() << "LV_CONT_STYLE_MAIN";
}

lv_style_t *LVGLContainer::style(lv_obj_t *obj, int type) const
{
	return const_cast<lv_style_t*>(lv_cont_get_style(obj, type & 0xff));
}

void LVGLContainer::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const
{
	lv_cont_set_style(obj, static_cast<lv_btn_style_t>(type), style);
}

lv_style_t *LVGLContainer::defaultStyle(int type) const
{
	if (type == LV_CONT_STYLE_MAIN)
		return &lv_style_pretty;
	return nullptr;
}
