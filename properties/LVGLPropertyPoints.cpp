#include "LVGLPropertyPoints.h"

#include <QTreeWidget>
#include <QDialogButtonBox>
#include <QToolButton>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

#include "LVGLCore.h"

LVGLPropertyPointsDialog::LVGLPropertyPointsDialog(QWidget *parent)
	: QDialog(parent)
{
	m_list = new QTreeWidget(this);
	m_list->setColumnCount(2);
	m_list->setHeaderLabels(QStringList() << "x" << "y");
	m_list->setItemDelegate(new LVGLPropertyPointsDelegate);

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
			QTreeWidgetItem *item = new QTreeWidgetItem;
			item->setText(0, "0");
			item->setText(1, "0");
			item->setFlags(item->flags() | Qt::ItemIsEditable);
			m_list->addTopLevelItem(item);
		} else if (b == rem) {
			int index = m_list->indexOfTopLevelItem(m_list->currentItem());
			m_list->takeTopLevelItem(index);
		}
	});

	QVBoxLayout *layout1 = new QVBoxLayout;
	layout1->addWidget(m_list);
	layout1->addWidget(box);

	setLayout(layout1);
}

QWidget *LVGLPropertyPointsDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option)
	if (index.isValid()) {
		QSpinBox *widget = new QSpinBox(parent);
		widget->setRange(0, index.column() == 0 ? lvgl.width() : lvgl.height());
		return widget;
	}
	return nullptr;
}

void LVGLPropertyPointsDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	if (index.isValid()) {
		QSpinBox *widget = qobject_cast<QSpinBox*>(editor);
		widget->setValue(index.data().toInt());
	}
}

void LVGLPropertyPointsDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	if (index.isValid()) {
		QSpinBox *widget = qobject_cast<QSpinBox*>(editor);
		model->setData(index, widget->value());
	}
}

void LVGLPropertyPointsDialog::setPoints(QVector<lv_point_t> points)
{
	for (const lv_point_t &p:points) {
		QTreeWidgetItem *item = new QTreeWidgetItem;
		item->setText(0, QString::number(p.x));
		item->setText(1, QString::number(p.y));
		item->setFlags(item->flags() | Qt::ItemIsEditable);
		m_list->addTopLevelItem(item);
	}
}

QVector<lv_point_t> LVGLPropertyPointsDialog::points() const
{
	QVector<lv_point_t> ret;
	for (int i = 0; i < m_list->topLevelItemCount(); ++i) {
		QTreeWidgetItem *item = m_list->topLevelItem(i);
		ret << lv_point_t{static_cast<lv_coord_t>(item->text(0).toInt()),
								static_cast<lv_coord_t>(item->text(1).toInt())
								};
	}
	return ret;
}

LVGLPropertyPoints::LVGLPropertyPoints(LVGLProperty *parent)
	: LVGLProperty(parent)
{
}

bool LVGLPropertyPoints::hasEditor() const
{
	return true;
}

QWidget *LVGLPropertyPoints::editor(QWidget *parent)
{
	m_widget = new LVGLPropertyPointsDialog(parent);
	return m_widget;
}

void LVGLPropertyPoints::updateEditor(LVGLObject *obj)
{
	m_widget->setPoints(get(obj));
}

void LVGLPropertyPoints::updateWidget(LVGLObject *obj)
{
	set(obj, m_widget->points());
}

QVariant LVGLPropertyPoints::value(LVGLObject *obj) const
{
	QStringList ret;
	QVector<lv_point_t> list = get(obj);
	for (const lv_point_t &p:list)
		ret << QString("(%1,%2)").arg(p.x).arg(p.y);
	return ret.join(", ");
}

void LVGLPropertyPoints::setValue(LVGLObject *obj, QVariant value)
{
	if (value.type() == QVariant::List) {
		QVariantList list = value.toList();
		QVector<lv_point_t> points;
		for (const QVariant &p:list) {
			QVariantMap map = p.toMap();
			points << lv_point_t{static_cast<lv_coord_t>(map["x"].toInt()),
										static_cast<lv_coord_t>(map["y"].toInt())};
		}
		set(obj, points);
	}
}

QJsonValue LVGLPropertyPoints::toJson(LVGLObject *obj) const
{
	QJsonArray ret;
	for (const lv_point_t &p:get(obj)) {
		ret << QJsonObject({{"x", p.x},
								  {"y", p.y}});
	}
	return ret;
}
