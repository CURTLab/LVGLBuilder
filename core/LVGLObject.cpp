#include "LVGLObject.h"

#include <QDebug>
#include <QJsonArray>
#include <QUuid>

#include "LVGLCore.h"
#include "LVGLHelper.h"
#include "MainWindow.h"

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
  delete m_widgetClass;
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
    if (parent == lv_scr_act()) break;
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

QRect LVGLObject::relgeometry() const { return QRect(position(), size()); }

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
    if (!v.isNull()) object.insert(p->codename().toLower(), v);
  }

  QJsonArray styles = jsonStyles();
  object.insert("styles", styles);
  if (!m_childs.isEmpty()) {
    QJsonArray childs;
    for (LVGLObject *c : m_childs) childs.append(c->toJson());
    object.insert("children", childs);
  }
  return object;
}

QJsonObject LVGLObject::propToJson() {
  QJsonObject object({{"widget", m_widgetClass->name()},
                      {"class", m_widgetClass->className()},
                      {"name", m_name}});
  if (m_index > -1) object.insert("index", m_index);
  for (LVGLProperty *p : m_widgetClass->properties()) {
    QJsonValue v = p->toJson(this);
    if (!v.isNull()) object.insert(p->codename().toLower(), v);
  }

  return object;
}

QJsonObject LVGLObject::styleToJson() {
  QJsonObject object({{"widget", m_widgetClass->name()},
                      {"class", m_widgetClass->className()},
                      {"name", m_name}});
  if (m_index > -1) object.insert("index", m_index);
  QJsonArray styles = jsonAllStyles();
  object.insert("styles", styles);
  return object;
}

QJsonArray LVGLObject::jsonStyles() const {
  QJsonArray styles;
  auto codemap = LVGLHelper::getInstance().getMainW()->getCodeMap();
  int statestylesize = 7;
  for (int i = 0; i < m_widgetClass->styles().size(); ++i) {
    for (int j = 0; j < statestylesize; ++j) {
      QJsonObject style;
      auto parts = m_widgetClass->parts();
      style.insert("part", parts[i]);
      style.insert("state", j);
      int partindex = i;
      int stateindex = j;
      auto obj1 = this->obj();
      auto obj2 = codemap[m_widgetClass->type()];
      if (this->widgetClass()->className() == "lv_dropdown") {
        lv_dropdown_open(obj1);
        lv_dropdown_open(obj2);
      }
      const LVGL::StyleParts editableParts =
          m_widgetClass->editableStyles(partindex);
      auto part = m_widgetClass->parts().at(partindex);
      auto state = LVGLCore::LVGL_STATE[stateindex];
      auto s1 = obj1;

      if (editableParts & LVGL::Mix) {
        // mix
        QJsonObject mix;
        if (!radius(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              obj1, part, LV_STYLE_RADIUS | (state << LV_STYLE_STATE_POS));
          mix.insert("radius", c);
        }
        if (!clip_corner(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              obj1, part, LV_STYLE_CLIP_CORNER | (state << LV_STYLE_STATE_POS));
          mix.insert("clip_corner", c);
        }
        if (!mix_size(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              obj1, part, LV_STYLE_SIZE | (state << LV_STYLE_STATE_POS));
          mix.insert("mix_size", c);
        }
        if (!transform_width(obj1, obj2, part,
                             LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_TRANSFORM_WIDTH | (state << LV_STYLE_STATE_POS));
          mix.insert("transform_width", c);
        }
        if (!transform_height(obj1, obj2, part,
                              LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_TRANSFORM_HEIGHT | (state << LV_STYLE_STATE_POS));
          mix.insert("transform_height", c);
        }
        if (!transform_angle(obj1, obj2, part,
                             LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_TRANSFORM_ANGLE | (state << LV_STYLE_STATE_POS));
          mix.insert("transform_angle", c);
        }
        if (!transform_zoom(obj1, obj2, part,
                            LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_TRANSFORM_ZOOM | (state << LV_STYLE_STATE_POS));
          mix.insert("transform_zoom", c);
        }
        if (!opa_scale(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_OPA_SCALE | (state << LV_STYLE_STATE_POS));
          mix.insert("opa_scale", c);
        }
        if (mix.size() > 0) style.insert("Mix", mix);
      }
      if (editableParts & LVGL::Padding) {
        // padding
        QJsonObject padding;
        if (!pad_top(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_PAD_TOP | (state << LV_STYLE_STATE_POS));
          padding.insert("pad_top", c);
        }
        if (!pad_bottom(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_PAD_BOTTOM | (state << LV_STYLE_STATE_POS));
          padding.insert("pad_bottom", c);
        }
        if (!pad_left(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_PAD_LEFT | (state << LV_STYLE_STATE_POS));
          padding.insert("pad_left", c);
        }
        if (!pad_right(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_PAD_RIGHT | (state << LV_STYLE_STATE_POS));
          padding.insert("pad_right", c);
        }
        if (!pad_inner(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_PAD_INNER | (state << LV_STYLE_STATE_POS));
          padding.insert("pad_inner", c);
        }
        if (padding.size() > 0) style.insert("Padding", padding);
      }
      if (editableParts & LVGL::Margin) {
        // margin
        QJsonObject margin;
        if (!margin_top(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_MARGIN_TOP | (state << LV_STYLE_STATE_POS));
          margin.insert("margin_top", c);
        }
        if (!margin_bottom(obj1, obj2, part,
                           LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_MARGIN_BOTTOM | (state << LV_STYLE_STATE_POS));
          margin.insert("margin_bottom", c);
        }
        if (!margin_left(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_MARGIN_LEFT | (state << LV_STYLE_STATE_POS));
          margin.insert("margin_left", c);
        }
        if (!margin_right(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_MARGIN_RIGHT | (state << LV_STYLE_STATE_POS));
          margin.insert("margin_right", c);
        }
        if (margin.size() > 0) style.insert("Margin", margin);
      }
      if (editableParts & LVGL::Background) {
        // background
        QJsonObject background;
        if (!bg_color(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c =
              QVariant(lvgl.toColor(_lv_obj_get_style_color(
                           s1, part,
                           LV_STYLE_BG_COLOR | (state << LV_STYLE_STATE_POS))))
                  .toString()
                  .replace("#", "0x");
          background.insert("bg_color", c);
        }
        if (!bg_opa(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_opa(
              s1, part, LV_STYLE_BG_OPA | (state << LV_STYLE_STATE_POS));
          background.insert("bg_opa", c);
        }
        if (!bg_grad_color(obj1, obj2, part,
                           LVGLCore::LVGL_STATE[stateindex])) {
          auto c = QVariant(lvgl.toColor(_lv_obj_get_style_color(
                                s1, part,
                                LV_STYLE_BG_GRAD_COLOR |
                                    (state << LV_STYLE_STATE_POS))))
                       .toString()
                       .replace("#", "0x");

          background.insert("bg_grad_color", c);
        }
        if (!bg_main_stop(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_BG_MAIN_STOP | (state << LV_STYLE_STATE_POS));
          background.insert("bg_main_stop", c);
        }
        if (!bg_grad_stop(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_BG_GRAD_STOP | (state << LV_STYLE_STATE_POS));
          background.insert("bg_grad_stop", c);
        }
        if (!bg_grad_dir(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_BG_GRAD_DIR | (state << LV_STYLE_STATE_POS));
          background.insert("bg_grad_dir", c);
        }
        if (!bg_blend_mode(obj1, obj2, part,
                           LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_BG_BLEND_MODE | (state << LV_STYLE_STATE_POS));
          background.insert("bg_blend_mode", c);
        }
        if (background.size() > 0) style.insert("Background", background);
      }
      if (editableParts & LVGL::Border) {
        // border
        QJsonObject border;
        if (!border_color(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = QVariant(lvgl.toColor(_lv_obj_get_style_color(
                                s1, part,
                                LV_STYLE_BORDER_COLOR |
                                    (state << LV_STYLE_STATE_POS))))
                       .toString()
                       .replace("#", "0x");

          border.insert("border_color", c);
        }
        if (!border_opa(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_opa(
              s1, part, LV_STYLE_BG_OPA | (state << LV_STYLE_STATE_POS));
          border.insert("border_opa", c);
        }
        if (!border_width(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_BORDER_WIDTH | (state << LV_STYLE_STATE_POS));
          border.insert("border_width", c);
        }
        if (!border_side(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_BORDER_SIDE | (state << LV_STYLE_STATE_POS));
          border.insert("border_side", c);
        }
        if (!border_post(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_BORDER_POST | (state << LV_STYLE_STATE_POS));
          border.insert("border_post", c);
        }
        if (!border_blend_mode(obj1, obj2, part,
                               LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_BORDER_BLEND_MODE | (state << LV_STYLE_STATE_POS));
          border.insert("border_blend_mode", c);
        }
        if (border.size() > 0) style.insert("Border", border);
      }
      if (editableParts & LVGL::Outline) {
        // outline
        QJsonObject outline;
        if (!outline_color(obj1, obj2, part,
                           LVGLCore::LVGL_STATE[stateindex])) {
          auto c = QVariant(lvgl.toColor(_lv_obj_get_style_color(
                                s1, part,
                                LV_STYLE_OUTLINE_COLOR |
                                    (state << LV_STYLE_STATE_POS))))
                       .toString()
                       .replace("#", "0x");

          outline.insert("outline_color", c);
        }
        if (!outline_opa(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_opa(
              s1, part, LV_STYLE_OUTLINE_OPA | (state << LV_STYLE_STATE_POS));
          outline.insert("outline_opa", c);
        }
        if (!outline_width(obj1, obj2, part,
                           LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_OUTLINE_WIDTH | (state << LV_STYLE_STATE_POS));
          outline.insert("outline_width", c);
        }
        if (!outline_pad(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_OUTLINE_PAD | (state << LV_STYLE_STATE_POS));
          outline.insert("outline_pad", c);
        }
        if (!outline_blend_mode(obj1, obj2, part,
                                LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_OUTLINE_BLEND_MODE | (state << LV_STYLE_STATE_POS));
          outline.insert("outline_blend_mode", c);
        }
        if (outline.size() > 0) style.insert("Outline", outline);
      }
      if (editableParts & LVGL::Shadow) {
        // shadow
        QJsonObject shadow;
        if (!shadow_color(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = QVariant(lvgl.toColor(_lv_obj_get_style_color(
                                s1, part,
                                LV_STYLE_SHADOW_COLOR |
                                    (state << LV_STYLE_STATE_POS))))
                       .toString()
                       .replace("#", "0x");

          shadow.insert("shadow_color", c);
        }
        if (!shadow_opa(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_opa(
              s1, part, LV_STYLE_SHADOW_OPA | (state << LV_STYLE_STATE_POS));
          shadow.insert("shadow_opa", c);
        }
        if (!shadow_width(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_SHADOW_WIDTH | (state << LV_STYLE_STATE_POS));
          shadow.insert("shadow_width", c);
        }
        if (!shadow_ofs_x(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_SHADOW_OFS_X | (state << LV_STYLE_STATE_POS));
          shadow.insert("shadow_ofs_x", c);
        }
        if (!shadow_ofs_y(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_SHADOW_OFS_Y | (state << LV_STYLE_STATE_POS));
          shadow.insert("shadow_ofs_y", c);
        }
        if (!shadow_spread(obj1, obj2, part,
                           LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_SHADOW_SPREAD | (state << LV_STYLE_STATE_POS));
          shadow.insert("shadow_spread", c);
        }
        if (!shadow_blend_mode(obj1, obj2, part,
                               LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_SHADOW_BLEND_MODE | (state << LV_STYLE_STATE_POS));
          shadow.insert("shadow_blend_mode", c);
        }
        if (shadow.size() > 0) style.insert("Shadow", shadow);
      }
      if (editableParts & LVGL::Pattern) {
        // patter
        QJsonObject patter;
        if (!pattern_image(obj1, obj2, part,
                           LVGLCore::LVGL_STATE[stateindex])) {
        }
        if (!pattern_opa(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_opa(
              s1, part, LV_STYLE_PATTERN_OPA | (state << LV_STYLE_STATE_POS));
          patter.insert("pattern_image", c);
        }
        if (!pattern_recolor(obj1, obj2, part,
                             LVGLCore::LVGL_STATE[stateindex])) {
          auto c = QVariant(lvgl.toColor(_lv_obj_get_style_color(
                                s1, part,
                                LV_STYLE_PATTERN_RECOLOR |
                                    (state << LV_STYLE_STATE_POS))))
                       .toString()
                       .replace("#", "0x");

          patter.insert("pattern_recolor", c);
        }
        if (!pattern_recolor_opa(obj1, obj2, part,
                                 LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_opa(
              s1, part,
              LV_STYLE_PATTERN_RECOLOR_OPA | (state << LV_STYLE_STATE_POS));
          patter.insert("pattern_recolor_opa", c);
        }
        if (!pattern_repeat(obj1, obj2, part,
                            LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_PATTERN_REPEAT | (state << LV_STYLE_STATE_POS));
          patter.insert("pattern_repeat", c);
        }
        if (!pattern_blend_mode(obj1, obj2, part,
                                LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_PATTERN_BLEND_MODE | (state << LV_STYLE_STATE_POS));
          patter.insert("pattern_blend_mode", c);
        }
        if (patter.size() > 0) style.insert("Patter", patter);
      }
      if (editableParts & LVGL::Value) {
        // value
        QJsonObject value;
        if (!value_str(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = (const char *)_lv_obj_get_style_ptr(
              s1, part, LV_STYLE_VALUE_STR | (state << LV_STYLE_STATE_POS));
          value.insert("value_str", c);
        }
        if (!value_color(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = QVariant(lvgl.toColor(_lv_obj_get_style_color(
                                s1, part,
                                LV_STYLE_VALUE_COLOR |
                                    (state << LV_STYLE_STATE_POS))))
                       .toString()
                       .replace("#", "0x");

          value.insert("value_color", c);
        }
        if (!value_opa(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_opa(
              s1, part, LV_STYLE_VALUE_OPA | (state << LV_STYLE_STATE_POS));
          value.insert("value_opa", c);
        }
        if (!value_font(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = lvgl.fontName((lv_font_t *)_lv_obj_get_style_ptr(
              s1, part, LV_STYLE_VALUE_FONT | (state << LV_STYLE_STATE_POS)));
          value.insert("value_font", c);
        }
        if (!value_letter_space(obj1, obj2, part,
                                LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_VALUE_LETTER_SPACE | (state << LV_STYLE_STATE_POS));
          value.insert("value_letter_space", c);
        }
        if (!value_line_space(obj1, obj2, part,
                              LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_VALUE_LINE_SPACE | (state << LV_STYLE_STATE_POS));
          value.insert("value_line_space", c);
        }
        if (!value_align(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_VALUE_ALIGN | (state << LV_STYLE_STATE_POS));
          value.insert("value_align", c);
        }
        if (!value_ofs_x(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_VALUE_OFS_X | (state << LV_STYLE_STATE_POS));
          value.insert("value_ofs_x", c);
        }
        if (!value_ofs_y(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_VALUE_OFS_Y | (state << LV_STYLE_STATE_POS));
          value.insert("value_ofs_y", c);
        }
        if (!value_blend_mode(obj1, obj2, part,
                              LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_VALUE_BLEND_MODE | (state << LV_STYLE_STATE_POS));
          value.insert("value_blend_mode", c);
        }
        if (value.size() > 0) style.insert("Value", value);
      }
      if (editableParts & LVGL::Text) {
        // text
        QJsonObject text;
        if (!text_color(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = QVariant(lvgl.toColor(_lv_obj_get_style_color(
                                s1, part,
                                LV_STYLE_TEXT_COLOR |
                                    (state << LV_STYLE_STATE_POS))))
                       .toString()
                       .replace("#", "0x");

          text.insert("text_color", c);
        }
        if (!text_opa(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_opa(
              s1, part, LV_STYLE_TEXT_OPA | (state << LV_STYLE_STATE_POS));
          text.insert("text_opa", c);
        }
        if (!text_font(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = lvgl.fontName((lv_font_t *)_lv_obj_get_style_ptr(
              s1, part, LV_STYLE_TEXT_FONT | (state << LV_STYLE_STATE_POS)));
          text.insert("text_font", c);
        }
        if (!text_letter_space(obj1, obj2, part,
                               LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_TEXT_LETTER_SPACE | (state << LV_STYLE_STATE_POS));
          text.insert("text_letter_space", c);
        }
        if (!text_line_space(obj1, obj2, part,
                             LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_TEXT_LINE_SPACE | (state << LV_STYLE_STATE_POS));
          text.insert("text_line_space", c);
        }
        if (!text_decor(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_TEXT_DECOR | (state << LV_STYLE_STATE_POS));
          text.insert("text_decor", c);
        }
        if (!text_sel_color(obj1, obj2, part,
                            LVGLCore::LVGL_STATE[stateindex])) {
          auto c = QVariant(lvgl.toColor(_lv_obj_get_style_color(
                                s1, part,
                                LV_STYLE_TEXT_SEL_COLOR |
                                    (state << LV_STYLE_STATE_POS))))
                       .toString()
                       .replace("#", "0x");

          text.insert("text_sel_color", c);
        }
        if (!text_sel_bg_color(obj1, obj2, part,
                               LVGLCore::LVGL_STATE[stateindex])) {
          auto c = QVariant(lvgl.toColor(_lv_obj_get_style_color(
                                s1, part,
                                LV_STYLE_TEXT_SEL_BG_COLOR |
                                    (state << LV_STYLE_STATE_POS))))
                       .toString()
                       .replace("#", "0x");
          text.insert("text_sel_bg_color", c);
        }
        if (!text_blend_mode(obj1, obj2, part,
                             LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_TEXT_BLEND_MODE | (state << LV_STYLE_STATE_POS));
          text.insert("text_blend_mode", c);
        }
        if (text.size() > 0) style.insert("Text", text);
      }
      if (editableParts & LVGL::Line) {
        // line
        QJsonObject line;
        if (!line_color(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = QVariant(lvgl.toColor(_lv_obj_get_style_color(
                                s1, part,
                                LV_STYLE_LINE_COLOR |
                                    (state << LV_STYLE_STATE_POS))))
                       .toString()
                       .replace("#", "0x");

          line.insert("line_color", c);
        }
        if (!line_opa(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_opa(
              s1, part, LV_STYLE_LINE_OPA | (state << LV_STYLE_STATE_POS));
          line.insert("line_opa", c);
        }
        if (!line_width(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_LINE_WIDTH | (state << LV_STYLE_STATE_POS));
          line.insert("line_width", c);
        }
        if (!line_dash_width(obj1, obj2, part,
                             LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_LINE_DASH_WIDTH | (state << LV_STYLE_STATE_POS));
          line.insert("line_dash_width", c);
        }
        if (!line_dash_gap(obj1, obj2, part,
                           LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_LINE_DASH_GAP | (state << LV_STYLE_STATE_POS));
          line.insert("line_dash_gap", c);
        }
        if (!line_rounded(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_LINE_ROUNDED | (state << LV_STYLE_STATE_POS));
          line.insert("line_rounded", c);
        }
        if (!line_blend_mode(obj1, obj2, part,
                             LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_LINE_BLEND_MODE | (state << LV_STYLE_STATE_POS));
          line.insert("line_blend_mode", c);
        }
        if (line.size() > 0) style.insert("Line", line);
      }
      if (editableParts & LVGL::Image) {
        // image
        QJsonObject image;
        if (!image_recolor(obj1, obj2, part,
                           LVGLCore::LVGL_STATE[stateindex])) {
          auto c = QVariant(lvgl.toColor(_lv_obj_get_style_color(
                                s1, part,
                                LV_STYLE_IMAGE_RECOLOR |
                                    (state << LV_STYLE_STATE_POS))))
                       .toString()
                       .replace("#", "0x");
          image.insert("image_recolor", c);
        }
        if (!image_recolor_opa(obj1, obj2, part,
                               LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_opa(
              s1, part,
              LV_STYLE_IMAGE_RECOLOR_OPA | (state << LV_STYLE_STATE_POS));
          image.insert("image_recolor_opa", c);
        }
        if (!image_opa(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_opa(
              s1, part, LV_STYLE_IMAGE_OPA | (state << LV_STYLE_STATE_POS));
          image.insert("image_opa", c);
        }
        if (!image_blend_mode(obj1, obj2, part,
                              LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_IMAGE_BLEND_MODE | (state << LV_STYLE_STATE_POS));
          image.insert("image_blend_mode", c);
        }
        if (image.size() > 0) style.insert("Image", image);
      }
      if (editableParts & LVGL::Transition) {
        // transition
        QJsonObject transition;
        if (!transition_time(obj1, obj2, part,
                             LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_TRANSITION_TIME | (state << LV_STYLE_STATE_POS));
          transition.insert("transition_time", c);
        }
        if (!transition_delay(obj1, obj2, part,
                              LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_TRANSITION_DELAY | (state << LV_STYLE_STATE_POS));
          transition.insert("transition_delay", c);
        }
        if (!transition_prop_1(obj1, obj2, part,
                               LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_TRANSITION_PROP_1 | (state << LV_STYLE_STATE_POS));
          transition.insert("transition_prop_1", c);
        }
        if (!transition_prop_2(obj1, obj2, part,
                               LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_TRANSITION_PROP_2 | (state << LV_STYLE_STATE_POS));
          transition.insert("transition_prop_2", c);
        }
        if (!transition_prop_3(obj1, obj2, part,
                               LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_TRANSITION_PROP_3 | (state << LV_STYLE_STATE_POS));
          transition.insert("transition_prop_3", c);
        }
        if (!transition_prop_4(obj1, obj2, part,
                               LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_TRANSITION_PROP_4 | (state << LV_STYLE_STATE_POS));
          transition.insert("transition_prop_4", c);
        }
        if (!transition_prop_5(obj1, obj2, part,
                               LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_TRANSITION_PROP_5 | (state << LV_STYLE_STATE_POS));
          transition.insert("transition_prop_5", c);
        }
        if (!transition_prop_6(obj1, obj2, part,
                               LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_TRANSITION_PROP_6 | (state << LV_STYLE_STATE_POS));
          transition.insert("transition_prop_6", c);
        }
        if (transition.size() > 0) style.insert("Transition", transition);
      }
      if (editableParts & LVGL::Scale) {
        // scale
        QJsonObject scale;
        if (!scale_grad_color(obj1, obj2, part,
                              LVGLCore::LVGL_STATE[stateindex])) {
          auto c = QVariant(lvgl.toColor(_lv_obj_get_style_color(
                                s1, part,
                                LV_STYLE_SCALE_GRAD_COLOR |
                                    (state << LV_STYLE_STATE_POS))))
                       .toString()
                       .replace("#", "0x");

          scale.insert("scale_grad_color", c);
        }
        if (!scale_end_color(obj1, obj2, part,
                             LVGLCore::LVGL_STATE[stateindex])) {
          auto c = QVariant(lvgl.toColor(_lv_obj_get_style_color(
                                s1, part,
                                LV_STYLE_SCALE_END_COLOR |
                                    (state << LV_STYLE_STATE_POS))))
                       .toString()
                       .replace("#", "0x");

          scale.insert("scale_end_color", c);
        }
        if (!scale_width(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_SCALE_WIDTH | (state << LV_STYLE_STATE_POS));
          scale.insert("scale_width", c);
        }
        if (!scale_border_width(obj1, obj2, part,
                                LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_SCALE_BORDER_WIDTH | (state << LV_STYLE_STATE_POS));
          scale.insert("scale_border_width", c);
        }
        if (!scale_end_border_width(obj1, obj2, part,
                                    LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_SCALE_BORDER_WIDTH | (state << LV_STYLE_STATE_POS));
          scale.insert("scale_end_border_width", c);
        }
        if (scale.size() > 0) style.insert("Scale", scale);
      }
      QString TypeName = QString("PSS%1%2").arg(i).arg(j);
      styles.append(style);
    }
  }
  if (this->widgetClass()->className() == "lv_dropdown") {
    lv_dropdown_close(this->obj());
  }
  return styles;
}

QJsonArray LVGLObject::jsonAllStyles() const {
  QJsonArray styles;
  auto codemap = LVGLHelper::getInstance().getMainW()->getCodeMap();
  int statestylesize = 7;
  for (int i = 0; i < m_widgetClass->styles().size(); ++i) {
    for (int j = 0; j < statestylesize; ++j) {
      QJsonObject style;
      auto parts = m_widgetClass->parts();
      style.insert("part", parts[i]);
      style.insert("state", j);
      int partindex = i;
      int stateindex = j;
      auto obj1 = this->obj();
      auto obj2 = codemap[m_widgetClass->type()];
      if (this->widgetClass()->className() == "lv_dropdown") {
        lv_dropdown_open(obj1);
        lv_dropdown_open(obj2);
      }
      const LVGL::StyleParts editableParts =
          m_widgetClass->editableStyles(partindex);
      auto part = m_widgetClass->parts().at(partindex);
      auto state = LVGLCore::LVGL_STATE[stateindex];
      auto s1 = obj1;

      if (editableParts & LVGL::Mix) {
        // mix
        QJsonObject mix;
        if (true) {
          auto c = _lv_obj_get_style_int(
              obj1, part, LV_STYLE_RADIUS | (state << LV_STYLE_STATE_POS));
          mix.insert("radius", c);
        }
        if (true) {
          auto c = _lv_obj_get_style_int(
              obj1, part, LV_STYLE_CLIP_CORNER | (state << LV_STYLE_STATE_POS));
          mix.insert("clip_corner", c);
        }
        if (true) {
          auto c = _lv_obj_get_style_int(
              obj1, part, LV_STYLE_SIZE | (state << LV_STYLE_STATE_POS));
          mix.insert("mix_size", c);
        }
        if (true) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_TRANSFORM_WIDTH | (state << LV_STYLE_STATE_POS));
          mix.insert("transform_width", c);
        }
        if (true) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_TRANSFORM_HEIGHT | (state << LV_STYLE_STATE_POS));
          mix.insert("transform_height", c);
        }
        if (true) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_TRANSFORM_ANGLE | (state << LV_STYLE_STATE_POS));
          mix.insert("transform_angle", c);
        }
        if (true) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_TRANSFORM_ZOOM | (state << LV_STYLE_STATE_POS));
          mix.insert("transform_zoom", c);
        }
        if (true) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_OPA_SCALE | (state << LV_STYLE_STATE_POS));
          mix.insert("opa_scale", c);
        }
        if (mix.size() > 0) style.insert("Mix", mix);
      }
      if (editableParts & LVGL::Padding) {
        // padding
        QJsonObject padding;
        if (true) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_PAD_TOP | (state << LV_STYLE_STATE_POS));
          padding.insert("pad_top", c);
        }
        if (true) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_PAD_BOTTOM | (state << LV_STYLE_STATE_POS));
          padding.insert("pad_bottom", c);
        }
        if (true) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_PAD_LEFT | (state << LV_STYLE_STATE_POS));
          padding.insert("pad_left", c);
        }
        if (true) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_PAD_RIGHT | (state << LV_STYLE_STATE_POS));
          padding.insert("pad_right", c);
        }
        if (true) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_PAD_INNER | (state << LV_STYLE_STATE_POS));
          padding.insert("pad_inner", c);
        }
        if (padding.size() > 0) style.insert("Padding", padding);
      }
      if (editableParts & LVGL::Margin) {
        // margin
        QJsonObject margin;
        if (true) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_MARGIN_TOP | (state << LV_STYLE_STATE_POS));
          margin.insert("margin_top", c);
        }
        if (true) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_MARGIN_BOTTOM | (state << LV_STYLE_STATE_POS));
          margin.insert("margin_bottom", c);
        }
        if (true) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_MARGIN_LEFT | (state << LV_STYLE_STATE_POS));
          margin.insert("margin_left", c);
        }
        if (true) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_MARGIN_RIGHT | (state << LV_STYLE_STATE_POS));
          margin.insert("margin_right", c);
        }
        if (margin.size() > 0) style.insert("Margin", margin);
      }
      if (editableParts & LVGL::Background) {
        // background
        QJsonObject background;
        if (true) {
          auto c =
              QVariant(lvgl.toColor(_lv_obj_get_style_color(
                           s1, part,
                           LV_STYLE_BG_COLOR | (state << LV_STYLE_STATE_POS))))
                  .toString()
                  .replace("#", "0x");
          background.insert("bg_color", c);
        }
        if (true) {
          auto c = _lv_obj_get_style_opa(
              s1, part, LV_STYLE_BG_OPA | (state << LV_STYLE_STATE_POS));
          background.insert("bg_opa", c);
        }
        if (true) {
          auto c = QVariant(lvgl.toColor(_lv_obj_get_style_color(
                                s1, part,
                                LV_STYLE_BG_GRAD_COLOR |
                                    (state << LV_STYLE_STATE_POS))))
                       .toString()
                       .replace("#", "0x");

          background.insert("bg_grad_color", c);
        }
        if (true) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_BG_MAIN_STOP | (state << LV_STYLE_STATE_POS));
          background.insert("bg_main_stop", c);
        }
        if (true) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_BG_GRAD_STOP | (state << LV_STYLE_STATE_POS));
          background.insert("bg_grad_stop", c);
        }
        if (true) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_BG_GRAD_DIR | (state << LV_STYLE_STATE_POS));
          background.insert("bg_grad_dir", c);
        }
        if (true) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_BG_BLEND_MODE | (state << LV_STYLE_STATE_POS));
          background.insert("bg_blend_mode", c);
        }
        if (background.size() > 0) style.insert("Background", background);
      }
      if (editableParts & LVGL::Border) {
        // border
        QJsonObject border;
        if (true) {
          auto c = QVariant(lvgl.toColor(_lv_obj_get_style_color(
                                s1, part,
                                LV_STYLE_BORDER_COLOR |
                                    (state << LV_STYLE_STATE_POS))))
                       .toString()
                       .replace("#", "0x");

          border.insert("border_color", c);
        }
        if (true) {
          auto c = _lv_obj_get_style_opa(
              s1, part, LV_STYLE_BG_OPA | (state << LV_STYLE_STATE_POS));
          border.insert("border_opa", c);
        }
        if (true) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_BORDER_WIDTH | (state << LV_STYLE_STATE_POS));
          border.insert("border_width", c);
        }
        if (true) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_BORDER_SIDE | (state << LV_STYLE_STATE_POS));
          border.insert("border_side", c);
        }
        if (true) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_BORDER_POST | (state << LV_STYLE_STATE_POS));
          border.insert("border_post", c);
        }
        if (true) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_BORDER_BLEND_MODE | (state << LV_STYLE_STATE_POS));
          border.insert("border_blend_mode", c);
        }
        if (border.size() > 0) style.insert("Border", border);
      }
      if (editableParts & LVGL::Outline) {
        // outline
        QJsonObject outline;
        if (true) {
          auto c = QVariant(lvgl.toColor(_lv_obj_get_style_color(
                                s1, part,
                                LV_STYLE_OUTLINE_COLOR |
                                    (state << LV_STYLE_STATE_POS))))
                       .toString()
                       .replace("#", "0x");

          outline.insert("outline_color", c);
        }
        if (true) {
          auto c = _lv_obj_get_style_opa(
              s1, part, LV_STYLE_OUTLINE_OPA | (state << LV_STYLE_STATE_POS));
          outline.insert("outline_opa", c);
        }
        if (true) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_OUTLINE_WIDTH | (state << LV_STYLE_STATE_POS));
          outline.insert("outline_width", c);
        }
        if (true) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_OUTLINE_PAD | (state << LV_STYLE_STATE_POS));
          outline.insert("outline_pad", c);
        }
        if (true) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_OUTLINE_BLEND_MODE | (state << LV_STYLE_STATE_POS));
          outline.insert("outline_blend_mode", c);
        }
        if (outline.size() > 0) style.insert("Outline", outline);
      }
      if (editableParts & LVGL::Shadow) {
        // shadow
        QJsonObject shadow;
        if (true) {
          auto c = QVariant(lvgl.toColor(_lv_obj_get_style_color(
                                s1, part,
                                LV_STYLE_SHADOW_COLOR |
                                    (state << LV_STYLE_STATE_POS))))
                       .toString()
                       .replace("#", "0x");

          shadow.insert("shadow_color", c);
        }
        if (true) {
          auto c = _lv_obj_get_style_opa(
              s1, part, LV_STYLE_SHADOW_OPA | (state << LV_STYLE_STATE_POS));
          shadow.insert("shadow_opa", c);
        }
        if (true) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_SHADOW_WIDTH | (state << LV_STYLE_STATE_POS));
          shadow.insert("shadow_width", c);
        }
        if (true) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_SHADOW_OFS_X | (state << LV_STYLE_STATE_POS));
          shadow.insert("shadow_ofs_x", c);
        }
        if (true) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_SHADOW_OFS_Y | (state << LV_STYLE_STATE_POS));
          shadow.insert("shadow_ofs_y", c);
        }
        if (true) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_SHADOW_SPREAD | (state << LV_STYLE_STATE_POS));
          shadow.insert("shadow_spread", c);
        }
        if (true) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_SHADOW_BLEND_MODE | (state << LV_STYLE_STATE_POS));
          shadow.insert("shadow_blend_mode", c);
        }
        if (shadow.size() > 0) style.insert("Shadow", shadow);
      }
      if (editableParts & LVGL::Pattern) {
        // patter
        QJsonObject patter;
        if (!pattern_image(obj1, obj2, part,
                           LVGLCore::LVGL_STATE[stateindex])) {
        }
        if (!pattern_opa(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_opa(
              s1, part, LV_STYLE_PATTERN_OPA | (state << LV_STYLE_STATE_POS));
          patter.insert("pattern_image", c);
        }
        if (!pattern_recolor(obj1, obj2, part,
                             LVGLCore::LVGL_STATE[stateindex])) {
          auto c = QVariant(lvgl.toColor(_lv_obj_get_style_color(
                                s1, part,
                                LV_STYLE_PATTERN_RECOLOR |
                                    (state << LV_STYLE_STATE_POS))))
                       .toString()
                       .replace("#", "0x");

          patter.insert("pattern_recolor", c);
        }
        if (!pattern_recolor_opa(obj1, obj2, part,
                                 LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_opa(
              s1, part,
              LV_STYLE_PATTERN_RECOLOR_OPA | (state << LV_STYLE_STATE_POS));
          patter.insert("pattern_recolor_opa", c);
        }
        if (!pattern_repeat(obj1, obj2, part,
                            LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_PATTERN_REPEAT | (state << LV_STYLE_STATE_POS));
          patter.insert("pattern_repeat", c);
        }
        if (!pattern_blend_mode(obj1, obj2, part,
                                LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_PATTERN_BLEND_MODE | (state << LV_STYLE_STATE_POS));
          patter.insert("pattern_blend_mode", c);
        }
        if (patter.size() > 0) style.insert("Patter", patter);
      }
      if (editableParts & LVGL::Value) {
        // value
        QJsonObject value;
        if (true) {
          auto c = (const char *)_lv_obj_get_style_ptr(
              s1, part, LV_STYLE_VALUE_STR | (state << LV_STYLE_STATE_POS));
          value.insert("value_str", c);
        }
        if (true) {
          auto c = QVariant(lvgl.toColor(_lv_obj_get_style_color(
                                s1, part,
                                LV_STYLE_VALUE_COLOR |
                                    (state << LV_STYLE_STATE_POS))))
                       .toString()
                       .replace("#", "0x");

          value.insert("value_color", c);
        }
        if (true) {
          auto c = _lv_obj_get_style_opa(
              s1, part, LV_STYLE_VALUE_OPA | (state << LV_STYLE_STATE_POS));
          value.insert("value_opa", c);
        }
        if (true) {
          auto c = lvgl.fontName((lv_font_t *)_lv_obj_get_style_ptr(
              s1, part, LV_STYLE_VALUE_FONT | (state << LV_STYLE_STATE_POS)));
          value.insert("value_font", c);
        }
        if (true) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_VALUE_LETTER_SPACE | (state << LV_STYLE_STATE_POS));
          value.insert("value_letter_space", c);
        }
        if (true) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_VALUE_LINE_SPACE | (state << LV_STYLE_STATE_POS));
          value.insert("value_line_space", c);
        }
        if (true) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_VALUE_ALIGN | (state << LV_STYLE_STATE_POS));
          value.insert("value_align", c);
        }
        if (true) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_VALUE_OFS_X | (state << LV_STYLE_STATE_POS));
          value.insert("value_ofs_x", c);
        }
        if (true) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_VALUE_OFS_Y | (state << LV_STYLE_STATE_POS));
          value.insert("value_ofs_y", c);
        }
        if (true) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_VALUE_BLEND_MODE | (state << LV_STYLE_STATE_POS));
          value.insert("value_blend_mode", c);
        }
        if (value.size() > 0) style.insert("Value", value);
      }
      if (editableParts & LVGL::Text) {
        // text
        QJsonObject text;
        if (true) {
          auto c = QVariant(lvgl.toColor(_lv_obj_get_style_color(
                                s1, part,
                                LV_STYLE_TEXT_COLOR |
                                    (state << LV_STYLE_STATE_POS))))
                       .toString()
                       .replace("#", "0x");

          text.insert("text_color", c);
        }
        if (true) {
          auto c = _lv_obj_get_style_opa(
              s1, part, LV_STYLE_TEXT_OPA | (state << LV_STYLE_STATE_POS));
          text.insert("text_opa", c);
        }
        if (true) {
          auto c = lvgl.fontName((lv_font_t *)_lv_obj_get_style_ptr(
              s1, part, LV_STYLE_TEXT_FONT | (state << LV_STYLE_STATE_POS)));
          text.insert("text_font", c);
        }
        if (true) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_TEXT_LETTER_SPACE | (state << LV_STYLE_STATE_POS));
          text.insert("text_letter_space", c);
        }
        if (true) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_TEXT_LINE_SPACE | (state << LV_STYLE_STATE_POS));
          text.insert("text_line_space", c);
        }
        if (true) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_TEXT_DECOR | (state << LV_STYLE_STATE_POS));
          text.insert("text_decor", c);
        }
        if (true) {
          auto c = QVariant(lvgl.toColor(_lv_obj_get_style_color(
                                s1, part,
                                LV_STYLE_TEXT_SEL_COLOR |
                                    (state << LV_STYLE_STATE_POS))))
                       .toString()
                       .replace("#", "0x");

          text.insert("text_sel_color", c);
        }
        if (true) {
          auto c = QVariant(lvgl.toColor(_lv_obj_get_style_color(
                                s1, part,
                                LV_STYLE_TEXT_SEL_BG_COLOR |
                                    (state << LV_STYLE_STATE_POS))))
                       .toString()
                       .replace("#", "0x");
          text.insert("text_sel_bg_color", c);
        }
        if (true) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_TEXT_BLEND_MODE | (state << LV_STYLE_STATE_POS));
          text.insert("text_blend_mode", c);
        }
        if (text.size() > 0) style.insert("Text", text);
      }
      if (editableParts & LVGL::Line) {
        // line
        QJsonObject line;
        if (!line_color(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = QVariant(lvgl.toColor(_lv_obj_get_style_color(
                                s1, part,
                                LV_STYLE_LINE_COLOR |
                                    (state << LV_STYLE_STATE_POS))))
                       .toString()
                       .replace("#", "0x");

          line.insert("line_color", c);
        }
        if (!line_opa(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_opa(
              s1, part, LV_STYLE_LINE_OPA | (state << LV_STYLE_STATE_POS));
          line.insert("line_opa", c);
        }
        if (!line_width(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_LINE_WIDTH | (state << LV_STYLE_STATE_POS));
          line.insert("line_width", c);
        }
        if (!line_dash_width(obj1, obj2, part,
                             LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_LINE_DASH_WIDTH | (state << LV_STYLE_STATE_POS));
          line.insert("line_dash_width", c);
        }
        if (!line_dash_gap(obj1, obj2, part,
                           LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_LINE_DASH_GAP | (state << LV_STYLE_STATE_POS));
          line.insert("line_dash_gap", c);
        }
        if (!line_rounded(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_LINE_ROUNDED | (state << LV_STYLE_STATE_POS));
          line.insert("line_rounded", c);
        }
        if (!line_blend_mode(obj1, obj2, part,
                             LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_LINE_BLEND_MODE | (state << LV_STYLE_STATE_POS));
          line.insert("line_blend_mode", c);
        }
        if (line.size() > 0) style.insert("Line", line);
      }
      if (editableParts & LVGL::Image) {
        // image
        QJsonObject image;
        if (!image_recolor(obj1, obj2, part,
                           LVGLCore::LVGL_STATE[stateindex])) {
          auto c = QVariant(lvgl.toColor(_lv_obj_get_style_color(
                                s1, part,
                                LV_STYLE_IMAGE_RECOLOR |
                                    (state << LV_STYLE_STATE_POS))))
                       .toString()
                       .replace("#", "0x");
          image.insert("image_recolor", c);
        }
        if (!image_recolor_opa(obj1, obj2, part,
                               LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_opa(
              s1, part,
              LV_STYLE_IMAGE_RECOLOR_OPA | (state << LV_STYLE_STATE_POS));
          image.insert("image_recolor_opa", c);
        }
        if (!image_opa(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_opa(
              s1, part, LV_STYLE_IMAGE_OPA | (state << LV_STYLE_STATE_POS));
          image.insert("image_opa", c);
        }
        if (!image_blend_mode(obj1, obj2, part,
                              LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_IMAGE_BLEND_MODE | (state << LV_STYLE_STATE_POS));
          image.insert("image_blend_mode", c);
        }
        if (image.size() > 0) style.insert("Image", image);
      }
      if (editableParts & LVGL::Transition) {
        // transition
        QJsonObject transition;
        if (!transition_time(obj1, obj2, part,
                             LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_TRANSITION_TIME | (state << LV_STYLE_STATE_POS));
          transition.insert("transition_time", c);
        }
        if (!transition_delay(obj1, obj2, part,
                              LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_TRANSITION_DELAY | (state << LV_STYLE_STATE_POS));
          transition.insert("transition_delay", c);
        }
        if (!transition_prop_1(obj1, obj2, part,
                               LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_TRANSITION_PROP_1 | (state << LV_STYLE_STATE_POS));
          transition.insert("transition_prop_1", c);
        }
        if (!transition_prop_2(obj1, obj2, part,
                               LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_TRANSITION_PROP_2 | (state << LV_STYLE_STATE_POS));
          transition.insert("transition_prop_2", c);
        }
        if (!transition_prop_3(obj1, obj2, part,
                               LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_TRANSITION_PROP_3 | (state << LV_STYLE_STATE_POS));
          transition.insert("transition_prop_3", c);
        }
        if (!transition_prop_4(obj1, obj2, part,
                               LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_TRANSITION_PROP_4 | (state << LV_STYLE_STATE_POS));
          transition.insert("transition_prop_4", c);
        }
        if (!transition_prop_5(obj1, obj2, part,
                               LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_TRANSITION_PROP_5 | (state << LV_STYLE_STATE_POS));
          transition.insert("transition_prop_5", c);
        }
        if (!transition_prop_6(obj1, obj2, part,
                               LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_TRANSITION_PROP_6 | (state << LV_STYLE_STATE_POS));
          transition.insert("transition_prop_6", c);
        }
        if (transition.size() > 0) style.insert("Transition", transition);
      }
      if (editableParts & LVGL::Scale) {
        // scale
        QJsonObject scale;
        if (!scale_grad_color(obj1, obj2, part,
                              LVGLCore::LVGL_STATE[stateindex])) {
          auto c = QVariant(lvgl.toColor(_lv_obj_get_style_color(
                                s1, part,
                                LV_STYLE_SCALE_GRAD_COLOR |
                                    (state << LV_STYLE_STATE_POS))))
                       .toString()
                       .replace("#", "0x");

          scale.insert("scale_grad_color", c);
        }
        if (!scale_end_color(obj1, obj2, part,
                             LVGLCore::LVGL_STATE[stateindex])) {
          auto c = QVariant(lvgl.toColor(_lv_obj_get_style_color(
                                s1, part,
                                LV_STYLE_SCALE_END_COLOR |
                                    (state << LV_STYLE_STATE_POS))))
                       .toString()
                       .replace("#", "0x");

          scale.insert("scale_end_color", c);
        }
        if (!scale_width(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part, LV_STYLE_SCALE_WIDTH | (state << LV_STYLE_STATE_POS));
          scale.insert("scale_width", c);
        }
        if (!scale_border_width(obj1, obj2, part,
                                LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_SCALE_BORDER_WIDTH | (state << LV_STYLE_STATE_POS));
          scale.insert("scale_border_width", c);
        }
        if (!scale_end_border_width(obj1, obj2, part,
                                    LVGLCore::LVGL_STATE[stateindex])) {
          auto c = _lv_obj_get_style_int(
              s1, part,
              LV_STYLE_SCALE_BORDER_WIDTH | (state << LV_STYLE_STATE_POS));
          scale.insert("scale_end_border_width", c);
        }
        if (scale.size() > 0) style.insert("Scale", scale);
      }
      QString TypeName = QString("PSS%1%2").arg(i).arg(j);
      styles.append(style);
    }
  }
  if (this->widgetClass()->className() == "lv_dropdown") {
    lv_dropdown_close(this->obj());
  }
  return styles;
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
  auto &fontset = LVGLHelper::getInstance().getSaveFontName();

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
      auto c = QVariant(lvgl.toColor(_lv_obj_get_style_color(
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
          QVariant(lvgl.toColor(_lv_obj_get_style_color(
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
          QVariant(lvgl.toColor(_lv_obj_get_style_color(
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
          QVariant(lvgl.toColor(_lv_obj_get_style_color(
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
          QVariant(lvgl.toColor(_lv_obj_get_style_color(
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
      auto c = QVariant(lvgl.toColor(_lv_obj_get_style_color(
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
      QString s = "static char* " + strname + " = \"" + c + "\";\n";
      ret << s;
      ret << ("lv_style_set_value_str(&" + styleVar + "," +
              LVGLCore::LVGL_STATE_STR[stateindex] + "," + strname + ");");
    }
    if (!value_color(obj1, obj2, part, LVGLCore::LVGL_STATE[stateindex])) {
      auto c =
          QVariant(lvgl.toColor(_lv_obj_get_style_color(
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
      auto c = lvgl.fontCodeName((lv_font_t *)_lv_obj_get_style_ptr(
          s1, part, LV_STYLE_VALUE_FONT | (state << LV_STYLE_STATE_POS)));
      LVGLHelper::getInstance().setSaveFontNmae(c);
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
          QVariant(lvgl.toColor(_lv_obj_get_style_color(
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
      auto c = lvgl.fontCodeName((lv_font_t *)_lv_obj_get_style_ptr(
          s1, part, LV_STYLE_TEXT_FONT | (state << LV_STYLE_STATE_POS)));
      LVGLHelper::getInstance().setSaveFontNmae(c);
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
      auto c = QVariant(lvgl.toColor(_lv_obj_get_style_color(
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
      auto c = QVariant(lvgl.toColor(_lv_obj_get_style_color(
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
          QVariant(lvgl.toColor(_lv_obj_get_style_color(
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
          QVariant(lvgl.toColor(_lv_obj_get_style_color(
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
      auto c = QVariant(lvgl.toColor(_lv_obj_get_style_color(
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
      auto c = QVariant(lvgl.toColor(_lv_obj_get_style_color(
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
    int part = style["part"].toInt();
    int statetype = style["state"].toInt();
    lv_state_t state = LV_STATE_DEFAULT;
    switch (statetype) {
      case 0:
        state = LV_STATE_DEFAULT;
        break;
      case 1:
        state = LV_STATE_CHECKED;
        break;
      case 2:
        state = LV_STATE_FOCUSED;
        break;
      case 3:
        state = LV_STATE_EDITED;
        break;
      case 4:
        state = LV_STATE_HOVERED;
        break;
      case 5:
        state = LV_STATE_PRESSED;
        break;
      case 6:
        state = LV_STATE_DISABLED;
        break;
      default:
        state = LV_STATE_DEFAULT;
    }
    if (style.contains("Mix")) {
      QJsonObject mix = style["Mix"].toObject();
      if (mix.contains("radius")) {
        auto c = mix["radius"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part, LV_STYLE_RADIUS | (state << LV_STYLE_STATE_POS), c);
      }
      if (mix.contains("clip_corner")) {
        auto c = mix["clip_corner"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part, LV_STYLE_CLIP_CORNER | (state << LV_STYLE_STATE_POS),
            c);
      }
      if (mix.contains("mix_size")) {
        auto c = mix["mix_size"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part, LV_STYLE_SIZE | (state << LV_STYLE_STATE_POS), c);
      }
      if (mix.contains("transform_width")) {
        auto c = mix["transform_width"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part,
            LV_STYLE_TRANSFORM_WIDTH | (state << LV_STYLE_STATE_POS), c);
      }
      if (mix.contains("transform_height")) {
        auto c = mix["transform_height"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part,
            LV_STYLE_TRANSFORM_HEIGHT | (state << LV_STYLE_STATE_POS), c);
      }
      if (mix.contains("transform_angle")) {
        auto c = mix["transform_angle"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part,
            LV_STYLE_TRANSFORM_ANGLE | (state << LV_STYLE_STATE_POS), c);
      }
      if (mix.contains("transform_zoom")) {
        auto c = mix["transform_zoom"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part,
            LV_STYLE_TRANSFORM_ZOOM | (state << LV_STYLE_STATE_POS), c);
      }
      if (mix.contains("opa_scale")) {
        auto c = mix["opa_scale"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part, LV_STYLE_OPA_SCALE | (state << LV_STYLE_STATE_POS), c);
      }
    }
    if (style.contains("Padding")) {
      QJsonObject mix = style["Padding"].toObject();
      if (mix.contains("pad_top")) {
        auto c = mix["pad_top"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part, LV_STYLE_PAD_TOP | (state << LV_STYLE_STATE_POS), c);
      }
      if (mix.contains("pad_bottom")) {
        auto c = mix["pad_bottom"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part, LV_STYLE_PAD_BOTTOM | (state << LV_STYLE_STATE_POS),
            c);
      }
      if (mix.contains("pad_left")) {
        auto c = mix["pad_left"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part, LV_STYLE_PAD_LEFT | (state << LV_STYLE_STATE_POS), c);
      }
      if (mix.contains("pad_right")) {
        auto c = mix["pad_right"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part, LV_STYLE_PAD_RIGHT | (state << LV_STYLE_STATE_POS), c);
      }
      if (mix.contains("pad_inner")) {
        auto c = mix["pad_inner"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part, LV_STYLE_PAD_INNER | (state << LV_STYLE_STATE_POS), c);
      }
    }
    if (style.contains("Margin")) {
      QJsonObject mix = style["Margin"].toObject();
      if (mix.contains("margin_top")) {
        auto c = mix["margin_top"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part, LV_STYLE_MARGIN_TOP | (state << LV_STYLE_STATE_POS),
            c);
      }
      if (mix.contains("margin_bottom")) {
        auto c = mix["margin_bottom"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part, LV_STYLE_MARGIN_BOTTOM | (state << LV_STYLE_STATE_POS),
            c);
      }
      if (mix.contains("margin_left")) {
        auto c = mix["margin_left"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part, LV_STYLE_MARGIN_LEFT | (state << LV_STYLE_STATE_POS),
            c);
      }
      if (mix.contains("margin_right")) {
        auto c = mix["margin_right"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part, LV_STYLE_MARGIN_RIGHT | (state << LV_STYLE_STATE_POS),
            c);
      }
    }
    if (style.contains("Background")) {
      QJsonObject mix = style["Background"].toObject();
      if (mix.contains("bg_color")) {
        auto c = lvgl.fromColor(
            QColor(QString("#%1").arg(mix["bg_color"].toString().mid(2, 7))));
        _lv_obj_set_style_local_color(
            m_obj, part, LV_STYLE_BG_COLOR | (state << LV_STYLE_STATE_POS), c);
      }
      if (mix.contains("bg_opa")) {
        auto c = mix["bg_opa"].toInt();
        _lv_obj_set_style_local_opa(
            m_obj, part, LV_STYLE_BG_OPA | (state << LV_STYLE_STATE_POS), c);
      }
      if (mix.contains("bg_grad_color")) {
        auto c = lvgl.fromColor(QColor(
            QString("#%1").arg(mix["bg_grad_color"].toString().mid(2, 7))));
        _lv_obj_set_style_local_color(
            m_obj, part, LV_STYLE_BG_GRAD_COLOR | (state << LV_STYLE_STATE_POS),
            c);
      }
      if (mix.contains("bg_main_stop")) {
        auto c = mix["bg_main_stop"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part, LV_STYLE_BG_MAIN_STOP | (state << LV_STYLE_STATE_POS),
            c);
      }
      if (mix.contains("bg_grad_stop")) {
        auto c = mix["bg_grad_stop"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part, LV_STYLE_BG_GRAD_STOP | (state << LV_STYLE_STATE_POS),
            c);
      }
      if (mix.contains("bg_grad_dir")) {
        auto c = mix["bg_grad_dir"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part, LV_STYLE_BG_GRAD_DIR | (state << LV_STYLE_STATE_POS),
            c);
      }
      if (mix.contains("bg_blend_mode")) {
        auto c = mix["bg_blend_mode"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part, LV_STYLE_BG_BLEND_MODE | (state << LV_STYLE_STATE_POS),
            c);
      }
    }
    if (style.contains("Border")) {
      QJsonObject mix = style["Border"].toObject();
      if (mix.contains("border_color")) {
        auto c = lvgl.fromColor(QColor(
            QString("#%1").arg(mix["border_color"].toString().mid(2, 7))));
        _lv_obj_set_style_local_color(
            m_obj, part, LV_STYLE_BORDER_COLOR | (state << LV_STYLE_STATE_POS),
            c);
      }
      if (mix.contains("border_opa")) {
        auto c = mix["border_opa"].toInt();
        _lv_obj_set_style_local_opa(
            m_obj, part, LV_STYLE_BORDER_OPA | (state << LV_STYLE_STATE_POS),
            c);
      }
      if (mix.contains("border_width")) {
        auto c = mix["border_width"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part, LV_STYLE_BORDER_WIDTH | (state << LV_STYLE_STATE_POS),
            c);
      }
      if (mix.contains("border_side")) {
        auto c = mix["border_side"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part, LV_STYLE_BORDER_SIDE | (state << LV_STYLE_STATE_POS),
            c);
      }
      if (mix.contains("border_post")) {
        auto c = mix["border_post"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part, LV_STYLE_BORDER_POST | (state << LV_STYLE_STATE_POS),
            c);
      }
      if (mix.contains("border_blend_mode")) {
        auto c = mix["border_blend_mode"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part,
            LV_STYLE_BORDER_BLEND_MODE | (state << LV_STYLE_STATE_POS), c);
      }
    }
    if (style.contains("Outline")) {
      QJsonObject mix = style["Outline"].toObject();
      if (mix.contains("outline_color")) {
        auto c = lvgl.fromColor(QColor(
            QString("#%1").arg(mix["outline_color"].toString().mid(2, 7))));
        _lv_obj_set_style_local_color(
            m_obj, part, LV_STYLE_OUTLINE_COLOR | (state << LV_STYLE_STATE_POS),
            c);
      }
      if (mix.contains("outline_opa")) {
        auto c = mix["outline_opa"].toInt();
        _lv_obj_set_style_local_opa(
            m_obj, part, LV_STYLE_OUTLINE_OPA | (state << LV_STYLE_STATE_POS),
            c);
      }
      if (mix.contains("outline_width")) {
        auto c = mix["outline_width"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part, LV_STYLE_OUTLINE_WIDTH | (state << LV_STYLE_STATE_POS),
            c);
      }
      if (mix.contains("outline_pad")) {
        auto c = mix["outline_pad"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part, LV_STYLE_OUTLINE_PAD | (state << LV_STYLE_STATE_POS),
            c);
      }
      if (mix.contains("outline_blend_mode")) {
        auto c = mix["outline_blend_mode"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part,
            LV_STYLE_OUTLINE_BLEND_MODE | (state << LV_STYLE_STATE_POS), c);
      }
    }
    if (style.contains("Shadow")) {
      QJsonObject mix = style["Shadow"].toObject();
      if (mix.contains("shadow_color")) {
        auto c = lvgl.fromColor(QColor(
            QString("#%1").arg(mix["shadow_color"].toString().mid(2, 7))));
        _lv_obj_set_style_local_color(
            m_obj, part, LV_STYLE_SHADOW_COLOR | (state << LV_STYLE_STATE_POS),
            c);
      }
      if (mix.contains("shadow_opa")) {
        auto c = mix["shadow_opa"].toInt();
        _lv_obj_set_style_local_opa(
            m_obj, part, LV_STYLE_SHADOW_OPA | (state << LV_STYLE_STATE_POS),
            c);
      }
      if (mix.contains("shadow_width")) {
        auto c = mix["shadow_width"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part, LV_STYLE_SHADOW_WIDTH | (state << LV_STYLE_STATE_POS),
            c);
      }
      if (mix.contains("shadow_ofs_x")) {
        auto c = mix["shadow_ofs_x"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part, LV_STYLE_SHADOW_OFS_X | (state << LV_STYLE_STATE_POS),
            c);
      }
      if (mix.contains("shadow_ofs_y")) {
        auto c = mix["shadow_ofs_y"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part, LV_STYLE_SHADOW_OFS_Y | (state << LV_STYLE_STATE_POS),
            c);
      }
      if (mix.contains("shadow_spread")) {
        auto c = mix["shadow_spread"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part, LV_STYLE_SHADOW_SPREAD | (state << LV_STYLE_STATE_POS),
            c);
      }
      if (mix.contains("shadow_blend_mode")) {
        auto c = mix["shadow_blend_mode"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part,
            LV_STYLE_SHADOW_BLEND_MODE | (state << LV_STYLE_STATE_POS), c);
      }
    }
    if (style.contains("Patter")) {
      QJsonObject mix = style["Patter"].toObject();
      if (mix.contains("pattern_image")) {
        //        auto c = mix["pattern_image"].toInt();
        //        _lv_obj_set_style_local_int(
        //            m_obj, part, LV_STYLE_PATTERN_IMAGE | (state <<
        //            LV_STYLE_STATE_POS), c);
      }
      if (mix.contains("pattern_opa")) {
        auto c = mix["pattern_opa"].toInt();
        _lv_obj_set_style_local_opa(
            m_obj, part, LV_STYLE_PATTERN_OPA | (state << LV_STYLE_STATE_POS),
            c);
      }
      if (mix.contains("pattern_recolor")) {
        auto c = lvgl.fromColor(QColor(
            QString("#%1").arg(mix["pattern_recolor"].toString().mid(2, 7))));
        _lv_obj_set_style_local_color(
            m_obj, part,
            LV_STYLE_PATTERN_RECOLOR | (state << LV_STYLE_STATE_POS), c);
      }
      if (mix.contains("pattern_recolor_opa")) {
        auto c = mix["pattern_recolor_opa"].toInt();
        _lv_obj_set_style_local_opa(
            m_obj, part,
            LV_STYLE_PATTERN_RECOLOR_OPA | (state << LV_STYLE_STATE_POS), c);
      }
      if (mix.contains("pattern_repeat")) {
        auto c = mix["pattern_repeat"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part,
            LV_STYLE_PATTERN_REPEAT | (state << LV_STYLE_STATE_POS), c);
      }
      if (mix.contains("pattern_blend_mode")) {
        auto c = mix["pattern_blend_mode"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part,
            LV_STYLE_PATTERN_BLEND_MODE | (state << LV_STYLE_STATE_POS), c);
      }
    }
    if (style.contains("Value")) {
      QJsonObject mix = style["Value"].toObject();
      if (mix.contains("value_str")) {
        memset(m_pStr, 0, sizeof(m_pStr));
        strcpy(m_pStr, mix["value_str"].toString().toUtf8().data());
        _lv_obj_set_style_local_ptr(
            m_obj, part, LV_STYLE_VALUE_STR | (state << LV_STYLE_STATE_POS),
            m_pStr);
      }
      if (mix.contains("value_color")) {
        auto c = lvgl.fromColor(QColor(
            QString("#%1").arg(mix["value_color"].toString().mid(2, 7))));
        _lv_obj_set_style_local_color(
            m_obj, part, LV_STYLE_VALUE_COLOR | (state << LV_STYLE_STATE_POS),
            c);
      }
      if (mix.contains("value_opa")) {
        auto c = mix["value_opa"].toInt();
        _lv_obj_set_style_local_opa(
            m_obj, part, LV_STYLE_VALUE_OPA | (state << LV_STYLE_STATE_POS), c);
      }
      if (mix.contains("value_font")) {
        auto c = lvgl.font(mix["value_font"].toString());
        _lv_obj_set_style_local_ptr(
            m_obj, part, LV_STYLE_VALUE_FONT | (state << LV_STYLE_STATE_POS),
            c);
      }
      if (mix.contains("value_letter_space")) {
        auto c = mix["value_letter_space"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part,
            LV_STYLE_VALUE_LETTER_SPACE | (state << LV_STYLE_STATE_POS), c);
      }
      if (mix.contains("value_line_space")) {
        auto c = mix["value_line_space"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part,
            LV_STYLE_VALUE_LINE_SPACE | (state << LV_STYLE_STATE_POS), c);
      }
      if (mix.contains("value_align")) {
        auto c = mix["value_align"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part, LV_STYLE_VALUE_ALIGN | (state << LV_STYLE_STATE_POS),
            c);
      }
      if (mix.contains("value_ofs_x")) {
        auto c = mix["value_ofs_x"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part, LV_STYLE_VALUE_OFS_X | (state << LV_STYLE_STATE_POS),
            c);
      }
      if (mix.contains("value_ofs_y")) {
        auto c = mix["value_ofs_y"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part, LV_STYLE_VALUE_OFS_Y | (state << LV_STYLE_STATE_POS),
            c);
      }
      if (mix.contains("value_blend_mode")) {
        auto c = mix["value_blend_mode"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part,
            LV_STYLE_VALUE_BLEND_MODE | (state << LV_STYLE_STATE_POS), c);
      }
    }
    if (style.contains("Text")) {
      QJsonObject mix = style["Text"].toObject();
      if (mix.contains("text_color")) {
        auto c = lvgl.fromColor(
            QColor(QString("#%1").arg(mix["text_color"].toString().mid(2, 7))));
        _lv_obj_set_style_local_color(
            m_obj, part, LV_STYLE_TEXT_COLOR | (state << LV_STYLE_STATE_POS),
            c);
      }
      if (mix.contains("text_opa")) {
        auto c = mix["text_opa"].toInt();
        _lv_obj_set_style_local_opa(
            m_obj, part, LV_STYLE_TEXT_OPA | (state << LV_STYLE_STATE_POS), c);
      }
      if (mix.contains("text_font")) {
        const lv_font_t *c = lvgl.font(mix["text_font"].toString());
        _lv_obj_set_style_local_ptr(
            m_obj, part, LV_STYLE_TEXT_FONT | (state << LV_STYLE_STATE_POS), c);
      }
      if (mix.contains("text_letter_space")) {
        auto c = mix["text_letter_space"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part,
            LV_STYLE_TEXT_LETTER_SPACE | (state << LV_STYLE_STATE_POS), c);
      }
      if (mix.contains("text_line_space")) {
        auto c = mix["text_line_space"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part,
            LV_STYLE_TEXT_LINE_SPACE | (state << LV_STYLE_STATE_POS), c);
      }
      if (mix.contains("text_decor")) {
        auto c = mix["text_decor"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part, LV_STYLE_TEXT_DECOR | (state << LV_STYLE_STATE_POS),
            c);
      }
      if (mix.contains("text_sel_color")) {
        auto c = lvgl.fromColor(QColor(
            QString("#%1").arg(mix["text_sel_color"].toString().mid(2, 7))));
        _lv_obj_set_style_local_color(
            m_obj, part,
            LV_STYLE_TEXT_SEL_COLOR | (state << LV_STYLE_STATE_POS), c);
      }
      if (mix.contains("text_sel_bg_color")) {
        auto c = lvgl.fromColor(QColor(
            QString("#%1").arg(mix["text_sel_bg_color"].toString().mid(2, 7))));
        _lv_obj_set_style_local_color(
            m_obj, part,
            LV_STYLE_TEXT_SEL_BG_COLOR | (state << LV_STYLE_STATE_POS), c);
      }
      if (mix.contains("text_blend_mode")) {
        auto c = mix["text_blend_mode"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part,
            LV_STYLE_TEXT_BLEND_MODE | (state << LV_STYLE_STATE_POS), c);
      }
    }
    if (style.contains("Line")) {
      QJsonObject mix = style["Line"].toObject();
      if (mix.contains("line_color")) {
        auto c = lvgl.fromColor(
            QColor(QString("#%1").arg(mix["line_color"].toString().mid(2, 7))));
        _lv_obj_set_style_local_color(
            m_obj, part, LV_STYLE_LINE_COLOR | (state << LV_STYLE_STATE_POS),
            c);
      }
      if (mix.contains("line_opa")) {
        auto c = mix["line_opa"].toInt();
        _lv_obj_set_style_local_opa(
            m_obj, part, LV_STYLE_LINE_OPA | (state << LV_STYLE_STATE_POS), c);
      }
      if (mix.contains("line_width")) {
        auto c = mix["line_width"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part, LV_STYLE_LINE_WIDTH | (state << LV_STYLE_STATE_POS),
            c);
      }
      if (mix.contains("line_dash_width")) {
        auto c = mix["line_dash_width"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part,
            LV_STYLE_LINE_DASH_WIDTH | (state << LV_STYLE_STATE_POS), c);
      }
      if (mix.contains("line_dash_gap")) {
        auto c = mix["line_dash_gap"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part, LV_STYLE_LINE_DASH_GAP | (state << LV_STYLE_STATE_POS),
            c);
      }
      if (mix.contains("line_rounded")) {
        auto c = mix["line_rounded"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part, LV_STYLE_LINE_ROUNDED | (state << LV_STYLE_STATE_POS),
            c);
      }
      if (mix.contains("line_blend_mode")) {
        auto c = mix["line_blend_mode"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part,
            LV_STYLE_LINE_BLEND_MODE | (state << LV_STYLE_STATE_POS), c);
      }
    }
    if (style.contains("Image")) {
      QJsonObject mix = style["Image"].toObject();
      if (mix.contains("image_recolor")) {
        auto c = lvgl.fromColor(QColor(
            QString("#%1").arg(mix["image_recolor"].toString().mid(2, 7))));
        _lv_obj_set_style_local_color(
            m_obj, part, LV_STYLE_IMAGE_RECOLOR | (state << LV_STYLE_STATE_POS),
            c);
      }
      if (mix.contains("image_recolor_opa")) {
        auto c = mix["image_recolor_opa"].toInt();
        _lv_obj_set_style_local_opa(
            m_obj, part,
            LV_STYLE_IMAGE_RECOLOR_OPA | (state << LV_STYLE_STATE_POS), c);
      }
      if (mix.contains("image_opa")) {
        auto c = mix["image_opa"].toInt();
        _lv_obj_set_style_local_opa(
            m_obj, part, LV_STYLE_IMAGE_OPA | (state << LV_STYLE_STATE_POS), c);
      }
      if (mix.contains("image_blend_mode")) {
        auto c = mix["image_blend_mode"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part,
            LV_STYLE_IMAGE_BLEND_MODE | (state << LV_STYLE_STATE_POS), c);
      }
    }
    if (style.contains("Transition")) {
      QJsonObject mix = style["Transition"].toObject();
      if (mix.contains("transition_time")) {
        auto c = mix["transition_time"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part,
            LV_STYLE_TRANSITION_TIME | (state << LV_STYLE_STATE_POS), c);
      }
      if (mix.contains("transition_delay")) {
        auto c = mix["transition_delay"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part,
            LV_STYLE_TRANSITION_DELAY | (state << LV_STYLE_STATE_POS), c);
      }
      if (mix.contains("transition_prop_1")) {
        auto c = mix["transition_prop_1"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part,
            LV_STYLE_TRANSITION_PROP_1 | (state << LV_STYLE_STATE_POS), c);
      }
      if (mix.contains("transition_prop_2")) {
        auto c = mix["transition_prop_2"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part,
            LV_STYLE_TRANSITION_PROP_2 | (state << LV_STYLE_STATE_POS), c);
      }
      if (mix.contains("transition_prop_3")) {
        auto c = mix["transition_prop_3"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part,
            LV_STYLE_TRANSITION_PROP_3 | (state << LV_STYLE_STATE_POS), c);
      }
      if (mix.contains("transition_prop_4")) {
        auto c = mix["transition_prop_4"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part,
            LV_STYLE_TRANSITION_PROP_4 | (state << LV_STYLE_STATE_POS), c);
      }
      if (mix.contains("transition_prop_5")) {
        auto c = mix["transition_prop_5"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part,
            LV_STYLE_TRANSITION_PROP_5 | (state << LV_STYLE_STATE_POS), c);
      }
      if (mix.contains("transition_prop_6")) {
        auto c = mix["transition_prop_6"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part,
            LV_STYLE_TRANSITION_PROP_6 | (state << LV_STYLE_STATE_POS), c);
      }
    }
    if (style.contains("Scale")) {
      QJsonObject mix = style["Scale"].toObject();
      if (mix.contains("scale_grad_color")) {
        auto c = lvgl.fromColor(QColor(
            QString("#%1").arg(mix["scale_grad_color"].toString().mid(2, 7))));
        _lv_obj_set_style_local_color(
            m_obj, part,
            LV_STYLE_SCALE_GRAD_COLOR | (state << LV_STYLE_STATE_POS), c);
      }
      if (mix.contains("scale_end_color")) {
        auto c = lvgl.fromColor(QColor(
            QString("#%1").arg(mix["scale_end_color"].toString().mid(2, 7))));
        _lv_obj_set_style_local_color(
            m_obj, part,
            LV_STYLE_SCALE_END_COLOR | (state << LV_STYLE_STATE_POS), c);
      }
      if (mix.contains("scale_width")) {
        auto c = mix["scale_width"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part, LV_STYLE_SCALE_WIDTH | (state << LV_STYLE_STATE_POS),
            c);
      }
      if (mix.contains("scale_border_width")) {
        auto c = mix["scale_border_width"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part,
            LV_STYLE_SCALE_END_BORDER_WIDTH | (state << LV_STYLE_STATE_POS), c);
      }
      if (mix.contains("scale_end_border_width")) {
        auto c = mix["scale_end_border_width"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part,
            LV_STYLE_SCALE_BORDER_WIDTH | (state << LV_STYLE_STATE_POS), c);
      }
      if (mix.contains("scale_end_line_width")) {
        auto c = mix["scale_end_line_width"].toInt();
        _lv_obj_set_style_local_int(
            m_obj, part,
            LV_STYLE_SCALE_END_LINE_WIDTH | (state << LV_STYLE_STATE_POS), c);
      }
    }
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
  for (LVGLObject *o : lvgl.allObjects()) {
    if (o == this) continue;
    if (o->name() == m_name) return true;
  }
  return false;
}

bool LVGLObject::doesNameExists(const QString &name, LVGLObject *except) {
  for (LVGLObject *o : lvgl.allObjects()) {
    if (except && (except == o)) continue;
    if (o->name() == name) return true;
  }
  return false;
}

void LVGLObject::generateName() {
  // create sorted list by type
  QList<int> numbers;
  const QString prefix = m_widgetClass->name().toLower().replace(' ', '_');

  for (LVGLObject *o : lvgl.allObjects()) {
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
  const LVGLWidget *widgetClass = lvgl.widget(className);
  if (widgetClass) {
    LVGLObject *newObj;
    QString name = object["name"].toString();
    if (parent)
      newObj = new LVGLObject(widgetClass, name, parent);
    else
      newObj = new LVGLObject(widgetClass, name, lv_scr_act());
    // generate name if nessessary
    if (newObj->name().isEmpty() || doesNameExists(name, newObj))
      newObj->generateName();
    lvgl.addObject(newObj);

    if (className == "lv_roller" || className == "lv_page") {
      if (object.contains("styles"))
        newObj->parseStyles(object["styles"].toArray());

      for (LVGLProperty *p : widgetClass->properties()) {
        QString pname = p->codename().toLower();
        if (pname == "name") continue;
        if (object.contains(pname)) {
          QVariant v = object[pname].toVariant();
          if (!v.isNull()) p->setValue(newObj, v);
        }
      }

      if (object.contains("children")) {
        QJsonArray childs = object["children"].toArray();
        for (int i = 0; i < childs.size(); ++i) {
          QJsonObject child = childs[i].toObject();
          if (widgetClass->type() == LVGLWidget::TabView &&
              child.contains("index")) {
            LVGLObject *page = newObj->findChildByIndex(child["index"].toInt());
            page->setName(child["name"].toString());
            // parse styles
            if (child.contains("styles"))
              page->parseStyles(child["styles"].toArray());

            // parse page props
            for (LVGLProperty *p :
                 lvgl.widget("lv_page")->specialProperties()) {
              const QString pname = p->codename().toLower();
              if (child.contains(pname)) {
                QVariant v = child[pname].toVariant();
                if (!v.isNull()) p->setValue(page, v);
              }
            }
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

    } else {
      for (LVGLProperty *p : widgetClass->properties()) {
        QString pname = p->codename().toLower();
        if (pname == "name") continue;
        if (object.contains(pname)) {
          QVariant v = object[pname].toVariant();
          if (!v.isNull()) p->setValue(newObj, v);
        }
      }

      if (object.contains("children")) {
        QJsonArray childs = object["children"].toArray();
        for (int i = 0; i < childs.size(); ++i) {
          QJsonObject child = childs[i].toObject();
          if (widgetClass->type() == LVGLWidget::TabView &&
              child.contains("index")) {
            LVGLObject *page = newObj->findChildByIndex(child["index"].toInt());
            page->setName(child["name"].toString());
            // parse styles
            if (child.contains("styles"))
              page->parseStyles(child["styles"].toArray());

            // parse page props
            for (LVGLProperty *p :
                 lvgl.widget("lv_page")->specialProperties()) {
              const QString pname = p->codename().toLower();
              if (child.contains(pname)) {
                QVariant v = child[pname].toVariant();
                if (!v.isNull()) p->setValue(page, v);
              }
            }
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

      if (object.contains("styles"))
        newObj->parseStyles(object["styles"].toArray());
    }

    return newObj;
  }
  return nullptr;
}

void LVGLObject::parseProp(QJsonObject object, LVGLObject *obj) {
  const LVGLWidget *widgetClass = obj->widgetClass();
  for (LVGLProperty *p : widgetClass->properties()) {
    QString pname = p->codename().toLower();
    if (pname == "name") continue;
    if (object.contains(pname)) {
      QVariant v = object[pname].toVariant();
      if (!v.isNull()) p->setValue(obj, v);
    }
  }
}

void LVGLObject::parseStyle(QJsonObject object, LVGLObject *obj) {
  if (object.contains("styles")) obj->parseStyles(object["styles"].toArray());
}

int LVGLObject::index() const { return m_index; }

LVGLObject *LVGLObject::findChildByIndex(int index) const {
  for (LVGLObject *child : m_childs) {
    if (child->index() == index) return child;
  }
  return nullptr;
}
