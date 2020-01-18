#include "LVGLPropertyTextList.h"

#include <QListWidget>
#include <QToolButton>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QDialog>
#include <QJsonArray>

class LVGLPropertyTextListDialog : public QDialog
{
public:
	inline LVGLPropertyTextListDialog(QWidget *parent = nullptr) : QDialog(parent)
	{
		m_list = new QListWidget(this);

		QDialogButtonBox *box = new QDialogButtonBox(QDialogButtonBox::Ok |
																	QDialogButtonBox::Cancel);
		QToolButton *add = new QToolButton();
		add->setIcon(QIcon(":/icons/add.png"));
		add->setIconSize(QSize(24, 24));
		QToolButton *rem = new QToolButton();
		rem->setIcon(QIcon(":/icons/delete.png"));
		rem->setIconSize(QSize(24, 24));

		box->addButton(add, QDialogButtonBox::ApplyRole);
		box->addButton(rem, QDialogButtonBox::ApplyRole);

		connect(box, &QDialogButtonBox::accepted, this, &QDialog::accept);
		connect(box, &QDialogButtonBox::rejected, this, &QDialog::reject);
		connect(box, &QDialogButtonBox::clicked, [this,add,rem](QAbstractButton *b) {
			if (b == add) {
				QListWidgetItem *item = new QListWidgetItem("New text");
				item->setFlags(item->flags() | Qt::ItemIsEditable);
				m_list->addItem(item);
			} else if (b == rem) {
				m_list->takeItem(m_list->currentRow());
			}
		});

		QVBoxLayout *layout1 = new QVBoxLayout;
		layout1->addWidget(m_list);
		layout1->addWidget(box);

		setLayout(layout1);
	}

	inline void setTextList(QStringList list)
	{
		m_list->clear();
		for (const QString &s:list) {
			QListWidgetItem *item = new QListWidgetItem(s);
			item->setFlags(item->flags() | Qt::ItemIsEditable);
			m_list->addItem(item);
		}
	}

	inline QStringList textList() const {
		QStringList ret;
		for (int i = 0; i < m_list->count(); ++i)
			ret << m_list->item(i)->text();
		return ret;
	}

private:
	class QListWidget *m_list;

};

LVGLPropertyTextList::LVGLPropertyTextList(LVGLProperty *parent)
	: LVGLProperty(parent)
{
}

bool LVGLPropertyTextList::hasEditor() const
{
	return true;
}

QWidget *LVGLPropertyTextList::editor(QWidget *parent)
{
	m_widget = new LVGLPropertyTextListDialog(parent);
	return m_widget;
}

void LVGLPropertyTextList::updateEditor(LVGLObject *obj)
{
	m_widget->setTextList(get(obj));
}

void LVGLPropertyTextList::updateWidget(LVGLObject *obj)
{
	if (m_widget->result() == QDialog::Accepted)
		set(obj, m_widget->textList());
}

QVariant LVGLPropertyTextList::value(LVGLObject *obj) const
{
	const QStringList items = get(obj);
	return items.isEmpty() ? "Empty list" : items.join(", ");
}

void LVGLPropertyTextList::setValue(LVGLObject *obj, QVariant value)
{
	if (value.type() == QVariant::List) {
		QVariantList list = value.toList();
		QStringList items;
		for (const QVariant &p:list) {
			if (p.type() == QVariant::String)
				items << p.toString();
		}
		set(obj, items);
	}
}

QJsonValue LVGLPropertyTextList::toJson(LVGLObject *obj) const
{
	const QStringList items = get(obj);
	QJsonArray val;
	for (const QString &i:items)
		val.append(i);
	return val;
}
