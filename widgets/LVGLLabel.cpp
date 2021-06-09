#include "LVGLLabel.h"

#include <QIcon>

#include "core/LVGLHelper.h"
#include "core/LVGLObject.h"
#include "properties/LVGLPropertyColor.h"

class LVGLPropertyLabelAlign : public LVGLPropertyEnum {
 public:
  LVGLPropertyLabelAlign()
      : LVGLPropertyEnum(QStringList()
                         << QObject::tr("Left") << QObject::tr("Center")
                         << QObject::tr("Right")),
        m_values({"LV_LABEL_ALIGN_LEFT", "LV_LABEL_ALIGN_CENTER",
                  "LV_LABEL_ALIGN_RIGHT"}) {}

  QString name() const { return QObject::tr("Align"); }
  QString codename() const { return "Align"; }

  QStringList function(LVGLObject *obj) const {
    if (get(obj) == LV_LABEL_ALIGN_LEFT) return QStringList();
    return QStringList() << QString("lv_label_set_align(%1, %2);")
                                .arg(obj->codeName())
                                .arg(m_values.at(get(obj)));
  }

 protected:
  int get(LVGLObject *obj) const { return lv_label_get_align(obj->obj()); }
  void set(LVGLObject *obj, int index) {
    lv_label_set_align(obj->obj(), index & 0xff);
  }

  QStringList m_values;
};

class LVGLPropertyLabelLongMode : public LVGLPropertyEnum {
 public:
  LVGLPropertyLabelLongMode()
      : LVGLPropertyEnum(QStringList()
                         << QObject::tr("Expand") << QObject::tr("Break")
                         << QObject::tr("Dot") << QObject::tr("Scroll")
                         << QObject::tr("Circular scroll")
                         << QObject::tr("Corp")),
        m_values({"LV_LABEL_LONG_EXPAND", "LV_LABEL_LONG_BREAK",
                  "LV_LABEL_LONG_DOT", "LV_LABEL_LONG_SROLL",
                  "LV_LABEL_LONG_SROLL_CIRC", "LV_LABEL_LONG_CROP"}) {}

  QString name() const { return QObject::tr("Long mode"); }
  QString codename() const { return "Long mode"; }

  QStringList function(LVGLObject *obj) const {
    if (get(obj) == LV_LABEL_LONG_EXPAND) return QStringList();
    return QStringList() << QString("lv_label_set_long_mode(%1, %2);")
                                .arg(obj->codeName())
                                .arg(m_values.at(get(obj)));
  }

 protected:
  int get(LVGLObject *obj) const { return lv_label_get_long_mode(obj->obj()); }
  void set(LVGLObject *obj, int index) {
    lv_label_set_long_mode(obj->obj(), index & 0xff);
  }

  QStringList m_values;
};

class LVGLPropertyLabelRecolor : public LVGLPropertyBool {
 public:
  QString name() const { return QObject::tr("Recolor"); }
  QString codename() const { return "Recolor"; }

  QStringList function(LVGLObject *obj) const {
    if (!get(obj)) return QStringList();
    return QStringList() << QString("lv_label_set_recolor(%1, %2);")
                                .arg(obj->codeName())
                                .arg(QVariant(get(obj)).toString());
  }

 protected:
  bool get(LVGLObject *obj) const { return lv_label_get_recolor(obj->obj()); }
  void set(LVGLObject *obj, bool boolean) {
    lv_label_set_recolor(obj->obj(), boolean);
  }
};

class LVGLPropertyLabelText : public LVGLPropertyStringPlus {
 public:
  QString name() const { return QObject::tr("Text"); }
  QString codename() const { return "Text"; }

  QStringList function(LVGLObject *obj) const {
    QString tmp = get(obj);
    QStringList list;
    if (!tmp.isEmpty()) {
      QString str;
      for (auto c : tmp) {
        if (c != '\n')
          str += c;
        else
          str += "\\n";
      }
      QString result = LVGLHelper::getInstance().getStringWithSymbol(str);
      list << QString("lv_label_set_text(%1,\"%2\");")
                  .arg(obj->codeName())
                  .arg(result);
    }
    return list;
  }

 protected:
  QString get(LVGLObject *obj) const {
    if (!havasyb)
      return lv_label_get_text(obj->obj());
    else
      return m_string;
  }
  void set(LVGLObject *obj, QString string) {
    if (string.contains("(LV_S_")) {
      havasyb = true;
      m_string = string;
      QString result = LVGLHelper::getInstance().getStringWithSymbol(string);
      lv_label_set_text(obj->obj(), result.toUtf8().data());
    } else {
      havasyb = false;
      lv_label_set_text(obj->obj(), string.toUtf8().data());
    }
  }

 private:
  bool havasyb = false;
  QString m_string;
};

LVGLLabel::LVGLLabel() {
  initStateStyles();
  m_parts << LV_LABEL_PART_MAIN;
  m_properties << new LVGLPropertyLabelText;
  m_properties << new LVGLPropertyLabelAlign;
  m_properties << new LVGLPropertyLabelLongMode;
  m_properties << new LVGLPropertyLabelRecolor;

  // swap geometry in order to stop autosize
  const int index = m_properties.indexOf(m_geometryProp);
  qSwap(m_properties[index], m_properties.last());

  m_editableStyles << LVGL::Label;  // LV_LABEL_PART_MAIN
}

QString LVGLLabel::name() const { return QObject::tr("Label"); }

QString LVGLLabel::className() const { return "lv_label"; }

LVGLWidget::Type LVGLLabel::type() const { return Label; }

QIcon LVGLLabel::icon() const { return QIcon(); }

lv_obj_t *LVGLLabel::newObject(lv_obj_t *parent) const {
  lv_obj_t *obj = lv_label_create(parent, nullptr);
  return obj;
}

QSize LVGLLabel::minimumSize() const { return QSize(100, 35); }

QStringList LVGLLabel::styles() const {
  return QStringList() << "LABEL_PART_MAIN";
}
lv_style_t *LVGLLabel::style(lv_obj_t *obj, lv_obj_part_t part) const {
  return lv_obj_get_local_style(obj, part);
}

void LVGLLabel::setStyle(lv_obj_t *obj, int type, lv_style_t *style) const {
  lv_obj_add_style(obj, LV_BTN_PART_MAIN, style);
}

void LVGLLabel::addStyle(lv_obj_t *obj, lv_style_t *style,
                         lv_obj_part_t part) const {
  lv_obj_add_style(obj, part, style);
}

void LVGLLabel::initStateStyles() {
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
