#include "LVGLTabWidget.h"

#include <QGridLayout>

#include "LVGLObject.h"
#include "LVGLSimulator.h"

LVGLTabWidget::LVGLTabWidget(QWidget *parent)
    : QWidget(parent), m_parent(lv_obj_create(NULL, NULL)) {}

void LVGLTabWidget::setSimulator(LVGLSimulator *sim) {
  delete this->layout();
  QGridLayout *glayout = new QGridLayout(this);
  glayout->addWidget(sim, 0, 0, 1, 1);
}

void LVGLTabWidget::removeObject(LVGLObject *object) {
  auto childs = object->childs();
  while (!childs.isEmpty()) {
    removeObject(childs.at(0));
    childs = object->childs();
  }
  if (object->parent()) object->parent()->removeChild(object);
  m_objects.removeOne(object);
  delete object;
  object = nullptr;
}

void LVGLTabWidget::removeAllObjects() {
  auto objs = m_objects;
  while (!objs.isEmpty()) {
    if (objs.at(0) && !objs.at(0)->parent()) removeObject(m_objects.at(0));
    objs = m_objects;
  }
}
