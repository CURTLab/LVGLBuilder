#include "LVGLThreads.h"

#include <QApplication>
#include <QDateTime>
#include <QDebug>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QTimer>

#include "LVGLCore.h"
#include "LVGLFontData.h"
#include "LVGLHelper.h"
#include "LVGLObject.h"
#include "LVGLProject.h"
#include "LVGLTabWidget.h"
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
    m_project->setProjeName(list[0]);
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
  stopped = false;
  m_saveCount = 0;
  if (!m_time) {
    delete m_time;
    m_time = new QTimer;
    connect(m_time, &QTimer::timeout, this, &LVGLAutoSaveThread::saveFile);
  } else
    m_time->stop();
  m_time->start(state * 60000);
}

void LVGLAutoSaveThread::saveFile() {
  if (m_saveCount > 2) m_saveCount = 0;
  ++m_saveCount;
  LVGLHelper::getInstance().updatetabDate();
  auto tabw = LVGLHelper::getInstance().getMainW()->getTabW();
  for (int i = 0; i < tabw->count(); ++i) {
    if (stopped) return;
    auto tab = static_cast<LVGLTabWidget *>(tabw->widget(i));
    QString name = tab->getfilename();
    QString timestr = QDateTime::currentDateTime().toString("yyMMdd");
    QString fileName = QString("%1/%2_%3_%4.lvgl")
                           .arg(QApplication::applicationDirPath())
                           .arg(m_saveCount)
                           .arg(timestr)
                           .arg(name);
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) return;
    if (stopped) {
      file.close();
      return;
    }

    QJsonArray widgetArr;
    for (LVGLObject *o : tab->allObject()) {
      if (o->parent() == nullptr) {
        widgetArr.append(o->toJson());
      }
    }
    if (stopped) {
      file.close();
      return;
    }

    QJsonArray imageArr;
    for (LVGLImageData *i : tab->allImages()) {
      if (!i->fileName().isEmpty()) imageArr.append(i->toJson());
    }
    if (stopped) {
      file.close();
      return;
    }

    QJsonArray fontArr;
    for (const LVGLFontData *f : lvgl.customFonts())
      fontArr.append(f->toJson());
    if (stopped) {
      file.close();
      return;
    }

    QJsonObject resolution(
        {{"width", lvgl.width()}, {"height", lvgl.height()}});
    QJsonObject screen(
        {{"widgets", widgetArr}, {"name", name}, {"resolution", resolution}});
    screen.insert("screen color",
                  QVariant(lvgl.screenColor(tab->getparent())).toString());
    QJsonObject lvgl(
        {{"lvgl", screen}, {"images", imageArr}, {"fonts", fontArr}});
    QJsonDocument doc(lvgl);

    file.write(doc.toJson());
    file.close();
  }
}

void LVGLAutoSaveThread::stop() {
  stopped = true;
  if (m_time) {
    delete m_time;
    m_time = nullptr;
  }
}
