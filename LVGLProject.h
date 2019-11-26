#ifndef LVGLPROJECT_H
#define LVGLPROJECT_H

#include <QString>

class LVGLProject
{
public:
	LVGLProject();

	QString name() const;
	void setName(const QString &name);

private:
	QString m_name;

};

#endif // LVGLPROJECT_H
