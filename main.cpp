#include <QApplication>

#include "LVGLSimulator.h"
#include "MainWindow.h"

int main(int argc, char *argv[]) {
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QApplication a(argc, argv);

  MainWindow w;
  LVGLKeyPressEventFilter filter(w.simulator(), &a);
  a.installEventFilter(&filter);

  w.showMaximized();

  return a.exec();
}
