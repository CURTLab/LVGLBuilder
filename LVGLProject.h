#ifndef LVGLPROJECT_H
#define LVGLPROJECT_H

#include <QString>
#include <QSize>

class LVGLProject
{
public:
	LVGLProject();
	LVGLProject(const QString &name, QSize resolution);

	QString name() const;
	QSize resolution() const;

	static LVGLProject *load(const QString &fileName);
	bool save(const QString &fileName) const;
	bool exportCode(const QString &path) const;

private:
	QString m_name;
	QSize m_resolution;

};

#endif // LVGLPROJECT_H
