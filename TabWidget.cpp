#include "TabWidget.h"

#include <QDebug>
#include <QFileDialog>
#include <QGridLayout>
#include <QInputDialog>
#include <QMessageBox>
#include <QSettings>
#include <QSortFilterProxyModel>

#include "LVGLCore.h"
#include "LVGLDialog.h"
#include "LVGLFontData.h"
#include "LVGLFontDialog.h"
#include "LVGLHelper.h"
#include "LVGLNewDialog.h"
#include "LVGLObjectModel.h"
#include "LVGLProject.h"
#include "LVGLPropertyModel.h"
#include "LVGLSimulator.h"
#include "LVGLStyleModel.h"
#include "LVGLWidgetModel.h"
#include "MainWindow.h"

TabWidget::TabWidget(const QString &name, QWidget *parent)
    : QWidget(parent),
      m_core(Q_NULLPTR),
      m_simulator(Q_NULLPTR),
      m_project(Q_NULLPTR),
      m_name(name) {
  initLvglWidget();
}

TabWidget::~TabWidget() {
  delete m_project;
  delete m_core;
  delete m_simulator;
}

void TabWidget::initLvglWidget() {
  m_core = new LVGLCore(this);
  m_simulator = new LVGLSimulator(m_core, this);
  m_project = new LVGLProject(m_name, QSize(720, 720));
  QGridLayout *glayout = new QGridLayout(this);
  glayout->addWidget(m_simulator, 0, 0, 1, 1);
}

void TabWidget::initLvglCore(int w, int h) { m_core->init(w, h); }
