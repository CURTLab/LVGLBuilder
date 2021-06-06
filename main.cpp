#include <QApplication>

#include "MainWindow.h"
#include "breakpad/client/windows/handler/exception_handler.h"
//#include "vld.h"

bool callback(const wchar_t *dump_path, const wchar_t *id, void *context,
              EXCEPTION_POINTERS *exinfo, MDRawAssertionInfo *assertion,
              bool succeeded) {
  return succeeded;
}

int main(int argc, char *argv[]) {
  google_breakpad::ExceptionHandler eh(
      L".", NULL, callback, NULL,
      google_breakpad::ExceptionHandler::HANDLER_ALL);
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QApplication a(argc, argv);

  MainWindow w;

  w.showMaximized();

  return a.exec();
}
