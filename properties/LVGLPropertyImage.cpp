#include "LVGLPropertyImage.h"

#include <QComboBox>

#include "LVGLCore.h"
#include "LVGLObject.h"

LVGLPropertyImage::LVGLPropertyImage(LVGLProperty *parent)
    : LVGLProperty(parent) {}

bool LVGLPropertyImage::hasEditor() const { return true; }

QWidget *LVGLPropertyImage::editor(QWidget *parent) {
  m_widget = new QComboBox(parent);
  m_widget->addItem("default");
  m_widget->addItems(lvgl.imageNames());
  return m_widget;
}

void LVGLPropertyImage::updateEditor(LVGLObject *obj) {
  m_widget->setCurrentText(value(obj).toString());
}

void LVGLPropertyImage::updateWidget(LVGLObject *obj) {
  set(obj, lvgl.image(m_widget->currentText()));
}

QVariant LVGLPropertyImage::value(LVGLObject *obj) const {
  const lv_img_dsc_t *dsc = get(obj);
  if (dsc)
    return lvgl.nameByImage(const_cast<lv_img_dsc_t *>(dsc));
  else
    return "default";
}

void LVGLPropertyImage::setValue(LVGLObject *obj, QVariant value) {
  set(obj, lvgl.image(value.toString()));
}
