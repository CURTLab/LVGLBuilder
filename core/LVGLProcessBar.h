#ifndef LVGLProcessBar_H
#define LVGLProcessBar_H

#include <QDialog>

class QProcess;

QT_BEGIN_NAMESPACE
namespace Ui {
class LVGLProcessBar;
}
QT_END_NAMESPACE

class LVGLProcessBar : public QDialog {
  Q_OBJECT

 public:
  LVGLProcessBar(QWidget *parent = nullptr);
  ~LVGLProcessBar();
 signals:
  void sigclose();

 protected:
  void closeEvent(QCloseEvent *event);

 private:
  Ui::LVGLProcessBar *ui;
};

#endif  // LVGLProcessBar_H
