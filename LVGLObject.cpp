#include "LVGLObject.h"

#include "LVGL.h"

#include <QJsonArray>

LVGLObject::LVGLObject(const LVGLWidget *widgetClass, QString name, LVGLObject *parent)
	: m_obj(widgetClass->newObject(parent->obj())), m_widgetClass(widgetClass)
	, m_name(name), m_locked(false), m_accessible(false), m_movable(true), m_index(-1), m_parent(parent)
{
	if (parent)
		parent->m_childs.append(this);
	if (m_name.isEmpty())
		generateName();
}

LVGLObject::LVGLObject(const LVGLWidget *widgetClass, QString name, lv_obj_t *parent)
	: m_obj(widgetClass->newObject(parent)), m_widgetClass(widgetClass)
	, m_name(name), m_locked(false), m_accessible(false), m_movable(true), m_index(-1), m_parent(nullptr)
{
	if (m_name.isEmpty())
		generateName();
}

LVGLObject::LVGLObject(const LVGLWidget *widgetClass, LVGLObject *parent, lv_obj_t *parentObj)
	: m_obj(widgetClass->newObject(parentObj)), m_widgetClass(widgetClass)
	, m_locked(false), m_accessible(false), m_movable(true), m_index(-1), m_parent(parent)
{
	generateName();
}

LVGLObject::LVGLObject(lv_obj_t *obj, const LVGLWidget *widgetClass, LVGLObject *parent, bool movable, int index)
	: m_obj(obj), m_widgetClass(widgetClass), m_locked(false), m_accessible(false)
	, m_movable(movable), m_index(index), m_parent(parent)
{
	assert(m_widgetClass);
	if (parent)
		parent->m_childs.append(this);
	generateName();
}

LVGLObject::~LVGLObject()
{
	lv_obj_del(m_obj);
}

lv_obj_t *LVGLObject::obj() const
{
	return m_obj;
}

QString LVGLObject::name() const
{
	return m_name;
}

void LVGLObject::setName(const QString &name)
{
	if (doesNameExists(name, this))
		generateName();
	else
		m_name = name;
}

QString LVGLObject::codeName() const
{
	return name().toLower().replace(" ", "_");
}

bool LVGLObject::isLocked() const
{
	return m_locked;
}

void LVGLObject::setLocked(bool locked)
{
	m_locked = locked;
}

bool LVGLObject::isMovable() const
{
	return m_movable;
}

const LVGLWidget *LVGLObject::widgetClass() const
{
	return m_widgetClass;
}

LVGLWidget::Type LVGLObject::widgetType() const
{
	return m_widgetClass->type();
}

QPoint LVGLObject::absolutePosition() const
{
	//if (lv_obj_get_parent(m_obj) == lv_scr_act())
	//	return QPoint(m_obj->coords.x1, m_obj->coords.x2);
	int x = lv_obj_get_x(m_obj);
	int y = lv_obj_get_y(m_obj);
	lv_obj_t *parent = lv_obj_get_parent(m_obj);
	while (parent) {
		if (parent == lv_scr_act()) break;
		x += lv_obj_get_x(parent);
		y += lv_obj_get_y(parent);
		parent = lv_obj_get_parent(parent);
	}
	return QPoint(x, y);
}

QPoint LVGLObject::position() const
{
	return QPoint(lv_obj_get_x(m_obj), lv_obj_get_y(m_obj));
}

int LVGLObject::x() const
{
	return lv_obj_get_x(m_obj);
}

int LVGLObject::y() const
{
	return lv_obj_get_y(m_obj);
}

void LVGLObject::setPosition(const QPoint &pos)
{
	if (pos != position()) {
		lv_obj_set_pos(m_obj, static_cast<lv_coord_t>(pos.x()),
							static_cast<lv_coord_t>(pos.y()));
		emit positionChanged();
	}
}

void LVGLObject::setX(int x)
{
	if (x != lv_obj_get_x(m_obj)) {
		lv_obj_set_x(m_obj, static_cast<lv_coord_t>(x));
		emit positionChanged();
	}
}

void LVGLObject::setY(int y)
{
	if (y != lv_obj_get_y(m_obj)) {
		lv_obj_set_y(m_obj, static_cast<lv_coord_t>(y));
		emit positionChanged();
	}
}

QSize LVGLObject::size() const
{
	return QSize(lv_obj_get_width(m_obj), lv_obj_get_height(m_obj));
}

QRect LVGLObject::geometry() const
{
	return QRect(absolutePosition(), size());
}

void LVGLObject::setGeometry(QRect geometry)
{
	setPosition(geometry.topLeft());
	lv_obj_set_size(m_obj,
						 static_cast<lv_coord_t>(geometry.width()),
						 static_cast<lv_coord_t>(geometry.height())
						 );
}

int LVGLObject::width() const
{
	return lv_obj_get_width(m_obj);
}

int LVGLObject::height() const
{
	return lv_obj_get_height(m_obj);
}

void LVGLObject::setWidth(int width)
{
	lv_obj_set_width(m_obj, static_cast<lv_coord_t>(width));
}

void LVGLObject::setHeight(int height)
{
	lv_obj_set_height(m_obj, static_cast<lv_coord_t>(height));
}

QJsonObject LVGLObject::toJson()
{
	QJsonObject object({{"widget", m_widgetClass->name()},
							  {"class", m_widgetClass->className()},
							  {"name", m_name}
							 });
	if (m_index > -1)
		object.insert("index", m_index);
	for (LVGLProperty *p:m_widgetClass->properties()) {
		QJsonValue v = p->toJson(this);
		if (!v.isNull())
			object.insert(p->name().toLower(), v);
	}

	QJsonArray styles = jsonStyles();
	if (!m_styles.isEmpty() && (styles.size() > 0))
		object.insert("styles", styles);
	if (!m_childs.isEmpty()) {
		QJsonArray childs;
		for (LVGLObject *c:m_childs)
			childs.append(c->toJson());
		object.insert("children", childs);
	}
	return object;
}

QJsonArray LVGLObject::jsonStyles() const
{
	QJsonArray styles;
	for (auto it = m_styles.begin(); it != m_styles.end(); ++it) {
		const lv_style_t &objStyle = it.value();
		const lv_style_t *defaultStyle = m_widgetClass->defaultStyle(it.key());

		const LVGL::StyleParts editableParts = m_widgetClass->editableStyles(it.key());
		if (defaultStyle && !LVGLStyle::hasStyleChanged(&objStyle, defaultStyle, editableParts))
			continue;

		QJsonObject style;
		style.insert("type", m_widgetClass->styles().at(it.key()));

		QString baseStyle = lvgl.baseStyleName(defaultStyle);
		if (!baseStyle.isEmpty())
			style.insert("base", baseStyle);

		if (editableParts & LVGL::Body) {
			QJsonObject body;
			if (!defaultStyle || (defaultStyle->body.main_color.full != objStyle.body.main_color.full))
				body.insert("main_color", QVariant(lvgl.toColor(objStyle.body.main_color)).toString());
			if (!defaultStyle || (defaultStyle->body.grad_color.full != objStyle.body.grad_color.full))
				body.insert("grad_color", QVariant(lvgl.toColor(objStyle.body.grad_color)).toString());
			if (!defaultStyle || (defaultStyle->body.radius != objStyle.body.radius))
				body.insert("radius", static_cast<int>(objStyle.body.radius));
			if (!defaultStyle || (defaultStyle->body.opa != objStyle.body.opa))
				body.insert("opa", objStyle.body.opa);

			if (editableParts & LVGL::BodyBorder) {
				QJsonObject border;
				if (!defaultStyle || (defaultStyle->body.border.color.full != objStyle.body.border.color.full))
					border.insert("color", QVariant(lvgl.toColor(objStyle.body.border.color)).toString());
				if (!defaultStyle || (defaultStyle->body.border.width != objStyle.body.border.width))
					border.insert("width", objStyle.body.border.width);
				if (!defaultStyle || (defaultStyle->body.border.part != objStyle.body.border.part))
					border.insert("part", objStyle.body.border.part);
				if (!defaultStyle || (defaultStyle->body.border.opa != objStyle.body.border.opa))
					border.insert("opa", objStyle.body.border.opa);
				if (!border.isEmpty())
					body.insert("border", border);
			}

			if (editableParts & LVGL::BodyShadow) {
				QJsonObject shadow;
				if (!defaultStyle || (defaultStyle->body.shadow.color.full != objStyle.body.shadow.color.full))
					shadow.insert("color", QVariant(lvgl.toColor(objStyle.body.shadow.color)).toString());
				if (!defaultStyle || (defaultStyle->body.shadow.width != objStyle.body.shadow.width))
					shadow.insert("width", objStyle.body.border.width);
				if (!defaultStyle || (defaultStyle->body.shadow.type != objStyle.body.shadow.type))
					shadow.insert("type", objStyle.body.shadow.type);
				if (!shadow.isEmpty())
					body.insert("shadow", shadow);
			}

			if (editableParts & LVGL::BodyPadding) {
				QJsonObject padding;
				if (!defaultStyle || (defaultStyle->body.padding.top != objStyle.body.padding.top))
					padding.insert("top", objStyle.body.padding.top);
				if (!defaultStyle || (defaultStyle->body.padding.bottom != objStyle.body.padding.bottom))
					padding.insert("bottom", objStyle.body.padding.bottom);
				if (!defaultStyle || (defaultStyle->body.padding.left != objStyle.body.padding.left))
					padding.insert("left", objStyle.body.padding.left);
				if (!defaultStyle || (defaultStyle->body.padding.right != objStyle.body.padding.right))
					padding.insert("right", objStyle.body.padding.right);
				if (!defaultStyle || (defaultStyle->body.padding.inner != objStyle.body.padding.inner))
					padding.insert("inner", objStyle.body.padding.inner);
				if (!padding.isEmpty())
					body.insert("padding", padding);
			}
			if (!body.isEmpty())
				style.insert("body", body);
		}

		if (editableParts & LVGL::Text) {
			QJsonObject text;
			if (!defaultStyle || (defaultStyle->text.color.full != objStyle.text.color.full))
				text.insert("color", QVariant(lvgl.toColor(objStyle.text.color)).toString());
			if (!defaultStyle || (defaultStyle->text.sel_color.full != objStyle.text.sel_color.full))
				text.insert("sel_color", QVariant(lvgl.toColor(objStyle.text.sel_color)).toString());
			if (!defaultStyle || (defaultStyle->text.font != objStyle.text.font))
				text.insert("font", lvgl.fontNames().at(lvgl.indexOfFont(objStyle.text.font)));
			if (!defaultStyle || (defaultStyle->text.letter_space != objStyle.text.letter_space))
				text.insert("letter_space", objStyle.text.letter_space);
			if (!defaultStyle || (defaultStyle->text.line_space != objStyle.text.line_space))
				text.insert("line_space", objStyle.text.line_space);
			if (!defaultStyle || (defaultStyle->text.opa != objStyle.text.opa))
				text.insert("opa", objStyle.text.opa);
			if (!text.isEmpty())
				style.insert("text", text);
		}

		if (editableParts & LVGL::Image) {
			QJsonObject image;
			if (!defaultStyle || (defaultStyle->image.color.full != objStyle.image.color.full))
				image.insert("color", QVariant(lvgl.toColor(objStyle.image.color)).toString());
			if (!defaultStyle || (defaultStyle->image.intense != objStyle.image.intense))
				image.insert("intense", objStyle.image.intense);
			if (!defaultStyle || (defaultStyle->image.opa != objStyle.image.opa))
				image.insert("opa", objStyle.image.opa);
			if (!image.isEmpty())
				style.insert("image", image);
		}

		if (editableParts & LVGL::Line) {
			QJsonObject line;
			if (!defaultStyle || (defaultStyle->line.color.full != objStyle.line.color.full))
				line.insert("color", QVariant(lvgl.toColor(objStyle.line.color)).toString());
			if (!defaultStyle || (defaultStyle->line.width != objStyle.line.width))
				line.insert("width", objStyle.line.width);
			if (!defaultStyle || (defaultStyle->line.opa != objStyle.line.opa))
				line.insert("opa", objStyle.image.opa);
			if (!line.isEmpty())
				style.insert("line", line);
		}

		if (style.size() > 0)
			styles.append(style);
	}
	return styles;
}

QStringList LVGLObject::codeStyle(QString styleVar, int type) const
{
	QStringList ret;
	if (!hasCustomStyle(type))
		return ret;

	const lv_style_t &objStyle = m_styles[type];
	const lv_style_t *defaultStyle = m_widgetClass->defaultStyle(type);

	const LVGL::StyleParts editableParts = m_widgetClass->editableStyles(type);
	if (defaultStyle && !LVGLStyle::hasStyleChanged(&objStyle, defaultStyle, editableParts))
		return ret;

	if (editableParts & LVGL::Body) {
		if (!defaultStyle || (defaultStyle->body.main_color.full != objStyle.body.main_color.full)) {
			QString color = QVariant(lvgl.toColor(objStyle.body.main_color)).toString().replace("#", "0x");
			ret << styleVar + ".body.main_color = lv_color_hex(" + color + ");";
		}
		if (!defaultStyle || (defaultStyle->body.grad_color.full != objStyle.body.grad_color.full)) {
			QString color = QVariant(lvgl.toColor(objStyle.body.grad_color)).toString().replace("#", "0x");
			ret << styleVar + ".body.grad_color = lv_color_hex(" + color + ");";
		}
		if (!defaultStyle || (defaultStyle->body.radius != objStyle.body.radius))
			ret << styleVar + ".body.radius = " + QString::number(objStyle.body.radius) + ";";
		if (!defaultStyle || (defaultStyle->body.opa != objStyle.body.opa))
			ret << styleVar + ".body.opa = " + QString::number(objStyle.body.opa) + ";";

		if (editableParts & LVGL::BodyBorder) {
			if (!defaultStyle || (defaultStyle->body.border.color.full != objStyle.body.border.color.full)) {
				QString color = QVariant(lvgl.toColor(objStyle.body.border.color)).toString().replace("#", "0x");
				ret << styleVar + ".body.border.color = lv_color_hex(" + color + ");";
			}
			if (!defaultStyle || (defaultStyle->body.border.width != objStyle.body.border.width))
				ret << styleVar + ".body.border.width = " + QString::number(objStyle.body.border.width) + ";";
			if (!defaultStyle || (defaultStyle->body.border.part != objStyle.body.border.part))
				ret << styleVar + ".body.border.part = " + QString::number(objStyle.body.border.part) + ";";
			if (!defaultStyle || (defaultStyle->body.border.opa != objStyle.body.border.opa))
				ret << styleVar + ".body.border.opa = " + QString::number(objStyle.body.border.opa) + ";";
		}

		if (editableParts & LVGL::BodyShadow) {
			QJsonObject shadow;
			if (!defaultStyle || (defaultStyle->body.shadow.color.full != objStyle.body.shadow.color.full)) {
				QString color = QVariant(lvgl.toColor(objStyle.body.shadow.color)).toString().replace("#", "0x");
				ret << styleVar + ".body.shadow.color = lv_color_hex(" + color + ");";
			}
			if (!defaultStyle || (defaultStyle->body.shadow.width != objStyle.body.shadow.width))
				ret << styleVar + ".body.shadow.width = " + QString::number(objStyle.body.shadow.width) + ";";
			if (!defaultStyle || (defaultStyle->body.shadow.type != objStyle.body.shadow.type))
				ret << styleVar + ".body.shadow.type = " + QString::number(objStyle.body.shadow.type) + ";";
		}

		if (editableParts & LVGL::BodyPadding) {
			QJsonObject padding;
			if (!defaultStyle || (defaultStyle->body.padding.top != objStyle.body.padding.top))
				ret << styleVar + ".body.padding.top = " + QString::number(objStyle.body.padding.top) + ";";
			if (!defaultStyle || (defaultStyle->body.padding.bottom != objStyle.body.padding.bottom))
				ret << styleVar + ".body.padding.bottom = " + QString::number(objStyle.body.padding.bottom) + ";";
			if (!defaultStyle || (defaultStyle->body.padding.left != objStyle.body.padding.left))
				ret << styleVar + ".body.padding.left = " + QString::number(objStyle.body.padding.left) + ";";
			if (!defaultStyle || (defaultStyle->body.padding.right != objStyle.body.padding.right))
				ret << styleVar + ".body.padding.right = " + QString::number(objStyle.body.padding.right) + ";";
			if (!defaultStyle || (defaultStyle->body.padding.inner != objStyle.body.padding.inner))
				ret << styleVar + ".body.padding.inner = " + QString::number(objStyle.body.padding.inner) + ";";
		}
	}

	if (editableParts & LVGL::Text) {
		if (!defaultStyle || (defaultStyle->text.color.full != objStyle.text.color.full)) {
			QString color = QVariant(lvgl.toColor(objStyle.text.color)).toString().replace("#", "0x");
			ret << styleVar + ".text.color = lv_color_hex(" + color + ");";
		}
		if (!defaultStyle || (defaultStyle->text.sel_color.full != objStyle.text.sel_color.full)) {
			QString color = QVariant(lvgl.toColor(objStyle.text.sel_color)).toString().replace("#", "0x");
			ret << styleVar + ".text.sel_color = lv_color_hex(" + color + ");";
		}
		if (!defaultStyle || (defaultStyle->text.font != objStyle.text.font))
			ret << styleVar + ".text.font = &" + lvgl.fontCodeNames().at(lvgl.indexOfFont(objStyle.text.font)) + ";";
		if (!defaultStyle || (defaultStyle->text.letter_space != objStyle.text.letter_space))
			ret << styleVar + ".text.letter_space = " + QString::number(objStyle.text.letter_space) + ";";
		if (!defaultStyle || (defaultStyle->text.line_space != objStyle.text.letter_space))
			ret << styleVar + ".text.line_space = " + QString::number(objStyle.text.line_space) + ";";
		if (!defaultStyle || (defaultStyle->text.opa != objStyle.text.opa))
			ret << styleVar + ".text.opa = " + QString::number(objStyle.text.opa) + ";";
	}

	if (editableParts & LVGL::Image) {
		if (!defaultStyle || (defaultStyle->image.color.full != objStyle.image.color.full)) {
			QString color = QVariant(lvgl.toColor(objStyle.image.color)).toString().replace("#", "0x");
			ret << styleVar + ".image.color = lv_color_hex(" + color + ");";
		}
		if (!defaultStyle || (defaultStyle->image.intense != objStyle.image.intense))
			ret << styleVar + ".image.intense = " + QString::number(objStyle.image.intense) + ";";
		if (!defaultStyle || (defaultStyle->image.opa != objStyle.image.opa))
			ret << styleVar + ".image.opa = " + QString::number(objStyle.image.opa) + ";";
	}

	if (editableParts & LVGL::Line) {
		if (!defaultStyle || (defaultStyle->line.color.full != objStyle.line.color.full)) {
			QString color = QVariant(lvgl.toColor(objStyle.line.color)).toString().replace("#", "0x");
			ret << styleVar + ".line.color = lv_color_hex(" + color + ");";
		}
		if (!defaultStyle || (defaultStyle->line.width != objStyle.line.width))
			ret << styleVar + ".line.width = " + QString::number(objStyle.line.width) + ";";
		if (!defaultStyle || (defaultStyle->line.opa != objStyle.line.opa))
			ret << styleVar + ".line.opa = " + QString::number(objStyle.line.opa) + ";";
	}

	return ret;
}

void LVGLObject::parseStyles(const QJsonArray &styles)
{
	for (int i = 0; i < styles.size(); ++i) {
		QJsonObject style = styles[i].toObject();
		int type = m_widgetClass->styles().indexOf(style["type"].toString());
		assert(type >= 0);

		lv_style_t *objStyle = this->style(type);
		if (style.contains("body")) {
			QJsonObject body = style["body"].toObject();
			if (body.contains("main_color")) {
				const QColor c = QVariant(body["main_color"].toString()).value<QColor>();
				objStyle->body.main_color = lvgl.fromColor(c);
			}
			if (body.contains("grad_color")) {
				const QColor c = QVariant(body["grad_color"].toString()).value<QColor>();
				objStyle->body.grad_color = lvgl.fromColor(c);
			}
			if (body.contains("radius"))
				objStyle->body.radius = static_cast<lv_coord_t>(body["radius"].toInt());
			if (body.contains("opa"))
				objStyle->body.opa = static_cast<lv_opa_t>(body["opa"].toInt());

			if (body.contains("border")) {
				QJsonObject border = body["border"].toObject();
				if (border.contains("color")) {
					const QColor c = QVariant(border["color"].toString()).value<QColor>();
					objStyle->body.border.color = lvgl.fromColor(c);
				}
				if (border.contains("width"))
					objStyle->body.border.width = static_cast<lv_coord_t>(border["width"].toInt());
				if (border.contains("part"))
					objStyle->body.border.part = static_cast<lv_border_part_t>(border["part"].toInt());
				if (border.contains("opa"))
					objStyle->body.border.opa = static_cast<lv_opa_t>(border["opa"].toInt());
			}

			if (body.contains("shadow")) {
				QJsonObject shadow = body["shadow"].toObject();
				if (shadow.contains("color")) {
					const QColor c = QVariant(shadow["color"].toString()).value<QColor>();
					objStyle->body.shadow.color = lvgl.fromColor(c);
				}
				if (shadow.contains("width"))
					objStyle->body.shadow.width = static_cast<lv_coord_t>(shadow["width"].toInt());
				if (shadow.contains("type"))
					objStyle->body.shadow.type = static_cast<lv_shadow_type_t>(shadow["type"].toInt());
			}

			if (body.contains("padding")) {
				QJsonObject padding = body["padding"].toObject();
				if (padding.contains("top"))
					objStyle->body.padding.top = static_cast<lv_coord_t>(padding["top"].toInt());
				if (padding.contains("bottom"))
					objStyle->body.padding.bottom = static_cast<lv_coord_t>(padding["bottom"].toInt());
				if (padding.contains("left"))
					objStyle->body.padding.left = static_cast<lv_coord_t>(padding["left"].toInt());
				if (padding.contains("right"))
					objStyle->body.padding.right = static_cast<lv_coord_t>(padding["right"].toInt());
				if (padding.contains("inner"))
					objStyle->body.padding.inner = static_cast<lv_coord_t>(padding["inner"].toInt());
			}
		}
		if (style.contains("text")) {
			QJsonObject text = style["text"].toObject();
			if (text.contains("color")) {
				const QColor c = QVariant(text["color"].toString()).value<QColor>();
				objStyle->text.color = lvgl.fromColor(c);
			}
			if (text.contains("sel_color")) {
				const QColor c = QVariant(text["sel_color"].toString()).value<QColor>();
				objStyle->text.sel_color = lvgl.fromColor(c);
			}
			if (text.contains("font")) {
				const lv_font_t *f = lvgl.font(lvgl.fontNames().indexOf(text["font"].toString()));
				objStyle->text.font = f;
			}
			if (text.contains("line_space"))
				objStyle->text.line_space = static_cast<lv_coord_t>(text["line_space"].toInt());
			if (text.contains("letter_space"))
				objStyle->text.letter_space = static_cast<lv_coord_t>(text["letter_space"].toInt());
			if (text.contains("opa"))
				objStyle->text.opa = static_cast<lv_opa_t>(text["opa"].toInt());
		}

		if (style.contains("image")) {
			QJsonObject image = style["image"].toObject();
			if (image.contains("color")) {
				const QColor c = QVariant(image["color"].toString()).value<QColor>();
				objStyle->image.color = lvgl.fromColor(c);
			}
			if (image.contains("intense"))
				objStyle->image.intense = static_cast<lv_opa_t>(image["intense"].toInt());
			if (image.contains("opa"))
				objStyle->image.opa = static_cast<lv_opa_t>(image["opa"].toInt());
		}

		if (style.contains("line")) {
			QJsonObject line = style["line"].toObject();
			if (line.contains("color")) {
				const QColor c = QVariant(line["color"].toString()).value<QColor>();
				objStyle->line.color = lvgl.fromColor(c);
			}
			if (line.contains("width"))
				objStyle->line.width = static_cast<lv_opa_t>(line["width"].toInt());
			if (line.contains("opa"))
				objStyle->line.opa = static_cast<lv_opa_t>(line["opa"].toInt());
		}
		m_widgetClass->setStyle(m_obj, type, objStyle);
	}
}

LVGLObject *LVGLObject::parent() const
{
	return m_parent;
}

bool LVGLObject::hasCustomStyle(int type) const
{
	return m_styles.contains(type);
}

lv_style_t *LVGLObject::style(int type)
{
	if (m_styles.contains(type))
		return &m_styles[type];

	lv_style_t *s = &m_styles[type];

	// first check default fonts
	lv_style_t *doner = m_widgetClass->defaultStyle(type);
	if (doner == nullptr) doner = m_widgetClass->style(m_obj, type);
	if (doner == nullptr) doner = &lv_style_plain;
	//lv_style_t *doner = m_widgetType->style(m_obj, type);
	// security check
	assert(doner != nullptr);


	lv_style_copy(s, doner);
	return s;
}

QString LVGLObject::styleCodeName(int type) const
{
	return "style" + QString::number(type) + "_" + name().toLower().replace(' ', '_');
}

QList<LVGLObject *> LVGLObject::childs() const
{
	return m_childs;
}

void LVGLObject::removeChild(LVGLObject *child)
{
	m_childs.removeOne(child);
}

bool LVGLObject::isAccessible() const
{
	return m_accessible;
}

void LVGLObject::setAccessible(bool accessible)
{
	m_accessible = accessible;
}

bool LVGLObject::doesNameExists(const QString &name, LVGLObject *except)
{
	for (LVGLObject *o:lvgl.allObjects()) {
		if (except && (except == o))
			continue;
		if (o->name() == name)
			return true;
	}
	return false;
}

void LVGLObject::generateName()
{
	// create sorted list by type
	QList<int> numbers;
	const QString prefix = m_widgetClass->name().toLower().replace(' ', '_');

	for (LVGLObject *o:lvgl.allObjects()) {
		if (o->widgetType() == m_widgetClass->type() &&
			 o->name().startsWith(prefix))
		{
			const int index = o->name().lastIndexOf('_');
			const int num = o->name().mid(index+1).toInt();
			if (num > 0) {
				auto ind = std::lower_bound(numbers.begin(), numbers.end(), num);
				numbers.insert(ind, num);
			}
		}
	}

	if (m_name.startsWith(prefix)) {
		const int index = m_name.lastIndexOf('_');
		const int num = m_name.mid(index+1).toInt();
		if (!numbers.contains(num))
			return; // no new name needed
	}

	// find next free id
	int id = 1;
	for (int num:numbers) {
		if (num == id)
			id++;
		else
			break;
	}
	m_name = QString("%1_%2").arg(prefix).arg(id);
	return;
}

LVGLObject *LVGLObject::parse(QJsonObject object, LVGLObject *parent)
{
	QString className = object["class"].toString();
	const LVGLWidget *widgetClass = lvgl.widget(className);
	if (widgetClass) {
		LVGLObject *newObj;
		if (parent)
			newObj = new LVGLObject(widgetClass, object["name"].toString(), parent);
		else
			newObj = new LVGLObject(widgetClass, object["name"].toString(), lv_scr_act());
		// generate name if nessessary
		if (newObj->name().isEmpty())
			newObj->generateName();
		lvgl.addObject(newObj);

		for (LVGLProperty *p:widgetClass->properties()) {
			QString pname = p->name().toLower();
			if (pname == "name") continue;
			if (object.contains(pname)) {
				QVariant v = object[pname].toVariant();
				if (!v.isNull())
					p->setValue(newObj, v);
			}
		}
		if (object.contains("styles"))
			newObj->parseStyles(object["styles"].toArray());

		if (object.contains("children")) {
			QJsonArray childs = object["children"].toArray();
			for (int i = 0; i < childs.size(); ++i) {
				QJsonObject child = childs[i].toObject();
				if (widgetClass->type() == LVGLWidget::Tabview && child.contains("index")) {
					LVGLObject *page = newObj->findChildByIndex(child["index"].toInt());
					page->setName(child["name"].toString());
					// parse page props
					for (LVGLProperty *p:lvgl.widget("lv_page")->specialProperties()) {
						const QString pname = p->name().toLower();
						if (child.contains(pname)) {
							QVariant v = child[pname].toVariant();
							if (!v.isNull()) p->setValue(page, v);
						}
					}
					// parse styles
					if (child.contains("styles"))
						page->parseStyles(child["styles"].toArray());
					// parse children of page
					if (child.contains("children")) {
						QJsonArray page_childs = child["children"].toArray();
						for (int j = 0; j < page_childs.size(); ++j)
							parse(page_childs[j].toObject(), page);
					}
				} else {
					parse(child, newObj);
				}
			}
		}
		return newObj;
	}
	return nullptr;
}

int LVGLObject::index() const
{
	return m_index;
}

LVGLObject *LVGLObject::findChildByIndex(int index) const
{
	for (LVGLObject *child:m_childs) {
		if (child->index() == index)
			return child;
	}
	return nullptr;
}
