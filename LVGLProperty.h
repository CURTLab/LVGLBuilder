#ifndef LVGLPROPERTY_H
#define LVGLPROPERTY_H

#include <lvgl/lvgl.h>

#include <QJsonValue>
#include <QSpinBox>
#include <QVariant>
#include <functional>

class LVGLObject;

class QComboBox;
class QLineEdit;
class QSpinBox;
class QTextEdit;

class LVGLProperty {
 public:
  LVGLProperty(LVGLProperty *parent = nullptr);
  virtual ~LVGLProperty();

  virtual QString name() const = 0;

  virtual bool hasEditor() const;
  virtual QWidget *editor(QWidget *parent);
  virtual void updateEditor(LVGLObject *obj);
  virtual void updateWidget(LVGLObject *obj);

  virtual QVariant defaultValue() const;
  virtual QVariant value(LVGLObject *obj) const;
  virtual void setValue(LVGLObject *obj, QVariant value);

  virtual QJsonValue toJson(LVGLObject *obj) const;

  const LVGLProperty *parent() const;
  int count() const;
  const LVGLProperty *child(int index) const;
  int row() const;
  int indexOf(const LVGLProperty *item) const;

  virtual QStringList function(LVGLObject *obj) const;

 protected:
  LVGLProperty *m_parent;
  QList<const LVGLProperty *> m_childs;
};

template <class T>
class LVGLPropertyType : public LVGLProperty {
 public:
  inline LVGLPropertyType(LVGLProperty *parent = nullptr)
      : LVGLProperty(parent) {}

  inline QVariant value(LVGLObject *obj) const override { return get(obj); }
  inline void setValue(LVGLObject *obj, QVariant value) override {
    return set(obj, value.value<T>());
  }
  inline bool hasEditor() const override { return true; }

 protected:
  virtual T get(LVGLObject *obj) const = 0;
  virtual void set(LVGLObject *obj, T value) = 0;
};

class LVGLPropertyEnum : public LVGLProperty {
 public:
  LVGLPropertyEnum(QStringList enumText, LVGLProperty *parent = nullptr);

  QVariant value(LVGLObject *obj) const override;
  void setValue(LVGLObject *obj, QVariant value) override;

  bool hasEditor() const override;

  QWidget *editor(QWidget *parent) override;
  void updateEditor(LVGLObject *obj) override;
  void updateWidget(LVGLObject *obj) override;

 protected:
  QStringList m_enum;
  QComboBox *m_widget;

  virtual int get(LVGLObject *obj) const = 0;
  virtual void set(LVGLObject *obj, int value) = 0;
};

class LVGLPropertyCoord : public LVGLPropertyType<lv_coord_t> {
 public:
  LVGLPropertyCoord(LVGLProperty *parent = nullptr);
  LVGLPropertyCoord(Qt::Orientation orientation,
                    LVGLProperty *parent = nullptr);

  QWidget *editor(QWidget *parent) override;
  void updateEditor(LVGLObject *obj) override;
  void updateWidget(LVGLObject *obj) override;

 protected:
  QSpinBox *m_widget;
  int m_max;
};

class LVGLPropertyInt : public LVGLPropertyType<int> {
 public:
  LVGLPropertyInt(int min, int max, LVGLProperty *parent = nullptr);
  LVGLPropertyInt(int min, int max, QString surfix,
                  LVGLProperty *parent = nullptr);

  QWidget *editor(QWidget *parent) override;
  void updateEditor(LVGLObject *obj) override;
  void updateWidget(LVGLObject *obj) override;

 protected:
  QSpinBox *m_widget;
  int m_min;
  int m_max;
  QString m_surfix;
};

class LVGLPropertyFont : public LVGLProperty {
 public:
  LVGLPropertyFont(LVGLProperty *parent = nullptr);

  QVariant value(LVGLObject *obj) const override;
  void setValue(LVGLObject *obj, QVariant value) override;

  bool hasEditor() const override;

  QWidget *editor(QWidget *parent) override;
  void updateEditor(LVGLObject *obj) override;
  void updateWidget(LVGLObject *obj) override;

 protected:
  QComboBox *m_widget;

  virtual const lv_font_t *get(LVGLObject *obj) const = 0;
  virtual void set(LVGLObject *obj, const lv_font_t *value) = 0;
};

template <class T>
class LVGLPropertyValT : public LVGLPropertyType<T> {
 public:
  LVGLPropertyValT(T min, T max, QString title, QString functionName,
                   std::function<void(lv_obj_t *, T)> setter,
                   std::function<T(lv_obj_t *)> getter,
                   LVGLProperty *parent = nullptr);

  QWidget *editor(QWidget *parent) override;
  void updateEditor(LVGLObject *obj) override;
  void updateWidget(LVGLObject *obj) override;

  QString name() const override;

  QStringList function(LVGLObject *obj) const override;

 protected:
  virtual T get(LVGLObject *obj) const override;
  virtual void set(LVGLObject *obj, T value) override;

  QSpinBox *m_widget;
  T m_min;
  T m_max;
  QString m_title;
  QString m_functionName;
  std::function<void(lv_obj_t *, T)> m_setter;
  std::function<T(lv_obj_t *)> m_getter;
};

class LVGLPropertyValInt16 : public LVGLPropertyValT<int16_t> {
 public:
  LVGLPropertyValInt16(int16_t min, int16_t max, QString title,
                       QString functionName,
                       std::function<void(lv_obj_t *, int16_t)> setter,
                       std::function<int16_t(lv_obj_t *)> getter,
                       LVGLProperty *parent = nullptr);
};

class LVGLPropertyValUInt16 : public LVGLPropertyValT<uint16_t> {
 public:
  LVGLPropertyValUInt16(uint16_t min, uint16_t max, QString title,
                        QString functionName,
                        std::function<void(lv_obj_t *, uint16_t)> setter,
                        std::function<uint16_t(lv_obj_t *)> getter,
                        LVGLProperty *parent = nullptr);
};

class LVGLPropertyBool : public LVGLPropertyType<bool> {
 public:
  LVGLPropertyBool(QString title = "", QString functionName = "",
                   LVGLProperty *parent = nullptr);
  LVGLPropertyBool(QString title, QString functionName,
                   std::function<void(lv_obj_t *, bool)> setter,
                   std::function<bool(lv_obj_t *)> getter,
                   LVGLProperty *parent = nullptr);

  QWidget *editor(QWidget *parent) override;
  void updateEditor(LVGLObject *obj) override;
  void updateWidget(LVGLObject *obj) override;

  QString name() const override;

  QStringList function(LVGLObject *obj) const override;

 protected:
  bool get(LVGLObject *obj) const override;
  void set(LVGLObject *obj, bool boolean) override;

  QComboBox *m_widget;
  QString m_title;
  QString m_functionName;
  std::function<void(lv_obj_t *, bool)> m_setter;
  std::function<bool(lv_obj_t *)> m_getter;
};

class LVGLPropertyString : public LVGLPropertyType<QString> {
 public:
  LVGLPropertyString(QString title = "", QString functionName = "",
                     LVGLProperty *parent = nullptr);
  LVGLPropertyString(QString title, QString functionName,
                     std::function<void(lv_obj_t *, const char *)> setter,
                     std::function<const char *(lv_obj_t *)> getter,
                     LVGLProperty *parent = nullptr);

  QString name() const override;

  QWidget *editor(QWidget *parent) override;
  void updateEditor(LVGLObject *obj) override;
  void updateWidget(LVGLObject *obj) override;

  QStringList function(LVGLObject *obj) const override;

 protected:
  QString get(LVGLObject *obj) const override;
  void set(LVGLObject *obj, QString string) override;

  QLineEdit *m_widget;
  QString m_title;
  QString m_functionName;
  std::function<void(lv_obj_t *, const char *)> m_setter;
  std::function<const char *(lv_obj_t *)> m_getter;
};

class LVGLPropertyStringPlus : public LVGLPropertyType<QString> {
 public:
  LVGLPropertyStringPlus(QString title = "", QString functionName = "",
                         LVGLProperty *parent = nullptr);
  LVGLPropertyStringPlus(QString title, QString functionName,
                         std::function<void(lv_obj_t *, const char *)> setter,
                         std::function<const char *(lv_obj_t *)> getter,
                         LVGLProperty *parent = nullptr);

  QString name() const override;

  QWidget *editor(QWidget *parent) override;
  void updateEditor(LVGLObject *obj) override;
  void updateWidget(LVGLObject *obj) override;

  QStringList function(LVGLObject *obj) const override;

 protected:
  QString get(LVGLObject *obj) const override;
  void set(LVGLObject *obj, QString string) override;

  QTextEdit *m_widget;
  QString m_title;
  QString m_functionName;
  std::function<void(lv_obj_t *, const char *)> m_setter;
  std::function<const char *(lv_obj_t *)> m_getter;
};

#endif  // LVGLPROPERTY_H
