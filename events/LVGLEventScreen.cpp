#include "LVGLEventScreen.h"

#include "LVGLHelper.h"
#include "LVGLTabWidget.h"
#include "MainWindow.h"

LVGLEventScreen::LVGLEventScreen() {}

void LVGLEventScreen::eventRun(lv_obj_t *obj) {
  auto name = m_result[3];
  auto tab = LVGLHelper::getInstance().getMainW()->getTabW();
  int index = 0;
  for (; index < tab->count(); ++index)
    if (name == tab->tabText(index)) break;
  auto tabw = static_cast<LVGLTabWidget *>(tab->widget(index));
  lv_obj_t *screen = tabw->getparent();
  lv_scr_load_anim_t t = LV_SCR_LOAD_ANIM_NONE;

  if (m_result[4] == "Move Left")
    t = LV_SCR_LOAD_ANIM_MOVE_LEFT;
  else if (m_result[4] == "Move Right")
    t = LV_SCR_LOAD_ANIM_MOVE_RIGHT;
  else if (m_result[4] == "Move Top")
    t = LV_SCR_LOAD_ANIM_MOVE_TOP;
  else if (m_result[4] == "Move Bottom")

    t = LV_SCR_LOAD_ANIM_MOVE_BOTTOM;
  else if (m_result[4] == "Over Left")
    t = LV_SCR_LOAD_ANIM_OVER_LEFT;
  else if (m_result[4] == "Over Right")
    t = LV_SCR_LOAD_ANIM_OVER_RIGHT;
  else if (m_result[4] == "Over Top")
    t = LV_SCR_LOAD_ANIM_OVER_TOP;
  else if (m_result[4] == "Over Bottom")
    t = LV_SCR_LOAD_ANIM_OVER_BOTTOM;
  else if (m_result[4] == "Fade On")
    t = LV_SCR_LOAD_ANIM_FADE_ON;

  int speed = m_result[5].toInt();
  int delay = m_result[6].toInt();

  lv_scr_load_anim(screen, t, speed, delay, false);
}

QStringList LVGLEventScreen::objCode(const QString &objName) {
  QStringList list;
  QString str =
      QString("lv_obj_set_event_cb(%1,%2);").arg(objName).arg(m_result[0]);
  list << "\t" << str << "\n";
  return list;
}

QString LVGLEventScreen::eventHeadCode() {
  m_eventHeadCode =
      QString("void %1(lv_obj_t *obj, lv_event_t event)").arg(m_result[0]);
  return m_eventHeadCode + ";\n";
}

QStringList LVGLEventScreen::eventCode() {
  QStringList list;
  list << QString("%1{").arg(m_eventHeadCode) << "\n";
  if (m_result[1] == "Pressed") {
    list << "\t"
         << "if (event == LV_EVENT_PRESSED)"
         << "\n";
  } else if (m_result[1] == "Clicked") {
    list << "\t"
         << "if (event == LV_EVENT_CLICKED)"
         << "\n";
  } else if (m_result[1] == "Long_Pressed") {
    list << "\t"
         << "if (event == LV_EVENT_LONG_PRESSED)"
         << "\n";
  } else if (m_result[1] == "Long_Pressed_Repeat") {
    list << "\t"
         << "if (event == LV_EVENT_LONG_PRESSED_REPEAT)"
         << "\n";
  } else if (m_result[1] == "Focused") {
    list << "\t"
         << "if (event == LV_EVENT_FOCUSED)"
         << "\n";
  } else if (m_result[1] == "Defocused") {
    list << "\t"
         << "if (event == LV_EVENT_DEFOCUSED)"
         << "\n";
  } else if (m_result[1] == "Value_Changed") {
    list << "\t"
         << "if (event == LV_EVENT_VALUE_CHANGED)"
         << "\n";
  }

  auto name = m_result[3];
  auto tab = LVGLHelper::getInstance().getMainW()->getTabW();
  int index = 0;
  for (; index < tab->count(); ++index)
    if (name == tab->tabText(index)) break;

  QString t = "LV_SCR_LOAD_ANIM_NONE";

  if (m_result[4] == "Move Left")
    t = "LV_SCR_LOAD_ANIM_MOVE_LEFT";
  else if (m_result[4] == "Move Right")
    t = "LV_SCR_LOAD_ANIM_MOVE_RIGHT";
  else if (m_result[4] == "Move Top")
    t = "LV_SCR_LOAD_ANIM_MOVE_TOP";
  else if (m_result[4] == "Move Bottom")
    t = "LV_SCR_LOAD_ANIM_MOVE_BOTTOM";
  else if (m_result[4] == "Over Left")
    t = "LV_SCR_LOAD_ANIM_OVER_LEFT";
  else if (m_result[4] == "Over Right")
    t = "LV_SCR_LOAD_ANIM_OVER_RIGHT";
  else if (m_result[4] == "Over Top")
    t = "LV_SCR_LOAD_ANIM_OVER_TOP";
  else if (m_result[4] == "Over Bottom")
    t = "LV_SCR_LOAD_ANIM_OVER_BOTTOM";
  else if (m_result[4] == "Fade On")
    t = "LV_SCR_LOAD_ANIM_FADE_ON";

  list << "\t\t"
       << QString("lv_scr_load_anim(page%1, %2, %3, %4, false);")
              .arg(index + 1)
              .arg(t)
              .arg(m_result[5])
              .arg(m_result[6])
       << "\n";
  list << "}";

  return list;
}
