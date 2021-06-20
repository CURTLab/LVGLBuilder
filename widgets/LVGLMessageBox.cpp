#include "LVGLMessageBox.h"

#include <QIcon>

#include "core/LVGLObject.h"
#include "properties/LVGLPropertyAnyFunc.h"
#include "properties/LVGLPropertyTextList.h"

class LVGLPropertyMsgBoxButtons : public LVGLPropertyAnyFunc {
 public:
  LVGLPropertyMsgBoxButtons(const AnyFuncColType arr[], int size,
                            LVGLMessageBox *msgB)
      : LVGLPropertyAnyFunc(arr, size), m_msgb(msgB) {}
  QString name() const { return QObject::tr("Add Buttons"); }
  QString codename() const { return "Add Buttons"; }

  QStringList function(LVGLObject *obj) const {
    QStringList list;
    return list;
  }

 protected:
  QStringList get(LVGLObject *obj) const {
    if (!m_list.isEmpty() && m_list[0] != "Empty list") return m_list;
    return QStringList();
  }
  void set(LVGLObject *obj, QStringList list) {
    m_list = list;
    m_btnNames.clear();
    for (int i = 0; i < m_list.size(); ++i) {
      QStringList strlist = list[i].split('@');
      int index = strlist[0].toInt();
      m_btnNames[index] = strlist[1];
    }
    const char **arr = new const char *[m_btnNames.size() + 1];
    if (!m_btnNames.isEmpty()) {
      for (int i = 1; i <= m_btnNames.size(); ++i) {
        auto data = m_btnNames[i].toUtf8();
        char *p = new char[data.size() + 1];
        p[data.size()] = '\0';
        strcpy(p, data.data());
        arr[i - 1] = p;
      }
      arr[m_btnNames.size()] = "";
      lv_msgbox_add_btns(obj->obj(), arr);
      m_msgb->havebtn = true;
      m_msgb->initStateStyles();
    }
  }

 private:
  QStringList m_list;
  QMap<int, QString> m_btnNames;
  QMap<int, QStringList> m_coderesult;
  LVGLMessageBox *m_msgb;
};

class LVGLPropertyMsgContent : public LVGLPropertyStringPlus {
 public:
  QString name() const { return QObject::tr("Content"); }
  QString codename() const { return "Content"; }

  QStringList function(LVGLObject *obj) const {
    QString tmp = get(obj);
    QStringList list;
    if (!tmp.isEmpty()) {
      QString str;
      for (auto c : tmp) {
        if (c != '\n')
          str += c;
        else
          str += "\\n";
      }
      list << QString("lv_msgbox_set_text(%1,\"%2\");")
                  .arg(obj->codeName())
                  .arg(str);
    }
    return list;
  }

 protected:
  QString get(LVGLObject *obj) const { return lv_msgbox_get_text(obj->obj()); }
  void set(LVGLObject *obj, QString string) {
    lv_msgbox_set_text(obj->obj(), qUtf8Printable(string));
  }
};

LVGLMessageBox::LVGLMessageBox() : havebtn(false) {
  initStateStyles();
  m_parts << LV_MSGBOX_PART_BG << LV_MSGBOX_PART_BTN_BG << LV_MSGBOX_PART_BTN;

  // m_properties << new LVGLPropertyMBoxButtons;
  m_properties << new LVGLPropertyMsgContent;
  m_properties << new LVGLPropertyValUInt16(
      0, UINT16_MAX, "Animation time", QObject::tr("Animation time"),
      "lv_msgbox_set_anim_time", lv_msgbox_set_anim_time,
      lv_msgbox_get_anim_time);
  m_properties << new LVGLPropertyBool(
      "Recolor", QObject::tr("Recolor"), "lv_mbox_set_recolor",
      lv_msgbox_set_recolor, lv_msgbox_get_recolor);

  static AnyFuncColType arr[] = {e_Seqlabel, e_QLineEdit};
  m_properties << new LVGLPropertyMsgBoxButtons(arr, 2, this);

  m_editableStyles << LVGL::MsgboxMAIN   // LV_MSGBOX_PART_BG
                   << LVGL::Background   // LV_MSGBOX_PART_BTN_BG
                   << LVGL::Background;  // LV_MSGBOX_PART_BTN
}

QString LVGLMessageBox::name() const { return QObject::tr("Message box"); }

QString LVGLMessageBox::codename() const { return "Message box"; }

QString LVGLMessageBox::className() const { return "lv_msgbox"; }

LVGLWidget::Type LVGLMessageBox::type() const { return MessageBox; }

QIcon LVGLMessageBox::icon() const { return QIcon(); }

lv_obj_t *LVGLMessageBox::newObject(lv_obj_t *parent) const {
  lv_obj_t *obj = lv_msgbox_create(parent, nullptr);
  return obj;
}

QSize LVGLMessageBox::minimumSize() const { return {200, 85}; }

QStringList LVGLMessageBox::styles() const {
  if (havebtn)
    return {"MSGBOX_PART_BG", "MSGBOX_PART_BTN_BG", "MSGBOX_PART_BTN"};
  else
    return {"MSGBOX_PART_BG"};
}

lv_style_t *LVGLMessageBox::style(lv_obj_t *obj, lv_obj_part_t part) const {
  return lv_obj_get_local_style(obj, part);
}

void LVGLMessageBox::setStyle(lv_obj_t *obj, int type,
                              lv_style_t *style) const {
  lv_obj_add_style(obj, LV_BTN_PART_MAIN, style);
}

void LVGLMessageBox::addStyle(lv_obj_t *obj, lv_style_t *style,
                              lv_obj_part_t part) const {
  lv_obj_add_style(obj, part, style);
}

void LVGLMessageBox::initStateStyles() {
  if (havebtn) {
    for (int i = 0; i < 3; ++i) {
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
  } else {
    for (int i = 0; i < 1; ++i) {
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
}
