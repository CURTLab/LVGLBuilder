#ifndef LVGLFONTDATA_H
#define LVGLFONTDATA_H

#include <lvgl/lvgl.h>
#include <QString>
#include <QJsonObject>

class LVGLFontData
{
public:
	LVGLFontData(const QString &name, const QString &codeName, uint8_t size, const lv_font_t *font);
	~LVGLFontData();

	static LVGLFontData *parse(const QString &fileName, uint8_t size, uint8_t bpp, uint32_t unicodeFirst, uint32_t unicodeLast);
	static LVGLFontData *parse(QJsonObject object);

	const lv_font_t *font() const;
	QString name() const;

	QString codeName() const;
	bool saveAsCode(const QString &fileName) const;

	bool isCustomFont() const;

	QString fileName() const;

	QJsonObject toJson() const;

	uint8_t size() const;

private:
	lv_font_t *m_font;
	QString m_fileName;
	QString m_name;
	QString m_codeName;
	uint8_t m_size;
	bool m_customFont;

};

#endif // LVGLFONTDATA_H
