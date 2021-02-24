#include "LVGLStyle.h"

#include <QColor>
#include <QDebug>

#include "LVGLCore.h"
#include "LVGLObject.h"

using lv_font_ref = const lv_font_t*;

LVGLStyleItem::LVGLStyleItem(QString name, LVGL::StyleParts stylePart,
                             LVGLStyleItem* parent)
    : m_name(name),
      m_parent(parent),
      m_type(Property),
      m_offset(std::numeric_limits<size_t>::max()),
      m_stylePart(stylePart) {}

LVGLStyleItem::LVGLStyleItem(QString name, Type type, lv_style_property_t prop,
                             LVGL::StyleParts stylePart, LVGLStyleItem* parent)
    : m_name(name),
      m_parent(parent),
      m_type(type),
      m_prop(prop),
      m_stylePart(stylePart) {}

LVGLStyleItem::~LVGLStyleItem() {}

QString LVGLStyleItem::name() const { return m_name; }

const LVGLStyleItem* LVGLStyleItem::parent() const { return m_parent; }

int LVGLStyleItem::count() const { return m_childs.size(); }

const LVGLStyleItem* LVGLStyleItem::child(int index) const {
  return m_childs.at(index);
}

int LVGLStyleItem::row() const {
  return m_parent ? m_parent->indexOf(this) : 0;
}

int LVGLStyleItem::indexOf(const LVGLStyleItem* item) const {
  return m_childs.indexOf(const_cast<LVGLStyleItem*>(item));
}

void LVGLStyleItem::addChild(LVGLStyleItem* child) { m_childs.append(child); }

LVGLStyleItem::Type LVGLStyleItem::type() const { return m_type; }

lv_style_property_t LVGLStyleItem::prop() const { return m_prop; }

LVGL::StyleParts LVGLStyleItem::stylePart() const { return m_stylePart; }

LVGLStyle::LVGLStyle()
    : LVGLStyleItem("", LVGL::None),
      m_style(nullptr),
      m_part(LV_OBJ_PART_MAIN),
      m_state(LV_STATE_DEFAULT),
      m_mixed(nullptr),
      m_padding(nullptr),
      m_margin(nullptr),
      m_background(nullptr),
      m_border(nullptr),
      m_outline(nullptr),
      m_shadow(nullptr),
      m_pattern(nullptr),
      m_value(nullptr),
      m_text(nullptr),
      m_line(nullptr),
      m_image(nullptr),
      m_transition(nullptr),
      m_scale(nullptr) {
  m_mixed = new LVGLStyleItem("mixed", LVGL::Mix, this);
  m_mixed->addChild(
      new LVGLStyleItem("radius", Coord, LV_STYLE_RADIUS, LVGL::Mix, m_mixed));
  m_mixed->addChild(new LVGLStyleItem(
      "clip_corner", BoolType, LV_STYLE_CLIP_CORNER, LVGL::Mix, m_mixed));
  m_mixed->addChild(
      new LVGLStyleItem("size", Coord, LV_STYLE_SIZE, LVGL::Mix, m_mixed));
  m_mixed->addChild(new LVGLStyleItem(
      "transform_width", Coord, LV_STYLE_TRANSFORM_WIDTH, LVGL::Mix, m_mixed));
  m_mixed->addChild(new LVGLStyleItem("transform_height", Coord,
                                      LV_STYLE_TRANSFORM_HEIGHT, LVGL::Mix,
                                      m_mixed));
  m_mixed->addChild(new LVGLStyleItem(
      "transform_angle", Coord, LV_STYLE_TRANSFORM_ANGLE, LVGL::Mix, m_mixed));
  m_mixed->addChild(new LVGLStyleItem(
      "transform_zoom", Coord, LV_STYLE_TRANSFORM_ZOOM, LVGL::Mix, m_mixed));
  m_mixed->addChild(new LVGLStyleItem("opa_scale", Coord, LV_STYLE_OPA_SCALE,
                                      LVGL::Mix, m_mixed));

  m_padding = new LVGLStyleItem("padding", LVGL::Padding, this);
  m_padding->addChild(new LVGLStyleItem("pad_top", Coord, LV_STYLE_PAD_TOP,
                                        LVGL::Padding, m_padding));
  m_padding->addChild(new LVGLStyleItem(
      "pad_bottom", Coord, LV_STYLE_PAD_BOTTOM, LVGL::Padding, m_padding));
  m_padding->addChild(new LVGLStyleItem("pad_left", Coord, LV_STYLE_PAD_LEFT,
                                        LVGL::Padding, m_padding));
  m_padding->addChild(new LVGLStyleItem("pad_right", Coord, LV_STYLE_PAD_RIGHT,
                                        LVGL::Padding, m_padding));
  m_padding->addChild(new LVGLStyleItem("pad_inner", Coord, LV_STYLE_PAD_INNER,
                                        LVGL::Padding, m_padding));

  m_margin = new LVGLStyleItem("margin", LVGL::Margin, this);
  m_margin->addChild(new LVGLStyleItem("margin_top", Coord, LV_STYLE_MARGIN_TOP,
                                       LVGL::Margin, m_margin));
  m_margin->addChild(new LVGLStyleItem(
      "margin_bottom", Coord, LV_STYLE_MARGIN_BOTTOM, LVGL::Margin, m_margin));
  m_margin->addChild(new LVGLStyleItem(
      "margin_left", Coord, LV_STYLE_MARGIN_LEFT, LVGL::Margin, m_margin));
  m_margin->addChild(new LVGLStyleItem(
      "margin_right", Coord, LV_STYLE_MARGIN_RIGHT, LVGL::Margin, m_margin));

  m_background = new LVGLStyleItem("background", LVGL::Background, this);
  m_background->addChild(new LVGLStyleItem("bg_color", Color, LV_STYLE_BG_COLOR,
                                           LVGL::Background, m_background));
  m_background->addChild(new LVGLStyleItem("bg_opa", Opacity, LV_STYLE_BG_OPA,
                                           LVGL::Background, m_background));
  m_background->addChild(new LVGLStyleItem("bg_grad_color", Color,
                                           LV_STYLE_BG_GRAD_COLOR,
                                           LVGL::Background, m_background));
  m_background->addChild(new LVGLStyleItem("bg_main_stop", Coord,
                                           LV_STYLE_BG_MAIN_STOP,
                                           LVGL::Background, m_background));
  m_background->addChild(new LVGLStyleItem("bg_grad_stop", Coord,
                                           LV_STYLE_BG_GRAD_STOP,
                                           LVGL::Background, m_background));
  m_background->addChild(new LVGLStyleItem("bg_grad_dir", Graddir,
                                           LV_STYLE_BG_GRAD_DIR,
                                           LVGL::Background, m_background));
  m_background->addChild(new LVGLStyleItem("bg_blend_mode", Blendmode,
                                           LV_STYLE_BG_BLEND_MODE,
                                           LVGL::Background, m_background));

  m_border = new LVGLStyleItem("border", LVGL::Border, this);
  m_border->addChild(new LVGLStyleItem(
      "border_color", Color, LV_STYLE_BORDER_COLOR, LVGL::Border, m_border));
  m_border->addChild(new LVGLStyleItem(
      "border_opa", Opacity, LV_STYLE_BORDER_OPA, LVGL::Border, m_border));
  m_border->addChild(new LVGLStyleItem(
      "border_width", Coord, LV_STYLE_BORDER_WIDTH, LVGL::Border, m_border));
  m_border->addChild(new LVGLStyleItem(
      "border_side", BorderPart, LV_STYLE_BORDER_POST, LVGL::Border, m_border));
  m_border->addChild(new LVGLStyleItem(
      "border_post", BoolType, LV_STYLE_BORDER_POST, LVGL::Border, m_border));
  m_border->addChild(new LVGLStyleItem("border_blend_mode", Blendmode,
                                       LV_STYLE_BORDER_BLEND_MODE, LVGL::Border,
                                       m_border));

  m_outline = new LVGLStyleItem("outline", LVGL::Outline, this);
  m_outline->addChild(new LVGLStyleItem("outline_color", Color,
                                        LV_STYLE_OUTLINE_COLOR, LVGL::Outline,
                                        m_outline));
  m_outline->addChild(new LVGLStyleItem(
      "outline_opa", Opacity, LV_STYLE_OUTLINE_OPA, LVGL::Outline, m_outline));
  m_outline->addChild(new LVGLStyleItem("outline_width", Coord,
                                        LV_STYLE_OUTLINE_WIDTH, LVGL::Outline,
                                        m_outline));
  m_outline->addChild(new LVGLStyleItem(
      "outline_pad", Coord, LV_STYLE_OUTLINE_PAD, LVGL::Outline, m_outline));
  m_outline->addChild(new LVGLStyleItem("outline_blend_mode", Blendmode,
                                        LV_STYLE_OUTLINE_BLEND_MODE,
                                        LVGL::Outline, m_outline));

  m_shadow = new LVGLStyleItem("shadow", LVGL::Shadow, this);
  m_shadow->addChild(new LVGLStyleItem(
      "shadow_color", Color, LV_STYLE_SHADOW_COLOR, LVGL::Shadow, m_shadow));
  m_shadow->addChild(new LVGLStyleItem(
      "shadow_opa", Opacity, LV_STYLE_SHADOW_OPA, LVGL::Shadow, m_shadow));
  m_shadow->addChild(new LVGLStyleItem(
      "shadow_width", Coord, LV_STYLE_SHADOW_WIDTH, LVGL::Shadow, m_shadow));
  m_shadow->addChild(new LVGLStyleItem(
      "shadow_ofs_x", Coord, LV_STYLE_SHADOW_OFS_X, LVGL::Shadow, m_shadow));
  m_shadow->addChild(new LVGLStyleItem(
      "shadow_ofs_y", Coord, LV_STYLE_SHADOW_OFS_Y, LVGL::Shadow, m_shadow));
  m_shadow->addChild(new LVGLStyleItem(
      "shadow_spread", Coord, LV_STYLE_SHADOW_SPREAD, LVGL::Shadow, m_shadow));
  m_shadow->addChild(new LVGLStyleItem("shadow_blend_mode", Blendmode,
                                       LV_STYLE_SHADOW_BLEND_MODE, LVGL::Shadow,
                                       m_shadow));

  m_pattern = new LVGLStyleItem("pattern", LVGL::Pattern, this);
  m_pattern->addChild(new LVGLStyleItem("pattern_image", PointImg,
                                        LV_STYLE_PATTERN_IMAGE, LVGL::Pattern,
                                        m_pattern));
  m_pattern->addChild(new LVGLStyleItem(
      "pattern_opa", Opacity, LV_STYLE_PATTERN_OPA, LVGL::Pattern, m_pattern));
  m_pattern->addChild(new LVGLStyleItem("pattern_recolor", Color,
                                        LV_STYLE_PATTERN_RECOLOR, LVGL::Pattern,
                                        m_pattern));
  m_pattern->addChild(new LVGLStyleItem("pattern_recolor_opa", Opacity,
                                        LV_STYLE_PATTERN_RECOLOR_OPA,
                                        LVGL::Pattern, m_pattern));
  m_pattern->addChild(new LVGLStyleItem("pattern_repeat", BoolType,
                                        LV_STYLE_PATTERN_REPEAT, LVGL::Pattern,
                                        m_pattern));
  m_pattern->addChild(new LVGLStyleItem("pattern_blend_mode", Blendmode,
                                        LV_STYLE_PATTERN_BLEND_MODE,
                                        LVGL::Pattern, m_pattern));

  m_value = new LVGLStyleItem("value", LVGL::Value, this);
  m_value->addChild(new LVGLStyleItem(
      "value_str", PointChar, LV_STYLE_VALUE_STR, LVGL::Value, m_value));
  m_value->addChild(new LVGLStyleItem(
      "value_color", Color, LV_STYLE_VALUE_COLOR, LVGL::Value, m_value));
  m_value->addChild(new LVGLStyleItem("value_opa", Opacity, LV_STYLE_VALUE_OPA,
                                      LVGL::Value, m_value));
  m_value->addChild(new LVGLStyleItem("value_font", Font, LV_STYLE_VALUE_FONT,
                                      LVGL::Value, m_value));
  m_value->addChild(new LVGLStyleItem("value_letter_space", Coord,
                                      LV_STYLE_VALUE_LETTER_SPACE, LVGL::Value,
                                      m_value));
  m_value->addChild(new LVGLStyleItem("value_line_space", Coord,
                                      LV_STYLE_VALUE_LINE_SPACE, LVGL::Value,
                                      m_value));
  m_value->addChild(new LVGLStyleItem(
      "value_align", Align, LV_STYLE_VALUE_ALIGN, LVGL::Value, m_value));
  m_value->addChild(new LVGLStyleItem(
      "value_ofs_x", Coord, LV_STYLE_VALUE_OFS_X, LVGL::Value, m_value));
  m_value->addChild(new LVGLStyleItem(
      "value_ofs_y", Coord, LV_STYLE_VALUE_OFS_Y, LVGL::Value, m_value));
  m_value->addChild(new LVGLStyleItem("value_blend_mode", Blendmode,
                                      LV_STYLE_VALUE_BLEND_MODE, LVGL::Value,
                                      m_value));

  m_text = new LVGLStyleItem("text", LVGL::Text, this);
  m_text->addChild(new LVGLStyleItem("text_color", Color, LV_STYLE_TEXT_COLOR,
                                     LVGL::Text, m_text));
  m_text->addChild(new LVGLStyleItem("text_opa", Opacity, LV_STYLE_TEXT_OPA,
                                     LVGL::Text, m_text));
  m_text->addChild(new LVGLStyleItem("text_font", Font, LV_STYLE_TEXT_FONT,
                                     LVGL::Text, m_text));
  m_text->addChild(new LVGLStyleItem("text_letter_space", Coord,
                                     LV_STYLE_TEXT_LETTER_SPACE, LVGL::Text,
                                     m_text));
  m_text->addChild(new LVGLStyleItem(
      "text_line_space", Coord, LV_STYLE_TEXT_LINE_SPACE, LVGL::Text, m_text));
  m_text->addChild(new LVGLStyleItem("text_decor", TextDecor,
                                     LV_STYLE_TEXT_DECOR, LVGL::Text, m_text));
  m_text->addChild(new LVGLStyleItem(
      "text_sel_color", Color, LV_STYLE_TEXT_SEL_COLOR, LVGL::Text, m_text));
  m_text->addChild(new LVGLStyleItem("text_sel_bg_color", Color,
                                     LV_STYLE_TEXT_SEL_BG_COLOR, LVGL::Text,
                                     m_text));
  m_text->addChild(new LVGLStyleItem("text_blend_mode", Blendmode,
                                     LV_STYLE_TEXT_BLEND_MODE, LVGL::Text,
                                     m_text));

  m_line = new LVGLStyleItem("line", LVGL::Line, this);
  m_line->addChild(new LVGLStyleItem("line_color", Color, LV_STYLE_LINE_COLOR,
                                     LVGL::Line, m_line));
  m_line->addChild(new LVGLStyleItem("line_width", Coord, LV_STYLE_LINE_WIDTH,
                                     LVGL::Line, m_line));
  m_line->addChild(new LVGLStyleItem("line_opa", Opacity, LV_STYLE_LINE_OPA,
                                     LVGL::Line, m_line));
  m_line->addChild(new LVGLStyleItem(
      "line_dash_width", Coord, LV_STYLE_LINE_DASH_WIDTH, LVGL::Line, m_line));
  m_line->addChild(new LVGLStyleItem(
      "line_dash_gap", Coord, LV_STYLE_LINE_DASH_GAP, LVGL::Line, m_line));
  m_line->addChild(new LVGLStyleItem(
      "line_rounded", BoolType, LV_STYLE_LINE_ROUNDED, LVGL::Line, m_line));
  m_line->addChild(new LVGLStyleItem("line_blend_mode", Blendmode,
                                     LV_STYLE_LINE_BLEND_MODE, LVGL::Line,
                                     m_line));

  m_image = new LVGLStyleItem("image", LVGL::Image, this);
  m_image->addChild(new LVGLStyleItem(
      "image_recolor", Color, LV_STYLE_IMAGE_RECOLOR, LVGL::Image, m_image));
  m_image->addChild(new LVGLStyleItem("image_recolor_opa", Opacity,
                                      LV_STYLE_IMAGE_RECOLOR_OPA, LVGL::Image,
                                      m_image));
  m_image->addChild(new LVGLStyleItem("image_opa", Opacity, LV_STYLE_IMAGE_OPA,
                                      LVGL::Image, m_image));
  m_image->addChild(new LVGLStyleItem("image_blend_mode", Blendmode,
                                      LV_STYLE_IMAGE_BLEND_MODE, LVGL::Image,
                                      m_image));

  m_transition = new LVGLStyleItem("transition", LVGL::Transition, this);
  m_transition->addChild(new LVGLStyleItem("transition_time", Coord,
                                           LV_STYLE_TRANSITION_TIME,
                                           LVGL::Transition, m_transition));
  m_transition->addChild(new LVGLStyleItem("transition_delay", Coord,
                                           LV_STYLE_TRANSITION_DELAY,
                                           LVGL::Transition, m_transition));
  m_transition->addChild(new LVGLStyleItem("transition_prop_1", Coord,
                                           LV_STYLE_TRANSITION_PROP_1,
                                           LVGL::Transition, m_transition));
  m_transition->addChild(new LVGLStyleItem("transition_prop_2", Coord,
                                           LV_STYLE_TRANSITION_PROP_2,
                                           LVGL::Transition, m_transition));
  m_transition->addChild(new LVGLStyleItem("transition_prop_3", Coord,
                                           LV_STYLE_TRANSITION_PROP_3,
                                           LVGL::Transition, m_transition));
  m_transition->addChild(new LVGLStyleItem("transition_prop_4", Coord,
                                           LV_STYLE_TRANSITION_PROP_4,
                                           LVGL::Transition, m_transition));
  m_transition->addChild(new LVGLStyleItem("transition_prop_5", Coord,
                                           LV_STYLE_TRANSITION_PROP_5,
                                           LVGL::Transition, m_transition));
  m_transition->addChild(new LVGLStyleItem("transition_prop_6", Coord,
                                           LV_STYLE_TRANSITION_PROP_6,
                                           LVGL::Transition, m_transition));

  m_scale = new LVGLStyleItem("scale", LVGL::Scale, this);
  m_scale->addChild(new LVGLStyleItem("scale_grad_color", Color,
                                      LV_STYLE_SCALE_GRAD_COLOR, LVGL::Scale,
                                      m_scale));
  m_scale->addChild(new LVGLStyleItem("scale_end_color", Color,
                                      LV_STYLE_SCALE_END_COLOR, LVGL::Scale,
                                      m_scale));
  m_scale->addChild(new LVGLStyleItem(
      "scale_width", Coord, LV_STYLE_SCALE_WIDTH, LVGL::Scale, m_scale));
  m_scale->addChild(new LVGLStyleItem("scale_border_width", Coord,
                                      LV_STYLE_SCALE_BORDER_WIDTH, LVGL::Scale,
                                      m_scale));
  m_scale->addChild(new LVGLStyleItem("scale_end_border_width", Coord,
                                      LV_STYLE_SCALE_END_BORDER_WIDTH,
                                      LVGL::Scale, m_scale));
  m_scale->addChild(new LVGLStyleItem("scale_end_line_width", Coord,
                                      LV_STYLE_SCALE_END_LINE_WIDTH,
                                      LVGL::Scale, m_scale));
}

LVGLStyle::~LVGLStyle() {
  delete m_mixed;
  delete m_padding;
  delete m_margin;
  delete m_background;
  delete m_border;
  delete m_outline;
  delete m_shadow;
  delete m_pattern;
  delete m_value;
  delete m_text;
  delete m_line;
  delete m_image;
  delete m_transition;
  delete m_scale;
}

QVariant LVGLStyle::get(const LVGLStyleItem* item) const {
  if ((m_curobj == nullptr) || (item->type() == Property)) return QVariant();
  auto type = item->prop();
  if (item->type() == Coord) {
    lv_coord_t c = _lv_obj_get_style_int(
        m_curobj, m_part, (m_state << LV_STYLE_STATE_POS) | type);
    return QVariant(c);
  } else if (item->type() == Opacity) {
    lv_opa_t c = _lv_obj_get_style_opa(m_curobj, m_part,
                                       (m_state << LV_STYLE_STATE_POS) | type);
    return QVariant(c);
  } else if (item->type() == Color) {
    lv_color_t c = _lv_obj_get_style_color(
        m_curobj, m_part, (m_state << LV_STYLE_STATE_POS) | type);
    return lvgl->toColor(c);
  } else if (item->type() == BorderPart) {
    lv_border_side_t c = _lv_obj_get_style_int(
        m_curobj, m_part, (m_state << LV_STYLE_STATE_POS) | type);
    QStringList props;
    if (c == LV_BORDER_SIDE_NONE)
      return "None";
    else if (c == LV_BORDER_SIDE_FULL)
      return "Full";
    else if (c == LV_BORDER_SIDE_INTERNAL)
      return "Internal";

    if (c & 0x01) props << "Bottom";
    if (c & 0x02) props << "Top";
    if (c & 0x04) props << "Left";
    if (c & 0x08) props << "Right";
    return props.join(" | ");
  } else if (item->type() == Font) {
    const lv_font_t* c = (lv_font_t*)_lv_obj_get_style_ptr(
        m_curobj, m_part, (m_state << LV_STYLE_STATE_POS) | type);
    const int index = lvgl->indexOfFont(c);
    if (index == -1) {
      const void* f = (void*)lvgl->font("lv_font_chinese_16");
      _lv_obj_set_style_local_ptr(m_curobj, m_part,
                                  (m_state << LV_STYLE_STATE_POS) | type, f);
      c = (lv_font_t*)_lv_obj_get_style_ptr(
          m_curobj, m_part, (m_state << LV_STYLE_STATE_POS) | type);
      const int index2 = lvgl->indexOfFont(c);
      return lvgl->fontNames().at(index2);
    } else
      return lvgl->fontNames().at(index);
  } else if (item->type() == BoolType) {
    bool b = _lv_obj_get_style_int(m_curobj, m_part,
                                   (m_state << LV_STYLE_STATE_POS) | type);
    return b;
  } else if (item->type() == Blendmode) {
    lv_blend_mode_t c = _lv_obj_get_style_int(
        m_curobj, m_part, (m_state << LV_STYLE_STATE_POS) | type);
    switch (c) {
      case LV_BLEND_MODE_NORMAL:
        return "Normal";
      case LV_BLEND_MODE_ADDITIVE:
        return "Additive";
      case LV_BLEND_MODE_SUBTRACTIVE:
        return "SubtrActive";
      default:
        return "Normal";
    }
  } else if (item->type() == Graddir) {
    lv_grad_dir_t c = _lv_obj_get_style_int(
        m_curobj, m_part, (m_state << LV_STYLE_STATE_POS) | type);
    switch (c) {
      case LV_GRAD_DIR_NONE:
        return "None";
      case LV_GRAD_DIR_VER:
        return "Vertical ";
      case LV_GRAD_DIR_HOR:
        return "Horizontal ";
      default:
        return "None";
    }
  } else if (item->type() == PointChar) {
    const char* c = (const char*)_lv_obj_get_style_ptr(
        m_curobj, m_part, (m_state << LV_STYLE_STATE_POS) | type);
    QString text(c);
    return text;
  }
  return QVariant();
}

lv_border_side_t LVGLStyle::getBorderPart(const LVGLStyleItem* item) const {
  if (item->type() == BorderPart)
    return lv_obj_get_style_border_side(m_curobj, m_part);
  return 0;
}

template <class T, class P>
void set_helper(P value, size_t offset, lv_style_t* style) {
  T& c = *reinterpret_cast<T*>(reinterpret_cast<uint8_t*>(style) + offset);
  c = static_cast<T>(value);
}

void LVGLStyle::set(const LVGLStyleItem* item, QVariant value) {
  if ((item->type() == Property)) return;
  auto type = item->prop();

  if (item->type() == Coord) {
    lv_coord_t c = value.toInt();
    _lv_obj_set_style_local_int(m_curobj, m_part,
                                type | (m_state << LV_STYLE_STATE_POS), c);
  } else if (item->type() == Opacity) {
    lv_opa_t c = value.toInt();

    _lv_obj_set_style_local_opa(m_curobj, m_part,
                                type | (m_state << LV_STYLE_STATE_POS), c);
  } else if (item->type() == Color) {
    lv_color_t c = lvgl->fromColor(value);
    _lv_obj_set_style_local_color(m_curobj, m_part,
                                  type | (m_state << LV_STYLE_STATE_POS), c);
  } else if (item->type() == Font) {
    const lv_font_t* font = lvgl->font(value.toString());
    _lv_obj_set_style_local_ptr(m_curobj, m_part,
                                type | (m_state << LV_STYLE_STATE_POS), font);
  } else if (item->type() == BorderPart) {
    lv_border_side_t side = value.toUInt();
    _lv_obj_set_style_local_int(m_curobj, m_part,
                                type | (m_state << LV_STYLE_STATE_POS), side);
  } else if (item->type() == BoolType) {
    bool b = value.toBool();
    _lv_obj_set_style_local_int(m_curobj, m_part,
                                type | (m_state << LV_STYLE_STATE_POS), b);
  } else if (item->type() == Blendmode) {
    lv_blend_mode_t c = value.toInt();
    _lv_obj_set_style_local_int(m_curobj, m_part,
                                type | (m_state << LV_STYLE_STATE_POS), c);
  } else if (item->type() == Graddir) {
    lv_grad_dir_t c = value.toInt();
    lv_obj_set_style_local_bg_grad_dir(m_curobj, m_part, m_state, c);
    _lv_obj_set_style_local_int(m_curobj, m_part,
                                type | (m_state << LV_STYLE_STATE_POS), c);
  } else if (item->type() == PointChar) {
    auto size = value.toString().toUtf8().size();
    char* p = new char(size + 1);
    memcpy(p, value.toString().toUtf8().data(), size);
    p[size] = '\0';
    _lv_obj_set_style_local_ptr(m_curobj, m_part,
                                type | (m_state << LV_STYLE_STATE_POS), p);
  }
}

lv_style_t* LVGLStyle::style() const { return m_style; }

void LVGLStyle::setStyle(lv_style_t* style) { m_style = style; }

bool LVGLStyle::hasStyleChanged(const lv_style_t* style, const lv_style_t* base,
                                LVGL::StyleParts parts) {
  //  if (parts & LVGL::Background) {
  //    if (!axs_style_equal_bg_color(style, base))
  //      return true;
  //    else if (axs_style_equal_bg_grad_color(style, base))
  //      return true;
  //    else if (axs_style_equal_radius(style, base))
  //      return true;
  //    else if (axs_style_equal_bg_opa(style, base))
  //      return true;
  //    if (parts & LVGL::Border) {
  //      if (axs_style_equal_border_color(style, base))
  //        return true;
  //      else if (axs_style_equal_border_width(style, base))
  //        return true;
  //      else if (axs_style_equal_border_side(style, base))
  //        return true;
  //      else if (axs_style_equal_bg_opa(style, base))
  //        return true;
  //    }
  //    if (parts & LVGL::Shadow) {
  //      if (axs_style_equal_shadow_color(style, base))
  //        return true;
  //      else if (axs_style_equal_shadow_width(style, base))
  //        return true;
  //    }
  //    if (parts & LVGL::Padding) {
  //      if (axs_style_equal_pad_top(style, base))
  //        return true;
  //      else if (axs_style_equal_pad_bottom(style, base))
  //        return true;
  //      else if (axs_style_equal_pad_left(style, base))
  //        return true;
  //      else if (axs_style_equal_pad_right(style, base))
  //        return true;
  //      else if (axs_style_equal_pad_inner(style, base))
  //        return true;
  //    }
  //  }
  //  if (parts & LVGL::Text) {
  //    if (axs_style_equal_text_color(style, base))
  //      return true;
  //    else if (axs_style_equal_text_sel_color(style, base))
  //      return true;
  //    else if (axs_style_equal_text_font(style, base))
  //      return true;
  //    else if (axs_style_equal_text_letter_space(style, base))
  //      return true;
  //    else if (axs_style_equal_text_line_space(style, base))
  //      return true;
  //    else if (axs_style_equal_text_opa(style, base))
  //      return true;
  //  }
  //  if (parts & LVGL::Image) {
  //    if (axs_style_equal_image_recolor(style, base))
  //      return true;
  //    else if (axs_style_equal_image_opa(style, base))
  //      return true;
  //  }
  //  if (parts & LVGL::Line) {
  //    if (axs_style_equal_line_color(style, base))
  //      return true;
  //    else if (axs_style_equal_line_width(style, base))
  //      return true;
  //    else if (axs_style_equal_line_opa(style, base))
  //      return true;
  //  }
  return false;
}

void LVGLStyle::updateDate() { lv_obj_add_style(m_curobj, m_part, m_style); }

void LVGLStyle::updateItems() {
  m_childs.clear();
  if (m_editableStyles & LVGL::Mix) m_childs << m_mixed;
  if (m_editableStyles & LVGL::Padding) m_childs << m_padding;
  if (m_editableStyles & LVGL::Margin) m_childs << m_margin;
  if (m_editableStyles & LVGL::Background) m_childs << m_background;
  if (m_editableStyles & LVGL::Border) m_childs << m_border;
  if (m_editableStyles & LVGL::Outline) m_childs << m_outline;
  if (m_editableStyles & LVGL::Shadow) m_childs << m_shadow;
  if (m_editableStyles & LVGL::Pattern) m_childs << m_pattern;
  if (m_editableStyles & LVGL::Value) m_childs << m_value;
  if (m_editableStyles & LVGL::Text) m_childs << m_text;
  if (m_editableStyles & LVGL::Line) m_childs << m_line;
  if (m_editableStyles & LVGL::Image) m_childs << m_image;
  if (m_editableStyles & LVGL::Transition) m_childs << m_transition;
  if (m_editableStyles & LVGL::Scale) m_childs << m_scale;
}
