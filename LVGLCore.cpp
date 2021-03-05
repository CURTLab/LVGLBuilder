#include "LVGLCore.h"

#include <ft2build.h>

#include <QDebug>
#include <QJsonObject>
#include <QPainter>
#include FT_FREETYPE_H

#include "LVGLFontData.h"
#include "LVGLHelper.h"
#include "LVGLObject.h"
#include "MainWindow.h"
#include "widgets/LVGLWidgets.h"

LVGLCore *lvgl(nullptr);

const char *LVGLCore::DEFAULT_DAYS[7] = {"Su", "Mo", "Tu", "We",
                                         "Th", "Fr", "Sa"};
const char *LVGLCore::DEFAULT_MONTHS[12] = {
    "January", "February", "March",     "April",   "May",      "June",
    "July",    "August",   "September", "October", "November", "December"};

const lv_state_t LVGLCore::LVGL_STATE[7] = {
    LV_STATE_DEFAULT, LV_STATE_CHECKED, LV_STATE_FOCUSED, LV_STATE_EDITED,
    LV_STATE_HOVERED, LV_STATE_PRESSED, LV_STATE_DISABLED};

const char *LVGLCore::LVGL_STATE_STR[7] = {
    "LV_STATE_DEFAULT", "LV_STATE_CHECKED", "LV_STATE_FOCUSED",
    "LV_STATE_EDITED",  "LV_STATE_HOVERED", "LV_STATE_PRESSED",
    "LV_STATE_DISABLED"};

lv_style_t lv_style_scr;

LVGLCore::LVGLCore(QObject *parent)
    : QObject(parent), m_defaultFont(nullptr), m_dispt(nullptr) {
  FT_Init_FreeType(&m_ft);
  lv_style_init(&lv_style_scr);
  lv_style_init(&m_screenStyle);
}

LVGLCore::~LVGLCore() {
  FT_Done_FreeType(m_ft);

  qDeleteAll(m_images);
  qDeleteAll(m_fonts);
}

void LVGLCore::init(int width, int height) {
  lv_init();

  const uint32_t n = static_cast<uint32_t>(width * height);
  m_dispFrameBuf.resize(n);
  m_buf1.resize(n);
  m_buf2.resize(n);

  lv_disp_buf_init(&m_dispBuf, m_buf1.data(), m_buf2.data(), n);

  lv_disp_drv_init(&m_dispDrv);
  m_dispDrv.hor_res = static_cast<lv_coord_t>(width);
  m_dispDrv.ver_res = static_cast<lv_coord_t>(height);
  m_dispDrv.user_data = this;
  m_dispDrv.flush_cb = flushCb;
  m_dispDrv.buffer = &m_dispBuf;
  m_dispt = lv_disp_drv_register(&m_dispDrv);

  // to be sure that there is no button press at the start
  m_inputData.state = LV_INDEV_STATE_REL;

  lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = inputCb;
  indev_drv.user_data = this;
  lv_indev_drv_register(&indev_drv);

  QImage pix(":/images/littlevgl_logo.png");
  m_default = lvgl->addImage(pix, "default");

  lv_style_copy(&m_screenStyle, &lv_style_scr);
#if LV_FONT_CHINESE_16
  m_fonts << new LVGLFontData("Chinese 16", "lv_font_chinese_16", 16,
                              &lv_font_chinese_16);
#endif
#if LV_FONT_SIMSUN_16_CJK
  m_fonts << new LVGLFontData("Simsun 16", "lv_font_simsun_16", 16,
                              &lv_font_simsun_16_cjk);
#endif
#if LV_FONT_ROBOTO_12
  m_fonts << new LVGLFontData("Roboto 12", "lv_font_roboto_12", 12,
                              &lv_font_roboto_12);
#endif
#if LV_FONT_ROBOTO_16
  m_fonts << new LVGLFontData("Roboto 16", "lv_font_roboto_16", 16,
                              &lv_font_roboto_16);
#endif
#if LV_FONT_ROBOTO_22
  m_fonts << new LVGLFontData("Roboto 22", "lv_font_roboto_22", 22,
                              &lv_font_roboto_22);
#endif
#if LV_FONT_ROBOTO_28
  m_fonts << new LVGLFontData("Roboto 28", "lv_font_roboto_28", 28,
                              &lv_font_roboto_28);
#endif
#if LV_FONT_UNSCII_8
  m_fonts << new LVGLFontData("UNSCII 8", "lv_font_unscii_8", 8,
                              &lv_font_unscii_8);
#endif

  // search for default font name
  for (const LVGLFontData *f : m_fonts) {
    if (f->font() == LV_FONT_DEFAULT) {
      m_defaultFont = f;
      break;
    }
  }
  Q_ASSERT(m_defaultFont != nullptr);

  setScreenColor(Qt::white);
  changeResolution({width, height});
  // lv_log_register_print_cb(logCb);
}

bool LVGLCore::changeResolution(QSize size) {
  const uint32_t n = static_cast<uint32_t>(size.width() * size.height());
  if (n != m_dispBuf.size) {
    m_dispFrameBuf.resize(n);
    m_buf1.resize(n);
    m_buf2.resize(n);
    lv_disp_buf_init(&m_dispBuf, m_buf1.data(), m_buf2.data(), n);
  }

  m_dispDrv.hor_res = static_cast<lv_coord_t>(size.width());
  m_dispDrv.ver_res = static_cast<lv_coord_t>(size.height());
  lv_disp_drv_update(m_dispt, &m_dispDrv);

  return false;
}

QPixmap LVGLCore::framebuffer() const {
  auto width = lv_disp_get_hor_res(m_dispt);
  auto height = lv_disp_get_ver_res(m_dispt);
  ;

  QImage img(width, height, QImage::Format_ARGB32);
  memcpy(img.bits(), m_dispFrameBuf.data(),
         static_cast<size_t>(width * height) * 4);
  return QPixmap::fromImage(img);
}

QPixmap LVGLCore::grab(const QRect &region) const {
  const auto stride = lv_disp_get_hor_res(m_dispt);

  QImage img(region.width(), region.height(), QImage::Format_ARGB32);
  for (auto y = 0; y < region.height(); ++y)
    memcpy(img.scanLine(y + region.y()),
           &m_dispFrameBuf[static_cast<size_t>(y * stride + region.x())],
           static_cast<size_t>(stride) * 4);
  QPixmap pix;
  try {
    pix = QPixmap::fromImage(img);
  } catch (std::exception const &ex) {
    qDebug() << ex.what();
  }
  return pix;
}

int LVGLCore::width() const { return lv_disp_get_hor_res(m_dispt); }

int LVGLCore::height() const { return lv_disp_get_ver_res(m_dispt); }

QSize LVGLCore::size() const {
  return QSize(lv_disp_get_hor_res(m_dispt), lv_disp_get_ver_res(m_dispt));
}

LVGLImageData *LVGLCore::addImage(QImage image, QString name) {
  if (image.isNull()) return nullptr;
  LVGLImageData *img = new LVGLImageData(image, "", name);
  m_images.insert(name, img);
  return img;
}

LVGLImageData *LVGLCore::addImage(QString fileName, QString name) {
  QImage image(fileName);
  if (image.isNull()) return nullptr;

  if (name.isEmpty()) {
    // create sorted list by type
    QList<int> numbers;
    for (LVGLImageData *o : m_images) {
      if (o->fileName().isEmpty()) continue;
      const int index = o->name().lastIndexOf('_');
      const int num = o->name().mid(index + 1).toInt();
      auto ind = std::lower_bound(numbers.begin(), numbers.end(), num);
      numbers.insert(ind, num);
    }

    // find next free id
    int id = 1;
    for (int num : numbers) {
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

void LVGLCore::addImage(LVGLImageData *image) {
  m_images.insert(image->name(), image);
}

QStringList LVGLCore::imageNames() const { return m_images.keys(); }

QList<LVGLImageData *> LVGLCore::images() const { return m_images.values(); }

lv_img_dsc_t *LVGLCore::image(QString name) {
  LVGLImageData *img = m_images.value(name, nullptr);
  if (img) return img->img_des();
  return nullptr;
}

lv_img_dsc_t *LVGLCore::defaultImage() const { return m_default->img_des(); }

QString LVGLCore::nameByImage(const lv_img_dsc_t *img_dsc) const {
  for (LVGLImageData *img : m_images) {
    if (img->img_des() == img_dsc) return img->name();
  }
  return "";
}

LVGLImageData *LVGLCore::imageByDesc(const lv_img_dsc_t *img_dsc) const {
  for (LVGLImageData *img : m_images) {
    if (img->img_des() == img_dsc) return img;
  }
  return nullptr;
}

bool LVGLCore::removeImage(LVGLImageData *img) {
  for (auto it = m_images.begin(); it != m_images.end(); ++it) {
    if (it.value() == img) {
      m_images.remove(it.key());
      delete img;
      return true;
    }
  }
  return false;
}

void LVGLCore::removeAllImages() {
  qDeleteAll(m_images);
  m_images.clear();
}

QStringList LVGLCore::symbolNames() const {
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

const char *LVGLCore::symbol(const QString &name) const {
  if (name == "LV_SYMBOL_AUDIO")
    return LV_SYMBOL_AUDIO;
  else if (name == "LV_SYMBOL_VIDEO")
    return LV_SYMBOL_VIDEO;
  else if (name == "LV_SYMBOL_LIST")
    return LV_SYMBOL_LIST;
  else if (name == "LV_SYMBOL_OK")
    return LV_SYMBOL_OK;
  else if (name == "LV_SYMBOL_CLOSE")
    return LV_SYMBOL_CLOSE;
  else if (name == "LV_SYMBOL_POWER")
    return LV_SYMBOL_POWER;
  else if (name == "LV_SYMBOL_SETTINGS")
    return LV_SYMBOL_SETTINGS;
  else if (name == "LV_SYMBOL_HOME")
    return LV_SYMBOL_HOME;
  else if (name == "LV_SYMBOL_DOWNLOAD")
    return LV_SYMBOL_DOWNLOAD;
  else if (name == "LV_SYMBOL_DRIVE")
    return LV_SYMBOL_DRIVE;
  else if (name == "LV_SYMBOL_REFRESH")
    return LV_SYMBOL_REFRESH;
  else if (name == "LV_SYMBOL_MUTE")
    return LV_SYMBOL_MUTE;
  else if (name == "LV_SYMBOL_VOLUME_MID")
    return LV_SYMBOL_VOLUME_MID;
  else if (name == "LV_SYMBOL_VOLUME_MAX")
    return LV_SYMBOL_VOLUME_MAX;
  else if (name == "LV_SYMBOL_IMAGE")
    return LV_SYMBOL_IMAGE;
  else if (name == "LV_SYMBOL_EDIT")
    return LV_SYMBOL_EDIT;
  else if (name == "LV_SYMBOL_PREV")
    return LV_SYMBOL_PREV;
  else if (name == "LV_SYMBOL_PLAY")
    return LV_SYMBOL_PLAY;
  else if (name == "LV_SYMBOL_PAUSE")
    return LV_SYMBOL_PAUSE;
  else if (name == "LV_SYMBOL_STOP")
    return LV_SYMBOL_STOP;
  else if (name == "LV_SYMBOL_NEXT")
    return LV_SYMBOL_NEXT;
  else if (name == "LV_SYMBOL_EJECT")
    return LV_SYMBOL_EJECT;
  else if (name == "LV_SYMBOL_LEFT")
    return LV_SYMBOL_LEFT;
  else if (name == "LV_SYMBOL_RIGHT")
    return LV_SYMBOL_RIGHT;
  else if (name == "LV_SYMBOL_PLUS")
    return LV_SYMBOL_PLUS;
  else if (name == "LV_SYMBOL_MINUS")
    return LV_SYMBOL_MINUS;
  else if (name == "LV_SYMBOL_EYE_OPEN")
    return LV_SYMBOL_EYE_OPEN;
  else if (name == "LV_SYMBOL_EYE_CLOSE")
    return LV_SYMBOL_EYE_CLOSE;
  else if (name == "LV_SYMBOL_WARNING")
    return LV_SYMBOL_WARNING;
  else if (name == "LV_SYMBOL_SHUFFLE")
    return LV_SYMBOL_SHUFFLE;
  else if (name == "LV_SYMBOL_UP")
    return LV_SYMBOL_UP;
  else if (name == "LV_SYMBOL_DOWN")
    return LV_SYMBOL_DOWN;
  else if (name == "LV_SYMBOL_LOOP")
    return LV_SYMBOL_LOOP;
  else if (name == "LV_SYMBOL_DIRECTORY")
    return LV_SYMBOL_DIRECTORY;
  else if (name == "LV_SYMBOL_UPLOAD")
    return LV_SYMBOL_UPLOAD;
  else if (name == "LV_SYMBOL_CALL")
    return LV_SYMBOL_CALL;
  else if (name == "LV_SYMBOL_CUT")
    return LV_SYMBOL_CUT;
  else if (name == "LV_SYMBOL_COPY")
    return LV_SYMBOL_COPY;
  else if (name == "LV_SYMBOL_SAVE")
    return LV_SYMBOL_SAVE;
  else if (name == "LV_SYMBOL_CHARGE")
    return LV_SYMBOL_CHARGE;
  else if (name == "LV_SYMBOL_PASTE")
    return LV_SYMBOL_PASTE;
  else if (name == "LV_SYMBOL_BELL")
    return LV_SYMBOL_BELL;
  else if (name == "LV_SYMBOL_KEYBOARD")
    return LV_SYMBOL_KEYBOARD;
  else if (name == "LV_SYMBOL_GPS")
    return LV_SYMBOL_GPS;
  else if (name == "LV_SYMBOL_FILE")
    return LV_SYMBOL_FILE;
  else if (name == "LV_SYMBOL_WIFI")
    return LV_SYMBOL_WIFI;
  else if (name == "LV_SYMBOL_BATTERY_FULL")
    return LV_SYMBOL_BATTERY_FULL;
  else if (name == "LV_SYMBOL_BATTERY_3")
    return LV_SYMBOL_BATTERY_3;
  else if (name == "LV_SYMBOL_BATTERY_2")
    return LV_SYMBOL_BATTERY_2;
  else if (name == "LV_SYMBOL_BATTERY_1")
    return LV_SYMBOL_BATTERY_1;
  else if (name == "LV_SYMBOL_BATTERY_EMPTY")
    return LV_SYMBOL_BATTERY_EMPTY;
  else if (name == "LV_SYMBOL_USB")
    return LV_SYMBOL_USB;
  else if (name == "LV_SYMBOL_BLUETOOTH")
    return LV_SYMBOL_BLUETOOTH;
  else if (name == "LV_SYMBOL_TRASH")
    return LV_SYMBOL_TRASH;
  else if (name == "LV_SYMBOL_BACKSPACE")
    return LV_SYMBOL_BACKSPACE;
  else if (name == "LV_SYMBOL_SD_CARD")
    return LV_SYMBOL_SD_CARD;
  else if (name == "LV_SYMBOL_NEW_LINE")
    return LV_SYMBOL_NEW_LINE;
  else
    return nullptr;
}

void LVGLCore::poll() {
  lv_task_handler();
  lv_tick_inc(static_cast<uint32_t>(m_time.elapsed()));
  m_time.restart();
}

void LVGLCore::sendMouseEvent(int x, int y, bool pressed) {
  m_inputData.point.x = static_cast<lv_coord_t>(x);
  m_inputData.point.y = static_cast<lv_coord_t>(y);
  m_inputData.state = pressed ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;
}

QPoint LVGLCore::get_absolute_position(const lv_obj_t *obj) const {
  if (obj == getdispt()->act_scr) return QPoint(0, 0);
  int x = lv_obj_get_x(obj);
  int y = lv_obj_get_y(obj);
  lv_obj_t *parent = lv_obj_get_parent(obj);
  while (parent) {
    if (parent == getdispt()->act_scr) break;
    x += lv_obj_get_x(parent);
    y += lv_obj_get_y(parent);
    parent = lv_obj_get_parent(parent);
  }
  return QPoint(x, y);
}

QSize LVGLCore::get_object_size(const lv_obj_t *lv_obj) const {
  return QSize(lv_obj_get_width(lv_obj), lv_obj_get_height(lv_obj));
}

QRect LVGLCore::get_object_rect(const lv_obj_t *lv_obj) const {
  return QRect(get_absolute_position(lv_obj), get_object_size(lv_obj));
}

void LVGLCore::addObject(LVGLObject *object) { m_objects << object; }

void LVGLCore::removeObject(LVGLObject *object) {
  for (LVGLObject *c : object->childs()) removeObject(c);
  if (object->parent()) object->parent()->removeChild(object);
  m_objects.removeOne(object);
  delete object;
}

void LVGLCore::removeAllObjects() {
  for (LVGLObject *c : m_objects) {
    if (c->parent() == nullptr) removeObject(c);
  }
}

QList<LVGLObject *> LVGLCore::allObjects() const { return m_objects; }

QList<LVGLObject *> LVGLCore::topLevelObjects() const {
  QList<LVGLObject *> ret;
  for (LVGLObject *c : m_objects) {
    if (c->parent() == nullptr) ret << c;
  }
  return ret;
}

QList<LVGLObject *> LVGLCore::objectsByType(QString className) const {
  QList<LVGLObject *> ret;
  for (LVGLObject *c : m_objects) {
    if (c->widgetClass()->className() == className) ret << c;
  }
  return ret;
}

LVGLObject *LVGLCore::object(QString name) const {
  if (name.isEmpty()) return nullptr;
  for (LVGLObject *c : m_objects) {
    if (c->name() == name) return c;
  }
  return nullptr;
}

LVGLObject *LVGLCore::object(lv_obj_t *obj) const {
  if (obj == nullptr) return nullptr;
  for (LVGLObject *c : m_objects) {
    if (c->obj() == obj) return c;
  }
  return nullptr;
}

QColor LVGLCore::toColor(lv_color_t c) const {
#if LV_COLOR_DEPTH == 24
  return QColor(c.ch.red, c.ch.green, c.ch.blue);
#elif LV_COLOR_DEPTH == 32
  return QColor(c.ch.red, c.ch.green, c.ch.blue, c.ch.alpha);
#endif
}

lv_color_t LVGLCore::fromColor(QColor c) const {
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

lv_color_t LVGLCore::fromColor(QVariant v) const {
  if (v.type() == QVariant::Map) {
    QVariantMap map = v.toMap();
#if LV_COLOR_DEPTH == 32
    lv_color_t c;
    c.ch.red = map["red"].toInt() & 0xff;
    c.ch.green = map["green"].toInt() & 0xff;
    c.ch.blue = map["blue"].toInt() & 0xff;
    c.ch.alpha = map["alpha"].toInt() & 0xff;
    return c;
#endif
  }
  return fromColor(v.value<QColor>());
}

QJsonObject LVGLCore::colorToJson(lv_color_t c) const {
  QJsonObject color(
      {{"red", c.ch.red}, {"green", c.ch.green}, {"blue", c.ch.blue}});
#if LV_COLOR_DEPTH == 32
  color.insert("alpha", c.ch.alpha);
#endif
  return color;
}

lv_color_t LVGLCore::colorFromJson(QJsonObject obj) const {
#if LV_COLOR_DEPTH == 32
  lv_color_t c;
  c.ch.red = obj["red"].toInt() & 0xff;
  c.ch.green = obj["green"].toInt() & 0xff;
  c.ch.blue = obj["blue"].toInt() & 0xff;
  c.ch.alpha = obj["alpha"].toInt() & 0xff;
  return c;
#else
  return lv_color_hex(0x000000);
#endif
}

LVGLFontData *LVGLCore::addFont(const QString &fileName, uint8_t size) {
  LVGLFontData *font = LVGLFontData::parse(fileName, size, 4, 0x0020, 0x007f);
  if (font) m_fonts << font;
  return font;
}

void LVGLCore::addFont(LVGLFontData *font) {
  if (font) m_fonts << font;
}

bool LVGLCore::removeFont(LVGLFontData *font) {
  return m_fonts.removeOne(font);
}

QStringList LVGLCore::fontNames() const {
  QStringList ret;
  for (const LVGLFontData *f : m_fonts) ret << f->name();
  return ret;
}

QStringList LVGLCore::fontCodeNames() const {
  QStringList ret;
  for (const LVGLFontData *f : m_fonts) ret << f->codeName();
  return ret;
}

const lv_font_t *LVGLCore::font(int index) const {
  if ((index > 0) && (index < m_fonts.size())) return m_fonts.at(index)->font();
  return m_defaultFont->font();
}

const lv_font_t *LVGLCore::font(const QString &name,
                                Qt::CaseSensitivity cs) const {
  for (const LVGLFontData *font : m_fonts) {
    if (name.compare(font->name(), cs) == 0) return font->font();
  }
  return m_defaultFont->font();
}

int LVGLCore::indexOfFont(const lv_font_t *font) const {
  int index = 0;
  for (auto it = m_fonts.begin(); it != m_fonts.end(); ++it, ++index) {
    if ((*it)->font() == font) return index;
  }
  return -1;
}

QString LVGLCore::fontName(const lv_font_t *font) const {
  for (const LVGLFontData *f : m_fonts) {
    if (f->font() == font) return f->name();
  }
  return m_defaultFont->name();
}

QString LVGLCore::fontCodeName(const lv_font_t *font) const {
  for (const LVGLFontData *f : m_fonts) {
    if (f->font() == font) return f->codeName();
  }
  return m_defaultFont->codeName();
}

QList<const LVGLFontData *> LVGLCore::customFonts() const {
  QList<const LVGLFontData *> ret;
  for (const LVGLFontData *font : m_fonts) {
    if (font->isCustomFont()) ret << font;
  }
  return ret;
}

void LVGLCore::removeCustomFonts() {
  for (auto it = m_fonts.begin(); it != m_fonts.end();) {
    if ((*it)->isCustomFont()) {
      delete *it;
      it = m_fonts.erase(it);
    } else {
      ++it;
    }
  }
}

QString LVGLCore::baseStyleName(const lv_style_t *style) const {
  if (style == &lv_style_scr) return "lv_style_scr";
  return "";
}

void LVGLCore::setScreenColor(QColor color) {
  lv_style_set_bg_color(&m_screenStyle, LV_STATE_DEFAULT, fromColor(color));
  lv_style_set_bg_grad_color(&m_screenStyle, LV_STATE_DEFAULT,
                             fromColor(color));
  lv_obj_add_style(getdispt()->act_scr, 0, &m_screenStyle);
}

QColor LVGLCore::screenColor() const {
  lv_color_t res =
      _lv_obj_get_style_color(getdispt()->act_scr, 0, LV_STYLE_BG_COLOR);
  return toColor(res);
}

bool LVGLCore::screenColorChanged() const {
  lv_color_t res =
      _lv_obj_get_style_color(getdispt()->act_scr, 0, LV_STYLE_BG_COLOR);
  QColor c = toColor(res);
  if (0xff == c.red() && 0xff == c.blue() && 0xff == c.green()) return false;
  return true;
}

QList<const LVGLWidget *> LVGLCore::widgets() const {
  return m_widgets.values();
}

QList<const LVGLWidget *> LVGLCore::widgetsDisplayW() const {
  return m_widgetsDisplayW.values();
}

QList<const LVGLWidget *> LVGLCore::widgetsInputW() const {
  return m_widgetsInputW.values();
}

const LVGLWidget *LVGLCore::widget(const QString &name) const {
  if (m_widgets.contains(name)) return m_widgets[name];
  if (m_widgetsDisplayW.contains(name)) return m_widgetsDisplayW[name];
  if (m_widgetsInputW.contains(name)) return m_widgetsInputW[name];
  return nullptr;
}

void LVGLCore::tick() {
  lv_task_handler();
  lv_tick_inc(20);
}

void LVGLCore::flushHandler(lv_disp_drv_t *disp, const lv_area_t *area,
                            lv_color_t *color_p) {
  const auto stride = disp->hor_res;
  for (auto y = area->y1; y <= area->y2; ++y) {
    for (auto x = area->x1; x <= area->x2; ++x) {
      m_dispFrameBuf[x + y * stride].full = color_p->full;
      color_p++;
    }
  }
  lv_disp_flush_ready(disp);
}

bool LVGLCore::inputHandler(lv_indev_drv_t *indev_driver,
                            lv_indev_data_t *data) {
  (void)indev_driver;

  data->state = m_inputData.state;
  data->point.x = m_inputData.point.x;
  data->point.y = m_inputData.point.y;

  return false; /*Return `false` because we are not buffering and no more data
                   to read*/
}

void LVGLCore::flushCb(lv_disp_drv_t *disp, const lv_area_t *area,
                       lv_color_t *color_p) {
  LVGLCore *self = reinterpret_cast<LVGLCore *>(disp->user_data);
  self->flushHandler(disp, area, color_p);
}

bool LVGLCore::inputCb(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
  LVGLCore *self = reinterpret_cast<LVGLCore *>(indev_driver->user_data);
  return self->inputHandler(indev_driver, data);
}

void LVGLCore::logCb(lv_log_level_t level, const char *file, uint32_t line,
                     const char *dsc) {
  qDebug().nospace() << file << " (" << line << "," << level << "): " << dsc;
}

// mix
bool radius(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part, lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_RADIUS | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_RADIUS | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool clip_corner(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                 lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_CLIP_CORNER | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_CLIP_CORNER | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool mix_size(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
              lv_state_t state) {
  auto a = _lv_obj_get_style_int(s1, part,
                                 LV_STYLE_SIZE | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(s2, part,
                                 LV_STYLE_SIZE | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool transform_width(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                     lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_TRANSFORM_WIDTH | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_TRANSFORM_WIDTH | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool transform_height(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                      lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_TRANSFORM_HEIGHT | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_TRANSFORM_HEIGHT | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool transform_angle(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                     lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_TRANSFORM_ANGLE | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_TRANSFORM_ANGLE | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool transform_zoom(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                    lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_TRANSFORM_ZOOM | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_TRANSFORM_ZOOM | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool opa_scale(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
               lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_OPA_SCALE | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_OPA_SCALE | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

// padding
bool pad_top(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part, lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_PAD_TOP | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_PAD_TOP | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool pad_bottom(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_PAD_BOTTOM | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_PAD_BOTTOM | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool pad_left(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
              lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_PAD_LEFT | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_PAD_LEFT | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool pad_right(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
               lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_PAD_RIGHT | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_PAD_RIGHT | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool pad_inner(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
               lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_PAD_INNER | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_PAD_INNER | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

// margin
bool margin_top(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_MARGIN_TOP | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_MARGIN_TOP | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool margin_bottom(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                   lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_MARGIN_BOTTOM | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_MARGIN_BOTTOM | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool margin_left(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                 lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_MARGIN_LEFT | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_MARGIN_LEFT | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool margin_right(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                  lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_MARGIN_RIGHT | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_MARGIN_RIGHT | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

// background
bool bg_color(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
              lv_state_t state) {
  auto a = _lv_obj_get_style_color(
      s1, part, LV_STYLE_BG_COLOR | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_color(
      s2, part, LV_STYLE_BG_COLOR | (state << LV_STYLE_STATE_POS));
  return (a.full == b.full);
}

bool bg_opa(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part, lv_state_t state) {
  auto a = _lv_obj_get_style_opa(
      s1, part, LV_STYLE_BG_OPA | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_opa(
      s2, part, LV_STYLE_BG_OPA | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool bg_grad_color(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                   lv_state_t state) {
  auto a = _lv_obj_get_style_color(
      s1, part, LV_STYLE_BG_GRAD_COLOR | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_color(
      s2, part, LV_STYLE_BG_GRAD_COLOR | (state << LV_STYLE_STATE_POS));
  return (a.full == b.full);
}

bool bg_main_stop(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                  lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_BG_MAIN_STOP | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_BG_MAIN_STOP | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool bg_grad_stop(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                  lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_BG_GRAD_STOP | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_BG_GRAD_STOP | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool bg_grad_dir(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                 lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_BG_GRAD_DIR | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_BG_GRAD_DIR | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool bg_blend_mode(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                   lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_BG_BLEND_MODE | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_BG_BLEND_MODE | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

// border
bool border_color(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                  lv_state_t state) {
  auto a = _lv_obj_get_style_color(
      s1, part, LV_STYLE_BORDER_COLOR | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_color(
      s2, part, LV_STYLE_BORDER_COLOR | (state << LV_STYLE_STATE_POS));
  return (a.full == b.full);
}

bool border_opa(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                lv_state_t state) {
  auto a = _lv_obj_get_style_opa(
      s1, part, LV_STYLE_BG_OPA | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_opa(
      s2, part, LV_STYLE_BG_OPA | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool border_width(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                  lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_BORDER_WIDTH | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_BORDER_WIDTH | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool border_side(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                 lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_BORDER_SIDE | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_BORDER_SIDE | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool border_post(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                 lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_BORDER_POST | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_BORDER_POST | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool border_blend_mode(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                       lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_BORDER_BLEND_MODE | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_BORDER_BLEND_MODE | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

// outline
bool outline_color(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                   lv_state_t state) {
  auto a = _lv_obj_get_style_color(
      s1, part, LV_STYLE_OUTLINE_COLOR | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_color(
      s2, part, LV_STYLE_OUTLINE_COLOR | (state << LV_STYLE_STATE_POS));
  return (a.full == b.full);
}

bool outline_opa(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                 lv_state_t state) {
  auto a = _lv_obj_get_style_opa(
      s1, part, LV_STYLE_OUTLINE_OPA | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_opa(
      s2, part, LV_STYLE_OUTLINE_OPA | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool outline_width(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                   lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_OUTLINE_WIDTH | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_OUTLINE_WIDTH | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool outline_pad(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                 lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_OUTLINE_PAD | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_OUTLINE_PAD | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool outline_blend_mode(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                        lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_OUTLINE_BLEND_MODE | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_OUTLINE_BLEND_MODE | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

// shadow
bool shadow_color(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                  lv_state_t state) {
  auto a = _lv_obj_get_style_color(
      s1, part, LV_STYLE_SHADOW_COLOR | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_color(
      s2, part, LV_STYLE_SHADOW_COLOR | (state << LV_STYLE_STATE_POS));
  return (a.full == b.full);
}

bool shadow_opa(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                lv_state_t state) {
  auto a = _lv_obj_get_style_opa(
      s1, part, LV_STYLE_SHADOW_OPA | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_opa(
      s2, part, LV_STYLE_SHADOW_OPA | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool shadow_width(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                  lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_SHADOW_WIDTH | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_SHADOW_WIDTH | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool shadow_ofs_x(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                  lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_SHADOW_OFS_X | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_SHADOW_OFS_X | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool shadow_ofs_y(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                  lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_SHADOW_OFS_Y | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_SHADOW_OFS_Y | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool shadow_spread(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                   lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_SHADOW_SPREAD | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_SHADOW_SPREAD | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool shadow_blend_mode(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                       lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_SHADOW_BLEND_MODE | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_SHADOW_BLEND_MODE | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

// patter
bool pattern_image(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                   lv_state_t state) {
  auto a = _lv_obj_get_style_ptr(
      s1, part, LV_STYLE_PATTERN_IMAGE | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_ptr(
      s2, part, LV_STYLE_PATTERN_IMAGE | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool pattern_opa(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                 lv_state_t state) {
  auto a = _lv_obj_get_style_opa(
      s1, part, LV_STYLE_PATTERN_OPA | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_opa(
      s1, part, LV_STYLE_PATTERN_OPA | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool pattern_recolor(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                     lv_state_t state) {
  auto a = _lv_obj_get_style_color(
      s1, part, LV_STYLE_PATTERN_RECOLOR | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_color(
      s2, part, LV_STYLE_PATTERN_RECOLOR | (state << LV_STYLE_STATE_POS));
  return (a.full == b.full);
}

bool pattern_recolor_opa(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                         lv_state_t state) {
  auto a = _lv_obj_get_style_opa(
      s1, part, LV_STYLE_PATTERN_RECOLOR_OPA | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_opa(
      s2, part, LV_STYLE_PATTERN_RECOLOR_OPA | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool pattern_repeat(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                    lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_PATTERN_REPEAT | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_PATTERN_REPEAT | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool pattern_blend_mode(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                        lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_PATTERN_BLEND_MODE | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_PATTERN_BLEND_MODE | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

// value
bool value_str(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
               lv_state_t state) {
  auto a = _lv_obj_get_style_ptr(
      s1, part, LV_STYLE_VALUE_STR | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_ptr(
      s2, part, LV_STYLE_VALUE_STR | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool value_color(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                 lv_state_t state) {
  auto a = _lv_obj_get_style_color(
      s1, part, LV_STYLE_VALUE_COLOR | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_color(
      s2, part, LV_STYLE_VALUE_COLOR | (state << LV_STYLE_STATE_POS));
  return (a.full == b.full);
}

bool value_opa(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
               lv_state_t state) {
  auto a = _lv_obj_get_style_opa(
      s1, part, LV_STYLE_VALUE_OPA | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_opa(
      s2, part, LV_STYLE_VALUE_OPA | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool value_font(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                lv_state_t state) {
  auto a = _lv_obj_get_style_ptr(
      s1, part, LV_STYLE_VALUE_FONT | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_ptr(
      s2, part, LV_STYLE_VALUE_FONT | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool value_letter_space(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                        lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_VALUE_LETTER_SPACE | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_VALUE_LETTER_SPACE | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool value_line_space(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                      lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_VALUE_LINE_SPACE | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_VALUE_LINE_SPACE | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool value_align(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                 lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_VALUE_ALIGN | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_VALUE_ALIGN | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool value_ofs_x(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                 lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_VALUE_OFS_X | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_VALUE_OFS_X | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool value_ofs_y(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                 lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_VALUE_OFS_Y | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_VALUE_OFS_Y | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool value_blend_mode(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                      lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_VALUE_BLEND_MODE | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_VALUE_BLEND_MODE | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

// text
bool text_color(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                lv_state_t state) {
  auto a = _lv_obj_get_style_color(
      s1, part, LV_STYLE_TEXT_COLOR | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_color(
      s2, part, LV_STYLE_TEXT_COLOR | (state << LV_STYLE_STATE_POS));
  return (a.full == b.full);
}

bool text_opa(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
              lv_state_t state) {
  auto a = _lv_obj_get_style_opa(
      s1, part, LV_STYLE_TEXT_OPA | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_opa(
      s2, part, LV_STYLE_TEXT_OPA | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool text_font(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
               lv_state_t state) {
  auto a = _lv_obj_get_style_ptr(
      s1, part, LV_STYLE_TEXT_FONT | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_ptr(
      s2, part, LV_STYLE_TEXT_FONT | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool text_letter_space(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                       lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_TEXT_LETTER_SPACE | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_TEXT_LETTER_SPACE | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool text_line_space(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                     lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_TEXT_LINE_SPACE | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_TEXT_LINE_SPACE | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool text_decor(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_TEXT_DECOR | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_TEXT_DECOR | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool text_sel_color(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                    lv_state_t state) {
  auto a = _lv_obj_get_style_color(
      s1, part, LV_STYLE_TEXT_SEL_COLOR | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_color(
      s2, part, LV_STYLE_TEXT_SEL_COLOR | (state << LV_STYLE_STATE_POS));
  return (a.full == b.full);
}

bool text_sel_bg_color(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                       lv_state_t state) {
  auto a = _lv_obj_get_style_color(
      s1, part, LV_STYLE_TEXT_SEL_BG_COLOR | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_color(
      s2, part, LV_STYLE_TEXT_SEL_BG_COLOR | (state << LV_STYLE_STATE_POS));
  return (a.full == b.full);
}

bool text_blend_mode(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                     lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_TEXT_BLEND_MODE | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_TEXT_BLEND_MODE | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

// line
bool line_color(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                lv_state_t state) {
  auto a = _lv_obj_get_style_color(
      s1, part, LV_STYLE_LINE_COLOR | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_color(
      s2, part, LV_STYLE_LINE_COLOR | (state << LV_STYLE_STATE_POS));
  return (a.full == b.full);
}

bool line_opa(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
              lv_state_t state) {
  auto a = _lv_obj_get_style_opa(
      s1, part, LV_STYLE_LINE_OPA | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_opa(
      s2, part, LV_STYLE_LINE_OPA | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool line_width(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_LINE_WIDTH | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_LINE_WIDTH | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool line_dash_width(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                     lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_LINE_DASH_WIDTH | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_LINE_DASH_WIDTH | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool line_dash_gap(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                   lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_LINE_DASH_GAP | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_LINE_DASH_GAP | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool line_rounded(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                  lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_LINE_ROUNDED | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_LINE_ROUNDED | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool line_blend_mode(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                     lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_LINE_BLEND_MODE | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_LINE_BLEND_MODE | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

// image
bool image_recolor(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                   lv_state_t state) {
  auto a = _lv_obj_get_style_color(
      s1, part, LV_STYLE_IMAGE_RECOLOR | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_color(
      s2, part, LV_STYLE_IMAGE_RECOLOR | (state << LV_STYLE_STATE_POS));
  return (a.full == b.full);
}

bool image_recolor_opa(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                       lv_state_t state) {
  auto a = _lv_obj_get_style_opa(
      s1, part, LV_STYLE_IMAGE_RECOLOR_OPA | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_opa(
      s2, part, LV_STYLE_IMAGE_RECOLOR_OPA | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool image_opa(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
               lv_state_t state) {
  auto a = _lv_obj_get_style_opa(
      s1, part, LV_STYLE_IMAGE_OPA | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_opa(
      s2, part, LV_STYLE_IMAGE_OPA | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool image_blend_mode(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                      lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_IMAGE_BLEND_MODE | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_IMAGE_BLEND_MODE | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

// transition
bool transition_time(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                     lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_TRANSITION_TIME | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_TRANSITION_TIME | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool transition_delay(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                      lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_TRANSITION_DELAY | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_TRANSITION_DELAY | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool transition_prop_1(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                       lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_TRANSITION_PROP_1 | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_TRANSITION_PROP_1 | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool transition_prop_2(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                       lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_TRANSITION_PROP_2 | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_TRANSITION_PROP_2 | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool transition_prop_3(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                       lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_TRANSITION_PROP_3 | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_TRANSITION_PROP_3 | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool transition_prop_4(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                       lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_TRANSITION_PROP_4 | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_TRANSITION_PROP_4 | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool transition_prop_5(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                       lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_TRANSITION_PROP_5 | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_TRANSITION_PROP_5 | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool transition_prop_6(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                       lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_TRANSITION_PROP_6 | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_TRANSITION_PROP_6 | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

// scale
bool scale_grad_color(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                      lv_state_t state) {
  auto a = _lv_obj_get_style_color(
      s1, part, LV_STYLE_SCALE_GRAD_COLOR | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_color(
      s2, part, LV_STYLE_SCALE_GRAD_COLOR | (state << LV_STYLE_STATE_POS));
  return (a.full == b.full);
}

bool scale_end_color(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                     lv_state_t state) {
  auto a = _lv_obj_get_style_color(
      s1, part, LV_STYLE_SCALE_END_COLOR | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_color(
      s2, part, LV_STYLE_SCALE_END_COLOR | (state << LV_STYLE_STATE_POS));
  return (a.full == b.full);
}

bool scale_width(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                 lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_SCALE_WIDTH | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_SCALE_WIDTH | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool scale_border_width(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                        lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_SCALE_BORDER_WIDTH | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_SCALE_BORDER_WIDTH | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool scale_end_border_width(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                            lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_SCALE_BORDER_WIDTH | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_SCALE_BORDER_WIDTH | (state << LV_STYLE_STATE_POS));
  return (a == b);
}

bool scale_end_line_width(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                          lv_state_t state) {
  auto a = _lv_obj_get_style_int(
      s1, part, LV_STYLE_SCALE_END_LINE_WIDTH | (state << LV_STYLE_STATE_POS));
  auto b = _lv_obj_get_style_int(
      s2, part, LV_STYLE_SCALE_END_LINE_WIDTH | (state << LV_STYLE_STATE_POS));
  return (a == b);
}
