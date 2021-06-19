#ifndef MAINWINDOTITLE_H
#define MAINWINDOTITLE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainwindoTitle;
}
QT_END_NAMESPACE

class MainwindoTitle : public QWidget {
  Q_OBJECT

 public:
  MainwindoTitle(QWidget *parent = nullptr);
  ~MainwindoTitle();

 protected:
  void paintEvent(QPaintEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseDoubleClickEvent(QMouseEvent *event) override;

 private slots:
  void on_pushButton_clicked();

  void on_pushButton_2_clicked();

  void on_pushButton_3_clicked();

 private:
  Ui::MainwindoTitle *ui;
  QPoint m_curPos;
};
#endif  // MAINWINDOTITLE_H
