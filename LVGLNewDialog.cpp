#include "LVGLNewDialog.h"
#include "ui_LVGLNewDialog.h"

LVGLNewDialog::LVGLNewDialog(QWidget *parent)
	: QDialog(parent)
	, m_ui(new Ui::LVGLNewDialog)
{
	m_ui->setupUi(this);
}

LVGLNewDialog::~LVGLNewDialog()
{
	delete m_ui;
}

QString LVGLNewDialog::selectedName() const
{
	return m_ui->edit_name->text();
}
