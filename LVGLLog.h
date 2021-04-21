#ifndef LVGLLOG_H
#define LVGLLOG_H

#include <QString>

class LVGLLog {
 public:
  enum eLogType {
    LOG_LEVEL_TRACE = 0,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_NONE
  };

 public:
  ~LVGLLog(){};
  LVGLLog(const LVGLLog&) = delete;
  LVGLLog operator=(const LVGLLog&) = delete;
  static LVGLLog& getInstance() {
    static LVGLLog loginstance;
    return loginstance;
  }
  void log(const QString& logStr, const char* file, int line, const char* func);
  void log(eLogType logtype, const QString& logStr, const char* file, int line,
           const char* func);
  void log_trace(const QString& logStr, const char* file, int line,
                 const char* func);
  void log_info(const QString& logStr, const char* file, int line,
                const char* func);
  void log_warn(const QString& logStr, const char* file, int line,
                const char* func);
  void log_error(const QString& logStr, const char* file, int line,
                 const char* func);

 private:
  LVGLLog();
  void logToFile(const QString& logStr, const char* file, int line,
                 const char* func);
};

extern LVGLLog::eLogType LogType;

#endif  // LVGLLOG_H
