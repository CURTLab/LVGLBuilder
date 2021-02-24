#include "LVGLButtonMatrix.h"

#include <QDebug>
#include <QIcon>
#include <QUuid>

#include "LVGLObject.h"
#include "properties/LVGLPropertyAnyFunc.h"

class LVGLPropertyBtnmatrixButtonsText : public LVGLPropertyStringPlus {
 public:
  LVGLPropertyBtnmatrixButtonsText() : m_btnTotal(5) {}

  int getBtnTotal() { return m_btnTotal; }

  QString name() const { return "Button's Text"; }

  QStringList function(LVGLObject *obj) const {
    const char **map = lv_btnmatrix_get_map_array(obj->obj());
    QStringList strmap;
    for (int i = 0; i <= m_btnTotal; ++i) strmap << QString(map[i]);
    QString arrname = "arr" + QString(QUuid::createUuid().toString()).mid(1, 7);
    QString str = "static const char* " + arrname + "[] = {";
    for (auto s : strmap) {
      if (s != "\n")
        str += "\"" + s + "\",";
      else
        str += "\"\\n\",";
    }
    str += "\"\"};";
    QString str2 = QString("lv_btnmatrix_set_map(%1, ").arg(obj->codeName());
    str2 += arrname + QString(");");
    return QStringList() << str << str2;
  }

 protected:
  QString get(LVGLObject *obj) const {
    const char **map = lv_btnmatrix_get_map_array(obj->obj());
    QStringList strmap;
    for (int i = 0; i <= m_btnTotal; ++i) strmap << QString(map[i]);
    QString tmp;
    for (int i = 0; i < strmap.size(); ++i)
      if (i < strmap.size() - 1)
        tmp += strmap[i] + ",";
      else
        tmp += strmap[i];
    return tmp;
  }
  void set(LVGLObject *obj, QString string) {
    QStringList strlist = string.split(',');
    m_btnTotal = strlist.size() - 1;
    const char **p = new const char *[strlist.size() + 1];
    for (int i = 0; i < strlist.size(); ++i) {
      auto text = strlist[i].toUtf8();
      char *data = new char[text.size() + 1];
      strcpy(data, text.data());
      p[i] = data;
    }
    p[strlist.size()] = "";
    lv_btnmatrix_set_map(obj->obj(), p);
  }

 private:
  int m_btnTotal;
};

class LVGLPropertyBtnmatrixTextAlign : public LVGLPropertyEnum {
 public:
  LVGLPropertyBtnmatrixTextAlign()
      : LVGLPropertyEnum({"Left", "Right", "Center", "Auto"}),
        m_values({"LV_LABEL_ALIGN_LEFT", "LV_LABEL_ALIGN_RIGHT",
                  "LV_LABEL_ALIGN_CENTER", "LV_LABEL_ALIGN_AUTO"}) {}

  QString name() const { return "Text Align"; }

  QStringList function(LVGLObject *obj) const {
    if (get(obj) != 0)
      return QStringList() << QString("lv_btnmatrix_set_align(%1, %2);")
                                  .arg(obj->codeName())
                                  .arg(m_values.at(get(obj)));
    return QStringList();
  }

 protected:
  int get(LVGLObject *obj) const { return lv_btnmatrix_get_align(obj->obj()); }
  void set(LVGLObject *obj, int index) {
    lv_btnmatrix_set_align(obj->obj(), index & 0xff);
  }
  QStringList m_values;
};

class LVGLPropertyBtnmatrixFocus : public LVGLPropertyInt {
 public:
  LVGLPropertyBtnmatrixFocus(LVGLPropertyBtnmatrixButtonsText *p)
      : LVGLPropertyInt(-1, UINT16_MAX - 1, ""), m_btnid(-1), m_lpbbt(p) {}

  QString name() const { return "Focus"; }

  QStringList function(LVGLObject *obj) const {
    if (m_btnid != -1 && m_btnid < 0xffff)
      return QStringList() << QString("lv_btnmatrix_set_focused_btn(%1,%2);")
                                  .arg(obj->codeName())
                                  .arg(m_btnid);
    else
      return QStringList();
  }

 protected:
  int get(LVGLObject *obj) const {
    auto max = lv_btnmatrix_get_focused_btn(obj->obj());
    if (max == LV_BTNMATRIX_BTN_NONE) return -1;
    return max;
  }
  void set(LVGLObject *obj, int value) {
    m_btnid = value;
    if (m_btnid == -1)
      lv_btnmatrix_set_focused_btn(obj->obj(), LV_BTNMATRIX_BTN_NONE);
    if (value >= m_lpbbt->getBtnTotal()) {
      auto var = m_lpbbt->getBtnTotal() - 1;
      lv_btnmatrix_set_focused_btn(obj->obj(), static_cast<uint16_t>(var));
    } else
      lv_btnmatrix_set_focused_btn(obj->obj(), static_cast<uint16_t>(value));
  }

 private:
  int m_btnid;
  LVGLPropertyBtnmatrixButtonsText *m_lpbbt;
};

class LVGLPropertyBtnmatrixButtonCtrl : public LVGLPropertyAnyFunc {
 public:
  LVGLPropertyBtnmatrixButtonCtrl(const AnyFuncColType arr[], int size,
                                  LVGLPropertyBtnmatrixButtonsText *p)
      : LVGLPropertyAnyFunc(arr, size),
        m_lpbbt(p),
        m_list(QStringList() << "Empty list"),
        m_frun(true) {}
  QString name() const { return "Button's Ctrl"; }

  QStringList function(LVGLObject *obj) const {
    QStringList list;
    if (m_coderesulet.isEmpty()) return list;
    for (int i = 0; i < m_lpbbt->getBtnTotal(); ++i) {
      if (m_coderesulet.contains(i) && m_coderesulet[i] != "None") {
        list << QString("lv_btnmatrix_set_btn_ctrl(%1, %2, %3);")
                    .arg(obj->codeName())
                    .arg(i)
                    .arg(m_coderesulet[i]);
      }
    }
    return list;
  }

 protected:
  QStringList get(LVGLObject *obj) const {
    if (m_frun) {
      m_frun = false;
      QStringList l1, l2;
      int total = m_lpbbt->getBtnTotal();
      for (int i = 0; i < total; ++i) l1 << QString::number(i);
      l2 << "LV_BTNMATRIX_CTRL_HIDDEN"
         << "LV_BTNMATRIX_CTRL_NO_REPEAT"
         << "LV_BTNMATRIX_CTRL_DISABLED"
         << "LV_BTNMATRIX_CTRL_CHECKABLE"
         << "LV_BTNMATRIX_CTRL_CHECK_STATE"
         << "LV_BTNMATRIX_CTRL_CLICK_TRIG"
         << "None";
      updateData(0, l1);
      updateData(1, l2);
    }

    if (m_list[0] != "Empty list") return m_list;
    return QStringList();
  }
  void set(LVGLObject *obj, QStringList list) {
    m_list = list;
    for (int i = 0; i < m_list.size(); ++i) {
      QStringList strlist = list[i].split(' ');
      int id = strlist[0].toInt();
      auto str = strlist[1];
      lv_btnmatrix_ctrl_t ctrl = LV_BTNMATRIX_CTRL_HIDDEN;
      m_coderesulet[id] = str;
      if ("LV_BTNMATRIX_CTRL_HIDDEN" == str)
        ctrl = LV_BTNMATRIX_CTRL_HIDDEN;
      else if ("LV_BTNMATRIX_CTRL_NO_REPEAT" == str)
        ctrl = LV_BTNMATRIX_CTRL_NO_REPEAT;
      else if ("LV_BTNMATRIX_CTRL_DISABLED" == str)
        ctrl = LV_BTNMATRIX_CTRL_DISABLED;
      else if ("LV_BTNMATRIX_CTRL_CHECKABLE" == str)
        ctrl = LV_BTNMATRIX_CTRL_CHECKABLE;
      else if ("LV_BTNMATRIX_CTRL_CHECK_STATE" == str)
        ctrl = LV_BTNMATRIX_CTRL_CHECK_STATE;
      else if ("LV_BTNMATRIX_CTRL_CLICK_TRIG" == str)
        ctrl = LV_BTNMATRIX_CTRL_CLICK_TRIG;
      else if ("None" == str) {
        if (m_saveresult.contains(id))
          lv_btnmatrix_clear_btn_ctrl(obj->obj(), id, m_saveresult[id]);
        continue;
      }
      m_saveresult[id] = ctrl;
      lv_btnmatrix_set_btn_ctrl(obj->obj(), id, ctrl);
    }
  }
  LVGLPropertyBtnmatrixButtonsText *m_lpbbt;

  QStringList m_list;
  mutable bool m_frun;
  QMap<int, lv_btnmatrix_ctrl_t> m_saveresult;
  QMap<int, QString> m_coderesulet;
};

class LVGLPropertyBtnmatrixButtonWidth : public LVGLPropertyAnyFunc {
 public:
  LVGLPropertyBtnmatrixButtonWidth(const AnyFuncColType arr[], int size,
                                   LVGLPropertyBtnmatrixButtonsText *p)
      : LVGLPropertyAnyFunc(arr, size),
        m_lpbbt(p),
        m_list(QStringList() << "Empty list"),
        m_frun(true) {}

  QString name() const { return "Button's Width"; }

  QStringList function(LVGLObject *obj) const {
    QStringList list;
    if (m_coderesulet.isEmpty()) return list;
    for (int i = 0; i < m_lpbbt->getBtnTotal(); ++i) {
      if (m_coderesulet.contains(i) && m_coderesulet[i] != "1") {
        list << QString("lv_btnmatrix_set_btn_width(%1, %2, %3);")
                    .arg(obj->codeName())
                    .arg(i)
                    .arg(m_coderesulet[i]);
      }
    }
    return list;
  }

 protected:
  QStringList get(LVGLObject *obj) const {
    if (m_frun) {
      m_frun = false;
      QStringList l1;
      int total = m_lpbbt->getBtnTotal();
      for (int i = 0; i < total; ++i) l1 << QString::number(i);
      updateData(0, l1);
      updateData(1, 1, false);
      updateData(1, 7, true);
    }

    if (m_list[0] != "Empty list") return m_list;
    return QStringList();
  }
  void set(LVGLObject *obj, QStringList list) {
    m_list = list;
    for (int i = 0; i < m_list.size(); ++i) {
      QStringList strlist = m_list[i].split(' ');
      int id = strlist[0].toInt();
      int width = strlist[1].toInt();
      m_coderesulet[id] = strlist[1];
      lv_btnmatrix_set_btn_width(obj->obj(), id, width);
    }
  }

 private:
  LVGLPropertyBtnmatrixButtonsText *m_lpbbt;
  QStringList m_list;
  mutable bool m_frun;
  QMap<int, QString> m_coderesulet;
};

LVGLButtonMatrix::LVGLButtonMatrix() {
  initStateStyles();
  auto p = new LVGLPropertyBtnmatrixButtonsText;
  m_properties << p;
  m_properties << new LVGLPropertyBtnmatrixFocus(p);
  m_properties << new LVGLPropertyBtnmatrixTextAlign;
  static const AnyFuncColType arr[2]{e_QComboBox, e_QComboBox};
  m_properties << new LVGLPropertyBtnmatrixButtonCtrl(arr, 2, p);
  static const AnyFuncColType arr2[2]{e_QComboBox, e_QSpinBox};
  m_properties << new LVGLPropertyBtnmatrixButtonWidth(arr2, 2, p);

  m_parts << LV_BTNMATRIX_PART_BG << LV_BTNMATRIX_PART_BTN;
  m_editableStyles << LVGL::Background;    // LV_BTNMATRIX_PART_BG
  m_editableStyles << LVGL::BtnMatrixBTN;  // LV_BTNMATRIX_PART_BTN
}

QString LVGLButtonMatrix::name() const { return "Button Matrix"; }

QString LVGLButtonMatrix::className() const { return "lv_btnmatrix"; }

LVGLWidget::Type LVGLButtonMatrix::type() const { return ButtonMatrix; }

QIcon LVGLButtonMatrix::icon() const { return QIcon(); }

lv_obj_t *LVGLButtonMatrix::newObject(lv_obj_t *parent) const {
  lv_obj_t *obj = lv_btnmatrix_create(parent, nullptr);
  return obj;
}

QSize LVGLButtonMatrix::minimumSize() const { return QSize(150, 100); }

QStringList LVGLButtonMatrix::styles() const {
  return QStringList() << "BTNMATRIX_PART_BG"
                       << "BTNMATRIX_PART_BTN";
}

lv_style_t *LVGLButtonMatrix::style(lv_obj_t *obj, lv_obj_part_t part) const {
  return lv_obj_get_local_style(obj, part);
}

void LVGLButtonMatrix::setStyle(lv_obj_t *obj, int type,
                                lv_style_t *style) const {
  lv_obj_add_style(obj, LV_BTN_PART_MAIN, style);
}

void LVGLButtonMatrix::addStyle(lv_obj_t *obj, lv_style_t *style,
                                lv_obj_part_t part) const {
  lv_obj_add_style(obj, part, style);
}

void LVGLButtonMatrix::initStateStyles() {
  for (int i = 0; i < 2; ++i) {
    lv_style_t *de = new lv_style_t;
    lv_style_t *ch = new lv_style_t;
    lv_style_t *fo = new lv_style_t;
    lv_style_t *ed = new lv_style_t;
    lv_style_t *ho = new lv_style_t;
    lv_style_t *pr = new lv_style_t;
    lv_style_t *di = new lv_style_t;
    lv_style_init(de);
    lv_style_init(ch);
    lv_style_init(fo);
    lv_style_init(ed);
    lv_style_init(ho);
    lv_style_init(pr);
    lv_style_init(di);
    QList<lv_style_t *> stateStyles;
    stateStyles << de << ch << fo << ed << ho << pr << di;
    m_partsStyles[i] = stateStyles;
  }
}
