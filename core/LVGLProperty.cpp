#include "LVGLProperty.h"

#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QTextEdit>

#include "LVGLCore.h"
#include "LVGLObject.h"

LVGLProperty::LVGLProperty(LVGLProperty *parent) : m_parent(parent) {}

LVGLProperty::~LVGLProperty() { qDeleteAll(m_childs); }

bool LVGLProperty::hasEditor() const { return false; }

QWidget *LVGLProperty::editor(QWidget *parent) {
  (void)parent;
  return nullptr;
}

void LVGLProperty::updateEditor(LVGLObject *obj) { (void)obj; }

void LVGLProperty::updateWidget(LVGLObject *obj) { (void)obj; }

QVariant LVGLProperty::defaultValue() const { return QVariant(); }

QVariant LVGLProperty::value(LVGLObject *obj) const {
  (void)obj;
  return QVariant();
}

void LVGLProperty::setValue(LVGLObject *obj, QVariant value) {
  (void)obj;
  (void)value;
}

QJsonValue LVGLProperty::toJson(LVGLObject *obj) const {
  return QJsonValue::fromVariant(value(obj));
}

const LVGLProperty *LVGLProperty::parent() const { return m_parent; }

int LVGLProperty::count() const { return m_childs.size(); }

const LVGLProperty *LVGLProperty::child(int index) const {
  return m_childs.at(index);
}

int LVGLProperty::row() const { return m_parent ? m_parent->indexOf(this) : 0; }

int LVGLProperty::indexOf(const LVGLProperty *item) const {
  return m_childs.indexOf(item);
}

QStringList LVGLProperty::function(LVGLObject *obj) const {
  Q_UNUSED(obj)
  return QStringList();
}

LVGLPropertyEnum::LVGLPropertyEnum(QStringList enumText, LVGLProperty *parent)
    : LVGLProperty(parent), m_enum(enumText), m_widget(nullptr) {}

QVariant LVGLPropertyEnum::value(LVGLObject *obj) const {
  const int index = get(obj);
  Q_ASSERT(index < m_enum.size());
  return m_enum.at(index);
}

void LVGLPropertyEnum::setValue(LVGLObject *obj, QVariant value) {
  Q_ASSERT(m_enum.indexOf(value.toString()) >= 0);
  set(obj, m_enum.indexOf(value.toString()));
}

bool LVGLPropertyEnum::hasEditor() const { return true; }

QWidget *LVGLPropertyEnum::editor(QWidget *parent) {
  m_widget = new QComboBox(parent);
  m_widget->addItems(m_enum);
  return m_widget;
}

void LVGLPropertyEnum::updateEditor(LVGLObject *obj) {
  m_widget->setCurrentIndex(get(obj));
}

void LVGLPropertyEnum::updateWidget(LVGLObject *obj) {
  set(obj, m_widget->currentIndex());
}

LVGLPropertyCoord::LVGLPropertyCoord(LVGLProperty *parent)
    : LVGLPropertyType(parent), m_max(std::min(lvgl.width(), lvgl.height())) {}

LVGLPropertyCoord::LVGLPropertyCoord(Qt::Orientation orientation,
                                     LVGLProperty *parent)
    : LVGLPropertyType(parent),
      m_max(orientation == Qt::Horizontal ? lvgl.width() : lvgl.height()) {}

QWidget *LVGLPropertyCoord::editor(QWidget *parent) {
  m_widget = new QSpinBox(parent);
  m_widget->setRange(0, m_max);
  return m_widget;
}

void LVGLPropertyCoord::updateEditor(LVGLObject *obj) {
  m_widget->setValue(static_cast<int>(get(obj)));
}

void LVGLPropertyCoord::updateWidget(LVGLObject *obj) {
  set(obj, static_cast<lv_coord_t>(m_widget->value()));
}

LVGLPropertyCoordUnlimit::LVGLPropertyCoordUnlimit(LVGLProperty *parent)
    : LVGLPropertyType(parent), m_max(std::min(lvgl.width(), lvgl.height())) {}

LVGLPropertyCoordUnlimit::LVGLPropertyCoordUnlimit(Qt::Orientation orientation,
                                                   LVGLProperty *parent)
    : LVGLPropertyType(parent),
      m_max(orientation == Qt::Horizontal ? lvgl.width() : lvgl.height()) {}

QWidget *LVGLPropertyCoordUnlimit::editor(QWidget *parent) {
  m_widget = new QSpinBox(parent);
  m_widget->setRange(-9999, 9999);
  return m_widget;
}

void LVGLPropertyCoordUnlimit::updateEditor(LVGLObject *obj) {
  m_widget->setValue(static_cast<int>(get(obj)));
}

void LVGLPropertyCoordUnlimit::updateWidget(LVGLObject *obj) {
  set(obj, static_cast<lv_coord_t>(m_widget->value()));
}

LVGLPropertyInt::LVGLPropertyInt(int min, int max, LVGLProperty *parent)
    : LVGLPropertyType(parent), m_min(min), m_max(max), m_widget(nullptr) {}

LVGLPropertyInt::LVGLPropertyInt(int min, int max, QString surfix,
                                 LVGLProperty *parent)
    : LVGLPropertyType(parent),
      m_min(min),
      m_max(max),
      m_surfix(surfix),
      m_widget(nullptr) {}

QWidget *LVGLPropertyInt::editor(QWidget *parent) {
  m_widget = new QSpinBox(parent);
  m_widget->setRange(m_min, m_max);
  m_widget->setSuffix(m_surfix);
  return m_widget;
}

void LVGLPropertyInt::updateEditor(LVGLObject *obj) {
  m_widget->setValue(get(obj));
}

void LVGLPropertyInt::updateWidget(LVGLObject *obj) {
  set(obj, static_cast<lv_coord_t>(m_widget->value()));
}

LVGLPropertyFont::LVGLPropertyFont(LVGLProperty *parent)
    : LVGLProperty(parent) {}

QVariant LVGLPropertyFont::value(LVGLObject *obj) const {
  const int index = lvgl.indexOfFont(get(obj));
  return lvgl.fontNames().at(index);
}

void LVGLPropertyFont::setValue(LVGLObject *obj, QVariant value) {
  const int index = lvgl.fontNames().indexOf(value.toString());
  set(obj, lvgl.font(index));
}

bool LVGLPropertyFont::hasEditor() const { return true; }

QWidget *LVGLPropertyFont::editor(QWidget *parent) {
  m_widget = new QComboBox(parent);
  m_widget->addItems(lvgl.fontNames());
  return m_widget;
}

void LVGLPropertyFont::updateEditor(LVGLObject *obj) {
  m_widget->setCurrentIndex(lvgl.indexOfFont(get(obj)));
}

void LVGLPropertyFont::updateWidget(LVGLObject *obj) {
  set(obj, lvgl.font(m_widget->currentIndex()));
}

template <class T>
LVGLPropertyValT<T>::LVGLPropertyValT(T min, T max, QString title,
                                      QString trtitle, QString functionName,
                                      std::function<void(lv_obj_t *, T)> setter,
                                      std::function<T(lv_obj_t *)> getter,
                                      LVGLProperty *parent)
    : LVGLPropertyType<T>(parent),
      m_widget(nullptr),
      m_min(min),
      m_max(max),
      m_title(title),
      m_trtitle(trtitle),
      m_functionName(functionName),
      m_setter(setter),
      m_getter(getter) {}

template <class T>
QWidget *LVGLPropertyValT<T>::editor(QWidget *parent) {
  m_widget = new QSpinBox(parent);
  m_widget->setRange(m_min, m_max);
  return m_widget;
}

template <class T>
void LVGLPropertyValT<T>::updateEditor(LVGLObject *obj) {
  m_widget->setValue(static_cast<int>(get(obj)));
}

template <class T>
void LVGLPropertyValT<T>::updateWidget(LVGLObject *obj) {
  set(obj, static_cast<T>(m_widget->value()));
}

template <class T>
QString LVGLPropertyValT<T>::name() const {
  return m_trtitle;
}

template <class T>
QString LVGLPropertyValT<T>::codename() const {
  return m_title;
}

template <class T>
QStringList LVGLPropertyValT<T>::function(LVGLObject *obj) const {
  if (m_functionName.isEmpty()) return {};
  return {QString("%1(%2, %3);")
              .arg(m_functionName)
              .arg(obj->codeName())
              .arg(get(obj))};
}

template <class T>
T LVGLPropertyValT<T>::get(LVGLObject *obj) const {
  return m_getter(obj->obj());
}

template <class T>
void LVGLPropertyValT<T>::set(LVGLObject *obj, T value) {
  try {
    m_setter(obj->obj(), value);
  } catch (std::exception e) {
    // qDebug() << "Error:" << e.what();
  }
}

LVGLPropertyValInt16::LVGLPropertyValInt16(
    int16_t min, int16_t max, QString title, QString trtitle,
    QString functionName, std::function<void(lv_obj_t *, int16_t)> setter,
    std::function<int16_t(lv_obj_t *)> getter, LVGLProperty *parent)
    : LVGLPropertyValT<int16_t>(min, max, title, trtitle, functionName, setter,
                                getter, parent) {}

LVGLPropertyValUInt16::LVGLPropertyValUInt16(
    uint16_t min, uint16_t max, QString title, QString trtitle,
    QString functionName, std::function<void(lv_obj_t *, uint16_t)> setter,
    std::function<uint16_t(lv_obj_t *)> getter, LVGLProperty *parent)
    : LVGLPropertyValT<uint16_t>(min, max, title, trtitle, functionName, setter,
                                 getter, parent) {}

LVGLPropertyBool::LVGLPropertyBool(QString title, QString trtitle,
                                   QString functionName, LVGLProperty *parent)
    : LVGLPropertyType<bool>(parent),
      m_widget(nullptr),
      m_title(title),
      m_trtitle(trtitle),
      m_functionName(functionName) {}

LVGLPropertyBool::LVGLPropertyBool(QString title, QString trtitle,
                                   QString functionName,
                                   std::function<void(lv_obj_t *, bool)> setter,
                                   std::function<bool(lv_obj_t *)> getter,
                                   LVGLProperty *parent)
    : LVGLPropertyType<bool>(parent),
      m_widget(nullptr),
      m_title(title),
      m_trtitle(trtitle),
      m_functionName(functionName),
      m_setter(setter),
      m_getter(getter) {}

QWidget *LVGLPropertyBool::editor(QWidget *parent) {
  m_widget = new QComboBox(parent);
  m_widget->addItems(QStringList() << "true"
                                   << "false");
  return m_widget;
}

void LVGLPropertyBool::updateEditor(LVGLObject *obj) {
  m_widget->setCurrentIndex(get(obj) ? 0 : 1);
}

void LVGLPropertyBool::updateWidget(LVGLObject *obj) {
  set(obj, m_widget->currentIndex() == 0);
}

QString LVGLPropertyBool::name() const { return m_trtitle; }

QString LVGLPropertyBool::codename() const { return m_title; }

QStringList LVGLPropertyBool::function(LVGLObject *obj) const {
  if (m_functionName.isEmpty()) return {};
  return {QString("%1(%2, %3);")
              .arg(m_functionName)
              .arg(obj->codeName())
              .arg(get(obj) ? "true" : "false")};
}

bool LVGLPropertyBool::get(LVGLObject *obj) const {
  return m_getter(obj->obj());
}

void LVGLPropertyBool::set(LVGLObject *obj, bool boolean) {
  m_setter(obj->obj(), boolean);
}

LVGLPropertyString::LVGLPropertyString(QString title, QString trtitle,
                                       QString functionName,
                                       LVGLProperty *parent)
    : LVGLPropertyType<QString>(parent),
      m_widget(nullptr),
      m_title(title),
      m_trtitle(trtitle),
      m_functionName(functionName),
      m_enable(true) {}

LVGLPropertyString::LVGLPropertyString(
    QString title, QString trtitle, QString functionName,
    std::function<void(lv_obj_t *, const char *)> setter,
    std::function<const char *(lv_obj_t *)> getter, LVGLProperty *parent)
    : LVGLPropertyType<QString>(parent),
      m_widget(nullptr),
      m_title(title),
      m_trtitle(trtitle),
      m_functionName(functionName),
      m_setter(setter),
      m_getter(getter),
      m_enable(true) {}

LVGLPropertyString::LVGLPropertyString(bool b, LVGLProperty *parent)
    : LVGLPropertyType<QString>(parent), m_enable(b) {}

QString LVGLPropertyString::name() const { return m_trtitle; }

QString LVGLPropertyString::codename() const { return m_title; }

QWidget *LVGLPropertyString::editor(QWidget *parent) {
  m_widget = new QLineEdit(parent);
  return m_widget;
}

void LVGLPropertyString::updateEditor(LVGLObject *obj) {
  if (!m_enable) m_widget->setDisabled(true);

  m_widget->setText(get(obj));
}

void LVGLPropertyString::updateWidget(LVGLObject *obj) {
  set(obj, m_widget->text());
}

QStringList LVGLPropertyString::function(LVGLObject *obj) const {
  if (m_functionName.isEmpty()) return {};
  return {QString("%1(%2, \"%3\");")
              .arg(m_functionName)
              .arg(obj->codeName())
              .arg(get(obj))};
}

QString LVGLPropertyString::get(LVGLObject *obj) const {
  return m_getter(obj->obj());
}

void LVGLPropertyString::set(LVGLObject *obj, QString string) {
  m_setter(obj->obj(), qUtf8Printable(string));
}

LVGLPropertyStringPlus::LVGLPropertyStringPlus(QString title, QString trtitle,
                                               QString functionName,
                                               LVGLProperty *parent)
    : LVGLPropertyType<QString>(parent),
      m_widget(nullptr),
      m_title(title),
      m_trtitle(trtitle),
      m_functionName(functionName) {}

LVGLPropertyStringPlus::LVGLPropertyStringPlus(
    QString title, QString trtitle, QString functionName,
    std::function<void(lv_obj_t *, const char *)> setter,
    std::function<const char *(lv_obj_t *)> getter, LVGLProperty *parent)
    : LVGLPropertyType<QString>(parent),
      m_widget(nullptr),
      m_title(title),
      m_trtitle(trtitle),
      m_functionName(functionName),
      m_setter(setter),
      m_getter(getter) {}

QString LVGLPropertyStringPlus::name() const { return m_trtitle; }

QString LVGLPropertyStringPlus::codename() const { return m_title; }

QWidget *LVGLPropertyStringPlus::editor(QWidget *parent) {
  m_widget = new QTextEdit(parent);
  return m_widget;
}

void LVGLPropertyStringPlus::updateEditor(LVGLObject *obj) {
  m_widget->setText(get(obj));
}

void LVGLPropertyStringPlus::updateWidget(LVGLObject *obj) {
  set(obj, m_widget->toPlainText());
}

QStringList LVGLPropertyStringPlus::function(LVGLObject *obj) const {
  if (m_functionName.isEmpty()) return {};
  return {QString("%1(%2, \"%3\");")
              .arg(m_functionName)
              .arg(obj->codeName())
              .arg(get(obj))};
}

QString LVGLPropertyStringPlus::get(LVGLObject *obj) const {
  return m_getter(obj->obj());
}

void LVGLPropertyStringPlus::set(LVGLObject *obj, QString string) {
  m_setter(obj->obj(), qUtf8Printable(string));
}

// needed declartion for gcc, code needs to be refactored
template class LVGLPropertyValT<int16_t>;
template class LVGLPropertyValT<uint16_t>;
