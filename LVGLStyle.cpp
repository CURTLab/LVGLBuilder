#include "LVGLStyle.h"

#include <QColor>
#include "LVGLCore.h"

using lv_font_ref = const lv_font_t *;

LVGLStyleItem::LVGLStyleItem(QString name, LVGL::StylePart stylePart, LVGLStyleItem *parent)
	: m_name(name)
	, m_parent(parent)
	, m_type(Property)
	, m_offset(std::numeric_limits<size_t>::max())
	, m_stylePart(stylePart)
{
}

LVGLStyleItem::LVGLStyleItem(QString name, Type type, size_t offset, LVGL::StylePart stylePart, LVGLStyleItem *parent)
	: m_name(name)
	, m_parent(parent)
	, m_type(type)
	, m_offset(offset)
	, m_stylePart(stylePart)
{
}

LVGLStyleItem::~LVGLStyleItem()
{
	qDeleteAll(m_childs);
}

QString LVGLStyleItem::name() const
{
	return m_name;
}

const LVGLStyleItem *LVGLStyleItem::parent() const
{
	return m_parent;
}

int LVGLStyleItem::count() const
{
	return m_childs.size();
}

const LVGLStyleItem *LVGLStyleItem::child(int index) const
{
	return  m_childs.at(index);
}

int LVGLStyleItem::row() const
{
	return m_parent?m_parent->indexOf(this):0;
}

int LVGLStyleItem::indexOf(const LVGLStyleItem *item) const
{
	return  m_childs.indexOf(const_cast<LVGLStyleItem*>(item));
}

void LVGLStyleItem::addChild(LVGLStyleItem *child)
{
	m_childs.append(child);
}

LVGLStyleItem::Type LVGLStyleItem::type() const
{
	return m_type;
}

size_t LVGLStyleItem::offset() const
{
	return m_offset;
}

LVGL::StylePart LVGLStyleItem::stylePart() const
{
	return m_stylePart;
}

LVGLStyle::LVGLStyle() : LVGLStyleItem("", LVGL::None), m_style(nullptr)
{
	LVGLStyleItem *body = new LVGLStyleItem("body", LVGL::Body, this);
	body->addChild(new LVGLStyleItem("main_color", Color, offsetof(lv_style_t, body.main_color), LVGL::Body, body));
	body->addChild(new LVGLStyleItem("grad_color", Color, offsetof(lv_style_t, body.grad_color), LVGL::Body, body));
	body->addChild(new LVGLStyleItem("radius", Coord, offsetof(lv_style_t, body.radius), LVGL::Body, body));
	body->addChild(new LVGLStyleItem("opa", Opacity, offsetof(lv_style_t, body.opa), LVGL::Body, body));

	LVGLStyleItem *border = new LVGLStyleItem("border", LVGL::BodyBorder, body);
	border->addChild(new LVGLStyleItem("color", Color, offsetof(lv_style_t, body.border.color), LVGL::BodyBorder, border));
	border->addChild(new LVGLStyleItem("width", Coord, offsetof(lv_style_t, body.border.width), LVGL::BodyBorder, border));
	border->addChild(new LVGLStyleItem("part", BorderPart, offsetof(lv_style_t, body.border.part), LVGL::BodyBorder, border));
	border->addChild(new LVGLStyleItem("opa", Opacity, offsetof(lv_style_t, body.border.opa), LVGL::BodyBorder, border));
	body->addChild(border);

	LVGLStyleItem *shadow = new LVGLStyleItem("shadow", LVGL::BodyShadow, body);
	shadow->addChild(new LVGLStyleItem("color", Color, offsetof(lv_style_t, body.shadow.color), LVGL::BodyShadow, shadow));
	shadow->addChild(new LVGLStyleItem("width", Coord, offsetof(lv_style_t, body.shadow.width), LVGL::BodyShadow, shadow));
	shadow->addChild(new LVGLStyleItem("type", ShadowType, offsetof(lv_style_t, body.shadow.type), LVGL::BodyShadow, shadow));
	body->addChild(shadow);

	LVGLStyleItem *padding = new LVGLStyleItem("padding", LVGL::BodyPadding, body);
	padding->addChild(new LVGLStyleItem("top", Coord, offsetof(lv_style_t, body.padding.top), LVGL::BodyPadding, padding));
	padding->addChild(new LVGLStyleItem("bottom", Coord, offsetof(lv_style_t, body.padding.bottom), LVGL::BodyPadding, padding));
	padding->addChild(new LVGLStyleItem("left", Coord, offsetof(lv_style_t, body.padding.left), LVGL::BodyPadding, padding));
	padding->addChild(new LVGLStyleItem("right", Coord, offsetof(lv_style_t, body.padding.right), LVGL::BodyPadding, padding));
	padding->addChild(new LVGLStyleItem("inner", Coord, offsetof(lv_style_t, body.padding.inner), LVGL::BodyPadding, padding));
	body->addChild(padding);

	m_childs << body;

	LVGLStyleItem *text = new LVGLStyleItem("text", LVGL::Text, this);
	text->addChild(new LVGLStyleItem("color", Color, offsetof(lv_style_t, text.color), LVGL::Text, text));
	text->addChild(new LVGLStyleItem("sel_color", Color, offsetof(lv_style_t, text.sel_color), LVGL::Text, text));
	text->addChild(new LVGLStyleItem("font", Font, offsetof(lv_style_t, text.font), LVGL::Text, text));
	text->addChild(new LVGLStyleItem("opa", Opacity, offsetof(lv_style_t, text.opa), LVGL::Text, text));
	text->addChild(new LVGLStyleItem("letter_space", Coord, offsetof(lv_style_t, text.letter_space), LVGL::Text, text));
	text->addChild(new LVGLStyleItem("line_space", Coord, offsetof(lv_style_t, text.line_space), LVGL::Text, text));
	m_childs << text;

	LVGLStyleItem *image = new LVGLStyleItem("image", LVGL::Image, this);
	image->addChild(new LVGLStyleItem("color", Color, offsetof(lv_style_t, image.color), LVGL::Image, image));
	image->addChild(new LVGLStyleItem("intense", Opacity, offsetof(lv_style_t, image.intense), LVGL::Image, image));
	image->addChild(new LVGLStyleItem("opa", Opacity, offsetof(lv_style_t, image.opa), LVGL::Image, image));
	m_childs << image;

	LVGLStyleItem *line = new LVGLStyleItem("line", LVGL::Line, this);
	line->addChild(new LVGLStyleItem("color", Color, offsetof(lv_style_t, line.color), LVGL::Line, line));
	line->addChild(new LVGLStyleItem("width", Coord, offsetof(lv_style_t, line.width), LVGL::Line, line));
	line->addChild(new LVGLStyleItem("opa", Opacity, offsetof(lv_style_t, line.opa), LVGL::Line, line));
	m_childs << line;
}

QVariant LVGLStyle::get(const LVGLStyleItem *item) const
{
	if ((m_style == nullptr) || (item->offset() == std::numeric_limits<size_t>::max()) ||
		 (item->type() == Property))
		return QVariant();
	if (item->type() == Coord) {
		lv_coord_t c = *reinterpret_cast<lv_coord_t*>(reinterpret_cast<uint8_t*>(m_style)+item->offset());
		return QVariant(c);
	} else if (item->type() == Opacity) {
		lv_coord_t c = *reinterpret_cast<lv_opa_t*>(reinterpret_cast<uint8_t*>(m_style)+item->offset());
		return QVariant(c);
	} else if (item->type() == Color) {
		lv_color_t c = *reinterpret_cast<lv_color_t*>(reinterpret_cast<uint8_t*>(m_style)+item->offset());
		return lvgl.toColor(c);
	} else if (item->type() == BorderPart) {
		lv_border_part_t c = *reinterpret_cast<lv_border_part_t*>(reinterpret_cast<uint8_t*>(m_style)+item->offset());
		QStringList props;
		if (c == 0) return "None";
		else if (c == 0x0F) return "Full";
		else if (c == 0x10) return "Internal";

		if (c & 0x01) props << "Bottom";
		if (c & 0x02) props << "Top";
		if (c & 0x04) props << "Left";
		if (c & 0x08) props << "Right";
		return props.join(" | ");
	} else if (item->type() == ShadowType) {
		lv_shadow_type_t c = *reinterpret_cast<lv_shadow_type_t*>(reinterpret_cast<uint8_t*>(m_style)+item->offset());
		if (c == 0) return "Bottom";
		else if (c == 1) return "Full";
	} else if (item->type() == Font) {
		const lv_font_t *c = *reinterpret_cast<lv_font_t**>(reinterpret_cast<uint8_t*>(m_style)+item->offset());
		const int index = lvgl.indexOfFont(c);
		return lvgl.fontNames().at(index);
	}
	return QVariant();
}

lv_border_part_t LVGLStyle::getBorderPart(const LVGLStyleItem *item) const
{
	if (item->type() == BorderPart)
		return *reinterpret_cast<lv_border_part_t*>(reinterpret_cast<uint8_t*>(m_style)+item->offset());
	return 0;
}

template<class T, class P>
void set_helper(P value, size_t offset, lv_style_t *style)
{
	T &c = *reinterpret_cast<T*>(reinterpret_cast<uint8_t*>(style)+offset);
	c = static_cast<T>(value);
}

void LVGLStyle::set(const LVGLStyleItem *item, QVariant value)
{
	if ((m_style == nullptr) || (item->offset() == std::numeric_limits<size_t>::max()) ||
		 (item->type() == Property))
		return;

	if (item->type() == Coord)
		set_helper<lv_coord_t>(value.toInt(), item->offset(), m_style);
	else if (item->type() == Opacity)
		set_helper<lv_opa_t>(value.toInt(), item->offset(), m_style);
	else if (item->type() == Color)
		set_helper<lv_color_t>(lvgl.fromColor(value), item->offset(), m_style);
	else if (item->type() == Font)
		set_helper<lv_font_ref>(lvgl.font(value.toString()), item->offset(), m_style);
	else if (item->type() == BorderPart)
		set_helper<lv_border_part_t>(value.toInt(), item->offset(), m_style);
}

lv_style_t *LVGLStyle::style() const
{
	return m_style;
}

void LVGLStyle::setStyle(lv_style_t *style)
{
	m_style = style;
}

bool LVGLStyle::hasStyleChanged(const lv_style_t *style, const lv_style_t *base, LVGL::StyleParts parts)
{
	if (parts & LVGL::Body) {
		if (style->body.main_color.full != base->body.main_color.full) return true;
		else if (style->body.grad_color.full != base->body.grad_color.full) return true;
		else if (style->body.radius != base->body.radius) return true;
		else if (style->body.opa != base->body.opa) return true;
		if (parts & LVGL::BodyBorder) {
			if (style->body.border.color.full != base->body.border.color.full) return true;
			else if (style->body.border.width != base->body.border.width) return true;
			else if (style->body.border.part != base->body.border.part) return true;
			else if (style->body.border.opa != base->body.border.opa) return true;
		}
		if (parts & LVGL::BodyShadow) {
			if (style->body.shadow.color.full != base->body.shadow.color.full) return true;
			else if (style->body.shadow.width != base->body.shadow.width) return true;
			else if (style->body.shadow.type != base->body.shadow.type) return true;
		}
		if (parts & LVGL::BodyPadding) {
			if (style->body.padding.top != base->body.padding.top) return true;
			else if (style->body.padding.bottom != base->body.padding.bottom) return true;
			else if (style->body.padding.left != base->body.padding.left) return true;
			else if (style->body.padding.right != base->body.padding.right) return true;
			else if (style->body.padding.inner != base->body.padding.inner) return true;
		}
	}
	if (parts & LVGL::Text) {
		if (style->text.color.full != base->text.color.full) return true;
		else if (style->text.sel_color.full != base->text.sel_color.full) return true;
		else if (style->text.font != base->text.font) return true;
		else if (style->text.letter_space != base->text.letter_space) return true;
		else if (style->text.line_space != base->text.line_space) return true;
		else if (style->text.opa != base->text.opa) return true;
	}
	if (parts & LVGL::Image) {
		if (style->image.color.full != base->image.color.full) return true;
		else if (style->image.intense != base->image.intense) return true;
		else if (style->image.opa != base->image.opa) return true;
	}
	if (parts & LVGL::Line) {
		if (style->line.color.full != base->line.color.full) return true;
		else if (style->line.width != base->line.width) return true;
		else if (style->line.opa != base->line.opa) return true;
	}
	return false;
}
