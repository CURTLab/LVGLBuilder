#include "LVGLPropertyFlags.h"

#include <QStandardItemModel>
#include <QComboBox>

LVGLPropertyFlags::LVGLPropertyFlags(QStringList flagNames, QStringList flagCodeName, QList<uint8_t> flags, LVGLProperty *parent)
	: LVGLProperty(parent)
	, m_flagNames(flagNames)
	, m_flagCodeName(flagCodeName)
	, m_flags(flags)
{
	assert(m_flagNames.size() == m_flagCodeName.size());
	assert(m_flagNames.size() == m_flags.size());
}

bool LVGLPropertyFlags::hasEditor() const
{
	return true;
}

QWidget *LVGLPropertyFlags::editor(QWidget *parent)
{
	m_model = new QStandardItemModel(3, 1);
	for (int r = 0; r < m_flagNames.size(); ++r) {
		QStandardItem *item = new QStandardItem(m_flagNames.at(r));

		item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
		item->setData(Qt::Unchecked, Qt::CheckStateRole);

		m_model->setItem(r, 0, item);
	}

	m_widget = new QComboBox(parent);
	m_widget->setModel(m_model);

	return m_widget;
}

void LVGLPropertyFlags::updateEditor(LVGLObject *obj)
{
	const int flags = get(obj);
	for (int r = 0; r < m_flagNames.size(); ++r) {
		Qt::CheckState state = (flags & m_flags.at(r) ? Qt::Checked : Qt::Unchecked);
		m_model->item(r)->setData(state, Qt::CheckStateRole);
	}
}

void LVGLPropertyFlags::updateWidget(LVGLObject *obj)
{
	int flags = 0;
	for (int r = 0; r < m_flagNames.size(); ++r) {
		if (m_model->item(r)->data(Qt::CheckStateRole).value<Qt::CheckState>() ==  Qt::Checked)
			flags |= m_flags.at(r);
	}
	set(obj, flags);
}

QVariant LVGLPropertyFlags::value(LVGLObject *obj) const
{
	QStringList ret;
	const int flags = get(obj);
	for (int r = 0; r < m_flagNames.size(); ++r) {
		if (flags & m_flags.at(r))
			ret << m_flagNames.at(r);
	}
	return ret.join(" | ");
}

QString LVGLPropertyFlags::codeValue(LVGLObject *obj) const
{
	QStringList ret;
	const int flags = get(obj);
	for (int r = 0; r < m_flagNames.size(); ++r) {
		if (flags & m_flags.at(r))
			ret << m_flagCodeName.at(r);
	}
	return ret.join(" | ");
}

void LVGLPropertyFlags::setValue(LVGLObject *obj, QVariant value)
{
	if (value.type() == QVariant::String) {
		QStringList flags = value.toString().split(" | ");
		int mask = 0;
		auto it = flags.begin();
		for (int i = 0; it != flags.end(); ++i, ++it) {
			int idx = m_flagNames.indexOf(*it);
			if (idx >= 0)
				mask |= m_flags.at(idx);
		}
		set(obj, mask);
	}
}
