#include "LVGLLog.h"

#include <QApplication>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QTextCodec>
#include <QTextStream>

LVGLLog::eLogType LogType = LVGLLog::eLogType::LOG_LEVEL_INFO;

void LVGLLog::log(const QString &logStr, const char *file, int line,
                  const char *func) {
  log(LogType, logStr, file, line, func);
}

void LVGLLog::log(LVGLLog::eLogType logtype, const QString &logStr,
                  const char *file, int line, const char *func) {
  switch (logtype) {
    case LOG_LEVEL_TRACE:
      log_trace(logStr, file, line, func);
      break;
    case LOG_LEVEL_INFO:
      log_info(logStr, file, line, func);
      break;
    case LOG_LEVEL_WARN:
      log_warn(logStr, file, line, func);
      break;
    case LOG_LEVEL_ERROR:
      log_error(logStr, file, line, func);
      break;
    case LOG_LEVEL_NONE:
      break;
  }
}

void LVGLLog::log_trace(const QString &logStr, const char *file, int line,
                        const char *func) {
  if (LogType <= LOG_LEVEL_TRACE) logToFile(logStr, file, line, func);
}

void LVGLLog::log_info(const QString &logStr, const char *file, int line,
                       const char *func) {
  if (LogType <= LOG_LEVEL_INFO) logToFile(logStr, file, line, func);
}

void LVGLLog::log_warn(const QString &logStr, const char *file, int line,
                       const char *func) {
  if (LogType <= LOG_LEVEL_WARN) logToFile(logStr, file, line, func);
}

void LVGLLog::log_error(const QString &logStr, const char *file, int line,
                        const char *func) {
  if (LogType <= LOG_LEVEL_ERROR) logToFile(logStr, file, line, func);
}

LVGLLog::LVGLLog() {}

void LVGLLog::logToFile(const QString &logStr, const char *file, int line,
                        const char *func) {
  QString path = qApp->applicationDirPath();
  QDateTime time = QDateTime::currentDateTime();
  QString dateStr = time.toString("yyyy-MM-dd");
  QString dateAllStr = time.toString(("yyyy-MM-dd hh:mm:ss ddd"));
  QString logFileName = "/axs_" + dateStr + ".log";

  QFile logFile(path + logFileName);
  QTextStream stream;
  stream.setDevice(&logFile);
  stream.setCodec(QTextCodec::codecForName("UTF-8"));
  QString message = QString("Date:%1  File:%2  Line:%3  Func:%4  Msg:%5\n")
                        .arg(dateAllStr)
                        .arg(file)
                        .arg(line)
                        .arg(func)
                        .arg(logStr);

  logFile.open(QIODevice::WriteOnly | QIODevice::Append);
  stream << message;
  logFile.close();
}
