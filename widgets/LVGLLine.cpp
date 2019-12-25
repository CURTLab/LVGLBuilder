#include "LVGLLine.h"

#include <QIcon>
#include "LVGLObject.h"
#include "properties/LVGLPropertyPoints.h"

class LVGLPropertyLineAutoSize : public LVGLPropertyBool
{
public:
	QString name() const { return "Auto size"; }

protected:
	bool get(LVGLObject *obj) const { return lv_line_get_auto_size(obj->obj()); }
	void set(LVGLObject *obj, bool statue) { lv_line_set_auto_size(obj->obj(), statue); }
};

class LVGLPropertyLineInvertY : public LVGLPropertyBool
{
public:
	QString name() const { return "InvertY"; }

protected:
	bool get(LVGLObject *obj) const { return lv_line_get_y_invert(obj->obj()); }
	void set(LVGLObject *obj, bool statue) { lv_line_set_y_invert(obj->obj(), statue); }
};

class LVGLPropertyLinePoints : public LVGLPropertyPoints
{
public:
	QString name() const { return "Points"; }

	QStringList function(LVGLObject *obj) const {
		lv_line_ext_t * ext = reinterpret_cast<lv_line_ext_t*>(lv_obj_get_ext_attr(obj->obj()));
		QStringList ret;
		QString var = obj->name() + "_points";
		ret << "static lv_point_t " + var + "[" + QString::number(ext->point_num) + "] = {";
		for (uint16_t i = 0; i < ext->point_num; ++i)
			ret << "\t{" + QString::number(ext->point_array[i].x) + "," + QString::number(ext->point_array[i].y) + "},";
		ret << "\t};";
		ret << QString("lv_line_set_points(%1, %2, %3);").arg(obj->codeName()).arg(var).arg(ext->point_num);
		return ret;
	}

protected:
	QVector<lv_point_t> get(LVGLObject *obj) const {
		lv_line_ext_t * ext = reinterpret_cast<lv_line_ext_t*>(lv_obj_get_ext_attr(obj->obj()));
		QVector<lv_point_t> ret(ext->point_num);
		memcpy(ret.data(), ext->point_array, ext->point_num * sizeof(lv_point_t));
		return ret;
	}
	void set(LVGLObject *obj, QVector<lv_point_t> data) {
		lv_line_ext_t * ext = reinterpret_cast<lv_line_ext_t*>(lv_obj_get_ext_attr(obj->obj()));
		if (ext->point_num && ext->point_array)
			delete [] ext->point_array;
		lv_point_t *points = new lv_point_t[static_cast<size_t>(data.size())];
		memcpy(points, data.data(), static_cast<size_t>(data.size()) * sizeof(lv_point_t));
		lv_line_set_points(obj->obj(), points, static_cast<uint16_t>(data.size()));
	}
};

LVGLLine::LVGLLine()
{
	m_properties << new LVGLPropertyLineAutoSize;
	m_properties << new LVGLPropertyLineInvertY;
	m_properties << new LVGLPropertyLinePoints;

	m_editableStyles << LVGL::Line; // LV_LINE_STYLE_MAIN
}

QString LVGLLine::name() const
{
	return "Line";
}

QString LVGLLine::className() const
{
	return "lv_line";
}

LVGLWidget::Type LVGLLine::type() const
{
	return Line;
}

QPixmap LVGLLine::preview() const
{
	return QPixmap();
}

QIcon LVGLLine::icon() const
{
	return QIcon();
}

lv_obj_t *LVGLLine::newObject(lv_obj_t *parent) const
{
	lv_obj_t *obj = lv_line_create(parent, nullptr);
	return obj;
}

QSize LVGLLine::minimumSize() const
{
	return QSize(200, 30);
}

QStringList LVGLLine::styles() const
{
	return QStringList() << "LV_LINE_STYLE_MAIN";
}

lv_style_t *LVGLLine::style(lv_obj_t *obj, int type) const
{
	return const_cast<lv_style_t*>(lv_line_get_style(obj, type & 0xff));
}

void LVGLLine::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const
{
	lv_line_set_style(obj, static_cast<lv_bar_style_t>(type), style);
}

lv_style_t *LVGLLine::defaultStyle(int type) const
{
	if (type == LV_LINE_STYLE_MAIN)
		return &lv_style_pretty;
	return nullptr;
}
