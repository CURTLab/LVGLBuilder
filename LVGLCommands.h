#ifndef LVGLCOMMANDS_H
#define LVGLCOMMANDS_H

#include <QJsonArray>
#include <QJsonObject>
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
  QString m_objWidgetparent;
  lv_obj_t *m_objParent;
  QRect m_widgetRect;
  QString m_lastobj;
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
  QString m_objWidgetparent;
  lv_obj_t *m_objParent;
  QRect m_widgetRect;
  QString m_lastobj;
};

class SetWidgetRectCommand : public QUndoCommand {
 public:
  explicit SetWidgetRectCommand(LVGLSimulator *sim, LVGLObject *obj, QRect rect,
                                QUndoCommand *parent = nullptr);
  explicit SetWidgetRectCommand(LVGLSimulator *sim, LVGLObject *obj,
                                QRect oldRect, QRect newRect,
                                QUndoCommand *parent = nullptr);
  void undo() override;
  void redo() override;

 private:
  LVGLSimulator *m_sim;
  LVGLObject *m_obj;
  QString m_widgetName;
  QRect m_oldRect;
  QRect m_newRect;
  QString m_lastobj;
};

class SetWidgetPropCommand : public QUndoCommand {
 public:
  explicit SetWidgetPropCommand(LVGLSimulator *sim, LVGLObject *obj,
                                QJsonObject oldWidgetArr,
                                QJsonObject newWidgetArr, QString propName,
                                QUndoCommand *parent = nullptr);
  void undo() override;
  void redo() override;

 private:
  LVGLSimulator *m_sim;
  LVGLObject *m_obj;
  QString m_widgetName;
  QJsonObject m_oldWidgetArr;
  QJsonObject m_newWidgetArr;
  QString m_lastobj;
};

class SetWidgetStyleCommand : public QUndoCommand {
 public:
  explicit SetWidgetStyleCommand(LVGLSimulator *sim, LVGLObject *obj,
                                 QJsonObject oldWidgetStyle,
                                 QJsonObject newWidgetStyle, QString styleName,
                                 QUndoCommand *parent = nullptr);
  void undo() override;
  void redo() override;

 private:
  LVGLSimulator *m_sim;
  LVGLObject *m_obj;
  QString m_widgetName;
  QJsonObject m_oldWidgetStyle;
  QJsonObject m_newWidgetStyle;
  QString m_lastobj;
};

#endif  // LVGLCOMMANDS_H
