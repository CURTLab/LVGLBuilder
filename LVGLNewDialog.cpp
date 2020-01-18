#include "LVGLNewDialog.h"
#include "ui_LVGLNewDialog.h"

#include <QSettings>

LVGLNewDialog::LVGLNewDialog(QWidget *parent)
	: QDialog(parent)
	, m_ui(new Ui::LVGLNewDialog)
{
	m_ui->setupUi(this);

	m_ui->group_oriantation->addButton(m_ui->radio_landscape, 0);
	m_ui->group_oriantation->addButton(m_ui->radio_portrait, 1);

	m_resolutions << qMakePair(480, 320);
	m_resolutions << qMakePair(320, 240);
	m_resolutions << qMakePair(160, 120);

	for (const auto &r:m_resolutions)
		m_ui->combo_resolution->addItem(QString("%1 x %2").arg(r.first).arg(r.second));

	connect(m_ui->combo_resolution, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
			  this, &LVGLNewDialog::resolutionChanged
			  );

	QSettings settings("at.fhooe.lvgl", "LVGL Builder");

	m_ui->combo_resolution->setCurrentIndex(settings.value("resolution index", 0).toInt());
	const QSize size = settings.value("resolution", QSize(1, 1)).toSize();
	m_ui->spin_width->setValue(size.width());
	m_ui->spin_height->setValue(size.height());
	const int oriantation = settings.value("oriantation", 0).toInt();
	m_ui->group_oriantation->button(oriantation)->setChecked(true);
}

LVGLNewDialog::~LVGLNewDialog()
{
	delete m_ui;
}

QString LVGLNewDialog::selectedName() const
{
	return m_ui->edit_name->text();
}

QPair<lv_coord_t, lv_coord_t> LVGLNewDialog::selectedResolution() const
{
	lv_coord_t w = static_cast<lv_coord_t>(m_ui->spin_width->value());
	lv_coord_t h = static_cast<lv_coord_t>(m_ui->spin_height->value());

	if (m_ui->radio_portrait->isChecked())
		qSwap(w, h);
	return qMakePair<lv_coord_t,lv_coord_t>(w, h);
}

void LVGLNewDialog::accept()
{
	QSettings settings("at.fhooe.lvgl", "LVGL Builder");
	settings.setValue("resolution index", m_ui->combo_resolution->currentIndex());
	settings.setValue("resolution", QSize(m_ui->spin_width->value(), m_ui->spin_height->value()));
	settings.setValue("oriantation", m_ui->group_oriantation->checkedId());
	QDialog::accept();
}

void LVGLNewDialog::resolutionChanged(int index)
{
	m_ui->spin_width->setEnabled(index == 0);
	m_ui->spin_height->setEnabled(index == 0);

	if (index > 0) {
		m_ui->spin_width->setValue(m_resolutions[index - 1].first);
		m_ui->spin_height->setValue(m_resolutions[index - 1].second);
	}
}
