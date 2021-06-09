#include "LVGLLine.h"

#include <QIcon>

#include "core/LVGLObject.h"
#include "lvgl/lvgl.h"
#include "properties/LVGLPropertyPoints.h"

class LVGLPropertyLineAutoSize : public LVGLPropertyBool {
 public:
  QString name() const { return QObject::tr("Auto size"); }
  QString codename() const { return "Auto size"; }

 protected:
  bool get(LVGLObject *obj) const { return lv_line_get_auto_size(obj->obj()); }
  void set(LVGLObject *obj, bool statue) {
    lv_line_set_auto_size(obj->obj(), statue);
  }
};

class LVGLPropertyLineInvertY : public LVGLPropertyBool {
 public:
  QString name() const { return QObject::tr("InvertY"); }
  QString codename() const { return "InvertY"; }

 protected:
  bool get(LVGLObject *obj) const { return lv_line_get_y_invert(obj->obj()); }
  void set(LVGLObject *obj, bool statue) {
    lv_line_set_y_invert(obj->obj(), statue);
  }
};

class LVGLPropertyLinePoints : public LVGLPropertyPoints {
 public:
  QString name() const { return QObject::tr("Points"); }
  QString codename() const { return "Points"; }

  QStringList function(LVGLObject *obj) const {
    lv_line_ext_t *ext =
        reinterpret_cast<lv_line_ext_t *>(lv_obj_get_ext_attr(obj->obj()));
    QStringList ret;
    QString var = obj->name() + "_points";
    ret << "static lv_point_t " + var + "[" + QString::number(ext->point_num) +
               "] = {";
    for (uint16_t i = 0; i < ext->point_num; ++i)
      ret << "\t{" + QString::number(ext->point_array[i].x) + "," +
                 QString::number(ext->point_array[i].y) + "},";
    ret << "\t};";
    ret << QString("lv_line_set_points(%1, %2, %3);")
               .arg(obj->codeName())
               .arg(var)
               .arg(ext->point_num);
    return ret;
  }

 protected:
  QVector<lv_point_t> get(LVGLObject *obj) const {
    lv_line_ext_t *ext =
        reinterpret_cast<lv_line_ext_t *>(lv_obj_get_ext_attr(obj->obj()));
    QVector<lv_point_t> ret(ext->point_num);
    memcpy(ret.data(), ext->point_array, ext->point_num * sizeof(lv_point_t));
    return ret;
  }
  void set(LVGLObject *obj, QVector<lv_point_t> data) {
    lv_line_ext_t *ext =
        reinterpret_cast<lv_line_ext_t *>(lv_obj_get_ext_attr(obj->obj()));
    if (ext->point_num && ext->point_array) delete[] ext->point_array;
    lv_point_t *points = new lv_point_t[static_cast<size_t>(data.size())];
    memcpy(points, data.data(),
           static_cast<size_t>(data.size()) * sizeof(lv_point_t));
    lv_line_set_points(obj->obj(), points, static_cast<uint16_t>(data.size()));
  }
};

LVGLLine::LVGLLine() {
  initStateStyles();
  m_parts << LV_LINE_PART_MAIN;
  m_properties << new LVGLPropertyLineAutoSize;
  m_properties << new LVGLPropertyLineInvertY;
  m_properties << new LVGLPropertyLinePoints;

  m_editableStyles << LVGL::Line;  // LV_LINE_PART_MAIN
}

QString LVGLLine::name() const { return QObject::tr("Line"); }

QString LVGLLine::className() const { return "lv_line"; }

LVGLWidget::Type LVGLLine::type() const { return Line; }

QIcon LVGLLine::icon() const { return QIcon(); }

lv_obj_t *LVGLLine::newObject(lv_obj_t *parent) const {
  lv_obj_t *obj = lv_line_create(parent, nullptr);
  return obj;
}

QSize LVGLLine::minimumSize() const { return QSize(200, 30); }

QStringList LVGLLine::styles() const {
  return QStringList() << "LINE_PART_MAIN";
}

lv_style_t *LVGLLine::style(lv_obj_t *obj, lv_obj_part_t part) const {
  return lv_obj_get_local_style(obj, part);
}

void LVGLLine::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const {
  lv_obj_add_style(obj, LV_BTN_PART_MAIN, style);
}

void LVGLLine::addStyle(lv_obj_t *obj, lv_style_t *style,
                        lv_obj_part_t part) const {
  lv_obj_add_style(obj, part, style);
}

void LVGLLine::initStateStyles() {
  for (int i = 0; i < 1; ++i) {
    lv_style_t *de = new lv_style_t;
    lv_style_t *ch = new lv_style_t;
    lv_style_t *fo = new lv_style_t;
    lv_style_t *ed = new lv_style_t;
    lv_style_t *ho = new lv_style_t;
    lv_style_t *pr = new lv_style_t;
    lv_style_t *di = new lv_style_t;
    lv_style_init(de);
    lv_style_init(ch);
    lv_style_init(fo);
    lv_style_init(ed);
    lv_style_init(ho);
    lv_style_init(pr);
    lv_style_init(di);
    QList<lv_style_t *> stateStyles;
    stateStyles << de << ch << fo << ed << ho << pr << di;
    m_partsStyles[i] = stateStyles;
  }
}
