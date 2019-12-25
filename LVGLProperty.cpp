#include "LVGLProperty.h"

#include "LVGLCore.h"

#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>

#include "LVGLObject.h"

/*void LVGLProperty::set(LVGLObject *obj, QVariant value) const
{
	if (functionSet == "lv_obj_set_x")
		lv_obj_set_x(obj, static_cast<lv_coord_t>(value.toInt()));
	else if (functionSet == "lv_obj_set_y")
		lv_obj_set_y(obj, static_cast<lv_coord_t>(value.toInt()));
	else if (functionSet == "lv_obj_set_width")
		lv_obj_set_width(obj, static_cast<lv_coord_t>(value.toInt()));
	else if (functionSet == "lv_obj_set_height")
		lv_obj_set_height(obj, static_cast<lv_coord_t>(value.toInt()));
	else if (functionSet == "lv_btn_set_toggle")
		lv_btn_set_toggle(obj, value.toBool());
	else if (functionSet == "lv_img_set_offset_x")
		lv_img_set_offset_x(obj, static_cast<lv_coord_t>(value.toInt()));
	else if (functionSet == "lv_img_set_offset_y")
		lv_img_set_offset_y(obj, static_cast<lv_coord_t>(value.toInt()));
	else if (functionSet == "lv_img_set_auto_size")
		lv_img_set_auto_size(obj, value.toBool());
	else if (functionSet == "lv_img_set_src") {
		lv_img_set_src(obj, lvgl.image(value.toString()));
	} else if (functionSet == "lv_label_set_text")
		lv_label_set_text(obj, qPrintable(value.toString()));
	else if (functionSet == "lv_label_set_align") {
		const QString align = value.toString();
		if (align == "LV_LABEL_ALIGN_LEFT")
			lv_label_set_align(obj, LV_LABEL_ALIGN_LEFT);
		else if (align == "LV_LABEL_ALIGN_CENTER")
			lv_label_set_align(obj, LV_LABEL_ALIGN_CENTER);
		else if (align == "LV_LABEL_ALIGN_RIGHT")
			lv_label_set_align(obj, LV_LABEL_ALIGN_RIGHT);
	} else if (functionSet == "lv_label_set_long_mode") {
		const QString mode = value.toString();
		if (mode == "LV_LABEL_LONG_EXPAND")
			lv_label_set_long_mode(obj, LV_LABEL_LONG_EXPAND);
		else if (mode == "LV_LABEL_LONG_BREAK")
			lv_label_set_long_mode(obj, LV_LABEL_LONG_BREAK);
		else if (mode == "LV_LABEL_LONG_DOT")
			lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
		else if (mode == "LV_LABEL_LONG_SROLL")
			lv_label_set_long_mode(obj, LV_LABEL_LONG_SROLL);
		else if (mode == "LV_LABEL_LONG_SROLL_CIRC")
			lv_label_set_long_mode(obj, LV_LABEL_LONG_SROLL_CIRC);
		else if (mode == "LV_LABEL_LONG_CROP")
			lv_label_set_long_mode(obj, LV_LABEL_LONG_CROP);
	} else if (functionSet == "lv_slider_set_value")
		lv_slider_set_value(obj, static_cast<int16_t>(value.toInt()), LV_ANIM_OFF);
	else if (functionSet == "lv_slider_set_min_value")
		lv_slider_set_range(obj, static_cast<int16_t>(value.toInt()), lv_slider_get_max_value(obj));
	else if (functionSet == "lv_slider_set_max_value")
		lv_slider_set_range(obj, lv_slider_get_min_value(obj), static_cast<int16_t>(value.toInt()));
	else if (functionSet == "lv_slider_set_knob_in")
		lv_slider_set_knob_in(obj, value.toBool());
	else if (functionSet == "lv_cb_set_text")
		lv_cb_set_text(obj, qPrintable(value.toString()));
	else if (functionSet == "lv_cb_set_checked")
		lv_cb_set_checked(obj, value.toBool());
	else if (functionSet == "lv_cb_set_inactive")
		lv_btn_set_state(obj, value.toBool() ? LV_BTN_STATE_INA : (lv_cb_is_checked(obj) ? LV_BTN_STATE_TGL_REL : LV_BTN_STATE_REL));
	else if (functionSet == "lv_sw_set_state") {
		if (value.toBool())
			lv_sw_on(obj, LV_ANIM_OFF);
		else
			lv_sw_off(obj, LV_ANIM_OFF);
	} else if (functionSet == "lv_bar_set_value")
		lv_bar_set_value(obj, static_cast<int16_t>(value.toInt()), LV_ANIM_OFF);
	else if (functionSet == "lv_bar_set_min_value")
		lv_bar_set_range(obj, static_cast<int16_t>(value.toInt()), lv_bar_get_max_value(obj));
	else if (functionSet == "lv_bar_set_max_value")
		lv_bar_set_range(obj, lv_bar_get_min_value(obj), static_cast<int16_t>(value.toInt()));
}

QVariant LVGLProperty::get(LVGLObject *obj) const
{
	if (functionGet == "lv_obj_get_x")
		return QVariant(lv_obj_get_x(obj));
	else if (functionGet == "lv_obj_get_y")
		return QVariant(lv_obj_get_y(obj));
	else if (functionGet == "lv_obj_get_width")
		return QVariant(lv_obj_get_width(obj));
	else if (functionGet == "lv_obj_get_height")
		return QVariant(lv_obj_get_height(obj));
	else if (functionGet == "lv_btn_get_toggle")
		return QVariant(lv_btn_get_toggle(obj));
	else if (functionGet == "lv_img_get_offset_x")
		return QVariant(lv_img_get_offset_x(obj));
	else if (functionGet == "lv_img_get_offset_y")
		return QVariant(lv_img_get_offset_y(obj));
	else if (functionGet == "lv_img_get_auto_size")
		return QVariant(lv_img_get_auto_size(obj));
	else if (functionGet == "lv_img_get_auto_size")
		return QVariant(lv_img_get_auto_size(obj));
	else if (functionGet == "lv_img_get_src") {
		const lv_img_dsc_t *dsc = reinterpret_cast<const lv_img_dsc_t*>(lv_img_get_src(obj));
		if (dsc)
			return lvgl.nameByImage(const_cast<lv_img_dsc_t *>(dsc));
		else
			return "default";
	} else if (functionGet == "lv_label_get_text")
		return QVariant(QString(lv_label_get_text(obj)));
	else if (functionGet == "lv_label_get_align") {
		const auto align = lv_label_get_align(obj);
		if (align == LV_LABEL_ALIGN_LEFT)
			return "LV_LABEL_ALIGN_LEFT";
		else if (align == LV_LABEL_ALIGN_CENTER)
			return "LV_LABEL_ALIGN_CENTER";
		else if (align == LV_LABEL_ALIGN_RIGHT)
			return "LV_LABEL_ALIGN_RIGHT";
	} else if (functionGet == "lv_label_get_long_mode") {
		const auto mode = lv_label_get_long_mode(obj);
		if (mode == LV_LABEL_LONG_EXPAND)
			return "LV_LABEL_LONG_EXPAND";
		else if (mode == LV_LABEL_LONG_BREAK)
			return "LV_LABEL_LONG_BREAK";
		else if (mode == LV_LABEL_LONG_DOT)
			return "LV_LABEL_LONG_DOT";
		else if (mode == LV_LABEL_LONG_SROLL)
			return "LV_LABEL_LONG_SROLL";
		else if (mode == LV_LABEL_LONG_SROLL_CIRC)
			return "LV_LABEL_LONG_SROLL_CIRC";
		else if (mode == LV_LABEL_LONG_CROP)
			return "LV_LABEL_LONG_CROP";
	} else if (functionGet == "lv_slider_get_value")
		return QVariant(lv_slider_get_value(obj));
	else if (functionGet == "lv_slider_get_min_value")
		return QVariant(lv_slider_get_min_value(obj));
	else if (functionGet == "lv_slider_get_max_value")
		return QVariant(lv_slider_get_max_value(obj));
	else if (functionGet == "lv_slider_get_knob_in")
		return QVariant(lv_slider_get_knob_in(obj));
	else if (functionGet == "lv_cb_get_text")
		return QVariant(lv_cb_get_text(obj));
	else if (functionGet == "lv_cb_is_checked")
		return QVariant(lv_cb_is_checked(obj));
	else if (functionGet == "lv_cb_is_inactive")
		return QVariant(!lv_cb_is_inactive(obj));
	else if (functionGet == "lv_sw_get_state")
		return QVariant(lv_sw_get_state(obj));
	else if (functionGet == "lv_bar_get_value")
		return QVariant(lv_bar_get_value(obj));
	else if (functionGet == "lv_bar_get_min_value")
		return QVariant(lv_bar_get_min_value(obj));
	else if (functionGet == "lv_bar_get_max_value")
		return QVariant(lv_bar_get_max_value(obj));
	return QVariant();
}*/

LVGLProperty::LVGLProperty(LVGLProperty *parent)
	: m_parent(parent)
{
}

LVGLProperty::~LVGLProperty()
{
	qDeleteAll(m_childs);
}

bool LVGLProperty::hasEditor() const
{
	return false;
}

QWidget *LVGLProperty::editor(QWidget *parent)
{
	(void)parent;
	return nullptr;
}

void LVGLProperty::updateEditor(LVGLObject *obj)
{
	(void)obj;
}

void LVGLProperty::updateWidget(LVGLObject *obj)
{
	(void)obj;
}

QVariant LVGLProperty::defaultValue() const
{
	return QVariant();
}

QVariant LVGLProperty::value(LVGLObject *obj) const
{
	(void)obj;
	return QVariant();
}

void LVGLProperty::setValue(LVGLObject *obj, QVariant value)
{
	(void)obj;
	(void)value;
}

QJsonValue LVGLProperty::toJson(LVGLObject *obj) const
{
	return QJsonValue::fromVariant(value(obj));
}

const LVGLProperty *LVGLProperty::parent() const
{
	return m_parent;
}

int LVGLProperty::count() const
{
	return m_childs.size();
}

const LVGLProperty *LVGLProperty::child(int index) const
{
	return m_childs.at(index);
}

int LVGLProperty::row() const
{
	return m_parent?m_parent->indexOf(this):0;
}

int LVGLProperty::indexOf(const LVGLProperty *item) const
{
	return m_childs.indexOf(item);
}

QStringList LVGLProperty::function(LVGLObject *obj) const
{
	Q_UNUSED(obj)
	return QStringList();
}

LVGLPropertyEnum::LVGLPropertyEnum(QStringList enumText, LVGLProperty *parent)
	: LVGLProperty(parent)
	, m_enum(enumText)
{
}

QVariant LVGLPropertyEnum::value(LVGLObject *obj) const
{
	const int index = get(obj);
	assert(index < m_enum.size());
	return m_enum.at(index);
}

void LVGLPropertyEnum::setValue(LVGLObject *obj, QVariant value)
{
	assert(m_enum.indexOf(value.toString()) >= 0);
	set(obj, m_enum.indexOf(value.toString()));
}

bool LVGLPropertyEnum::hasEditor() const
{
	return true;
}

QWidget *LVGLPropertyEnum::editor(QWidget *parent)
{
	m_widget = new QComboBox(parent);
	m_widget->addItems(m_enum);
	return m_widget;
}

void LVGLPropertyEnum::updateEditor(LVGLObject *obj)
{
	m_widget->setCurrentIndex(get(obj));
}

void LVGLPropertyEnum::updateWidget(LVGLObject *obj)
{
	set(obj, m_widget->currentIndex());
}

QWidget *LVGLPropertyBool::editor(QWidget *parent)
{
	m_widget = new QComboBox(parent);
	m_widget->addItems(QStringList() << "true" << "false");
	return m_widget;
}

void LVGLPropertyBool::updateEditor(LVGLObject *obj)
{
	m_widget->setCurrentIndex(get(obj) ? 0 : 1);
}

void LVGLPropertyBool::updateWidget(LVGLObject *obj)
{
	set(obj, m_widget->currentIndex() == 0);
}

QWidget *LVGLPropertyString::editor(QWidget *parent)
{
	m_widget = new QLineEdit(parent);
	return m_widget;
}

void LVGLPropertyString::updateEditor(LVGLObject *obj)
{
	m_widget->setText(get(obj));
}

void LVGLPropertyString::updateWidget(LVGLObject *obj)
{
	set(obj, m_widget->text());
}

LVGLPropertyCoord::LVGLPropertyCoord(Qt::Orientation orientation, LVGLProperty *parent)
	: LVGLPropertyType(parent)
	, m_orientation(orientation)
{
}

QWidget *LVGLPropertyCoord::editor(QWidget *parent)
{
	m_widget = new QSpinBox(parent);
	m_widget->setRange(0, (m_orientation == Qt::Horizontal ? lvgl.width() : lvgl.height()));
	return m_widget;
}

void LVGLPropertyCoord::updateEditor(LVGLObject *obj)
{
	m_widget->setValue(static_cast<int>(get(obj)));
}

void LVGLPropertyCoord::updateWidget(LVGLObject *obj)
{
	set(obj, static_cast<lv_coord_t>(m_widget->value()));
}

LVGLPropertyInt::LVGLPropertyInt(int min, int max, LVGLProperty *parent)
	: LVGLPropertyType(parent), m_min(min), m_max(max)
{
}

LVGLPropertyInt::LVGLPropertyInt(int min, int max, QString surfix, LVGLProperty *parent)
	: LVGLPropertyType(parent), m_min(min), m_max(max), m_surfix(surfix)
{
}

QWidget *LVGLPropertyInt::editor(QWidget *parent)
{
	m_widget = new QSpinBox(parent);
	m_widget->setRange(m_min, m_max);
	m_widget->setSuffix(m_surfix);
	return m_widget;
}

void LVGLPropertyInt::updateEditor(LVGLObject *obj)
{
	m_widget->setValue(get(obj));
}

void LVGLPropertyInt::updateWidget(LVGLObject *obj)
{
	set(obj, static_cast<lv_coord_t>(m_widget->value()));
}

LVGLPropertyFont::LVGLPropertyFont(LVGLProperty *parent)
	: LVGLProperty(parent)
{
}

QVariant LVGLPropertyFont::value(LVGLObject *obj) const
{
	const int index = lvgl.indexOfFont(get(obj));
	return lvgl.fontNames().at(index);
}

void LVGLPropertyFont::setValue(LVGLObject *obj, QVariant value)
{
	const int index = lvgl.fontNames().indexOf(value.toString());
	set(obj, lvgl.font(index));
}

bool LVGLPropertyFont::hasEditor() const
{
	return true;
}

QWidget *LVGLPropertyFont::editor(QWidget *parent)
{
	m_widget = new QComboBox(parent);
	m_widget->addItems(lvgl.fontNames());
	return m_widget;
}

void LVGLPropertyFont::updateEditor(LVGLObject *obj)
{
	m_widget->setCurrentIndex(lvgl.indexOfFont(get(obj)));
}

void LVGLPropertyFont::updateWidget(LVGLObject *obj)
{
	set(obj, lvgl.font(m_widget->currentIndex()));
}
