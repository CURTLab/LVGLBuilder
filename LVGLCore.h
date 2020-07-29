#ifndef LVGLCORE_HPP
#define LVGLCORE_HPP

#include <lvgl/lvgl.h>

#include "LVGLImageData.h"
#include "widgets/LVGLWidget.h"

#include <QTimer>
#include <QElapsedTimer>
#include <QObject>
#include <QPixmap>
#include <QList>
#include <QHash>

class LVGLObject;
class LVGLFontData;

class LVGLCore : public QObject
{
	Q_OBJECT
public:
	LVGLCore(QObject *parent = nullptr);
	~LVGLCore();

	void init(int width, int height);

	bool changeResolution(QSize size);

	QPixmap framebuffer() const;
	QPixmap grab(const QRect &region) const;

	int width() const;
	int height() const;
	QSize size() const;

	LVGLImageData *addImage(QImage image, QString name);
	LVGLImageData *addImage(QString fileName, QString name = QString());
	void addImage(LVGLImageData *image);
	QStringList imageNames() const;
	QList<LVGLImageData *> images() const;
	lv_img_dsc_t *image(QString name);
	lv_img_dsc_t *defaultImage() const;
	QString nameByImage(const lv_img_dsc_t *img_dsc) const;
	LVGLImageData *imageByDesc(const lv_img_dsc_t *img_dsc) const;
	bool removeImage(LVGLImageData *img);
	void removeAllImages();

	QStringList symbolNames() const;
	const char *symbol(const QString &name) const;

	void poll();
	void sendMouseEvent(int x, int y, bool pressed);

	QPoint get_absolute_position(const lv_obj_t *lv_obj) const;
	QSize get_object_size(const lv_obj_t *lv_obj) const;
	QRect get_object_rect(const lv_obj_t *lv_obj) const;

	void addObject(LVGLObject *object);
	void removeObject(LVGLObject *object);
	void removeAllObjects();

	QList<LVGLObject *> allObjects() const;
	QList<LVGLObject *> topLevelObjects() const;
	QList<LVGLObject *> objectsByType(QString className) const;
	LVGLObject *object(QString name) const;
	LVGLObject *object(lv_obj_t *obj) const;

	QColor toColor(lv_color_t c) const;
	lv_color_t fromColor(QColor c) const;
	lv_color_t fromColor(QVariant v) const;
	QJsonObject colorToJson(lv_color_t c) const;
	lv_color_t colorFromJson(QJsonObject obj) const;

	LVGLFontData *addFont(const QString &fileName, uint8_t size);
	void addFont(LVGLFontData *font);
	bool removeFont(LVGLFontData *font);
	QStringList fontNames() const;
	QStringList fontCodeNames() const;
	const lv_font_t *font(int index) const;
	const lv_font_t *font(const QString &name, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
	int indexOfFont(const lv_font_t *font) const;
	QString fontName(const lv_font_t *font) const;
	QString fontCodeName(const lv_font_t *font) const;
	QList<const LVGLFontData *> customFonts() const;
	void removeCustomFonts();

	QString baseStyleName(const lv_style_t *style) const;

	void setScreenColor(QColor color);
	QColor screenColor() const;
	bool screenColorChanged() const;

	QList<const LVGLWidget *> widgets() const;
	const LVGLWidget *widget(const QString &name) const;

private slots:
	void tick();

private:
	void addWidget(LVGLWidget *w);
	void flushHandler(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
	bool inputHandler(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);

	static void flushCb(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
	static bool inputCb(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);
	static void logCb(lv_log_level_t level, const char *file, uint32_t line, const char *dsc);

	QTimer m_timer;
	QElapsedTimer m_time;
	QHash<QString,LVGLImageData*> m_images;
	QHash<QString,const LVGLWidget *> m_widgets;
	LVGLImageData *m_default;
	QList<LVGLObject*> m_objects;
	QList<LVGLFontData *> m_fonts;
	lv_style_t m_screenStyle;
	const LVGLFontData *m_defaultFont;

	std::vector<lv_color_t> m_dispFrameBuf;
	std::vector<lv_color_t> m_buf1;
	std::vector<lv_color_t> m_buf2;
	lv_disp_buf_t m_dispBuf;
	lv_disp_drv_t m_dispDrv;

	lv_indev_data_t m_inputData;

	struct FT_LibraryRec_ *m_ft;
	friend class LVGLFontData;
};

extern LVGLCore lvgl;

// cast helpers for internal models
union LVGLImageDataCast {
	LVGLImageData *ptr;
	qintptr i;
};

union LVGLFontDataCast {
	LVGLFontData *ptr;
	qintptr i;
};

union LVGLWidgetCast {
	LVGLWidget *ptr;
	qintptr i;
};

union LVGLObjectCast {
	LVGLObject *ptr;
	qintptr i;
};

#endif // LVGLCORE_HPP
