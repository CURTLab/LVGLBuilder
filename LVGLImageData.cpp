#include "LVGLImageData.h"

#include <QDebug>
#include <QFile>
#include <QJsonObject>
#include <QPainter>
#include <QTextStream>

#include "LVGLCore.h"

LVGLImageData::LVGLImageData()
    : m_size(0), m_data(nullptr), m_colorFormat(LV_COLOR_32Bit) {}

LVGLImageData::LVGLImageData(QImage image, QString fileName, QString name)
    : m_size(static_cast<uint32_t>(image.width() * image.height() * 4)),
      m_data(new uint8_t[m_size]),
      m_name(name),
      m_fileName(fileName),
      m_colorFormat(LV_COLOR_32Bit) {
  QImage img = image.convertToFormat(QImage::Format_ARGB32);
  memcpy(m_data, img.bits(), m_size);

  m_img_dsc.header.always_zero = 0;
  m_img_dsc.header.w = static_cast<uint32_t>(image.width());
  m_img_dsc.header.h = static_cast<uint32_t>(image.height());
  m_img_dsc.header.cf = LV_IMG_CF_TRUE_COLOR_ALPHA;
  m_img_dsc.data = m_data;
  m_img_dsc.data_size = m_size;

  QPixmap icon(ICON_SIZE, ICON_SIZE);
  icon.fill(Qt::transparent);

  QRect rect(img.rect());
  QRect devRect(0, 0, icon.width(), icon.height());
  rect.moveCenter(devRect.center());

  QPainter p;
  p.begin(&icon);
  p.drawImage(rect.topLeft(), img);
  p.end();

  m_icon = QIcon(icon);
}

LVGLImageData::LVGLImageData(QString fileName, QString name)
    : m_size(0),
      m_data(nullptr),
      m_name(name),
      m_fileName(fileName),
      m_colorFormat(LV_COLOR_32Bit) {
  if (!QFile(fileName).exists()) return;

  memset(&m_img_dsc, 0, sizeof(m_img_dsc));

  QImage img = QImage(fileName).convertToFormat(QImage::Format_ARGB32);

  m_size = static_cast<uint32_t>(img.width() * img.height() * 4);
  m_data = new uint8_t[m_size];

  memcpy(m_data, img.bits(), m_size);

  m_img_dsc.header.always_zero = 0;
  m_img_dsc.header.w = static_cast<uint32_t>(img.width());
  m_img_dsc.header.h = static_cast<uint32_t>(img.height());
  m_img_dsc.header.cf = LV_IMG_CF_TRUE_COLOR_ALPHA;
  m_img_dsc.data = m_data;
  m_img_dsc.data_size = m_size;

  QPixmap icon(ICON_SIZE, ICON_SIZE);
  icon.fill(Qt::transparent);

  QRect rect(img.rect());
  QRect devRect(0, 0, icon.width(), icon.height());
  rect.moveCenter(devRect.center());

  QPainter p;
  p.begin(&icon);
  p.drawImage(rect.topLeft(), img);
  p.end();

  m_icon = QIcon(icon);
}

LVGLImageData::LVGLImageData(QJsonObject object)
    : LVGLImageData(object["fileName"].toString(), object["name"].toString()) {
  if (object.contains("colorFormat")) {
    QString color = object["colorFormat"].toString();
    int index = colorFormats().indexOf(color);
    if (index >= 0) m_colorFormat = static_cast<ColorFormat>(index);
  }
}

LVGLImageData::~LVGLImageData() { delete[] m_data; }

bool LVGLImageData::isValid() {
  return (m_size > 0) && (m_data != nullptr) && (m_img_dsc.header.w > 0) &&
         (m_img_dsc.header.h > 0);
}

lv_img_dsc_t *LVGLImageData::img_des() { return &m_img_dsc; }

QString LVGLImageData::name() const { return m_name; }

QJsonValue LVGLImageData::toJson() const {
  QJsonObject obj(
      {{"fileName", m_fileName},
       {"name", m_name},
       {"colorFormat", colorFormats().at(static_cast<int>(m_colorFormat))}});
  return obj;
}

QString LVGLImageData::fileName() const { return m_fileName; }

QIcon LVGLImageData::icon() const { return m_icon; }

int LVGLImageData::width() const { return m_img_dsc.header.w; }

int LVGLImageData::height() const { return m_img_dsc.header.h; }

static QString hex(uint8_t h) {
  QString t = QString::number(h, 16);
  return "0x" + (t.size() < 2 ? '0' + t : t);
}

QString LVGLImageData::codeName() const {
  return m_name.toLower().replace(' ', '_').replace("-", "_");
}

LVGLImageData::ColorFormat LVGLImageData::colorFormat() const {
  return m_colorFormat;
}

void LVGLImageData::setColorFormat(const ColorFormat &colorFormat) {
  m_colorFormat = colorFormat;
}

QStringList LVGLImageData::colorFormats() {
  return QStringList() << "Mono"
                       << "RGB332"
                       << "RGB565"
                       << "RGB888"
                       << "RGBA";
}

bool LVGLImageData::saveAsCode(const QString &fileName) const {
  QFile file(fileName);
  if (!file.open(QIODevice::WriteOnly)) return false;
  uint8_t *d = m_data;
  QTextStream stream(&file);
  stream << "#include \"lvgl/lvgl.h\"\n";

  size_t bytes = 0;
  if (m_colorFormat == LV_COLOR_32Bit)
    bytes = 4;
  else if (m_colorFormat == LV_COLOR_24Bit)
    bytes = 3;
  else if (m_colorFormat == LV_COLOR_16Bit)
    bytes = 2;
  else if ((m_colorFormat == LV_COLOR_8Bit) || (m_colorFormat == LV_COLOR_1Bit))
    bytes = 1;

  stream << "#ifndef LV_ATTRIBUTE_MEM_ALIGN\n";
  stream << "#define LV_ATTRIBUTE_MEM_ALIGN\n";
  stream << "#endif\n\n";

  stream << "#ifndef LV_ATTRIBUTE_IMG_" << codeName().toUpper() << "\n";
  stream << "#define LV_ATTRIBUTE_IMG_" << codeName().toUpper() << "\n";
  stream << "#endif\n\n";

  stream << "const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_IMG_"
         << codeName().toUpper() << " uint8_t " << codeName() << "_map[] = {\n";
  stream << "\t/*Pixel format: Red: 8 bit, Green: 8 bit, Blue: 8 bit*/\n";
  for (uint32_t y = 0; y < m_img_dsc.header.h; ++y) {
    stream << '\t';
    for (uint32_t x = 0; x < m_img_dsc.header.w; ++x) {
      if ((m_colorFormat == LV_COLOR_32Bit) ||
          (m_colorFormat == LV_COLOR_24Bit)) {
        stream << hex(d[0]) << ',';
        stream << hex(d[1]) << ',';
        stream << hex(d[2]) << ',';
        if (m_colorFormat == LV_COLOR_32Bit) stream << hex(d[3]) << ',';
      } else if (m_colorFormat == LV_COLOR_16Bit) {
        const uint16_t c = lv_color_to16(lv_color_make(d[0], d[1], d[2]));
        stream << hex(c & 0xff) << ',';
        stream << hex((c >> 8) & 0xff) << ',';
      } else if (m_colorFormat == LV_COLOR_8Bit) {
        const uint8_t c = lv_color_to8(lv_color_make(d[0], d[1], d[2]));
        stream << hex(c) << ',';
      } else if (m_colorFormat == LV_COLOR_1Bit) {
        const uint8_t c = lv_color_to1(lv_color_make(d[0], d[1], d[2]));
        stream << hex(c) << ',';
      }
      d += 4;
    }
    stream << '\n';
  }

  stream << "};\n\n";
  stream << "const lv_img_dsc_t " << codeName() << " = {\n";
  stream << "  .header.always_zero = 0,\n";
  stream << "  .header.w = " << m_img_dsc.header.w << ",\n";
  stream << "  .header.h = " << m_img_dsc.header.h << ",\n";
  stream << "  .data_size = "
         << (static_cast<size_t>(m_img_dsc.header.w) * m_img_dsc.header.h * 3)
         << ",\n";
  if (m_colorFormat == LV_COLOR_32Bit)
    stream << "  .header.cf = LV_IMG_CF_TRUE_COLOR_ALPHA,\n";
  else
    stream << "  .header.cf = LV_IMG_CF_TRUE_COLOR,\n";
  stream << "  .data = " << codeName() << "_map\n";
  stream << "};\n";

  file.close();

  return true;
}

bool LVGLImageData::saveAsBin(const QString &fileName) const {
  if ((m_colorFormat != LV_COLOR_32Bit) && (m_colorFormat != LV_COLOR_24Bit))
    return false;

  QFile file(fileName);
  if (!file.open(QIODevice::WriteOnly)) return false;
  lv_img_header_t header;
  memset(&header, 0, sizeof(lv_img_header_t));
  header.w = m_img_dsc.header.w;
  header.h = m_img_dsc.header.h;
  header.cf = (m_colorFormat == LV_COLOR_32Bit ? LV_IMG_CF_TRUE_COLOR_ALPHA
                                               : LV_IMG_CF_TRUE_COLOR);

  file.write(reinterpret_cast<const char *>(&header), sizeof(lv_img_header_t));

  if (m_colorFormat == LV_COLOR_32Bit) {
    file.write(reinterpret_cast<const char *>(m_img_dsc.data),
               m_img_dsc.data_size);
  } else if (m_colorFormat == LV_COLOR_24Bit) {
    union conv_32_to_24 {
      uint32_t rgba;
      struct {
        uint8_t rgb[3];
        uint8_t a;
      } b24;
    };

    conv_32_to_24 *ptr = reinterpret_cast<conv_32_to_24 *>(m_data);
    const uint32_t n = m_img_dsc.header.h * m_img_dsc.header.w;
    for (uint32_t i = 0; i < n; ++i, ++ptr)
      file.write(reinterpret_cast<const char *>(ptr->b24.rgb), 3);
  }

  file.close();
  return true;
}
