#include "LVGLEventStateResume.h"

#include "core/LVGLCore.h"
#include "core/LVGLObject.h"

void LVGLEventStateResume::addEvent(LVGLObject *obj) {
  if (!m_objProp.contains(obj)) {
    auto prop = obj->propToJson();
    m_objProp[obj] = prop;
  }
}

void LVGLEventStateResume::stateResume() {
  auto iter = m_objProp.begin();
  auto objs = lvgl.allObjects();
  QList<LVGLObject *> lists;

  for (; iter != m_objProp.end(); ++iter) {
    if (objs.contains(iter.key())) {
      LVGLObject::parseProp(iter.value(), iter.key());
      lists.push_back(iter.key());
    }
  }
  for (auto o : lists) m_objProp.remove(o);
}

LVGLEventStateResume::LVGLEventStateResume() {}
