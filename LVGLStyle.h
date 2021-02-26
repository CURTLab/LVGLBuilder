#ifndef LVGLSTYLE_H
#define LVGLSTYLE_H

#include <lvgl/lvgl.h>

#include <QList>
#include <QString>
#include <QVariant>
class LVGLObject;

namespace LVGL {
enum StylePart {
  None = 0x00,
  Mix = 0x01,
  Padding = 0x02,
  Margin = 0x04,
  Background = 0x08,
  Border = 0x10,
  Outline = 0x20,
  Shadow = 0x40,
  Pattern = 0x80,
  Value = 0x100,
  Text = 0x200,
  Line = 0x400,
  Image = 0x800,
  Transition = 0x1000,
  Scale = 0x2000,
  All = Mix | Padding | Margin | Background | Border | Outline | Shadow |
        Pattern | Value | Text | Line | Image | Transition | Scale,
  Body = Mix | Background | Border | Outline,
  ArcBG = Background | Line | Padding,
  ArcINDIC = Mix | Line | Padding,
  ArcKNOB = Background | Padding,
  Button = Mix | Background | Border | Outline | Shadow | Value | Pattern |
           Transition,
  BtnMatrixBTN = Background | Text,
  CalendarHead = Mix | Background | Padding | Margin | Text,
  Canvas = Image,
  ChartBG = Background | Text | Padding,
  ChartSERIESBG = Background | Line | Padding,
  ChartSERIES = Mix | Padding | Line | Background,
  ChartCURSOR = Line,
  Checkbox = Background | Text,
  Container = Background | Padding,
  CPickerBG = Mix | Padding | Background,
  CpickerKNOB = Mix | Padding | Background,
  DropdownMAIN = Mix | Background | Text,
  DropdownLIST = Background | Text,
  DropdownDCRLBAR = Background,
  DropdownSELECTED = Background,
  GaugeMAIN = Background | Text | Line | Scale,
  GaugeMAJOR = Line | Scale,
  GaugeNEEDLE = Line | Background,
  ImgMAIN = Background | Image | Padding,
  KeyboardBTN = Background | Text,
  Label = Background | Padding | Text,
  PageBG = Background | Padding,
  PageSCRL = Background | Padding,
  PageSCROLLBAR = Background | Padding,
  PageEDGEFLASH = Background,
  LinemeterMAIN = Background | Line | Scale,
  MsgboxMAIN = Background | Padding | Text,
  RollerMAIN = Body | Text,
  SliderBG = Background | Padding,
  SliderINDIC = Background,
  SliderKNOB = Background | Padding,
  SpinboxBG = Background | Text,
  SpinboxCURSOR = Text,
  TableCELL = Background | Line,
  TableTABBTN = Button | Text,
  WindowHEAD = Background | Text | Padding,
};
Q_DECLARE_FLAGS(StyleParts, StylePart)

}  // namespace LVGL
Q_DECLARE_OPERATORS_FOR_FLAGS(LVGL::StyleParts)

class LVGLStyleItem {
 public:
  enum Type {
    Color,
    Coord,
    Opacity,
    BorderPart,
    ShadowType,
    Font,
    Property,
    Graddir,
    Blendmode,
    BoolType,
    PointImg,
    PointChar,
    Align,
    TextDecor,
  };

  LVGLStyleItem(QString name, LVGL::StyleParts stylePart,
                LVGLStyleItem *parent = nullptr);
  LVGLStyleItem(QString name, Type type, lv_style_property_t prop,
                LVGL::StyleParts stylePart, LVGLStyleItem *parent = nullptr);
  LVGLStyleItem(QString name, LVGLStyleItem *parent = nullptr);
  ~LVGLStyleItem();

  QString name() const;

  const LVGLStyleItem *parent() const;
  int count() const;
  const LVGLStyleItem *child(int index) const;
  int row() const;
  int indexOf(const LVGLStyleItem *item) const;
  void addChild(LVGLStyleItem *child);

  Type type() const;

  lv_style_property_t prop() const;

  LVGL::StyleParts stylePart() const;

 protected:
  QString m_name;
  LVGLStyleItem *m_parent;
  Type m_type;
  size_t m_offset;
  lv_style_property_t m_prop;
  QList<LVGLStyleItem *> m_childs;
  LVGL::StyleParts m_stylePart;
};

class LVGLStyle : public LVGLStyleItem {
 public:
  LVGLStyle();
  ~LVGLStyle();

  QVariant get(const LVGLStyleItem *item) const;
  lv_border_side_t getBorderPart(const LVGLStyleItem *item) const;
  void set(const LVGLStyleItem *item, QVariant value);

  lv_style_t *style() const;
  void setStyle(lv_style_t *style);
  void setCurobj(lv_obj_t *obj) { m_curobj = obj; }
  void setState(lv_state_t state) { m_state = state; }
  void setPart(lv_obj_part_t part) { m_part = part; }

  static bool hasStyleChanged(const lv_style_t *style, const lv_style_t *base,
                              LVGL::StyleParts parts = LVGL::All);
  void changepart(lv_obj_part_t part) { m_part = part; }
  void changestate(lv_state_t state) { m_state = state; }
  void updateDate();
  void updateItems();
  void seteditableStyle(LVGL::StyleParts s) { m_editableStyles = s; }

 private:
  lv_style_t *m_style;
  lv_obj_t *m_curobj;
  lv_obj_part_t m_part;
  lv_state_t m_state;
  LVGL::StyleParts m_editableStyles;
  LVGLStyleItem *m_mixed;
  LVGLStyleItem *m_padding;
  LVGLStyleItem *m_margin;
  LVGLStyleItem *m_background;
  LVGLStyleItem *m_border;
  LVGLStyleItem *m_outline;
  LVGLStyleItem *m_shadow;
  LVGLStyleItem *m_pattern;
  LVGLStyleItem *m_value;
  LVGLStyleItem *m_text;
  LVGLStyleItem *m_line;
  LVGLStyleItem *m_image;
  LVGLStyleItem *m_transition;
  LVGLStyleItem *m_scale;
};

#endif  // LVGLSTYLE_H
