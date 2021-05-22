#include "LVGLExportThread.h"

#include <QFileDialog>

#include "LVGLProject.h"

LVGLExportThread::LVGLExportThread() : m_project(nullptr) {}

LVGLExportThread::~LVGLExportThread() {
  if (m_project) delete m_project;
}

void LVGLExportThread::startrun(QString path) {
  stopped = false;
  if (!m_project) m_project = new LVGLProject;
  if (path.isEmpty()) {
    if (!stopped) emit failed();
  } else {
    if (m_project->exportCode(path)) {
      if (!stopped) emit successful();
    } else {
      if (!stopped) emit failed();
    }
  }
}

void LVGLExportThread::stop() { stopped = true; }
