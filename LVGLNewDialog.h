#ifndef LVGLNEWDIALOG_H
#define LVGLNEWDIALOG_H

#include <QDialog>

#include "LVGLCore.h"

namespace Ui {
class LVGLNewDialog;
}

class LVGLNewDialog : public QDialog
{
	Q_OBJECT

public:
	explicit LVGLNewDialog(QWidget *parent = nullptr);
	virtual ~LVGLNewDialog() override;

	QString selectedName() const;
	QPair<lv_coord_t,lv_coord_t> selectedResolution() const;

public slots:
	virtual void accept() override;

private slots:
	void resolutionChanged(int index);

private:
	Ui::LVGLNewDialog *m_ui;
	QVector<QPair<lv_coord_t,lv_coord_t>> m_resolutions;

};

#endif // LVGLNEWDIALOG_H
