#include "LVGLEventWidgetBasic.h"

#include "core/LVGLCore.h"
#include "core/LVGLObject.h"
#include "core/LVGLSimulator.h"

LVGLEventWidgetBasic::LVGLEventWidgetBasic() {}

void LVGLEventWidgetBasic::eventRun(lv_obj_t* obj) {}

QStringList LVGLEventWidgetBasic::objCode(const QString& objName) {
  return QStringList();
}

QString LVGLEventWidgetBasic::eventHeadCode() { return QString(); }

QStringList LVGLEventWidgetBasic::eventCode() { return QStringList(); }
