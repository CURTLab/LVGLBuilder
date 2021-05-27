#ifndef LVGLCORE_HPP
#define LVGLCORE_HPP

#include <lvgl/lvgl.h>

#include <QElapsedTimer>
#include <QHash>
#include <QList>
#include <QObject>
#include <QPixmap>
#include <QTimer>

#include "LVGLImageData.h"
#include "widgets/LVGLWidget.h"

class LVGLObject;
class LVGLFontData;

class LVGLCore : public QObject {
  Q_OBJECT
 public:
  LVGLCore(QObject *parent = nullptr);
  ~LVGLCore();

  void init(int width, int height);
  void initfont();

  void initw(const QHash<QString, const LVGLWidget *> &w) { m_widgets = w; }
  void initwDP(const QHash<QString, const LVGLWidget *> &w) {
    m_widgetsDisplayW = w;
  }
  void initwIP(const QHash<QString, const LVGLWidget *> &w) {
    m_widgetsInputW = w;
  }

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
  LVGLObject *findObjByName(const QString &objname);

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
  QHash<QString, LVGLImageData *> allImages() const;
  QList<LVGLFontData *> allFonts() const;

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
  const lv_font_t *font(const QString &name,
                        Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
  int indexOfFont(const lv_font_t *font) const;
  QString fontName(const lv_font_t *font) const;
  QString fontCodeName(const lv_font_t *font) const;
  QList<const LVGLFontData *> customFonts() const;
  void removeCustomFonts();

  void setScreenColor(QColor color);
  QColor screenColor() const;
  QColor screenColor(lv_obj_t *scr) const;
  bool screenColorChanged() const;

  QList<const LVGLWidget *> widgets() const;
  QList<const LVGLWidget *> widgetsDisplayW() const;
  QList<const LVGLWidget *> widgetsInputW() const;
  const LVGLWidget *widget(const QString &name) const;

  static const char *DEFAULT_DAYS[7];
  static const char *DEFAULT_MONTHS[12];
  static const lv_state_t LVGL_STATE[7];
  static const char *LVGL_STATE_STR[7];

  void objsclear();
  void setAllObjects(QList<LVGLObject *> objs);
  void setAllImages(QHash<QString, LVGLImageData *> imgs);
  void setAllFonts(QList<LVGLFontData *> fonts);

 private slots:
  void tick();

 private:
  QPixmap getPix(int type);
  void flushHandler(lv_disp_drv_t *disp, const lv_area_t *area,
                    lv_color_t *color_p);
  bool inputHandler(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);

  static void flushCb(lv_disp_drv_t *disp, const lv_area_t *area,
                      lv_color_t *color_p);
  static bool inputCb(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);
  static void logCb(lv_log_level_t level, const char *file, uint32_t line,
                    const char *dsc);

  QTimer m_timer;
  QElapsedTimer m_time;
  QHash<QString, LVGLImageData *> m_images;
  QHash<QString, const LVGLWidget *> m_widgets;
  QHash<QString, const LVGLWidget *> m_widgetsDisplayW;
  QHash<QString, const LVGLWidget *> m_widgetsInputW;
  LVGLImageData *m_default;
  QList<LVGLObject *> m_objects;
  QList<LVGLFontData *> m_fonts;
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

// mix
bool radius(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part, lv_state_t state);
bool clip_corner(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                 lv_state_t state);
bool mix_size(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part, lv_state_t state);
bool transform_width(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                     lv_state_t state);
bool transform_height(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                      lv_state_t state);
bool transform_angle(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                     lv_state_t state);
bool transform_zoom(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                    lv_state_t state);
bool opa_scale(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
               lv_state_t state);
// padding
bool pad_top(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part, lv_state_t state);
bool pad_bottom(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                lv_state_t state);
bool pad_left(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part, lv_state_t state);
bool pad_right(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
               lv_state_t state);
bool pad_inner(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
               lv_state_t state);
// margin
bool margin_top(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                lv_state_t state);
bool margin_bottom(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                   lv_state_t state);
bool margin_left(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                 lv_state_t state);
bool margin_right(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                  lv_state_t state);
// background
bool bg_color(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part, lv_state_t state);
bool bg_opa(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part, lv_state_t state);
bool bg_grad_color(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                   lv_state_t state);
bool bg_main_stop(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                  lv_state_t state);
bool bg_grad_stop(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                  lv_state_t state);
bool bg_grad_dir(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                 lv_state_t state);
bool bg_blend_mode(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                   lv_state_t state);
// border
bool border_color(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                  lv_state_t state);
bool border_opa(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                lv_state_t state);
bool border_width(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                  lv_state_t state);
bool border_side(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                 lv_state_t state);
bool border_post(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                 lv_state_t state);
bool border_blend_mode(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                       lv_state_t state);
// outline
bool outline_color(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                   lv_state_t state);
bool outline_opa(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                 lv_state_t state);
bool outline_width(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                   lv_state_t state);
bool outline_pad(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                 lv_state_t state);
bool outline_blend_mode(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                        lv_state_t state);
// shadow
bool shadow_color(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                  lv_state_t state);
bool shadow_opa(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                lv_state_t state);
bool shadow_width(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                  lv_state_t state);
bool shadow_ofs_x(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                  lv_state_t state);
bool shadow_ofs_y(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                  lv_state_t state);
bool shadow_spread(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                   lv_state_t state);
bool shadow_blend_mode(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                       lv_state_t state);
// patter
bool pattern_image(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                   lv_state_t state);
bool pattern_opa(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                 lv_state_t state);
bool pattern_recolor(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                     lv_state_t state);
bool pattern_recolor_opa(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                         lv_state_t state);
bool pattern_repeat(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                    lv_state_t state);
bool pattern_blend_mode(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                        lv_state_t state);
// value
bool value_str(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
               lv_state_t state);
bool value_color(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                 lv_state_t state);
bool value_opa(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
               lv_state_t state);
bool value_font(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                lv_state_t state);
bool value_letter_space(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                        lv_state_t state);
bool value_line_space(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                      lv_state_t state);
bool value_align(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                 lv_state_t state);
bool value_ofs_x(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                 lv_state_t state);
bool value_ofs_y(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                 lv_state_t state);
bool value_blend_mode(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                      lv_state_t state);
// text
bool text_color(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                lv_state_t state);
bool text_opa(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part, lv_state_t state);
bool text_font(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
               lv_state_t state);
bool text_letter_space(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                       lv_state_t state);
bool text_line_space(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                     lv_state_t state);
bool text_decor(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                lv_state_t state);
bool text_sel_color(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                    lv_state_t state);
bool text_sel_bg_color(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                       lv_state_t state);
bool text_blend_mode(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                     lv_state_t state);
// line
bool line_color(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                lv_state_t state);
bool line_opa(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part, lv_state_t state);
bool line_width(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                lv_state_t state);
bool line_dash_width(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                     lv_state_t state);
bool line_dash_gap(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                   lv_state_t state);
bool line_rounded(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                  lv_state_t state);
bool line_blend_mode(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                     lv_state_t state);
// image
bool image_recolor(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                   lv_state_t state);
bool image_recolor_opa(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                       lv_state_t state);
bool image_opa(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
               lv_state_t state);
bool image_blend_mode(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                      lv_state_t state);
// transition
bool transition_time(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                     lv_state_t state);
bool transition_delay(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                      lv_state_t state);
bool transition_prop_1(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                       lv_state_t state);
bool transition_prop_2(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                       lv_state_t state);
bool transition_prop_3(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                       lv_state_t state);
bool transition_prop_4(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                       lv_state_t state);
bool transition_prop_5(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                       lv_state_t state);
bool transition_prop_6(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                       lv_state_t state);
// scale
bool scale_grad_color(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                      lv_state_t state);
bool scale_end_color(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                     lv_state_t state);
bool scale_width(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                 lv_state_t state);
bool scale_border_width(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                        lv_state_t state);
bool scale_end_border_width(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                            lv_state_t state);
bool scale_end_line_width(lv_obj_t *s1, lv_obj_t *s2, lv_obj_part_t part,
                          lv_state_t state);
#endif  // LVGLCORE_HPP
