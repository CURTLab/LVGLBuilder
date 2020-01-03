#include "LVGLCore.h"

#include <QPainter>
#include <QDebug>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "widgets/LVGLWidgets.h"
#include "LVGLFontData.h"

static lv_color_t *disp_framebuffer;
static lv_disp_buf_t disp_buf;
static lv_color_t *buf1;
static lv_color_t *buf2;
static lv_indev_data_t disp_mouse;

QLVGL lvgl(nullptr);

#include "LVGLObject.h"

bool my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t * data)
{
	(void)indev_driver;

	data->state = disp_mouse.state;
	data->point.x = disp_mouse.point.x;
	data->point.y = disp_mouse.point.y;

	return false; /*Return `false` because we are not buffering and no more data to read*/
}

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
	const auto stride = disp->hor_res;
	for (auto y = area->y1; y <= area->y2; ++y) {
		for (auto x = area->x1; x <= area->x2; ++x) {
			disp_framebuffer[x + y * stride].full = color_p->full;
			color_p++;
		}
	}
	lv_disp_flush_ready(disp);
}

QLVGL::QLVGL(QObject *parent) : QObject(parent), m_defaultFont(nullptr)
{
	FT_Init_FreeType(&m_ft);
}

QLVGL::~QLVGL()
{
	FT_Done_FreeType(m_ft);

	qDeleteAll(m_images);
	qDeleteAll(m_widgets);
	qDeleteAll(m_fonts);
}

void lvgl_print_cb(lv_log_level_t level, const char *file, uint32_t line, const char *dsc)
{
	qDebug().nospace() << file << " (" << line << "," << level << "): " << dsc;
}

void QLVGL::init(int width, int height)
{
	lv_init();

	const uint32_t n = static_cast<uint32_t>(width * height);
	disp_framebuffer = new lv_color_t[n];
	buf1 = new lv_color_t[n];
	buf2 = new lv_color_t[n];

	lv_disp_buf_init(&disp_buf, buf1, buf2, n);

	lv_disp_drv_t disp_drv;
	lv_disp_drv_init(&disp_drv);
	disp_drv.hor_res = static_cast<lv_coord_t>(width);
	disp_drv.ver_res = static_cast<lv_coord_t>(height);

	disp_drv.flush_cb = my_disp_flush;
	disp_drv.buffer = &disp_buf;
	lv_disp_drv_register(&disp_drv);

	lv_indev_drv_t indev_drv;
	lv_indev_drv_init(&indev_drv);             /*Descriptor of a input device driver*/
	indev_drv.type = LV_INDEV_TYPE_POINTER;    /*Touch pad is a pointer-like device*/
	indev_drv.read_cb = my_touchpad_read;      /*Set your driver function*/
	lv_indev_drv_register(&indev_drv);         /*Finally register the driver*/

	QImage pix(":/images/littlevgl_logo.png");
	m_default = lvgl.addImage(pix, "default");

	lv_style_copy(&m_screen_style, &lv_style_scr);

#if LV_FONT_ROBOTO_12
	m_fonts << new LVGLFontData("Roboto 12", "lv_font_roboto_12", 12, &lv_font_roboto_12);
#endif
#if LV_FONT_ROBOTO_16
	m_fonts << new LVGLFontData("Roboto 16", "lv_font_roboto_16", 16, &lv_font_roboto_16);
#endif
#if LV_FONT_ROBOTO_22
	m_fonts << new LVGLFontData("Roboto 22", "lv_font_roboto_22", 22, &lv_font_roboto_22);
#endif
#if LV_FONT_ROBOTO_28
	m_fonts << new LVGLFontData("Roboto 28", "lv_font_roboto_28", 28, &lv_font_roboto_28);
#endif
#if LV_FONT_UNSCII_8
	m_fonts << new LVGLFontData("UNSCII 8", "lv_font_unscii_8", 8, &lv_font_unscii_8);
#endif

	// search for default font name
	for (const LVGLFontData *f:m_fonts) {
		if (f->font() == LV_FONT_DEFAULT) {
			m_defaultFont = f;
			break;
		}
	}
	Q_ASSERT(m_defaultFont != nullptr);

	addWidget(new LVGLBar);
	addWidget(new LVGLButton);
	addWidget(new LVGLButtonMatrix);
	addWidget(new LVGLCalendar);
	addWidget(new LVGLCanvas);
	addWidget(new LVGLCheckBox);
	addWidget(new LVGLColorPicker);
	addWidget(new LVGLContainer);
	addWidget(new LVGLDropDownList);
	addWidget(new LVGLGauge);
	addWidget(new LVGLImage);
	addWidget(new LVGLImageButton);
	addWidget(new LVGLImageSlider);
	addWidget(new LVGLKeyboard);
	addWidget(new LVGLLabel);
	addWidget(new LVGLLine);
	addWidget(new LVGLList);
	addWidget(new LVGLPage);
	addWidget(new LVGLPreloader);
	addWidget(new LVGLSlider);
	addWidget(new LVGLSwitch);
	addWidget(new LVGLTabview);
	addWidget(new LVGLTextArea);

	//lv_log_register_print_cb(lvgl_print_cb);
}

QPixmap QLVGL::framebuffer() const
{
	auto disp = lv_disp_get_default();
	auto width = lv_disp_get_hor_res(disp);
	auto height = lv_disp_get_ver_res(disp);

	QImage img(width, height, QImage::Format_ARGB32);
	memcpy(img.bits(), disp_framebuffer, static_cast<size_t>(width * height) * 4);
	return QPixmap::fromImage(img);
}

QPixmap QLVGL::grab(const QRect &region) const
{
	QPixmap ret(region.size());
	QPainter painter(&ret);
	const auto stride = lv_disp_get_hor_res(lv_disp_get_default());

	QImage img(region.width(), region.height(), QImage::Format_ARGB32);
	for (auto y = 0; y < region.height(); ++y)
		memcpy(img.scanLine(y + region.y()),
				 &disp_framebuffer[y * stride + region.x()],
				static_cast<size_t>(stride) * 4
				);
	return QPixmap::fromImage(img);
}

int QLVGL::width() const
{
	return lv_disp_get_hor_res(lv_disp_get_default());
}

int QLVGL::height() const
{
	return lv_disp_get_ver_res(lv_disp_get_default());
}

QSize QLVGL::size() const
{
	auto disp = lv_disp_get_default();
	return QSize(lv_disp_get_hor_res(disp),
					 lv_disp_get_ver_res(disp));
}

LVGLImageData *QLVGL::addImage(QImage image, QString name)
{
	if (image.isNull())
		return nullptr;
	LVGLImageData *img = new LVGLImageData(image, "", name);
	m_images.insert(name, img);
	return img;
}

LVGLImageData *QLVGL::addImage(QString fileName, QString name)
{
	QImage image(fileName);
	if (image.isNull())
		return nullptr;

	if (name.isEmpty()) {
		// create sorted list by type
		QList<int> numbers;
		for (LVGLImageData *o:m_images) {
			if (o->fileName().isEmpty())
				continue;
			const int index = o->name().lastIndexOf('_');
			const int num = o->name().mid(index+1).toInt();
			auto ind = std::lower_bound(numbers.begin(), numbers.end(), num);
			numbers.insert(ind, num);
		}

		// find next free id
		int id = 1;
		for (int num:numbers) {
			if (num == id)
				id++;
			else
				break;
		}
		name = QString("img_%1").arg(id);
	}

	LVGLImageData *img = new LVGLImageData(image, fileName, name);
	m_images.insert(name, img);
	return img;
}

void QLVGL::addImage(LVGLImageData *image)
{
	m_images.insert(image->name(), image);
}

QStringList QLVGL::imageNames() const
{
	return m_images.keys();
}

QList<LVGLImageData *> QLVGL::images() const
{
	return m_images.values();
}

lv_img_dsc_t *QLVGL::image(QString name)
{
	LVGLImageData *img = m_images.value(name, nullptr);
	if (img)
		return img->img_des();
	return nullptr;
}

lv_img_dsc_t *QLVGL::defaultImage() const
{
	return m_default->img_des();
}

QString QLVGL::nameByImage(const lv_img_dsc_t *img_dsc) const
{
	for (LVGLImageData *img:m_images) {
		if (img->img_des() == img_dsc)
			return img->name();
	}
	return "";
}

LVGLImageData *QLVGL::imageByDesc(const lv_img_dsc_t *img_dsc) const
{
	for (LVGLImageData *img:m_images) {
		if (img->img_des() == img_dsc)
			return img;
	}
	return nullptr;
}

bool QLVGL::removeImage(LVGLImageData *img)
{
	for (auto it = m_images.begin(); it != m_images.end(); ++it) {
		if (it.value() == img) {
			m_images.remove(it.key());
			delete img;
			return true;
		}
	}
	return false;
}

void QLVGL::removeAllImages()
{
	qDeleteAll(m_images);
	m_images.clear();
}

QStringList QLVGL::symbolNames() const
{
	return QStringList() << "LV_SYMBOL_AUDIO"
								<< "LV_SYMBOL_VIDEO"
								<< "LV_SYMBOL_LIST"
								<< "LV_SYMBOL_OK"
								<< "LV_SYMBOL_CLOSE"
								<< "LV_SYMBOL_POWER"
								<< "LV_SYMBOL_SETTINGS"
								<< "LV_SYMBOL_HOME"
								<< "LV_SYMBOL_DOWNLOAD"
								<< "LV_SYMBOL_DRIVE"
								<< "LV_SYMBOL_REFRESH"
								<< "LV_SYMBOL_MUTE"
								<< "LV_SYMBOL_VOLUME_MID"
								<< "LV_SYMBOL_VOLUME_MAX"
								<< "LV_SYMBOL_IMAGE"
								<< "LV_SYMBOL_EDIT"
								<< "LV_SYMBOL_PREV"
								<< "LV_SYMBOL_PLAY"
								<< "LV_SYMBOL_PAUSE"
								<< "LV_SYMBOL_STOP"
								<< "LV_SYMBOL_NEXT"
								<< "LV_SYMBOL_EJECT"
								<< "LV_SYMBOL_LEFT"
								<< "LV_SYMBOL_RIGHT"
								<< "LV_SYMBOL_PLUS"
								<< "LV_SYMBOL_MINUS"
								<< "LV_SYMBOL_EYE_OPEN"
								<< "LV_SYMBOL_EYE_CLOSE"
								<< "LV_SYMBOL_WARNING"
								<< "LV_SYMBOL_SHUFFLE"
								<< "LV_SYMBOL_UP"
								<< "LV_SYMBOL_DOWN"
								<< "LV_SYMBOL_LOOP"
								<< "LV_SYMBOL_DIRECTORY"
								<< "LV_SYMBOL_UPLOAD"
								<< "LV_SYMBOL_CALL"
								<< "LV_SYMBOL_CUT"
								<< "LV_SYMBOL_COPY"
								<< "LV_SYMBOL_SAVE"
								<< "LV_SYMBOL_CHARGE"
								<< "LV_SYMBOL_PASTE"
								<< "LV_SYMBOL_BELL"
								<< "LV_SYMBOL_KEYBOARD"
								<< "LV_SYMBOL_GPS"
								<< "LV_SYMBOL_FILE"
								<< "LV_SYMBOL_WIFI"
								<< "LV_SYMBOL_BATTERY_FULL"
								<< "LV_SYMBOL_BATTERY_3"
								<< "LV_SYMBOL_BATTERY_2"
								<< "LV_SYMBOL_BATTERY_1"
								<< "LV_SYMBOL_BATTERY_EMPTY"
								<< "LV_SYMBOL_USB"
								<< "LV_SYMBOL_BLUETOOTH"
								<< "LV_SYMBOL_TRASH"
								<< "LV_SYMBOL_BACKSPACE"
								<< "LV_SYMBOL_SD_CARD"
								<< "LV_SYMBOL_NEW_LINE";
}

const char *QLVGL::symbol(const QString &name) const
{
	if (name == "LV_SYMBOL_AUDIO") return LV_SYMBOL_AUDIO;
	else if (name == "LV_SYMBOL_VIDEO") return LV_SYMBOL_VIDEO;
	else if (name == "LV_SYMBOL_LIST") return LV_SYMBOL_LIST;
	else if (name == "LV_SYMBOL_OK") return LV_SYMBOL_OK;
	else if (name == "LV_SYMBOL_CLOSE") return LV_SYMBOL_CLOSE;
	else if (name == "LV_SYMBOL_POWER") return LV_SYMBOL_POWER;
	else if (name == "LV_SYMBOL_SETTINGS") return LV_SYMBOL_SETTINGS;
	else if (name == "LV_SYMBOL_HOME") return LV_SYMBOL_HOME;
	else if (name == "LV_SYMBOL_DOWNLOAD") return LV_SYMBOL_DOWNLOAD;
	else if (name == "LV_SYMBOL_DRIVE") return LV_SYMBOL_DRIVE;
	else if (name == "LV_SYMBOL_REFRESH") return LV_SYMBOL_REFRESH;
	else if (name == "LV_SYMBOL_MUTE") return LV_SYMBOL_MUTE;
	else if (name == "LV_SYMBOL_VOLUME_MID") return LV_SYMBOL_VOLUME_MID;
	else if (name == "LV_SYMBOL_VOLUME_MAX") return LV_SYMBOL_VOLUME_MAX;
	else if (name == "LV_SYMBOL_IMAGE") return LV_SYMBOL_IMAGE;
	else if (name == "LV_SYMBOL_EDIT") return LV_SYMBOL_EDIT;
	else if (name == "LV_SYMBOL_PREV") return LV_SYMBOL_PREV;
	else if (name == "LV_SYMBOL_PLAY") return LV_SYMBOL_PLAY;
	else if (name == "LV_SYMBOL_PAUSE") return LV_SYMBOL_PAUSE;
	else if (name == "LV_SYMBOL_STOP") return LV_SYMBOL_STOP;
	else if (name == "LV_SYMBOL_NEXT") return LV_SYMBOL_NEXT;
	else if (name == "LV_SYMBOL_EJECT") return LV_SYMBOL_EJECT;
	else if (name == "LV_SYMBOL_LEFT") return LV_SYMBOL_LEFT;
	else if (name == "LV_SYMBOL_RIGHT") return LV_SYMBOL_RIGHT;
	else if (name == "LV_SYMBOL_PLUS") return LV_SYMBOL_PLUS;
	else if (name == "LV_SYMBOL_MINUS") return LV_SYMBOL_MINUS;
	else if (name == "LV_SYMBOL_EYE_OPEN") return LV_SYMBOL_EYE_OPEN;
	else if (name == "LV_SYMBOL_EYE_CLOSE") return LV_SYMBOL_EYE_CLOSE;
	else if (name == "LV_SYMBOL_WARNING") return LV_SYMBOL_WARNING;
	else if (name == "LV_SYMBOL_SHUFFLE") return LV_SYMBOL_SHUFFLE;
	else if (name == "LV_SYMBOL_UP") return LV_SYMBOL_UP;
	else if (name == "LV_SYMBOL_DOWN") return LV_SYMBOL_DOWN;
	else if (name == "LV_SYMBOL_LOOP") return LV_SYMBOL_LOOP;
	else if (name == "LV_SYMBOL_DIRECTORY") return LV_SYMBOL_DIRECTORY;
	else if (name == "LV_SYMBOL_UPLOAD") return LV_SYMBOL_UPLOAD;
	else if (name == "LV_SYMBOL_CALL") return LV_SYMBOL_CALL;
	else if (name == "LV_SYMBOL_CUT") return LV_SYMBOL_CUT;
	else if (name == "LV_SYMBOL_COPY") return LV_SYMBOL_COPY;
	else if (name == "LV_SYMBOL_SAVE") return LV_SYMBOL_SAVE;
	else if (name == "LV_SYMBOL_CHARGE") return LV_SYMBOL_CHARGE;
	else if (name == "LV_SYMBOL_PASTE") return LV_SYMBOL_PASTE;
	else if (name == "LV_SYMBOL_BELL") return LV_SYMBOL_BELL;
	else if (name == "LV_SYMBOL_KEYBOARD") return LV_SYMBOL_KEYBOARD;
	else if (name == "LV_SYMBOL_GPS") return LV_SYMBOL_GPS;
	else if (name == "LV_SYMBOL_FILE") return LV_SYMBOL_FILE;
	else if (name == "LV_SYMBOL_WIFI") return LV_SYMBOL_WIFI;
	else if (name == "LV_SYMBOL_BATTERY_FULL") return LV_SYMBOL_BATTERY_FULL;
	else if (name == "LV_SYMBOL_BATTERY_3") return LV_SYMBOL_BATTERY_3;
	else if (name == "LV_SYMBOL_BATTERY_2") return LV_SYMBOL_BATTERY_2;
	else if (name == "LV_SYMBOL_BATTERY_1") return LV_SYMBOL_BATTERY_1;
	else if (name == "LV_SYMBOL_BATTERY_EMPTY") return LV_SYMBOL_BATTERY_EMPTY;
	else if (name == "LV_SYMBOL_USB") return LV_SYMBOL_USB;
	else if (name == "LV_SYMBOL_BLUETOOTH") return LV_SYMBOL_BLUETOOTH;
	else if (name == "LV_SYMBOL_TRASH") return LV_SYMBOL_TRASH;
	else if (name == "LV_SYMBOL_BACKSPACE") return LV_SYMBOL_BACKSPACE;
	else if (name == "LV_SYMBOL_SD_CARD") return LV_SYMBOL_SD_CARD;
	else if (name == "LV_SYMBOL_NEW_LINE") return LV_SYMBOL_NEW_LINE;
	else return nullptr;
}

void QLVGL::poll()
{
	lv_task_handler();
	lv_tick_inc(static_cast<uint32_t>(m_time.elapsed()));
	m_time.restart();
}

void QLVGL::send_mouse_event(int x, int y, bool pressed)
{
	disp_mouse.point.x = static_cast<lv_coord_t>(x);
	disp_mouse.point.y = static_cast<lv_coord_t>(y);
	disp_mouse.state = pressed ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;
}

QList<lv_obj_t *> QLVGL::get_objects_under_coords(int x, int y, lv_obj_t *parent) const
{
	QList<lv_obj_t *> objects;

	lv_obj_t *child = lv_obj_get_child(parent, nullptr);
	while (child) {
		auto rect = get_object_rect(child);
		if (rect.contains(x, y)) {
			objects << child;
			objects << get_objects_under_coords(x, y, child);
		}
		child = lv_obj_get_child(parent, child);
	}

	return objects;
}

QList<lv_obj_t *> QLVGL::get_object_children(const lv_obj_t *obj, bool recursive) const
{
	QList<lv_obj_t *> objects;
	lv_obj_t *child = lv_obj_get_child(obj, nullptr);
	while (child) {
		objects << child;
		if (recursive)
			objects << get_object_children(child, recursive);
		child = lv_obj_get_child(obj, child);
	}
	return objects;
}

QList<lv_obj_t *> QLVGL::get_objects_by_type(QString type, lv_obj_t *parent) const
{
	QList<lv_obj_t *> objects;
	lv_obj_t *child = lv_obj_get_child(lv_scr_act(), nullptr);
	while (child) {
		if (get_object_class(child) == type)
			objects << child;
		child = lv_obj_get_child(parent, child);
	}

	return objects;
}

int QLVGL::get_object_child_index(const lv_obj_t *obj) const
{
	lv_obj_t *parent = lv_obj_get_parent(obj);
	if (parent == nullptr) return -1;
	lv_obj_t *child = lv_obj_get_child(parent, nullptr);
	for (int i = 0; child != nullptr; ++i, child = lv_obj_get_child(parent, child)) {
		if (child == obj)
			return i;
	}
	return -1;
}

QPoint QLVGL::get_absolute_position(const lv_obj_t *obj) const
{
	if (obj == lv_scr_act())
		return QPoint(0, 0);
	int x = lv_obj_get_x(obj);
	int y = lv_obj_get_y(obj);
	lv_obj_t *parent = lv_obj_get_parent(obj);
	while (parent) {
		if (parent == lv_scr_act()) break;
		x += lv_obj_get_x(parent);
		y += lv_obj_get_y(parent);
		parent = lv_obj_get_parent(parent);
	}
	return QPoint(x, y);
}

QSize QLVGL::get_object_size(const lv_obj_t *lv_obj) const
{
	return QSize(lv_obj_get_width(lv_obj), lv_obj_get_height(lv_obj));
}

QRect QLVGL::get_object_rect(const lv_obj_t *lv_obj) const
{
	return QRect(get_absolute_position(lv_obj), get_object_size(lv_obj));
}

QString QLVGL::get_object_class(lv_obj_t *lv_obj) const
{
	lv_obj_type_t buf;
	lv_obj_get_type(lv_obj, &buf);
	return QString(buf.type[0]);
}

void QLVGL::set_object_position(lv_obj_t *lv_obj, const QPoint &pos)
{
	lv_obj_set_pos(lv_obj, static_cast<lv_coord_t>(pos.x()), static_cast<lv_coord_t>(pos.y()));
}

void QLVGL::set_object_geometry(lv_obj_t *lv_obj, const QRect &geometry)
{
	lv_obj_set_pos(lv_obj, static_cast<lv_coord_t>(geometry.x()), static_cast<lv_coord_t>(geometry.y()));
	lv_obj_set_size(lv_obj, static_cast<lv_coord_t>(geometry.width()), static_cast<lv_coord_t>(geometry.height()));
}

void QLVGL::addObject(LVGLObject *object)
{
	m_objects << object;
}

void QLVGL::removeObject(LVGLObject *object)
{
	for (LVGLObject *c:object->childs())
		removeObject(c);
	if (object->parent())
		object->parent()->removeChild(object);
	m_objects.removeOne(object);
	delete object;
}

void QLVGL::removeAllObjects()
{
	for (LVGLObject *c:m_objects) {
		if (c->parent() == nullptr)
			removeObject(c);
	}
}

QList<LVGLObject *> QLVGL::allObjects() const
{
	return m_objects;
}

QList<LVGLObject *> QLVGL::topLevelObjects() const
{
	QList<LVGLObject *> ret;
	for (LVGLObject *c:m_objects) {
		if (c->parent() == nullptr)
			ret << c;
	}
	return ret;
}

QList<LVGLObject *> QLVGL::objectsByType(QString className) const
{
	QList<LVGLObject *> ret;
	for (LVGLObject *c:m_objects) {
		if (c->widgetClass()->className() == className)
			ret << c;
	}
	return ret;
}

LVGLObject *QLVGL::object(QString name) const
{
	if (name.isEmpty())
		return nullptr;
	for (LVGLObject *c:m_objects) {
		if (c->name() == name)
			return c;
	}
	return nullptr;
}

LVGLObject *QLVGL::object(lv_obj_t *obj) const
{
	if (obj == nullptr)
		return nullptr;
	for (LVGLObject *c:m_objects) {
		if (c->obj() == obj)
			return c;
	}
	return nullptr;
}

QColor QLVGL::toColor(lv_color_t c) const
{
#if LV_COLOR_DEPTH == 24
	return QColor(c.ch.red, c.ch.green, c.ch.blue);
#elif LV_COLOR_DEPTH == 32
	return QColor(c.ch.red, c.ch.green, c.ch.blue, c.ch.alpha);
#endif
}

lv_color_t QLVGL::fromColor(QColor c) const
{
#if LV_COLOR_DEPTH == 24
	lv_color_t ret;
	ret.ch.red = c.red() & 0xff;
	ret.ch.green = c.green() & 0xff;
	ret.ch.blue = c.blue() & 0xff;
	return ret;
#elif LV_COLOR_DEPTH == 32
	lv_color_t ret;
	ret.ch.red = c.red() & 0xff;
	ret.ch.green = c.green() & 0xff;
	ret.ch.blue = c.blue() & 0xff;
	ret.ch.alpha = c.alpha() & 0xff;
	return ret;
#endif
}

lv_color_t QLVGL::fromColor(QVariant v) const
{
	return fromColor(v.value<QColor>());
}

LVGLFontData *QLVGL::addFont(const QString &fileName, uint8_t size)
{
	LVGLFontData *font = LVGLFontData::parse(fileName, size, 4, 0x0020, 0x007f);
	if (font)
		m_fonts << font;
	return font;
}

void QLVGL::addFont(LVGLFontData *font)
{
	if (font)
		m_fonts << font;
}

bool QLVGL::removeFont(LVGLFontData *font)
{
	return m_fonts.removeOne(font);
}

QStringList QLVGL::fontNames() const
{
	QStringList ret;
	for (const LVGLFontData *f:m_fonts)
		ret << f->name();
	return ret;
}

QStringList QLVGL::fontCodeNames() const
{
	QStringList ret;
	for (const LVGLFontData *f:m_fonts)
		ret << f->codeName();
	return ret;
}

const lv_font_t *QLVGL::font(int index) const
{
	if ((index > 0) && (index < m_fonts.size()))
		return m_fonts.at(index)->font();
	return m_defaultFont->font();
}

const lv_font_t *QLVGL::font(const QString &name, Qt::CaseSensitivity cs) const
{
	for (const LVGLFontData *font:m_fonts) {
		if (name.compare(font->name(), cs) == 0)
			return font->font();
	}
	return m_defaultFont->font();
}

int QLVGL::indexOfFont(const lv_font_t *font) const
{
	int index = 0;
	for (auto it = m_fonts.begin(); it != m_fonts.end(); ++it, ++index) {
		if ((*it)->font() == font)
			return index;
	}
	return -1;
}

QString QLVGL::fontName(const lv_font_t *font) const
{
	for (const LVGLFontData *f:m_fonts) {
		if (f->font() == font)
			return f->name();
	}
	return m_defaultFont->name();
}

QString QLVGL::fontCodeName(const lv_font_t *font) const
{
	for (const LVGLFontData *f:m_fonts) {
		if (f->font() == font)
			return f->codeName();
	}
	return m_defaultFont->codeName();
}

QList<const LVGLFontData *> QLVGL::customFonts() const
{
	QList<const LVGLFontData *> ret;
	for (const LVGLFontData *font:m_fonts) {
		if (font->isCustomFont())
			ret << font;
	}
	return  ret;
}

void QLVGL::removeCustomFonts()
{
	for (auto it = m_fonts.begin(); it != m_fonts.end();) {
		if ((*it)->isCustomFont()) {
			delete *it;
			it = m_fonts.erase(it);
		} else {
			++it;
		}
	}
}

QString QLVGL::baseStyleName(const lv_style_t *style) const
{
	if (style == &lv_style_scr)
		return "lv_style_scr";
	else if (style == &lv_style_transp)
		return "lv_style_transp";
	else if (style == &lv_style_transp_fit)
		return "lv_style_transp_fit";
	else if (style == &lv_style_transp_tight)
		return "lv_style_transp_tight";
	else if (style == &lv_style_plain)
		return "lv_style_plain";
	else if (style == &lv_style_plain_color)
		return "lv_style_plain_color";
	else if (style == &lv_style_pretty)
		return "lv_style_pretty";
	else if (style == &lv_style_pretty_color)
		return "lv_style_pretty_color";
	else if (style == &lv_style_btn_rel)
		return "lv_style_btn_rel";
	else if (style == &lv_style_btn_pr)
		return "lv_style_btn_pr";
	else if (style == &lv_style_btn_tgl_rel)
		return "lv_style_btn_tgl_rel";
	else if (style == &lv_style_btn_tgl_pr)
		return "lv_style_btn_tgl_pr";
	else if (style == &lv_style_btn_ina)
		return "lv_style_btn_ina";
	return  "";
}

void QLVGL::setScreenColor(QColor color)
{
	m_screen_style.body.main_color = fromColor(color);
	m_screen_style.body.grad_color = fromColor(color);
	lv_obj_set_style(lv_scr_act(), &m_screen_style);
}

QColor QLVGL::screenColor() const
{
	return toColor(m_screen_style.body.main_color);
}

bool QLVGL::screenColorChanged() const
{
	return (m_screen_style.body.main_color.full != lv_style_scr.body.main_color.full &&
			m_screen_style.body.grad_color.full != lv_style_scr.body.grad_color.full);
}

QList<const LVGLWidget *> QLVGL::widgets() const
{
	return m_widgets.values();
}

const LVGLWidget *QLVGL::widget(const QString &name) const
{
	if (m_widgets.contains(name))
		return m_widgets[name];
	return nullptr;
}

void QLVGL::tick()
{
	lv_task_handler();
	lv_tick_inc(20);
}

void QLVGL::addWidget(const LVGLWidget *w)
{
	m_widgets.insert(w->className(), w);
}
