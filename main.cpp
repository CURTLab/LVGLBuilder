#include "MainWindow.h"

#include <QApplication>
#include "LVGLCore.h"
#include "LVGLSimulator.h"
#include <QDebug>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	lvgl.init(320, 480);

	MainWindow w;
	LVGLKeyPressEventFilter filter(w.simulator(), &a);
	a.installEventFilter(&filter);

	w.showMaximized();

	return a.exec();
}
