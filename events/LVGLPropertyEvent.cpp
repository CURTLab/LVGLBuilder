#include "LVGLPropertyEvent.h"

#include <QComboBox>
#include <QDebug>
#include <QDialog>
#include <QDialogButtonBox>
#include <QHeaderView>
#include <QJsonArray>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QTableWidget>
#include <QTextEdit>
#include <QToolButton>
#include <QVBoxLayout>

#include "EventsListWIdget.h"

LVGLPropertyEvent::LVGLPropertyEvent(LVGLProperty *parent)
    : LVGLProperty(parent) {}

bool LVGLPropertyEvent::hasEditor() const { return true; }

QWidget *LVGLPropertyEvent::editor(QWidget *parent) {
  m_widget = new EventsListWIdget(parent);

  return m_widget;
}

void LVGLPropertyEvent::updateEditor(LVGLObject *obj) {
  auto list = get(obj);
  if (!list.isEmpty()) m_widget->setTextList(list);
}

void LVGLPropertyEvent::updateWidget(LVGLObject *obj) {
  //  if (m_widget->result() == QDialog::Accepted) set(obj,
  //  m_widget->textList());
}

QVariant LVGLPropertyEvent::value(LVGLObject *obj) const {
  const QStringList items = get(obj);
  return items.isEmpty() ? "Empty list" : items.join(",");
}

void LVGLPropertyEvent::setValue(LVGLObject *obj, QVariant value) {
  if (value.type() == QVariant::List) {
    QVariantList list = value.toList();
    QStringList items;
    for (const QVariant &p : list) {
      if (p.type() == QVariant::String) items << p.toString();
    }
    set(obj, items);
  }
}

QJsonValue LVGLPropertyEvent::toJson(LVGLObject *obj) const {
  const QStringList items = get(obj);
  QJsonArray val;
  for (const QString &i : items) val.append(i);
  return val;
}
