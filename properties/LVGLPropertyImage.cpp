#include "LVGLPropertyImage.h"

#include <QComboBox>

#include "MainWindow.h"
#include "core/LVGLCore.h"
#include "core/LVGLHelper.h"
#include "core/LVGLObject.h"
#include "core/LVGLTab.h"

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
  if (dsc) {
    auto str = lvgl.nameByImage(const_cast<lv_img_dsc_t *>(dsc));
    if (str.isEmpty()) {
      auto tabw = LVGLHelper::getInstance().getMainW()->getTabW();
      auto curindex = tabw->currentIndex();
      for (int i = 0; i < tabw->count(); ++i) {
        if (i != curindex) {
          auto tab = static_cast<LVGLTab *>(tabw->widget(i));
          auto imgs = tab->allImages();
          for (LVGLImageData *img : imgs) {
            if (img->img_des() == dsc) return img->name();
          }
        }
      }
    }
    return str;
  } else
    return "default";
}

void LVGLPropertyImage::setValue(LVGLObject *obj, QVariant value) {
  set(obj, lvgl.image(value.toString()));
}
