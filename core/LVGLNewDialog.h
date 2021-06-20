#ifndef LVGLNEWDIALOG_H
#define LVGLNEWDIALOG_H

#include <QDialog>

#include "LVGLCore.h"

namespace Ui {
class LVGLNewDialog;
}

class LVGLNewDialog : public QDialog {
  Q_OBJECT

 public:
  explicit LVGLNewDialog(QWidget *parent = nullptr);
  virtual ~LVGLNewDialog() override;

  QString selectName() const;
  QSize selectedResolution() const;
  void setoptenable(bool able);

 public slots:
  virtual void accept() override;

 protected:
  void mousePressEvent(QMouseEvent *e) override;
  void mouseMoveEvent(QMouseEvent *e) override;
  void keyPressEvent(QKeyEvent *e) override;
  void paintEvent(QPaintEvent *event) override;

 private slots:
  void resolutionChanged(int index);

  void on_close_clicked();

 private:
  Ui::LVGLNewDialog *m_ui;
  QVector<QPair<lv_coord_t, lv_coord_t>> m_resolutions;
  QPoint m_curPos;
  QColor m_bgcolor;
};

#endif  // LVGLNEWDIALOG_H
