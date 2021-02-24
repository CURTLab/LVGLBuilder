#include "LVGLObject.h"

#include <QDebug>
#include <QJsonArray>
#include <QUuid>

#include "LVGLCore.h"

LVGLObject::LVGLObject(const LVGLWidget *widgetClass, QString name,
                       LVGLObject *parent)
    : m_obj(widgetClass->newObject(parent->obj())),
      m_widgetClass(widgetClass),
      m_name(name),
      m_locked(false),
      m_accessible(false),
      m_movable(true),
      m_index(-1),
      m_parent(parent) {
  if (parent) parent->m_childs.append(this);
  if (m_name.isEmpty()) generateName();
}

LVGLObject::LVGLObject(const LVGLWidget *widgetClass, QString name,
                       lv_obj_t *parent)
    : m_obj(widgetClass->newObject(parent)),
      m_widgetClass(widgetClass),
      m_name(name),
      m_locked(false),
      m_accessible(false),
      m_movable(true),
      m_index(-1),
      m_parent(nullptr) {
  if (m_name.isEmpty()) generateName();
}

LVGLObject::LVGLObject(const LVGLWidget *widgetClass, LVGLObject *parent,
                       lv_obj_t *parentObj)
    : m_obj(widgetClass->newObject(parentObj)),
      m_widgetClass(widgetClass),
      m_locked(false),
      m_accessible(false),
      m_movable(true),
      m_index(-1),
      m_parent(parent) {
  generateName();
}

LVGLObject::LVGLObject(lv_obj_t *obj, const LVGLWidget *widgetClass,
                       LVGLObject *parent, bool movable, int index)
    : m_obj(obj),
      m_widgetClass(widgetClass),
      m_locked(false),
      m_accessible(false),
      m_movable(movable),
      m_index(index),
      m_parent(parent) {
  Q_ASSERT(m_widgetClass);
  if (parent) parent->m_childs.append(this);
  generateName();
}

LVGLObject::~LVGLObject() {
  lv_obj_del(m_obj);
  for (auto s : m_liststyles) delete s;
}

lv_obj_t *LVGLObject::obj() const { return m_obj; }

QString LVGLObject::name() const { return m_name; }

void LVGLObject::setName(const QString &name) {
  if (doesNameExists(name, this))
    generateName();
  else
    m_name = name;
}

QString LVGLObject::codeName() const {
  return name().toLower().replace(" ", "_");
}

bool LVGLObject::isLocked() const { return m_locked; }

void LVGLObject::setLocked(bool locked) { m_locked = locked; }

bool LVGLObject::isMovable() const { return m_movable; }

const LVGLWidget *LVGLObject::widgetClass() const { return m_widgetClass; }

LVGLWidget::Type LVGLObject::widgetType() const {
  return m_widgetClass->type();
}

QPoint LVGLObject::absolutePosition() const {
  // if (lv_obj_get_parent(m_obj) == lv_scr_act())
  //	return QPoint(m_obj->coords.x1, m_obj->coords.x2);
  int x = lv_obj_get_x(m_obj);
  int y = lv_obj_get_y(m_obj);
  lv_obj_t *parent = lv_obj_get_parent(m_obj);
  while (parent) {
    if (parent == lvgl->getdispt()->act_scr) break;
    x += lv_obj_get_x(parent);
    y += lv_obj_get_y(parent);
    parent = lv_obj_get_parent(parent);
  }
  return QPoint(x, y);
}

QPoint LVGLObject::position() const {
  return QPoint(lv_obj_get_x(m_obj), lv_obj_get_y(m_obj));
}

int LVGLObject::x() const { return lv_obj_get_x(m_obj); }

int LVGLObject::y() const { return lv_obj_get_y(m_obj); }

void LVGLObject::setPosition(const QPoint &pos) {
  if (pos != position()) {
    lv_obj_set_pos(m_obj, static_cast<lv_coord_t>(pos.x()),
                   static_cast<lv_coord_t>(pos.y()));
    emit positionChanged();
  }
}

void LVGLObject::setX(int x) {
  if (x != lv_obj_get_x(m_obj)) {
    lv_obj_set_x(m_obj, static_cast<lv_coord_t>(x));
    emit positionChanged();
  }
}

void LVGLObject::setY(int y) {
  if (y != lv_obj_get_y(m_obj)) {
    lv_obj_set_y(m_obj, static_cast<lv_coord_t>(y));
    emit positionChanged();
  }
}

QSize LVGLObject::size() const {
  return QSize(lv_obj_get_width(m_obj), lv_obj_get_height(m_obj));
}

QRect LVGLObject::geometry() const { return QRect(absolutePosition(), size()); }

void LVGLObject::setGeometry(QRect geometry) {
  setPosition(geometry.topLeft());
  lv_obj_set_size(m_obj, static_cast<lv_coord_t>(geometry.width()),
                  static_cast<lv_coord_t>(geometry.height()));
}

int LVGLObject::width() const { return lv_obj_get_width(m_obj); }

int LVGLObject::height() const { return lv_obj_get_height(m_obj); }

void LVGLObject::setWidth(int width) {
  lv_obj_set_width(m_obj, static_cast<lv_coord_t>(width));
}

void LVGLObject::setHeight(int height) {
  lv_obj_set_height(m_obj, static_cast<lv_coord_t>(height));
}

QJsonObject LVGLObject::toJson() {
  QJsonObject object({{"widget", m_widgetClass->name()},
                      {"class", m_widgetClass->className()},
                      {"name", m_name}});
  if (m_index > -1) object.insert("index", m_index);
  for (LVGLProperty *p : m_widgetClass->properties()) {
    QJsonValue v = p->toJson(this);
    if (!v.isNull()) object.insert(p->name().toLower(), v);
  }

  QJsonArray styles = jsonStyles();
  if (!m_styles.isEmpty() && (styles.size() > 0))
    object.insert("styles", styles);
  if (!m_childs.isEmpty()) {
    QJsonArray childs;
    for (LVGLObject *c : m_childs) childs.append(c->toJson());
    object.insert("children", childs);
  }
  return object;
}

QJsonArray LVGLObject::jsonStyles() const {
  QJsonArray styles;
  for (auto it = m_styles.begin(); it != m_styles.end(); ++it) {
    const lv_style_t &objStyle = it.value();
    const lv_style_t *defaultStyle = m_widgetClass->style(m_obj);

    const LVGL::StyleParts editableParts =
        m_widgetClass->editableStyles(it.key());
    if (defaultStyle &&
        !LVGLStyle::hasStyleChanged(&objStyle, defaultStyle, editableParts))
      continue;

    QJsonObject style;
    style.insert("type", m_widgetClass->styles().at(it.key()));

    QString baseStyle = lvgl->baseStyleName(defaultStyle);
    if (!baseStyle.isEmpty()) style.insert("base", baseStyle);

    //    if (editableParts & LVGL::Background) {
    //      QJsonObject body;
    //      if (!defaultStyle || (axs_style_equal_bg_color(defaultStyle,
    //      &objStyle)))
    //        body.insert("main_color",
    //                    QVariant(lvgl->toColor(axs_lv_style_get_bg_color(&objStyle)))
    //                        .toString());
    //      if (!defaultStyle ||
    //          (axs_style_equal_bg_grad_color(defaultStyle, &objStyle)))
    //        body.insert(
    //            "grad_color",
    //            QVariant(lvgl->toColor(axs_lv_style_get_bg_grad_color(&objStyle)))
    //                .toString());
    //      if (!defaultStyle || (axs_style_equal_radius(defaultStyle,
    //      &objStyle)))
    //        body.insert("radius",
    //                    static_cast<int>(axs_lv_style_get_radius(&objStyle)));
    //      if (!defaultStyle || (axs_style_equal_bg_opa(defaultStyle,
    //      &objStyle)))
    //        body.insert("opa", axs_lv_style_get_bg_opa(&objStyle));

    //      if (editableParts & LVGL::Border) {
    //        QJsonObject border;
    //        if (!defaultStyle ||
    //            (axs_style_equal_border_color(defaultStyle, &objStyle)))
    //          border.insert(
    //              "color",
    //              QVariant(lvgl->toColor(axs_lv_style_get_border_color(&objStyle)))
    //                  .toString());
    //        if (!defaultStyle ||
    //            (axs_style_equal_border_width(defaultStyle, &objStyle)))
    //          border.insert("width",
    //          axs_lv_style_get_border_width(&objStyle));
    //        if (!defaultStyle ||
    //            (axs_style_equal_border_side(defaultStyle, &objStyle)))
    //          border.insert("part", axs_lv_style_get_border_side(&objStyle));
    //        if (!defaultStyle ||
    //            (axs_style_equal_border_opa(defaultStyle, &objStyle)))
    //          border.insert("opa", axs_lv_style_get_bg_opa(&objStyle));
    //        if (!border.isEmpty()) body.insert("border", border);
    //      }

    //      if (editableParts & LVGL::Shadow) {
    //        QJsonObject shadow;
    //        if (!defaultStyle ||
    //            (axs_style_equal_shadow_color(defaultStyle, &objStyle)))
    //          shadow.insert(
    //              "color",
    //              QVariant(lvgl->toColor(axs_lv_style_get_shadow_color(&objStyle)))
    //                  .toString());
    //        if (!defaultStyle ||
    //            (axs_style_equal_shadow_width(defaultStyle, &objStyle)))
    //          shadow.insert("width",
    //          axs_lv_style_get_shadow_width(&objStyle));
    //        if (!shadow.isEmpty()) body.insert("shadow", shadow);
    //      }

    //      if (editableParts & LVGL::Padding) {
    //        QJsonObject padding;
    //        if (!defaultStyle || (axs_style_equal_pad_top(defaultStyle,
    //        &objStyle)))
    //          padding.insert("top", axs_lv_style_get_pad_top(&objStyle));
    //        if (!defaultStyle ||
    //            (axs_style_equal_pad_bottom(defaultStyle, &objStyle)))
    //          padding.insert("bottom",
    //          axs_lv_style_get_pad_bottom(&objStyle));
    //        if (!defaultStyle ||
    //            (axs_style_equal_pad_left(defaultStyle, &objStyle)))
    //          padding.insert("left", axs_lv_style_get_pad_left(&objStyle));
    //        if (!defaultStyle ||
    //            (axs_style_equal_pad_right(defaultStyle, &objStyle)))
    //          padding.insert("right", axs_lv_style_get_pad_right(&objStyle));
    //        if (!defaultStyle ||
    //            (axs_style_equal_pad_inner(defaultStyle, &objStyle)))
    //          padding.insert("inner", axs_lv_style_get_pad_inner(&objStyle));
    //        if (!padding.isEmpty()) body.insert("padding", padding);
    //      }
    //      if (!body.isEmpty()) style.insert("background", body);
    //    }

    //    if (editableParts & LVGL::Text) {
    //      QJsonObject text;
    //      if (!defaultStyle ||
    //          (axs_style_equal_text_color(defaultStyle, &objStyle)))
    //        text.insert(
    //            "color",
    //            QVariant(lvgl->toColor(axs_lv_style_get_text_color(&objStyle)))
    //                .toString());
    //      if (!defaultStyle ||
    //          (axs_style_equal_text_sel_color(defaultStyle, &objStyle)))
    //        text.insert(
    //            "sel_color",
    //            QVariant(lvgl->toColor(axs_lv_style_get_text_sel_color(&objStyle)))
    //                .toString());
    //      if (!defaultStyle ||
    //          (axs_style_equal_text_font(defaultStyle, &objStyle))) {
    //        text.insert("font",
    //                    lvgl->fontName(axs_lv_style_get_text_font(&objStyle)));
    //      }
    //      if (!defaultStyle ||
    //          (axs_style_equal_text_letter_space(defaultStyle, &objStyle)))
    //        text.insert("letter_space",
    //                    axs_lv_style_get_text_letter_space(&objStyle));
    //      if (!defaultStyle ||
    //          (axs_style_equal_text_line_space(defaultStyle, &objStyle)))
    //        text.insert("line_space",
    //        axs_lv_style_get_text_line_space(&objStyle));
    //      if (!defaultStyle || (axs_style_equal_text_opa(defaultStyle,
    //      &objStyle)))
    //        text.insert("opa", axs_lv_style_get_text_opa(&objStyle));
    //      if (!text.isEmpty()) style.insert("text", text);
    //    }

    //    if (editableParts & LVGL::Image) {
    //      QJsonObject image;
    //      if (!defaultStyle ||
    //          (axs_style_equal_image_recolor(defaultStyle, &objStyle)))
    //        image.insert(
    //            "recolor",
    //            QVariant(lvgl->toColor(axs_lv_style_get_image_recolor(&objStyle)))
    //                .toString());
    //      if (!defaultStyle || (axs_style_equal_image_opa(defaultStyle,
    //      &objStyle)))
    //        image.insert("opa", axs_lv_style_get_image_opa(&objStyle));
    //      if (!image.isEmpty()) style.insert("image", image);
    //    }

    //    if (editableParts & LVGL::Line) {
    //      QJsonObject line;
    //      if (!defaultStyle ||
    //          (axs_style_equal_line_color(defaultStyle, &objStyle)))
    //        line.insert(
    //            "color",
    //            QVariant(lvgl->toColor(axs_lv_style_get_line_color(&objStyle)))
    //                .toString());
    //      if (!defaultStyle ||
    //          (axs_style_equal_line_width(defaultStyle, &objStyle)))
    //        line.insert("width", axs_lv_style_get_line_width(&objStyle));
    //      if (!defaultStyle || (axs_style_equal_line_opa(defaultStyle,
    //      &objStyle)))
    //        line.insert("opa", axs_lv_style_get_line_opa(&objStyle));
    //      if (!line.isEmpty()) style.insert("line", line);
    //    }

    if (style.size() > 0) styles.append(style);
  }
  return styles;
}

QStringList LVGLObject::codeStyle(QString styleVar, int type) const {
  QStringList ret;
  if (!hasCustomStyle(type)) return ret;

  const lv_style_t &objStyle = m_styles[type];
  const lv_style_t *defaultStyle = m_widgetClass->style(m_obj);

  const LVGL::StyleParts editableParts = m_widgetClass->editableStyles(type);
  if (defaultStyle &&
      !LVGLStyle::hasStyleChanged(&objStyle, defaultStyle, editableParts))
    return ret;

  //  if (editableParts & LVGL::Background) {
  //    if (!defaultStyle || (axs_style_equal_bg_color(defaultStyle,
  //    &objStyle))) {
  //      QString color =
  //          QVariant(lvgl->toColor(axs_lv_style_get_bg_color(&objStyle)))
  //              .toString()
  //              .replace("#", "0x");
  //      ret << styleVar + ".body.main_color = lv_color_hex(" + color + ");";
  //    }
  //    if (!defaultStyle ||
  //        (axs_style_equal_bg_grad_color(defaultStyle, &objStyle))) {
  //      QString color =
  //          QVariant(lvgl->toColor(axs_lv_style_get_bg_grad_color(&objStyle)))
  //              .toString()
  //              .replace("#", "0x");
  //      ret << styleVar + ".body.grad_color = lv_color_hex(" + color + ");";
  //    }
  //    if (!defaultStyle || (axs_style_equal_radius(defaultStyle, &objStyle)))
  //      ret << styleVar + ".body.radius = " +
  //                 QString::number(axs_lv_style_get_radius(&objStyle)) + ";";
  //    if (!defaultStyle || (axs_style_equal_bg_opa(defaultStyle, &objStyle)))
  //      ret << styleVar + ".body.opa = " +
  //                 QString::number(axs_lv_style_get_bg_opa(&objStyle)) + ";";

  //    if (editableParts & LVGL::Border) {
  //      if (!defaultStyle ||
  //          (axs_style_equal_border_color(defaultStyle, &objStyle))) {
  //        QString color =
  //            QVariant(lvgl->toColor(axs_lv_style_get_border_color(&objStyle)))
  //                .toString()
  //                .replace("#", "0x");
  //        ret << styleVar + ".body.border.color = lv_color_hex(" + color +
  //        ");";
  //      }
  //      if (!defaultStyle ||
  //          (axs_style_equal_border_width(defaultStyle, &objStyle)))
  //        ret << styleVar + ".body.border.width = " +
  //                   QString::number(axs_lv_style_get_border_width(&objStyle))
  //                   +
  //                   ";";
  //      if (!defaultStyle ||
  //          (axs_style_equal_border_side(defaultStyle, &objStyle)))
  //        ret << styleVar + ".body.border.part = " +
  //                   QString::number(axs_lv_style_get_border_side(&objStyle))
  //                   +
  //                   ";";
  //      if (!defaultStyle ||
  //          (axs_style_equal_border_opa(defaultStyle, &objStyle)))
  //        ret << styleVar + ".body.border.opa = " +
  //                   QString::number(axs_lv_style_get_bg_opa(&objStyle)) +
  //                   ";";
  //    }

  //    if (editableParts & LVGL::Shadow) {
  //      QJsonObject shadow;
  //      if (!defaultStyle ||
  //          (axs_style_equal_shadow_color(defaultStyle, &objStyle))) {
  //        QString color =
  //            QVariant(lvgl->toColor(axs_lv_style_get_shadow_color(&objStyle)))
  //                .toString()
  //                .replace("#", "0x");
  //        ret << styleVar + ".body.shadow.color = lv_color_hex(" + color +
  //        ");";
  //      }
  //      if (!defaultStyle ||
  //          (axs_style_equal_shadow_width(defaultStyle, &objStyle)))
  //        ret << styleVar + ".body.shadow.width = " +
  //                   QString::number(axs_lv_style_get_shadow_width(&objStyle))
  //                   +
  //                   ";";
  //    }

  //    if (editableParts & LVGL::Padding) {
  //      QJsonObject padding;
  //      if (!defaultStyle || (axs_style_equal_pad_top(defaultStyle,
  //      &objStyle)))
  //        ret << styleVar + ".body.padding.top = " +
  //                   QString::number(axs_lv_style_get_pad_top(&objStyle)) +
  //                   ";";
  //      if (!defaultStyle ||
  //          (axs_style_equal_pad_bottom(defaultStyle, &objStyle)))
  //        ret << styleVar + ".body.padding.bottom = " +
  //                   QString::number(axs_lv_style_get_pad_bottom(&objStyle)) +
  //                   ";";
  //      if (!defaultStyle || (axs_style_equal_pad_left(defaultStyle,
  //      &objStyle)))
  //        ret << styleVar + ".body.padding.left = " +
  //                   QString::number(axs_lv_style_get_pad_left(&objStyle)) +
  //                   ";";
  //      if (!defaultStyle || (axs_style_equal_pad_right(defaultStyle,
  //      &objStyle)))
  //        ret << styleVar + ".body.padding.right = " +
  //                   QString::number(axs_lv_style_get_pad_right(&objStyle)) +
  //                   ";";
  //      if (!defaultStyle || (axs_style_equal_pad_inner(defaultStyle,
  //      &objStyle)))
  //        ret << styleVar + ".body.padding.inner = " +
  //                   QString::number(axs_lv_style_get_pad_inner(&objStyle)) +
  //                   ";";
  //    }
  //  }

  //  if (editableParts & LVGL::Text) {
  //    if (!defaultStyle ||
  //        (axs_style_equal_text_color(defaultStyle, &objStyle))) {
  //      QString color =
  //          QVariant(lvgl->toColor(axs_lv_style_get_text_color(&objStyle)))
  //              .toString()
  //              .replace("#", "0x");
  //      ret << styleVar + ".text.color = lv_color_hex(" + color + ");";
  //    }
  //    if (!defaultStyle ||
  //        (axs_style_equal_text_sel_color(defaultStyle, &objStyle))) {
  //      QString color =
  //          QVariant(lvgl->toColor(axs_lv_style_get_text_sel_color(&objStyle)))
  //              .toString()
  //              .replace("#", "0x");
  //      ret << styleVar + ".text.sel_color = lv_color_hex(" + color + ");";
  //    }
  //    if (!defaultStyle || (axs_style_equal_text_font(defaultStyle,
  //    &objStyle))) {
  //      ret << styleVar + ".text.font = &" +
  //                 lvgl->fontCodeName(axs_lv_style_get_text_font(&objStyle)) +
  //                 ";";
  //    }
  //    if (!defaultStyle ||
  //        (axs_style_equal_text_letter_space(defaultStyle, &objStyle)))
  //      ret << styleVar + ".text.letter_space = " +
  //                 QString::number(
  //                     axs_lv_style_get_text_letter_space(&objStyle)) +
  //                 ";";
  //    if (!defaultStyle ||
  //        (axs_style_equal_text_line_space(defaultStyle, &objStyle)))
  //      ret << styleVar + ".text.line_space = " +
  //                 QString::number(axs_lv_style_get_text_line_space(&objStyle))
  //                 +
  //                 ";";
  //    if (!defaultStyle || (axs_style_equal_text_opa(defaultStyle,
  //    &objStyle)))
  //      ret << styleVar + ".text.opa = " +
  //                 QString::number(axs_lv_style_get_text_opa(&objStyle)) +
  //                 ";";
  //  }

  //  if (editableParts & LVGL::Image) {
  //    if (!defaultStyle ||
  //        (axs_style_equal_image_recolor(defaultStyle, &objStyle))) {
  //      QString color =
  //          QVariant(lvgl->toColor(axs_lv_style_get_image_recolor(&objStyle)))
  //              .toString()
  //              .replace("#", "0x");
  //      ret << styleVar + ".image.recolor = lv_color_hex(" + color + ");";
  //    }
  //    if (!defaultStyle || (axs_style_equal_image_opa(defaultStyle,
  //    &objStyle)))
  //      ret << styleVar + ".image.opa = " +
  //                 QString::number(axs_lv_style_get_image_opa(&objStyle)) +
  //                 ";";
  //  }

  //  if (editableParts & LVGL::Line) {
  //    if (!defaultStyle ||
  //        (axs_style_equal_line_color(defaultStyle, &objStyle))) {
  //      QString color =
  //          QVariant(lvgl->toColor(axs_lv_style_get_line_color(&objStyle)))
  //              .toString()
  //              .replace("#", "0x");
  //      ret << styleVar + ".line.color = lv_color_hex(" + color + ");";
  //    }
  //    if (!defaultStyle || (axs_style_equal_line_width(defaultStyle,
  //    &objStyle)))
  //      ret << styleVar + ".line.width = " +
  //                 QString::number(axs_lv_style_get_line_width(&objStyle)) +
  //                 ";";
  //    if (!defaultStyle || (axs_style_equal_line_opa(defaultStyle,
  //    &objStyle)))
  //      ret << styleVar + ".line.opa = " +
  //                 QString::number(axs_lv_style_get_line_opa(&objStyle)) +
  //                 ";";
  //  }

  return ret;
}

QStringList LVGLObject::codeStyle(QString styleVar, lv_obj_t *obj1,
                                  lv_obj_t *obj2, int partindex,
                                  int stateindex) {
  QStringList ret;
  const LVGL::StyleParts editableParts =
      m_widgetClass->editableStyles(partindex);
  auto part = m_widgetClass->parts().at(partindex);
  auto state = LVGLCore::LVGL_STATE[stateindex];
  auto s1 = obj1;

  if (editableParts & LVGL::Mix) {
    // mix
    if (!radius(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          obj1, part, LV_STYLE_RADIUS | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_radius(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!clip_corner(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          obj1, part, LV_STYLE_CLIP_CORNER | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_clip_corner(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!mix_size(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          obj1, part, LV_STYLE_SIZE | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_size(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!transform_width(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_TRANSFORM_WIDTH | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_transform_width(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!transform_height(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_TRANSFORM_HEIGHT | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_transform_height(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!transform_angle(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_TRANSFORM_ANGLE | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_transform_angle(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!transform_zoom(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_TRANSFORM_ZOOM | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_transform_zoom(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!opa_scale(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_OPA_SCALE | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_opa_scale(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
  }
  if (editableParts & LVGL::Padding) {
    // padding
    if (!pad_top(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_PAD_TOP | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_pad_top(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!pad_bottom(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_PAD_BOTTOM | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_pad_bottom(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!pad_left(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_PAD_LEFT | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_pad_left(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!pad_right(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_PAD_RIGHT | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_pad_right(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!pad_inner(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_PAD_INNER | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_pad_inner(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
  }
  if (editableParts & LVGL::Margin) {
    // margin
    if (!margin_top(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_MARGIN_TOP | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_margin_top(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!margin_bottom(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_MARGIN_BOTTOM | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_margin_bottom(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!margin_left(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_MARGIN_LEFT | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_margin_left(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!margin_right(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_MARGIN_RIGHT | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_margin_right(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
  }
  if (editableParts & LVGL::Background) {
    // background
    if (!bg_color(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = QVariant(lvgl->toColor(_lv_obj_get_style_color(
                            s1, part,
                            LV_STYLE_BG_COLOR | (state << LV_STYLE_STATE_POS))))
                   .toString()
                   .replace("#", "0x");

      ret << ("lv_style_set_bg_color(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + ",lv_color_hex(" + c +
              "));");
    }
    if (!bg_opa(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_opa(
          s1, part, LV_STYLE_BG_OPA | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_bg_opa(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!bg_grad_color(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c =
          QVariant(lvgl->toColor(_lv_obj_get_style_color(
                       s1, part,
                       LV_STYLE_BG_GRAD_COLOR | (state << LV_STYLE_STATE_POS))))
              .toString()
              .replace("#", "0x");

      ret << ("lv_style_set_bg_grad_color(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + ",lv_color_hex(" + c +
              "));");
    }
    if (!bg_main_stop(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_BG_MAIN_STOP | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_bg_main_stop(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!bg_grad_stop(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_BG_GRAD_STOP | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_bg_grad_stop(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!bg_grad_dir(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_BG_GRAD_DIR | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_bg_grad_dir(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!bg_blend_mode(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_BG_BLEND_MODE | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_bg_blend_mode(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
  }
  if (editableParts & LVGL::Border) {
    // border
    if (!border_color(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c =
          QVariant(lvgl->toColor(_lv_obj_get_style_color(
                       s1, part,
                       LV_STYLE_BORDER_COLOR | (state << LV_STYLE_STATE_POS))))
              .toString()
              .replace("#", "0x");

      ret << ("lv_style_set_border_color(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + ",lv_color_hex(" + c +
              "));");
    }
    if (!border_opa(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_opa(
          s1, part, LV_STYLE_BG_OPA | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_border_opa(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!border_width(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_BORDER_WIDTH | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_border_width(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!border_side(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_BORDER_SIDE | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_border_side(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!border_post(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_BORDER_POST | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_border_post(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!border_blend_mode(obj1, obj2, part,
                           LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_BORDER_BLEND_MODE | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_border_blend_mode(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
  }
  if (editableParts & LVGL::Outline) {
    // outline
    if (!outline_color(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c =
          QVariant(lvgl->toColor(_lv_obj_get_style_color(
                       s1, part,
                       LV_STYLE_OUTLINE_COLOR | (state << LV_STYLE_STATE_POS))))
              .toString()
              .replace("#", "0x");

      ret << ("lv_style_set_outline_color(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + ",lv_color_hex(" + c +
              "));");
    }
    if (!outline_opa(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_opa(
          s1, part, LV_STYLE_OUTLINE_OPA | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_outline_opa(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!outline_width(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_OUTLINE_WIDTH | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_outline_width(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!outline_pad(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_OUTLINE_PAD | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_outline_pad(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!outline_blend_mode(obj1, obj2, part,
                            LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part,
          LV_STYLE_OUTLINE_BLEND_MODE | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_outline_blend_mode(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
  }
  if (editableParts & LVGL::Shadow) {
    // shadow
    if (!shadow_color(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c =
          QVariant(lvgl->toColor(_lv_obj_get_style_color(
                       s1, part,
                       LV_STYLE_SHADOW_COLOR | (state << LV_STYLE_STATE_POS))))
              .toString()
              .replace("#", "0x");

      ret << ("lv_style_set_shadow_color(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + ",lv_color_hex(" + c +
              "));");
    }
    if (!shadow_opa(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_opa(
          s1, part, LV_STYLE_SHADOW_OPA | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_shadow_opa(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!shadow_width(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_SHADOW_WIDTH | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_shadow_width(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!shadow_ofs_x(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_SHADOW_OFS_X | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_shadow_ofs_x(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!shadow_ofs_y(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_SHADOW_OFS_Y | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_shadow_ofs_y(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!shadow_spread(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_SHADOW_SPREAD | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_shadow_spread(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!shadow_blend_mode(obj1, obj2, part,
                           LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_SHADOW_BLEND_MODE | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_shadow_blend_mode(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
  }
  if (editableParts & LVGL::Pattern) {
    // patter
    if (!pattern_image(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
    }
    if (!pattern_opa(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_opa(
          s1, part, LV_STYLE_PATTERN_OPA | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_pattern_opa(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!pattern_recolor(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = QVariant(lvgl->toColor(_lv_obj_get_style_color(
                            s1, part,
                            LV_STYLE_PATTERN_RECOLOR |
                                (state << LV_STYLE_STATE_POS))))
                   .toString()
                   .replace("#", "0x");

      ret << ("lv_style_set_pattern_recolor(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + ",lv_color_hex(" + c +
              "));");
    }
    if (!pattern_recolor_opa(obj1, obj2, part,
                             LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_opa(
          s1, part,
          LV_STYLE_PATTERN_RECOLOR_OPA | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_pattern_recolor_opa(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!pattern_repeat(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_PATTERN_REPEAT | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_pattern_repeat(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!pattern_blend_mode(obj1, obj2, part,
                            LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part,
          LV_STYLE_PATTERN_BLEND_MODE | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_pattern_blend_mode(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
  }
  if (editableParts & LVGL::Value) {
    // value
    if (!value_str(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = (const char *)_lv_obj_get_style_ptr(
          s1, part, LV_STYLE_VALUE_STR | (state << LV_STYLE_STATE_POS));
      QString strname =
          "str_" + QString(QUuid::createUuid().toString()).mid(1, 7);
      QString s = "static char* " + strname + " = " + c + ";\n";
      ret << s;
      ret << ("lv_style_set_value_value_str(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + strname + ");");
    }
    if (!value_color(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c =
          QVariant(lvgl->toColor(_lv_obj_get_style_color(
                       s1, part,
                       LV_STYLE_VALUE_COLOR | (state << LV_STYLE_STATE_POS))))
              .toString()
              .replace("#", "0x");

      ret << ("lv_style_set_value_color(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + ",lv_color_hex(" + c +
              "));");
    }
    if (!value_opa(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_opa(
          s1, part, LV_STYLE_VALUE_OPA | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_value_opa(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!value_font(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = lvgl->fontCodeName((lv_font_t *)_lv_obj_get_style_ptr(
          s1, part, LV_STYLE_VALUE_FONT | (state << LV_STYLE_STATE_POS)));
      ret << ("lv_style_set_value_font(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + ",&" + c + ");");
    }
    if (!value_letter_space(obj1, obj2, part,
                            LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part,
          LV_STYLE_VALUE_LETTER_SPACE | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_value_letter_space(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!value_line_space(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_VALUE_LINE_SPACE | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_value_line_space(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!value_align(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_VALUE_ALIGN | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_value_align(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!value_ofs_x(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_VALUE_OFS_X | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_value_ofs_x(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!value_ofs_y(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_VALUE_OFS_Y | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_value_ofs_y(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!value_blend_mode(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_VALUE_BLEND_MODE | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_value_blend_mode(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
  }
  if (editableParts & LVGL::Text) {
    // text
    if (!text_color(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c =
          QVariant(lvgl->toColor(_lv_obj_get_style_color(
                       s1, part,
                       LV_STYLE_TEXT_COLOR | (state << LV_STYLE_STATE_POS))))
              .toString()
              .replace("#", "0x");

      ret << ("lv_style_set_text_color(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + ",lv_color_hex(" + c +
              "));");
    }
    if (!text_opa(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_opa(
          s1, part, LV_STYLE_TEXT_OPA | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_text_opa(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!text_font(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = lvgl->fontCodeName((lv_font_t *)_lv_obj_get_style_ptr(
          s1, part, LV_STYLE_TEXT_FONT | (state << LV_STYLE_STATE_POS)));
      ret << ("lv_style_set_text_font(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + ",&" + c + ");");
    }
    if (!text_letter_space(obj1, obj2, part,
                           LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_TEXT_LETTER_SPACE | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_text_letter_space(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!text_line_space(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_TEXT_LINE_SPACE | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_text_line_space(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!text_decor(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_TEXT_DECOR | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_text_decor(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!text_sel_color(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = QVariant(lvgl->toColor(_lv_obj_get_style_color(
                            s1, part,
                            LV_STYLE_TEXT_SEL_COLOR |
                                (state << LV_STYLE_STATE_POS))))
                   .toString()
                   .replace("#", "0x");

      ret << ("lv_style_set_text_sel_color(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + ",lv_color_hex(" + c +
              "));");
    }
    if (!text_sel_bg_color(obj1, obj2, part,
                           LVGLCore::LVGL_STATE[stateindex])) {
      auto c = QVariant(lvgl->toColor(_lv_obj_get_style_color(
                            s1, part,
                            LV_STYLE_TEXT_SEL_BG_COLOR |
                                (state << LV_STYLE_STATE_POS))))
                   .toString()
                   .replace("#", "0x");

      ret << ("lv_style_set_text_sel_bg_color(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + ",lv_color_hex(" + c +
              "));");
    }
    if (!text_blend_mode(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_TEXT_BLEND_MODE | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_text_blend_mode(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
  }
  if (editableParts & LVGL::Line) {
    // line
    if (!line_color(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c =
          QVariant(lvgl->toColor(_lv_obj_get_style_color(
                       s1, part,
                       LV_STYLE_LINE_COLOR | (state << LV_STYLE_STATE_POS))))
              .toString()
              .replace("#", "0x");

      ret << ("lv_style_set_line_color(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + ",lv_color_hex(" + c +
              "));");
    }
    if (!line_opa(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_opa(
          s1, part, LV_STYLE_LINE_OPA | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_line_opa(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!line_width(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_LINE_WIDTH | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_line_width(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!line_dash_width(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_LINE_DASH_WIDTH | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_line_dash_width(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!line_dash_gap(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_LINE_DASH_GAP | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_line_dash_gap(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!line_rounded(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_LINE_ROUNDED | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_line_rounded(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!line_blend_mode(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_LINE_BLEND_MODE | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_line_blend_mode(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
  }
  if (editableParts & LVGL::Image) {
    // image
    if (!image_recolor(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c =
          QVariant(lvgl->toColor(_lv_obj_get_style_color(
                       s1, part,
                       LV_STYLE_IMAGE_RECOLOR | (state << LV_STYLE_STATE_POS))))
              .toString()
              .replace("#", "0x");

      ret << ("lv_style_set_image_recolor(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + ",lv_color_hex(" + c +
              "));");
    }
    if (!image_recolor_opa(obj1, obj2, part,
                           LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_opa(
          s1, part, LV_STYLE_IMAGE_RECOLOR_OPA | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_image_recolor_opa(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!image_opa(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_opa(
          s1, part, LV_STYLE_IMAGE_OPA | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_image_opa(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!image_blend_mode(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_IMAGE_BLEND_MODE | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_image_blend_mode(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
  }
  if (editableParts & LVGL::Transition) {
    // transition
    if (!transition_time(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_TRANSITION_TIME | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_transition_time(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!transition_delay(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_TRANSITION_DELAY | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_transition_delay(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!transition_prop_1(obj1, obj2, part,
                           LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_TRANSITION_PROP_1 | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_transition_prop_1(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!transition_prop_2(obj1, obj2, part,
                           LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_TRANSITION_PROP_2 | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_transition_prop_2(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!transition_prop_3(obj1, obj2, part,
                           LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_TRANSITION_PROP_3 | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_transition_prop_3(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!transition_prop_4(obj1, obj2, part,
                           LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_TRANSITION_PROP_4 | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_transition_prop_4(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!transition_prop_5(obj1, obj2, part,
                           LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_TRANSITION_PROP_5 | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_transition_prop_5(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!transition_prop_6(obj1, obj2, part,
                           LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_TRANSITION_PROP_6 | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_transition_prop_6(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
  }
  if (editableParts & LVGL::Scale) {
    // scale
    if (!scale_grad_color(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = QVariant(lvgl->toColor(_lv_obj_get_style_color(
                            s1, part,
                            LV_STYLE_SCALE_GRAD_COLOR |
                                (state << LV_STYLE_STATE_POS))))
                   .toString()
                   .replace("#", "0x");

      ret << ("lv_style_set_scale_grad_color(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + ",lv_color_hex(" + c +
              "));");
    }
    if (!scale_end_color(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = QVariant(lvgl->toColor(_lv_obj_get_style_color(
                            s1, part,
                            LV_STYLE_SCALE_END_COLOR |
                                (state << LV_STYLE_STATE_POS))))
                   .toString()
                   .replace("#", "0x");

      ret << ("lv_style_set_scale_end_color(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + ",lv_color_hex(" + c +
              "));");
    }
    if (!scale_width(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part, LV_STYLE_SCALE_WIDTH | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_scale_width(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!scale_border_width(obj1, obj2, part,
                            LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part,
          LV_STYLE_SCALE_BORDER_WIDTH | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_scale_border_width(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
    if (!scale_end_border_width(obj1, obj2, part,
                                LVGLCore::LVGL_STATE[stateindex])) {
      auto c = _lv_obj_get_style_int(
          s1, part,
          LV_STYLE_SCALE_BORDER_WIDTH | (state << LV_STYLE_STATE_POS));
      ret << ("lv_style_set_scale_end_border_width(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + QString::number(c) +
              ");");
    }
  }
  return ret;
}

void LVGLObject::parseStyles(const QJsonArray &styles) {
  for (int i = 0; i < styles.size(); ++i) {
    QJsonObject style = styles[i].toObject();
    int type = m_widgetClass->styles().indexOf(style["type"].toString());
    Q_ASSERT(type >= 0);

    //    lv_style_t *objStyle = this->style(type);
    //    if (style.contains("background")) {
    //      QJsonObject body = style["background"].toObject();
    //      if (body.contains("main_color")) {
    //        const QColor c =
    //            QVariant(body["main_color"].toString()).value<QColor>();
    //        axs_lv_style_set_bg_color(objStyle, lvgl->fromColor(c));
    //      }
    //      if (body.contains("grad_color")) {
    //        const QColor c =
    //            QVariant(body["grad_color"].toString()).value<QColor>();
    //        axs_lv_style_set_bg_grad_color(objStyle, lvgl->fromColor(c));
    //      }
    //      if (body.contains("radius"))
    //        axs_lv_style_set_radius(
    //            objStyle, static_cast<lv_coord_t>(body["radius"].toInt()));
    //      if (body.contains("opa"))
    //        axs_lv_style_set_bg_opa(objStyle,
    //                                static_cast<lv_opa_t>(body["opa"].toInt()));

    //      if (body.contains("border")) {
    //        QJsonObject border = body["border"].toObject();
    //        if (border.contains("color")) {
    //          const QColor c =
    //          QVariant(border["color"].toString()).value<QColor>();
    //          axs_lv_style_set_border_color(objStyle, lvgl->fromColor(c));
    //        }
    //        if (border.contains("width"))
    //          axs_lv_style_set_border_width(
    //              objStyle, static_cast<lv_coord_t>(border["width"].toInt()));

    //        if (border.contains("part"))
    //          axs_lv_style_set_border_side(
    //              objStyle,
    //              static_cast<lv_border_side_t>(border["part"].toInt()));
    //        if (border.contains("opa"))
    //          axs_lv_style_set_border_opa(
    //              objStyle, static_cast<lv_opa_t>(border["opa"].toInt()));
    //      }

    //      if (body.contains("shadow")) {
    //        QJsonObject shadow = body["shadow"].toObject();
    //        if (shadow.contains("color")) {
    //          const QColor c =
    //          QVariant(shadow["color"].toString()).value<QColor>();
    //          axs_lv_style_set_shadow_color(objStyle, lvgl->fromColor(c));
    //        }
    //        if (shadow.contains("width"))
    //          axs_lv_style_set_shadow_width(
    //              objStyle, static_cast<lv_coord_t>(shadow["width"].toInt()));
    //      }

    //      if (body.contains("padding")) {
    //        QJsonObject padding = body["padding"].toObject();
    //        if (padding.contains("top"))
    //          axs_lv_style_set_pad_top(
    //              objStyle, static_cast<lv_coord_t>(padding["top"].toInt()));
    //        if (padding.contains("bottom"))
    //          axs_lv_style_set_pad_bottom(
    //              objStyle,
    //              static_cast<lv_coord_t>(padding["bottom"].toInt()));
    //        if (padding.contains("left"))
    //          axs_lv_style_set_pad_left(
    //              objStyle, static_cast<lv_coord_t>(padding["left"].toInt()));
    //        if (padding.contains("right"))
    //          axs_lv_style_set_pad_right(
    //              objStyle,
    //              static_cast<lv_coord_t>(padding["right"].toInt()));
    //        if (padding.contains("inner"))
    //          axs_lv_style_set_pad_inner(
    //              objStyle,
    //              static_cast<lv_coord_t>(padding["inner"].toInt()));
    //      }
    //    }
    //    if (style.contains("text")) {
    //      QJsonObject text = style["text"].toObject();
    //      if (text.contains("color")) {
    //        const QColor c =
    //        QVariant(text["color"].toString()).value<QColor>();
    //        axs_lv_style_set_text_color(objStyle, lvgl->fromColor(c));
    //      }
    //      if (text.contains("sel_color")) {
    //        const QColor c =
    //        QVariant(text["sel_color"].toString()).value<QColor>();
    //        axs_lv_style_set_text_sel_color(objStyle, lvgl->fromColor(c));
    //      }
    //      if (text.contains("font")) {
    //        const QString fontName = text["font"].toString();
    //        const lv_font_t *f = lvgl->font(fontName);
    //        axs_lv_style_set_text_font(objStyle, f);
    //      }
    //      if (text.contains("line_space"))
    //        axs_lv_style_set_text_line_space(
    //            objStyle,
    //            static_cast<lv_coord_t>(text["line_space"].toInt()));
    //      if (text.contains("letter_space"))
    //        axs_lv_style_set_text_letter_space(
    //            objStyle,
    //            static_cast<lv_coord_t>(text["letter_space"].toInt()));
    //      if (text.contains("opa"))
    //        axs_lv_style_set_text_opa(objStyle,
    //                                  static_cast<lv_opa_t>(text["opa"].toInt()));
    //    }

    //    if (style.contains("image")) {
    //      QJsonObject image = style["image"].toObject();
    //      if (image.contains("recolor")) {
    //        const QColor c =
    //        QVariant(image["recolor"].toString()).value<QColor>();
    //        axs_lv_style_set_image_recolor(objStyle, lvgl->fromColor(c));
    //      }
    //      if (image.contains("opa"))
    //        axs_lv_style_set_image_opa(objStyle,
    //                                   static_cast<lv_opa_t>(image["opa"].toInt()));
    //    }

    //    if (style.contains("line")) {
    //      QJsonObject line = style["line"].toObject();
    //      if (line.contains("color")) {
    //        const QColor c =
    //        QVariant(line["color"].toString()).value<QColor>();
    //        axs_lv_style_set_line_color(objStyle, lvgl->fromColor(c));
    //      }
    //      if (line.contains("width"))
    //        axs_lv_style_set_line_width(
    //            objStyle, static_cast<lv_opa_t>(line["width"].toInt()));
    //      if (line.contains("opa"))
    //        axs_lv_style_set_line_opa(objStyle,
    //                                  static_cast<lv_opa_t>(line["opa"].toInt()));
    //    }
    //    m_widgetClass->setStyle(m_obj, type, objStyle);
    //    // m_widgetClass->addStyle(m_obj,type,objStyle);
    //  }
  }
}

LVGLObject *LVGLObject::parent() const { return m_parent; }

LVGLObject::operator const lv_obj_t *() const noexcept { return m_obj; }

LVGLObject::operator lv_obj_t *() noexcept { return m_obj; }

bool LVGLObject::hasCustomStyle(int type) const {
  return m_styles.contains(type);
}

lv_style_t *LVGLObject::style(int type) {
  auto s = m_widgetClass->style(m_obj);
  if (s == nullptr) {
    auto p = new (lv_style_t);
    m_liststyles << p;
    lv_style_init(p);
    m_widgetClass->addStyle(m_obj, p);
    s = p;
  }
  return s;
}

lv_style_t *LVGLObject::style(lv_obj_part_t part) {
  auto s = m_widgetClass->style(m_obj, part);
  if (s == nullptr) {
    auto p = new (lv_style_t);
    m_liststyles << p;
    lv_style_init(p);
    m_widgetClass->addStyle(m_obj, p, part);
    s = p;
  }
  return s;
}

lv_style_t *LVGLObject::style(int partsindex, int stateindex) {
  return m_widgetClass->getstyle(partsindex, stateindex);
}

QString LVGLObject::styleCodeName(int type) const {
  return name().toLower().replace(' ', '_') + "_s" + QString::number(type);
}

QList<LVGLObject *> LVGLObject::childs() const { return m_childs; }

void LVGLObject::removeChild(LVGLObject *child) { m_childs.removeOne(child); }

bool LVGLObject::isAccessible() const { return m_accessible; }

void LVGLObject::setAccessible(bool accessible) { m_accessible = accessible; }

bool LVGLObject::doesNameExists() const {
  for (LVGLObject *o : lvgl->allObjects()) {
    if (o == this) continue;
    if (o->name() == m_name) return true;
  }
  return false;
}

bool LVGLObject::doesNameExists(const QString &name, LVGLObject *except) {
  for (LVGLObject *o : lvgl->allObjects()) {
    if (except && (except == o)) continue;
    if (o->name() == name) return true;
  }
  return false;
}

void LVGLObject::generateName() {
  // create sorted list by type
  QList<int> numbers;
  const QString prefix = m_widgetClass->name().toLower().replace(' ', '_');

  for (LVGLObject *o : lvgl->allObjects()) {
    if (o->widgetType() == m_widgetClass->type() &&
        o->name().startsWith(prefix)) {
      const int index = o->name().lastIndexOf('_');
      const int num = o->name().mid(index + 1).toInt();
      if (num > 0) {
        auto ind = std::lower_bound(numbers.begin(), numbers.end(), num);
        numbers.insert(ind, num);
      }
    }
  }

  if (m_name.startsWith(prefix)) {
    const int index = m_name.lastIndexOf('_');
    const int num = m_name.mid(index + 1).toInt();
    if (!numbers.contains(num)) return;  // no new name needed
  }

  // find next free id
  int id = 1;
  for (int num : numbers) {
    if (num == id)
      id++;
    else
      break;
  }
  m_name = QString("%1_%2").arg(prefix).arg(id);
  return;
}

LVGLObject *LVGLObject::parse(QJsonObject object, LVGLObject *parent) {
  QString className = object["class"].toString();
  const LVGLWidget *widgetClass = lvgl->widget(className);
  if (widgetClass) {
    LVGLObject *newObj;
    QString name = object["name"].toString();
    if (parent)
      newObj = new LVGLObject(widgetClass, name, parent);
    else
      newObj = new LVGLObject(widgetClass, name, lvgl->getdispt()->act_scr);
    // generate name if nessessary
    if (newObj->name().isEmpty() || doesNameExists(name, newObj))
      newObj->generateName();
    lvgl->addObject(newObj);

    for (LVGLProperty *p : widgetClass->properties()) {
      QString pname = p->name().toLower();
      if (pname == "name") continue;
      if (object.contains(pname)) {
        QVariant v = object[pname].toVariant();
        if (!v.isNull()) p->setValue(newObj, v);
      }
    }
    if (object.contains("styles"))
      newObj->parseStyles(object["styles"].toArray());

    if (object.contains("children")) {
      QJsonArray childs = object["children"].toArray();
      for (int i = 0; i < childs.size(); ++i) {
        QJsonObject child = childs[i].toObject();
        if (widgetClass->type() == LVGLWidget::TabView &&
            child.contains("index")) {
          LVGLObject *page = newObj->findChildByIndex(child["index"].toInt());
          page->setName(child["name"].toString());
          // parse page props
          for (LVGLProperty *p : lvgl->widget("lv_page")->specialProperties()) {
            const QString pname = p->name().toLower();
            if (child.contains(pname)) {
              QVariant v = child[pname].toVariant();
              if (!v.isNull()) p->setValue(page, v);
            }
          }
          // parse styles
          if (child.contains("styles"))
            page->parseStyles(child["styles"].toArray());
          // parse children of page
          if (child.contains("children")) {
            QJsonArray page_childs = child["children"].toArray();
            for (int j = 0; j < page_childs.size(); ++j)
              parse(page_childs[j].toObject(), page);
          }
        } else {
          parse(child, newObj);
        }
      }
    }
    return newObj;
  }
  return nullptr;
}

int LVGLObject::index() const { return m_index; }

LVGLObject *LVGLObject::findChildByIndex(int index) const {
  for (LVGLObject *child : m_childs) {
    if (child->index() == index) return child;
  }
  return nullptr;
}
