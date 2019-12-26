#include "LVGLFontDialog.h"
#include "ui_LVGLFontDialog.h"

#include <QSettings>
#include <QFile>
#include <QFileDialog>

LVGLFontDialog::LVGLFontDialog(QWidget *parent)
	: QDialog(parent)
	, m_ui(new Ui::LVGLFontDialog)
{
	m_ui->setupUi(this);

	// from https://www.qtcentre.org/threads/26924-Retrieve-the-filename-of-a-QFont
	QSettings settings("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts", QSettings::NativeFormat);
	QStringList list = settings.allKeys();

	QString winPath = QString::fromUtf8(qgetenv("windir"));
	for (int i = 0; i < list.size(); ++i) {
		QString key = list.at(i);
		QString text = settings.value(key).toString();
		if (!text.endsWith(".ttf", Qt::CaseInsensitive))
			continue;
		QString fileName = winPath + "\\Fonts\\" + text;
		if (!QFile(fileName).exists())
			continue;

		QStringList keys = key.split(" & ");
		QString last = keys.last();
		keys[keys.size()-1] = last.left(last.indexOf(" ("));
		for(int j = 0; j < keys.size(); ++j) {
			m_index.insert(keys[j], j);
			m_name.insert(keys[j], fileName);
			m_ui->list_font->addItem(keys[j]);
		}
	}
}

LVGLFontDialog::~LVGLFontDialog()
{
	delete m_ui;
}

QString LVGLFontDialog::selectedFontPath() const
{
	return m_ui->preview->text();
}

uint8_t LVGLFontDialog::selectedFontSize() const
{
	return m_ui->spin_size->value() & 0xff;
}

uint8_t LVGLFontDialog::selectedBPP() const
{
	if (m_ui->comboBox->currentIndex() == 0)
		return 4;
	else if (m_ui->comboBox->currentIndex() == 1)
		return 2;
	else
		return 1;
}

void LVGLFontDialog::on_button_select_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this, "Select font", "", "TTF Font (*.ttf)");
	if (fileName.isEmpty())
		return;
	m_ui->preview->setText(fileName);
}

void LVGLFontDialog::on_list_font_currentTextChanged(const QString &currentText)
{
	m_ui->preview->setText(m_name[currentText]);
}
