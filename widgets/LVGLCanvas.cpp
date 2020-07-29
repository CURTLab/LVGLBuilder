#include "LVGLCanvas.h"

#include <QIcon>

#include "LVGLObject.h"

LVGLCanvas::LVGLCanvas()
{
	m_editableStyles << LVGL::Body; // LV_CANVAS_STYLE_MAIN
}

QString LVGLCanvas::name() const
{
	return "Canvas";
}

QString LVGLCanvas::className() const
{
	return "lv_canvas";
}

LVGLWidget::Type LVGLCanvas::type() const
{
	return Canvas;
}

QIcon LVGLCanvas::icon() const
{
	return QIcon();
}

lv_obj_t *LVGLCanvas::newObject(lv_obj_t *parent) const
{
	lv_color_t *cbuf = new lv_color_t[LV_CANVAS_BUF_SIZE_TRUE_COLOR(minimumSize().width(), minimumSize().height())];
	lv_obj_t *obj = lv_canvas_create(parent, nullptr);
	lv_canvas_set_buffer(obj, cbuf, minimumSize().width(), minimumSize().height(), LV_IMG_CF_TRUE_COLOR);
	lv_canvas_fill_bg(obj, lv_color_hex(0xc0c0c0));
	return obj;
}

QSize LVGLCanvas::minimumSize() const
{
	return QSize(100, 75);
}

QStringList LVGLCanvas::styles() const
{
	return QStringList() << "LV_CANVAS_STYLE_MAIN";
}

lv_style_t *LVGLCanvas::style(lv_obj_t *obj, int type) const
{
	return const_cast<lv_style_t*>(lv_canvas_get_style(obj, type & 0xff));
}

void LVGLCanvas::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const
{
	lv_canvas_set_style(obj, static_cast<lv_btn_style_t>(type), style);
}

lv_style_t *LVGLCanvas::defaultStyle(int type) const
{
	if (type == LV_CANVAS_STYLE_MAIN)
		return &lv_style_pretty;
	return nullptr;
}
