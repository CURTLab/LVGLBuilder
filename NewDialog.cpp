#include "NewDialog.h"
#include "ui_NewDialog.h"

NewDialog::NewDialog(QWidget *parent)
	: QDialog(parent)
	, m_ui(new Ui::NewDialog)
{
	m_ui->setupUi(this);
}

NewDialog::~NewDialog()
{
	delete m_ui;
}

QString NewDialog::name() const
{
	return m_ui->edit_name->text();
}
