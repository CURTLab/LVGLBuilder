#ifndef LVGLPROJECT_H
#define LVGLPROJECT_H

#include <QString>

class LVGLProject
{
public:
	LVGLProject();
	LVGLProject(const QString &name);

	QString name() const;

	static LVGLProject *load(const QString &fileName);
	bool save(const QString &fileName) const;
	bool exportCode(const QString &path) const;

private:
	QString m_name;

};

#endif // LVGLPROJECT_H
