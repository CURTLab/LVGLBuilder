#include "LVGLThreads.h"

#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QTimer>

#include "LVGLCore.h"
#include "LVGLFontData.h"
#include "LVGLHelper.h"
#include "LVGLObject.h"
#include "LVGLProject.h"
#include "MainWindow.h"

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

LVGLAutoSaveThread::LVGLAutoSaveThread() : m_time(nullptr) {}

LVGLAutoSaveThread::~LVGLAutoSaveThread() {}

void LVGLAutoSaveThread::startrun(int state) {
  if (m_time) {
    delete m_time;
    m_time = new QTimer;
    connect(m_time, &QTimer::timeout, this, &LVGLAutoSaveThread::saveFile);
  }
  m_time->start(state * 60000);
}

void LVGLAutoSaveThread::saveFile() {
  QString fileName;
  QFile file(fileName);
  if (!file.open(QIODevice::WriteOnly)) return;

  QJsonArray widgetArr;
  for (LVGLObject *o : lvgl.allObjects()) {
    if (o->parent() == nullptr) {
      if (o->doesNameExists()) o->generateName();
      widgetArr.append(o->toJson());
    }
  }

  QJsonArray imageArr;
  for (LVGLImageData *i : lvgl.images()) {
    if (!i->fileName().isEmpty()) imageArr.append(i->toJson());
  }

  //  QJsonArray fontArr;
  //  for (const LVGLFontData *f : lvgl.customFonts())
  //  fontArr.append(f->toJson());

  //  QJsonObject resolution(
  //      {{"width", m_resolution.width()}, {"height", m_resolution.height()}});
  //  QJsonObject screen(
  //      {{"widgets", widgetArr}, {"name", m_name}, {"resolution",
  //      resolution}});
  //  screen.insert("screen color", QVariant(lvgl.screenColor()).toString());
  //  QJsonObject lvgl(
  //      {{"lvgl", screen}, {"images", imageArr}, {"fonts", fontArr}});
  //  QJsonDocument doc(lvgl);

  // file.write(doc.toJson());
  file.close();
}

void LVGLAutoSaveThread::stop() {
  stopped = true;
  if (m_time) {
    delete m_time;
    m_time = nullptr;
  }
}
