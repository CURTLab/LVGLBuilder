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

  void setres(const QSize &res) { m_resolution = res; };
  void setName(const QString &name) { m_name = name; }
  QString fileName() const;

 private:
  QString m_name;
  QString m_fileName;
  QSize m_resolution;
};

#endif  // LVGLPROJECT_H
