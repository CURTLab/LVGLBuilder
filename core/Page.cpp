#include "Page.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QLineEdit>

PageWidget::PageWidget(QWidget *parent)
    : QWidget{parent}, m_edit{new QLineEdit(this)} {
  auto hly = new QHBoxLayout(this);
  hly->setContentsMargins(0, 0, 0, 0);
  hly->setSpacing(0);
  hly->addWidget(m_edit);
  connect(m_edit, &QLineEdit::editingFinished, this,
          &PageWidget::editingFinished);
}

QString PageWidget::getName() const { return m_edit->text(); }

void PageWidget::setName(const QString &name) { m_edit->setText(name); }

Page::Page(const QString &name, Page *parent)
    : m_name{name}, m_parent{parent}, m_tabWidget(nullptr) {}

Page::~Page() { qDeleteAll(m_childs); }

QString Page::name() const { return m_name; }

QString Page::Icon() const {
  if (m_parent->parent())
    return ":/icons/subPage.png";
  else
    return ":/icons/mainPage.png";
}

Page *Page::parent() const { return m_parent; }

QList<Page *> Page::childs() const { return m_childs; }

int Page::childSize() const { return m_childs.size(); }

Page *Page::child(int index) const {
  Q_ASSERT(0 <= index && index < m_childs.size());
  return m_childs[index];
}

int Page::row() const {
  if (m_parent) return m_parent->childs().indexOf(this);
}

QWidget *Page::editor(QWidget *parent) {
  m_editWidget = new PageWidget(parent);
  return m_editWidget;
}

QWidget *Page::tabWidget() { return m_tabWidget; }

void Page::setName(const QString &name) { m_name = name; }

void Page::addChild(Page *page) {
  Q_ASSERT(page);
  m_childs.append(page);
}

void Page::delChild(Page *page) {
  Q_ASSERT(m_childs.removeOne(page));
  delete page;
  page = nullptr;
}

void Page::appendChild(Page *child) {
  Q_ASSERT(child);
  m_childs.append(child);
}

void Page::insertChild(int idx, Page *child) {
  Q_ASSERT(child);
  m_childs.insert(idx, child);
}

void Page::removeChild(Page *p) {
  int index = m_childs.indexOf(p);
  Q_ASSERT(index >= 0);

  m_childs.removeAt(index);
  delete p;
  p = nullptr;
}
