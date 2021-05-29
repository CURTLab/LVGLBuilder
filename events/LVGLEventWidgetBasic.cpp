#include "LVGLEventWidgetBasic.h"

#include "LVGLEventStateResume.h"
#include "MainWindow.h"
#include "core/LVGLCore.h"
#include "core/LVGLHelper.h"
#include "core/LVGLObject.h"
#include "core/LVGLTabWidget.h"
#include "lvgl/lvgl.h"

LVGLEventWidgetBasic::LVGLEventWidgetBasic() {}

void LVGLEventWidgetBasic::eventRun(lv_obj_t *obj) {
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
  QString way = m_result.at(6);
  if (proprety == "X") {
    int x = m_result.at(7).toInt();
    lv_obj_set_x(targert, x);
  } else if (proprety == "Y") {
    int y = m_result.at(7).toInt();
    lv_obj_set_y(targert, y);
  } else if (proprety == "Width") {
    int w = m_result.at(7).toInt();
    lv_obj_set_width(targert, w);
  } else if (proprety == "Height") {
    int h = m_result.at(7).toInt();
    lv_obj_set_height(targert, h);
  } else if (proprety == "Clickable") {
    bool b = m_result.at(7).toInt() == 0 ? false : true;
    lv_obj_set_click(targert, b);
  } else if (proprety == "Dragable") {
    bool b = m_result.at(7).toInt() == 0 ? false : true;
    lv_obj_set_drag(targert, b);
  } else if (proprety == "Hidden") {
    bool b = m_result.at(7).toInt() == 0 ? false : true;
    lv_obj_set_hidden(targert, b);
  }
}

QStringList LVGLEventWidgetBasic::eventCode() {
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

  QString proprety = m_result.at(5);
  QString way = m_result.at(6);

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

  if (proprety == "X") {
    list << QString("lv_obj_set_x(%1_ev_%2, %3);\n")
                .arg(name)
                .arg(tabindex)
                .arg(m_result[7]);
  } else if (proprety == "Y") {
    list << QString("lv_obj_set_y(%1_ev_%2, %3);\n")
                .arg(name)
                .arg(tabindex)
                .arg(m_result[7]);
  } else if (proprety == "Width") {
    list << QString("lv_obj_set_width(%1_ev_%2, %3);\n")
                .arg(name)
                .arg(tabindex)
                .arg(m_result[7]);
  } else if (proprety == "Height") {
    list << QString("lv_obj_set_height(%1_ev_%2, %3);\n")
                .arg(name)
                .arg(tabindex)
                .arg(m_result[7]);
  } else if (proprety == "Clickable") {
    QString b = m_result.at(7).toInt() == 0 ? "false" : "true";
    list << QString("lv_obj_set_click(%1_ev_%2, %3);\n")
                .arg(name)
                .arg(tabindex)
                .arg(b);
  } else if (proprety == "Dragable") {
    bool b = m_result.at(7).toInt() == 0 ? false : true;
    list << QString("lv_obj_set_drag(%1_ev_%2, %3);\n")
                .arg(name)
                .arg(tabindex)
                .arg(b);
  } else if (proprety == "Hidden") {
    bool b = m_result.at(7).toInt() == 0 ? false : true;
    list << QString("lv_obj_set_hidden(%1_ev_%2, %3);\n")
                .arg(name)
                .arg(tabindex)
                .arg(b);
  }

  list << "\t}\n";
  list << "}";
  return list;
}
