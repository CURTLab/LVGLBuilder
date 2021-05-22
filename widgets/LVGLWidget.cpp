#include "LVGLWidget.h"

#include "MainWindow.h"
#include "core/LVGLHelper.h"
#include "core/LVGLObject.h"
#include "core/LVGLTabWidget.h"
#include "events/EventSelectWIdget.h"
#include "events/LVGLEventAnim.h"
#include "events/LVGLEventScreen.h"
#include "events/LVGLEventWidgts.h"
#include "events/LVGLPropertyEvent.h"
#include "lvgl/lvgl.h"
#include "properties/LVGLPropertyGeometry.h"

static void obj_events(lv_obj_t *obj, lv_event_t event) {
  QMap<lv_obj_t *, QList<LVGLEvent *>> &objevlists =
      LVGLHelper::getInstance().getObjEvents();
  if (event == LV_EVENT_PRESSED) {
    if (objevlists.contains(obj)) {
      QList<LVGLEvent *> &listev = objevlists[obj];
      for (auto e : listev) {
        if (e->getResult().at(1) == "Pressed") e->eventRun(obj);
      }
    }
  } else if (event == LV_EVENT_CLICKED) {
    if (objevlists.contains(obj)) {
      QList<LVGLEvent *> &listev = objevlists[obj];
      for (auto e : listev) {
        if (e->getResult().at(1) == "Clicked") e->eventRun(obj);
      }
    }
  } else if (event == LV_EVENT_LONG_PRESSED) {
    if (objevlists.contains(obj)) {
      QList<LVGLEvent *> &listev = objevlists[obj];
      for (auto e : listev) {
        if (e->getResult().at(1) == "Long_Pressed") e->eventRun(obj);
      }
    }
  } else if (event == LV_EVENT_VALUE_CHANGED) {
    if (objevlists.contains(obj)) {
      QList<LVGLEvent *> &listev = objevlists[obj];
      for (auto e : listev) {
        if (e->getResult().at(1) == "Value_Changed") e->eventRun(obj);
      }
    }
  } else if (event == LV_EVENT_FOCUSED) {
    if (objevlists.contains(obj)) {
      QList<LVGLEvent *> &listev = objevlists[obj];
      for (auto e : listev) {
        if (e->getResult().at(1) == "Focused") e->eventRun(obj);
      }
    }
  } else if (event == LV_EVENT_LONG_PRESSED_REPEAT) {
    if (objevlists.contains(obj)) {
      QList<LVGLEvent *> &listev = objevlists[obj];
      for (auto e : listev) {
        if (e->getResult().at(1) == "Long_Pressed_Repeat") e->eventRun(obj);
      }
    }
  }
}

class LVGLPropertyName : public LVGLPropertyString {
 public:
  LVGLPropertyName() : LVGLPropertyString(false) {}
  inline QString name() const override { return "Name"; }

 protected:
  inline QString get(LVGLObject *obj) const override { return obj->name(); }
  inline void set(LVGLObject *obj, QString string) override {
    obj->setName(string);
  }
};

class LVGLPropertyAccessible : public LVGLPropertyBool {
 public:
  inline LVGLPropertyAccessible() : LVGLPropertyBool("Accessible") {}

 protected:
  inline bool get(LVGLObject *obj) const override {
    return obj->isAccessible();
  }
  inline void set(LVGLObject *obj, bool statue) override {
    obj->setAccessible(statue);
  }
};

class LVGLPropertyLocked : public LVGLPropertyBool {
 public:
  inline LVGLPropertyLocked() : LVGLPropertyBool("Locked") {}

 protected:
  inline bool get(LVGLObject *obj) const override { return obj->isLocked(); }
  inline void set(LVGLObject *obj, bool statue) override {
    obj->setLocked(statue);
  }
};

static LVGLEvent *getWidgetEvent(QString name) {
  LVGLEvent *ev = nullptr;
  if (name == "Basic") {
    ev = new LVGLEventWidgetBasic;
  } else if (name == "Arc") {
    ev = new LVGLEventArc;
  } else if (name == "Bar") {
    ev = new LVGLEventBar;
  } else if (name == "Button") {
    ev = new LVGLEventButton;
  } else if (name == "CheckBox") {
    ev = new LVGLEventCheckBox;
  } else if (name == "DropDownList") {
    ev = new LVGLEventDDList;
  } else if (name == "Image") {
    ev = new LVGLEventImage;
  } else if (name == "Image Button") {
    ev = new LVGLEventImageButton;
  } else if (name == "Label") {
    ev = new LVGLEventLabel;
  } else if (name == "Led") {
    ev = new LVGLEventLed;
  } else if (name == "Line") {
    ev = new LVGLEventLine;
  } else if (name == "Roller") {
    ev = new LVGLEventRoller;
  } else if (name == "Slider") {
    ev = new LVGLEventSlider;
  } else if (name == "Spinbox") {
    ev = new LVGLEventSpinbox;
  } else if (name == "Switch") {
    ev = new LVGLEventSwitch;
  } else if (name == "TextArea") {
    ev = new LVGLEventTextArea;
  }
  return ev;
}

class LVGLPropertySetEvent : public LVGLPropertyEvent {
 public:
  inline LVGLPropertySetEvent(LVGLWidget *w)
      : LVGLPropertyEvent(w), m_firstRun(true) {}
  inline QString name() const override { return "Set Event"; }

 protected:
  inline QStringList get(LVGLObject *obj) const override {
    Q_UNUSED(obj)
    if (!m_list.isEmpty() && m_list[0] != "Empty list") return m_list;
    return QStringList();
  }
  inline void set(LVGLObject *obj, QStringList list) override {
    m_list = list;
    QMap<lv_obj_t *, QList<LVGLEvent *>> &objevlists =
        LVGLHelper::getInstance().getObjEvents();
    if (objevlists.contains(obj->obj())) {
      QList<LVGLEvent *> &listev = objevlists[obj->obj()];
      qDeleteAll(listev);
      objevlists.remove(obj->obj());
    }
    for (int i = 0; i < m_list.size(); ++i) {
      QStringList strlist = m_list.at(i).split('#');
      QString eventType = strlist[2];
      if (m_firstRun) {
        m_firstRun = false;
        lv_obj_set_click(obj->obj(), true);
        lv_obj_set_event_cb(obj->obj(), obj_events);
      }
      if (eventType == "Change Screen") {
        if (!changeScreen(objevlists, strlist, obj)) m_list[i] = "";
      } else if (eventType == "Set Property") {
        setPorp(objevlists, strlist, obj);
      } else if (eventType == "Play Animation") {
        if (!playAnim(objevlists, strlist, obj)) m_list[i] = "";
      }
    }
    while (m_list.removeOne(""))
      ;
  }

 private:
  bool changeScreen(QMap<lv_obj_t *, QList<LVGLEvent *>> &objevlists,
                    QStringList strlist, LVGLObject *obj) {
    if (objevlists.contains(obj->obj())) {
      QList<LVGLEvent *> &listev = objevlists[obj->obj()];
      for (auto s : listev)
        if (s->getResult().at(3) == strlist[3]) return false;
      LVGLEventScreen *screen = new LVGLEventScreen;
      screen->setResule(strlist);
      listev.push_back(screen);
    } else {
      QList<LVGLEvent *> listev;
      LVGLEventScreen *screen = new LVGLEventScreen;
      screen->setResule(strlist);
      listev.push_back(screen);
      objevlists[obj->obj()] = listev;
    }
    return true;
  }

  void setPorp(QMap<lv_obj_t *, QList<LVGLEvent *>> &objevlists,
               QStringList strlist, LVGLObject *obj) {
    if (objevlists.contains(obj->obj())) {
      QList<LVGLEvent *> &listev = objevlists[obj->obj()];
      // maybe need it
      //          for (auto s : listev)
      //            if (s->getResult().at(4) == strlist[4]) {
      //              listev.removeOne(s);
      //              delete s;
      //              break;
      //            }
      LVGLEvent *ev = getWidgetEvent(strlist[3]);
      ev->setResule(strlist);
      listev.push_back(ev);
    } else {
      QList<LVGLEvent *> listev;
      LVGLEvent *ev = getWidgetEvent(strlist[3]);
      ev->setResule(strlist);
      listev.push_back(ev);
      objevlists[obj->obj()] = listev;
    }
  }

  bool playAnim(QMap<lv_obj_t *, QList<LVGLEvent *>> &objevlists,
                QStringList strlist, LVGLObject *obj) {
    if (objevlists.contains(obj->obj())) {
      QList<LVGLEvent *> &listev = objevlists[obj->obj()];
      LVGLEvent *playanim = new LVGLEventAnim;
      playanim->setResule(strlist);
      listev.push_back(playanim);
    } else {
      QList<LVGLEvent *> listev;
      LVGLEvent *playanim = new LVGLEventAnim;
      playanim->setResule(strlist);
      listev.push_back(playanim);
      objevlists[obj->obj()] = listev;
    }
    return true;
  }

 private:
  QStringList m_list;
  bool m_firstRun;
};

LVGLWidget::LVGLWidget() {
  m_geometryProp = new LVGLPropertyGeometry;
  m_properties << new LVGLPropertyName;
  // m_properties << new LVGLPropertyAccessible;
  m_properties << new LVGLPropertyLocked;
  m_properties << m_geometryProp;
  m_properties << new LVGLPropertySetEvent(this);
}

LVGLWidget::~LVGLWidget() {
  qDeleteAll(m_properties);
  for (auto x : m_partsStyles) qDeleteAll(x);
}

QPixmap LVGLWidget::preview() const { return m_preview; }

void LVGLWidget::setPreview(QPixmap preview) { m_preview = preview; }

int LVGLWidget::getindexofparts(lv_obj_part_t part) {
  return m_parts.indexOf(part);
}

QList<lv_obj_part_t> LVGLWidget::parts() const { return m_parts; }

QVector<LVGLProperty *> LVGLWidget::properties() const { return m_properties; }

QList<LVGLProperty *> LVGLWidget::specialProperties() const {
  QList<LVGLProperty *> ret;
  for (LVGLProperty *p : m_properties) {
    if ((p->name() != "Name") && (p->name() != "Accessible") &&
        (p->name() != "Locked") && (p->name() != "Geometry"))
      ret << p;
  }
  return ret;
}

LVGLProperty *LVGLWidget::property(QString name) const {
  for (LVGLProperty *p : m_properties) {
    if (p->name() == name) return p;
  }
  return nullptr;
}

LVGL::StyleParts LVGLWidget::editableStyles(int type) const {
  Q_ASSERT(!m_editableStyles.isEmpty());
  if (m_editableStyles.size() > type) return m_editableStyles.at(type);
  return LVGL::None;
}

LVGLProperty *LVGLWidget::geometryProp() const { return m_geometryProp; }

lv_style_t *LVGLWidget::getstyle(int partindex, int stateindex) const {
  auto statestyle = m_partsStyles[partindex];
  return statestyle[stateindex];
}
