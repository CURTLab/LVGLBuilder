#include "LVGLProject.h"

LVGLProject::LVGLProject()
	: m_name("App")
{
}

QString LVGLProject::name() const
{
	return m_name;
}

void LVGLProject::setName(const QString &name)
{
	m_name = name;
}
