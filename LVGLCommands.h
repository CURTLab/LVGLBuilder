#ifndef LVGLCOMMANDS_H
#define LVGLCOMMANDS_H

#include <QUndoCommand>

#include "LVGLCore.h"

class LVGLSimulator;
class LVGLObject;

class AddWidgetCommand : public QUndoCommand {
 public:
  explicit AddWidgetCommand(LVGLSimulator *sim, LVGLObject *obj,
                            QUndoCommand *parent = nullptr);
  void undo() override;
  void redo() override;

 private:
  LVGLSimulator *m_sim;
  LVGLObject *m_obj;
  QString m_widgetName;
  QString m_widgetClassName;
  LVGLObject *m_objWidgetparent;
  lv_obj_t *m_objParent;
  QRect m_widgetRect;
};

class RemoveWidgetCommand : public QUndoCommand {
 public:
  explicit RemoveWidgetCommand(LVGLSimulator *sim, LVGLObject *obj,
                               QUndoCommand *parent = nullptr);
  void undo() override;
  void redo() override;

 private:
  LVGLSimulator *m_sim;
  LVGLObject *m_obj;
  QString m_widgetName;
  QString m_widgetClassName;
  LVGLObject *m_objWidgetparent;
  lv_obj_t *m_objParent;
  QRect m_widgetRect;
};

#endif  // LVGLCOMMANDS_H
