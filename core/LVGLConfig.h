#ifndef LVGLCONFIG_H
#define LVGLCONFIG_H

#include <QSettings>
#include <QVariant>

class LVGLConfig {
 public:
  LVGLConfig(QString cgname = "");
  virtual ~LVGLConfig(void);
  void setVar(QString, QString, QVariant);
  QVariant getVar(QString, QString);

 private:
  QString m_cgname;
  QSettings *m_psetting;
};

#endif  // LVGLCONFIG_H
