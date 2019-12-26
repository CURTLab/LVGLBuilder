#ifndef LVGLFONT_H
#define LVGLFONT_H

#include <lvgl/lvgl.h>
#include <QString>

class LVGLFont
{
public:
	LVGLFont(const QString &name, const QString &codeName, const lv_font_t *font);

	static LVGLFont *parse(const QString &fileName, uint8_t size, uint8_t bpp, uint32_t unicodeFirst, uint32_t unicodeLast);

	const lv_font_t *font() const;
	QString name() const;

	QString codeName() const;
	bool saveAsCode(const QString &fileName);

private:
	lv_font_t *m_font;
	QString m_fileName;
	QString m_name;
	QString m_codeName;
	uint8_t m_size;
	uint8_t m_bpp;

};

#endif // LVGLFONT_H
