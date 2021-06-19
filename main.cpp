#include <QApplication>
#include <QTranslator>
#include <QVBoxLayout>
#include <QWidget>

#include "MainWindow.h"
#include "core/LVGLConfig.h"
#include "core/MainwindoTitle.h"

//#include "vld.h"
#ifdef Q_OS_WIN
#include "breakpad/client/windows/handler/exception_handler.h"

bool callback(const wchar_t *dump_path, const wchar_t *id, void *context,
              EXCEPTION_POINTERS *exinfo, MDRawAssertionInfo *assertion,
              bool succeeded) {
  return succeeded;
}
#endif

int main(int argc, char *argv[]) {
#ifdef Q_OS_WIN
  google_breakpad::ExceptionHandler eh(
      L".", NULL, callback, NULL,
      google_breakpad::ExceptionHandler::HANDLER_ALL);
#endif
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QApplication a(argc, argv);
  LVGLConfig config;

  QString lgfile = config.getVar("Language", "name").toString();
  if (lgfile.isEmpty()) {
    config.setVar("Language", "name", "en.qm");
    config.setVar("Language", "index", 1);
    lgfile = "en.qm";
  }

  QTranslator qtTranslator;
  if (qtTranslator.load(a.applicationDirPath() + "/translations/" + lgfile))
    a.installTranslator(&qtTranslator);

  QWidget parent;
  parent.setWindowFlags(Qt::FramelessWindowHint | parent.windowFlags());
  QVBoxLayout *ly = new QVBoxLayout(&parent);
  ly->setContentsMargins(0, 0, 0, 0);
  ly->setSpacing(0);

  MainwindoTitle wt(&parent);
  MainWindow w(&parent);
  w.settranslator(&qtTranslator);
  w.setconfig(&config);

  ly->insertWidget(0, &wt);
  ly->insertWidget(1, &w);
  parent.setLayout(ly);
  parent.showMaximized();

  return a.exec();
}
