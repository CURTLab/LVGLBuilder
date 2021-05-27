#include "LVGLEvent.h"

LVGLEvent::LVGLEvent() {}

LVGLEvent::~LVGLEvent() {}

QString LVGLEvent::eventHeadCode() {
  m_eventHeadCode =
      QString("void %1(lv_obj_t *obj, lv_event_t event)").arg(m_result[0]);
  return m_eventHeadCode + ";\n";
}
