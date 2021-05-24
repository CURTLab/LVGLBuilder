#include "LVGLExportThread.h"

#include <QFileDialog>

#include "LVGLProject.h"

LVGLExportThread::LVGLExportThread() : m_project(nullptr) {}

LVGLExportThread::~LVGLExportThread() {
  if (m_project) delete m_project;
}

void LVGLExportThread::startrun(const QStringList &list) {
  stopped = false;
  if (!m_project) m_project = new LVGLProject;
  if (list.isEmpty() || list.count() != 2) {
    if (!stopped) emit failed();
  } else {
    m_project->setName(list[0]);
    if (m_project->exportCode(list[1])) {
      if (!stopped) emit successful();
    } else {
      if (!stopped) emit failed();
    }
  }
}

void LVGLExportThread::stop() { stopped = true; }
