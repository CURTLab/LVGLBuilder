#include "LVGLPropertyColor.h"

#include <QColorDialog>
#include <QDebug>
#include <QHBoxLayout>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QToolButton>

#include "LVGLCore.h"

LVGLPropertyColor::LVGLPropertyColor(LVGLProperty *parent)
    : LVGLProperty(parent) {}

bool LVGLPropertyColor::hasEditor() const { return true; }

QWidget *LVGLPropertyColor::editor(QWidget *parent) {
  m_color = QColor(Qt::black);
  m_widget = new QColorDialog(parent);
  return m_widget;
}

void LVGLPropertyColor::updateEditor(LVGLObject *obj) {
  lv_color_t c = get(obj);
  m_widget->setCurrentColor(lvgl.toColor(c));
}

void LVGLPropertyColor::updateWidget(LVGLObject *obj) {
  QColor c = m_widget->selectedColor();
  set(obj, lvgl.fromColor(c));
}

QVariant LVGLPropertyColor::value(LVGLObject *obj) const {
  lv_color_t c = get(obj);
  return QVariant::fromValue<QColor>(lvgl.toColor(c));
}

void LVGLPropertyColor::setValue(LVGLObject *obj, QVariant value) {
  if (value.type() == QVariant::Map) {
    QVariantMap map = value.toMap();
#if LV_COLOR_DEPTH == 32
    lv_color_t c;
    c.ch.red = map["red"].toInt() & 0xff;
    c.ch.green = map["green"].toInt() & 0xff;
    c.ch.blue = map["blue"].toInt() & 0xff;
    c.ch.alpha = map["alpha"].toInt() & 0xff;
    set(obj, c);
#endif
  }
}

QJsonValue LVGLPropertyColor::toJson(LVGLObject *obj) const {
  lv_color_t c = get(obj);
  QJsonObject object(
      {{"red", c.ch.red}, {"green", c.ch.green}, {"blue", c.ch.blue}});

#if LV_COLOR_DEPTH == 32
  object.insert("alpha", c.ch.alpha);
#endif
  return object;
}
