#include "LVGLPropertyAnyFunc.h"

#include <QComboBox>
#include <QDebug>
#include <QDialog>
#include <QDialogButtonBox>
#include <QHeaderView>
#include <QJsonArray>
#include <QSpinBox>
#include <QTableWidget>
#include <QToolButton>
#include <QVBoxLayout>

class LVGLPropertyAnyFuncDialog : public QDialog {
 public:
  LVGLPropertyAnyFuncDialog(const AnyFuncColType coltype[], int arrsize,
                            bool oneline = false, bool canInsert = true,
                            QWidget *parent = nullptr)
      : QDialog(parent),
        m_rowCount(0),
        m_afctarr(coltype),
        m_arrsize(arrsize),
        m_oneline(oneline) {
    m_tableW = new QTableWidget(this);
    m_tableW->setColumnCount(arrsize);
    m_tableW->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QStringList headerlabels;
    for (int i = 0; i < arrsize; ++i) headerlabels << "-";
    m_tableW->setHorizontalHeaderLabels(headerlabels);
    m_tableW->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    QDialogButtonBox *box =
        new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(box, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(box, &QDialogButtonBox::rejected, this, &QDialog::reject);

    if (canInsert) {
      QToolButton *add = new QToolButton();
      add->setIcon(QIcon(":/icons/add.png"));
      add->setIconSize(QSize(24, 24));
      QToolButton *rem = new QToolButton();
      rem->setIcon(QIcon(":/icons/delete.png"));
      rem->setIconSize(QSize(24, 24));

      box->addButton(add, QDialogButtonBox::ApplyRole);
      box->addButton(rem, QDialogButtonBox::ApplyRole);

      connect(box, &QDialogButtonBox::clicked,
              [this, add, rem](QAbstractButton *b) {
                if (b == add) {
                  if (m_oneline) {
                    if (m_rowCount < 1) {
                      ++m_rowCount;
                      m_tableW->setRowCount(m_rowCount);
                      QMap<int, void *> map;
                      for (int i = 0; i < m_arrsize; ++i) {
                        if (m_afctarr[i] == AnyFuncColType::e_QComboBox) {
                          QComboBox *comp = new QComboBox(this);
                          comp->addItems(m_liststrlist[i]);
                          m_tableW->setCellWidget(m_rowCount - 1, i, comp);
                          map[i] = comp;

                        } else if (m_afctarr[i] == AnyFuncColType::e_QSpinBox) {
                          QSpinBox *spinp = new QSpinBox(this);
                          spinp->setMaximum(m_maxlist[i]);
                          spinp->setMinimum(m_minlist[i]);
                          m_tableW->setCellWidget(m_rowCount - 1, i, spinp);
                          map[i] = spinp;
                        }
                      }
                      m_listmap.push_back(map);
                    }
                  } else {
                    ++m_rowCount;
                    m_tableW->setRowCount(m_rowCount);
                    QMap<int, void *> map;
                    for (int i = 0; i < m_arrsize; ++i) {
                      if (m_afctarr[i] == AnyFuncColType::e_QComboBox) {
                        QComboBox *comp = new QComboBox(this);
                        comp->addItems(m_liststrlist[i]);
                        m_tableW->setCellWidget(m_rowCount - 1, i, comp);
                        map[i] = comp;

                      } else if (m_afctarr[i] == AnyFuncColType::e_QSpinBox) {
                        QSpinBox *spinp = new QSpinBox(this);
                        spinp->setMaximum(m_maxlist[i]);
                        spinp->setMinimum(m_minlist[i]);
                        m_tableW->setCellWidget(m_rowCount - 1, i, spinp);
                        map[i] = spinp;
                      }
                    }
                    m_listmap.push_back(map);
                  }

                } else if (b == rem) {
                  if (!m_oneline) {
                    int row = m_tableW->currentRow();
                    m_tableW->removeRow(row);
                    m_listmap.removeAt(row);
                    --m_rowCount;
                  }
                }
              });
    }

    QVBoxLayout *layout1 = new QVBoxLayout;
    layout1->addWidget(m_tableW);
    layout1->addWidget(box);

    setLayout(layout1);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  }

  bool isequal(const QStringList &l1, const QStringList &l2) {
    if (l1.size() != l2.size()) return false;
    for (int i = 0; i < l1.size(); ++i)
      if (l1[i] != l2[i]) return false;
    return true;
  }

  void setTextList(QStringList list) {
    if (isequal(textList(), list)) return;
    m_rowCount = list.size();
    m_tableW->setRowCount(m_rowCount);
    m_tableW->clear();
    m_listmap.clear();  // will delete ,because they have parent
    int index = 0;
    for (const QString &s : list) {
      QMap<int, void *> map;
      QStringList spacestr = s.split(' ');
      for (int j = 0; j < spacestr.size() - 1; ++j) {
        if (m_afctarr[j] == AnyFuncColType::e_QComboBox) {
          auto comp = new QComboBox(this);
          comp->addItems(m_liststrlist[j]);
          auto curindx = comp->findText(spacestr[j]);
          comp->setCurrentIndex(curindx);
          m_tableW->setCellWidget(index, j, comp);
          map[j] = comp;
        } else if (m_afctarr[j] == AnyFuncColType::e_QSpinBox) {
          QSpinBox *spinp = new QSpinBox(this);
          spinp->setMaximum(m_maxlist[j]);
          spinp->setMinimum(m_minlist[j]);
          spinp->setValue(spacestr[j].toUInt());
          m_tableW->setCellWidget(index, j, spinp);
          map[j] = spinp;
        }
      }
      ++index;
      m_listmap.push_back(map);
    }
  }

  inline void updateData(int col, QStringList list) {
    m_liststrlist[col] = list;
  }

  inline void updateData(int col, int value, bool ismax) {
    if (ismax)
      m_maxlist[col] = value;
    else
      m_minlist[col] = value;
  }

  inline QStringList textList() const {
    QStringList ret;
    for (int i = 0; i < m_rowCount; ++i) {
      auto map = m_listmap[i];
      QString tmp;
      for (int j = 0; j < m_arrsize; ++j) {
        if (m_afctarr[j] == AnyFuncColType::e_QComboBox) {
          auto p = static_cast<QComboBox *>(map[j]);
          tmp += p->currentText() + " ";
        } else if (m_afctarr[j] == AnyFuncColType::e_QSpinBox) {
          auto p = static_cast<QSpinBox *>(map[j]);
          tmp += QString::number(p->value()) + " ";
        }
      }
      ret << tmp;
    }

    return ret;
  }

 private:
  QTableWidget *m_tableW;
  int m_rowCount;
  const AnyFuncColType *m_afctarr;
  int m_arrsize;
  bool m_oneline;
  QList<QMap<int, void *>> m_listmap;
  QMap<int, QStringList> m_liststrlist;
  QMap<int, int> m_maxlist;
  QMap<int, int> m_minlist;
};

LVGLPropertyAnyFunc::LVGLPropertyAnyFunc(const AnyFuncColType coltype[],
                                         int arrsize, bool oneline,
                                         bool canInsert, LVGLProperty *parent)
    : LVGLProperty(parent),
      m_widget(nullptr),
      m_oneline(oneline),
      m_canInsert(canInsert),
      m_coltype(coltype),
      m_arrssize(arrsize) {}

bool LVGLPropertyAnyFunc::hasEditor() const { return true; }

QWidget *LVGLPropertyAnyFunc::editor(QWidget *parent) {
  m_widget = new LVGLPropertyAnyFuncDialog(m_coltype, m_arrssize, m_oneline,
                                           m_canInsert, parent);
  for (int i = 0; i < m_collist.size(); ++i)
    m_widget->updateData(m_collist[i], m_strlist[i]);

  for (int i = 0; i < m_maxlist.size(); ++i)
    m_widget->updateData(m_maxlist[i], m_maxvaluelist[i], true);

  for (int i = 0; i < m_minlist.size(); ++i)
    m_widget->updateData(m_minlist[i], m_minvaluelist[i], false);

  return m_widget;
}

void LVGLPropertyAnyFunc::updateEditor(LVGLObject *obj) {
  auto list = get(obj);
  if (!list.isEmpty()) m_widget->setTextList(list);
}

void LVGLPropertyAnyFunc::updateWidget(LVGLObject *obj) {
  if (m_widget->result() == QDialog::Accepted) set(obj, m_widget->textList());
}

void LVGLPropertyAnyFunc::updateData(int col, QStringList list) const {
  m_collist.push_back(col);
  m_strlist.push_back(list);
}

void LVGLPropertyAnyFunc::updateData(int col, int value, bool ismax) const {
  if (ismax) {
    m_maxlist.push_back(col);
    m_maxvaluelist.push_back(value);
  } else {
    m_minlist.push_back(col);
    m_minvaluelist.push_back(value);
  }
}

QVariant LVGLPropertyAnyFunc::value(LVGLObject *obj) const {
  const QStringList items = get(obj);
  return items.isEmpty() ? "Empty list" : items.join(",");
}

void LVGLPropertyAnyFunc::setValue(LVGLObject *obj, QVariant value) {
  if (value.type() == QVariant::List) {
    QVariantList list = value.toList();
    QStringList items;
    for (const QVariant &p : list) {
      if (p.type() == QVariant::String) items << p.toString();
    }
    set(obj, items);
  }
}

QJsonValue LVGLPropertyAnyFunc::toJson(LVGLObject *obj) const {
  //  const QStringList items = get(obj);
  QJsonArray val;
  //  for (const QString &i : items) val.append(i);
  return val;
}
