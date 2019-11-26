#ifndef LVGLIMAGEDATA_H
#define LVGLIMAGEDATA_H

#include <QImage>
#include <QIcon>
#include <lvgl/lvgl.h>

class LVGLImageData
{
public:
	LVGLImageData();
	LVGLImageData(QImage image, QString fileName, QString name);
	LVGLImageData(QString fileName, QString name);
	LVGLImageData(QJsonObject object);
	~LVGLImageData();

	enum ColorFormat {
		LV_COLOR_1Bit,
		LV_COLOR_8Bit,
		LV_COLOR_16Bit,
		LV_COLOR_24Bit,
		LV_COLOR_32Bit
	};

	lv_img_dsc_t *img_des();

	QString name() const;

	QJsonValue toJson() const;

	QString fileName() const;

	QIcon icon() const;

	int width() const;
	int height() const;

	QString codeName() const;
	bool saveAsCode(const QString &fileName);

	ColorFormat colorFormat() const;
	void setColorFormat(const ColorFormat &colorFormat);

	static QStringList colorFormats();

private:
	uint32_t m_size;
	uint8_t *m_data;
	lv_img_dsc_t m_img_dsc;
	QString m_name;
	QString m_fileName;
	QIcon m_icon;
	ColorFormat m_colorFormat;

	static constexpr int ICON_SIZE = 128;

};

#endif // LVGLIMAGEDATA_H
