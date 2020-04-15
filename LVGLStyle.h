#ifndef LVGLSTYLE_H
#define LVGLSTYLE_H

#include <QString>
#include <QList>
#include <QVariant>

#include <lvgl/lvgl.h>

namespace LVGL {

enum StylePart {
	None = 0x00,
	BodyBorder = 0x01,
	BodyShadow = 0x02,
	BodyPadding = 0x04,
	Body = BodyBorder | BodyShadow | BodyPadding,
	Text = 0x08,
	Image = 0x10,
	Line = 0x20,
	All = Body | Text | Image | Line
};
Q_DECLARE_FLAGS(StyleParts, StylePart)

}
Q_DECLARE_OPERATORS_FOR_FLAGS(LVGL::StyleParts)

class LVGLStyleItem {
public:
	enum Type {
		Color,
		Coord,
		Opacity,
		BorderPart,
		ShadowType,
		Font,
		Property
	};

	LVGLStyleItem(QString name, LVGL::StylePart stylePart, LVGLStyleItem *parent = nullptr);
	LVGLStyleItem(QString name, Type type, size_t offset, LVGL::StylePart stylePart, LVGLStyleItem *parent = nullptr);
	~LVGLStyleItem();

	QString name() const;

	const LVGLStyleItem *parent() const;
	int count() const;
	const LVGLStyleItem *child(int index) const;
	int row() const;
	int indexOf(const LVGLStyleItem *item) const;
	void addChild(LVGLStyleItem *child);

	Type type() const;

	size_t offset() const;

	LVGL::StylePart stylePart() const;

protected:
	QString m_name;
	LVGLStyleItem *m_parent;
	Type m_type;
	size_t m_offset;
	QList<LVGLStyleItem*> m_childs;
	LVGL::StylePart m_stylePart;

};

class LVGLStyle : public LVGLStyleItem
{
public:
	LVGLStyle();

	QVariant get(const LVGLStyleItem *item) const;
	lv_border_part_t getBorderPart(const LVGLStyleItem *item) const;
	void set(const LVGLStyleItem *item, QVariant value);

	lv_style_t *style() const;
	void setStyle(lv_style_t *style);

	static bool hasStyleChanged(const lv_style_t *style, const lv_style_t *base, LVGL::StyleParts parts = LVGL::All);

private:
	lv_style_t *m_style;

};

#endif // LVGLSTYLE_H
