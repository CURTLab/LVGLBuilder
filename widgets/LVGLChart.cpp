#include "LVGLChart.h"

#include <QIcon>

#include "LVGLObject.h"
#include "properties/LVGLPropertyRange.h"
#include "properties/LVGLPropertySeries.h"
#include "properties/LVGLPropertyFlags.h"

class LVGLPropertyChartType : public LVGLPropertyFlags
{
public:
	LVGLPropertyChartType()
		: LVGLPropertyFlags({"None", "Line", "Columns", "Points", "Area", "Vertical lines"},
								  {"LV_CHART_TYPE_NONE", "LV_CHART_TYPE_LINE", "LV_CHART_TYPE_COLUMN", "LV_CHART_TYPE_POINT", "LV_CHART_TYPE_AREA", "LV_CHART_TYPE_VERTICAL_LINE"},
								  {LV_CHART_TYPE_NONE, LV_CHART_TYPE_LINE, LV_CHART_TYPE_COLUMN, LV_CHART_TYPE_POINT, LV_CHART_TYPE_AREA, LV_CHART_TYPE_VERTICAL_LINE})
	{}

	QString name() const { return "Type"; }

	QStringList function(LVGLObject *obj) const {
		return QStringList() << QString("lv_chart_set_type(%1, %2);").arg(obj->codeName()).arg(codeValue(obj));
	}

protected:
	int get(LVGLObject *obj) const { return lv_chart_get_type(obj->obj()); }
	void set(LVGLObject *obj, int index) { lv_chart_set_type(obj->obj(), index & 0xff); }

};

class LVGLPropertyChartRange : public LVGLPropertyRange
{
public:
	QString name() const override { return "Y range"; }

	QStringList function(LVGLObject *obj) const override {
		return QStringList() << QString("lv_chart_set_range(%1, %2, %3);").arg(obj->codeName()).arg(getMin(obj)).arg(getMax(obj));
	}

protected:
	int getMin(LVGLObject *obj) const override {
		return reinterpret_cast<lv_chart_ext_t*>(lv_obj_get_ext_attr(obj->obj()))->ymin;
	}
	int getMax(LVGLObject *obj) const override {
		return reinterpret_cast<lv_chart_ext_t*>(lv_obj_get_ext_attr(obj->obj()))->ymax;
	}
	void set(LVGLObject *obj, int min, int max) override {
		lv_chart_set_range(obj->obj(), static_cast<lv_coord_t>(min), static_cast<lv_coord_t>(max));
	}

};

class LVGLPropertyChartXDiv : public LVGLPropertyInt
{
public:
	LVGLPropertyChartXDiv() : LVGLPropertyInt(0, 255) {}

	QString name() const override { return "X division lines"; }

protected:
	virtual int get(LVGLObject *obj) const override {
		return reinterpret_cast<lv_chart_ext_t*>(lv_obj_get_ext_attr(obj->obj()))->vdiv_cnt;
	}
	virtual void set(LVGLObject *obj, int value) override {
		lv_chart_ext_t * ext = reinterpret_cast<lv_chart_ext_t*>(lv_obj_get_ext_attr(obj->obj()));
		lv_chart_set_div_line_count(obj->obj(), ext->hdiv_cnt, static_cast<uint8_t>(value));
	}

};

class LVGLPropertyChartYDiv : public LVGLPropertyInt
{
public:
	LVGLPropertyChartYDiv() : LVGLPropertyInt(0, 255) {}

	QString name() const override { return "Y division lines"; }

protected:
	virtual int get(LVGLObject *obj) const override {
		return reinterpret_cast<lv_chart_ext_t*>(lv_obj_get_ext_attr(obj->obj()))->hdiv_cnt;
	}
	virtual void set(LVGLObject *obj, int value) override {
		lv_chart_ext_t * ext = reinterpret_cast<lv_chart_ext_t*>(lv_obj_get_ext_attr(obj->obj()));
		lv_chart_set_div_line_count(obj->obj(), static_cast<uint8_t>(value), ext->vdiv_cnt);
	}

};

class LVGLPropertyChartWidth : public LVGLPropertyCoord
{
public:
	LVGLPropertyChartWidth(LVGLProperty *p) : LVGLPropertyCoord(p) {}

	QString name() const override { return "Width"; }

protected:
	virtual lv_coord_t get(LVGLObject *obj) const override { return lv_chart_get_series_width(obj->obj()); }
	virtual void set(LVGLObject *obj, lv_coord_t value) override { lv_chart_set_series_width(obj->obj(), value); }

};

class LVGLPropertyChartOpa : public LVGLPropertyInt
{
public:
	LVGLPropertyChartOpa(LVGLProperty *p) : LVGLPropertyInt(0, 255, p) {}

	QString name() const override { return "Opacity"; }

protected:
	virtual int get(LVGLObject *obj) const override { return lv_chart_get_series_opa(obj->obj()); }
	virtual void set(LVGLObject *obj, int value) override { lv_chart_set_series_opa(obj->obj(), static_cast<lv_opa_t>(value)); }

};

class LVGLPropertyChartDarking : public LVGLPropertyInt
{
public:
	LVGLPropertyChartDarking(LVGLProperty *p) : LVGLPropertyInt(0, 255, p) {}

	QString name() const override { return "Dark fade"; }

protected:
	virtual int get(LVGLObject *obj) const override { return lv_chart_get_series_darking(obj->obj()); }
	virtual void set(LVGLObject *obj, int value) override { lv_chart_set_series_darking(obj->obj(), static_cast<lv_opa_t>(value)); }

};

class LVGLPropertyChartSeries : public LVGLProperty
{
	LVGLPropertyChartWidth *m_w;
	LVGLPropertyChartOpa *m_opa;
	LVGLPropertyChartDarking *m_darking;
public:
	LVGLPropertyChartSeries()
		: m_w(new LVGLPropertyChartWidth(this))
		, m_opa(new LVGLPropertyChartOpa(this))
		, m_darking(new LVGLPropertyChartDarking(this))
	{
		m_childs << m_w << m_opa << m_darking;
	}

	QString name() const override { return "Series"; }
};

class LVGLPropertyChartMargin : public LVGLPropertyInt
{
public:
	LVGLPropertyChartMargin(LVGLProperty *p = nullptr) : LVGLPropertyInt(0, UINT16_MAX, p) {}

	QString name() const override { return "Margin"; }

protected:
	virtual int get(LVGLObject *obj) const override { return lv_chart_get_margin(obj->obj()); }
	virtual void set(LVGLObject *obj, int value) override { lv_chart_set_margin(obj->obj(), static_cast<uint16_t>(value)); }

};

LVGLChart::LVGLChart()
{
	m_properties << new LVGLPropertyChartType;
	m_properties << new LVGLPropertyChartRange;
	m_properties << new LVGLPropertyChartXDiv;
	m_properties << new LVGLPropertyChartYDiv;
	m_properties << new LVGLPropertyChartSeries;
	m_properties << new LVGLPropertyChartMargin;
	m_properties << new LVGLPropertySeries;

	m_editableStyles << LVGL::StyleParts(LVGL::Body | LVGL::Line | LVGL::Text); // LV_CHART_STYLE_MAIN
}

QString LVGLChart::name() const
{
	return "Chart";
}

QString LVGLChart::className() const
{
	return "lv_chart";
}

LVGLWidget::Type LVGLChart::type() const
{
	return Chart;
}

QIcon LVGLChart::icon() const
{
	return QIcon();
}

lv_obj_t *LVGLChart::newObject(lv_obj_t *parent) const
{
	lv_obj_t *obj = lv_chart_create(parent, nullptr);
	return obj;
}

QSize LVGLChart::minimumSize() const
{
	return QSize(200, 150);
}

QStringList LVGLChart::styles() const
{
	return QStringList() << "LV_CHART_STYLE_MAIN";
}

lv_style_t *LVGLChart::style(lv_obj_t *obj, int type) const
{
	return const_cast<lv_style_t*>(lv_chart_get_style(obj, type & 0xff));
}

void LVGLChart::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const
{
	lv_chart_set_style(obj, static_cast<lv_btn_style_t>(type), style);
}

lv_style_t *LVGLChart::defaultStyle(int type) const
{
	if (type == LV_CHART_STYLE_MAIN)
		return &lv_style_pretty;
	return nullptr;
}
