#ifndef NEWDIALOG_H
#define NEWDIALOG_H

#include <QDialog>

namespace Ui {
class NewDialog;
}

class NewDialog : public QDialog
{
	Q_OBJECT

public:
	explicit NewDialog(QWidget *parent = nullptr);
	~NewDialog();

	QString name() const;

private:
	Ui::NewDialog *m_ui;

};

#endif // NEWDIALOG_H
