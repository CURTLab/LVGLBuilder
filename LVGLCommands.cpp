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

  if (m_obj->parent()) {
    m_objWidgetparent = m_obj->parent()->name();
    m_widgetRect =
        QRect(m_obj->geometry().x() - m_obj->parent()->geometry().x(),
              m_obj->geometry().y() - m_obj->parent()->geometry().y(),
              m_obj->geometry().width(), m_obj->geometry().height());
  } else
    m_widgetRect = m_obj->geometry();
  m_objParent = lvgl->getdispt()->act_scr;
}

void AddWidgetCommand::undo() {
  m_sim->removeObject(m_sim->findObject(m_widgetName));
  m_obj = nullptr;
}

void AddWidgetCommand::redo() {
  if (!m_obj) {
    auto wc = lvgl->widget(m_widgetClassName);
    if (!m_objWidgetparent.isEmpty())
      m_obj = new LVGLObject(wc, m_widgetName,
                             m_sim->findObject(m_objWidgetparent));
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
  if (m_obj->parent()) m_objWidgetparent = m_obj->parent()->name();
  m_objParent = lvgl->getdispt()->act_scr;
  m_widgetRect = m_obj->geometry();
  setText(QObject::tr("Remove %1").arg(m_obj->name()));
}

void RemoveWidgetCommand::undo() {
  if (!m_obj) {
    auto wc = lvgl->widget(m_widgetClassName);
    if (!m_objWidgetparent.isEmpty())
      m_obj = new LVGLObject(wc, m_widgetName,
                             m_sim->findObject(m_objWidgetparent));
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

SetWidgetRectCommand::SetWidgetRectCommand(LVGLSimulator *sim, LVGLObject *obj,
                                           QRect rect, QUndoCommand *parent)
    : QUndoCommand(parent),
      m_sim(sim),
      m_obj(obj),
      m_widgetName(obj->name()),
      m_oldRect(obj->geometry()),
      m_newRect(rect) {
  setText(QObject::tr("Change %1's geometry").arg(m_widgetName));
}

SetWidgetRectCommand::SetWidgetRectCommand(LVGLSimulator *sim, LVGLObject *obj,
                                           QRect oldRect, QRect newRect,
                                           QUndoCommand *parent)
    : QUndoCommand(parent),
      m_sim(sim),
      m_obj(obj),
      m_widgetName(obj->name()),
      m_oldRect(oldRect),
      m_newRect(newRect) {
  setText(QObject::tr("Change %1's geometry").arg(m_widgetName));
}

void SetWidgetRectCommand::undo() {
  m_sim->findObject(m_widgetName)->setGeometry(m_oldRect);
  if (m_sim->selectedObject() != nullptr &&
      m_sim->selectedObject()->name() == m_widgetName)
    emit m_sim->objectSelected(m_sim->findObject(m_widgetName));
  else
    m_sim->setSelectedObject(m_sim->findObject(m_widgetName));
}

void SetWidgetRectCommand::redo() {
  m_sim->findObject(m_widgetName)->setGeometry(m_newRect);
  if (m_sim->selectedObject() != nullptr &&
      m_sim->selectedObject()->name() == m_widgetName)
    emit m_sim->objectSelected(m_sim->findObject(m_widgetName));
  else
    m_sim->setSelectedObject(m_sim->findObject(m_widgetName));
}

SetWidgetPropCommand::SetWidgetPropCommand(LVGLSimulator *sim, LVGLObject *obj,
                                           QJsonObject oldWidgetArr,
                                           QJsonObject newWidgetArr,
                                           QString propName,
                                           QUndoCommand *parent)
    : QUndoCommand(parent),
      m_sim(sim),
      m_obj(obj),
      m_widgetName(obj->name()),
      m_oldWidgetArr(oldWidgetArr),
      m_newWidgetArr(newWidgetArr) {
  setText(QObject::tr("Change %1's %2").arg(m_widgetName).arg(propName));
}

void SetWidgetPropCommand::undo() {
  auto obj = m_sim->findObject(m_widgetName);
  if (obj) {
    LVGLObject::parseProp(m_oldWidgetArr, obj);
    if (m_sim->selectedObject()->name() == m_widgetName)
      emit m_sim->objectSelected(obj);
    else
      m_sim->setSelectedObject(obj);
  }
}

void SetWidgetPropCommand::redo() {
  auto obj = m_sim->findObject(m_widgetName);
  if (obj) {
    LVGLObject::parseProp(m_newWidgetArr, obj);
    if (m_sim->selectedObject()->name() == m_widgetName)
      emit m_sim->objectSelected(obj);
    else
      m_sim->setSelectedObject(obj);
  }
}

SetWidgetStyleCommand::SetWidgetStyleCommand(
    LVGLSimulator *sim, LVGLObject *obj, QJsonObject oldWidgetStyle,
    QJsonObject newWidgetStyle, QString styleName, QUndoCommand *parent)
    : QUndoCommand(parent),
      m_sim(sim),
      m_obj(obj),
      m_widgetName(obj->name()),
      m_oldWidgetStyle(oldWidgetStyle),
      m_newWidgetStyle(newWidgetStyle) {
  setText(QObject::tr("Change %1's %2").arg(m_widgetName).arg(styleName));
}

void SetWidgetStyleCommand::undo() {
  auto obj = m_sim->findObject(m_widgetName);
  if (obj) {
    LVGLObject::parseStyle(m_oldWidgetStyle, obj);
    if (m_sim->selectedObject()->name() == m_widgetName)
      emit m_sim->objectSelected(obj);
    else
      m_sim->setSelectedObject(obj);
  }
}

void SetWidgetStyleCommand::redo() {
  auto obj = m_sim->findObject(m_widgetName);
  if (obj) {
    LVGLObject::parseStyle(m_newWidgetStyle, obj);
    if (m_sim->selectedObject()->name() == m_widgetName)
      emit m_sim->objectSelected(obj);
    else
      m_sim->setSelectedObject(obj);
  }
}
