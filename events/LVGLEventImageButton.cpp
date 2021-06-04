#include "LVGLEventImageButton.h"

#include "LVGLEventStateResume.h"
#include "MainWindow.h"
#include "core/LVGLCore.h"
#include "core/LVGLHelper.h"
#include "core/LVGLObject.h"
#include "core/LVGLTabWidget.h"
#include "lvgl/lvgl.h"

LVGLEventImageButton::LVGLEventImageButton() {}

void LVGLEventImageButton::eventRun(lv_obj_t *obj) {
  QString name = m_result.at(4);
  lv_obj_t *targert = nullptr;
  LVGLHelper::getInstance().updatetabDate();
  QList<LVGLObject *> objs;
  auto tabw = LVGLHelper::getInstance().getMainW()->getTabW();
  for (int i = 0; i < tabw->count(); ++i) {
    auto tab = static_cast<LVGLTabWidget *>(tabw->widget(i));
    auto os = tab->allObject();
    for (auto o : os) {
      if (o->obj() == obj) {
        objs = os;
        break;
      }
    }
  }

  for (auto o : objs) {
    if (o->name() == name) {
      targert = o->obj();
      LVGLEventStateResume::getInstance().addEvent(o);
      break;
    }
  }

  QString proprety = m_result.at(5);
  QString imgname = m_result.at(6);
  if (proprety == "Release") {
    auto dsc = lvgl.image(imgname);
    lv_imgbtn_set_src(targert, LV_BTN_STATE_RELEASED, dsc);
  } else if (proprety == "Pressed") {
    auto dsc = lvgl.image(imgname);
    lv_imgbtn_set_src(targert, LV_BTN_STATE_PRESSED, dsc);
  } else if (proprety == "Checked Release") {
    auto dsc = lvgl.image(imgname);
    lv_imgbtn_set_src(targert, LV_BTN_STATE_CHECKED_RELEASED, dsc);
  } else if (proprety == "Checked Pressed") {
    auto dsc = lvgl.image(imgname);
    lv_imgbtn_set_src(targert, LV_BTN_STATE_CHECKED_PRESSED, dsc);
  }
}

QStringList LVGLEventImageButton::eventCode() {
  QStringList list;
  list << QString("%1{").arg(m_eventHeadCode) << "\n";
  if (m_result[1] == "Pressed") {
    list << "\t"
         << "if (event == LV_EVENT_PRESSED){"
         << "\n";
  } else if (m_result[1] == "Clicked") {
    list << "\t"
         << "if (event == LV_EVENT_CLICKED){"
         << "\n";
  } else if (m_result[1] == "Long_Pressed") {
    list << "\t"
         << "if (event == LV_EVENT_LONG_PRESSED){"
         << "\n";
  } else if (m_result[1] == "Long_Pressed_Repeat") {
    list << "\t"
         << "if (event == LV_EVENT_LONG_PRESSED_REPEAT){"
         << "\n";
  } else if (m_result[1] == "Focused") {
    list << "\t"
         << "if (event == LV_EVENT_FOCUSED){"
         << "\n";
  } else if (m_result[1] == "Defocused") {
    list << "\t"
         << "if (event == LV_EVENT_DEFOCUSED){"
         << "\n";
  } else if (m_result[1] == "Value_Changed") {
    list << "\t"
         << "if (event == LV_EVENT_VALUE_CHANGED){"
         << "\n";
  }
  list << "\t\t";

  auto tabw = LVGLHelper::getInstance().getMainW()->getTabW();
  int tabindex = tabw->currentIndex();
  auto objs = lvgl.allObjects();
  auto nowtab = static_cast<LVGLTabWidget *>(tabw->widget(tabindex));
  LVGLWidget::Type type;
  QString codename;
  int nameindex = tabindex;

  nowtab->setAllObjects(lvgl.allObjects());
  nowtab->setAllImages(lvgl.allImages());

  QString name;
  QMap<lv_obj_t *, QList<LVGLEvent *>> &objevs =
      LVGLHelper::getInstance().getObjEvents();
  lv_obj_t *obb = nullptr;
  auto iter = objevs.begin();
  for (; iter != objevs.end(); ++iter) {
    QList<LVGLEvent *> &listev = iter.value();
    for (auto e : listev)
      if (e == this) {
        obb = iter.key();
        break;
      }
  }

  for (int i = 0; i < tabw->count(); ++i) {
    auto tb = static_cast<LVGLTabWidget *>(tabw->widget(i));
    auto os = tb->allObject();
    for (auto o : os) {
      if (obb == o->obj()) {
        tabindex = i;
        nameindex = i;
        codename = o->codeName();
        type = o->widgetType();
      }
    }
  }

  auto tb = static_cast<LVGLTabWidget *>(tabw->widget(nameindex));
  auto os = tb->allObject();
  for (auto o : os)
    if (o->name() == m_result[4]) name = o->codeName();

  QString proprety = m_result.at(5);
  QString picname = m_result.at(6).toLower();

  list << QString("LV_IMG_DECLARE(%1);\n").arg(picname);

  if (proprety == "Release") {
    list << "\t\t"
         << QString(
                "lv_imgbtn_set_src(%1_ev_%2, LV_BTN_STATE_RELEASED, &%3);\n")
                .arg(name)
                .arg(tabindex)
                .arg(picname);
  } else if (proprety == "Pressed") {
    list << "\t\t"
         << QString("lv_imgbtn_set_src(%1_ev_%2, LV_BTN_STATE_PRESSED, &%3);\n")
                .arg(name)
                .arg(tabindex)
                .arg(picname);
  } else if (proprety == "Checked Release") {
    list << "\t\t"
         << QString(
                "lv_imgbtn_set_src(%1_ev_%2, LV_BTN_STATE_CHECKED_RELEASED, "
                "&%3);\n")
                .arg(name)
                .arg(tabindex)
                .arg(picname);
  } else if (proprety == "Checked Pressed") {
    list << "\t\t"
         << QString(
                "lv_imgbtn_set_src(%1_ev_%2, LV_BTN_STATE_CHECKED_PRESSED, "
                "&%3);\n")
                .arg(name)
                .arg(tabindex)
                .arg(picname);
  }

  list << "\t}\n";
  list << "}";
  return list;
}
