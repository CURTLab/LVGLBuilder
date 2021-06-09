#include "LVGLConfig.h"

#include <QApplication>
#include <QFile>

LVGLConfig::LVGLConfig(QString cgname) {
  if (cgname.isEmpty()) {
    m_cgname = QCoreApplication::applicationDirPath() + "/config.ini";
  } else {
    m_cgname = cgname;
  }

  m_psetting = new QSettings(m_cgname, QSettings::IniFormat);
}

LVGLConfig::~LVGLConfig() {
  delete m_psetting;
  m_psetting = 0;
}

void LVGLConfig::setVar(QString nodename, QString keyname, QVariant value) {
  m_psetting->setValue(QString("%1/%2").arg(nodename, keyname), value);
  m_psetting->sync();
}

QVariant LVGLConfig::getVar(QString nodename, QString keyname) {
  return m_psetting->value(QString("%1/%2").arg(nodename, keyname));
}
