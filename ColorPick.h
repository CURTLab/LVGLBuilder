#ifndef COLORPICK_H
#define COLORPICK_H

#include <QWidget>

class QColorDialog;

QT_BEGIN_NAMESPACE
namespace Ui {
class ColorPick;
}
QT_END_NAMESPACE

class ColorPick : public QWidget {
  Q_OBJECT

 public:
  ColorPick(QWidget *parent = nullptr);
  ~ColorPick();
  QString colorString();
  void setColor(const QColor &col);

 private slots:
  void on_toolButton_clicked();

 private:
  Ui::ColorPick *ui;
  QColorDialog *m_colordig;
  QPixmap *m_pix;
  QColor m_curColor;
  ;
};
#endif  // COLORPICK_H
