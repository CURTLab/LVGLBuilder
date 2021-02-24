#ifndef LVGLWIDGET_H
#define LVGLWIDGET_H

#include <QJsonObject>
#include <QPixmap>

#include "LVGLProperty.h"
#include "LVGLStyle.h"

class LVGLWidget {
 public:
  LVGLWidget();
  virtual ~LVGLWidget();

  enum Type {
    Arc,
    Bar,
    Button,
    ButtonMatrix,
    Calendar,
    Canvas,
    CheckBox,
    Chart,
    Container,
    ColorPicker,
    DropDownList,
    Gauge,
    Image,
    ImageButton,
    ImageSlider,
    ImageBar,
    Keyboard,
    Label,
    LED,
    Line,
    List,
    LineMeter,
    MessageBox,
    ObjectMask,
    Page,
    Roller,
    Slider,
    SpinBox,
    Spinner,
    Switch,
    Table,
    TabView,
    TextArea,
    TileView,
    Window
  };

  virtual QString name() const = 0;
  virtual QString className() const = 0;
  virtual Type type() const = 0;
  virtual QIcon icon() const = 0;
  virtual QSize minimumSize() const = 0;
  virtual lv_obj_t *newObject(lv_obj_t *parent) const = 0;
  virtual QStringList styles() const = 0;
  virtual lv_style_t *style(lv_obj_t *obj,
                            lv_obj_part_t part = LV_OBJ_PART_MAIN) const = 0;
  virtual void setStyle(lv_obj_t *obj, int type, lv_style_t *style) const = 0;
  virtual void addStyle(lv_obj_t *obj, lv_style_t *style,
                        lv_obj_part_t part = LV_OBJ_PART_MAIN) const = 0;
  virtual void initStateStyles() = 0;

  virtual QPixmap preview() const;
  void setPreview(QPixmap preview);
  int getindexofparts(lv_obj_part_t part);

  QList<lv_obj_part_t> parts() const;

  QVector<LVGLProperty *> properties() const;
  QList<LVGLProperty *> specialProperties() const;

  LVGLProperty *property(QString name) const;

  LVGL::StyleParts editableStyles(int type) const;

  LVGLProperty *geometryProp() const;
  lv_style_t *getstyle(int partindex, int stateindex) const;

 protected:
  LVGLProperty *m_geometryProp;
  QVector<LVGLProperty *> m_properties;
  QList<LVGL::StyleParts> m_editableStyles;
  QPixmap m_preview;
  QList<lv_obj_part_t> m_parts;
  QMap<int, QList<lv_style_t *>> m_partsStyles;
};
Q_DECLARE_METATYPE(LVGLWidget *)
Q_DECLARE_METATYPE(const LVGLWidget *)

#endif  // LVGLWIDGET_H
