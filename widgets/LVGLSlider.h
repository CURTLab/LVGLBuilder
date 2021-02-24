#ifndef LVGLSLIDER_H
#define LVGLSLIDER_H

#include "LVGLWidget.h"

class LVGLSlider : public LVGLWidget {
 public:
  LVGLSlider();

  virtual QString name() const;
  virtual QString className() const;
  virtual Type type() const;
  virtual QIcon icon() const;
  virtual lv_obj_t *newObject(lv_obj_t *parent) const;
  virtual QSize minimumSize() const;
  virtual QStringList styles() const;
  virtual lv_style_t *style(lv_obj_t *obj,
                            lv_obj_part_t part = LV_OBJ_PART_MAIN) const;
  virtual void setStyle(lv_obj_t *obj, int type, lv_style_t *style) const;
  virtual void addStyle(lv_obj_t *obj, lv_style_t *style,
                        lv_obj_part_t part = LV_OBJ_PART_MAIN) const;
  virtual void initStateStyles();
};

#endif  // LVGLSLIDER_H
