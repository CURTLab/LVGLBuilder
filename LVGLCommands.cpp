#include "LVGLCommands.h"

#include <QDebug>

#include "LVGLCore.h"
#include "LVGLObject.h"
#include "LVGLSimulator.h"
#include "widgets/LVGLWidget.h"

// Add
AddWidgetCommand::AddWidgetCommand(LVGLSimulator *sim, LVGLObject *obj,
                                   QUndoCommand *parent)
    : QUndoCommand(parent), m_sim(sim), m_obj(obj) {
  m_widgetName = m_obj->name();
  m_widgetClassName = m_obj->widgetClass()->className();
  m_objWidgetparent = m_obj->parent();
  m_objParent = lvgl->getdispt()->act_scr;
  m_widgetRect = m_obj->geometry();
}

void AddWidgetCommand::undo() {
  m_sim->removeObject(m_sim->findObject(m_widgetName));
  m_obj = nullptr;
}

void AddWidgetCommand::redo() {
  if (!m_obj) {
    auto wc = lvgl->widget(m_widgetClassName);
    if (m_objWidgetparent)
      m_obj = new LVGLObject(wc, m_widgetName, m_objWidgetparent);
    else
      m_obj = new LVGLObject(wc, m_widgetName, m_objParent);
    m_obj->setGeometry(m_widgetRect);
    m_sim->addObject(m_obj);
  }
  setText(QObject::tr("Add %1").arg(m_obj->name()));
};

// Remove
RemoveWidgetCommand::RemoveWidgetCommand(LVGLSimulator *sim, LVGLObject *obj,
                                         QUndoCommand *parent)
    : QUndoCommand(parent), m_sim(sim), m_obj(obj) {
  m_widgetName = m_obj->name();
  m_widgetClassName = m_obj->widgetClass()->className();
  m_objWidgetparent = m_obj->parent();
  m_objParent = lvgl->getdispt()->act_scr;
  m_widgetRect = m_obj->geometry();
  setText(QObject::tr("Remove %1").arg(m_obj->name()));
}

void RemoveWidgetCommand::undo() {
  if (!m_obj) {
    auto wc = lvgl->widget(m_widgetClassName);
    if (m_objWidgetparent)
      m_obj = new LVGLObject(wc, m_widgetName, m_objWidgetparent);
    else
      m_obj = new LVGLObject(wc, m_widgetName, m_objParent);
    m_obj->setGeometry(m_widgetRect);
    m_sim->addObject(m_obj);
  }
}

void RemoveWidgetCommand::redo() {
  m_sim->removeObject(m_sim->findObject(m_widgetName));
  m_obj = nullptr;
}
