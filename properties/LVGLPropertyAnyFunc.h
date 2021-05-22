#ifndef LVGLPROPERTYANYFUNC_H
#define LVGLPROPERTYANYFUNC_H

#include "core/LVGLProperty.h"

enum AnyFuncColType {
  e_QComboBox = 0,
  e_QSpinBox,
  e_QLineEdit,
  e_QTextEdit,
  e_ColorPick,
  e_Seqlabel,
};

class LVGLPropertyAnyFunc : public LVGLProperty {
 public:
  LVGLPropertyAnyFunc(const AnyFuncColType coltype[], int arrsize,
                      bool oneline = false, bool canInsert = true,
                      LVGLProperty *parent = nullptr);
  bool hasEditor() const;
  QWidget *editor(QWidget *parent);
  void updateEditor(LVGLObject *obj);
  void updateWidget(LVGLObject *obj);
  void updateData(int col, QStringList list) const;
  void updateData(int col, int value, bool ismax) const;

  QVariant value(LVGLObject *obj) const;
  void setValue(LVGLObject *obj, QVariant value);

  QJsonValue toJson(LVGLObject *obj) const;

 protected:
  class LVGLPropertyAnyFuncDialog *m_widget;
  bool m_oneline;
  bool m_canInsert;
  const AnyFuncColType *m_coltype;
  int m_arrssize;

  virtual QStringList get(LVGLObject *obj) const = 0;
  virtual void set(LVGLObject *obj, QStringList list) = 0;

 private:
  mutable QList<int> m_collist;
  mutable QList<QStringList> m_strlist;
  mutable QList<int> m_maxlist;
  mutable QList<int> m_maxvaluelist;
  mutable QList<int> m_minlist;
  mutable QList<int> m_minvaluelist;
};

#endif  // LVGLPROPERTYANYFUNC_H
