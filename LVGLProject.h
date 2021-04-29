#ifndef LVGLPROJECT_H
#define LVGLPROJECT_H

#include <QSize>
#include <QString>

class LVGLProject {
 public:
  LVGLProject();
  LVGLProject(const QString &name, QSize resolution);

  QString name() const;
  QSize resolution() const;

  static LVGLProject *load(const QString &fileName);
  bool save(const QString &fileName);
  bool exportCode(const QString &path) const;

  QString fileName() const;
  void setName(const QString &name) { m_name = name; }

 private:
  QString m_name;
  QString m_fileName;
  QSize m_resolution;
};

#endif  // LVGLPROJECT_H
