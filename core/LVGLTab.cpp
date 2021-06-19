#include "LVGLTab.h"

#include <QGridLayout>
#include <QUndoStack>

#include "LVGLObject.h"
#include "LVGLSimulator.h"

LVGLTab::LVGLTab(QWidget *parent)
    : QWidget(parent),
      m_parent(lv_obj_create(NULL, NULL)),
      m_undoStack(new QUndoStack(this)) {}

void LVGLTab::setSimulator(LVGLSimulator *sim) {
  delete this->layout();
  QGridLayout *glayout = new QGridLayout(this);
  glayout->addWidget(sim, 0, 0, 1, 1);
  glayout->setContentsMargins(0, 0, 0, 0);
}

void LVGLTab::removeAll() {
  removeAllObjects();
  removeAllImages();
  // qDeleteAll(m_fonts);
}

void LVGLTab::removeObject(LVGLObject *object) {
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

void LVGLTab::removeAllObjects() {
  auto objs = m_objects;
  while (!objs.isEmpty()) {
    if (objs.at(0) && !objs.at(0)->parent()) removeObject(m_objects.at(0));
    objs = m_objects;
  }
  m_objects.clear();
}

void LVGLTab::removeAllImages() { qDeleteAll(m_images); }

void LVGLTab::clean() {
  m_objects.clear();
  m_images.clear();
}
