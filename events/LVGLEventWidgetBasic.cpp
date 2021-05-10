#include "LVGLEventWidgetBasic.h"

#include <LVGLObject.h>

#include "LVGLCore.h"
#include "LVGLSimulator.h"

LVGLEventWidgetBasic::LVGLEventWidgetBasic() {}

void LVGLEventWidgetBasic::eventRun(lv_obj_t *obj) {}

QStringList LVGLEventWidgetBasic::objCode(const QString& objName) {
  return QStringList();
}

QString LVGLEventWidgetBasic::eventHeadCode() { return QString(); }

QStringList LVGLEventWidgetBasic::eventCode() { return QStringList(); }
