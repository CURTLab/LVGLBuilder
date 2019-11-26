#ifndef LVGL_HPP
#define LVGL_HPP

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

class QLVGL : public QObject
{
	Q_OBJECT
public:
	QLVGL(QObject *parent = nullptr);
	~QLVGL();

	void init(int width, int height);

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

	void poll();
	void send_mouse_event(int x, int y, bool pressed);

	QList<lv_obj_t *> get_objects_under_coords(int x, int y, lv_obj_t *parent = lv_scr_act()) const;
	QList<lv_obj_t *> get_object_children(const lv_obj_t *obj, bool recursive) const;
	QList<lv_obj_t *> get_objects_by_type(QString type, lv_obj_t *parent = lv_scr_act()) const;
	int get_object_child_index(const lv_obj_t *obj) const;

	QPoint get_absolute_position(const lv_obj_t *lv_obj) const;
	QSize get_object_size(const lv_obj_t *lv_obj) const;
	QRect get_object_rect(const lv_obj_t *lv_obj) const;
	QString get_object_class(lv_obj_t *lv_obj) const;
	void set_object_position(lv_obj_t *lv_obj, const QPoint &pos);
	void set_object_geometry(lv_obj_t *lv_obj, const QRect &geometry);

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

	QStringList fontNames() const;
	QStringList fontCodeNames() const;
	const lv_font_t *font(int index) const;
	int indexOfFont(const lv_font_t *font) const;

	QString baseStyleName(const lv_style_t *style) const;

	void setScreenColor(QColor color);
	QColor screenColor() const;
	bool screenColorChanged() const;

	QList<const LVGLWidget *> widgets() const;
	const LVGLWidget *widget(const QString &name) const;

private slots:
	void tick();

private:
	void addWidget(const LVGLWidget *w);

	QTimer m_timer;
	QElapsedTimer m_time;
	QHash<QString,LVGLImageData*> m_images;
	QHash<QString,const LVGLWidget *> m_widgets;
	LVGLImageData *m_default;
	QList<LVGLObject*> m_objects;
	QStringList m_fontNames;
	QStringList m_fontCodeNames;
	QList<const lv_font_t*> m_fonts;
	lv_style_t m_screen_style;

};

extern QLVGL lvgl;

#endif // LVGL_HPP
