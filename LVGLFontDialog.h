#ifndef LVGLFONTDIALOG_H
#define LVGLFONTDIALOG_H

#include <QDialog>

namespace Ui {
class LVGLFontDialog;
}

class LVGLFontDialog : public QDialog
{
	Q_OBJECT

public:
	explicit LVGLFontDialog(QWidget *parent = nullptr);
	~LVGLFontDialog();

	QString selectedFontPath() const;
	uint8_t selectedFontSize() const;
	uint8_t selectedBPP() const;

private slots:
	void on_button_select_clicked();
	void on_list_font_currentTextChanged(const QString &currentText);

private:
	Ui::LVGLFontDialog *m_ui;
	QHash<QString, int> m_index;
	QHash<QString, QString> m_name;

};

#endif // LVGLFONTDIALOG_H
