#ifndef PAGE_H
#define PAGE_H

#include <QWidget>

class QLineEdit;

class PageWidget : public QWidget {
  Q_OBJECT
 public:
  PageWidget(QWidget *parent = nullptr);

 public:
  QString getName() const;

  void setName(const QString &name);

 signals:
  void editingFinished();

 private:
  QLineEdit *m_edit;
};

class Page {
 public:
  Page(const QString &name, Page *parent = nullptr);
  ~Page();

 public:
  // get
  QString name() const;
  QString Icon() const;
  Page *parent() const;
  QList<Page *> childs() const;
  int childSize() const;
  Page *child(int index) const;
  int row() const;
  QWidget *editor(QWidget *parent);
  QWidget *tabWidget();

  // set
  void setName(const QString &name);

  // other
  void addChild(Page *page);
  void delChild(Page *page);
  void appendChild(Page *child);
  void insertChild(int idx, Page *child);
  void removeChild(Page *p);

 private:
  QString m_name;
  Page *m_parent;
  QList<Page *> m_childs;
  PageWidget *m_editWidget;
  QWidget *m_tabWidget;
};

#endif  // PAGE_H
