#ifndef LVGLNEWDIALOG_H
#define LVGLNEWDIALOG_H

#include <QDialog>

namespace Ui {
class LVGLNewDialog;
}

class LVGLNewDialog : public QDialog
{
	Q_OBJECT

public:
	explicit LVGLNewDialog(QWidget *parent = nullptr);
	~LVGLNewDialog();

	QString selectedName() const;

private:
	Ui::LVGLNewDialog *m_ui;

};

#endif // LVGLNEWDIALOG_H
